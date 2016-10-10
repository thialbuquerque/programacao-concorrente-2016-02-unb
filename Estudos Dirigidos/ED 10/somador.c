/*
*Algorithm:			Somador dos elementos de uma matriz
*Description:		O objetivo é somar todos os elementos de uma matriz NxM particionando a atividade de somar cada linha entre threads
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
#include <unistd.h>
#include <time.h>
#include <pthread.h>

void * sum (void * arg);

pthread_barrier_t barrier;

int N = 0,
	M = 0;
float ** matrix = NULL,
	  * results = NULL,
	  answer = 0;	

int main (int argc, char * argv[])
{
	pthread_t * threadSum = NULL;
	int scan = 0,
		i = 0,
		j = 0, 
		* id = NULL;
	float value = 0;

	system ("clear || cls");
	printf ("Digite as dimensoes da matriz A(NxM)\nN = ");
	while (scan = scanf ("%d", &N) == 0)
		printf ("Digite somente valores inteiros\n");		
	printf ("M = ");
	while (scan = scanf ("%d", &M) == 0)
		printf ("Digite somente valores inteiros\n");			

	system ("clear || cls");
	
	pthread_barrier_init (&barrier, NULL, N);
	
	matrix = (float **) calloc (N, sizeof(float *));
	for (i = 0; i < N; i++)
	{
		*(matrix + i) = (float *) calloc (M, sizeof(float));
	}
	
	results = (float *) calloc (N, sizeof(float));
	threadSum = (pthread_t *) calloc (N, sizeof(pthread_t));	//One thread per line
	
	printf ("Digite os valores dos elementos da matriz A(%dx%d)\n", N, M);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf ("A[%d][%d] = ", i, j);
			while (scan = scanf ("%f", &value) == 0)
				printf ("Digite somente valores reais\nA[%d][%d] = ", i, j);

			matrix[i][j] = value;
		}
		putchar ('\n');
	}
			
	for (i = 0; i < N; i++)
	{
		id = (int *) malloc (sizeof(int));
		*id = i;
		
		pthread_create (threadSum + i, NULL, sum, (void *) id);
	}
	
	for (i = 0; i < N; i++)
	{
		pthread_join (*(threadSum + i), NULL);
	}

	return 0;
}

void * sum (void * arg)
{
	int i = 0,
		id = *((int * ) arg);
	float partial = 0;
	
	for (i = 0; i < M; i++)		//Each thread sums the elements of one line
		partial += matrix[id][i];	
	
	results[id] = partial;
	
	pthread_barrier_wait (&barrier);	/*Waits until all the threads had calculated the sum of the elements of each line separately
										  Holds until the number of thread on the barrier be N*/
	if (id == 0)	//There is at least one thread, that is, at least one line, 
	{
		for (i = 0; i < N; i++)
			answer += results[i];
		printf ("Resultado da soma dos elementos da matriz A(%dx%d): %.2f\n", N, M, answer);	
	}
	
	pthread_exit (NULL);
}
