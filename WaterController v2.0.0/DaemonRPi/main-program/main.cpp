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
    websocketService server_webSocket = websocketService("192.168.0.170", 8083);


    config.ReadSetting();
    uint8_t status = 1;

    server_webSocket.handshake();
    thread taskWebsocket(&websocketService::process, &server_webSocket);
    taskWebsocket.detach();

    while(1){
        int x = 1;
    }


    return 0;
}
