//
// Created by ahk on 05.03.23.
//

#include "Record.h"


uint16_t Record::get_id() {
    return this->ID;
}

Record::Record(Database *database, uint16_t id) {
    this->database = database;
    this->ID = id;

}


ESP_unit::ESP_unit(Database *database, uint16_t id) : Record(database, id) {

}

uint8_t ESP_unit::get_data(vector<string> &data) {
    string *comand = new string;
    *comand = "SELECT * FROM ESP WHERE id=" + to_string(this->ID);
    this->database->SelectData(comand, data);




    delete comand;

    if(data.empty()){
        return 0;
    }

    return 1;

}

uint8_t ESP_unit::put_data(vector<string> &data) {


}

