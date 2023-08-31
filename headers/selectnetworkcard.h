/*

* Copyright:Cfreeze

* Author: Cfreeze

* Date:2023-04-11

* Description: 网卡选择类

*/

#ifndef SELECTNETWORKCARD_H
#define SELECTNETWORKCARD_H

#include"mynpacp.h"

class selectnetworkcard
{
public:
    selectnetworkcard();
    pcap_if_t* getInformation();

private:
    mynpacp* mynpacp_  = new mynpacp();
    pcap_if_t* list;
};

#endif // SELECTNETWORKCARD_H
