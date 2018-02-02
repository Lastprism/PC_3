#include "debug.h"
#include "ui_debug.h"

Debug::Debug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Debug)
{
    ui->setupUi(this);
}

Debug::~Debug()
{
    delete ui;
}
