#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}

void ConfirmDialog::on_closeBtn_clicked()
{
     close();
}

void ConfirmDialog::on_confirmBtn_clicked()
{
    accept();
}
