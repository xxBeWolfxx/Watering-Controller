//
// Created by arkadiusz on 16.09.22.
//

#ifndef CRAWLERROBOT_DATABASE_H
#define CRAWLERROBOT_DATABASE_H
#include <sqlite3.h>
#include <string>
#include <vector>

typedef struct Flower{
    std::string name;
    std::string coordinate;
    int timestamp;
}Flower;


class Database {

private:
    const std::string flower_table = "flowerTable";
    const std::string measurements_table = "measurementsTable";


    sqlite3 *db;
    std::string dbName = "rpi.db";



public:
    std::vector<Flower> vData;


    Database();
    ~Database();

    uint8_t openDatabase(std::string database);
    void closeDatabase(std::string database);
    uint8_t insertData(std::string *command);
    uint8_t getFlower(Flower object);


    uint8_t selectData();



    static int callback(void *NotUsed, int argc, char **argv, char **azColName);

};


#endif //CRAWLERROBOT_DATABASE_H
