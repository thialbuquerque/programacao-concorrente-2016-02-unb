/*
*Algorithm:			
*Description:		
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
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <mpi.h>
#include <unistd.h>

void * consumer (void * arg);
void * producer (void * arg);

int main (int argc, char * argv[])
{
	
	return 0;
}

void * consumer (void * arg)
{
	pthread_exit (0);
}

void * producer (void * arg)
{
	pthread_exit (0);
}