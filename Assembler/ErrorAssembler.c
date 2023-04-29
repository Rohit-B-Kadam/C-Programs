#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct mnt
{
	char mnemonic[6];
	char class[3];
	int opcode;
};

struct sym
{
	char symbol[20];
	int address;
	char defined;	
	char used;
};




struct mnt OPTAB[15] = { 
		    				{"STOP","IS",0},{"ADD","IS",1},{"SUB","IS",2},{"MULT","IS",3},{"MOVER","IS",4},{"MOVEM","IS",5},
		    				{"COMP","IS",6},{"BC","IS",7},{"DIV","IS",8},{"READ","IS",9},{"PRINT","IS",10},
		    				{"START","AD",1},{"END","AD",2},
			    			{"DC","DL",1},{"DS","DL",2}
						};


char REGTAB[][5] = {"","AREG","BREG","CREG","DREG"};

char CCTAB[][5] = {" ","LT","LE","EQ","GT","GE","ANY"};

struct sym SYMTAB[100];



//--------pointer-------------------//

int LC = 0;
int icptr = 0;
int symptr = 0;


//-----function define----------------//

void passOne(char* );
int checkInOptab(char* );
int checkInRegtab(char* );
int checkInCCtab(char* );
int checkSymbolExist(char* );
int usedSymbol(char* );
void definedSymbol(char* );
void definedLabel(char* );

void displayTable();
void checkSymbolError();

//--------------main-----------------//


int main()
{
    char filename[20];
    printf("Enter the File name: ");
    scanf("%s",filename);

    passOne(filename);
    displayTable();
    checkSymbolError();
	printf("\n");
	return 0;
}

//----------------PASS ONE ------------//

void passOne(char  fname[] )
{
    
    FILE *fp;
    fp=fopen(fname,"r");
    if(fp == NULL)
    {
        printf("ERROR: %s File can't open",fname);
        return;
    }
        	
	int n ,index;
	char tok1[10],tok2[10],tok3[10],tok4[10];

	char buffer[80];
	char ch[80];	
	
	while( fgets( buffer, 80 , fp )  )
	{

		n = sscanf(buffer,"%s %s %s %s",tok1,tok2,tok3,tok4);

		if(n == 1)                                     //END STOP
		{
            //no need n == 1
			index = checkInOptab(tok1);

			
				if((strcmp(tok1,"END")) == 0)		//END
				{
					printf("Pass one end");
				}
					else if(( strcmp(tok1,"STOP" )) == 0)  //STOP
				{
					//updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'\0',0);
					LC++;
				}


			
		}
		else if(n == 2 )
		{
			index = checkInOptab(tok1);
			

                if((strcmp(tok1,"START")) == 0)
				{
					LC = atoi(tok2);  //START tell us where the program is started  . LC is set at START  
					//updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'C',LC);
					
				}
				else if( ((strcmp(tok1,"READ")) == 0) || ((strcmp(tok1,"PRINT")) == 0) || ( (strcmp(tok1 , "END")) == 0))
				{
					int loc = usedSymbol(tok2);
					//updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'S',loc);
					LC = LC + 1; 
				}



		}
		else if(n ==3 )
		{
			int regcode = -1;
			index = checkInOptab(tok1);
            if( index != -1)	 	//tok1 is menomic   eg MULT AREG NUM   or BC LT LOOP
			{

				if( (checkInRegtab(tok2)) != -1)    
					regcode = checkInRegtab(tok2);
				if( (checkInCCtab(tok2)) != -1)
                    regcode = checkInCCtab(tok2); 		
				

					int loc = usedSymbol(tok3);
					//updateIctab(OPTAB[index].class , OPTAB[index].opcode,regcode,'S',loc);
					LC = LC + 1;
				
			}
			else  // tok1 is Label    LOOP PRINT NUM   or NUM DS 1
			{

				index = checkInOptab(tok2);
            
					int value = 0 ;
			
					if( strcmp( OPTAB[index].class , "DL" ) == 0)
					{
						value = atoi(tok3);  //atoi means string to integer
						definedSymbol(tok1);
						//updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'C',value);
					
					
						if( strcmp( OPTAB[index].mnemonic , "DS" ) == 0)
							LC = LC + value;
						else
							LC = LC + 1;

					}
					else if(strcmp( OPTAB[index].class , "IS" ) == 0)
					{
						definedSymbol(tok1);
						int loc = usedSymbol(tok3);
						//updateIctab(OPTAB[index].class , OPTAB[index].opcode,0,'S',loc);
						LC = LC + 1;

					}
					
			}

		}
		else if(n == 4)
		{
			int regcode = -1;
			definedSymbol(tok1);
			index = checkInOptab(tok2);
			
                if( (checkInRegtab(tok3)) != -1)
                        regcode = checkInRegtab(tok3);
        	   
			    if( (checkInCCtab(tok3)) != -1)
                        regcode = checkInCCtab(tok3);                
                        int loc = usedSymbol(tok4);
                        //updateIctab(OPTAB[index].class , OPTAB[index].opcode,regcode,'S',loc);
                        LC = LC + 1;
			
		}
			
	}
	
}


void displayTable()
{
	 printf("\n\n-----------Symbolic table---------------\n");

        printf("\nSYMBOL \t Address \t Defined \t Used \n");

        int i = 0;

        for( i = 0 ; i < symptr ; i++  )
        {
                printf("%s \t %d  \t\t %c \t\t %c \n",SYMTAB[i].symbol,SYMTAB[i].address,SYMTAB[i].defined,SYMTAB[i].used);
        }

}


//------------check condition-----------------//



int checkInOptab(char search[] )
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
        for(index = 1  ; index < 5 ; index++ )
        {
                if(  strcmp(REGTAB[index] , search) == 0 )
                        return index;
        }
        return -1;
}

int checkInCCtab(char* search)
{
        int index;
        for(index = 1  ; index <= 6 ; index++ )
        {
                if(  strcmp(CCTAB[index] , search)  == 0)
                        return index;
        }
        return -1;
}


//-------------SYMBOL TABLE FUNCTION----------------//

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
        if(SYMTAB[loc].defined == 'Y')
            printf("ERROR: %s redeclared\n",tok);
        else
        {
			SYMTAB[loc].defined = 'Y';
			SYMTAB[loc].address = LC;
        }
    }


}

//--------CHECK ERROR ---------//

void checkSymbolError()
{
    int i;
    for( i = 0 ; i < symptr ; i++)
    {
        if(SYMTAB[i].used != 'Y')
            printf("ERROR: %s is not used\n",SYMTAB[i].symbol);
        
        if(SYMTAB[i].defined != 'Y')
            printf("ERROR: %s is not defined\n",SYMTAB[i].symbol);
    }
}


//-------------END---------------//