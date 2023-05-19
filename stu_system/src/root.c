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
#include"root_easy.h"
#include"root_ui.h"

int nowpage = 1;
void show_root_bmp(char *lcdbuf,STU head_stu,INFO head_user)
{
	int bmpfd = open("../lib/ui_templates/root_menu.bmp",O_RDWR);
	char str[50] = {0};
	sprintf(str,"../lib/stu_head/%ld.bmp",head_stu->account);
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
	int x_start1 = 112;
	int y_start1 = 24;
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
	
	
	INFO t_user = head_user->next;
	while(t_user!=NULL)
	{
		if(t_user->user_name == head_stu->account)
			break;
		t_user = t_user->next;
	}
	
	Init_Font();
	
	char age[5] = {0};
	sprintf(age,"%d",head_stu->age);	
	// printf("%s\n",stu->sex);
	char uid[10] = {0};
	sprintf(uid,"%d",head_stu->uid);
	
	char now_p[3] = {0};
	sprintf(now_p,"%d",nowpage);
	
	char count_p[3] = {0};
	sprintf(count_p,"%d",count);
	
	Display_characterX(178,228,head_stu->name,0x00000000,2);
	Display_characterX(178,318,age,0x00000000,2);
	Display_characterX(178,408,head_stu->sex,0x00000000,2);
	Display_characterX(362,48,head_stu->class,0x00000000,1);
	Display_characterX(362,118,uid,0x00000000,1);
	Display_characterX(362,188,t_user->phonenumber,0x00000000,1);
	Display_characterX(362,258,head_stu->home,0x00000000,1);
	Display_characterX(362,328,head_stu->position,0x00000000,1);
	Display_characterX(362,398,t_user->email,0x00000000,1);
	
	Display_characterX(502,442,now_p,0x006f599c,2);
	Display_characterX(550,442,count_p,0x006f599c,2);
	UnInit_Font();
} 


void show_root_emun(STU head_stu,INFO head_user)
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

	show_root_bmp(lcdbuf,head_stu,head_user);
}

int panduan_root()
{
	int x1,y1,p,x2,y2;
	get_xy(&x1,&y1,&p);
	if(p!=0)
	{
		if(x1>10&&x1<85)
		{
			if(y1>104&&y1<180)
				return 1;
			else if(y1>236&&y1<313)
				return 5;
			else if(y1>370&&y1<440)
				return 2;
		}
	}
	get_xy(&x2,&y2,&p);
	if(p == 0) 
	{
		int deta_x = x2-x1;
		if(deta_x>50)
			return 3;
		else if(deta_x<-50)
			return 4; 
	}
	return 0;
}


void root_oper()
{
	INFO head_user = init_user_info();
	get_info(head_user);
	STU head_stu = info_chain();
	STU t = head_stu->next;
	while(1)
	{
		show_root_emun(t,head_user);
		int ret = panduan_root();
		if(ret == 1)
		{
			root_easy(head_stu,head_user);
		}
		else if(ret == 2)
		{
			printf("退出\n");
			free(head_stu);
			free(t);
			free(head_user);
			break;
		}
		else if(ret == 3)
		{
			
			if(t->prev!=head_stu)
			{
				nowpage--;
				t = t->prev;
				// printf("%ld %s %d %d %s %s %s %s\n",t->account,t->name,t->age,t->uid,t->home,t->position,t->class,t->sex);
			}
		}
		else if(ret == 4)
		{
			if(t->next!=NULL)	
			{
				nowpage++;
				t = t->next;
				// printf("%ld %s %d %d %s %s %s %s\n",t->account,t->name,t->age,t->uid,t->home,t->position,t->class,t->sex);
			}
		}
		else if(ret == 5)
		{
			root_ui();
			show_root_emun(t,head_user);
		}
	}
}


void root()
{
	root_oper();
}