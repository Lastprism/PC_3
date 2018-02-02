#include "resultdialog.h"
#include "ui_resultdialog.h"
#include<QString>
#include<client.h>
#include <QFont>
#include <QPalette>
ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::on_closeBtn_clicked()
{
    close();
}

void ResultDialog::recv_res(QString sr,QString sp,QString qs_runid)
{
    QFont ft;
    QPalette pa;
    ft.setPointSize(18);

    ui->resLabel->setText("RunId: " + qs_runid + "\n" + "Problem: " + sp + "\n" + "Answer: " + sr);
    ui->resLabel->setFont(ft);
    if(sr[0] == 'a')
        pa.setColor(QPalette::WindowText,Qt::green);
    else
        pa.setColor(QPalette::WindowText,Qt::red);
    ui->resLabel->setPalette(pa);

}
