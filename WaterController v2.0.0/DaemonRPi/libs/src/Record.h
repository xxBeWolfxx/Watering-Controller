//
// Created by ahk on 05.03.23.
//

#ifndef DAEMONRPI_RECORD_H
#define DAEMONRPI_RECORD_H

#include <cstdint>
#include <ctime>
#include <vector>
#include "Database.h"
#include "WebsocketService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



using namespace std;

enum ESP_STATUS{
    INIT = 0,
    CHECKING_DATABASE,
    IN_DATABASE,
    NEW_ELEMENT,
    WORKING
};



class Record {
protected:
    uint16_t ID;


public:
    Record();

    virtual uint8_t get_record(vector<string> &data) = 0;
    virtual uint8_t put_record() = 0;
    virtual void assign_values(string data) = 0;
    uint16_t get_id();

    static Database *database;


};

struct Measurement{
    uint8_t humidity, insolation;
    float temperature;

    std::vector<uint8_t> listOfHumidity;
    std::vector<uint8_t> listOfTemperature;
    std::vector<uint8_t> listOfInsolation;

    Measurement(){
        listOfHumidity = std::vector<uint8_t>(10);
        listOfInsolation = std::vector<uint8_t>(10);
        listOfTemperature = std::vector<uint8_t>(10);
    };

};


class Flower : public Record{
private:
    uint16_t espID;
    std::string namePlant;

    Measurement measurementOfFlower;

public:
    Flower();
    ~Flower();

    uint8_t get_record(vector<string> &data) override {};
    uint8_t put_record() override {};
    void assign_values(string data) override {};

};

class ESP_unit : public Record{
private:
    ESP_STATUS status;

public:
    time_t timestampOfLastMessage;

    std::shared_ptr<WebsocketService> websocketESP;
    std::vector<Flower> vectorOfFlowers;

    ESP_unit();
    ~ESP_unit();

    uint8_t get_record(vector<string> &data) override;
    uint8_t put_record() override;
    void assign_values(string data) override;

    bool check_message_status();
    void validate_incoming_messages();
    void get_values_from_json(std::vector<std::string> parameters, std::vector<std::string> *containerForValues);

};




#endif //DAEMONRPI_RECORD_H
