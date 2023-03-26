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



using namespace std;

class Record {
protected:
    uint16_t ID;
    Database *database;

public:
    Record();

    virtual uint8_t get_record(vector<string> &data) = 0;
    virtual uint8_t put_record() = 0;
    virtual void assign_values(string data) = 0;
    uint16_t get_id();



};

class ESP_unit : public Record{
private:



public:
    time_t timestampOfLastMessage;

    std::shared_ptr<WebsocketService> websocketESP;

    ESP_unit();
    ~ESP_unit();

    uint8_t get_record(vector<string> &data) override;
    uint8_t put_record() override;
    void assign_values(string data) override;

    void assign_pointer_websocket(std::shared_ptr<WebsocketService> ptr);

    static uint8_t check_it_is_exist(vector<ESP_unit> esp_modules, shared_ptr<WebsocketService> &ptr);
};


//class Flower : public Record{
//
//
//
//};


#endif //DAEMONRPI_RECORD_H
