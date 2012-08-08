// This is the revised version of the assignment - please read the 3-page
// text.
//
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

int shmid = 31100 ;
int shmidg = 31100 ;
int semid = 31100 ;
key_t key ;

struct login {
	int pid ;
	// more stuff ;
} ;
struct login *login ;
struct bet {
	short mult ;
	short number[6] ;
} ;
struct bet *winningbet ;
int LoginEmpty , LoginFull , Winningbet , NextbetEmpty , NextbetFull ;
struct global {
	long round ;
} ;
struct global *global ;
struct nextbet {
	int pid ;
	int round ;
	struct bet bet ;
} ;
struct nextbet *nextbet ;
int numplayers = 0 ;
struct play {
	int bets ;
	int loss ;
} player[20] ;
#define MAXROUNDS 20

void SVC( int code , char *string ) ;

int main( )
{
int round ;
void finalreport() ;
void initglobal() , initlottery() , oneround() ; //to be written
union semun {
	int val ;
	struct semid_ds *buf ;
	unsigned short *array ;
} dummy ;
int Vc , Pc , Bc ;
/*	sethandler( SIGPROF , prof ) ; */
	if( freopen( "lotto.log" , "w" , stdout ) == NULL ) {
		perror( "lotto cannot open log" ) ;
		exit( -1 ) ;
	}
	Pc = Vc = Bc = 0 ;
	initglobal() ;

	initlottery() ;
	for( round = 0 ; round < MAXROUNDS; round++ )
		oneround() ;
	finalreport() ;
	sleep( 5 ) ;	/* To give players a chance to see it is over */
	SVC( shmctl( shmid , IPC_RMID , (struct shmid_ds *)0 ) , "Shm rm fails" ) ;
	SVC( shmctl( shmidg , IPC_RMID , (struct shmid_ds *)0 ) , "Shm rm fails" ) ;
	SVC( semctl( semid , 0 , IPC_RMID , dummy ) , "Sem rm fails" ) ;
	printf( "Pc=%ld Vc=%ld Bc=%ld\n" , Pc , Vc , Bc ) ;
	exit( 1 ) ;
}

void finalreport()
{
int i , j ;
struct bet dummy ;
	for( i = 0 ; i < numplayers ; i++ ) {
		dummy.mult = player[i].loss ;
		dummy.number[0] = player[i].bets ;
		for( j = 1 ; j < 6 ; j++ )
			dummy.number[j] = 0 ;
	}
}

void SVC( code , string )
int code ;
char *string ;
{
	if( code < 0 ) {
		perror( string ) ;
		exit( -1 ) ;
	}
	return ;
}

void initglobal()
{
/* semaphores and memory */
	key = 31100 ;
	semid = semget( (key_t)key , 5 , 0600 | IPC_CREAT ) ;
	if( semid < 0 ) perror( "could not create sem" ) ;
	shmid = shmget( (key_t)key
		, sizeof(struct login)
		+ sizeof(struct bet) + sizeof(struct nextbet)
		, IPC_CREAT | 0600 ) ;
	if( shmid < 0 ) perror( "could not create shared mem" ) ;
	if( semid < 0 || shmid < 0 )
		exit( -1 ) ;
	login =  (struct login*) shmat( shmid , (char *)0 , 0 ) ;
	if( login == (struct login *)(-1) ) {
		perror( "Login not mapped" ) ;
		exit( -2 ) ;
	}
	winningbet = (struct bet *) ((char *)login + sizeof(struct login) ) ;
	nextbet = (struct nextbet *) ((char *)winningbet + sizeof(struct bet) ) ;

	shmidg = shmget( (key_t)(key+200) , sizeof(global) , 0600 | IPC_CREAT ) ;
	if( shmidg < 0 ) perror( "could not access global" ) ;
	global = (struct global *)shmat( shmidg , (char *)0 , 0 ) ;
	if( global == (struct gloabal *)(-1) ) {
		perror( "Global not mapped" ) ;
		exit( -2 ) ;
	}

}
// Part of the code of Player
//
struct global *global ;
int pid ;
key_t key = 31100 ;
struct nextbet  localbet ;
void sowbet( struct bet * ) ;

int betonce( int mult )
{
int ret ;
	pid = getpid() ;
	pickbet( localbet.bet.number ) ;
	localbet.pid = pid ;
	localbet.bet.mult = mult ;
	for( ; ; usleep( 1000 ) ) {
		P( Winningbet ) ;
		if( ret = (global->round != -1) ) {
			if( B( NextbetEmpty ) ) {	/* Avoid a deadlock */
				V( Winningbet ) ;
				continue ;
			}
			P( NextbetEmpty ) ;
			printf( "Player %d bets: " , pid ) ;
			*nextbet = localbet ;
			localbet.round = nextbet->round = global->round ;
			showbet( &(nextbet->bet) ) ;
			V( NextbetFull ) ;
		}
		else
			printf( "[%ld] asks: Lottery out of business?\n" , pid ) ;
		V( Winningbet ) ;
		return ret ;
	}
}

void init()
{
int ret ;
/* semaphores and memory */
	semid = semget( key , 5 , IPC_EXCL | 0600 ) ;
	if( semid < 0 ) perror( "could not access sem" ) ;
	ret = shmget( (key_t)key
		, sizeof(struct login)
		+ sizeof(struct bet)
		+ sizeof(struct nextbet)
		, IPC_EXCL | 0600 ) ;
	if( ret < 0 ) perror( "could not access shared mem" ) ;
	if( semid < 0 || ret < 0 )
		exit( -1 ) ;
	login =  (struct login*) shmat( ret , (char *)0 , 0 ) ;
	if( login < 0 ) {
		perror( "Login not mapped" ) ;
		exit( -1 ) ;
	}
	winningbet = (struct bet *) ((char *)login + sizeof(struct login) ) ;
	nextbet = (struct nextbet *) ((char *)winningbet + sizeof(struct bet) ) ;

	ret = shmget( (key_t)(key+200) , sizeof(global) , IPC_EXCL | 0600 ) ;
	if( ret < 0 ) perror( "could not access global" ) ;
	global = (struct global *)shmat( ret , (char *)0 , 0 ) ;
	if( global < 0 ) {
		perror( "Global not mapped" ) ;
		exit( -1 ) ;
	}
}
