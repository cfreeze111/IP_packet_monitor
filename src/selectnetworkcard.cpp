#include "headers/selectnetworkcard.h"

selectnetworkcard::selectnetworkcard(){
    list = nullptr;
}

pcap_if_t* selectnetworkcard::getInformation(){
   if( mynpacp_ ->infoNetwork() == true ){
       list = ::allNetwork;
       return list;
   }
}
