//
// Created by ahk on 17.11.22.
//

#include "websocketService.h"

websocketService::websocketService(std::string ipAddress, uint32_t port) {

    this->address = net::ip::make_address(ipAddress);
    this->port = port;


}
