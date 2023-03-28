//
// Created by ahk on 05.03.23.
//

#include "Record.h"

#include <utility>


uint16_t Record::get_id() {
    return this->ID;
}

Record::Record() {
//    this->database = database;
//    this->ID = id;

}


ESP_unit::ESP_unit() : Record() {
    this->status = ESP_STATUS::INIT;
    this->timestampOfLastMessage = std::time(0);
}

ESP_unit::~ESP_unit() {
//    this->websocketESP.reset();
   std::cout << "ESP DEL" << std::endl;
}


uint8_t ESP_unit::get_record(vector<string> &data) {
    string *comand = new string;
    *comand = "SELECT * FROM ESP WHERE id=" + to_string(this->ID);
    this->database->SelectData(comand, data);
    delete comand;

    if(data.empty()){
        return 0;
    }


    return 1;

}

uint8_t ESP_unit::put_record() {
    string *values = new string;
    string *columns = new string;
    string *table = new string;

    *values = to_string(this->ID) + "," + to_string(this->timestampOfLastMessage);
    *columns = "ID, timestamp_last_msg";
    *table = "ESP";


    database->InsertData(table, columns, values);

    delete values;
    delete columns;
    delete table;

}

void ESP_unit::assign_values(string data) {


}

void ESP_unit::assign_pointer_websocket(std::shared_ptr<WebsocketService> ptr){
    this->websocketESP = std::move(ptr);

}

bool ESP_unit::check_message_status() {
    return this->websocketESP->new_message_appeared;
}

void ESP_unit::message_validation() {

    switch(this->status){

        case INIT: {
            std::stringstream message;
            message.str(this->websocketESP->getContent());
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(message, pt);
            this->ID = pt.get<std::uint16_t>("ID");

            this->status = ESP_STATUS::CHECKING_DATABASE;
            break;
        }
        case CHECKING_DATABASE: {
            this->websocketESP->new_message_appeared = false;
            this->status = ESP_STATUS::WORKING;
            break;
        }

//        case IN_DATABASE:
//            break;
//        case NEW_ELEMENT:
//            break;
        case WORKING: {
            std::uint8_t x = 10;
            this->websocketESP->new_message_appeared = false;
            this->websocketESP->sy_write("CHECKING");
            sleep(2);
            break;
        }
    }

}



