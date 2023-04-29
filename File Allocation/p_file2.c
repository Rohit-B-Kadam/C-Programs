#include<stdio.h>
//#include<conio.h>
#include<string.h>
#include<malloc.h>
#define newnode (struct FAL *)malloc(sizeof(struct FAL))

struct FAL
{
 char fname[20];
 int start,end,flag;
 struct FAL *next;
}*h=NULL;


int FAT[100],free_blocks,noblcks;


struct FAL *append(struct FAL *);

void disp_dir();


int main()
{
 int ch,bsize,i,j,f;
 char fnm[20];
 struct FAL *p;

 system("clear");

 printf("Enter number of blocks:");
 scanf("%d",&noblcks);

 for(i=0;i<noblcks;i++)
	FAT[i]=-1;


 free_blocks=noblcks;
 do
 {
  f=0;
  printf("\n1.INSERTION\n2.DELETION\n3.DISPLAY\n4.EXIT\nEnter your choice:");
  scanf("%d",&ch);
  switch(ch)
  {
   case 1:printf("Enter the file name:");
	  scanf("%s",fnm);
	  printf("Enter the file size in blocks:");
	  scanf("%d",&bsize);
          if(bsize<=free_blocks)
	  {
   	   for(i=0;i<noblcks;i++)
	   {
	    if(FAT[i]==-1)break;
	   }
	   p=newnode;
	   strcpy(p->fname,fnm);
	   p->flag=0;
	   p->start=j=i;
	   i++;
	   for(;bsize>0;)
	   {
	    if(FAT[i]==-1)
	    {
	     FAT[j]=i;
	     j=i;bsize--;	
	    }
	    i++;
	    if(i>noblcks){i=0;j=i;i++;}
 	    if(bsize==0)break;
	   }
        FAT[j-1]=-2;
	   p->end=j;
 	   p->next=NULL;
           h=append(p);
           free_blocks-=bsize;
	   printf("The given file is placed in free space...");
        for(i=0;i<noblcks;i++)printf("%d\t",FAT[i]);
	  }
	  else printf("The given file cannot be placed...");
	  break;
   case 2:printf("Enter file name to be deleted:");
	  scanf("%s",fnm);
	  for(p=h;p!=NULL;p=p->next)
	  {
           if((strcmp(p->fname,fnm)==0)&&(p->flag==0)){f=1;break;}
	  }
	  if(f!=1)printf("File doesn't exist...");
	  else 
	  {
	   p->flag=1;
	   i=p->start;
	   bsize=0;
	   while(FAT[i]!=-2)
	   {
	    j=FAT[i];
	    FAT[i]=-1;//printf("%d",FAT[i]);
	    i=j;bsize++;
	   }
	   if(FAT[i]==-2){FAT[i]=-1;}
           free_blocks+=bsize;
	   for(i=0;i<noblcks;i++)printf("%d\t",FAT[i]);
	   printf("File is deleted from the free space...");
          }
          break;
   case 3:disp_dir();break;
   
  }
 }while(ch<=3);
 
 return 0;
}


struct FAL *append(struct FAL *f)
{
 struct FAL *s;
 if(h==NULL)
 {
  h=newnode;h=f;
 }
 else
 {
  for(s=h;s->next!=NULL;s=s->next);
  s->next=f;
  s=f;
 }
 return h;
}
void disp_dir()
{
 struct FAL *f;
 printf("DIRECTORY LIST:");
 printf("\nFILE\tSTART\tEND");
 for(f=h;f!=NULL;f=f->next)
 {if(f->flag==0)printf("\n%s\t%d\t%d",f->fname,f->start,f->end);}
}
