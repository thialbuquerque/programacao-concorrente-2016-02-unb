/*
*Algorithm:			Simulador de Restaurante
*Description:		O programa visa implementar a ideia de um restaurante vegano aplicando conceitos de programação concorrente 
					para manter um certo nível de fidelidade à realidade.
*Author: 			Thiago de Oliveira Albuquerque
*Discipline:		Tópicos Avançados em Computadores - Programação Concorrente
*Class: 			E
*Date: 				2016/02
*Registration:		14/0177442
*Institution: 		Universidade De Brasília (UnB)
*Academic level:	Graduação
*Course:			Engenharia de Computação
*
**/

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


#ifndef CLIENTS
	#define CLIENTS 55
#endif
#ifndef CLIENTS_MAX
	#define CLIENTS_MAX 100
#endif
#ifndef COOKER
	#define COOKER 10
#endif
#ifndef WAITER
	#define WAITER 10 
#endif
#ifndef TABLES
	#define TABLES 20 
#endif
#ifndef CHAIRS
	#define CHAIRS 5 
#endif



int main (int argc, char * argv[])
{
	/*system ("stty -echo");*/
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

	/*queue (cashierQueue, 1);
	queue (cashierQueue, 3);
	queue (cashierQueue, 2);
	consultQueue (*cashierQueue);
	dequeue (cashierQueue);
	consultQueue (*cashierQueue);
	dequeue (cashierQueue);
	consultQueue (*cashierQueue);
	dequeue (cashierQueue);
	consultQueue (*cashierQueue);
	queue (cashierQueue, 3);
	queue (cashierQueue, 2);
	consultQueue (*cashierQueue);
	getchar ();*/

	printf (RESET);
	presentation ();

	printf ("Here we have 20 tables, 5 chairs each!\n");
	printf("\nPlease, come in and have a good meal!\n");
	sleep (5);
	clrscr ();
	printf ("INFO: Get out of the monitoring by typing any character, but 'c'...\n");
	sleep (3);
	printf ("Color legend:\n");
	printf (RESET "Cooker - " GREEN "%c\n" RESET "Hoster - " YELLOW "%c\n" RESET "Waiter - " MAGENTA "%c\n" RESET "Client - " CYAN "%c\n" RESET, 127, 127, 127, 127);
	sleep (2);
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

	pthread_mutex_lock (&exitMutex);
	if (exitCode == 0)
	{	
		pthread_cond_wait (&exitCond, &exitMutex);
		if (clientCounter > 0)
		{
			printf ("*WAITING FOR THE REMAINING CLIENTS!*\n");
			pthread_cond_wait (&counterCond, &exitMutex);
		}
		clrscr ();
		printf (RESET);
		printf ("\n*Restaurant is closing!!*\n");
		sleep (3);
		clrscr ();
		system ("stty echo");
		/*if (seconds != TIME)	
		{
			if (pthread_join (readerThread, NULL))
				return -1;
		}
		else	
		{
			if (pthread_join (timeThread, NULL))
				return -1;
		}*/
		//return 0;
	}
	pthread_mutex_unlock (&exitMutex);

		/*ESPERAR ATÉ QUE OS CLIENTES RESTANTES SAIAM DO RESTAURANTE CASO O TEMPO LIMITE SEJA ALCANÇADO
*/
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
	if (exitCode == 0)
	{
		for (i = 0; i < CLIENTS; i++)
		{
			if (pthread_join (clientsThread[i], NULL))
				return -1;
		}
	}*/
	return 0;
}

