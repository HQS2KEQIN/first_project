#include<string.h>
#include<stdio.h>
void input_password(char *str)
{
	memset(str, 0, sizeof(str));
	printf("ÇëÊäÈëÃÜÂë:");
	scanf("%s",str);
}