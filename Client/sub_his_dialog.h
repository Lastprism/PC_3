#ifndef SUB_HIS_DIALOG_H
#define SUB_HIS_DIALOG_H

#include <QDialog>

namespace Ui {
class Sub_His_Dialog;
}

class Sub_His_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Sub_His_Dialog(QWidget *parent = 0);
    ~Sub_His_Dialog();

public:
    void show_data();

private slots:
    void on_closeBtn_clicked();

private:
    Ui::Sub_His_Dialog *ui;
};

#endif // SUB_HIS_DIALOG_H
