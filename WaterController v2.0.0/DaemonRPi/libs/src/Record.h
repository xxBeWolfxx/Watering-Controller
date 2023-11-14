//
// Created by ahk on 05.03.23.
//

#ifndef DAEMONRPI_RECORD_H
#define DAEMONRPI_RECORD_H

#include <cstdint>
#include <ctime>
#include <vector>
#include <iomanip>
#include "Database.h"
#include "WebsocketService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



using namespace std;

enum ESP_STATUS{
    INIT = 0,
    CHECKING_DATABASE,
    IN_DATABASE,
    MISSING_ELEMENT,
    WORKING,
    ERROR,
    CLOSE
};



class Record {
protected:
    uint16_t ID;


public:
    Record();

    virtual uint8_t get_record(vector<string> &data) = 0;
    virtual uint8_t create_record_in_database() = 0;
    virtual void update_values() = 0;
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

    std::vector<uint8_t > clearMeasurmentVector(std::vector<uint8_t> &vec, uint8_t limit);
    std::vector<float> clearMeasurmentVector(std::vector<float> &vec, uint8_t limit);

    Measurement(){
        rescipeHumidity = rescipeInsolation = avgHumidity = avgInsolation = 0;
        rescipeTemperature = avgTemperature = 0.0;
        vecOfHumidity = std::vector<uint8_t>();
        vecOfInsolation = std::vector<uint8_t>();
        vecOfTemperature = std::vector<float>();
    };

};


class Flower : public Record{
private:
    uint16_t espID;
    uint16_t limitOfMeasuments = 41;

    bool newData = false;

    template<typename T>
    float calculate_average(const std::vector<T> &vec);
    template<typename T>
    std::string make_vector_of_measurement(const std::vector<T> &vec);
    template<typename T>
    std::vector<T> read_vector_from_database(std::string &str, bool ifInt);

public:
    std::string namePlant;
    Measurement measurementOfFlower;

    Flower();
    explicit Flower(uint16_t ID) : espID(ID) {};
    ~Flower();

    uint8_t get_record(vector<string> &data) override;
    uint8_t get_all_record_with_id_esp(vector<string> &data) const;
    uint8_t create_record_in_database() override;
    void update_values() override;

    void create_measurement_in_database();
    void update_measurement_in_database();
    void get_measurement_from_database();

    void get_recipe(std::string input);
    bool get_status_new_data();
    void set_flag_data(bool flag);
    void set_ID(uint16_t id);

    void check_quantity_of_measurments();
    void calculate_all_averages();




};


class ESP_Entity : public Record{
private:
    std::string ipAddress;
    std::string name;
    time_t timestampOfLastMessage;

public:

    std::vector<Flower> vectorOfFlowers;

    ESP_Entity(std::string ipAddress);
    ESP_Entity();
    ~ESP_Entity();


    uint8_t get_record(vector<string> &data) override;
    uint8_t create_record_in_database() override;
    void update_values() override;

    void assign_id_name(std::string name, uint16_t id);
    void get_all_flowers_from_database();
    void assign_values_to_vector_flowers(vector<string> &data);
    time_t get_last_timestamp();

};


class ESP {
private:
    ESP_STATUS status;
    uint16_t ID;


public:
    std::string name;
    std::shared_ptr<WebsocketService> websocketESP;
    ESP_Entity espDatabase;

    ESP();
    ~ESP();


    void get_ESP_database();
    bool check_message_appearance();
    void validate_incoming_messages();
    void get_values_from_json(std::vector<std::string> parameters, std::vector<std::string> *containerForValues);
    
    
    void create_log(std::string type);

    int8_t receive_new_measurment();
    void manage_the_flower_measurments();

};




#endif //DAEMONRPI_RECORD_H
