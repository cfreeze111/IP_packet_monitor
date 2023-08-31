#include "headers/mynpacp.h"

#define FUN(S,N) std::string(N - S.size(),'0') + S

pcap_if_t*  allNetwork = nullptr;     // 网卡信息
int   size = 0;           // 网卡数量
char   errbuf[SIZE];   // 错误信息
pcap_t*   sniff = nullptr;
struct pcap_pkthdr* packHeader = nullptr;     // 数据包头
const u_char* packData = nullptr;       // 数据报
std::vector<struct ip_header*> data_head_;
ip_parse* ip_protocal;


mynpacp::mynpacp(){}

char buf[SIZE / 2];
template <class T>
std::string dataToString(T data,int radix) {
    memset(buf,0x00,sizeof buf);
    itoa(data,buf,radix);
    return buf;
}

// 点分十进制
std::string iptos(long in)
{
    u_char *p;
    p = (u_char *)&in;
    memset(buf,0x00,sizeof buf);
    sprintf(buf, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return buf;
}

// 获取网卡信息
bool mynpacp::infoNetwork(){
    if(::allNetwork) closeNetwork();
    ::allNetwork = nullptr;
    memset(errbuf, 0x00, sizeof (errbuf));
    int ret = pcap_findalldevs_ex(PCAP_SRC_IF_STRING,NULL,&(::allNetwork),::errbuf);
    if(ret == -1){
        return false;
    }
    pcap_if_t* cur = ::allNetwork;
    ::size = 0;
    while(cur){
        cur = cur ->next;
        ::size++;
    }
    return true;
}

// 释放网卡信息
bool mynpacp::closeNetwork() {
    pcap_freealldevs(allNetwork);
    return true;
}

// 解析网络层
std::string mynpacp::ParseNetWork(const u_char* packData) {
    struct ip_hander {
        u_char      _version_handerLen;     // 版本 4  首部长度 4
        u_char      _diffserv;              // 服务类型
        u_short     _totalLen;              // 总长度
        u_short     _identification;        // 标识
        u_short     _flag_offset;           // 标志 3 + 片偏移 13
        u_char      _timeLive;              // 生存时间
        u_char      _protocol;              // 协议
        u_short     _checkSum;              // 首部校验和
        long        _src;                   // 源地址
        long        _desc;                  // 目的地址
    };

    ip_hander* handl = (ip_hander*)(::packData + 14);
    ::ip_protocal = new ip_parse();

    // ip 版本 ipv4  ipv6
    if((handl->_version_handerLen & (0x4)) == 0x4) (::ip_protocal)->_version = "ipv4";
    else if((handl->_version_handerLen & (0x6)) == 0x6) (::ip_protocal)->_version = "ipv6";
    else (::ip_protocal)->_version = "ipvx";

    char len = handl->_version_handerLen & 0x0f;
    (::ip_protocal)->_handerLen = "0x" + dataToString(len,16);                              // 首部长度
    (::ip_protocal)->_diffserv =  "0x" + dataToString(handl->_diffserv,16);                 // 服务类型
    (::ip_protocal)->_totalLen =  "0x" + dataToString(handl->_totalLen,16);                 // 总长度
    (::ip_protocal)->_identification =  "0x" + dataToString(handl->_identification,16);     // 标识

    char flag = (handl->_flag_offset) >> 13;
    std::string fRet = dataToString(flag,2);
    fRet = std::string(3-fRet.size(),'0') + fRet; // 补前置0
    char offset = handl->_flag_offset & 0x1fff;
    std::string oRet = dataToString(offset,16);
    oRet = std::string(4 - oRet.size(),'0') + oRet;
    (::ip_protocal)->_flag_offset = fRet + "0x" +oRet;                   // 标志 3 片偏移 13
    (::ip_protocal)->_timeLive = "0x" + dataToString(handl->_timeLive,16);      // 生存时间
    (::ip_protocal)->_protocol = dataToString(handl->_protocol,10);             // 协议
    (::ip_protocal)->_checkSum = "0x" + dataToString(handl->_checkSum,16);      // 首部校验和
    (::ip_protocal)->_src   = iptos(handl->_src);                               // 源地址
    (::ip_protocal)->_desc  = iptos(handl->_desc);                              // 目标地址

    (::ip_protocal)->_str[0] = "0x" + FUN(dataToString(handl->_version_handerLen,16),2) + " " + FUN(dataToString(handl->_diffserv,16),2) + " " + FUN(dataToString(handl->_totalLen,16),4);
    (::ip_protocal)->_str[1] = "0x" + FUN(dataToString(handl->_identification,16),4) + " " + FUN(dataToString(handl->_flag_offset,16),4);
    (::ip_protocal)->_str[2] = "0x" + FUN(dataToString(handl->_timeLive,16),2) + " " + FUN(dataToString(handl->_protocol,16),2) + " " + FUN(dataToString(handl->_checkSum,16),4);

    std::string s = (::ip_protocal)->_version + " " +
                    (::ip_protocal)->_handerLen + " " +
                    (::ip_protocal)->_diffserv  + " " +
                    (::ip_protocal)->_totalLen  + " " +
                    (::ip_protocal)->_identification  + " " +
                    (::ip_protocal)->_flag_offset  + " " +
                    (::ip_protocal)->_timeLive  + " " +
                    (::ip_protocal)->_protocol  + " " +
                    (::ip_protocal)->_checkSum  + " " +
                    (::ip_protocal)->_src  + " " +
                    (::ip_protocal)->_desc;
    //qDebug() << QString::fromStdString(s);
    return s;

//    qDebug() << "版本::" << QString::fromStdString((::ip_protocal)->_version);
//    qDebug() << "首部长度::" << QString::fromStdString((::ip_protocal)->_handerLen);
//    qDebug() << "服务类型::" << QString::fromStdString((::ip_protocal)->_diffserv);
//    qDebug() << "总长度::" << QString::fromStdString((::ip_protocal)->_totalLen);
//    qDebug() << "标识::" << QString::fromStdString((::ip_protocal)->_identification);
//    qDebug() << "标志 3 片偏移 13::" << QString::fromStdString((::ip_protocal)->_flag_offset);
//    qDebug() << "生存时间::" << QString::fromStdString((::ip_protocal)->_timeLive);
//    qDebug() << "协议::" << QString::fromStdString((::ip_protocal)->_protocol);
//    qDebug() << "首部校验和::" << QString::fromStdString((::ip_protocal)->_checkSum);
//    qDebug() << "src::" << QString::fromStdString((::ip_protocal)->_src);
//    qDebug() << "dst::"  << QString::fromStdString((::ip_protocal)->_desc);
}



