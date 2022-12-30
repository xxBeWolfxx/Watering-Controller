//
// Created by ahk on 17.11.22.
//

#ifndef DAEMONRPI_WEBSOCKETSERVICE_H
#define DAEMONRPI_WEBSOCKETSERVICE_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class websocketService {
private:
    websocket::stream<tcp::socket> *ws;
    tcp::socket *socket;
    tcp::acceptor *acceptor;
    bool state;


public:
    net::io_context ioc{1};
    net::ip::address address = {};
    uint32_t port;


    websocketService(std::string ipAddress, uint32_t port);

    void setState(bool status);
    bool getState();

    static void process(websocketService *websocket);
    void handshake();
    void test();


};


#endif //DAEMONRPI_WEBSOCKETSERVICE_H
