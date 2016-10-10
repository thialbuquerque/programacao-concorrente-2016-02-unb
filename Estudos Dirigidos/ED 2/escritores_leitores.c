#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

pthread_mutex_t lock_bd, 
				m = PTHREAD_MUTEX_INITIALIZER;
int contador = 0;



int main (int argc, char * argv[])
{
	pthread_t process[NUM_THREADS];
	
	return 0;
}


leitores(){
	while(1){
		lock(&m);		
		l++;
		if(l=1){lock(&bd)}		
		unlock(&m);		
		//ler dados;
		lock(&m);		
		l--;
		if(l=0){unlock(&bd)}
		unlock(&m);
		//processar dados;
	}
}

escritores(){
	while(1){
		//produz dados;
		lock(&bd);
		//escrevendo dados;
		unlock(&bd);
	}

}
