#include "errordialog.h"
#include "ui_errordialog.h"
#include<string>
#include<QString>

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}
void ErrorDialog::showError(QString infor)
{
    ui->label->setText(infor);
}

void ErrorDialog::on_closeBtn_clicked()
{
    close();
}
