/*
*Algoritmo: Problema do Pombo
*Descricao: Algoritmo que simula o envio de cartas de uma cidade à outra por intermédio de um pombo-correio
*Autor: Thiago de Oliveira Albuquerque
*Matricula: 14/0177442
*Disciplina: Topicos Avancados em Computadores - Programacao Concorrente
*Turma: C
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

void * writer (void * id);
void * dove (void * id);


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t 	fullOfLetters,
		emptyOfLetters;
const int 	LETTERS = 30,
			WRITERS = 50;
int lettersIn = 0; 

int main (int argc, char * argv[])
{
	pthread_t 	doveThread,
				writerThread[WRITERS];
	int * idW = NULL,
		* idD = NULL,
		i = 0;
	
	sem_init (&fullOfLetters, 0, 0);
	sem_init (&emptyOfLetters, 0, LETTERS);
	
	srand (time(NULL));
	
	for (i = 0; i < WRITERS; i++)
	{
		idW = (int *) malloc (sizeof(int));
		*idW = i;
		if (pthread_create (&writerThread[i], NULL, writer, (void *) idW))
		{
			//printf ("Falha ao criar thread para os escritores!\n");
			return -1;
		}
	}
	
	if (pthread_create (&doveThread, NULL, dove, NULL))
	{
		//printf ("Falha ao criar thread para o pombo!\n");
		return -1;
	}
	
	for (i = 0; i < WRITERS; i++)
	{
		if (pthread_join (writerThread[i], NULL))
		{
			//printf ("Falha ao realizar join!\n");
			return -1;
		}
	}
	
	pthread_join (doveThread, NULL);
	
	return 0;
}

void * writer (void * id)
{
	int i = *((int *) id) + 1;
	
	while (1)
	{
		sem_wait (&emptyOfLetters);
		
		printf ("Remetente %d: Vou escrever uma carta\n", i);
		sleep ((rand()%3 + 1));
		
		pthread_mutex_lock (&mutex);
		printf ("Remetente %d: Escrevendo...\n", i);
		sleep (1);
		printf ("Remetente %d: Carta escrita\n", i);
		
		lettersIn++;
		printf ("Cartas pendentes para envio: %d\n", lettersIn);

		pthread_mutex_unlock (&mutex);
		
		if (lettersIn == 30)
			sem_post (&fullOfLetters);
	}
	
	pthread_exit (0);
}

void * dove (void * id)
{
	int i = 0;
	while (1)
	{
		sem_wait (&fullOfLetters);
		
		printf ("\nPombo pronto para enviar cartas para a cidade B\n");
		
		pthread_mutex_lock (&mutex);
		
		printf ("Pombo iniciando viagem...\n");
		sleep (1);
		printf ("Pombo viajando...\n");
		sleep (5);
		printf ("Cartas entregues!\nPombo preparando para retornar\n");
		sleep (1);
		lettersIn = 0;
		printf ("Pombo retornando a cidade A...\n");
		sleep (5);
		
		pthread_mutex_unlock (&mutex);
		
		for (i = 0; i < LETTERS; i++)
			sem_post (&emptyOfLetters);
	}
	
	pthread_exit (0);
}
