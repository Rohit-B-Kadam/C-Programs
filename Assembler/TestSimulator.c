#include<stdio.h>

int memory[1000];
int address,content ,opcode,regNum;
int reg[4],cc[]={0,0,0,0,0,1};
int pc,lc;

void load(char filename[])
{
	pc=0;
	FILE *fp;
	if((fp=fopen(filename,"r"))==NULL)
	{
		printf("Error in opening file");
	}
	else{
		while(fscanf(fp,"%d%d",&address,&content)!=EOF)
		{
			if(address==-1)
				memory[pc]=content;
			else
				memory[pc++]=content;
		}
	}
	printf("File is successfully loaded ");
	fclose(fp);
}

void printSimulatorProgram()
{
	lc=pc;
	printf("\n\nSimulator program:-\n");
	for(pc=0;pc<lc;pc++)
		printf("\t\t%d\n",memory[pc]);
}

void runSimulatorProgram()
{
	lc=pc;
	for(pc=0;pc<lc;pc++)
	{
		opcode=memory[pc] / 10000;
		regNum=(memory[pc]/1000)%10;
		address=memory[pc]%1000;
		switch(opcode)
		{
			case 0: ///Exit;
				break;
			case 1: //Addition
				reg[regNum] += memory[address];
				break;
			case 2: //Sub
				reg[regNum] -= memory[address];
                                break;
                        case 3: //MULT
                                reg[regNum] *= memory[address];
                                break;
                        case 8: //dIV
                                reg[regNum] /= memory[address];
                                break;
			case 4: //MoveR
				reg[regNum] = memory[address];
				break;
			case 5: //MoveM
				memory[address] = reg[regNum];
				break;
			case 6: //Comp;
				break;
			case 7://bc
				break;
			case 9://Read
				printf("\nEnter the number to read ");
				scanf("%d",&memory[address]);
				break;
			case 10://Print;
				printf("Print: %d",memory[address]);				

		}
	}
}

int main()
{
	int choice;
	char filename[20];
	do{
		printf("\n----------------- MENU ------------------\n");
		printf("\n\t1.Load (load first)\n\t2.Print\n\t3.Run\n\t4.Quit ");
		again: printf("\n\nSelect Your Choice: ");
		scanf("%d",&choice);
		switch(choice){
			case 1: //load
				printf("Enter the Simulator File name:  ");
				scanf("%s",filename);
				load(filename);
				break;
			case 2: //Print
				if(pc==0)
					printf("\nFirst load the Simulator file\n");
				else
					printSimulatorProgram();
                                break;
			case 3: //Run
				runSimulatorProgram();
                                break;
			case 4: //Quit
				printf("Thanks to use\n");
				break;
			default: printf("Invalid Choice\n");
				goto again;
		}
	}while(choice!=4);
}
