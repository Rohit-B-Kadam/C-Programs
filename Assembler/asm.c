#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct mnt
{
	char mnemonic[6];
	char class[5];
	int opcode;
}

struct sym
{
	
	char symbol[5];
	char used;
	char define;
	int address;
}

struct iccode
{
	int pc;
	char class[5];
	int code;
	char reg;
	char stype;
	int value;
}

struct mnt Optab[15] = {
				{"STOP","IS",0},{"ADD","IS",1},{"SUB","IS",2},{"MULT","IS",3},{"MOVER","IS",4},{"MOVEM","IS",5},{"COMP","IS",6},{"BC","IS",7},{"DIV","IS",8},{"READ","IS",9},
				{"PRINT","IS",10},{"START","AD",1},{"END","AD",2},{"DC","DL",1},{"DS","AD",2}
			};


char Regtab[][5]={"","AREG","BREG","CREG","DREG"};

char Cctab[][7]={"","LT","LE","EQ","GT","GE","ANY"};

struct sym Symtab[100];

struct iccode Ictab[200];

int lc=0;
int Symptr=0;
int Icptr=0;

int main()
{
	printf("Enter file name: ");
	scanf("%s",filename);

	
}

int checkInOptab(char search[])
{	
	int index;
	for(index=0;index<=15;i++)
	{
		if(strcmp(Optab[index].class,search)==0)
		return index;
	}
	return -1;
}

int checkInCctab(char search[])
{
	for(index=0;index<7;index++)
	{
		if(strcmp(Cctab[index],search)==0)
		return index;
	}
	return -1;
}

int checkInRegtab(char search[])
{
	for(index=0;index<=5;index++)
	{
		if(strcmp(Regtab[index],sraech)==0)
		{
			return index;
		}
		return -1;
	}
}


int checkSymbolExist(char search[])
{
	int index;
	for(index=0;index<Symptr;index++)
	{
		if(strcmp(Symtab[index].symbol,search)==0)
		{
			return index;
		}
		return -1;
	}
}

int usedSymbol(char tok[])
{
	int loc=checkSymbolExist(tok);
	if(loc==-1)
	{
		strcpy(Symtab[Symptr].symbol,tok);
		Symtab[Symptr].used = 'Y';
		loc=Symptr;
		Symptr++;
	}
	else
	Symtab[Symptr].used = 'Y';
	return -1;
