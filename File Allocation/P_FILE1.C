#include<stdio.h>
#include<stdlib.h>
//#include<conio.h>
#include<string.h>
#include<malloc.h>
#define newdir (struct fat *)malloc(sizeof(struct fat))
#define newnode (struct fbl *)malloc(sizeof(struct fbl))
struct fat
{
 char fname[30];
 int start,length,flag;
 struct fat *link;
}*first=NULL;
struct fbl
{
 int start,length;
 struct fbl *link;
}*f1=NULL;
int blocks;
void disp_dir();
void disp_free_space();
struct fat *append(struct fat *);
int main()
{
 int ch,bno,f;
 char fn[30];
 struct fbl *p;struct fat *q;
 
 printf("Enter number of blocks:");
 scanf("%d",&blocks);
 f1=newnode;
 f1->start=0;
 f1->length=blocks;
 f1->link=NULL;
 do
 {
  f=1;
  printf("\n1.INSERTION\n2.DELETION\n3.DISPLAY\n4.EXIT\nEnter your choice:");
  scanf("%d",&ch);
  switch(ch)
  {
   case 1:printf("Enter the file name:");
	  scanf("%s",fn);
	  printf("Enter the file size in number of blocks:");
	  scanf("%d",&bno);
	  for(p=f1;p!=NULL;p=p->link)
	  {
           if(p->length>=bno)break;
	  }
	  if(p==NULL)printf("Not enough space...");
	  else
	  {
           q=newdir;
	   strcpy(q->fname,fn);
	   q->length=bno;
	   q->start=p->start;
	   q->flag=0;
	   q->link=NULL;
	   first=append(q);
	   p->start+=bno;
	   p->length-=bno;
	   printf("File is placed in free space...");
	  }break;
   case 2:printf("Enter the file name to be deleted:");
	  scanf("%s",fn);
	  for(q=first;q!=NULL;q=q->link)
	  {
   	   if(strcmp(q->fname,fn)==0){f=0;break;}
	  }
	  if(f==1)printf("The given file doesn't exist...");
	  else
	  {//printf("%s",q->fname);
	   q->flag=1;
	   p=newnode;
	   p->start=q->start;
	   p->length=q->length;
	   p->link=NULL;
           for(;f1->link!=NULL;f1=f1->link);
	   f1->link=p;
	   printf("The given file is deleted from the disk...");
	  }break;
   case 3:disp_dir();disp_free_space();break;
   
  }
 }while(ch<=3);
 
 return 0;
}
void disp_dir()
{
 struct fat *p;
 printf("\nDisk List:");
 printf("\nName\tStart\tLength");
 for(p=first;p!=NULL;p=p->link)
 {
  if(p->flag==0)printf("\n%s\t%d\t%d",p->fname,p->start,p->length);
 }
}
void disp_free_space()
{
 struct fbl *q;
 printf("\nFree Block-space List:");
 printf("\nStart\tLength");
 for(q=f1;q!=NULL;q=q->link)
 {printf("\n%d\t%d",q->start,q->length);}
}
struct fat *append(struct fat *f)
{
 struct fat *s;
 if(first==NULL)
 {
  first=newdir;
  first=f;
 }
 else
 {
  for(s=first;s->link!=NULL;s=s->link);
  s->link=f;
  s=f;
 }
 return first;
}
