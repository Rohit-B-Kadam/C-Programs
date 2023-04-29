//  Round Robin
#include <stdio.h>
#include "FIFOqueue.h"


struct ProcessControl
{

	int pid;
	int burst_t;
    int cburst_t;  //chaning burst time
	int arrival_t;
	int completion_t;
	int wait_t;
	int start_t;
	int turn_around_t;	

}job[20];


int no_of_process;
double average_w = 0;      //average wait time
double average_t = 0;      //average turn around time

int quantum;



void addProcess( int time)
{
    for( int i = 0 ; i < no_of_process ; i++)
    {
        if( job[i].arrival_t == time )
        {
            addNODE(i);
        }
    }
}



void process()
{
    int time = 0;       //set to 0
    int pid = 0;        //running Process pid ..set at 0 
    int complete = 0;   //set to 0

    int quantumCount = 0;
    do{

        addProcess(time);
        
        
        if(complete == 0 ||  ( quantumCount == quantum) )
        {
            //jr running process complete zli tr aat yenre
            job[pid ].completion_t = time;
            if( complete != 0)
                addNODE(pid);

            if( !isEmpty() )
            {
                //neven process kadnr ani to start honr
                pid = removeNODE();

                if( job[pid].burst_t == job[pid].cburst_t )
                    job[pid].start_t = time;

                complete = job[pid].cburst_t;
            }

            quantumCount = 0;
        }

        quantumCount++;
        job[pid].cburst_t --;
        complete--;
        time++;

    }while( complete != -1 );
}


void calculate()
{
    int additionWait;
    average_t = 0;
    average_w = 0;
    for( int i = 0 ; i < no_of_process ; i++)
    {

        job[i].turn_around_t  =   job[i].completion_t - job[i].arrival_t;

        job[i].wait_t         =   job[i].start_t - job[i].arrival_t;
        additionWait          =   job[i].completion_t - job[i].start_t - job[i].burst_t;
        job[i].wait_t         =   job[i].wait_t + additionWait;

        //calcaulating total
        average_t = average_t + job[i].turn_around_t;
        average_w = average_w + job[i].wait_t;
    }

    //calculating average
    average_t = average_t / no_of_process;
    average_w = average_w / no_of_process;

}


void display()
{
int i;

	printf("\n\n\t\tProcess     Burst Time     Arrival Time     start Time     Wait Time      Completion Time     Turn Around Time \n");
	printf("\t-----------------------------------------------------------------------------------------------------------------------\n");
	for(i=0; i< no_of_process; i++)
	{
	printf("\n\t\tP%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\n",job[i].pid	,job[i].burst_t,job[i].arrival_t,job[i].start_t,job[i].wait_t,job[i].completion_t,job[i].turn_around_t);
	printf("\t------------------------------------------------------------------------------------------------------------------------\n");
	}

	printf("\t\tAverage Wait Time = %f\n \t\tAverage Turn Around Time = %f\n",average_w,average_t);


}

int main()
{
	
	int i;

    printf("Enter the quantum time: ");
    scanf("%d",&quantum);
	printf("Enter the number of process\n");
	scanf("%d",&no_of_process);

	for(i=0; i < no_of_process; i++)
	{
		printf("Enter the Process Name , Burst Time and Arrival Time for the process \n");
		scanf("%d %d %d",&job[i].pid,&job[i].burst_t,&job[i].arrival_t);

        job[i].cburst_t = job[i].burst_t;
		
	}

	process();
    calculate();
	display();
}




