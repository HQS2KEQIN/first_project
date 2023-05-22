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
#include "register.h"
#include "main.h"
#include "font.h"
#include "user.h"


void show_login_bmp(char *lcdbuf)
{
	int bmpfd = open("../lib/ui_templates/login_user.bmp",O_RDWR);
	if(bmpfd == -1)
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

	close(bmpfd);
} 

void show_login_emun()
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

	show_login_bmp(lcdbuf);
}

void show_login_title(int input_user,char *input_passw)
{
	Init_Font();
	char user_name[20] = {0};
	sprintf(user_name,"%ld",input_user);
	Display_characterX(310,146,user_name,0x00905a3d,3);
	Display_characterX(310,245,input_passw,0x00905a3d,3);
	UnInit_Font();
}

int loginif(INFO head,int input_user,char *input_passw)
{
	INFO t = head->next;
	while(t!=NULL)
	{
		if(input_user == 230200000)
		{
			printf("此为管理员账户,请从管理员入口进入\n");
			return 0;
		}	
		if(t->user_name == input_user)
		{
			if(strcmp(input_passw,t->password)==0)
			{
				printf("登陆成功\n");
				return 1;
			}
			printf("密码错误\n");
			return 0;
		}
		t = t->next;
	}
	printf("用户不存在\n");
	return 0;
}

int panduan_user_login()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(x>240&&x<300)
	{
		if(y>138&&y<197)
			return 1;
		else if(y>237&&y<295)
			return 2;
	}
	if(y>331&&y<386)
	{
		if(x>240&&x<380)
			return 3;

		else if(x>425&&x<565)
			return 5;
	}
	return 0;
}

void login_oper()
{	
	long input_user = 0;
	char input_passw[30] = {0};
	int input_count = 3;
	INFO head = init_user_info(); 
	get_info(head);
	show_login_emun();
	while(input_count)
	{
		show_login_title(input_user,input_passw);
		int ret = panduan_user_login();
		if(ret == 1)
		{
			long input_user1;
			printf("请输入账户:");
			scanf("%ld",&input_user1);
			input_user = input_user1;
			show_login_emun();
		}
		else if(ret == 2)
		{
			printf("请输入密码:");
			scanf("%s",input_passw);
			show_login_emun();
		}
		else if(ret == 3)
		{
			int ack = loginif(head,input_user,input_passw);
			if(ack == 0)
			{
				input_count--;
				if(input_count)
					printf("还有%d次输入机会",input_count);
				continue;
			}
			user(input_user,head);
			show_login_emun();
			input_user = 0;
			memset(input_passw, 0, sizeof(input_passw));
		}
		else if(ret == 5)
		{
			printf("退出\n");
			free(head);
			break;
		}
	}
}
	
void login()
{
	login_oper();
}
	
	