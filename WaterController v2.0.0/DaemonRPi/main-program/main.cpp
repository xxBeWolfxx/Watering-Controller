#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <cstdint>
#include "Database.h"
#include "websocketService.h"
#include <Configuration.h>

using namespace std;




int main() {
    Configuration config = Configuration("config.txt");
//    websocketService server_webSocket = websocketService("192.168.0.170", 8083);


    config.ReadSetting();
    auto const port = 8083;
    net::io_context ioc{};

    auto test = std::make_shared<ListenerWebsocket>(ioc, "192.168.0.170",port);

    test->asyncAccpet();

    ioc.run();



    return 0;
}
