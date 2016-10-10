/*
*Algorithm:			Barbeiro Sonolento
*Description:		Barbeiro dorme enquanto não há clientes. Acorda quando ao menos um cliente está esperando para ser atendido, 
					podendo ter no máximo 5 clientes em espera e no máximo um sendo atendido. 
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
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 5	//Cadeiras para os clientes que aguardam pelo atendimento
#define CLIENTS 10	//Quantidade de clientes ao todo

void * barber (void * arg);	//Rotina para o barbeiro
void * client (void * arg);	//Rotina para os clientes

sem_t  	haircutSem,	//Semáforo para indicar que um cliente cortou o cabelo
 		barberSem,	//Semáforo para o barbeiro
		clientSem,	//Semáforo para os clientes que estão esperando pelo atendimento
		chairsSem;	//Semáforo para os clientes que chegam à barbearia
int	waiting = 0;	//Quantidade de clientes esperando atentimento	

int main (int argc, char * argv[])
{
	pthread_t	barberThread,			//Thread para o barbeiro
				clientThread[CLIENTS];	//Conjunto de threads para os clientes
	int i = 0,		//Contador
		* idC = NULL,	//Identificador para as threads de cliente
		valid = 0;  //Valida a criação das threads
	
	//Inicializando os semáforos
	sem_init (&barberSem, 0, 0);
	sem_init (&clientSem, 0, 1);
	sem_init (&haircutSem, 0, 0);
	sem_init (&chairsSem, 0, CHAIRS);
	//Criando semente geradora
	srand (time(NULL));
	//Criando thread para o barbeiro
	valid = pthread_create (&barberThread, NULL, barber, NULL);
	
	if (valid)
	{
		printf ("Falha na criacao da thread barberThread!\n");
		return -1;
	}
	//Criando threads para os clientes
	for (i = 0; i < CLIENTS; i++)
	{
		idC = (int *) malloc (sizeof(int));
		*idC = i;
		
		valid = pthread_create (&clientThread[i], NULL, client, (void *) idC);
		
		if (valid)
		{
			printf ("Falha na criacao da thread clientThread %d!\n", i);
			return -1;	
		}
	}
	
	valid = pthread_join (barberThread, NULL);
	
	if (valid)
	{
		printf ("Falha no join da thread barberThread!\n");
		return -1;
	}
	
	
	for (i = 0; i < CLIENTS; i++)
	{
		valid = pthread_join (clientThread[i], NULL);
		
		if (valid)
		{
			printf ("Falha no join da thread clientThread %d!\n", i);
			return -1;	
		}
	}
	
	return 0;
}
//Rotina responsável pelas tarefas do barbeiro
void * barber (void * arg)
{
	while (1)
	{
		sem_wait (&barberSem);
		waiting--;
		printf ("Cortando o cabelo de um cliente\n");
		sleep (3);
		sem_post (&haircutSem);
	}
	
	pthread_exit (NULL);
}
//Rotina responsável pelas tarefas dos clientes
void * client (void * arg)
{
	int id = *((int *) arg);
	
	while (1)
	{	
		sleep ((rand()%3 + 1));
		if (sem_trywait(&chairsSem) == 0 && waiting < CHAIRS)	//sem_wait executado com sucesso
		{
			waiting++;
			//tem lugar para um cliente sentar-se, ocupa-se um lugar
			printf ("Tem lugar disponivel para o cliente %d\n", id);
			sem_wait (&clientSem);	//cliente aguardando atendimento
			printf ("Cliente %d aguardando...\n", id);
			sleep (1);
			sem_post (&barberSem);	//acorda o barbeiro
			sleep (1);
			sem_post (&chairsSem);	//disponibiliza uma cadeira para um novo cliente
			sem_wait (&haircutSem);	//cortou o cabelo do cliente, cliente na cadeira do barbeiro
			printf ("Cliente %d ja foi atendido\n\n", id);
			sem_post (&clientSem);	//cliente libera o barbeiro, o próximo cliente pode ser atendido
		}
		else
			if (waiting == CHAIRS)
				sem_post (&barberSem);
			//printf ("Nao ha lugar vazio para o cliente %d sentar-se\n\n", id);	
	}
	
	pthread_exit (NULL);
}
