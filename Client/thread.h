#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <string>
#include <QTimer>
#include <mutex>
using namespace  std ;

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(string run_id_,string prob_id_);
    void query_res();
    void stop();
    void run();
protected:

private:
    mutex mu;
    bool stopped;
    char res_of_query[50];
    string run_id;
    string prob_id;
    QTimer *m_pTimer;
};

#endif // THREAD_H
