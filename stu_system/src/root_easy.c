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
#include"root.h"

void show_root_easy_bmp(char *lcdbuf,STU head_stu,INFO head_user)
{
	int bmpfd = open("../lib/ui_templates/root_easy_emun.bmp",O_RDWR);
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
	STU t_stu = head_stu->next;
	INFO t_user = head_user->next;
	t_user = t_user->next;
	Init_Font();
	char age[5] = {0};
	char uid[10] = {0};
	char account[20] = {0};
	int i = 0;
	while(t_stu!=NULL&&t_user!=NULL)
	{
	
		sprintf(age,"%d",t_stu->age);
		sprintf(uid,"%d",t_stu->uid);
		sprintf(account,"%ld",t_stu->account);
		Display_characterX(67,107+i*20,t_stu->name,0x007fb80e,1);
		Display_characterX(167,107+i*20,age,0x007fb80e,1);
		Display_characterX(267,107+i*20,t_stu->class,0x007fb80e,1);
		Display_characterX(367,107+i*20,uid,0x007fb80e,1);
		Display_characterX(467,107+i*20,t_stu->position,0x007fb80e,1);
		Display_characterX(567,107+i*20,account,0x007fb80e,1);
		Display_characterX(667,107+i*20,t_user->password,0x007fb80e,1);
		i++;
		t_stu = t_stu->next;
		t_user = t_user->next;
	}
	UnInit_Font();
	close(bmpfd);
} 

void show_root_easy_emun(STU head_stu,INFO head_user)
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
	show_root_easy_bmp(lcdbuf,head_stu,head_user);
}


int panduan_root_easy()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(x>334&&y>431&&x<462&&y<474)
		return 1;
	return 0;
}

void root_easy_oper(STU head_stu,INFO head_user)
{	
	while(1)
	{
		show_root_easy_emun(head_stu,head_user);
		int ret = panduan_root_easy();
		if(ret == 1)
			break;
		
	}
	
}



void root_easy(STU head_stu,INFO head_user)
{
	root_easy_oper(head_stu,head_user);
}