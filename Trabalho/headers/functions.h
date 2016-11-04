/*CONSTANTS*/
#define CLIENTS 55		/*Clients that may go to the restaurant*/ 
#define CLIENTS_MAX 100 /*Maximum number of clients that can be in the restaurant*/
#define COOKER 10		/*Number of cookers*/
#define WAITER 10		/*Number of waiters*/ 
#define TABLES 20		/*Number of tables*/
#define CHAIRS 5		/*Number of chairs*/
#define BATHROOM 5		/*Maximum number of people allowed in the bathroom*/
#define LIM 49			/*Number of asterisks*/
#define TIME 120		/*Number of seconds allowed to the execution of the program*/ 

/*COLORS FOR PRINTF*/
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

/*GLOABAL VARIABES*/
int clientCounter = 0,
	finished = 0,
	firstID = 0;	/*Will store the value of the first thread that represents the first client arriving at the restaurant*/
float seconds = 0;
char exitCode = 0,	/*Character typed by the user to exit the program*/
	 ** chairs = NULL; /*100 chairs at all, 20 tables and 5 chairs each*/	
time_t timeElapsed;
sem_t 	hoster,				/*Used to limit the quantity of clients allowed to be in the restaurant*/
		bathroomFemale,		/*Used to limit how many woman can use the bathroom*/
		bathroomMale,		/*Used to limit how many man can use the bathroom*/
		cooker, 			/*Used to limit the access to one specified cooker*/
		waiter, 			/*Used to limit the access to one specified waiter*/
		serve,				/*Used to sinchronize the cooker and the waiter*/
		eat;				/*Used to sincronize the waiter and the client*/
pthread_mutex_t clientCounterMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the variable clientCounter*/
				/*firstIDMutex = PTHREAD_MUTEX_INITIALIZER,		/*Limits the access to the variable firstID*/
				cashierMutex = PTHREAD_MUTEX_INITIALIZER,		/*Limits the access to the cashier*/
				queueAccessMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the queue of the cashier*/
				entranceDoorMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the entrance door before the hoster (avoids race conditions)*/
				exitDoorMutex = PTHREAD_MUTEX_INITIALIZER,		/*Limits the access to the exit door*/
				/*tableMutex = PTHREAD_MUTEX_INITIALIZER,			/*Limits the access to the plain of seats*/
				seatMutex = PTHREAD_MUTEX_INITIALIZER,			/*Avoids that the same seat is selected to more than one client*/
				exitMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t exitCond = PTHREAD_COND_INITIALIZER,
			   counterCond = PTHREAD_COND_INITIALIZER;
pthread_barrier_t barrier;

/*PROTOTYPES*/
void clrscr(void);				/*CLEARS THE SCREEN*/
void asterisk (void);			/*PRESENTS THE ASTERISKS IN A SPECIFIC FORM*/
void presentation (void);		/*CREATES THE LAYOUT OF THE CALCULATOR USING asterisks FUNCTION*/
void initializer (void);		/*INITIALIZES ALL SEMAPHORES*/
void print (char * string); 
int searchSeat (int * table);	/*SEARCHES FOR AVAILABLE A SEAT TO ONE CLIENT*/
char getch (void );
void * cookers (void * arg);	/*ROTINE USED BY THE THREADS OF COOKERS*/
void * waiters (void * arg);	/*ROTINE USED BY THE THREADS OF WAITERS*/
void * clients (void * arg);	/*ROTINE USED BY THE THREADS OF CLIENTS*/
void * reader (void * arg);
void * timeCounter (void *arg);

#include "../functions/functions.c"
