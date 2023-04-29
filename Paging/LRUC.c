#include<stdio.h>

#define MAX_MEM_SIZE 50
#define MAX_REFERENCES 50
#define DATA 0
#define TIME_STAMP 1


//shared variables
int no_of_frames, mem[MAX_MEM_SIZE][2], page_ref[MAX_REFERENCES], page_req_ptr=0, victim=-1, page_faults=0, page_replacements=0;




void print_memory()
{
	int i;
	printf("\nMEMORY: ");
	for(i=0; i<no_of_frames; i++)
	{
		printf(" | %d ",mem[i][DATA]);
		//printf(" | %d :@ %d ",mem[i][DATA],mem[i][TIME_STAMP]);
	}
	printf("	Current Page Req: %d | Total Page Faults: %d\n",page_ref[page_req_ptr],page_faults);
}





//allocate_frame() searches for first free frame to allocate
//returns 0 if Page replacement not required, i.e. free memory available
//else returns 1 if page replacement required, appropriate algorithm is chosen in main()
int allocate_frame()
{
	int i;
	//search in memory if page already exists; if exists return 0
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i][DATA] == page_ref[page_req_ptr])
		{
			mem[i][TIME_STAMP] = page_req_ptr;	
			return 0;
		}	
	}
	//page not found in memory
	page_faults++;	
	
		
	//search for free memory
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i][DATA] == -1) break; //free frame available, stop searching
	}
	
	if(i >= no_of_frames) // free frame not found
	return 1; //page replacement needed
	
	
	//free frame allocation
	else if(i < no_of_frames) // free frame is i'th frame, allocate to page
	{
		mem[i][DATA] = page_ref[page_req_ptr];
		mem[i][TIME_STAMP] = page_req_ptr;
		return 0;
	}	
	
	
}//end of allocate_frame()



void LRUC()
{
	int i,minimum = mem[0][TIME_STAMP], indexOfMinTimeStamp = 0;

	//victim:
	//frame that is last recently used has lowest timestamp
	//LRU finds lowest timestamp out of total frames and swaps that page out
	for(i=0; i<no_of_frames; i++)
	{
		if(mem[i][TIME_STAMP] < minimum)
		{
			minimum = mem[i][TIME_STAMP];
			indexOfMinTimeStamp = i;
		}	
		///*for debugging printf("*%d*",mem[i][TIME_STAMP]); */
	}
	
	victim = indexOfMinTimeStamp;
	
	mem[victim][DATA] = -1; // swap out victim frame

	printf("\nLRU (Using Counter): Victim Frame Evacuated: ");
	
	print_memory(); //print memory status: after replacement

	
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
		mem[i][DATA] = -1;
		mem[i][TIME_STAMP] = 0;
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
		if(allocate_frame() == 0) //allocation successful
		{
			print_memory(); //print memory status: page scheduling
			page_req_ptr++;
		}
		
		//else if allocation not successful
		else
		{
			LRUC();
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
