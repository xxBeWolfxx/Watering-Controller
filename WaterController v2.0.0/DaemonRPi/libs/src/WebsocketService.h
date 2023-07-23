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

    std::string clientIPaddress;


public:
    net::ip::address address = {};
    uint32_t port;
    std::string content;

    bool new_message_appeared;

    explicit WebsocketService(tcp::socket&& socket);
    ~WebsocketService();

    void assignClientIP(std::string ip);
    void setState(bool status);
    bool getState() const;
    std::string getContent();
    std::string getIPaddress();

    void process();
    void echo();
    void asy_write(std::string msg);
    void sy_write(std::string msg);
    void asyn_close();

};





#endif //DAEMONRPI_WEBSOCKETSERVICE_H
