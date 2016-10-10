#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define A 10 //Quantidade de macacos no morro A
#define B 10 //Quantidade de macacos no morro B

/*
Tem-se dois morros, e uma corda liga esses dois morros.
Há macacos que querem ir do morro A para o B e do morro B para o A. A corda é resistente e suporta n macacos.
Mas se há macacos indo de A para B, não pode haver macacos indo de B para A.
Lembre-se, podemos ter n quantidades de macacos indo de A para B ou de B para A, 
desde que quando houverem macacos indo de A para B não se tenham macacos indo de B para A.
*/
pthread_mutex_t lockFlow;	//Mutex object to lock the rope thread
pthread_mutex_t lockHillA;	//Mutex objetc to lock the monkeys from hill A thread
pthread_mutex_t lockHillB;	//Mutex object to lock the monkeys from hill B thread
pthread_mutex_t lockTurn;	//Mutex object to lock the turn thread
	
void * flowAtoB (void * arg);
void * flowBtoA (void * arg);
	
int monAtoB = 0,	//número de macacos do morro A indo pro B
	monBtoA = 0;	//número de macacos do morro B indo pro A
	
int main (int argc, char * argv[])
{
	pthread_t monkeysAB[A + B]; 
	int * id = NULL,
		i = 0;
		
	pthread_mutex_init (&lockFlow, NULL);
	pthread_mutex_init (&lockHillA, NULL);
	pthread_mutex_init (&lockHillB, NULL);
	pthread_mutex_init (&lockTurn, NULL);	

	for (i = 0; i < (A + B); i++)
	{
		id = (int *) malloc (sizeof(int));
		*id = i;
		
		if (i % 2 == 0)	
		{
			if (pthread_create (&monkeysAB[i], NULL, flowAtoB, (void *) id))
			{
				printf ("Falha na criacao da thread %d\n", i);
				return - 1;
			}
		}
		else
		{
			if (pthread_create (&monkeysAB[i], NULL, flowBtoA, (void *) id))
			{
				printf ("Falha na criacao da thread %d\n", i);
				return - 1;
			}
		}
		id++;
	}
	for (i = 0; i < (A + B); i++)
	{
		if (pthread_join (monkeysAB[i], NULL))
		{
			printf ("Falha na uniao das threads\n");
			return -1;
		}
	}
	return 0;
}

void * flowAtoB (void * arg)
{
	int i = *((int *) arg);
	
	while (1)
	{
		pthread_mutex_lock (&lockTurn);		
		pthread_mutex_lock (&lockHillA);	
		monAtoB++;			
		if (monAtoB == 1)
			pthread_mutex_lock (&lockFlow);
		pthread_mutex_unlock (&lockHillA);
		pthread_mutex_unlock (&lockTurn);
		
		printf ("\nMacaco %d do morro A estah atravessando para o morro B\n", i);
		sleep (2);
		
		pthread_mutex_lock (&lockHillA);
		monAtoB--;
		printf ("\nMacaco %d do morro A terminou de atravessar para o morro B\nMacacos na corda: %d\n", i, monAtoB);						
		if (monAtoB == 0)
			pthread_mutex_unlock (&lockFlow);
		pthread_mutex_unlock (&lockHillA);	
	}
	pthread_exit (0);	
}

void * flowBtoA (void * arg)
{
	int i = *((int *) arg);
	
	while (1)
	{
		pthread_mutex_lock (&lockTurn);		
		pthread_mutex_lock (&lockHillB);	
		monBtoA++;			
		if (monBtoA == 1)
			pthread_mutex_lock (&lockFlow);
		pthread_mutex_unlock (&lockHillB);
		pthread_mutex_unlock (&lockTurn);
		
		printf ("\nMacaco %d do morro B estah atravessando para o morro A\n", i);
		sleep (2);
		
		pthread_mutex_lock (&lockHillB);
		monBtoA--;
		printf ("\nMacaco %d do morro B terminou de atravessar para o morro A\nMacacos na corda: %d\n", i, monBtoA);						
		if (monBtoA == 0)
			pthread_mutex_unlock (&lockFlow);
		pthread_mutex_unlock (&lockHillB);	
	}
	pthread_exit (0);
}
