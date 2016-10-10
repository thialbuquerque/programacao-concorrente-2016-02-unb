/*
*Algorithm:			Jantar dos Filósofos, proposto por Djikstra
*Description:		5 filósofos estão sentados em uma mesa redonda, cada um tendo um garfo à sua direita. Cada filósofo deve, alternadamente, refletir e comer.
					Para que um filósofo coma, ele deve possuir 2 garfos (um à direita, e um à esquerda). Um garfo somente pode ser pego por um filósofo e se, 
					e somente se, não estiver em uso. Quando um filósofo termina de comer, ele libera os garfos em sua posse.
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
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


void * filosofo (void *);


int main (int argc, char * argv[])
{
	
	return 0;
}

