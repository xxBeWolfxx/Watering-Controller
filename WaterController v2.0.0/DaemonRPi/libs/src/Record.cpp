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
    this->vectorOfFlowers = std::vector<Flower>(2);
}

ESP_unit::~ESP_unit() {
//    this->websocketESP.reset();
//   std::cout << "ESP DEL" << std::endl;
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

bool ESP_unit::check_message_status() {
    return this->websocketESP->new_message_appeared;
}

void ESP_unit::validate_incoming_messages() {

    switch(this->status){

        case INIT: {

            std::vector<std::string> parameters = { "ID" };
            std::vector<std::string> values;
            this->get_values_from_json(parameters, &values);

            if(!values.empty()){
                this->ID = (uint16_t) std::stoi(values[0]);
            }

            this->status = ESP_STATUS::CHECKING_DATABASE;
            break;
        }
        case CHECKING_DATABASE: {
            std::vector<std::string> data;
            this->get_record(data);

            if(data.empty()){
                this->status = ESP_STATUS::NEW_ELEMENT;
            }
            else{
                this->status = ESP_STATUS::IN_DATABASE;
            }

            this->websocketESP->new_message_appeared = false;

            break;
        }

        case IN_DATABASE: {
            this->status = ESP_STATUS::WORKING;
            break;
        }
        case NEW_ELEMENT: {
            this->status = ESP_STATUS::WORKING;
            break;
        }
        case WORKING: {
            std::uint8_t x = 10;
            this->websocketESP->new_message_appeared = false;
            sleep(2);
            break;
        }
    }

}

void ESP_unit::get_values_from_json(std::vector<std::string> parameters, std::vector<std::string> *containerForValues) {
    std::stringstream message;
    message.str(this->websocketESP->getContent());
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(message, pt);

    for (std::string param : parameters){
        containerForValues->push_back(pt.get<string>(param));
    }



}


Flower::Flower()  {

}
