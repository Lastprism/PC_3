#include "sub_his_dialog.h"
#include "ui_sub_his_dialog.h"
#include "client.h"
#include <QTableWidgetItem>
#include <sstream>

Sub_His_Dialog::Sub_His_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sub_His_Dialog)
{
    ui->setupUi(this);
}

Sub_His_Dialog::~Sub_His_Dialog()
{
    delete ui;
}

void Sub_His_Dialog::on_closeBtn_clicked()
{
    close();
}
void Sub_His_Dialog::show_data()
{
    ui->showHisTW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->showHisTW->setColumnCount(3);
    ui->showHisTW->setRowCount(all_res_data.size());
    ui->showHisTW->setHorizontalHeaderLabels(QStringList()<<tr("RunID") << tr("Problem") << tr("Result"));
    ui->showHisTW->setColumnWidth(1,150);
    ui->showHisTW->setColumnWidth(1,150);
    ui->showHisTW->setColumnWidth(2,200);
    for(int i = 0; i < all_res_data.size();i++)
    {
        for(int j=0;j<3;j++)
        {
            ui->showHisTW->setItem(i,j,new QTableWidgetItem(QString::fromStdString(all_res_data[i][j])));
        }
    }
}
