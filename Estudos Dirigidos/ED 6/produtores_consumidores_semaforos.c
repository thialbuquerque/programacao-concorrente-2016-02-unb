/*
*Descricao: Algoritmo que simula interacoes entre produtores e consumidores
*Autor: Thiago de Oliveira Albuquerque
*Matricula: 14/0177442
*Disciplina: Topicos Avancados em Computadores - Programacao Concorrente
*Turma: C
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define BUFFER 20
#define CONSUMER 7
#define PRODUCER 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t freePos,
	  filledPos;		

int prod_pos = 0,
    cons_pos = 0,
    stock = 0,
    buffer[BUFFER];

void * producer (void * arg);
void * consumer (void * arg);

int main (int argc, char * argv[])
{
	pthread_t producerThread[PRODUCER],
			  consumerThread[CONSUMER];
	int i = 0,
	    * idC = NULL,
	    * idP = NULL;
	
	srand (time(NULL));
	
	sem_init (&freePos, 0, BUFFER);
	sem_init (&filledPos, 0, 0);
	
	for (i = 0; i < PRODUCER; i++)
	{
		idP = (int *) malloc (sizeof (int));
		*idP = i;
		
		if (pthread_create (&producerThread[i], NULL, producer, (void *) idP))
		{
			printf ("Falha ao criar producerThread numero %d!!\n", i);
			return -1;
		}
	}
	
	for (i = 0; i < CONSUMER; i++)
	{
		idC = (int *) malloc (sizeof (int));
		*idC = i;
		
		if (pthread_create (&consumerThread[i], NULL, consumer, (void *) idC))
		{
			printf ("Falha ao criar consumerThread numero %d!!\n", i);
			return -1;
		}	
	}
	
	for (i = 0; i < PRODUCER; i++)
	{
		if (pthread_join (producerThread[i], NULL))
			return -1;
	}	
	
	for (i = 0; i < CONSUMER; i++)
	{
		if (pthread_join (consumerThread[i], NULL))
			return -1;
	}	
	return 0;
}

void * producer (void * arg)
{
	int i = *((int *) arg);
	int dataProd;
	
	while (1)
	{
		printf ("Produtor %d: vou produzir um item\n", i);
		sleep (2);
		
		sem_wait (&freePos);	//DOWN
		pthread_mutex_lock (&mutex);
		
		dataProd = rand()%1001;
		
		printf ("Produtor %d: Produzindo item...\n", i);
		sleep (2);
		printf ("Produtor %d: vou inserir item %d na posicao %d\n", i, dataProd, prod_pos);
		buffer[prod_pos] = dataProd;
		prod_pos = (prod_pos + 1)%BUFFER;
		stock++;
		printf ("Dados em estoque: %d\n\n", stock);
	
		pthread_mutex_unlock (&mutex);
		sem_post (&filledPos);	//UP
	}
	
	pthread_exit (0);
}

void * consumer (void * arg)
{
	int i = *((int *) arg);
	int removed = 0;
	
	while (1)
	{
		sleep (2);
		
		sem_wait (&filledPos);	//DOWN
		pthread_mutex_lock (&mutex);

		if (stock == 0)
			printf ("\tConsumidor %d:\n\tVou esperar por itens\n", i);
			
		removed = buffer[cons_pos];
		printf ("\tConsumidor %d:\n\tDado removido da posicao %d: %d\n", i, cons_pos, removed);
		cons_pos = (cons_pos + 1)%BUFFER;
		stock--;
		printf ("\tDados em estoque: %d\n\n", stock);
		
		pthread_mutex_unlock (&mutex);
		
		printf ("\tConsumidor %d:\n\tConsumindo dado: %d\n", i, removed);
		sem_post (&freePos);	//UP
		
		sleep (20);	
	}
	
	pthread_exit (0);
}


