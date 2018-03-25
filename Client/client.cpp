#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>
#include<fstream>
#include<time.h>
#include<string>
#include<vector>
#include<map>
#include<sstream>
#include"des.h"
using namespace std;

char res[1505];
char prob[20][100];
string user,pwd;
int cnt_prob;
map<string,int>all_res_map;
vector< vector<string> > all_res_data;
char IP[100];
int Port;
vector<string>waiting_run_id;
#pragma comment(lib, "ws2_32.lib")

//输出文件调试
template<class T>
void output(const T msg)
{
    ofstream fout;
    fout.open("e:\\out.txt",ios::app);
    if(fout.is_open())
    {
        fout << msg << endl;
        fout.close();
    }
}
//向文件输出提交历史记录
void outFileSub_His(const char* filename,string run_id)
{
    ofstream out;
    out.open(filename,ios::app);
    if(out.is_open())
    {
        int int_run_id = all_res_map[run_id];
        out << all_res_data[int_run_id][0]<<endl;
        out << all_res_data[int_run_id][1]<<endl;
        out << all_res_data[int_run_id][2]<<endl;
        out.close();
    }
}
//从文件中读入提交历史记录
void inputFileSub_His(const char* filename)
{
    ifstream input;
    input.open(filename,ios::app);
    if(input.is_open())
    {
        while(!input.eof())
        {
            vector<string> v;
            int int_cnt = all_res_map.size();
            char buff[20];
            int _cnt = 3;
            while(_cnt--)
            {
                memset(buff,0,sizeof(buff));
                input.getline(buff,20);
                v.push_back(buff);
            }
            if(v[0] != "")
            {
                all_res_data.push_back(v);
                all_res_map[all_res_data[int_cnt][0]] = int_cnt;
            }
        }
        input.close();
    }
}

string getTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}

//哈希函数
long long int RSHash(string str)
{
      int b     = 378551;
      int a     = 63689;
      long long int hash = 0;
      for(int i = 0; i < str.length(); i++)
      {
         hash = hash * a + str[i];
         a    = a * b;
      }
      return hash;
}
//add表示ip地址，port表示端口，data_of_send表示要发送的数据，size_of_datas表示发送数据的大小，result表示接受返回结果的缓冲区，result表示缓冲区大小
int client(const char* add,int port,char data_of_send[],int size_of_datas,char* result,int result_size)
{
    //初始化
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if(WSAStartup(sockVersion, &data)!=0)
        return -1;

    //新建套接字
    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
        return -1 ;

    //连接
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(add);
    if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {  //连接失败
        closesocket(sclient);
        return -1;
    }

    //数据加密
    string s_data_of_encrypt;
    for(int i=0;i<size_of_datas;i++)
        s_data_of_encrypt += data_of_send[i];
    DES des_encrypt;
    s_data_of_encrypt =des_encrypt.Encrypt(s_data_of_encrypt,"123456");
    size_of_datas = s_data_of_encrypt.size();

    //添加数据包大小
    char tmp1[10] ;              memset(tmp1,0,sizeof(tmp1));
    stringstream ss;             ss.clear();
    ss<<(size_of_datas+10);      ss>>tmp1;
    string tmp2;
    for(int i=0;i<10;i++)
        tmp2 += tmp1[i];
    s_data_of_encrypt = tmp2 + s_data_of_encrypt;
    size_of_datas += 10;

    //发送数据
    int send_ans = 1;
    if(send(sclient, s_data_of_encrypt.c_str(), size_of_datas, 0) == -1)
        send_ans = -1;
    else
    {
        memset(result,0,sizeof(result));
        int ret = recv(sclient, result, result_size, 0);
        cout<<result<<endl;
        if(ret>0){
            result[ret] = 0x00;
            if(strcmp(result,"again")==0)
                send_ans = 0;
        }
    }

    //send()用来将数据由指定的socket传给对方主机
    //int send(int s, const void * msg, int len, unsigned int flags)
    //s为已建立好连接的socket，msg指向数据内容，len则为数据长度，参数flags一般设0
    //成功则返回实际传送出去的字符数，失败返回-1，错误原因存于error

    closesocket(sclient);
    WSACleanup();
    return send_ans;
}

