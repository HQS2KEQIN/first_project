#ifndef __CHAIN__H__
#define __CHAIN__H__
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
STU info_chain();
int count;


#endif