//priorityQueue.h
//First out but insert depending upon priorty
// header file of SJF Priority

#include <stdio.h>
#include <stdlib.h>


typedef struct Queue
{

	int pid;
	struct Queue *next;

}QUEUE;

QUEUE *head = NULL;


int comparePriority( int pid1 , int pid2);// nhi lihla tr nusta warning yenr

void addNODE( int Id )
{
	
    QUEUE *newnode = (QUEUE*)malloc(sizeof(QUEUE));
	newnode -> pid = Id;
	newnode -> next = NULL;


	if(head == NULL)
		head = newnode;
	else
	{

		if( comparePriority(head->pid , Id) == 0  )
		{
			newnode ->next = head;
			head = newnode;	
		}
		else
		{

			QUEUE *temp = head;
			while(temp -> next != NULL )
			{
				if(   comparePriority( temp->next->pid , Id) == 0 )
					break;
				temp  =  temp -> next;

			}

        newnode -> next  =  temp->next;
		temp -> next = newnode;
		}

		//bodkache khtkhte	 
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


int topNode()
{
	if(isEmpty())
	{
		return 0;
	}
	else
	{
		return head->pid;
	}

}