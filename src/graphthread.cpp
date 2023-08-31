#include "headers/graphthread.h"
#include "headers/moniterthread.h"
#include <QDebug>

graphThread::graphThread(QObject *parent) : QThread(parent)
{
    //qDebug() << "graphThread chaungjian";
}


void graphThread::run()
{
    //qDebug() << "graphThread run";
    //qDebug() << "graphThreadcurrentThreadId"  << currentThreadId();
    QString ss;
    while(1){
        //qDebug() << "graphThread run!!!";
        Sleep(4000);
        ss = ::link.getList();
        emit sendNum(ss);
        //qDebug() << "graphThread runend";
    }
}

