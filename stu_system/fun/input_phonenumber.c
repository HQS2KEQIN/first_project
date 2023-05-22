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
#include "font.h"
#include "main.h"

/*
功能:电话输入界面
参数:char *lcdbuf char *型虚拟内存地址
返回值:无
*/
void show_bmp(char *lcdbuf)
{
	int bmpfd = open("../lib/ui_templates/lock_phonenumber.bmp",O_RDWR);
	if(bmpfd == -1)
	{
		printf("open fail");
		return ;
	}
	int width,high;
	lseek(bmpfd,18,SEEK_SET);
	read(bmpfd,&width,4);//读取宽度
	read(bmpfd,&high,4);//读取高度
	int x_start = (800-width)/2;
	int y_start = (480-high)/2;
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

/*
功能:虚拟内存的创建
参数:无
返回值:无
*/
void show_user_info()
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
	show_bmp(lcdbuf);
}

//触屏判断
char panduan_phonenumber()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(y>=50&&y<150)
	{
		if(x>=130&&x<230)
			return '1';
		else if(x>=230&&x<330)
			return '2';
		else if(x>=330&&x<430)
			return '3';
		else if(x>=430&&x<530)
			return 'b';
	}
	else if(y>=150&&y<250)
	{
		if(x>=130&&x<230)
			return '4';
		else if(x>=230&&x<330)
			return '5';
		else if(x>=330&&x<430)
			return '6';
		else if(x>=430&&x<530)
			return '0';
	}
	else if(y>=230&&y<330)
	{
		if(x>=130&&x<230)
			return '7';
		else if(x>=230&&x<330)
			return '8';
		else if(x>=330&&x<430)
			return '9';
		else if(x>=430&&x<530)
			return 'a';
	}
	if(x>550&&x<660)
	{
		if(y>100&&y<190)
			return 'c';
		else if(y>225&&y<315)
			return 'q';
	}

	return ' ';
}

//画面显示
void show_touch(char *str)
{
	for(int i = 0;i<12;i++)
	{
		Display_characterX(280,365,str,0x00000000,3);	
		int ret = panduan_phonenumber();
		if(ret == 'b')
		{
			
			
			if(i != 0)
				str[i-1] = 0;
			i-=2;
			if(i<0)
				i = -1;
			Clean_Area(280,365,500,100,0x00ffffff);
		}
		else if(ret == 'a')
		{
			str[12] = '\0';
			return ;
		}
		else if(ret == 'c')
		{
			
			memset(str, 0, strlen(str)); 
			i = -1;
			Clean_Area(280,365,500,100,0x00ffffff);
			continue;
		}
		else if(ret == 'q')
			return ;
	
		else if(ret == ' ')
		{
			i--;
			continue;
		}
		else
		{
			if(i == 11)
			{
				i--;
				continue;
			}
			str[i] = ret;
		}
	}
}

//显示字
void show_input(char *str)
{
	show_user_info();
	Init_Font();
	show_touch(str);
	UnInit_Font();
}


//
void input_phonenumber(char *str)
{
	memset(str, 0, sizeof(str));
	show_input(str);

}
