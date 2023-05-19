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
#include"info_chain.h"
#include"register.h"

// void show(STU head);
void show_user_info_bmp(long account,char *lcdbuf)
{
	
	int bmpfd = open("../lib/ui_templates/user_menu.bmp",O_RDWR);
	char str[50] = {0};
	sprintf(str,"../lib/stu_head/%ld.bmp",account);
	int bmpfd1 = open(str,O_RDWR);
	// printf("%s\n",str);
	if(bmpfd == -1||bmpfd1 == -1)
	{
		printf("open fail");
		return ;
	}
	int width,high;
	lseek(bmpfd,18,SEEK_SET);
	read(bmpfd,&width,4);//读取宽度
	read(bmpfd,&high,4);//读取高度
	int x_start = 0;
	int y_start = 0;
	char bmpbuf[3*width*high];
	lseek(bmpfd,54,SEEK_SET);
	read(bmpfd,bmpbuf,sizeof(bmpbuf));
	int x,y;	
	for(x = x_start;x<(x_start+width);x++)
	{
		for(y = y_start;y<(y_start+high);y++)
		{                                 
			lcdbuf[0+4*x+4*800*y] = bmpbuf[0+3*(x-x_start)+3*width*(high-1-(y-y_start))];
			lcdbuf[1+4*x+4*800*y] = bmpbuf[1+3*(x-x_start)+3*width*(high-1-(y-y_start))];
			lcdbuf[2+4*x+4*800*y] = bmpbuf[2+3*(x-x_start)+3*width*(high-1-(y-y_start))];
			lcdbuf[3+4*x+4*800*y] = 0;
		}
	}
	int width1,high1;
	lseek(bmpfd1,18,SEEK_SET);
	read(bmpfd1,&width1,4);//读取宽度
	read(bmpfd1,&high1,4);//读取高度
	int x_start1 = 75;
	int y_start1 = 90;
	char bmpbuf1[3*width1*high1];
	lseek(bmpfd1,54,SEEK_SET);
	read(bmpfd1,bmpbuf1,sizeof(bmpbuf1));
	int x1,y1;
	for(x1 = x_start1;x1<(x_start1+width1);x1++)
	{
		for(y1 = y_start1;y1<(y_start1+high1);y1++)
		{                                 
			lcdbuf[0+4*x1+4*800*y1] = bmpbuf1[0+3*(x1-x_start1)+3*width1*(high1-1-(y1-y_start1))];
			lcdbuf[1+4*x1+4*800*y1] = bmpbuf1[1+3*(x1-x_start1)+3*width1*(high1-1-(y1-y_start1))];
			lcdbuf[2+4*x1+4*800*y1] = bmpbuf1[2+3*(x1-x_start1)+3*width1*(high1-1-(y1-y_start1))];
			lcdbuf[3+4*x1+4*800*y1] = 0;
		}
	}
	close(bmpfd);
	

} 

void show_user_info_emun(long account)
{
	int lcdfd = open("/dev/fb0",O_RDWR);
	if(lcdfd == -1)
	{
		perror("open lcd fail\n");
		return ;
	}

	int x,y;
	unsigned char *lcdbuf =(unsigned char *)mmap(NULL,480*800*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);	
	for(x = 0;x<800;x++)
	{
		for(y = 0;y<480;y++)
		{                                 
			lcdbuf[0+4*x+4*800*y] = 255;
			lcdbuf[1+4*x+4*800*y] = 255;
			lcdbuf[2+4*x+4*800*y] = 255;
			lcdbuf[3+4*x+4*800*y] = 0;
		}
	}

	show_user_info_bmp(account,lcdbuf);
}

void show_user_info_font(STU head_stu,INFO head_user,long account)
{
	
	STU t_stu = head_stu->next;
	while(t_stu!=NULL)
	{
		if(t_stu->account == account)
			break;
		t_stu = t_stu->next;
	}
	

	INFO t_user = head_user->next;
	while(t_user!=NULL)
	{
		if(t_user->user_name == account)
			break;
		t_user = t_user->next;
	}
	char age[5] = {0};
	sprintf(age,"%d",t_stu->age);	

	char uid[10] = {0};
	sprintf(uid,"%d",t_stu->uid);
	Init_Font();
	Display_characterX(125,305,t_stu->name,0x00000000,1);
	Display_characterX(125,340,age,0x00000000,1);
	Display_characterX(125,372,t_stu->sex,0x00000000,1);
	Display_characterX(315,40,t_user->phonenumber,0x00000000,1);
	Display_characterX(315,108,uid,0x00000000,1);
	Display_characterX(315,176,t_stu->class,0x00000000,1);
	Display_characterX(315,240,t_stu->home,0x00000000,1);
	Display_characterX(315,307,t_stu->position,0x00000000,1);
	UnInit_Font();
}


int panduan_user()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(y>385&&y<450)
	{
		if(x>340&x<440)
			return 1;
		else if(x>515&x<615)
			return 2;
	}
	return 0;
}

int ifexit(STU head,long user_name)
{
	STU t = head->next;
	while(t!=NULL)
	{
		if(t->account == user_name)
			return 0;
		t = t->next;
	}
	return 1;
	
}

void input_info(STU head,long account)
{
	STU t = head;
	while(t->next!=NULL)
		t = t->next;
	STU node = malloc(sizeof(stu));
	node->account = account;
	printf("你未录入信息,请先录入信息\n");
	printf("==============================\n");
	printf("请输入姓名");
	scanf("%s",node->name);
	printf("==============================\n");
	printf("请输入年龄");
	scanf("%d",&node->age);
	printf("==============================\n");
	printf("请输入学号");
	scanf("%d",&node->uid);
	printf("==============================\n");
	printf("请输入职位");
	scanf("%s",node->position);
	printf("==============================\n");
	printf("请输入住址");
	scanf("%s",node->home);
	printf("==============================\n");
	printf("请输入班级");
	scanf("%s",node->class);
	printf("==============================\n");
	printf("请输入性别");
	scanf("%s",node->sex);
	t->next = node;
	node->next = NULL;
	node->prev = t;
	FILE *file = fopen("../lib/stu_info.txt","a+");
	fprintf(file,"%ld %s %d %d %s %s %s %s\n",node->account,node->name,node->age,node->uid,node->position,node->home,node->class,node->sex);
	fclose(file);
}

void repair_value(long account,STU head_stu)
{
	STU t = head_stu->next;
	while(t!=NULL)
	{
		if(t->account == account)
			break;
		t = t->next;
	}
	int oper;
	printf("请输入你想修改的内容\n");
	printf("(1.年龄 2.性别 3.住址 4.职位 5.学号 6.班级)\n");
	scanf("%d",&oper);
	switch(oper)
	{
		case 1:
		{
			int age ;
			printf("请输入年龄:");
			scanf("%d",&age);
			t->age = age;
			break;
		}
		case 2:
		{
			char sex[5] = {0};
			printf("请输入性别:");
			scanf("%s",sex);
			stpcpy(t->sex,sex);
			break;
		}
		case 3:
		{
			char home[30] = {0};
			printf("请输入住址:");
			scanf("%s",home);
			stpcpy(t->home,home);
			break;
		}
		case 4:
		{
			char position[30] = {0};
			printf("请输入职位:");
			scanf("%s",position);
			stpcpy(t->position,position);
			break;
		}
		case 5:
		{
			int uid;
			printf("请输入学号:");
			scanf("%d",&uid);
			t->uid = uid;
			break;
		}
		case 6:
		{
			char class[10] = {0};
			printf("请输入班级:");
			scanf("%s",class);
			stpcpy(t->class,class);
			break;
		}
		default:
		{
			printf("无此项信息\n");
			break;
		}
	}
	

}


void write_file(STU head)
{
	STU t = head->next;
	FILE *file = fopen("../lib/stu_info.txt","w+");	
	fprintf(file,"%s\n","用户信息");
	while(t!=NULL)	
	{
		fprintf(file,"%ld %s %d %d %s %s %s %s\n",t->account,t->name,t->age,t->uid,t->position,t->home,t->class,t->sex);
		t = t->next;
	}
	fclose(file);
}


void user_oper(long user_name,INFO head_user)
{
	
	STU head_stu = info_chain();
	
	int ret  = ifexit(head_stu,user_name);
	if(ret)
		input_info(head_stu,user_name);
	show_user_info_emun(user_name);
	while(1)
	{
		// show(head_stu);
		show_user_info_font(head_stu,head_user,user_name);
		int rett = panduan_user();
		if(rett == 1)
		{
			show_user_info_emun(user_name);
			repair_value(user_name,head_stu);
		}
		
		else if(rett == 2)
		{
			printf("退出\n");
			write_file(head_stu);
			free(head_stu);
			break;
		}	
	}
}

void user(long user_name,INFO head)
{
	
	user_oper(user_name,head);
}
