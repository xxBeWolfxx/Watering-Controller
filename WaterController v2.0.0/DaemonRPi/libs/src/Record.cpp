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
    this->timestampOfLastMessage = std::time(0);
}

ESP_unit::~ESP_unit() {
//    this->websocketESP.reset();
//    std::cout << "TEST" << std::endl;
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

std::uint8_t ESP_unit::check_it_is_exist(vector<ESP_unit> esp_modules, shared_ptr<WebsocketService> &ptr) {


    return 0;
}


