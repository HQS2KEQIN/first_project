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
#include "font.h"
#include "login.h"
#include "login_admin.h"
/* 
功能:清屏
参数:无
返回值:无
*/
extern void clear_lcd()
{
	Init_Font();
	Clean_Area(0,0,800,480,0x00000000);
	UnInit_Font();
}


/*
功能:打开LCD驱动
参数:无
返回值:int类型的lcd数据流
*/

int open_lcd()
{
	int lcdfd = open("/dev/input/event0",O_RDWR);
	if(lcdfd == -1)
	{
		perror("open lcd fail\n");
		return -1;
	}
	return lcdfd;
}

/*
功能:打印主页图片
参数:一个虚拟内存的地址
返回值:无
*/
void show_index_bmp(char *lcdbuf)
{
	int bmpfd = open("../lib/ui_templates/index.bmp",O_RDWR);
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

/*
功能:创建虚拟地址,并对页面进行清屏处理
参数:无
返回值:无
*/
void show_index_emun()
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
	show_index_bmp(lcdbuf);
}

/*
功能:获取x、y坐标以及按压状态
参数:int *x,int *y,int *p  
返回值:无
*/
void get_xy(int *x,int *y,int *p)
{
	int lcdt =  open_lcd();
	struct input_event en;
	bool x_if = false;
	bool y_if = false;
	bool p_if = false;
	while(1)
	{
		bzero(&en,sizeof(en));
		read(lcdt,&en,sizeof(en));
		if(en.type == EV_ABS)
		{
			if(en.code == ABS_X)
			{
				x_if = true;
				*x = en.value*800/1024;
			}
			if(en.code == ABS_Y)
			{
				y_if = true;
				*y = en.value*480/600;
			}
		}
		if(en.type == EV_KEY)
		{
			if(en.code == BTN_TOUCH)
			{
				*p = en.value;
				p_if = true;
			}
		}
		if(x_if&&y_if&&p_if)
			break;
	}
}
/*
功能:判断触屏操作
参数:无
返回值:int 操作数
*/
int panduan()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(y>135&&y<230)
	{
		if(x>182&&x<333)
			return 1;			//学生登陆入口
		else if(x>464&&x<618)
			return 2;			//管理员登陆入口
	}
	else if(y>314&&y<420)
	{
		if(x>179&&x<322)
			return 3;			//注册系统入口
		else if(x>467&&x<621)
			return 4;			//退出学生管理系统,也是退出整个程序的通道
	}
	return 0;					//其它空白处
}

/*
功能:主页操作
参数:无
返回值:无
*/
void index_oper()
{
	show_index_emun();
	while(1)
	{
		int ret = panduan();
		if(ret == 1)
		{
			login();
			show_index_emun();
		}
		else if(ret == 2)
		{
			login_admin();
			show_index_emun();
		}
		else if(ret == 3)
		{
			register_info();
			show_index_emun();
		}
		else if(ret == 4)
		{
			printf("退出界面\n");
			clear_lcd();
			break;
		}
	}
}



//项目的唯一主函数
int main()
{
	index_oper();
	
	return 0;
}
