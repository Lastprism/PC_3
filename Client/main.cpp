#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include "client.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
        inputFileSub_His(user.c_str());
        MainWindow w;
        w.show();
        return a.exec();
    }
}
