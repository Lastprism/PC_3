#include "thread.h"
#include "client.h"
#include "cstring"
#include "resultdialog.h"
#include <QString>
#include <qtimer.h>
#include <QTimer>
#include <fstream>
#include "errordialog.h"

Thread::Thread(string run_id_,string prob_id_)
{
    stopped = false;
    run_id = run_id_;
    prob_id = prob_id_;
    vector <string>v;
    v.push_back(run_id);
    v.push_back(prob_id);
    v.push_back("waiting");
    mu.lock();
    all_res_map.insert(pair<string,int>(run_id,all_res_map.size()));
    all_res_data.push_back(v);
    mu.unlock();
}


void Thread::run()
{
    m_pTimer = new QTimer();
    m_pTimer->setInterval(10000);
    connect(m_pTimer, &QTimer::timeout, this, &Thread::query_res);
    m_pTimer->start();
    this->exec();
}


void Thread::stop()
{
    stopped = true;
}

void Thread::query_res()
{
    char send_query[75];
    memset(send_query,0,sizeof(send_query));
    send_query[0] = 2;

    for(int i=1,j=0;i<=32&&j<user.size();i++,j++)
        send_query[i] = user[j];
    for(int i=33,j=0;i<=64&&j<pwd.size();i++,j++)
        send_query[i] = pwd[j];

    for(int i=65,j=0;i<=74&&j<run_id.size();i++,j++)
        send_query[i] = run_id[j];
    int zz_Res = client(IP,Port,send_query,75,res_of_query,50,0);
    if(zz_Res == -1|| zz_Res == 0)
    {
        m_pTimer->stop();
        ErrorDialog edlg;
        edlg.showError(QString::fromStdString("Sorry!\nCannot Connect Server!\nNo Answer Response!"));
        edlg.exec();
        return ;
    }
    if(strcmp(res_of_query,"waiting")!=0)
    {
        m_pTimer->stop();
        all_res_data[all_res_map[run_id]].pop_back() ;
        all_res_data[all_res_map[run_id]].push_back(res_of_query);
        mu.lock();
        outFileSub_His(user.c_str(),run_id);
        mu.unlock();
        ResultDialog *Rdlg = new ResultDialog;
        Rdlg->setWindowTitle(QString::fromStdString("PC3"));
        Rdlg ->recv_res(QString::fromStdString(res_of_query),QString::fromStdString(prob_id),QString::fromStdString(run_id));
        Rdlg->exec();
    }
}
