/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 网卡信息类

*/

#ifndef MYNPACP_H
#define MYNPACP_H

#include "pcap.h"
#include <winsock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <QString>
#include <QDebug>

#define SIZE 1024

extern pcap_if_t*  allNetwork;     // 网卡信息
extern int   size;           // 网卡数量
extern char   errbuf[SIZE];   // 错误信息
extern pcap_t*   sniff;      // 嗅探
extern struct pcap_pkthdr*      packHeader;     // 数据包头
extern const u_char*            packData;       // 数据报
extern std::vector<struct ip_header*> data_head_;


struct ip_header //IP首部

{
    uint8_t ip_header_length : 4,//首部长度
    ip_version : 4;//版本
    uint8_t tos;         //服务类型
    uint16_t total_length;  //总长度
    uint16_t ip_id;         //标识identification
    uint16_t ip_offset;        //片偏移
    uint8_t ttl;            //生存时间
    uint8_t ip_protocol;     //协议类型（TCP或者UDP协议）
    uint16_t ip_checksum;  //首部检验和
    uint16_t cal_checksum();//手动计算首部检验和
    struct in_addr  ip_source_address; //源IP struct表示一个32位的IPv4地址
    struct in_addr ip_destination_address; //目的IP
};

// ip 协议
struct ip_parse {
    std::string      _version;           // 版本
    std::string      _handerLen;         // 首部长度
    std::string      _diffserv;          // 区分服务
    std::string      _totalLen;          // 总长度
    std::string      _identification;    // 标识
    std::string      _flag_offset;       // 标志 3 + 片偏移 13
    std::string      _timeLive;          // 生存时间
    std::string      _protocol;          // 协议
    std::string      _checkSum;          // 首部校验和
    std::string      _src;               // 源地址
    std::string      _desc;              // 目的地址
    std::string      _str[4];            // 16进制字符串信息数组
};
extern ip_parse* ip_protocal;       // ip 协议

class mynpacp
{

public:
    mynpacp();

    // 获取网卡信息
    bool infoNetwork();
    // 释放网卡信息
    bool closeNetwork();
    // 网络层解析
    std::string ParseNetWork(const u_char* packData);

};


class ipNode
{
private:
    std::string sourceAddress;
    int num;

public:
    ipNode* next;

    ipNode(std::string sourceAdd)
    {
        sourceAddress = sourceAdd;
        num = 1;
        next = NULL;
    }
    void addNum()
    {
        num++;
    }

    std::string getsourceAddress()
    {
        return sourceAddress;
    }

    int getNum()
    {
        return num;
    }
};

class nodeList
{
    ipNode* head;
    ipNode* tail;
    int length;
    std::vector<std::string> message;

public:
    nodeList()
    {
        head = tail = NULL;
        length = 0;
    }
    ~nodeList()
    {
        if (head != NULL)
        {
            ipNode* temp = head;
            head = head->next;
            delete temp;
        }
    } 
    void addNode(std::string sourceAddress,std::string message)//, u_char*  packData)
    {
        this->message.push_back(message);
        if (head == NULL)
        {
            tail = new ipNode(sourceAddress);
            head = tail;
            tail->next = NULL;
            length++;
        }
        else
        {
            int flag2 = 0;
            for (ipNode* temp=head; temp; temp = temp->next)
            {
                //如果链表中存在此IP，IP包个数+1
                if (temp->getsourceAddress() == sourceAddress)
                {
                    flag2 = 1;
                    temp->addNum();
                    break;
                }
                //如果链表中没有这个IP，那么加入链表
            }
            if (flag2 == 0)
            {
                length++;
                tail->next = new ipNode(sourceAddress);
                tail = tail->next;
                tail->next = NULL;
            }
        }
    }
    QString getList()
    {
        QString ss,sss;
        for (ipNode* temp = head; temp; temp = temp->next)
        {
            //源地址： temp->getsourceAddress()
            //包的个数：temp->getNum()
            std::string s = (temp->getsourceAddress());
            ss = QString::fromStdString( s + " " + std::to_string(temp->getNum()));
            sss = sss + " " + ss + " ";
           // ip1 ipnum ip2 ipnum
        }
        return sss;
    }

    std::vector<struct ip_header*> getDataHead(){
        return data_head_;
    }

};

#endif // MYNPACP_H
