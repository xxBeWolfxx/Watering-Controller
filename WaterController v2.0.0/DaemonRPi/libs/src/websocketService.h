//
// Created by ahk on 17.11.22.
//

#ifndef DAEMONRPI_WEBSOCKET_H
#define DAEMONRPI_WEBSOCKET_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
// g++ -I /usr/include/boost -pthread websocket.cpp
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class websocket {

};


#endif //DAEMONRPI_WEBSOCKET_H
