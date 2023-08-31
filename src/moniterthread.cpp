#include "headers/moniterthread.h"
#include <iostream>
#include <Winsock2.h>
#include <QDebug>

using namespace std;
#pragma warning( disable : 4996 )//要使用旧函数
nodeList link;

moniterThread::moniterThread(QObject *parent) : QThread(parent)
{
    //qDebug() << "moniterThread chuangjian ";
    lockFlag_ = 0;
}

void moniterThread::revIdx(int idx)
{
    idx_ = idx;
}
void moniterThread::lock()
{
    lockFlag_ = 1;
    lock_.lock();
}
void moniterThread::unlock()
{
    lockFlag_ = 0;
    lock_.unlock();
}

void moniterThread::run()
{
    //qDebug() << "moniterThread run ";
    //qDebug() << "moniterThreadcurrentThreadId" << currentThreadId();
    moniter();
}

void moniterThread::moniter()
{
   //打开网卡获取包
   //指向用户选择网卡
   pcap_if_t* ptr;
   int i;
   for (ptr = ::allNetwork, i = 0; i < idx_ - 1; ptr = ptr->next, i++);
   //获取网卡信息
   pcap_freealldevs(::allNetwork);

   //统计捕获的IP包

    int retValue = 0;
    double d;
    A:
    while (retValue = pcap_next_ex(sniff, &(::packHeader), &(::packData)) >= 0)
    {
        //qDebug() << "moniter run !!!!";

          if (retValue == 0)
              continue;
          //printf("监听长度：%d\n", packHeader->len);
          typedef struct ip_header
          {
              char version : 4;
              char headerlength : 4;
              char cTOS;
              unsigned short totla_length;
              unsigned short identification;
              unsigned short flags_offset;
              char time_to_live;
              char Protocol;
              unsigned short check_sum;
              unsigned int SrcAddr;
              unsigned int DstAddr;
          };

          // +14 跳过数据链路层
          if( ::packData==nullptr){
              goto A;
          }

          std::string network = mynpacp_->ParseNetWork(packData);

          emit sendNetWork(QString::fromStdString(network));

          struct ip_header* ip= (struct ip_header*)(packData + 14);

          SOCKADDR_IN Src_Addr, Dst_Addr = { 0 };

          u_short check_sum = ntohs(ip->check_sum);
          uchar ttl = ip->time_to_live;
          int proto = ip->Protocol;

          Src_Addr.sin_addr.s_addr = ip->SrcAddr;
          Dst_Addr.sin_addr.s_addr = ip->DstAddr;

          std::string protocol;
          switch (ip->Protocol)
          {
               case 1: protocol="ICMP"; break;
               case 2: protocol="IGMP"; break;
               case 6: protocol="TCP";  break;
               case 17: protocol="UDP"; break;
               case 89: protocol="OSPF"; break;
               default: protocol="None"; break;
          }

          std::string s = inet_ntoa(Src_Addr.sin_addr);
          std::string s1 = inet_ntoa(Dst_Addr.sin_addr);
          std::string s2 = std::to_string(ttl);

          string message = s + " " + s1 + " " + protocol + " " + s2;//源地址，目标地址，协议类型，TTL
          QString qmessage = QString::fromStdString(message);
          lock_.lock();
          ::link.addNode(s,message);
          lock_.unlock();
          //传递
          emit sendMessage(qmessage);
          //qDebug() << "moniter run end";
          Sleep(100);
    }
}


