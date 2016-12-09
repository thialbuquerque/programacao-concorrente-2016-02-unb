/*DEFINITIONS*/

void clrscr(void )
{
	system ("clear || cls");	/*CLEARS TEXT OF THE SCREEN OF THE TERMINAL*/
}

void asterisk (void )
{
	int asterisco;
	
	printf (YELLOW "\t\t");
	for (asterisco = 0; asterisco < LIM; asterisco++)
		putchar ('*');
	putchar ('\n');
}

void presentation (void)
{
	clrscr();
		
	asterisk ();
	printf (YELLOW "\t\t*\t\t" GREEN "Peaceful Taste" YELLOW "\t\t\t*\n");
	asterisk ();
	putchar ('\n');
	printf (RESET "Be welcome to the most famous vegan restaurant in the world!!\n");
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

char getch(void) 
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

		pthread_mutex_lock (&exitMutex);
		if (finished || (seconds == TIME && clientCounter == 0))	/*Either all clients expected to show were attended*/ 
		{															/*or it is already time for the restaurant to close and all clients*/
																	/*who were inside the restaurant are gone*/
			pthread_mutex_unlock (&exitMutex);
			break;
		}
		pthread_mutex_unlock (&exitMutex);		

		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (GREEN "Cooker %d" RESET " received the request!\n", id);
			printf (GREEN "Cooker %d" RESET " started cooking...\n", id);
		}
		pthread_mutex_unlock (&outputMutex);
		
		timer = rand () % 6 + 1;
		sleep (timer);

		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (GREEN "Cooker %d:" RESET " Food prepared and ready do serve!\n", id);
			printf (GREEN "Cooker %d is calling a waiter...\n", id);
		}
		pthread_mutex_unlock (&outputMutex);
		
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

		pthread_mutex_lock (&exitMutex);
		if (finished || (seconds == TIME && clientCounter == 0))	/*Either all clients expected to show were attended*/ 
		{															/*or it is already time for the restaurant to close and all clients*/
																	/*who were inside the restaurant are gone*/
			pthread_mutex_unlock (&exitMutex);
			break;
		}
		pthread_mutex_unlock (&exitMutex);
		
		timer = rand () % 3 + 1;
		sleep (timer);
		
		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (MAGENTA "Waiter %d" RESET " called!\n", id);
		}
		pthread_mutex_unlock (&outputMutex);

		sleep (1);
		
		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (MAGENTA "Waiter %d" RESET " noting the request...\n", id);
		}
		pthread_mutex_unlock (&outputMutex);

		request = rand () % 3 + 1;
		sleep (request);
		
		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (MAGENTA "Waiter %d" RESET " making the request to a cooker...\n", id);
		}
		pthread_mutex_unlock (&outputMutex);

		sem_post (&cooker);
		sem_wait (&serve);
		
		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (MAGENTA "Waiter %d" RESET " is serving the food...\n", id);
		}
		pthread_mutex_unlock (&outputMutex);

		sleep (1);
		
		pthread_mutex_lock (&outputMutex);
		if (output)
		{
			printf (MAGENTA "Food Served!\n");
		}
		pthread_mutex_unlock (&outputMutex);

		sem_post (&eat);
	}
	pthread_exit (NULL);
}

void * clients (void * arg)				
{
	int id = *((int *) arg) + 1, 
		action = 0,
		table = 0, 
		usedChair = 0,
		gender = rand () % 2 + 1,	/*1 indicates the client is a woman, 2 indicates the client is a man*/
		timer = 0,
		diff = 0,
		ate = 0,
		i = 0;

		pthread_mutex_lock (&entranceDoorMutex);	/*Only one client can pass through the entrance door at a time*/
		if (sem_trywait (&hoster) == 0)		/*Once the client is able to pass through the entrance door,*/ 
		{									/*the client can verify if the restaurant is full or not*/
			pthread_mutex_unlock (&entranceDoorMutex);	/*Frees the entrance door*/

			if (firstID == 0)	/*The first thread that enters in the critical region/first client in the restaurant*/
				firstID = id;	/*The id of the first client in the restaurant is stored*/
			sleep (1);	

			pthread_mutex_lock (&seatMutex);	/*One client at a time is gonna sit. It avoids race conditions*/
			
			if (seconds == TIME)	/*This condition avoids unnecessary output is shown in case the time for the restaurant to close is reached*/
			{
				pthread_mutex_unlock (&seatMutex);
				pthread_exit (NULL);
			}
			pthread_mutex_lock (&clientCounterMutex);
			clientCounter++;	/*Counts the number of clients inside the restaurant*/
			pthread_mutex_unlock (&clientCounterMutex);
			
			pthread_mutex_lock (&outputMutex);
			if (output)
			{
				printf (CYAN "Client %d" RESET " has just arrived!\n", id);
			}
			pthread_mutex_unlock (&outputMutex);

			sleep (1);
			
			pthread_mutex_lock (&outputMutex);
			if (output)
			{
				printf (YELLOW "Hoster" RESET " is taking the client %d to %s seat...\n", id, (gender == 1) ? "her" : "his");	
			}
			pthread_mutex_unlock (&outputMutex);

			usedChair = searchSeat (&table) + 1;
			table++;
			sleep (2);
			
			diff = CHAIRS - usedChair;
			
			pthread_mutex_lock (&outputMutex);
			if (output)
			{
				printf (CYAN "Client %d" RESET " just sat on the chair %d, chair number %d at the table %d!\n", id, ((table * CHAIRS) - diff), usedChair, table);		
			}
			pthread_mutex_unlock (&outputMutex);

			pthread_mutex_unlock (&seatMutex);	/*Another client can seat*/
			
			do
			{
				if (seconds == TIME)	/*The restaurant is going to close*/
					action = 3;
				else
					action = rand () % 7 + 1;	/*The client selects one action. 
												The client choose between on of the 3 options*/		
				switch (action) 
				{
					case 1:	/*Uses bathroom*/
					{
						switch (gender)
						{
							timer = rand () % 2 + 1;
							case 1:	/*The client is a woman*/
							{
								sem_wait (&bathroomFemale);
								
								pthread_mutex_lock (&outputMutex);
								if (output)
								{
									printf (CYAN "Client %d" RESET " is using the lady's room\n", id);		
								}
								pthread_mutex_unlock (&outputMutex);
								
								sem_post (&bathroomFemale);
							} break;
							case 2:	/*The client is a man*/
							{
								sem_trywait (&bathroomMale);
								
								pthread_mutex_lock (&outputMutex);
								if (output)
								{
									printf (CYAN "Client %d" RESET " is using the men's room\n", id);
								}
								pthread_mutex_unlock (&outputMutex);

								sem_post (&bathroomMale);
							} break;
						}
						sleep (timer);
						
						pthread_mutex_lock (&outputMutex);
						if (output)
						{
							printf (CYAN "Client %d" RESET " got out of the bathroom\n", id);
						}
						pthread_mutex_unlock (&outputMutex);	
					} break;
					case 2:	/*Order food and eat it*/
					{
						sem_post (&waiter);
						sem_wait (&eat);
						
						pthread_mutex_lock (&outputMutex);
						if (output)
						{
							printf (CYAN "Client %d" RESET " is eating...\n", id);		
						}
						pthread_mutex_unlock (&outputMutex);
						
						timer = rand () % 9 + 1;
						sleep (timer);
						
						pthread_mutex_lock (&outputMutex);
						if (output)
						{
							printf (CYAN "Client %d" RESET " finished eating!\n", id);
						}
						pthread_mutex_unlock (&outputMutex);

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
							
							pthread_mutex_lock (&outputMutex);
							if (output)
							{
								printf(CYAN "Client %d" RESET " is paying the bill.\n", id);						
							}
							pthread_mutex_unlock (&outputMutex);
							
							sleep (1);

							pthread_mutex_lock (&queueAccessMutex);
							dequeue (cashierQueue);
							pthread_mutex_unlock (&queueAccessMutex);
							
							pthread_mutex_unlock (&cashierMutex);

							pthread_mutex_lock (&outputMutex);
							if (output)
							{
								printf(CYAN "Client %d" RESET " payed the bill.\n", id);
							}
							pthread_mutex_unlock (&outputMutex);	
						}
						
						pthread_mutex_lock (&outputMutex);
						if (output)
						{			
							printf (CYAN "Client %d" RESET " is leaving!!\n", id);
						}
						pthread_mutex_unlock (&outputMutex);
						
						pthread_mutex_lock (&seatMutex);
						table--;
						usedChair--;
						chairs[table][usedChair] = '0';
						pthread_mutex_unlock (&seatMutex);
						
						pthread_mutex_lock (&clientCounterMutex);
						clientCounter--;
						if (clientCounter == 0)
							pthread_cond_signal (&counterCond);
						pthread_mutex_unlock (&clientCounterMutex);

						sleep (1);
						
						pthread_mutex_lock (&exitDoorMutex);
						pthread_mutex_lock (&exitMutex);
						if (seconds != TIME)
						{	
							pthread_mutex_unlock (&exitMutex);
							sem_post (&hoster);
						}
						else
						{
							pthread_mutex_unlock (&exitMutex);
							pthread_mutex_unlock (&exitDoorMutex);
							pthread_exit (NULL);
						}
						pthread_mutex_unlock (&exitDoorMutex);

						sleep (1);	
					} break;
				}
			} while (action != 3);
		}
		else
		{
			pthread_mutex_unlock (&entranceDoorMutex);
		}

	pthread_barrier_wait (&barrier);	/*Controls the access to "finished", which determines if all clients expected
										to appear were attended*/
	if (firstID == id)	/*Just the first thread that had access to the entrance door 
						executes the instructions after the barrier*/
	{
		finished = 1;	
		pthread_cond_signal (&exitCond);
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
		pthread_mutex_lock (&exitMutex); 
		exitCode = 0;
		pthread_mutex_unlock (&exitMutex); 

		do
		{
			fflush (stdin);
			ch = getch ();
			ch = tolower(ch);
		} while (ch == 'c');

		pthread_mutex_lock (&exitMutex); 
		exitCode = ch;
		pthread_cond_signal (&exitCond);
		pthread_cond_wait (&confirmationCond, &exitMutex);
		pthread_mutex_unlock (&exitMutex);
		
	} while (exitConfirmation != 'N');	/*If false: The user does not want to keep monitoring the flow in the restaurant.*/
		
	pthread_exit (NULL);
}

void * timeCounter (void * arg)
{		
	time_t beginning = 0;
	beginning = timeElapsed;
	
	do
	{
		if (finished) 
			break;
		seconds = difftime (timeElapsed, beginning);
		time (&timeElapsed);
	} while (seconds != TIME);

	pthread_cond_signal (&exitCond);

	pthread_exit (NULL); 
}
