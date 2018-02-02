#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>
#include <QString>
namespace Ui {
class ResultDialog;
}

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();

public:
    void recv_res(QString sr,QString sp,QString qs_runid);
private slots:
    void on_closeBtn_clicked();

private:
    Ui::ResultDialog *ui;
};

#endif // RESULTDIALOG_H
