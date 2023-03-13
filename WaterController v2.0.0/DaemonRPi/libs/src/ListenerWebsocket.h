//
// Created by ahk on 08.03.23.
//

#ifndef DAEMONRPI_LISTENERWEBSOCKET_H
#define DAEMONRPI_LISTENERWEBSOCKET_H

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "WebsocketService.h"
#include "Record.h"

using namespace std;

class ListenerWebsocket : public std::enable_shared_from_this<ListenerWebsocket>{
    net::io_context &ioc;
    tcp::acceptor acceptor;



public:
    std::vector<ESP_unit> *ptrVector;


    ListenerWebsocket(net::io_context& ioc, std::string ipAddress, unsigned short int port);

    void assignVectorWebsocket(std::vector<std::shared_ptr<WebsocketService>> *ptr);
    void asyncAccpet();

    void check_all_pointers();
    void get_all_messages(std::vector<std::string> &payload);



};



#endif //DAEMONRPI_LISTENERWEBSOCKET_H
