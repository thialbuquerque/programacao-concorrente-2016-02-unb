/*DEFINITIONS*/

void clrscr(void )
{
	system ("clear || cls");	/*CLEARS TEXT OF THE SCREEN OF THE TERMINAL*/
}

void asterisk (void )
{
	int asterisco;
	
	#ifndef LIM
		#define LIM 49 
	#endif
	printf ("\t\t");
	for (asterisco = 0; asterisco < LIM; asterisco++)
		putchar ('*');
	putchar ('\n');
}

void presentation (void)
{
	clrscr();
		
	asterisk ();
	printf ("\t\t*\t\tPeaceful Taste\t\t\t*\n");
	asterisk ();
	putchar ('\n');
	printf ("Be welcome to the most famous vegan restaurant in the world!!\n");
}

void initializer (void)
{
	sem_init (&hoster, 0, CLIENTS_MAX);				/*Initializing hoster semaphore*/	
	sem_init (&bathroomFemale, 0, BATHROOM);		/*Initializing female bathroom semaphore*/	
	sem_init (&bathroomMale, 0, BATHROOM);			/*Initializing male bathroom semaphore*/
	sem_init (&cooker, 0, 0);						/*Initializing cooker semaphore*/
	sem_init (&waiter, 0, 0);						/*Initializing waiter semaphore*/
	sem_init (&serve, 0, 0);						/*Initializing serve semaphore*/	
	sem_init (&eat, 0, 0);							/*Initializing eat semaphore*/
	pthread_barrier_init (&barrier, NULL, CLIENTS);	/*Initializing barrier*/
}

void print (char * string)
{
	mutex
	verifica se codigo de saida é table
	imprime
	mutex
}

int searchSeat (int * table)
{
	int	sat = 0,		/*binary variable that determines if one specific client sat down on a chair*/	
		usedChair = 0,	/*Stores the id of the chair one client will be sitting down on*/
		i = 0;			/*Counter*/
	do
	{	
		i = 0;
			
		*table = rand()%(TABLES - 1) + 1;	/*Selects one of the tables*/
		do 	/*Searches for one of the EMPTY seats to the client*/
		{
			if (chairs[*table][i] == '0')	/*Found one empty chair*/
			{
				chairs[*table][i] = '1';	/*Marks the chair as occupied*/
				sat = 1;				/*Marks the client as sat*/
				usedChair = i;
			}
			i++;	/*Goes to the next chair of the selected table*/
		} while (i < CHAIRS && sat == 0);
		
	} while (sat == 0);

	return usedChair;
}

char getch() 
{
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

void * cookers (void * arg)
{
	int id = *((int *) arg) + 1;
	int timer = 0;
	
	while (1)
	{	
		sem_wait (&cooker);
		
		if (finished || exitCode)
			break;
		if (seconds == TIME && clientCounter == 0)
		{

		}

		printf (GREEN "Cooker %d" RESET " received the request!\n", id);
		printf (GREEN "Cooker %d" RESET " started cooking...\n", id);
		timer = rand()%6 + 1;
		sleep (timer);
		printf (GREEN "Cooker %d:" RESET " Food prepared and ready do serve!\n", id);
		printf (GREEN "Calling a waiter...\n");
		sleep (1);
		sem_post (&serve);
	}
	pthread_exit (NULL);
}

void * waiters (void * arg)						
{
	int id = *((int *) arg) + 1;
	int request = 0,
		timer = 0;
	
	while (1)														
	{											
																	
		sem_wait (&waiter);														
		
		if (finished || exitCode)
			break;
		if (seconds == TIME && clientCounter == 0)
		{

		}
		
		timer = rand()%3 + 1; 
		sleep (timer);
		printf (MAGENTA "Waiter %d" RESET " called!\n", id);
		sleep (1);
		printf (MAGENTA "Waiter %d" RESET " noting the request...\n", id);
		request = rand()%3 + 1;
		sleep (request);
		printf (MAGENTA "Waiter %d" RESET " making the request to a cooker...\n", id);
		sem_post (&cooker);
		sem_wait (&serve);
		printf (MAGENTA "Waiter %d" RESET " is serving the food...\n", id);
		sleep (1);
		printf(MAGENTA "Food Served!\n");
		sem_post (&eat);
	}
	pthread_exit (NULL);
}

/*situações em que vai fechar	                            
quando o tempo acabar	- espera pelos remanecentes		
quando os clientes acabarem -                           
quando o codigo for digitado - espera pelos remanecentes*/
void * clients (void * arg)				
{
	int id = *((int *) arg) + 1, 
		action = 0,
		table = 0, 
		usedChair = 0,
		gender = rand()%2 + 1,	/*1 indicates the client is a woman, 2 indicates the client is a man*/
		timer = 0,
		diff = 0,
		ate = 0,
		i = 0;

		pthread_mutex_lock (&entranceDoorMutex);	/*Only one client can pass through the entrance door at a time*/
		if (sem_trywait (&hoster) == 0)		/*Once the client is able to pass through the entrance door,*/ 
		{									/*the client can verify if the restaurant is full or not*/
			if (firstID == 0)	/*The first thread that enters in the critial region/first client in te restaurant*/
				firstID = id;	/*The id of the first client in the restaurant is stored*/

			pthread_mutex_unlock (&entranceDoorMutex);	/*Frees the entrance door*/
			sleep (1);

			pthread_mutex_lock (&clientCounterMutex);
				clientCounter++;
			pthread_mutex_unlock (&clientCounterMutex);

			pthread_mutex_lock (&seatMutex);	/*One client at a time is gonna sit. It avoids race conditions*/
			if (exitCode || seconds == TIME)
			{
				pthread_mutex_lock (&clientCounterMutex);
				clientCounter--;
				if (clientCounter == 0)
					pthread_cond_signal (&counterCond);
				pthread_mutex_unlock (&clientCounterMutex);
				
				pthread_mutex_unlock (&seatMutex);
				pthread_exit (NULL);
			}

			printf (CYAN "Client %d" RESET " has just arrived!\n", id);
			sleep (1);
			printf (YELLOW "Hoster" RESET " is taking the client %d to %s seat...\n", id, (gender == 1) ? "her" : "his");	
			usedChair = searchSeat (&table) + 1;
			table++;
			sleep (2);
			
			diff = CHAIRS - usedChair;
			printf (CYAN "Client %d" RESET " just sat on the chair %d, chair number %d at the table %d!\n", id, ((table * CHAIRS) - diff), usedChair, table);		
			
			/*
			for (i = 0; i < TABLES; i++)
			{
				for (j = 0; i < CHAIRS; j++)
				{
					printf ("%c\n", chairs[i][j]); //imprimir o ID? maitriz de inteiros (short)
				}
			}
			*/

			pthread_mutex_unlock (&seatMutex);	/*Another client can seat*/
			

			do
			{
				if (exitCode || seconds == TIME)	/*The restaurant is going to close*/
					action = 3;
				else
					action = rand()%3 + 1;	/*The client selects one action*/
				
				switch (action) 
				{
					case 1:	/*Uses bathroom*/
					{
						switch (gender)
						{
							timer = rand()%5 + 1;
							case 1:	/*The client is a woman*/
							{
								sem_wait (&bathroomFemale);
								printf (CYAN "Client %d" RESET " is using the lady's room\n", id);		
								sem_post (&bathroomFemale);
							} break;
							case 2:	/*The client is a man*/
							{
								sem_wait (&bathroomMale);
								printf (CYAN "Client %d" RESET " is using the men's room\n", id);
								sem_post (&bathroomMale);
							} break;
						}
						sleep (timer);
						printf (CYAN "Client %d" RESET " got out of the bathroom\n", id);
					} break;
					case 2:	/*Order food and eat it*/
					{
						sem_post (&waiter);
						sem_wait (&eat);
						printf (CYAN "Client %d" RESET " is eating...\n", id);
						timer = rand()%9 + 1;
						sleep (timer);
						printf (CYAN "Client %d" RESET " finished eating!\n", id);
						ate = 1;
					} break;
					case 3: /*Go to the cashier to pay the bill and leave the restaurant*/
					{
						if (ate == 1)	/*If the client ate something, he or she pays and gets out. Otherwise, just gets out*/
						{				
							pthread_mutex_lock (&queueAccessMutex);
							queue (cashierQueue, id);
							/*consultQueue (*cashierQueue);*/
							pthread_mutex_unlock (&queueAccessMutex);
						
							pthread_mutex_lock (&cashierMutex);
							printf(CYAN "Client %d" RESET " is paying the bill.\n", id);
							sleep (1);

							pthread_mutex_lock (&queueAccessMutex);
							dequeue (cashierQueue);
							pthread_mutex_unlock (&queueAccessMutex);
							
							pthread_mutex_unlock (&cashierMutex);

							printf(CYAN "Client %d" RESET " payed the bill.\n", id);
						}
									
						printf (CYAN "Client %d" RESET " is leaving!!\n", id);
						table--;
						usedChair--;
						
						pthread_mutex_lock (&seatMutex);
						chairs[table][usedChair] = '0';
						pthread_mutex_unlock (&seatMutex);
						
						sleep (1);
						pthread_mutex_lock (&clientCounterMutex);
						clientCounter--;
						if (exitCode && clientCounter == 0)
							pthread_cond_signal (&counterCond);
						pthread_mutex_unlock (&clientCounterMutex);
						
						pthread_mutex_lock (&exitDoorMutex);
						if (exitCode == 0 && seconds != TIME)	
							sem_post (&hoster);
						else
							pthread_exit (NULL);
						sleep (1);
						pthread_mutex_unlock (&exitDoorMutex);	
					} break;
				}
			} while (action != 3);
		}
		else
		{
			pthread_mutex_unlock (&entranceDoorMutex);
		}

	pthread_barrier_wait (&barrier);
	if (firstID == id)
	{
		finished = 1;
		for (i = 0; i < COOKER; i++)
			sem_post (&cooker);
		for (i = 0; i < WAITER; i++)
			sem_post (&waiter);	
		sleep (3);
	}
	pthread_exit (NULL);
}

void * reader (void * arg)
{		
	char ch;
	do
	{
		ch = tolower(getch ());
	} while (ch == 'c');

	exitCode = ch;
	pthread_cond_signal (&exitCond);
	pthread_exit (NULL);
}

void * timeCounter (void * arg)
{		
	time_t beginning = 0;
	beginning = timeElapsed;
	
	do
	{
		if ((finished || exitCode))
			break;
		seconds = difftime (timeElapsed, beginning);
		timeElapsed = time (NULL);
	} while (seconds != TIME);

	pthread_cond_signal (&exitCond);

	pthread_exit (NULL);
}