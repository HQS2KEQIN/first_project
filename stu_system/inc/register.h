#ifndef __REGISTER__H__
#define __REGISTER__H__
void register_info();
typedef struct user_info
{
	char phonenumber[15];
	long user_name;
	char password[30];
	char email[30];
	struct user_info *next;
}info,*INFO;
INFO init_user_info();
void add_info(INFO head,char *phonenumber,long user_name,char *password,char *email);
void get_info(INFO head);
#endif