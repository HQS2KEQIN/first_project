#include<string.h>
#include<stdio.h>
void input_password(char *str)
{
	memset(str, 0, sizeof(str));
	printf("����������:");
	scanf("%s",str);
}