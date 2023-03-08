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



class WebsocketService : public std::enable_shared_from_this<WebsocketService>{
private:
    websocket::stream<tcp::socket> ws;
    beast::flat_buffer buffer;

    bool state;
    std::string content;
    std::string clientIPaddress;


public:
    net::ip::address address = {};
    uint32_t port;

    bool new_message_appeared;


    explicit WebsocketService(tcp::socket&& socket);
    ~WebsocketService();

    void assignClientIP(std::string ip);
    void setState(bool status);
    bool getState();
    std::string getContent();

    void process();
    void echo();
    void asy_write(std::string msg);
    void sy_write(std::string msg);


};

class ListenerWebsocket : public std::enable_shared_from_this<ListenerWebsocket>{
    net::io_context &ioc;
    tcp::acceptor acceptor;



public:
    std::vector<std::shared_ptr<WebsocketService>> ptrVector;


    ListenerWebsocket(net::io_context& ioc, std::string ipAddress, unsigned short int port);

    void asyncAccpet();

    void check_all_pointers();
    void get_all_messages(std::vector<std::string> &payload);



};




#endif //DAEMONRPI_WEBSOCKETSERVICE_H
