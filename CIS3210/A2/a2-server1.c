//
//      This is a preliminary version.
//      It appears to work correctly for correct requests
//
//      A much more complex version will be used for testing
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include<errno.h>

int errno ;

int startserver( int argc , char *argv[] )
{
	short int PORT = 11708 ;    // default port number

	struct sockaddr_in lsSA ;
	int ls ;

	if( argc > 1 )
		PORT = atoi( argv[1] ) ;

	printf( "Server will use port number %d\n" , PORT ) ;
	ls = socket( PF_INET , SOCK_STREAM , 0 ) ; // 0 = TCP
	if( ls == -1 ) {
		perror( "Socket" ) ;
		exit( -1 ) ;
	}

	bzero( &lsSA , sizeof lsSA ) ;
	lsSA.sin_family = AF_INET ;
	lsSA.sin_addr.s_addr = htonl( INADDR_ANY ) ;
	lsSA.sin_port = htons( PORT ) ;

	if( bind( ls , (void *) &lsSA , sizeof lsSA ) == -1 ) {
		perror( "Bind" ) ;
		exit( -1 ) ;
	}
	if( listen( ls , 5 ) < 0 ) {
		perror( "Listen" ) ;
		exit( -1 ) ;
	}
	return ls ;
}

int handlerequest( int cs )
{
// This will work for no more than 20 connections 
// and a document size not exceeding MAXPAGE
// BUFSIZE is just a number (use a different number in client
#define MAX 20
#define MAXPAGE 30000
#define BUFSIZE 520
	int stream[MAX] ;
    int offset[MAX] ;
    int index[MAX] ;
    int fd ;
    char buf[BUFSIZE] ;
    short *S ;
    int i , rc ;
    short N , P ;
    char file[512] ;
    S = (short *) buf ;
    char FB[MAXPAGE] ;
    int size ;
    struct sockaddr_in from ;
    socklen_t fromlen ;
    int clip ;

    fromlen = sizeof from ;
    bzero( &from , fromlen ) ;
    rc = recvfrom( cs , buf , sizeof( buf ) - 1 , 0
    , (struct sockaddr *)&from , &fromlen ) ;
    if( rc <= 0 ) {
        printf( "This connection is dead (%d)\n" , rc ) ;
        return 0 ;
    }
    clip = from.sin_addr.s_addr ;   // This is the client's IP
    N = ntohs( S[0] ) ;
    P = ntohs( S[1] ) ;
    printf( "Decoded parameters: N=%d port=%d\n" , N , P ) ;
    buf[rc] = '\0' ;
    strcpy( file , buf+4 ) ;
    printf( "File %s requested\n" , file ) ;
    fd = open( file , O_RDONLY ) ;
    if( fd < 0 ) {
        perror( "Could not open" ) ;
        printf( "For now the requested file must be in the current dir\n" ) ;
        exit( -1 ) ;
    }
    rc = read( fd , FB , MAXPAGE ) ;
    size = rc ;
    printf( "file read. size = %d\n" , size ) ;
 //     Crude way to slice the document into streams
    offset[0] = 0 ;
    int mul = 10 ;
    while( mul * BUFSIZE * (N-1) > size )
        mul-- ;
    for( i = 1 ; i < N ; i++ ) {
        offset[i] = offset[i-1] + mul*BUFSIZE ;
        if( offset[i] > size ) offset[i] = size ;
    }
    offset[N] = size ;
 //     offset[N] - serves as sentinel (stream N does not exist)
    printf( "Offsets:" ) ;
    for( i = 0 ; i <= N ; i++ ) printf( " %d" , offset[i] ) ;
    printf( "\n" ) ;

// Streams created here

	from.sin_family = AF_INET ;
    from.sin_port = htons( P ) ;
    from.sin_addr.s_addr = clip ;
    fromlen = sizeof from ;

    for( i = 0 ; i < N ; i++ ) {
        stream[i] = socket( PF_INET , SOCK_STREAM , 0 ) ; // 0 = TCP
        if( stream[i] == -1 ) {
            perror( "Socket" ) ;
            exit( -1 ) ;
        }
        if( connect( stream[i] , (struct sockaddr *) &from , fromlen ) == -1 ) {
            perror( "Server: Connect" ) ;
            exit( -1 ) ;
        }
        int noff ;
        noff = htonl( offset[i] ) ;
        send( stream[i] , (void *)&noff , sizeof( int ) , 0 ) ;
    }
// Streams OK. Offsets already sent.
//
// Now the document is sent in a polite way
    for( i = 0 ; i < N ; i++ )
        index[i] = offset[i] ;
    int con , len ;
    con = N ;
    while( con > 0 )
    for( i = 0 ; i < N ; i++ ) 
        if( index[i] < offset[i+1] ) {
            len = index[i] + BUFSIZE < offset[i+1] ? BUFSIZE 
                : offset[i+1] - index[i] ;
            rc = send( stream[i] , FB+index[i] , len , 0 ) ;
            printf( "Server sends %d (%d) bytes from byte %d on channel %d\n"
                , rc , len , index[i] , i ) ;
            index[i] += rc ;
        } else if( stream[i] > 0 ) { 
            printf( "Server closes stream %d\n" , i ) ;
            con-- ;
            close( stream[i] ) ;
            stream[i] = -1 ;
        }
    return 0 ;
}

int main( int argc , char *argv[] )
{
	int ls , cs ;

	ls = startserver( argc , argv ) ;
	while( (cs = accept( ls , NULL , NULL )) > 0 )
        handlerequest( cs ) ; 
	perror( "Accept" ) ;
}

