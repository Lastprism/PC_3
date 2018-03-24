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
#define MAX_BUFF 400000

using namespace std;

int runid = 10000;
char c_decrypt[MAX_BUFF];
request_handle test;

mutex mx1,mx2,mx3;

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
    char user[35],pwd[35];
    memset(user,0,sizeof(user));
    memset(pwd,0,sizeof(pwd));
    //获取用户名     1-32字节
    for(int i=1,j=0;i<=32;i++,j++)
        user[j] = revData[i];

    //获取密码  33-65字节
    for(int i = 33,j=0;i<=64;i++,j++)
        pwd[j] = revData[i];

    string s_pwd = pwd;
    long long ll_pwd;
    ss.clear();
    ss<<s_pwd;
    ss>>ll_pwd;
    cout<<"**********************login******************************"<<endl;
    cout<<"user:   "<<user<<endl;
    cout<<"s_pwd:  "<<s_pwd<<endl;
    cout<<"ll_pwd: "<<ll_pwd<<endl;
    cout<<"**********************login******************************"<<endl;

    //判断信息是否正确
    //if(check(user,pwd))
    mx2.lock();
    int zz_Res = test.check_login(user,ll_pwd);
    mx2.unlock();
    if(zz_Res==1)
    {
        //cout<<"zzz"<<endl;
        char sendData[1501];
        memset(sendData,0,sizeof(sendData));

        mx2.lock();
        vector<string> prob_set = test.get_problemset();
        mx2.unlock();

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
    char user[35],pwd[35],c_runid[30];
	memset(user,0,sizeof(user));
	memset(pwd,0,sizeof(pwd));
	memset(c_runid,0,sizeof(c_runid));
    for(int i=0,j=1;j<=32;i++,j++)
        user[i] = revData[j];

    for(int i=0,j=33;j<=64;i++,j++)
        pwd[i] = revData[j];

    ss.clear();
    ss<<pwd;
    long long ll_pwd;
    ss>>ll_pwd;
    //获取runid
    for(int i=65,j=0;i<=74;i++,j++)
        c_runid[j] = revData[i];

    cout<<"**************************query**************************"<<endl;
    cout<<"User:  "<<user<<endl;
    cout<<"Pwd:   "<<pwd<<endl;
    cout<<"Run_id:"<<c_runid<<endl;
    cout<<"**************************query**************************"<<endl;
    mx2.lock();
    int zz_Res = test.check_login(user,ll_pwd);
    mx2.unlock();
    if(zz_Res==1){

        mx2.lock();
        string s_judge_res = test.query_status(user,ll_pwd,c_runid);
        mx2.unlock();

        if(s_judge_res.size()>0){
            const char *c_judge_res = s_judge_res.c_str();
            send(sClient, c_judge_res,s_judge_res.size(),0);
        }
    }
}

void submit_Code(int sClient,string revData)
{
    stringstream ss;
    char c_prob_no[15],c_lang[15],file_name[40],c_runid[40],c_size[8];
    memset(c_prob_no,0,sizeof(c_prob_no));
    memset(c_lang,0,sizeof(c_lang));
    memset(file_name,0,sizeof(file_name));
    memset(c_runid,0,sizeof(c_runid));
    string s_runid;
    // 获取题号
    for(int i=1;i<=10;i++)
  	c_prob_no[i-1] = revData[i];
    // 获取语言号
    int lang_no = revData[11];
    if(lang_no == 0)
        strcpy(c_lang,"cpp");
    else
        strcpy(c_lang,"java");


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

    cout<<"**************************submit*************************"<<endl;
    cout<<"user:      "<<user<<endl<<"pwd:"<<pwd<<endl;
    cout<<"prob_no:   "<<c_prob_no<<endl;
    cout<<"lang_np:   "<<c_lang<<endl;

    //输出代码到文件
    mx2.lock();
    int zz_Res = test.check_login(user,ll_pwd);
    mx2.unlock();
    if(zz_Res==1){
        mx3.lock();mx2.lock();
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
        mx2.unlock();mx3.unlock();

        s_runid += "." ;
        s_runid += c_lang;
        ss.clear();
        ss<<s_runid;
        ss>>file_name;

        cout<<"file_name:   "<<file_name<<endl;

      //cout<<&revData[76]<<endl;

        //保存成功返回runid
        outfile(file_name,&revData[76]);
        cout<<"c_runid:   "<<c_runid<<endl;
        cout<<"*************************submit**************************"<<endl;
        send(sClient,c_runid,10,0);

        mx2.lock();
        test.submit_code(file_name,c_lang,user,ll_pwd,c_prob_no,c_runid);
        mx2.unlock();
    }
}
void thread1(int sClient)
{

    mx1.lock();
    //访问全局变量c_decrypt,要先lock该资源
    memset(c_decrypt,0,sizeof(c_decrypt));

    //printf("receive a connect... \n");
    int ret = recv(sClient,c_decrypt,MAX_BUFF,0);
    DES des_decrypt;
    string tmp1;
    for(int i=0;i<10;i++)
    tmp1 += c_decrypt[i];
    string revData;
	for(int i=10;i<ret;i++)
		revData += c_decrypt[i];
    revData = des_decrypt.Decrypt(revData,"123456");
    mx1.unlock();

    stringstream ss;
    ss.clear();
    ss<<tmp1;
    int tmp2;
    ss>>tmp2;
    cout<<endl;
    //cout<<"*********************************************************"<<endl;
    //cout<<"tmp:"<<tmp2<<endl;
    //cout<<"ret"<<ret<<endl;
    //cout<<"revsize:"<<revData.size()<<endl;
    //cout<<"*********************************************************"<<endl;

    if(tmp2!=ret)
    {
        send(sClient,"again",6,0);
        return ;
    }

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
