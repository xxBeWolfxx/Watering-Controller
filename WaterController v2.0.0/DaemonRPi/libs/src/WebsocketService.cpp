//
// Created by ahk on 17.11.22.
//

#include "WebsocketService.h"
#include "Record.h"



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

void WebsocketService::assignClientIP(std::string ip) {
    this->clientIPaddress = ip;
}



