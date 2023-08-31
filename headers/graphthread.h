/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 绘图线程类

*/

#ifndef GRAPHTHREAD_H
#define GRAPHTHREAD_H

#include <QThread>
#include <qmutex.h>

class graphThread : public QThread
{
    Q_OBJECT
public:
    explicit graphThread(QObject *parent = nullptr);

protected:
    void run() override;
private:
    QMutex lock_;

signals:
    void sendNum(QString);

};

#endif // GRAPHTHREAD_H
