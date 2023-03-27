//
// Created by ahk on 09.03.23.
//
#include "ListenerWebsocket.h"

ListenerWebsocket::ListenerWebsocket(net::io_context &ioc, std::string ipAddress, unsigned short port) :
        ioc(ioc), acceptor(ioc, {net::ip::make_address(ipAddress), port}){

}

void ListenerWebsocket::asyncAccpet() {
    acceptor.async_accept(ioc, [self{shared_from_this()}](boost::system::error_code ec, tcp::socket socket){
        std::string ipAddress = socket.remote_endpoint().address().to_string();
        std::shared_ptr<ESP_unit> newUnit = std::make_shared<ESP_unit>();

        newUnit->websocketESP = std::make_shared<WebsocketService>(std::move(socket));
        newUnit->websocketESP->assignClientIP(ipAddress);
        newUnit->websocketESP->process();

        self->ptrVector->push_back(std::move(newUnit));


        self->asyncAccpet();

    });


}

void ListenerWebsocket::delete_all_not_working_ESP() {
    std::vector<uint8_t> positionOfItem;
    bool itemFound = false;
    uint8_t iterator = 0;
    for ( auto &ptr : *this->ptrVector){
        ESP_unit *esp = ptr.get();
        WebsocketService *item = esp->websocketESP.get();

        if(!item->getState()){
            esp->websocketESP.reset();
            ptr.reset();
            itemFound = true;
            positionOfItem.push_back(iterator);
        }
        iterator++;
    }

    if (itemFound){
        std::reverse(positionOfItem.begin(), positionOfItem.end());
        for (uint8_t item : positionOfItem){
            this->ptrVector->erase(ptrVector->begin() + item);
        }
    }

}

void ListenerWebsocket::get_all_messages(std::vector<std::string> &payload) {
    for(std::shared_ptr<ESP_unit> &ptr : *this->ptrVector){
        ESP_unit *esp = ptr.get();
        WebsocketService *item = esp->websocketESP.get();

        if(item->new_message_appeared){
            payload.push_back(item->getContent());
            item->new_message_appeared = false;
        }
    }
}

void ListenerWebsocket::assignVectorWebsocket(std::vector<std::shared_ptr<ESP_unit>> &ptr) {
    this->ptrVector = &ptr;
}
