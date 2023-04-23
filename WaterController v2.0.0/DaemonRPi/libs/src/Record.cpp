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

std::vector<std::string> Record::split_record_to_seprate_values(std::string record, char symbole) {
    std::vector<std::string> output = std::vector<std::string>();
    std::stringstream input(record);
    std::string segment;

    while(std::getline(input, segment, symbole))
    {
        output.push_back(segment);
    }

    return output;
}


ESP_unit::ESP_unit() : Record() {
    this->status = ESP_STATUS::INIT;
    this->timestampOfLastMessage = std::time(0);
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

uint8_t ESP_unit::create_record_in_database() {
    string *values = new string;
    string *columns = new string;
    string *table = new string;

    *values = to_string(this->ID) + ",'" + this->websocketESP->getIPaddress() + "','" + this->name + "'," + to_string(this->timestampOfLastMessage);
    *columns = "ID, IP_ADDRESS, NAME, TIMESTAMP_LAST_MSG";
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

            std::vector<std::string> parameters = { "ID", "NAME" };
            std::vector<std::string> values;
            this->get_values_from_json(parameters, &values);

            if(!values.empty()){
                this->ID = (uint16_t) std::stoi(values[0]);
                this->name = values[1];
            }

            this->status = ESP_STATUS::CHECKING_DATABASE;
            break;
        }
        case CHECKING_DATABASE: {
            std::vector<std::string> data;
            this->get_record(data);

            if(data.empty()){
                this->status = ESP_STATUS::NEW_ELEMENT;
                this->websocketESP->sy_write("{\"STATUS\": 101}");
            }
            else{
                this->status = ESP_STATUS::IN_DATABASE;
            }



            break;
        }

        case IN_DATABASE: {
            this->get_all_flowers_from_database();
            this->status = ESP_STATUS::WORKING;
            break;
        }
        case NEW_ELEMENT: {
            this->create_record_in_database();
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

void ESP_unit::get_all_flowers_from_database() {
    Flower tempFlower = Flower(this->ID);
    std::vector<std::string> data;

    tempFlower.get_all_record_with_id_esp(data);
    this->assign_values_to_vector_flowers(data);
    uint8_t test = 0;





}

void ESP_unit::assign_values_to_vector_flowers(vector<string> &data) {

    for ( std::string &item : data){
        Flower tempFlower = Flower(this->ID);
        auto values = tempFlower.split_record_to_seprate_values(item, '/');
        tempFlower.id = (uint16_t) stoi(values[0]);
        tempFlower.namePlant = values[2];
        tempFlower.get_recipe(values[3]);



        this->vectorOfFlowers.push_back(std::move(tempFlower));


    }

    // "{1}{1}{Rose}{[22,45,45]}"

}


Flower::Flower()  {

}

Flower::~Flower() {

}

uint8_t Flower::get_record(vector<string> &data) {
    string *comand = new string;
    *comand = "SELECT * FROM PLANT WHERE id=" + to_string(this->ID);
    this->database->SelectData(comand, data);
    delete comand;

    if(data.empty()){
        return 0;
    }


    return 1;
}

uint8_t Flower::get_all_record_with_id_esp(vector<string> &data) {
    string *comand = new string;
    *comand = "SELECT * FROM PLANT WHERE ID_ESP=" + to_string(this->espID);
    this->database->SelectData(comand, data);
    delete comand;

    if(data.empty()){
        return 0;
    }
    return 0;
}

void Flower::assign_values(string data) {


}

void Flower::get_recipe(std::string input) {
    input = input.substr(1, input.size() - 2);

    std::vector<std::string> output = this->split_record_to_seprate_values(input, ',');

    this->measurementOfFlower.temperature = stoi(output[0]);
    this->measurementOfFlower.humidity = stoi(output[1]);
    this->measurementOfFlower.insolation = stoi(output[2]);




}


