//
// Created by ahk on 05.03.23.
//

#ifndef DAEMONRPI_RECORD_H
#define DAEMONRPI_RECORD_H

#include <cstdint>
#include <ctime>
#include <vector>
#include <list>
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
    virtual uint8_t create_record_in_database() = 0;
    virtual void assign_values(string data) = 0;
    uint16_t get_id();

    std::vector<std::string> split_record_to_seprate_values(std::string record, char symbole);

    static Database *database;


};

struct Measurement{
    uint8_t rescipeHumidity, rescipeInsolation, avgHumidity, avgInsolation;
    float rescipeTemperature, avgTemperature;

    std::vector<uint8_t> vecOfHumidity;
    std::vector<float> vecOfTemperature;
    std::vector<uint8_t> vecOfInsolation;

    Measurement(){
        rescipeHumidity = rescipeInsolation = avgHumidity = avgInsolation = 0;
        rescipeTemperature = avgTemperature = 0.0;
        vecOfHumidity = std::vector<uint8_t>();
        vecOfInsolation = std::vector<uint8_t>();
        vecOfTemperature = std::vector<uint8_t>();
    };

};


class Flower : public Record{
private:
    uint16_t espID;


    Measurement measurementOfFlower;

public:
    uint16_t id;
    std::string namePlant;

    Flower();
    explicit Flower(uint16_t ID) : espID(ID) {};
    ~Flower();

    uint8_t get_record(vector<string> &data) override;
    uint8_t get_all_record_with_id_esp(vector<string> &data) const;
    uint8_t create_record_in_database() override;
    void assign_values(string data) override;

    void get_recipe(std::string input);

    void calculate_all_averages();

    template<typename T>
    static float calculate_average(const std::vector<T> &vec);



};

class ESP_unit : public Record{
private:
    ESP_STATUS status;
    std::string name;

public:
    time_t timestampOfLastMessage;

    std::shared_ptr<WebsocketService> websocketESP;
    std::list<Flower> vectorOfFlowers;

    ESP_unit();
    ~ESP_unit();

    uint8_t get_record(vector<string> &data) override;
    uint8_t create_record_in_database() override;
    void assign_values(string data) override;

    bool check_message_status();
    void validate_incoming_messages();
    void get_values_from_json(std::vector<std::string> parameters, std::vector<std::string> *containerForValues);
    void get_all_flowers_from_database();
    void assign_values_to_vector_flowers(vector<string> &data);

};




#endif //DAEMONRPI_RECORD_H
