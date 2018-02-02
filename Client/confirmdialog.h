#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = 0);
    ~ConfirmDialog();

private slots:
    void on_closeBtn_clicked();

    void on_confirmBtn_clicked();

private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
