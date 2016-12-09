/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *Algorithm:			Simulador de Restaurante                                    *
 *File name:         restaurante.c                                               *
 *Description:		O programa visa implementar a ideia de um restaurante vegano* 
 *                   aplicando conceitos de programação concorrente              *
 *					para manter um certo nível de fidelidade à realidade.       *
 *Author: 			Thiago de Oliveira Albuquerque                              *
 *Discipline:		Tópicos Avançados em Computadores - Programação Concorrente *
 *Class: 			E                                                           *
 *Date: 				2016/02                                                     *
 *Registration:		14/0177442                                                  *
 *Institution: 		Universidade De Brasília (UnB)                              *
 *Academic level:	Graduação                                                   *
 *Course:			Engenharia de Computação                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include "headers/queue.h"
#include "headers/functions.h"

int main (int argc, char * argv[])
{
	pthread_t	clientsThread[CLIENTS],
				cookerThread[COOKER],
				waiterThread[WAITER], 
				readerThread,
				timeThread;
	int * clientsID = NULL,
		* cookerID = NULL,
		* waiterID = NULL,
		i = 0;
	time_t timer;

	time (&timer);
	srand (timer);
	timeElapsed = timer;

	initializer ();

	printf (RESET);
	presentation ();

	printf ("Here we have 20 tables, 5 chairs each!\n");
	printf("\nPlease, come in and have a good meal!\n");
	sleep (5);
	clrscr ();
	printf ("INFO: Get out of the monitoring by typing any character, but 'c'...\n");
	sleep (3);
	printf ("\nColor legend:\n");
	printf (RESET "Cooker - " GREEN "%c\n" RESET "Hoster - " YELLOW "%c\n" RESET "Waiter - " MAGENTA "%c\n" RESET "Client - " CYAN "%c\n" RESET, 35, 35, 35, 35);
	sleep (5);
	clrscr ();

	cashierQueue = (QUEUE **) malloc (sizeof(QUEUE *));
	*cashierQueue = create_queue ();
	
	chairs = (char **) malloc (TABLES * sizeof(char *));
	for (i = 0; i < TABLES; i++)
		*(chairs + i) = (char *) malloc (CHAIRS * sizeof(char));
		
	for (i = 0; i < TABLES; i++)
		memset (*(chairs + i), '0', CHAIRS);

	/*CREATING THREADS*/
	pthread_create (&readerThread, NULL, reader, NULL);

	for (i = 0; i < COOKER; i++)	
	{
		cookerID = (int *) malloc (sizeof(int));
		*cookerID = i;

		if (pthread_create (&cookerThread[i], NULL, cookers, (void *) cookerID))	/*Creation of new thread failed*/
			return -1;	
	}

	for (i = 0; i < WAITER; i++)	
	{
		waiterID = (int *) malloc (sizeof(int));
		*waiterID = i;

		if (pthread_create (&waiterThread[i], NULL, waiters, (void *) waiterID))	/*Creation of new thread failed*/
			return -1;	
	}

	pthread_create (&timeThread, NULL, timeCounter, NULL);
	for (i = 0; i < CLIENTS; i++)
	{
		clientsID = (int *) malloc (sizeof(int));
		*clientsID = i;

		if (pthread_create (&clientsThread[i], NULL, clients, (void *) clientsID))	/*Creation of new thread failed*/
			return -1;
	}

	pthread_mutex_lock (&confirmationMutex);
	do
	{
		pthread_cond_wait (&exitCond, &confirmationMutex);
		
		pthread_mutex_lock (&outputMutex);
		output = 0;
		pthread_mutex_unlock (&outputMutex);

        clrscr ();
        if (seconds != TIME && finished != 1)
       	{
       		printf ("Do you want to keep monitoring the client flow in the restaurant?\nOption (Y/N): ");
	    
		    do
		    {
		       	scanf (" %c", &exitConfirmation);
		       	exitConfirmation = toupper (exitConfirmation);
		       	if (exitConfirmation != 'Y' && exitConfirmation != 'N')
		       	{	
		       		clrscr ();
		       		printf ("Wrong input!! Try again!!\nOption (Y/N): ");
		       	}
		    } while (exitConfirmation != 'Y' && exitConfirmation != 'N');

		    clrscr ();
		    if (exitConfirmation == 'Y')
		    {
		    	pthread_mutex_lock (&outputMutex);
				output = 1;
				pthread_mutex_unlock (&outputMutex);
				exitConfirmation = 0;
				pthread_cond_signal (&confirmationCond); /*After reading the exit confirmation code, the thread "reader" will verify its value.*/
		    }
		    else
		    	return 0; /*break?*/
      	}
      	else
      		break;
    
	} while (exitConfirmation != 'N'); /*If false: The user does not want to keep monitoring the flow in the restaurant.*/
	pthread_mutex_unlock (&confirmationMutex);    
	
	pthread_mutex_lock (&outputMutex);
	clrscr ();
	printf (RESET);
	printf ("\n*Restaurant is closing!!*\n");
	pthread_mutex_unlock (&outputMutex);
	sleep (3);
	
	pthread_mutex_lock (&exitMutex);
	if (clientCounter > 0)
	{
		pthread_mutex_lock (&outputMutex);
		printf ("*WAITING FOR THE REMAINING CLIENTS!*\n");
		sleep (2);
		clrscr ();
		output = 1;
		pthread_mutex_unlock (&outputMutex);
	
		pthread_cond_wait (&counterCond, &exitMutex);
		clrscr ();
		printf ("\n*Restaurant is empty!!*\n");
		sleep (2);
	}
	pthread_mutex_unlock (&exitMutex);

	clrscr ();
	system ("stty echo");

	/*JOINNINGS*/
	/*if (finished == 1)
	{	
		for (i = 0; i < COOKER; i++)
		{
			if (pthread_join (cookerThread[i], NULL))
				return -1;
		}

		for (i = 0; i < WAITER; i++)
		{
			if (pthread_join (waiterThread[i], NULL))
				return -1;
		}
	}
	
	for (i = 0; i < CLIENTS; i++)
	{
		if (pthread_join (clientsThread[i], NULL))
			return -1;
	}*/
	
	return 0;
}

