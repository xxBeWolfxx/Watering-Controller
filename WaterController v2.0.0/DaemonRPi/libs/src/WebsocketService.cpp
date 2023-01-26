//
// Created by ahk on 17.11.22.
//

#include "WebsocketService.h"



WebsocketService::WebsocketService(tcp::socket&& socket) : ws(std::move(socket)) {
    this->state = true;
    this->new_message_appeared = false;
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

    while(this->new_message_appeared){

    }

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

                    std::string out = beast::buffers_to_string(self->buffer.cdata());
                    std::cout<<out<<" "<<std::flush;
                    self->buffer.consume(self->buffer.size());

                    self->new_message_appeared = true;

                    self->asy_write("{\"Status\":\"OK\"}");

                    self->echo();

                });




}

WebsocketService::~WebsocketService() {
    std::cout << "DELETING" << std::endl;
}

void WebsocketService::asy_write(std::string msg) {
    this->ws.async_write(
            net::buffer(msg),
            [this](beast::error_code ec, std::size_t bytes_transferred)
            {

                if(ec){
                    this->state = false;
                    std::cout << ec.message() << "\n"; return;
                }


            });
}

std::string WebsocketService::getContent() {
    return this->content;
}

void WebsocketService::sy_write(std::string msg) {
    this->ws.write(net::buffer(msg));

}


ListenerWebsocket::ListenerWebsocket(net::io_context &ioc, std::string ipAddress, unsigned short port) :
        ioc(ioc), acceptor(ioc, {net::ip::make_address(ipAddress), port}){

}

void ListenerWebsocket::asyncAccpet() {
    acceptor.async_accept(ioc, [self{shared_from_this()}](boost::system::error_code ec, tcp::socket socket){
        auto ptr = std::make_shared<WebsocketService>(std::move(socket));
        self->ptrVector.push_back(ptr);
        ptr->process();
        self->asyncAccpet();

    });


}

void ListenerWebsocket::check_all_pointers() {
    std::vector<uint8_t> positionOfItem;
    bool itemFound = false;
    uint8_t iterator = 0;
    for ( std::shared_ptr<WebsocketService> ptr : this->ptrVector){
        WebsocketService *item = ptr.get();

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

void ListenerWebsocket::get_all_messages(std::vector<std::string> &payload) {
    for(std::shared_ptr<WebsocketService> ptr : this->ptrVector){
        WebsocketService *item = ptr.get();

        if(item->new_message_appeared){
            payload.push_back(item->getContent());
            item->new_message_appeared = false;
        }
    }
}
