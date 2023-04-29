#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structure of each node

typedef struct node
{
    char line[80];            //to store each line of file
    struct node *next;        //pointing to next node(line)
}NODE;


NODE * head = NULL;             //pointer to point start of linklist
                                //must set NULL

char filename[20];              

void insert( char buffer[] , int position )
{

    //First Create newnode and fill the node
    NODE *newnode;
    newnode = (NODE *) malloc (sizeof(NODE));
    newnode->next = NULL;      //must set NULL 
    strcpy(newnode->line , buffer );

    //Set the newnode at given Position

    if( position == 1 )
    {
        newnode->next = head;   // 1
        head = newnode;         // 2
    }
    else
    {

            NODE * temp = head;  //temp is pointing to start of link list

            //traverse the link list to the position - 1
            //ie move the temp to the node before the insert position
            for( int i = 1 ; i < position - 1 ; i++ )       // here every  i is starting from 1
            {
                temp = temp->next;
            }

            //now insert the newnode
            newnode->next = temp->next;     //1
            temp->next = newnode;           //2

    }

}


void delete( int position)
{

    NODE    *del;       //to stored the address of Delete node

    if( position == 1)
    {
        del = head;
        head = head->next;
        free(del);
    }
    else
    {
        NODE *temp = head;
        
        // 1. Move the temp to  ( position - 1 ) position
        for( int i = 1 ; i < position - 1 ; i++ )
        {
            temp = temp->next;
        }
        // 2.
         del = temp->next;
        // 3.
         temp->next = del->next;
         free(del);

    }



}

NODE * getLineNode( int position )
{
    NODE *temp = head;

    for( int i = 1 ; i < position ; i++)    //send the node at that postion not before
    {
        temp = temp->next;
    }

    return temp;
}

int lineCount()
{
    int count = 0;
    NODE * temp = head;
    while (temp!=NULL)
    {
        count++;
        temp = temp->next;
    }
    return count;
}


void create()
{
    FILE *fp;
    char buffer[80];
    int position = 1 ;              //to give position to insert function 
    fp = fopen(filename , "r+");

    while( fgets(buffer , 80 , fp ) )
    {
            insert( buffer , position );
            position++;
    }

    fclose(fp);
}


void append( char buffer[] )
{
    int count;
    count = lineCount();
    insert(buffer , count + 1);
}

void deleteLines( int n1 , int n2)
{
    for( int i = n1 ; i <= n2 ; i++)
        delete(i);
}


void save()
{
    FILE *fp;

    fp = fopen(filename , "w+");
    
    int count;
    count = lineCount();
    NODE *temp;

    for(int i = 1 ;  i <= count  ; i++ )
    {
        temp = getLineNode(i);
        fprintf(fp,"%s",temp->line);
    }

    fclose(fp);

}

void printAll()
{
    int count;
    count = lineCount();
    NODE *temp;

    printf("Data is : \n");
    for(int i = 1 ;  i <= count  ; i++ )
    {
        temp = getLineNode(i);
        printf("%s",temp->line);
    }


}

void print( int n1 , int n2)
{
    NODE * temp;
    printf("Line between %d and %d : \n",n1,n2);
    for(int i = n1 ;  i <= n2  ; i++ )
    {
        temp = getLineNode(i);
        printf("%s",temp->line);
    }
    
}

void move( int n1 , int n2)
{
    /*
        1. copy the line of given node position(n1) into the buffer
        2. delete (n1)
        3 insert (buffer , n2)
    */

    //1
    char buffer[80];
    NODE * temp;
    temp = getLineNode(n1);
    strcpy(buffer , temp->line );

    //2
    delete(n1);

    //3
    insert(buffer , n2);
}

void copy( int n1 , int n2)
{
    /*
        1. copy the line of given node position(n1) into the buffer
        2 insert (buffer , n2)
    */

    //1
    char buffer[80];
    NODE * temp;
    temp = getLineNode(n1);
    strcpy(buffer , temp->line );

    //2
    insert(buffer , n2);
}

int main(int argc , char* argv[] )
{
    char buffer[80];
    char cmd[80];
    int n1;
    int n2;

    if(argc == 2)
        strcpy(filename , argv[1]);
    else
        strcpy(filename , "temp.txt");

    create(); // create the linklist

    printf("-------------------Command------\n");
    printf("Append        ::   append\n");
    printf("Insert        ::   insert n\n");
    printf("Delete        ::   delete n\n");
    printf("DeleteLines   ::   deletelines n1 n2\n");
    printf("Move          ::   move n1 n2\n");
    printf("Copy          ::   copy n1 n2\n");
    printf("printAll      ::   printAll\n");
    printf("Print         ::   print n1 n2\n");
    printf("Save          ::   save\n");
    printf("Exit          ::   exit");
    printf("\n");
    while(1)
    {
        fflush(stdin);

        printf("\n\n$ ");
        fgets( buffer , 80 , stdin);

        sscanf(buffer , "%s%d%d",cmd , &n1 , &n2);

        if( strcmp( cmd , "append" ) == 0 )
        {
            fgets(buffer , 80 , stdin);
            append(buffer);
        }
        else if( strcmp( cmd , "insert" ) == 0 )
        {
            fgets(buffer , 80 , stdin);
            insert(buffer , n1);
        }
        else if( strcmp( cmd , "delete" ) == 0 )
        {
            delete(n1);
        }
        else if( strcmp( cmd , "deleteLines" ) == 0 )
        {
            deleteLines(n1 , n2);
        }
        else if( strcmp( cmd , "move" ) == 0 )
        {
            move(n1 , n2);
        }
        else if( strcmp( cmd , "copy" ) == 0 )
        {
            copy(n1 , n2);
        }
        else if( strcmp( cmd , "printAll" ) == 0 )
        {
            printAll();
        }
        else if( strcmp( cmd , "print" ) == 0 )
        {
            print(n1 , n2);
        }
        else if( strcmp( cmd , "save" ) == 0 )
        {
            save();
        }
        else if( strcmp( cmd , "exit" ) == 0 )
        {
            printf("Thankyou for using my line editor\n");
            exit(0);
        }
        else
        {
            printf("ERROR: command not found\n");
        }
    }        

    return 0;
}

