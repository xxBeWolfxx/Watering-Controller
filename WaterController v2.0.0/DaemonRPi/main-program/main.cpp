#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "Database.h"
#include "websocketService.h"
#include <Configuration.h>




int main() {
    Configuration config = Configuration("config.txt");
    websocketService server_webSocket = websocketService("192.168.0.93", 8083);
    tcp::acceptor acceptor{server_webSocket.ioc, {server_webSocket.address, (unsigned short)server_webSocket.port}};
    config.ReadSetting();



    try {

        for (;;) {
            tcp::socket socket{server_webSocket.ioc};

            // Block until we get a connection
            acceptor.accept(socket);

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
