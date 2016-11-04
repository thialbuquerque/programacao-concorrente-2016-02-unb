/*DEFINITIONS*/
QUEUE * create_queue (void)	/*CREATES THE QUEUE*/
{
	QUEUE * newQueue = (QUEUE *) malloc (sizeof(QUEUE));
	if (newQueue == NULL)
		return NULL;
	
	newQueue->start = NULL;
	newQueue->end = NULL;

	return newQueue;
}

int queue (QUEUE ** q, int id)	/*QUEUE NEW ELEMENTS (THREAD IDs)*/
{
	if (q == NULL || (*q) == NULL)	/*QUEUE DOES NOT EXIST*/
		return -2;

	CLI_QUEUE * newClient = (CLI_QUEUE *) malloc (sizeof(QUEUE));	/*ALLOCATED MEMORY SPACE FOR THE NEW ELEMENT*/
	CLI_QUEUE 	* aux1 = (*q)->start,	/*AUXILIARY REFERENCE TO THE FIRST ELEMENT IN THE QUEUE*/
				* aux2 = (*q)->end;		/*AUXILIARY REFERENCE TO THE LAST ELEMENT IN THE QUEUE*/
	newClient->id = id;		/*NEW ELEMENT IS CREATED*/
	newClient->next = NULL;	/*THE NEW ELEMENTE MAY BE THE FIRST OR THE LAST, SO THE NEXT ELEMENT MUST BE NULL*/

	if (aux1 == NULL)	/*QUEUE IS EMPTY*/
	{
		(*q)->start = newClient;	/*REFERENCE TO THE FIRST ELEMENT IN THE QUEUE IS UPDATED*/
		(*q)->end = newClient; 		/*REFERENCE TO THE LAST ELEMENT IN THE QUEUE IS UPDATED*/
	}
	else 	/*QUEUE IS NOT EMPTY*/
	{
		aux2->next = newClient;	/**/
		aux2 = aux2->next;		/**/
		(*q)->end = aux2;		/**/
	}
    
	return 0;
}

int dequeue (QUEUE ** q)	/*DEQUEUE ELEMENTS*/
{
	CLI_QUEUE * aux = (*q)->start;
	int value = 0;

	if (q == NULL || (*q) == NULL)	/*QUEUE DOES NOT EXIST*/
		return -2;

	if ((*q)->start == NULL)
		return -1;
	else
	{	

		(*q)->start = aux->next;
		value = aux->id;
		free (aux);
	}

	return value;
}

int isEmpty (QUEUE ** q)
{
	if (q == NULL || (*q) == NULL)
		return -2;

	if ((*q)->start == NULL)
		return 0;
	else
		return 1;
}

int consultQueue (QUEUE * q)	/*CONSULT ELEMENTS OF THE QUEUE*/
{
	CLI_QUEUE * aux = q->start;

	printf("Fila:\n");
	
	if (aux == NULL)
		printf ("NULL\n");
	while (aux != NULL)
	{
		printf ("%d\n", aux->id);
		aux = aux->next;
	} 

	return 0;
}
