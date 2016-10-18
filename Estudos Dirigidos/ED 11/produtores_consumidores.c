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

#define BUFFER 20
#define CONSUMER 7
#define PRODUCER 5

pthread_cond_t prod = PTHREAD_COND_INITIALIZER,
	       	   cons = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		

int prod_pos = 0,
    cons_pos = 0,
    stock = 0,
    dataProd = 0,
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
	
	pthread_join (consumerThread[0], NULL);
	pthread_join (producerThread[0], NULL);
		
	return 0;
}

void * producer (void * arg)
{
	int i = *((int *) arg);
	
	while (1)
	{
		printf ("Produtor %d: vou produzir um item\n", i);
		sleep (2);
		pthread_mutex_lock (&mutex);
		
		dataProd = rand()%1000 + 1;
		while (stock == BUFFER)	//Enquanto o buffer estiver cheio, o produtor espera que o consumidor consuma um dado
		{
			printf ("Produtor %d: vou esperar espaco no buffer\n", i);
			pthread_cond_wait (&prod, &mutex);
		}
		
		printf ("Produtor %d: Produzindo item...\n", i);
		sleep (2);
		printf ("Produtor %d: vou inserir item %d na posicao %d\n", i, dataProd, prod_pos);
		buffer[prod_pos] = dataProd;
		prod_pos = (prod_pos + 1)%BUFFER;
		stock++;
		printf ("Dados em estoque: %d\n\n", stock);
		
		if (stock == 1)
			pthread_cond_signal (&cons);
		
		pthread_mutex_unlock (&mutex);
	}
	
	pthread_exit (0);
}

void * consumer (void * arg)
{
	int i = *((int *) arg);
	int removed = 0;
	
	while (1)
	{
		pthread_mutex_lock (&mutex);
		
		while (stock == 0)
		{
			printf ("Consumidor %d: vou esperar por itens\n", i);
			pthread_cond_wait (&cons, &mutex);
		}
		
		removed = buffer[cons_pos];
		printf ("Consumidor %d:\nDado removido da posicao %d: %d\n", i, cons_pos, removed);
		cons_pos = (cons_pos + 1)%BUFFER;
		stock--;
		printf ("Dados em estoque: %d\n\n", stock);
		
		if (stock == (BUFFER - 1))
			pthread_cond_signal (&prod);
		
		pthread_mutex_unlock (&mutex);
		
		printf ("Consumidor %d: Consumindo dado: %d\n", i, removed);
		sleep (15);	
	}
	
	pthread_exit (0);
}


