#ifndef DEBUG_H
#define DEBUG_H

#include <QDialog>

namespace Ui {
class Debug;
}

class Debug : public QDialog
{
    Q_OBJECT

public:
    explicit Debug(QWidget *parent = 0);
    ~Debug();

private slots:
    void on_closeBtn_clicked();

private:
    Ui::Debug *ui;
};

#endif // DEBUG_H
