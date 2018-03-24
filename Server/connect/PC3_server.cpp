#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include<string>
#include<fstream>
#include <iostream>
#include<sstream>
#include<mutex>
#include<thread>

#include"pc3_server.h"
#include"DES.h"

#define PORT 7777
#define QUEUE 20
#define MAX_BUFF 500000

using namespace std;

int runid = 10000;
char c_decrypt[MAX_BUFF];
request_handle test;

mutex mx;

void outfile(const char* file_name,char* str)
{
    ofstream out;
    out.open(file_name);
    if(out.is_open())
    {
        cout<<"succeed to write"<<endl;
        out << str ;
        out.close();
    }
    else
        cout<<"Fail to write"<<endl;
}

void check_User_Pwd(int sClient,string revData)
{
    stringstream ss;
    //存储用户和密码
    char user[32],pwd[32];

    //获取用户名     1-32字节
    int j = 0;
    for(int i=1;i<33 && revData[i] != '\0';i++,j++)
        user[j] = revData[i];
    user[j] = '\0';

    //获取密码  33-65字节
    j=0;
    for(int i = 33;i<=64 && revData[i] != '\0';i++,j++)
        pwd[j] = revData[i];
    pwd[j] = '\0';

    string s_pwd = pwd;
    long long ll_pwd;
    ss.clear();
    ss<<s_pwd;
    ss>>ll_pwd;
    cout<<"user:"<<user<<endl<<"s_pwd:"<<s_pwd<<endl<<"ll_pwd:"<<ll_pwd<<endl;

    //判断信息是否正确
    //if(check(user,pwd))
    if(test.check_login(user,ll_pwd)==1)
    {
        //cout<<"zzz"<<endl;
        char sendData[1501];
        memset(sendData,0,sizeof(sendData));
        vector<string> prob_set = test.get_problemset();
        sendData[0] = (char)prob_set.size();
        for(int i=0;i<prob_set.size();i++)
        {
            for(int j=0;j<prob_set[i].size();j++)
            {
                sendData[i*100+j+1] = prob_set[i][j];
            }
        }
        send(sClient,sendData,1501,0);
    }
}

void query_res(int sClient,string revData)
{
    stringstream ss;
    //获取user && pwd
    char user[32],pwd[32],c_runid[10];

    for(int i=0,j=1;j<=32;i++,j++)
        user[i] = revData[j];

    for(int i=0,j=33;j<=64;i++,j++)
        pwd[i] = revData[j];

    ss.clear();
    ss<<pwd;
    long long ll_pwd;
    ss>>ll_pwd;
    //获取runid
    for(int i=65;i<=74;i++,j++)
        c_runid[j] = revData[i];


    cout<<"User:"<<user<<endl
    cout<<"Pwd:"<<pwd<<endl
    cout<<"Run_id:"<<c_runid<<endl;
    if(test.check_login(user,ll_pwd)==1){
        string s_judge_res = test.query_status(user,ll_pwd,c_runid);
        if(s_judge_res.size()>0){
            const char *c_judge_res = s_judge_res.c_str();
            send(sClient, c_judge_res,s_judge_res.size(),0);
        }
    }
}

void submit_Code(int sClient,string revData)
{
    stringstream ss;
    char c_prob_no[10],c_lang[10],file_name[20],c_runid[20];
    string s_runid;
    // 获取题号
    for(int i=1;i<=10;i++)
  	c_prob_no[i-1] = revData[i];
    // 获取语言号
    int lang_no = revData[2];
    if(lang_no == 0)
        strcpy(c_lang,"cpp");
    else
        strcpy(c_lang,"java");
	/*
    // 获取当前时间
    char current_time[20];
    for(int i=3;i<23;i++)
        current_time[i-3] = revData[i];
	*/

    //获取user && pwd
    char user[32],pwd[32];
    for(int i=0,j=12;j<=43;i++,j++)
        user[i] = revData[j];

    for(int i=0,j=44;i<=75;i++,j++)
        pwd[i] = revData[j];

    long long ll_pwd;
    string s_pwd = pwd;
    ss.clear();
    ss<<s_pwd;
    ss>>ll_pwd;

    cout<<"*********************************************************"<<endl;
    cout<<"user:"<<user<<endl<<"pwd:"<<pwd<<endl;
    cout<<"prob_no:"<<c_prob_no<<endl;
    cout<<"lang_np:"<<c_lang<<endl;
    cout<<"current_time:"<<current_time<<endl;


    //输出代码到文件
    if(test.check_login(user,ll_pwd)==1){
        mx.lock();
        do{
            ss.clear();
            ss<<runid;
            ss>>s_runid;
            ss.clear();
            ss<<runid;
            memset(c_runid,0,sizeof(c_runid));
            ss>>c_runid;
            runid++;
        }while(test.query_status(user,ll_pwd,c_runid)!="");
        mx.unlock();

        s_runid += "." ;
        s_runid += c_lang;
        ss.clear();
        ss<<s_runid;
        ss>>file_name;

        cout<<file_name<<endl;
        cout<<&revData[76]<<endl;


        //保存成功返回runid
        outfile(file_name,&revData[76]);
        cout<<c_runid<<endl;
        cout<<"*********************************************************"<<endl;
        send(sClient,c_runid,10,0);

        test.submit_code(file_name,c_lang,user,ll_pwd,c_prob_no,c_runid);
    }
}
void thread1(int sClient)
{

    mx.lock();
    //访问全局变量c_decrypt,要先lock该资源
    memset(c_decrypt,0,sizeof(c_decrypt));

    printf("receive a connect... \n");
    int ret = recv(sClient,c_decrypt,MAX_BUFF,0);

    cout<<c_decrypt<<endl;

    //DES des_decrypt;
    string revData;

    for(int i=0;i<ret;i++)
        revData += c_decrypt[i] + 127;

    //revData = des_decrypt.Decrypt(revData,"123456");
    mx.unlock();
    //全局变量c_decrypt已经用完unlock该资源

    int rev_no = revData[0];
    if(ret > 0)
    {
        //登录请求
        if(rev_no == 0)
            check_User_Pwd(sClient,revData);
        //提交请求
        else if(rev_no == 1)
            submit_Code(sClient,revData);
        //查询请求
        else if(rev_no == 2)
           query_res(sClient,revData);
    }
    close(sClient);
}
int main() {

    int conn;

    test.connect_to("localhost","root","123456","contest");

    int ss = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_sockaddr;

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1) {
        perror("bind");
        exit(1);
    }

    if(listen(ss, QUEUE) == -1) {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    ///成功返回非负描述字，出错返回-1
    while(1)
    {
    	printf("waiting for connect...\n");
    	conn = accept(ss, (struct sockaddr*)&client_addr, &length);
    	if( conn < 0 ) {
       		 perror("connect");
        	exit(1);
   		 }

        thread task(thread1,conn);

        task.detach();
    }

    close(ss);
    return 0;
}
