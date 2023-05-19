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


void show_root_ui_bmp(char *lcdbuf)
{
	int bmpfd = open("../lib/ui_templates/root_ui.bmp",O_RDWR);
	int bmpfd1 = open("../lib/stu_head/230200000.bmp",O_RDWR);
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

void show_root_ui_emun()
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
	show_root_ui_bmp(lcdbuf);
}


int  panduan_ui()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(x>394&x<540&&y>385&y<446)
		return 1;
	return 0;
}



void root_ui_oper()
{
	show_root_ui_emun();
	while(1)
	{
		int ret =panduan_ui();
		if(ret == 1)
			break;
	}
		
}


void root_ui()
{
	root_ui_oper();
}
