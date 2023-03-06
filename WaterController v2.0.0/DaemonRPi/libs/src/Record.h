//
// Created by ahk on 05.03.23.
//

#ifndef DAEMONRPI_RECORD_H
#define DAEMONRPI_RECORD_H

#include <cstdint>
#include <vector>
#include "Database.h"

using namespace std;

class Record {
protected:
    uint16_t ID;
    Database *database;

public:
    Record(Database *database, uint16_t id);

    virtual uint8_t get_data(vector<string> &data) = 0;
    virtual uint8_t put_data(vector<string> &data) = 0;
    uint16_t get_id();



};

class ESP_unit : public Record{

public:
    ESP_unit(Database *database, uint16_t id);

    uint8_t get_data(vector<string> &data) override;
    uint8_t put_data(vector<string> &data) override;
};


//class Flower : public Record{
//
//
//
//};


#endif //DAEMONRPI_RECORD_H
