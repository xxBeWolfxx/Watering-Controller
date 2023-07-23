#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <cstdint>
#include "Database.h"
#include "WebsocketService.h"
#include "Record.h"
#include "Configuration.h"
#include "ListenerWebsocket.h"

using namespace std;

void websocketTask(net::io_context *ioc) {
    sleep(2);
    ioc->run();
}

Database *Record::database;

int main() {
    Configuration config = Configuration("config.txt");
    config.ReadSetting();

    net::io_context ioc{};
    std::thread taskWebsocketDeamon(websocketTask, &ioc);



    shared_ptr<ListenerWebsocket> ptr_ListenerWebsocket = std::make_shared<ListenerWebsocket>(ioc, "127.0.0.1",config.config.port);
    ListenerWebsocket *listiener = ptr_ListenerWebsocket.get();

    Database database = Database();

//    database.Select_all_data("PLANT", data);

    vector<shared_ptr<ESP_unit>> vecESP;
    listiener->assignVectorWebsocket(vecESP);
    Record::database = &database;


    Configuration::codeCycle status = Configuration::STARTING;

    while(1){

        switch (status) {

            case Configuration::STARTING:{

                status = Configuration::OPEN_WEBSOCKET;
                break;
            }
            case Configuration::OPEN_WEBSOCKET:{

                taskWebsocketDeamon.detach();
                ptr_ListenerWebsocket->asyncAccpet();

                status = Configuration::VALIDATION_INCOMING_MSG;
                break;
            }

            case Configuration::VALIDATION_INCOMING_MSG:{
                listiener->delete_all_not_working_ESP();
                vector<string> temp;


                for(auto &item : vecESP){
                    if (item->check_message_status()){
                        item->validate_incoming_messages();
                    }
                }

                if(!temp.empty())
                {
                    cout << "STATUS: " << status << endl;
                }


//                status = (Configuration::codeCycle)(status + 1);
                break;
            }


            case Configuration::OPEN_DATABASE:
                break;
            case Configuration::SENDING_TO_ESP:
                break;
        }




    }


    return 0;
}
