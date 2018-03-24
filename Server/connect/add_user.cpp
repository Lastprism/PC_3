#include "pc3_server.h"
#include<iostream>
#include<cstring>
using namespace std;

int main()
{
    int n;
    request_handle test;
    test.connect_to("localhost","root","123456","contest");
    cout<<"how much user:";
    cin>>n;
    char user[50],pwd[50];
    for(int i=0;i<n;i++)
    {
        memset(user,0,sizeof(user));
        memset(pwd,0,sizeof(pwd));
        cout<<"user"<<endl;
        cin>>user;
        cout<<"pwd"<<endl;
        cin>>pwd;
        test.add_user(user,pwd);
    }
    test.close();
    return 0;
}
