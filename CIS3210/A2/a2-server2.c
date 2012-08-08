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
	short int PORT = 11708 ;

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

int handlerequest( int cs , struct sockaddr_in from )
{
#define MAX 20
#define BUFSIZE 520
	int stream[MAX] ;
    int offset[MAX] ;
    int chunkend[MAX] ;
    int index[MAX] ;
    int fd ;
    char buf[BUFSIZE] ;
    short *S ;
    int i , rc ;
    short N , P ;
    char file[512] ;
    char *FB ;
    int size ;
    socklen_t fromlen ;
    int ch , temp ;
    int mul = 10 ;
    int con , len ;
    int noff ;

    rc = recv( cs , buf , sizeof( buf ) - 1 , 0 );
    if( rc <= 0 ) {
        printf( "This connection is dead (%d)\n" , rc ) ;
        return 0 ;
    }
    S = (short *) buf ;
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
    size = lseek( fd , 0 , SEEK_END ) ; // Moves to the end
    lseek( fd , 0 , SEEK_SET ) ;    // Move back to start
    FB = malloc( size ) ;
    if( FB < 0 ) {
        perror( "Server: malloc" ) ;
        exit( -1 ) ;
    }
    rc = read( fd , FB , size ) ;
    if( rc != size ) {
        printf( "Could not read the whole file (%d %d)\n" , size , rc ) ;
        exit( -1 ) ;
    }
    printf( "file read. size = %d\n" , size ) ;
    offset[0] = 0 ;
    while( mul * BUFSIZE * (N-1) > size )
        mul-- ;
    for( i = 1 ; i < N ; i++ ) {
        offset[i] = offset[i-1] + mul*BUFSIZE ;
        chunkend[i-1] = offset[i] ;
        if( offset[i] > size ) offset[i] = size ;
    }
    chunkend[N-1] = size ;
    srand( getpid() ) ;
    for( i = 1 ; i < N ; i++ ) {
        ch = rand() % (N-1) ;
        temp = offset[ch+1] ;
        offset[ch+1] = offset[ch] ;
        offset[ch] = temp ;
        temp = chunkend[ch+1] ;
        chunkend[ch+1] = chunkend[ch] ;
        chunkend[ch] = temp ;
    }
    printf( "Offsets: " ) ;
    for( i = 0 ; i < N ; i++ ) printf( "(%d--%d)" , offset[i] , chunkend[i] ) ;
    printf( "\n" ) ;

    printf( "Server: connecting to client: %s\n" , inet_ntoa( from.sin_addr ) ) ;
	from.sin_family = AF_INET ;
    from.sin_port = htons( P ) ;
    from.sin_addr.s_addr = from.sin_addr.s_addr ;
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
        printf( "channel established with socket %d\n" , stream[i] ) ;
        noff = htonl( offset[i] ) ;
        send( stream[i] , (void *)&noff , sizeof( int ) , 0 ) ;
    }
    for( i = 0 ; i < N ; i++ )
        index[i] = offset[i] ;
    con = N ;
    while( con > 0 ) {
        i = rand() % N ;
        if( index[i] < chunkend[i] ) {
            len = index[i] + BUFSIZE < chunkend[i] ? BUFSIZE 
                : chunkend[i] - index[i] ;
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
    }
    return 0 ;
}

int main( int argc , char *argv[] )
{
	int ls , cs ;
    struct sockaddr_in from ;
    socklen_t fromlen ;

	ls = startserver( argc , argv ) ;
	while( (cs = accept( ls , (struct sockaddr *)&from  , &fromlen )) > 0 )
        handlerequest( cs , from ) ; 
	perror( "Accept" ) ;
}

