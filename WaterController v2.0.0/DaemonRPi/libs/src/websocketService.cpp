//
// Created by ahk on 17.11.22.
//

#include "websocketService.h"

websocketService::websocketService(std::string ipAddress, uint32_t port) {

    this->address = net::ip::make_address(ipAddress);
    this->port = port;


}


void websocketService::test() {
    std::cout << "TESTcik" << std::endl;
}

void websocketService::handshake() {

    try {
        this->acceptor = new tcp::acceptor{this->ioc, {this->address, (unsigned short)this->port}};
        this->socket = new tcp::socket{this->ioc};

        // Block until we get a connection
        acceptor->accept(*socket);

        this->ws = new websocket::stream<tcp::socket>{std::move(*socket)};
        this->ws->set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res)
                {
                    res.set(http::field::server,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-sync");
                }));

        // Accept the websocket handshake
        this->ws->accept();
        this->state = true;
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }


}

void websocketService::process(websocketService *websocket) {

    while(websocket->state){
        try{
            beast::flat_buffer buffer;

            // Read a message
            websocket->ws->read(buffer);
            // Echo the message back
            websocket->ws->text(websocket->ws->got_text());
            auto text = net::buffer("TEST");
            websocket->ws->write(text);
            websocket->test();
        }

        catch(beast::system_error const& se)
        {
            // This indicates that the session was closed
            if(se.code() != websocket::error::closed)
                std::cerr << "Error: " << se.code().message() << " ### " << std::endl;
            websocket->state = false;
        }
        catch(std::exception const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    std::cout << "KUPA" << std::endl;



}

void websocketService::setState(bool status) {
    this->state = status;
}

bool websocketService::getState() {
    return this->state;
}



