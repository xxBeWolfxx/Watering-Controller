#include <iostream>
#include "Database.h"
#include "websocketService.h"
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

int main() {
    DataRobot date = {};
    std::cout << "Hello, World!" << std::endl;

//    std::string address = "127.0.0.1";
//
//    websocketService serverRPi = websocketService(address, 8083);

    auto const address = net::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(std::atoi("8083"));

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};


    return 0;
}
