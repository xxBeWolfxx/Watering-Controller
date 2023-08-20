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


    this->database->InsertData(table, columns, values);

    delete values;
    delete columns;
    delete table;

}

void ESP_unit::update_values() {
    string *command = new string;
    string *condition = new string;


    *condition = " WHERE ID=" + std::to_string(this->ID);

    *command = "UPDATE ESP SET IP_ADDRESS='" + this->websocketESP->getIPaddress()
            + "',NAME='" + this->name + "',TIMESTAMP_LAST_MSG=" + std::to_string(this->timestampOfLastMessage) + *condition;

    this->database->ExecCommand(command);

    delete command;
    delete condition;



}

bool ESP_unit::check_message_appearance() {
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
            else{

            }

            this->status = ESP_STATUS::CHECKING_DATABASE;
            break;
        }
        case CHECKING_DATABASE: {
            std::vector<std::string> data;
            this->get_record(data);

            if(data.empty()){
                this->status = ESP_STATUS::MISSING_ELEMENT;
                this->websocketESP->sy_write("{\"STATUS\": 404}");
            }
            else{
                this->status = ESP_STATUS::IN_DATABASE;
                this->websocketESP->sy_write("{\"STATUS\": 201}");
                this->websocketESP->new_message_appeared = false;
            }



            break;
        }

        case IN_DATABASE: {
            this->get_all_flowers_from_database();
            this->status = ESP_STATUS::WORKING;
            break;
        }
        case MISSING_ELEMENT: {

            this->create_log("MISSING_ELEMENT");
            this->websocketESP->setState(false);
            this->websocketESP->new_message_appeared = false;

            this->status = ESP_STATUS::CLOSE;

            break;
        }
        case WORKING: {
            this->websocketESP->new_message_appeared = false;


            int8_t retVal = this->receive_new_measurment();
            if (retVal > -1){

            }

            // UAKTUALNIENIE WARTOŚCI ŚREDNIEJ
            // WPISANIE DO BAZY DANYCH


            sleep(2);
            break;
        }
        case ERROR:{

            this->create_log("ERROR");
            this->websocketESP->setState(false);
            this->websocketESP->new_message_appeared = false;

            this->status = ESP_STATUS::CLOSE;

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
        auto it = pt.find(param);
        if( it != pt.not_found() ) {
            std::string value = pt.get<string>(param);
            containerForValues->push_back(std::move(value));
        }
    }

}

void ESP_unit::get_all_flowers_from_database() {
    Flower tempFlower = Flower(this->ID);
    std::vector<std::string> data;

    tempFlower.get_all_record_with_id_esp(data);
    this->assign_values_to_vector_flowers(data);

    for (Flower &item : this->vectorOfFlowers){
        item.get_measurement_from_database();

    }






}

void ESP_unit::assign_values_to_vector_flowers(vector<string> &data) {

    for ( std::string &item : data){
        Flower tempFlower = Flower(this->ID);
        auto values = tempFlower.split_record_to_seprate_values(item, '/');
        tempFlower.set_ID( (uint16_t) stoi(values[0]) );
        tempFlower.namePlant = values[2];
        tempFlower.get_recipe(values[3]);


        this->vectorOfFlowers.push_back(std::move(tempFlower));


    }

    // "{1}{1}{Rose}{[22,45,45]}"

}

int8_t ESP_unit::receive_new_measurment() {

    std::vector<std::string> parameters = { "CODE" };
    std::vector<std::string> values;
    Measurement *handleMeasurment;
    Flower *handleFlower;

    this->get_values_from_json(parameters, &values);
    if (values.empty()) return -1;

    uint16_t typeCase = (uint16_t) std::stoi(values[0]);
    values.clear();
    parameters.clear();

    parameters = { "HUM", "TEMP", "ISO" };
    this->get_values_from_json(parameters, &values);
    if(values.size() < 3) return -1;


    switch(typeCase){
        case 101:{
            handleFlower = &this->vectorOfFlowers[0];
            break;
        }
        case 102:{
            handleFlower = &this->vectorOfFlowers[1].;
            break;
        }
        default:{
            break;
        }

    }

    handleMeasurment = &handleFlower->measurementOfFlower;
    handleMeasurment->vecOfHumidity.push_back(std::stoi(values[0]));
    handleMeasurment->vecOfTemperature.push_back(std::stof(values[1]));
    handleMeasurment->vecOfInsolation.push_back(std::stoi(values[2]));

    handleFlower->set_flag_data(true);

    return 0;



}

void ESP_unit::create_log(std::string type) {
    string *values = new string;
    string *columns = new string;
    string *table = new string;

    *values = "'" + this->name + "','" + this->websocketESP->getIPaddress()
            + "'," + to_string(this->timestampOfLastMessage)
            + ",'" + type + "'" ; //add timestamp do websocketa
    *columns = "ESP_NAME, ESP_IP, TIMESTAMP, TYPE";
    *table = "LOGESP";

    this->database->InsertData(table, columns, values);

    delete values;
    delete columns;
    delete table;


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

uint8_t Flower::get_all_record_with_id_esp(vector<string> &data) const {
    string *comand = new string;
    *comand = "SELECT * FROM PLANT WHERE ID_ESP=" + to_string(this->espID);
    this->database->SelectData(comand, data);
    delete comand;

    if(data.empty()){
        return 0;
    }
    return 0;
}

void Flower::update_values() {

    string *command = new string;
    string *condition = new string;
    string *recipe = new string;

    *recipe = "'[" + std::to_string(this->measurementOfFlower.rescipeTemperature) + ","
                + std::to_string(this->measurementOfFlower.rescipeHumidity) + ","
                + std::to_string(this->measurementOfFlower.rescipeInsolation) + "]'";

    *condition = "WHERE ID=" + std::to_string(this->ID) + "";

    *command = "UPDATE PLANT SET ID_ESP=" + std::to_string(this->espID)
               + ",NAME='" + this->namePlant + "',RECIPE=" + *recipe + *condition;

    this->database->ExecCommand(command);

    delete command;
    delete condition;
    delete recipe;



}

void Flower::get_recipe(std::string input) {
    input = input.substr(1, input.size() - 2);

    std::vector<std::string> output = this->split_record_to_seprate_values(input, ',');

    this->measurementOfFlower.rescipeTemperature = stof(output[0]);
    this->measurementOfFlower.rescipeHumidity = stoi(output[1]);
    this->measurementOfFlower.rescipeInsolation = stoi(output[2]);




}

uint8_t Flower::create_record_in_database() {
    string *values = new string;
    string *columns = new string;
    string *table = new string;
    string *recipe = new string;

    *recipe = "'[" + to_string(this->measurementOfFlower.rescipeTemperature) + ","
              + to_string(this->measurementOfFlower.rescipeHumidity) + ","
              + to_string(this->measurementOfFlower.rescipeInsolation) + "]'";

    *values = to_string(this->ID) + ",'" + this->namePlant + "'," + to_string(this->espID) + "," + *recipe;
    *columns = "ID, NAME, ID_ESP, RECIPE";
    *table = "PLANT";


    database->InsertData(table, columns, values);

    delete values;
    delete columns;
    delete table;
    delete recipe;


    return 0;
}

template<typename T>
float Flower::calculate_average(const vector<T> &vec) {
    uint8_t size = vec.size();
    float addition = 0;

    for (auto &item : vec){
        addition = addition + item;
    }

    if ( addition != 0 ){
        return addition / size;
    }
    return 0;
}

void Flower::calculate_all_averages() {

    this->measurementOfFlower.avgHumidity = (uint8_t) this->calculate_average(this->measurementOfFlower.vecOfHumidity);
    this->measurementOfFlower.avgInsolation = (uint8_t) this->calculate_average(this->measurementOfFlower.vecOfInsolation);
    this->measurementOfFlower.avgTemperature = this->calculate_average(this->measurementOfFlower.vecOfTemperature);

}

void Flower::create_measurement_in_database() {
    this->calculate_all_averages();
    std::string temperature = this->make_vector_of_measurement(this->measurementOfFlower.vecOfTemperature);
    std::string humidity = this->make_vector_of_measurement(this->measurementOfFlower.vecOfHumidity);
    std::string insolation = this->make_vector_of_measurement(this->measurementOfFlower.vecOfInsolation);


    string *values = new string;
    string *columns = new string;
    string *table = new string;

    *values = to_string(this->ID) + "," + std::to_string(this->measurementOfFlower.avgHumidity) + ","
            + std::to_string(this->measurementOfFlower.avgTemperature) + "," + std::to_string(this->measurementOfFlower.avgInsolation)
            + ",'" + humidity + "','" + temperature + "','" + insolation + "'";
    *columns = "ID_PLANT, HUM, TEM, INSO, LIST_HUM, LIST_TEMP, LIST_INSO";
    *table = "MEASUREMENT";


    database->InsertData(table, columns, values);

    delete values;
    delete columns;
    delete table;


}

void Flower::update_measurement_in_database() {

    string *command = new string;
    string *condition = new string;

    std::string temperature = this->make_vector_of_measurement(this->measurementOfFlower.vecOfTemperature);
    std::string humidity = this->make_vector_of_measurement(this->measurementOfFlower.vecOfHumidity);
    std::string insolation = this->make_vector_of_measurement(this->measurementOfFlower.vecOfInsolation);


    *condition = "WHERE ID_PLANT=" + std::to_string(this->ID) + "";

    *command = "UPDATE MEASUREMENT SET HUM=" + std::to_string(this->measurementOfFlower.avgHumidity)
               + ",TEM=" + std::to_string(this->measurementOfFlower.avgTemperature)
               + ",INSO=" + std::to_string(this->measurementOfFlower.avgInsolation)
               + ",LIST_HUM='" + humidity + "',LIST_TEMP='" + temperature + "',LIST_INSO='" + insolation
               + "'" + *condition;

    this->database->ExecCommand(command);

    this->set_flag_data(false);

    delete command;
    delete condition;



}

void Flower::set_ID(uint16_t id) {
    this->ID = id;

}

void Flower::get_measurement_from_database() {
    std::vector<std::string> data;
    string *comand = new string;

    *comand = "SELECT * FROM MEASUREMENT WHERE ID_PLANT=" + to_string(this->ID);
    this->database->SelectData(comand, data);
    delete comand;

    if (data.empty()){
        return;
    }

    std::vector<std::string> output = this->split_record_to_seprate_values(data[0], '/');
    this->measurementOfFlower.avgHumidity = std::stoi(output[1]);
    this->measurementOfFlower.avgTemperature = std::stoi(output[2]);
    this->measurementOfFlower.avgInsolation = std::stoi(output[3]);

    this->measurementOfFlower.vecOfHumidity = this->read_vector_from_database<uint8_t>(output[4], true);
    this->measurementOfFlower.vecOfTemperature = this->read_vector_from_database<float>(output[5], false);
    this->measurementOfFlower.vecOfInsolation = this->read_vector_from_database<uint8_t>(output[6], true);




//TODO missing calculating avg number 


}

bool Flower::get_status_new_data() {
    return this->newData;
}

void Flower::set_flag_data(bool flag){
    this->newData = flag;
}

void Flower::check_quantity_of_measurments(){
    if ( this->measurementOfFlower.vecOfInsolation.size() > (this->limitOfMeasuments - 1) ){

    }

    if ( this->measurementOfFlower.vecOfHumidity.size() > (this->limitOfMeasuments - 1) ){
        uint8_t lastElement = this->measurementOfFlower.vecOfInsolation[this->limitOfMeasuments];
        this->measurementOfFlower.vecOfInsolation.clear();
        this->measurementOfFlower.vecOfInsolation.push_back(lastElement);
    }
}

template<typename T>
std::vector<T> Flower::read_vector_from_database(std::string &str, bool ifInt) {
    str = str.substr(1, str.size() - 2);
    std::vector<std::string> vec = this->split_record_to_seprate_values(str, ',');

    std::vector<T> measurmentVec;

    for ( auto item : vec) {
        if (ifInt){
            measurmentVec.push_back(std::stoi(item));
        }
        else{
            measurmentVec.push_back(std::stof(item));
        }
    }

    return measurmentVec;
}


template<typename T>
std::string Flower::make_vector_of_measurement(const vector<T> &vec) {
    std::string output = "[";

    for (auto &item : vec){
        output = output + std::to_string(item) + ",";
    }
    output.erase(output.size() - 1);
    output = output + "]";


    return output;
}


std::vector<float> Measurement::clearMeasurmentVector(std::vector<float> &vec, uint8_t limit) {
    float lastElement = vec[limit];
    vec.clear();
    vec.push_back(lastElement);

    return vec;

}

std::vector<uint8_t> Measurement::clearMeasurmentVector(std::vector<uint8_t> &vec, uint8_t limit) {
    uint8_t lastElement = vec[limit];
    vec.clear();
    vec.push_back(lastElement);

    return vec;
}
