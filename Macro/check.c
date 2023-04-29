#include<stdio.h>
#include<string.h>

int main()
{
	char a[] = "&ROHIT";
	char b[20];
	strcpy(b,a+1);
	printf("%s",b);
return  0;
}
