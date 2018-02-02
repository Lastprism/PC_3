
#ifndef CLIENT_H
#define CLIENT_H

#define ADD "127.0.0.1"
#define PORT 8888
#include<string>
#include"des.h"
using namespace std;

//通信返回结果
extern char res[1505];

//题目信息
extern char prob[20][100];
//extern char code[500000];

//题目数量
extern int cnt_prob;

extern map<string,int>all_res_map;
extern vector< vector<string> > all_res_data;


//Hash后的用户信息
extern string user,pwd;

//IP,端口
extern char IP[100];
extern int Port;

//等待评测的runid
extern vector<string>waiting_run_id;

//发送数据，add是服务器地址，port是端口，data_of_send是待发送数据，size_of_data是数据大小,res是返回结果
extern int client(const char* add,int port,char data_of_send[],int size_of_data,char* result,int result_size,int sendCount);

//输出调试信息
extern void outFileSub_His(const char*filename,string run_id);
extern void inputFileSub_His(const char*filename);
extern void outfilechar(char* str);
extern void outfileint(int str);
extern void outfilestring(string str);
extern void outfilelong(long long  str);
//获取当前时间
extern string getTime();

//hash密码
extern long long int RSHash(string str);


#endif // CLIENT_H
