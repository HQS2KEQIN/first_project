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
#include"input_phonenumber.h"
#include"input_password.h"
#include"input_email.h"

//用户账号信息链表: 单链表   电话、用户名/账号、密码、邮箱、指针域(指向下一节点)
typedef struct user_info
{
	char phonenumber[15];
	long user_name;
	char password[30];
	char email[30];
	struct user_info *next;
}info,*INFO;

// extern char input_phone[30] = {0};
int i = 0;               		 		//全局变量i,用于计算读取的用户个数,以便账号的分配
long acc = 0;							//注册时分配的账号
char passwd[30] = {0};					//注册时的密码


/*
功能:创建链表的头节点
参数:无
返回值:INFO链表头节点
*/
INFO init_user_info()
{
	INFO head = malloc(sizeof(info));
	if(head == NULL)
	{
		printf("创建失败\n");
		return NULL;
	}
	head->next = NULL;
	return head;
}

/*
功能:显示注册页面
参数:一个char *虚拟内存地址
返回值:无
*/
void show_register_bmp(char *lcdbuf)
{
	int bmpfd = open("../lib/ui_templates/register.bmp",O_RDWR);
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
功能:打印字
参数:char *str_p,char *str_pa,char *str_email,即输入的电话、密码、邮箱
返回值:无
*/
void show_font_register(char *str_p,char *str_pa,char *str_email)
{
	Init_Font();
	Display_characterX(283,137,str_p,0x007fb80e,2);
	Display_characterX(283,222,str_pa,0x007fb80e,2);
	Display_characterX(283,308,str_email,0x007fb80e,2);
	UnInit_Font();
}

/*
功能:创建虚拟内存,调用show_register_bmp()函数显示图片
参数:无
返回值:无
*/
void show_register_emun()
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

	show_register_bmp(lcdbuf);
}

/*
功能:账号信息链表插入,尾插(仅对新建用户)
参数:INFO head,char *input_pn,char *input_pa,char *input_e
返回值:无
*/
void insert_info(INFO head,char *input_pn,char *input_pa,char *input_e)
{
	INFO t = head;
	INFO node = malloc(sizeof(info));
	while(t->next!=NULL)
	{
		if(strcmp(t->next->phonenumber,input_pn)==0)
		{
			printf("电话重复注册\n");
			return ;
		}
		if(strcmp(t->next->email,input_e)==0)
		{
			printf("邮箱重复注册\n");
			return ;
		}
		t = t->next;
	}

	long user_name = 230200000+i;
	acc = user_name;
	strcpy(node->phonenumber,input_pn);
	node->user_name = user_name;
	strcpy(node->password,input_pa);
	strcpy(passwd,input_pa);
	strcpy(node->email,input_e);
	t->next = node;
	node->next = NULL;
}
/*
功能:用户账号信息链表载入
参数:INFO head,char *phonenumber,long user_name,char *password,char *email
返回值:无
*/
void add_info(INFO head,char *phonenumber,long user_name,char *password,char *email)
{
	INFO t = head;
	while(t->next!=NULL)
		t = t->next;
	INFO node = malloc(sizeof(info));
	strcpy(node->phonenumber,phonenumber);
	node->user_name = user_name;
	strcpy(node->password,password);
	strcpy(node->email,email);
	t->next = node;
	node->next = NULL;
}

/*
功能:从文件中获取账号信息
参数:INFO head
返回值:无
*/
void get_info(INFO head)
{
	
	DIR * dir = opendir("../lib/accont_info");
	struct dirent *ptr = readdir(dir);
	chdir("../lib/accont_info");
	i = 0;
	while(ptr!=NULL)
	{
		
		if(ptr->d_type == 8)
		{
			FILE * file  = fopen(ptr->d_name,"r+");
			if(file == NULL)
			{
				printf("打开失败\n");
				return;
			}	
			char str[100] = {0};
			char phonenumber[15] = {0};
			long user_name;
			char email[30] = {0};
			char password[30] = {0};
			i++;
			fscanf(file,"%[^\n]\n",str);
			sscanf(str,"%s %ld %s %s\n",phonenumber,&user_name,password,email);
			add_info(head,phonenumber,user_name,password,email);
			fclose(file);
		}
		ptr = readdir(dir);
	}
	closedir(dir);
	chdir("../../bin");
}

/*
功能:将账号信息写入文件中
参数:INFO head
返回值:无
*/
void write_info(INFO head)
{
	chdir("../lib/accont_info");
	INFO t = head->next;
	while(t!=NULL)
	{
		char filename[30] = {0};
		sprintf(filename, "%ld", t->user_name);
		strcat(filename,".txt");
		FILE *file  = fopen(filename,"w+");
		fprintf(file,"%s %ld %s %s\n",t->phonenumber,t->user_name,t->password,t->email);
		fclose(file);
		t = t->next;
	}
	chdir("../../bin");
}

/*
功能:判断触屏操作
参数:无
返回值:触屏操作数
*/
int panduan_register()
{
	int x,y,p;
	get_xy(&x,&y,&p);
	if(x>176&&x<250)
	{
		if(y>136&&y<166)
			return 1;					//输入电话
		else if(y>220&&y<250)
			return 2;					//输入密码
		else if(y>307&&y<337)
			return 3;					//输入邮箱

	}
	if(y>337&&y<466)
	{ 
		if(x>190&&x<290)
			return 4;					//注册
		else if(x>565&&x<665)
			return 5;					//退出
	}
	return 0;

}

/*
功能:注册系统操作
参数:无
返回值:无
*/
void register_oper()
{
	INFO head = init_user_info(); 
	char input_pn[15] = {0};
	char input_passw[30] = {0};
	char input_e[30] = {0};
	get_info(head);
	show_register_emun();
	while(1)
	{		
		show_font_register(input_pn,input_passw,input_e);
		int ret = panduan_register();
		if(ret == 1)
		{
			memset(input_pn,0,sizeof(input_pn));
			input_phonenumber(input_pn);
			printf("电话:%s\n",input_pn);
			show_register_emun();
		}
		else if(ret == 2)
		{
			memset(input_passw,0,sizeof(input_passw));
			input_password(input_passw);
			printf("密码:%s\n",input_passw);
			show_register_emun();
		}
	
		else if(ret == 3)
		{
			memset(input_e,0,sizeof(input_e));
			input_email(input_e);
			printf("邮箱:%s\n",input_e);
			show_register_emun();
		}
		
		else if(ret == 4)
		{
	
			insert_info(head,input_pn,input_passw,input_e);
			printf("创建成功,你的账号是:%d 密码是:%s\n",acc,passwd);
		}
		else if(ret == 5)
		{
			write_info(head);
			free(head);
			printf("退出注册系统\n");
			break;
		}					
	}
}



//注册系统
void register_info()
{
	register_oper();
}