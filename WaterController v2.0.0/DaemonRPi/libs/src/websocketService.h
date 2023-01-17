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
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class websocketService : public std::enable_shared_from_this<websocketService>{
private:
    websocket::stream<tcp::socket> ws;
    beast::flat_buffer buffer;

    bool state;


public:
    static int id;
    net::ip::address address = {};
    uint32_t port;


    websocketService(tcp::socket&& socket);

    void setState(bool status);
    bool getState();

    void process();
    void echo();
    void test();


};

class ListenerWebsocket : public std::enable_shared_from_this<ListenerWebsocket>{
    net::io_context &ioc;
    tcp::acceptor acceptor;
    std::shared_ptr<websocketService> listOfWebsockets[10];


public:
    std::vector<std::shared_ptr<websocketService>> ptrVector;
    ListenerWebsocket(net::io_context& ioc, std::string ipAddress, unsigned short int port);
    void asyncAccpet();

};




#endif //DAEMONRPI_WEBSOCKETSERVICE_H
