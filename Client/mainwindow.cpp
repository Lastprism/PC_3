#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringList>
#include <QString>
#include <client.h>
#include <string>
#include "resultdialog.h"
#include "confirmdialog.h"
#include "thread.h"
#include <sstream>
#include "sub_his_dialog.h"
#include <QDialog>
#include "runiddialog.h"
#include "errordialog.h"
#include "logindialog.h"
#include "mainwindow.h"
#include <QApplication>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //背景透明
    ui->codeTextEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowTitle("PC3");

    //定义一个QString来装题目，cnt_prob是题目数量，然后prob[][]是get到的题目信息
    QStringList problist;
    for(int i=0;i<cnt_prob;i++)
        problist.append(prob[i]);

    //把题目装进下拉框
    ui->problemComBox->addItems(problist);
}

MainWindow::~MainWindow()
{
    delete ui;

}
char code[500000];
void MainWindow::on_submitBtn_clicked()
{
    memset(code,0,sizeof(code));
    //第0个字节标志发送的信息是代码
    code[0] =1;

    //第1-10Byte表示题号
    int curProblem = ui->problemComBox->currentIndex();
    for(int i=1;i<=10 && prob[curProblem][i-1] != '\0';i++)
        code[i] = prob[curProblem][i-1];

    //第11位表示语言
    code[11] = ui->langComboBox->currentIndex();

    //后面20位是时间
    //string currnet_time = getTime();
    //for(int i=2;i<32;i++)
    //    code[i] = currnet_time[i-2];
    //s = s + QString::fromStdString(currnet_time) + '\0';

    //后面64位表示账号和密码
    for(int i=12,j=0;i<=43&&j<user.size();i++,j++)
        code[i] = user[j];
    for(int i=44,j=0;i<=75&&j<pwd.size();i++,j++)
        code[i] = pwd[j];

    //获取代码扔到后面
    QString qsCode = ui->codeTextEdit->toPlainText();
    if(qsCode.size()==0)
    {
        ErrorDialog edlg;
        edlg.showError(QString::fromStdString("No Code!"));
        edlg.exec();
        return;
    }
    else if(qsCode.size()>20000)
    {
        ErrorDialog edlg;
        edlg.showError(QString::fromStdString("The Code Is Too Long"));
        edlg.exec();
        return ;
    }
    string sCode = qsCode.toStdString();
    int sCodeSize = sCode.size();
    for(int i=76,j=0;j<sCodeSize;j++,i++)
        code[i] = sCode[j];
    sCodeSize += 76;
    ConfirmDialog cdlg;
    if(cdlg.exec() == QDialog::Accepted)
    {
        char res_of_run_id[20];
        int zz_Res = client(IP,Port,code,sCodeSize,res_of_run_id,20);
        if(zz_Res==0)
        {
            ErrorDialog edlg;
            edlg.showError(QString::fromStdString("Sorry!\nSubmit Code Fails!\nPlease Submit Again!"));
            edlg.exec();
            return ;
        }
        else if(zz_Res == -1)
        {
            ErrorDialog edlg;
            edlg.showError(QString::fromStdString("Sorry!\nCannot Connect Server!"));
            edlg.exec();
            return ;
        }
        string string_res_of_run_id = res_of_run_id;
        string string_prob_id = "";
        for(int i=1;i<=10&&code[i]!='\0';i++)
            string_prob_id+=code[i];
        RunIDDialog rIDdlg;
        rIDdlg.runID(QString::fromStdString(string_prob_id),QString::fromStdString(string_res_of_run_id));
        rIDdlg.exec();
        Thread thread_of_query(string_res_of_run_id,string_prob_id);
        thread_of_query.run();
    }
}

void MainWindow::on_closeBtn_clicked()
{
    close();
}

void MainWindow::on_pushButton_clicked()
{
    Sub_His_Dialog shdlg;
    shdlg.show_data();
    shdlg.exec();
}
