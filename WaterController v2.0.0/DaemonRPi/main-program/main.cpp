#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <cstdint>
#include "Database.h"
#include "WebsocketService.h"
#include <Configuration.h>

using namespace std;

void websocketTask(net::io_context *ioc){
    sleep(1);
    ioc->run();
}


int main() {
    Configuration config = Configuration("config.txt");
    net::io_context ioc{};
    std::thread taskWebsocketDeamon (websocketTask, &ioc);
//    WebsocketService server_webSocket = WebsocketService("192.168.0.170", 8083);


    config.ReadSetting();
    auto const port = 8083;


    auto test = std::make_shared<ListenerWebsocket>(ioc, "127.0.0.1",port);

    test->asyncAccpet();

    taskWebsocketDeamon.detach();



    for(;;){

    }

    return 0;
}
