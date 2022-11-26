#include <iostream>
#include "Database.h"
#include "websocketService.h"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>




int main() {
    DataRobot date = {};
    std::cout << "Hello, World!" << std::endl;

    websocketService server_webSocket = websocketService("192.168.0.170", 8083);

    tcp::acceptor acceptor{server_webSocket.ioc, {server_webSocket.address, (unsigned short)server_webSocket.port}};


    try {

        for (;;) {
            tcp::socket socket{server_webSocket.ioc};

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread t1(&websocketService::process, std::move(socket));
            t1.detach();
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
