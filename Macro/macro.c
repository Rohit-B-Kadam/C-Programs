#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char mnemonic[15][8] = {"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT","START","END","DC","DS"};
char REGTAB[][5] = {" ","AREG","BREG","CREG","DREG"};

struct MNT
{
	char mname[20];
	int ppcnt,kpcnt,kptp,mdtp;
}mnt[5];

struct MDT
{
	char opcode[20], op1[20],op2[20];
}mdt[15];

struct KPT
{
	char kname[30];
	char def[30];
}kpt[40];

int mntptr=0,mdtptr=0,kptptr=0,pntptr=0,aptptr=0,m;
char pnt[10][10],apt[10][10];
char sfile[30];
char buffer [80],t1[40],t2[40],t3[40],t4[40],str[80];
FILE *fp;

//Print Table
void printMNT()
{
	printf("\n\n MACRO NAME TABLE\n");
	printf("-----------------------------------------------------");
	printf("\nM_NO \tMacro Name \t*pp \t*kp \tMDPT \tKPDTP\n");
	printf("-----------------------------------------------------");
	for(m=0;m<mntptr;m++)
	{
		printf("\n %d \t%s \t\t%d \t%d \t%d \t%d",m,mnt[m].mname,mnt[m].ppcnt,mnt[m].kpcnt,mnt[m].mdtp,mnt[m].kptp);
	}
}

void printKPT()
{
	printf("\n\nKEYWORD PARAMETER DEFAULT TABLE\n");
	printf("-----------------------------------------------------");
	for(m=0;m<kptptr;m++)
	{
		printf("\n%s \t%s",kpt[m].kname,kpt[m].def);
	}
	printf("\n");
}

void printPNT()
{
	printf("\n\nPARAMETER NAME TABLE\n");
	printf("-----------------------------------------------------");
	for(m=0;m<pntptr;m++)
	{
		printf("\n%s",pnt[m]);
	}
}

void printMDT()
{
        printf("\n\n MACRO Definition TABLE\n");
        printf("-----------------------------------------------------");
        for(m=0;m<mdtptr;m++)
        {
		if(strcmp(mdt[m].opcode,"MEND" ) !=0 )
                	printf("\n %d \t%s \t\t%s \t%s\n",m,mdt[m].opcode,mdt[m].op1,mdt[m].op2);
		else
			printf("\n %d \t%s\n",m,mdt[m].opcode);
        }
}

void printAPT()
{
	printf("\n\nACTUAL NAME TABLE\n");
	printf("-----------------------------------------------------");
	for(m=0;m<aptptr;m++)
	{
		printf("\n%s",apt[m]);
	}	
}


//Check function
int checkInMnt(char* search)
{
	int index;
	for(index = 0  ; index < 15 ; index++ )
	{
		if(  strcmp(mnemonic[index], search) == 0  )
			return 1;
	}
	return -1;
}

int checkInReg(char* search)
{
        int index;
        for(index = 1  ; index <= 4 ; index++ )
        {
                if(  strcmp(REGTAB[index] , search) == 0 )
                        return 1;
        }
        return -1;
}


int searchpnt(char *s)
{
	for(m=0;m<pntptr;m++)
	{

		if(strcmp(pnt[m],s)==0)
		{
			return m;
		}
	}
	return -1;
}

int searchMacro(char *s)
{
	for(m=0; m < mntptr ; m++)
	{
		if(strcmp(mnt[m].mname,s) == 0)
		{
			return m;		
		}	
	}
	return -1;
}

//Process
void make_kpt_pnt(char *s)
{
	int i=0,j=0,k=0;
	char temp[10];

	// FOR POSITIONAL PARAMETER
	strcat(s,",");
	while(*s && *s!='=')
	{
		if(*s==',')
		{
			temp[i]='\0';
			j++;
			i=0;
			k=searchpnt(temp);
			if(k==-1)
			{
				strcpy(pnt[pntptr++],temp);
			}
	   		else
			{
				printf("\n ERROR :multiple declaration of symbol %s",temp);
				exit(0);
			}
		}
		else if(*s!='&')
			temp[i++]=*s;
		s++;
	}//while
	mnt[mntptr].ppcnt=j;
	j=0;

	//for default parameter
	while(*s)
	{
		if(*s=='=')
		{
			temp[i]='\0';
			i=0;
			k=searchpnt(temp);
			if(k==-1)
			{
				strcpy(pnt[pntptr++],temp);
				strcpy(kpt[kptptr].kname,temp);
			}
			else
			{
				printf("\n ERROR :multiple declaration of symbol %s",temp);
				exit(0);
			}
		}
		else if(*s==',')
		{
			temp[i]='\0';
			j++;
			i=0;
			strcpy(kpt[kptptr++].def,temp);
		}
		else if(*s!='&')
			temp[i++]=*s;
		s++;
	}//while
	mnt[mntptr++].kpcnt=j;
}


void addMacro(FILE *fa,int mIndex)
{
	int point;
	int pc = mnt[mIndex].mdtp;
	
	char amTemp[40],ctemp[40];

	while(strcmp(mdt[pc].opcode,"MEND") != 0)
	{
		//opcode
		strcpy(amTemp,mdt[pc].opcode);
		if(checkInMnt(amTemp) == 1)
			fprintf(fa,"%s ",amTemp);
		else
		{
			sscanf((amTemp+3),"%[^)]",ctemp);
			point = atoi(ctemp);
			if(mIndex == 1)
				point -= 3;
			strcpy(ctemp,apt[point]);
			fprintf(fa,"%s ",ctemp);		
		}
		//reg
		strcpy(amTemp,mdt[pc].op1);
		if(checkInReg(amTemp) == 1)
			fprintf(fa,"%s ",amTemp);
		else
		{
			sscanf((amTemp+3),"%[^)]",ctemp);
			point = atoi(ctemp);
			if(mIndex == 1)
				point -= 3;
			strcpy(ctemp,apt[point]);
			fprintf(fa,"%s ",ctemp);		
		}
		
		//operand
		strcpy(amTemp,mdt[pc].op2);
		sscanf((amTemp+3),"%[^)]",ctemp);
		point = atoi(ctemp);
		if(mIndex == 1)
			point -= 3;
		strcpy(ctemp,apt[point]);
		fprintf(fa,"%s\n",ctemp);		
		pc++;
	}
}

//Actual Parameter
void make_apt(FILE *fa,char *s , int mIndex)
{
	int i=0,j=0,k=0;
	char temp[10];

	// FOR POSITIONAL PARAMETER
	strcat(s,",");
	while(*s && *s!='=')
	{
		if(*s==',')
		{
			temp[i]='\0';
			j++;
			i=0;
			strcpy(apt[aptptr++],temp);
		}
		else
			temp[i++]=*s;
		s++;
	}//while
	j=0;
	
	int icnt;
	for(icnt = 0 ; icnt < mnt[mIndex].kpcnt ; icnt++ )
	{
			strcpy(apt[aptptr+icnt],kpt[mnt[mIndex].kptp+ icnt].def);
			
	}
	char L[40],R[40];
	//for default parameter
	while(*s)
	{
		if(*s=='=')
		{
			temp[i]='\0';
			i=0;
			strcpy(L,temp);
		}
		else if(*s==',')
		{
			temp[i]='\0';
			j++;
			i=0;
			strcpy(R,temp);
			
			for(icnt = 0 ; icnt < mnt[mIndex].kpcnt ; icnt++)
			{
				if(strcmp(kpt[icnt+mnt[mIndex].kptp].kname,L) == 0)
				{
					strcpy(apt[aptptr+icnt],R);		
				}
			}
			
		}
		else if(*s!='&')
			temp[i++]=*s;
		s++;
		

	}//while
	aptptr += mnt[mIndex].kpcnt ;
	printAPT();
	addMacro(fa,mIndex);
	aptptr = 0;	
	}

void createMDT(FILE* fp)
{
	char copy[40];
	int n;
	char temp1[20];
	int index;
	char tok1[40],tok2[40],tok3[40];
	mnt[mntptr].mdtp = mdtptr;
	printf("In createMDt");
	while(fgets(buffer,80,fp) )
        {
                n=sscanf(buffer,"%s %s %s",tok1,tok2,tok3);
		if(strcmp(tok1,"MEND") != 0)
		{
			
			if(checkInMnt(tok1) != -1)
			{
				strcpy(mdt[mdtptr].opcode,tok1);
			}
			else if(checkInMnt(tok1) == -1) 
			{
				strcpy(copy,tok1+1);
				if((index = searchpnt(copy)) != -1 )
				{
					sprintf(temp1,"(P,%d)",index);
					strcpy(mdt[mdtptr].opcode,temp1);	
				}
			}
			if(checkInReg(tok2) != -1)
                        {
                                strcpy(mdt[mdtptr].op1,tok2);
                        }
                        else
                        {
				strcpy(copy,tok2+1);
                                if((index = searchpnt(copy)) >=0 )
                                {
             				 sprintf(temp1,"(P,%d)",index);
                                        strcpy(mdt[mdtptr].op1, temp1) ;
                                }       
                        }
			strcpy(copy,tok3+1);
			if((index = searchpnt(copy)) != -1 )
                        {
                               	 sprintf(temp1,"(P,%d)",index);
	                          strcpy(mdt[mdtptr].op2, temp1);
			}					
		}
		else if( strcmp(tok1,"MEND") == 0 )
		{
			strcpy(mdt[mdtptr].opcode,tok1);
			mdtptr++;
			return;
		}	
		mdtptr++;
	}
}

void separate()
{
	int n;
	fp=fopen(sfile,"r");
	while(fgets(buffer,80,fp))
	{
		n=sscanf(buffer,"%s,%s,%s",t1,t2,t3);
		if(strcmp(t1,"MACRO")==0 && n==1)
		{
			fgets(buffer,80,fp);
			sscanf(buffer,"%s %s",t1,t2);
			printf("\nMacro prototype statement\n");
			printf("-------------------------------------------------------");
			printf("\nToken_1 => %s \nToken_2 => %s",t1,t2);
			strcpy(mnt[mntptr].mname,t1);
			mnt[mntptr].mdtp=mdtptr;
			mnt[mntptr].kptp=kptptr;
			make_kpt_pnt(t2);
			createMDT(fp);
		}
	}
	fclose(fp);
}

void display(char *f)
{
	fp=fopen(f,"r");
	if(fp==NULL)
	{
		printf("\nFile does not Exist");
		exit(0);
	}
	else
	{
		while(!feof(fp))
		{
			fgets(str,80,fp);
			printf("%s",str);
		}
	}
	fclose(fp);
}

//   MACRO CALL EXPANSION
void macroCallExpansion()
{
	int n;
	char smfile[40];
	FILE *fm,*fa;
	int index;
	
	printf("Enter the file name which contain marco call: ");
	scanf("%s",smfile);	
		
	fm = fopen(smfile,"r");
	fa = fopen("AssemblyWithoutMacro.asm","w");
	while(fgets(buffer,80,fm))
	{
		n=sscanf(buffer,"%s %s %s",t1,t2,t3);
		if( (index = searchMacro(t1)) != -1 )
		{
			make_apt(fa,t2,index);	
		}
		else
			fprintf(fa,"%s",buffer);
	}
	fclose(fm);
	fclose(fa);
}

void displayTable()
{
	printMNT();
        printPNT();
        printKPT();
	printMDT();
}

void processMacroDefinition()
{
	display(sfile);
	printf("\n\n");
        separate();
}


main(int args,char *argv[])
{
	if(args<2)
	{
		printf("\nEnter File Name :");
		scanf("%s",sfile);
	}
	else
	{
		strcpy(sfile,argv[1]);
	}
	printf("\nAssembly with macro\n");
	processMacroDefinition();
	displayTable();
	macroCallExpansion();
	printf("\n\nAssembly with out macro\n\n");
	display("AssemblyWithoutMacro.asm");
}


