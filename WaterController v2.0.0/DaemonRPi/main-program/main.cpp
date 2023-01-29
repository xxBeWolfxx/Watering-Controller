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

void websocketTask(net::io_context *ioc) {
    sleep(2);
    ioc->run();
}


int main() {
    Configuration config = Configuration("config.txt");
    config.ReadSetting();

    net::io_context ioc{};
    std::thread taskWebsocketDeamon(websocketTask, &ioc);

    shared_ptr<ListenerWebsocket> ptr_ListenerWebsocket = std::make_shared<ListenerWebsocket>(ioc, "127.0.0.1",config.config.port);
    ptr_ListenerWebsocket->asyncAccpet();
    ListenerWebsocket *listiener = ptr_ListenerWebsocket.get();

    taskWebsocketDeamon.detach();


    //TESTING PART
    std::uint8_t i = 0;

    for (;;) {
        if (i > 10) {


            listiener->check_all_pointers();
            i = 0;
        }
        vector<string> temp;

        listiener->get_all_messages(temp);
        sleep(1);
        i++;

    }

    return 0;
}
