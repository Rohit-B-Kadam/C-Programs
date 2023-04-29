#include<stdio.h>
#include<string.h>

struct DFA
{

	int no_of_states;
	int no_of_symbols;
	int no_of_final_states;
	char symbols[10];
	int delta[10][10]; //transition table
	int final[10];     //array of final states

}system;

void accept(){
	
	int i,j,n;
	
	printf("\n\nEnter the no. of States: ");
	scanf("%d",&system.no_of_states);
	
	
	printf("\nEnter the no. of symbols: ");
	scanf("%d",&system.no_of_symbols);
	
	
	printf("\nEnter the %d Symbols: \n",system.no_of_symbols);
	for(i = 0 ; i < system.no_of_symbols ; i++)
	{
		printf("Enter %d symbols : ",i+1);
		scanf(" %c",&system.symbols[i]);
	}

	
	printf("\nEnter the transition table: \n");
	for(i = 0; i < system.no_of_states ; i++)
		for(j= 0 ; j < system.no_of_symbols ; j++)
		{
			printf("\nd( q%d , %c ) = ",i,system.symbols[j]);
			scanf("%d",&system.delta[i][j]);
		}
	

	printf("\nEnter the number of final: ");
	scanf("%d",&system.no_of_final_states);
	
	for( i = 0 ; i < system.no_of_final_states ; i++)
	{
		printf("%d Final state ",i+1);
		scanf(" %d",&system.final[i]);	
	}
	
	

}

void display(){
	int i,j;

	printf("FIVE TUPLE {Q,E,d,S,F}\n\n");
	
	printf("\n\tQ => { ");
	for(i = 0 ; i < system.no_of_states ; i++)
		printf(" q%d, ",i);
	printf("}\n");

	
	printf("\n\tE => { ");
	for(i = 0 ; i < system.no_of_symbols ; i++)
        {
                printf(" %c, ",system.symbols[i]);
        }
	printf(" }\n");
	
	 printf("\n\tS => { q0 } \n");

	printf("\n\tF => { ");
	for( i = 0 ; i < system.no_of_final_states ; i++)
        {
                printf(" q%d ,",system.final[i]);
        }
	 printf("}\n");

		
	printf("\n\td =>\n\t\t ");
	for(i = 0 ; i < system.no_of_symbols ; i++)
        {
                printf("%c\t",system.symbols[i]);
        }
	printf("\n");
        for(i = 0; i < system.no_of_states ; i++)
	{
		printf("\tq%d |",i);
                for(j= 0 ; j < system.no_of_symbols ; j++)
                {
                        printf("\t%d",system.delta[i][j]);
                }
		printf("\n");
	}
	 printf("}\n");

}

void execute(){
	
	char string[20];
	int encode[20];
	int i,j;
	
	printf("Enter the string: ");
	scanf(" %s",&string);
	
	int flag = 0;
	
	for(i = 0 ; i < strlen(string) ; i++)
	{
		flag=0;
		for(j = 0; j < system.no_of_symbols ; j++ )
		{
			if(string[i] == system.symbols[j])
			{
				encode[i] = j;
				flag = 1;
			}
		}
		if(flag == 0)
		{
			printf("User enter invalid String");
			return;
		}
	}
	
	int currentState = 0;
	
	for(i = 0 ; i < strlen(string) ; i++)
	{
		currentState = system.delta[currentState][encode[i]];
	}
	
	for( i = 0 ; i < system.no_of_final_states ; i++)
        {
                if(system.final[i] == currentState )
			printf("\n\nValid String");
		else
			printf("\n\nInvalid String");
        }

	

}

int main()
{
	int choice = 0;
	do{
		printf("\n----------Menu--------");
		printf("\n\t1.Accept\n\t2.Display\n\t3.Execute\n\t4.Exit");
		printf("\n\nSelect your choice(first time select Accept ): ");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1: accept();
				break;
			case 2: display();
				break;
			case 3: execute();
				break;
			case 4 :printf("\n\n---------------END---------------\n\n");
				break;
			default: printf("Wrong Choice");
		}
	}while(choice != 4);
		
	return 0;	
}
