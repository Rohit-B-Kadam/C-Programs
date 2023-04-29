#include<stdio.h>

#define MAX_MEM_SIZE 50
#define MAX_REFERENCES 50
#define STACK_SIZE 50
#define BOTTOM 0



//shared variables
int no_of_frames, mem[MAX_MEM_SIZE], page_ref[MAX_REFERENCES], page_req_ptr=0, victim=-1, page_faults=0, page_replacements=0;

//LRU stack
int stack[STACK_SIZE], buffer[STACK_SIZE], top=-1;

//Stack operations

int isEmpty()
{
	return(top == BOTTOM);
}

int isFull()
{
	return(top == STACK_SIZE -1);
}

void push(int item)
{
	if(!isFull())
	{
		stack[++top] = item;
	}
}

int pop()
{
	if(!isEmpty())
	return stack[top--];
	else return -1;
}


int peep()
{
	if(!isEmpty()) 	return stack[0];
	else return -1;
}

// end of stack operations

void print_memory()
{
	int i;
	printf("\nMEMORY: ");
	for(i=0; i<no_of_frames; i++)
	{
		printf(" | %d ",mem[i]);
		//printf(" | %d :@ %d ",mem[i][DATA],mem[i][TIME_STAMP]);
	}
	printf("	Current Page Req: %d | Total Page Faults: %d\n",page_ref[page_req_ptr],page_faults);
}





//allocate_frame() searches for first free frame to allocate
//returns 0 if Page replacement not required, i.e. free memory available
//else returns 1 if page replacement required, appropriate algorithm is chosen in main()
int allocate_frame()
{
	int i, temp, j=0, theOne;
	//search in memory if page already exists; if exists return 0
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i] == page_ref[page_req_ptr])
		{
			//code of stack manipulation
			
			//search in stack the page number
			//bring to top
			//popping loop
			while(1)
			{
				buffer[j++] = pop();
				if(buffer[j-1] == mem[i])
				break;	
			}
			
			theOne = j-1; 
			// theOne is bufferLocation where the pageNumber is stored
			
			j = j-2; // j is set to one location behind theOne
			
			//pushing back
			while(1)
			{
				push(buffer[j--]);
				if(j == -1) break;	
			}
			
			push(buffer[theOne]);
			
			return 0;
		}	
	}
	//page not found in memory
	page_faults++;	
	
		
	//search for free memory
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i] == -1) break; //free frame available, stop searching
	}
	
	if(i >= no_of_frames) // free frame not found
	return 1; //page replacement needed
	
	
	//free frame allocation
	else if(i < no_of_frames) // free frame is i'th frame, allocate to page
	{
		mem[i] = page_ref[page_req_ptr];
				
		// add page number to stack: recent used page
		push(page_ref[page_req_ptr]);
		
		return 0;
	}	
	
	
}//end of allocate_frame()



void LRUS()
{
	int i;
	
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i] == peep())
		{
			victim = i;
			break;
		}
	}
		
	
	mem[victim] = -1;
	print_memory();	
}





 
main()
{
	int  i;	
	
	//accepting no of frames
	printf("\nDEMAND PAGING: LRU (Using counter) SIMULATION\n");
	printf("\nEnter number of frames: ");
	scanf("%d",&no_of_frames);
	
	//initializing memory
	for(i=0; i<MAX_MEM_SIZE; i++)
	{
		mem[i] = -1;
		page_ref[i] = -1;
	}
	
	print_memory();
	
	//accept page reference string
	printf("\nPage references (HIT \"E\" after after your input): ");
	for(i=0; i<MAX_MEM_SIZE; i++)
	{
		scanf("%d",&page_ref[i]);
		
		if(page_ref[i] == 'e' || page_ref[i] == 'E')
		{
			page_ref[i] = -1;
			break;
		}
	}
	
	while(page_ref[page_req_ptr] != -1) // process all page requests
	{
		//allocate frame to next page
		//page_ref initialized to -1, to know end of references
		
		//0 returned by allocate_frame() indicates no replacement needed
		
		if(allocate_frame() == 0) //allocation successful
		{
			print_memory(); //print memory status: page scheduling
			page_req_ptr++;
		}
		
		//else if allocation not successful
		else
		{
			LRUS();
			page_faults--; 
			// since above call to allocate_frame() incremented faults
			// LRUC was called to free a frame
			// Again a call to actually allocate frame
			// (to prohibit re-increment of page_faults)  
			
			allocate_frame(); // this allocation is surely successful
			page_replacements++;
			
			print_memory(); //print memory status: page scheduling
			page_req_ptr++; // hence page_req_ptr incremented
		}		
		
		
		
	}//while(page_requests)
	
	printf("\nTotal Page Faults: %d\nTotal Page Replacements: %d\n",page_faults, page_replacements);
	
}
