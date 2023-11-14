//
// Created by arkadiusz on 16.09.22.
//

#ifndef CRAWLERROBOT_DATABASE_H
#define CRAWLERROBOT_DATABASE_H
#include <sqlite3.h>
#include <string>
#include <vector>

class Database {

private:

    uint8_t OpenDatabase(std::string database);
    void CloseDatabase(std::string database);

    sqlite3 *db;
    std::string dbName = "rpi.db";

public:


    Database();
    Database(std::string database);
    ~Database();

    uint8_t InsertData(std::string *table, std::string *columns, std::string *value);
    void ExecCommand(std::string *command);

    uint8_t SelectData(std::string *command, std::vector<std::string> &data);

    static uint16_t countWords(std::string str, char separator);

};


#endif //CRAWLERROBOT_DATABASE_H
