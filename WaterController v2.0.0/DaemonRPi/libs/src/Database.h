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
    std::string filename;
    sqlite3 *db;
    std::string dbName = "flower";



public:
    std::vector<Flower> vData;


    Database(std::string filename);
    ~Database();

    uint8_t openDatabase();
    uint8_t insertData(std::string name, int16_t coordinates[], int timestamp);
    uint8_t selectData();

    static int callback(void *NotUsed, int argc, char **argv, char **azColName);

};


#endif //CRAWLERROBOT_DATABASE_H
