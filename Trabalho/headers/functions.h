/*CONSTANTS*/
#define CLIENTS 130		/*Clients that may go to the restaurant*/ 
#define CLIENTS_MAX 100 /*Maximum number of clients allowed in the restaurant*/
#define COOKER 10		/*Number of cookers*/
#define WAITER 10		/*Number of waiters*/ 
#define TABLES 20		/*Number of tables*/
#define CHAIRS 5		/*Number of chairs per table*/
#define BATHROOM 5		/*Maximum number of people allowed in the bathroom*/
#define LIM 49			/*Number of asterisks*/
#define TIME 240		/*Number of seconds allowed to the execution of the program*/ 

/*COLORS FOR PRINTF*/
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

/*GLOABAL VARIABES*/
int clientCounter = 0,		/*Counts the number of clients inside the restaurant*/
	output = 1,				/*Determines whether the outputs must be shown or not. 1 means yes, 0 means no*/
	finished = 0,			/*Determines if the restaurant is empty and all the clients that may appear were attended. 1 means yes, 0 means no*/
	firstID = 0;			/*Will store the value of the first thread that represents the first client arriving at the restaurant*/
float seconds = 0;    		/*Receives the number of seconds elapsed from a certain moment after the program started*/
char exitCode = 0,			/*Character typed by the user to exit the program*/
	 exitConfirmation = 0,
	 ** chairs = NULL; 		/*100 chairs at all, 20 tables and 5 chairs each*/	
time_t timeElapsed;    		/*Stores the value that represents the edge of major value of a certain interval of time, just as in [a,b]*/
sem_t 	hoster,				/*Used to limit the quantity of clients allowed to be in the restaurant*/
		bathroomFemale,		/*Used to limit how many woman can use the bathroom*/
		bathroomMale,		/*Used to limit how many man can use the bathroom*/
		cooker, 			/*Used to limit the access to one specified cooker*/
		waiter, 			/*Used to limit the access to one specified waiter*/
		serve,				/*Used to synchronize the cooker and the waiter*/
		eat;				/*Used to synchronize the waiter and the client*/
pthread_mutex_t clientCounterMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the variable clientCounter*/
				cashierMutex = PTHREAD_MUTEX_INITIALIZER,		/*Limits the access to the cashier*/
				queueAccessMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the queue of the cashier*/
				entranceDoorMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the entrance door before the hoster (avoids race conditions)*/
				exitDoorMutex = PTHREAD_MUTEX_INITIALIZER,		/*Limits the access to the exit door*/
				seatMutex = PTHREAD_MUTEX_INITIALIZER,			/*Avoids that the same seat is selected to more than one client*/
				exitMutex = PTHREAD_MUTEX_INITIALIZER,			/*Limits the access to the exitCode and finished variables*/
				confirmationMutex = PTHREAD_MUTEX_INITIALIZER,	/*Limits the access to the exitConfirmation variable*/
				outputMutex = PTHREAD_MUTEX_INITIALIZER;		/*Limits the access to the printfs*/
pthread_cond_t exitCond = PTHREAD_COND_INITIALIZER,				/*Condition variable which controls the way the program is exited*/
			   counterCond = PTHREAD_COND_INITIALIZER,			/*Condition variable which controls what happens when all clients are attended*/
			   confirmationCond = PTHREAD_COND_INITIALIZER;		/*Condition variable which controls the used decision to monitor the client flow or not*/
pthread_barrier_t barrier;										/*Used to control the execution of he program right after all the CLIENTS that may
																go to the restaurant are attended and gone*/

/*PROTOTYPES*/
void clrscr(void);				/*CLEARS THE SCREEN*/
void asterisk (void);			/*PRESENTS THE ASTERISKS IN A SPECIFIC FORM*/
void presentation (void);		/*CREATES THE LAYOUT OF THE CALCULATOR USING asterisks FUNCTION*/
void initializer (void);		/*INITIALIZES ALL SEMAPHORES*/
int searchSeat (int * table);	/*SEARCHES FOR AVAILABLE A SEAT TO ONE CLIENT*/
char getch (void);				/*READS A HIDDEN CHARACTER FROM THE TERMINAL AND WITHOUT WAITING FOR THE "ENTER"*/
void * cookers (void * arg);	/*ROTINE USED BY THE THREADS OF COOKERS*/
void * waiters (void * arg);	/*ROTINE USED BY THE THREADS OF WAITERS*/
void * clients (void * arg);	/*ROTINE USED BY THE THREADS OF CLIENTS*/
void * reader (void * arg);		/*WAITS FOR A HIDDEN INPUT FROM THE TERMINAL*/
void * timeCounter (void * arg);/*COUNTS THE TIME BY SECONDS SINCE THE EXCECUTION OF THE LINE 46 FROM restaurante.c*/

#include "../functions/functions.c"
