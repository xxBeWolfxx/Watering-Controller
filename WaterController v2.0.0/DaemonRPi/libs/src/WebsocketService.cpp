//
// Created by ahk on 17.11.22.
//

#include "WebsocketService.h"



WebsocketService::WebsocketService(tcp::socket&& socket) : ws(std::move(socket)) {
    this->state = true;
}


void WebsocketService::test() {
    std::cout << "TESTcik" << std::endl;
}

void WebsocketService::process() {

    ws.async_accept(
            [self{shared_from_this()}](beast::error_code ec){

                if(ec){
                    self->state = false;
                    std::cout << ec.message() << "\n"; return;
                }

                self->echo();
            });
}

void WebsocketService::setState(bool status) {
    this->state = status;
}

bool WebsocketService::getState() {
    return this->state;
}

void WebsocketService::echo() {

    ws.async_read(
            buffer,
            [self{shared_from_this()}](beast::error_code ec, std::size_t bytes_transferred)
            {
                if(ec == websocket::error::closed){
                    self->state = false;
                    self->ws.close(beast::websocket::close_code::normal, ec);
                    return;
                }


                if(ec){
                    self->state = false;
                    std::cout << ec.message() << "\n"; return;
                }

                auto out = beast::buffers_to_string(self->buffer.cdata());
                std::cout<<out<<" "<<std::flush;
                self->ws.async_write(
                        self->buffer.data(),
                        [self](beast::error_code ec, std::size_t bytes_transferred)
                        {

                            if(ec){
                                self->state = false;
                                std::cout << ec.message() << "\n"; return;
                            }

                            self->buffer.consume(self->buffer.size());

                            self->echo();
                        });
            });
}

WebsocketService::~WebsocketService() {
    std::cout << "DELETING" << std::endl;
}


ListenerWebsocket::ListenerWebsocket(net::io_context &ioc, std::string ipAddress, unsigned short port) :
        ioc(ioc), acceptor(ioc, {net::ip::make_address(ipAddress), port}){

}

void ListenerWebsocket::asyncAccpet() {
    acceptor.async_accept(ioc, [self{shared_from_this()}](boost::system::error_code ec, tcp::socket socket){
        auto ptr = std::make_shared<WebsocketService>(std::move(socket));
        self->ptrVector.push_back(ptr);
        ptr->process();
        self->checkAllPointers();
        self->asyncAccpet();

    });


}

void ListenerWebsocket::checkAllPointers() {
    std::vector<uint8_t> positionOfItem;
    bool itemFound = false;
    uint8_t iterator = 0;
    for ( std::shared_ptr<WebsocketService> ptr : this->ptrVector){
        auto item = ptr.get();
        if(!item->getState()){
            ptr.reset();
            ptr = nullptr;
            itemFound = true;
            positionOfItem.push_back(iterator);
        }
        iterator++;
    }

    if (itemFound){
        std::reverse(positionOfItem.begin(), positionOfItem.end());
        for (uint8_t item : positionOfItem){
            this->ptrVector.erase(ptrVector.begin() + item);
        }
    }

}
