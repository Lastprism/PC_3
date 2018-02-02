#include "logindialog.h"
#include "ui_logindialog.h"
#include <string>
#include "client.h"
#include <fstream>
#include <QIcon>
#include <QPalette>
#include "errordialog.h"
using namespace  std;

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("PC3 Login");
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    ui->userErrorLabel->setText("");
    ui->pwdErrorLabel->setText("");
    ui->IPErrorLabel->setText("");

    //用两个QString获取用户名和密码,IP和端口
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->userErrorLabel->setPalette(pa);
    ui->pwdErrorLabel->setPalette(pa);
    ui->IPErrorLabel->setPalette(pa);

    QString Quser,Qpwd,QIP,Qport;
    Quser = ui->UserLineEdit->text();
    if(Quser.size()>16)
    {
        ui->userErrorLabel->setText("The maximum is 16!");
        return ;
    }
    if(Quser.size()==0)
    {
        ui->userErrorLabel->setText("Empty!");
        return ;
    }
    for(int i=0;i<Quser.size();i++)
    {
        if(!((Quser[i]>='0' && Quser[i] <= '9') || (Quser[i] >= 'a' && Quser[i] <= 'z') || (Quser[i] >= 'A' && Quser[i] <= 'Z') || Quser[i] == '_'))
        {
            ui->userErrorLabel->setText("Only Letters,Number and \'_\'!");
            return ;
        }
    }
    Qpwd = ui->pwdLineEdit->text();
    if(Qpwd.size()>16)
    {
        ui->pwdErrorLabel->setText("The maximum is 16!");
        return ;
    }
    if(Qpwd.size()==0)
    {
        ui->pwdErrorLabel->setText("Empty!");
        return ;
    }
    for(int i=0;i<Qpwd.size();i++)
    {
        if(!((Qpwd[i]>='0' && Qpwd[i] <= '9') || (Qpwd[i] >= 'a' && Qpwd[i] <= 'z') || (Qpwd[i] >= 'A' && Qpwd[i] <= 'Z')))
        {
            ui->pwdErrorLabel->setText("Only Letters and Number!");
            return ;
        }
    }
    QIP = ui->IPLineEdit->text();
    if(QIP.size() > 15)
    {
        ui->IPErrorLabel->setText("Too Long!");
        return;
    }
    int IP_cnt = 0,IP_dot_cnt = 0;
    for(int i=0;i<QIP.size();i++)
    {
        if(!((QIP[i] >= '0' && QIP[i] <= '9') || QIP[i] == '.'))
        {
            ui->IPErrorLabel->setText("Only Number and \'.\'!");
            return;
        }
        if(QIP[i] >= '0' && QIP[i] <= '9')
            IP_cnt++;
        if(QIP[i] == '.')
            IP_cnt = 0,IP_dot_cnt ++;
        if(IP_cnt > 3)
        {
            ui->IPErrorLabel->setText("IP Input Error!");
            return;
        }
    }
    if(IP_dot_cnt!=3)
    {
        ui->IPErrorLabel->setText("IP Input Error!");
        return;
    }
    //把QString转换成string
    Port = 7777;
    user.clear();
    pwd.clear();
    user =Quser.toStdString();
    pwd = Qpwd.toStdString();
    string sIP = QIP.toStdString();
    strcpy(IP,sIP.c_str());

    //hash密码
    long long int long_pwd = RSHash(pwd);
    //吧hash后的密码转成字符串
    pwd.clear();
    pwd = to_string(long_pwd);
    //用sendUser作为发送数据，第0位为0表示发送的信息是验证信息，1-32位为用户名，33-64为密码,
    char sendUser[65];
    memset(sendUser,0,sizeof(sendUser));
    for(int i=1,j=0;j<user.size();i++,j++)
        sendUser[i] = user[j];
    for(int i=33,j=0;j<pwd.size();i++,j++)
        sendUser[i] = pwd[j];
    //发送数据64比特的sendUser，返回结果放在res里面
    int zz_Res = client(IP,Port,sendUser,65,res,1501,0);
    //如果有回馈结果，第一位表示题目数量
    if(zz_Res==1)
    {
        if(res[0] > 0)
        {
            cnt_prob = res[0];
            //每一百位表示一道题的题目

            for(int i=0;i<cnt_prob;i++)
            {
                memset(prob[i],0,sizeof(prob[i]));
                int start = i*100+1;
                int end = start + 100;
                int k = 0;
                for(int j =start;j<end && res[j] != '\0';j++,k++)
                    prob[i][k] = res[j];
                prob[i][k] = '\0';
            }
            accept();
        }
        else
        {
            ErrorDialog edlg;
            edlg.showError(QString::fromStdString("User Input Error or\nPassword Input Error"));
            edlg.exec();
        }
    }
    else if(zz_Res==-1)
    {
        ErrorDialog edlg;
        edlg.showError(QString::fromStdString("Cannot Connect Server"));
        edlg.exec();
    }
    else
    {
        ErrorDialog edlg;
        edlg.showError(QString::fromStdString("Login Fails"));
        edlg.exec();
    }
}
