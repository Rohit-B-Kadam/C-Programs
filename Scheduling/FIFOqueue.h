// FIFOqueue.h
// Normal first in  first out
//header file of FCFS and Round Robin

#include <stdio.h>
#include <stdlib.h>

typedef struct Queue
{

	int pid;
	struct Queue *next;

}QUEUE;

QUEUE *head = NULL;



void addNODE( int Id)
{
	
    QUEUE *newnode = (QUEUE*)malloc(sizeof(QUEUE));
	newnode -> pid = Id;
	newnode -> next = NULL;


	if(head == NULL)
		head = newnode;
	else
	{
		QUEUE *temp = head;

        //last node paryant ja ani tithe add kr node
		while(temp -> next != NULL )
		{
			temp  =  temp -> next;
		}
        newnode -> next = temp -> next;
		temp -> next = newnode;	 
	}
		

}


int isEmpty()
{
	if( head == NULL)
		return 1;       //true
	else
		return 0;       //false
}

int removeNODE()
{
	
        //First node la temp madhi ghal ani head la phude ne
        //first node ch pid return kr

		QUEUE *temp = head;
		head = head -> next;
		int pid = temp -> pid;
		free(temp);
		return pid;		

}
