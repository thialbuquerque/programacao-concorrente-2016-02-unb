#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 100 //Canibais

int food = 10;		//Quantity of food available
int portions = 0;	//Quantity of food cooked

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_cooker = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cannibal = PTHREAD_COND_INITIALIZER;

void * cooker (void * arg); 		//Function for the cooker
void * cannibals (void * arg);		//Function for the cannibals


int main (int argc, char * argv[])
{
	pthread_t 	cookerThread,		//Thread for the cooker
				cannibalThread[N];	//Thread for the cannibals
	int i = 0, 						//Counter
		* id = NULL;				//Pointer which stores the id for each cannibals' thread
	
	srand (time(NULL));
	
	for (i = 0; i < N; i++)														
	{
		id = (int *) malloc (sizeof (int));										
		*id = i;																
		
		if (pthread_create (&cannibalThread[i], NULL, cannibals, (void *) id))	
		{
			printf ("Failed to create thread cannibalThread number %d\n", i);	
			return -1;
		}
		
	}
	
	if (pthread_create (&cookerThread, NULL, cooker, (void *) id))
	{
		printf ("Fail at creating cookerThread\n");
		return -1;
	}
	
	if (pthread_join (cannibalThread[0], NULL))
	{
		printf ("Could not do the join\n");
		return -1;
	}
	
	return 0;	
}

void * cooker (void * arg)
{
	int i = *((int *)arg);
	
	while (1)
	{
		pthread_mutex_lock (&lock);
		
		while (food > 0)
			pthread_cond_wait (&cond_cooker, &lock);
		
		portions = rand()%97 + 3;	
		printf ("Cooker is cooking %d portions.\n", portions);
		sleep (5);
		food = portions;
		printf ("Cooker has already cooked %d portions.\nCooker is sleeping.\n", portions);
		pthread_cond_broadcast (&cond_cannibal);
		pthread_mutex_unlock (&lock);
	}
	
	pthread_exit (0);
}

void * cannibals (void * arg)
{
	int i = *((int *) arg);
	
	while (1)
	{
		pthread_mutex_lock (&lock);
		
		while (food == 0)
		{
			printf ("Cannibal %d is waiting for food.\n", i);
			pthread_cond_wait (&cond_cannibal, &lock);
		}
		food--;
		printf ("Cannibal %d is eating. Food available: %d\n", i, food);
		
		if (food == 0)
		{
			printf ("Cooker waken up by cannibal %d!\n", i);
			pthread_cond_signal (&cond_cooker);
		}
		pthread_mutex_unlock (&lock);	
	}
	
	pthread_exit (0);
}

