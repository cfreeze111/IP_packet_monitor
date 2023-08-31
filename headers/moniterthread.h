/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 监控解析线程类

*/

#ifndef MONITERTHREAD_H
#define MONITERTHREAD_H

#include <QThread>
#include <headers/mynpacp.h>
#include <qmutex.h>

extern nodeList link;


class moniterThread : public QThread
{
    Q_OBJECT
public:
    explicit moniterThread(QObject *parent = nullptr);
    void revIdx(int idx);
    void lock();
    void unlock();

protected:
    void run() override;

signals:
    void sendMessage(QString);

    void sendNetWork(QString);

private:
    int idx_;
    int lockFlag_;
    QMutex lock_;
    mynpacp* mynpacp_;
    //监控
    void moniter();
    //解析
    void ParseNetWork(const u_char* packData);

signals:

};

#endif // MONITERTHREAD_H
