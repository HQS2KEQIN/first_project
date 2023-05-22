#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <linux/input.h>
#include"font.h"
#include"main.h"
#include"register.h"

typedef struct stu_info
{
	long account;
	char name[30];
	int age;
	int uid;
	char position[30];
	char home[30];
	char class[30];
	char sex[5];
	struct stu_info *next;
	struct stu_info *prev;
}stu,*STU;

int count = 0;

STU init_stu_head()
{
	STU head = malloc(sizeof(stu));
	if(head == NULL)
	{
		printf("´´½¨Ê§°Ü\n");
		return NULL;
	}
	head->next = NULL;
	head->prev = NULL;
	return head;
}

void add_stu_info(STU head,long account,char *name,int age,int uid,char *position,char *home,char *class,char *sex)
{
	STU t = head;
	STU node = malloc(sizeof(stu));
	while(t->next!=NULL)
		t = t->next;
	node->account = account;
	strcpy(node->name,name);
	node->age = age;
	node->uid = uid;
	strcpy(node->position,position);
	strcpy(node->home,home);
	strcpy(node->class,class);
	strcpy(node->sex,sex);
	t->next = node;
	node->prev = t;
	node->next = NULL;
}

void show(STU head)
{
	STU t = head;
	while(t->next!=NULL)
	{
		t = t->next;
		printf("%ld %s %d %d %s %s %s %s\n",t->account,t->name,t->age,t->uid,t->position,t->home,t->class,t->sex);
		
	}
}


STU get_stu_info()
{
	STU head = init_stu_head();
	FILE *file = fopen("../lib/stu_info.txt","r+");
	long account;
	char name[30];
	int age;
	int uid;
	char position[30];
	char home[30];
	char class[30];
	char sex[5];
	char str[100] = {0};
	int i = 0;
	count = 0;
	while(!feof(file))
	{
		if(i == 0)
		{
			fscanf(file,"%[^\n]\n",str);
			i = 1;
			continue;
		}
		count++;
		fscanf(file,"%[^\n]\n",str);
		sscanf(str,"%ld %s %d %d %s %s %s %s",&account,name,&age,&uid,position,home,class,sex);
		add_stu_info(head,account,name,age,uid,position,home,class,sex);
			
	}
	fclose(file);
	return head;
	
}

STU info_chain()
{
	STU head = get_stu_info();
	return head;
}
