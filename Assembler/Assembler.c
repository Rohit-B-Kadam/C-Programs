#include<stdio.h>
#include<string.h>

struct mnt
{
	char mnemonic[6];
	char class[3];
	int opcode;
};

struct mnt OPTAB[15] = { 
		{"STOP","IS",0},{"ADD","IS",1},{"SUB","IS",2},{"MULT","IS",3},{"MOVER","IS",4},{"MOVEM","IS",5},
		{"COMP","IS",6},{"BC","IS",7},{"DIV","IS",8},{"READ","IS",9},{"PRINT","IS",10},
		{"START","AD",1},{"END","AD",2},
		{"DC","DL",1},{"DS","DL",2}
	};


char REGTAB[][5] = {" ","AREG","BREG","CREG","DREG"};

char CCTAB[][5] =  {" ","LT","LE","EQ","GT","GE","ANY"};

struct sym
{
	char symbol[20];
	int address;
	char defined;	
	char used;
}SYMTAB[100];

struct iccode
{
	int pc;
	char mncls[3];
	int code;
	int reg;
	char stype;
	int value;
}ICTAB[200];

struct error
{
	int lineno;
	int errno;
}ERRTAB[100];

char *errmsg[7] = 	{	"INVALID MNEMONIC" ,
					 	"SYNTAX ERROR" ,
						"SAME LABEL IS POINT AT TWO DIFFERENT LOCATION" ,
						"DUPLICATE SYMBOL","UNDEFINE SYMBOL",
						"WARNING SYSMBOL IS DEFINED BUT NO USED",
						"INVALID ASSEMBLER STATEMENT"
					};

unsigned int binary[200];

//--------pointer-------------------//

int LC = 0;
int icptr = 0;
int symptr = 0;
int errptr = 0;

//-----function define----------------//

void passOne(char* );
void separateProcessToken(FILE*);
int checkInOptab(char* );
int checkInRegtab(char* );
int checkInCCtab(char* );
int checkSymbolExist(char* );
int usedSymbol(char* );
void definedSymbol(char* );
void definedLabel(char* );
void error(int errno );
void updateIctab( char* , int  , int  , char , int );
void displayTable();

void passTwo();


//--------------main-----------------//


int main(int argc,char *argv[])
{
	if(argc==2)
	{
		passOne(argv[1]);
		if( errptr == 0 )
			passTwo(argv[1]);
	}
	else
	{
		printf("Pass file name as a command line");
	}
	printf("\n");
	return 0;
}

//----------------PASS ONE ------------//

void passOne(char* fname)
{



        FILE *fp;
        fp=fopen(fname,"r");
        if(fp==NULL)
        {
                printf("ERROR: %s File can't open",fname);
        }
        else
        {
                separateProcessToken(fp);
     	}	
	
	displayTable();
}

void passTwo(char* fname)
{
	FILE *fp;
	int  i = 0;
	int count=0;
	
	for(i = 0 ; fname[i] != '.' ; i++,count++);
	
	//fname[i] = '\0';
	
	
	for(i = count ; i < strlen(fname) ; i++)
	{
			fname[i] = '\0';
	}

	
	strcat(fname,".sm");
	
	fp = fopen(fname,"w");
	
	printf("\n\nMachine Language: \n");
	int point ;
	for(i = 1 ; i < icptr ; i++)
	{
		
		if(! strcmp(ICTAB[i].mncls,"DL")== 0 )
		{	
			binary[i] = ICTAB[i].code;
			binary[i] *= 10;
			binary[i] += ICTAB[i].reg;
			binary[i] *= 1000;
			
			int sysValue;
		
			if(ICTAB[i].stype == 'S')
				sysValue = SYMTAB[ ICTAB[i].value ].address;
			else
				sysValue = 0;
			
			binary[i] += sysValue;
			
			printf("%d %d\n",ICTAB[i].pc,binary[i]);
			fprintf(fp,"%d %d\n",ICTAB[i].pc,binary[i]);

			
		}
		else
		{
			//binary[i] = 0;
			//printf("%d %d\n",ICTAB[i].pc,binary[i]);

		}
	}
	int neg = -1;	
	printf("%d %d",neg,ICTAB[0].value);
	fprintf(fp,"%d %d \n",neg,ICTAB[0].value);
	return;	
}

void displayTable()
{
	 printf("\n\n-----------Symbolic table---------------\n");

        printf("\nSYMBOL \t Address \t Defined \t Used \n");

        int i = 0;

        for( i = 0 ; i < symptr ; i++  )
        {
                printf("%s \t %d  \t %c \t %c \n",SYMTAB[i].symbol,SYMTAB[i].address,SYMTAB[i].defined,SYMTAB[i].used);
        }

        printf("\n\n--Intermediate code ---------------\n\n");

        for( i = 0 ; i < icptr ; i++  )
        {
                printf("%d \t ( %s , %d )",ICTAB[i].pc,ICTAB[i].mncls,ICTAB[i].code);
		if(ICTAB[i].reg != 0)
			printf(" (%d) ",ICTAB[i].reg);
		else
			printf("     ");
		printf("( %c , %d )\n",ICTAB[i].stype,ICTAB[i].value);
        }

	printf("\n\nERROR LIST------------\n");
	printf("\nLine no \t Error detail");
	for( i=0 ; i < errptr ; i++)
	{
		printf("\n%d \t %s",ERRTAB[i].lineno,errmsg[ERRTAB[i].errno]);
	}

}

void separateProcessToken(FILE * fp)
{
	int n = 1,index;
	char tok1[10],tok2[10],tok3[10],tok4[10],tok5[10];

	int read,len;
	char * line = NULL;
	char ch[80];	
	
	while( (read = getline(&line, &len, fp)) != -1)
	{

		strcpy(tok1,"\0");
                strcpy(tok2,"\0");
                strcpy(tok3,"\0");
                strcpy(tok4,"\0");

		n = sscanf(line,"%s %s %s %s %s",tok1,tok2,tok3,tok4,tok5);
		//printf(" %s %s %s %s \n",s1,s2,s3,s4 );
		
		if(n == 1)                                     // END STOP
		{
			index = checkInOptab(tok1);
			if( index == -1)
			{
				error(0);
			}
			else
			{
				if((strcmp(tok1,"END")) == 0)
				{
					printf("Pass one end ");
					//passtwo();
				}
				else if(( strcmp(tok1,"STOP" )) == 0)
				{
					
                 			updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'\0',0);
					LC++;
				}
				else
				{
					error(1);
				}
			
			}
			
		}
		else if(n == 2 )
		{
			index = checkInOptab(tok1);
			if( index != -1)
                {
				    if((strcmp(tok1,"START")) == 0)
				{
					LC = atoi(tok2);  //tok2 must integer handle error
					updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'C',LC);
					
				}
				else if( ((strcmp(tok1,"READ")) == 0) || ((strcmp(tok1,"PRINT")) == 0))
				{
					int loc = usedSymbol(tok2);
					updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'S',loc);
					LC = LC + 1; 
				}
				else
				{
					error(1);
				}

                        }
			else
			{
				error(0);
			}

		}
		else if(n ==3 )
		{
			int regcode = -1;
			index = checkInOptab(tok1);
             if( index != -1)
			{
				if( (checkInRegtab(tok2)) != -1)
					regcode = checkInRegtab(tok2);
				if( (checkInCCtab(tok2)) != -1)
                                        regcode = checkInCCtab(tok2); 		
				
				if(regcode != -1)	
				{
					int loc = usedSymbol(tok3);
					updateIctab(OPTAB[index].class , OPTAB[index].opcode,regcode,'S',loc);
					LC = LC + 1;
				}
				else
				{
					error(1);
				}
			}
			else
			{
				index = checkInOptab(tok2);
                       		if( index != -1)
                        	{
					int value = 0 ;
			
					if( strcmp( OPTAB[index].class , "DL" ) == 0)
					{
						value = atoi(tok3) ;
						definedSymbol(tok1);
						updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'C',value);
					
					
						if( strcmp( OPTAB[index].mnemonic , "DS" ) == 0)
							LC = LC + value;
						else
							LC = LC + 1;
					}
					else if(strcmp( OPTAB[index].class , "IS" ) == 0)
					{
						definedLabel(tok1);
						int loc = usedSymbol(tok3);
						updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'S',loc);
						LC = LC + 1;
					}
					else
					{
						error(0);
					}
				}

			}

		}
		else if(n == 4)
		{
			int regcode = -1;
			definedLabel(tok1);
			index = checkInOptab(tok2);
			if( index != -1)
            {
                if( (checkInRegtab(tok3)) != -1)
                        regcode = checkInRegtab(tok3);
        	   
			    if( (checkInCCtab(tok3)) != -1)
                        regcode = checkInCCtab(tok3);

                if(regcode != -1)
                {
                        int loc = usedSymbol(tok4);
                        updateIctab(OPTAB[index].class , OPTAB[index].opcode,regcode,'S',loc);
                        LC = LC + 1;
                }
        	    else
                {
                        error(1);
               }
			}
		}
		else
		{
			error(6);
		}	
	}
}



//------------check condition-----------------//



int checkInOptab(char* search)
{
	int index;
	for(index = 0  ; index < 15 ; index++ )
	{
		if(  strcmp(OPTAB[index].mnemonic , search) == 0  )
			return index;
	}
	return -1;
}

int checkInRegtab(char* search)
{
        int index;
        for(index = 1  ; index <= 4 ; index++ )
        {
                if(  strcmp(REGTAB[index] , search) == 0 )
                        return index;
        }
        return -1;
}

int checkInCCtab(char* search)
{
        int index;
        for(index = 1  ; index <= 5 ; index++ )
        {
                if(  strcmp(CCTAB[index] , search)  == 0)
                        return index;
        }
        return -1;
}

int checkSymbolExist(char* search)
{
	int index;
        for(index = 0  ; index < symptr ; index++ )
        {
                if(  strcmp(SYMTAB[index].symbol , search) == 0  )
                        return index;
        }
        return -1;

}

//-------------SYMBOL TABLE FUNCTION----------------//

int usedSymbol(char* tok)
{
	int loc = checkSymbolExist(tok);
	if(loc == -1)
	{
		strcpy(SYMTAB[symptr].symbol,tok);
		SYMTAB[symptr].used = 'Y';
		loc = symptr;
		symptr++;
	}
	else
		SYMTAB[loc].used = 'Y';

	return loc;
}

void definedSymbol(char* tok)
{
	int loc = checkSymbolExist(tok);
        if(loc == -1)
        {
                strcpy(SYMTAB[symptr].symbol,tok);
                SYMTAB[symptr].defined = 'Y';
		SYMTAB[symptr].address = LC;
                symptr++;
        }
	else
	{
		SYMTAB[loc].defined = 'Y';
		//--- check if address exist
                SYMTAB[loc].address = LC;

	}

}

void definedLabel(char* tok)
{
	int loc = checkSymbolExist(tok);
        if(loc == -1)
        {
                strcpy(SYMTAB[symptr].symbol,tok);
                SYMTAB[symptr].defined = 'Y';
                SYMTAB[symptr].address = LC;
                symptr++;
        }
	else
	{
		//error(lc,02);
	}

}

//-------------ERROR---------------//


void error(int errno )
{
	updateIctab("\0" ,-1,0,'\0',-1);
	ERRTAB[errptr].lineno = LC;
	ERRTAB[errptr].errno = errno;
	errptr++;
	LC++;
}

//------------UPDATE IC TABLE----------//

void updateIctab( char* class, int code , int reg , char stype, int value)
{
	strcpy(ICTAB[icptr].mncls , class);
	ICTAB[icptr].pc = LC;
	ICTAB[icptr].code = code;
	ICTAB[icptr].reg = reg;
	ICTAB[icptr].stype = stype;
	ICTAB[icptr].value = value;
	
	icptr = icptr + 1;
}


//-------------END---------------//

