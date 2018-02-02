#include "runiddialog.h"
#include "ui_runiddialog.h"

RunIDDialog::RunIDDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunIDDialog)
{
    ui->setupUi(this);
}

RunIDDialog::~RunIDDialog()
{
    delete ui;
}
void RunIDDialog::runID(QString sp,QString qs_runid)
{
    QFont ft;
    QPalette pa;
    ft.setPointSize(18);

    ui->runIDLabel->setText("RunId: " + qs_runid + "\n" + "Problem: " + sp + "\n");
    ui->runIDLabel->setFont(ft);
    pa.setColor(QPalette::WindowText,Qt::blue);
    ui->runIDLabel->setPalette(pa);
}

void RunIDDialog::on_closeBtn_clicked()
{
    close();
}
