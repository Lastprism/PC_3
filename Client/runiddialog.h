#ifndef RUNIDDIALOG_H
#define RUNIDDIALOG_H

#include <QDialog>

namespace Ui {
class RunIDDialog;
}

class RunIDDialog : public QDialog
{
    Q_OBJECT
public:
    void runID(QString sp,QString qs_runid);
public:
    explicit RunIDDialog(QWidget *parent = 0);
    ~RunIDDialog();

private slots:
    void on_closeBtn_clicked();

private:
    Ui::RunIDDialog *ui;
};

#endif // RUNIDDIALOG_H
