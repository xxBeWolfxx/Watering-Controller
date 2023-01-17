//
// Created by ahk on 17.11.22.
//

#include "websocketService.h"



websocketService::websocketService(tcp::socket&& socket) : ws(std::move(socket)) {
    id++;
}


void websocketService::test() {
    std::cout << "TESTcik" << std::endl;
}

void websocketService::process() {

    ws.async_accept(
            [self{shared_from_this()}](beast::error_code ec){

                if(ec){ std::cout << ec.message() << "\n"; return;}

                self->echo();
            });
}

void websocketService::setState(bool status) {
    this->state = status;
}

bool websocketService::getState() {
    return this->state;
}

void websocketService::echo() {
    ws.async_read(
            buffer,
            [self{shared_from_this()}](beast::error_code ec, std::size_t bytes_transferred)
            {
                if(ec == websocket::error::closed){
                    self->ws.close(beast::websocket::close_code::normal, ec);
                    return;
                }


                if(ec){ std::cout << ec.message() << "\n"; return;}

                auto out = beast::buffers_to_string(self->buffer.cdata());
                std::cout<<out<<" "<<std::flush;
                self->ws.async_write(
                        self->buffer.data(),
                        [self](beast::error_code ec, std::size_t bytes_transferred)
                        {

                            if(ec){ std::cout << ec.message() << "\n"; return;}

                            self->buffer.consume(self->buffer.size());

                            self->echo();
                        });
            });
}



ListenerWebsocket::ListenerWebsocket(net::io_context &ioc, std::string ipAddress, unsigned short port) :
        ioc(ioc), acceptor(ioc, {net::ip::make_address(ipAddress), port}){

}

void ListenerWebsocket::asyncAccpet() {
    acceptor.async_accept(ioc, [self{shared_from_this()}](boost::system::error_code ec, tcp::socket socket){
        auto ptr = std::make_shared<websocketService>(std::move(socket));
        self->ptrVector.push_back(ptr);
        ptr->process();
        self->asyncAccpet();

    });

}