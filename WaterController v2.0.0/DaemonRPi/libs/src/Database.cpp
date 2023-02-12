//
// Created by arkadiusz on 16.09.22.
//

#include "Database.h"
#include <iostream>

static int callback1(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}




Database::Database() {

}

int Database::callback(void *NotUsed, int argc, char **argv, char **azColName) {
    Flower temp;
    temp.name = std::string(argv[0]);
    temp.coordinate = std::string(argv[1]);
    sscanf(argv[2], "%d", &temp.timestamp);
//    vData.push_back(temp);
    return 0;

}

uint8_t Database::OpenDatabase(std::string database) {
    uint8_t rc = 0;
    rc = sqlite3_open(database.c_str(), &this->db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(this->db));
        return 0;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }


    return 1;

}

void Database::CloseDatabase(std::string database) {

    sqlite3_close(this->db);
}


uint8_t Database::InsertData(std::string *command){ // int16_t coordinates[], int timestamp
//    std::string *record = new std::string();
    char *zErrMsg = 0;

//    std::string values = std::to_string(coordinates[0]) + ";"
//                         + std::to_string(coordinates[1]) + ";"
//                         + std::to_string(coordinates[2]);
//    *record = "INSERT INTO " + this->dbName
//              + "(NAME,COORDINATES,TIMESTAMP) VALUES ('" + name + "', '" + values + "', "
//              + std::to_string(timestamp) + ");";

    int16_t rc = sqlite3_exec(this->db, command->c_str(), NULL, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }

//    delete record;

}

uint8_t Database::Select_all_data(std::string tableName, std::vector<std::string> &data) {
    std::string *record = new std::string();
    sqlite3_stmt *stmt;
    *record = "SELECT * from " + tableName;

    std::string temp = "";

    /* Execute SQL statement */
    sqlite3_prepare(this->db, record->c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    uint8_t columnNumber = sqlite3_column_count(stmt);

    while (sqlite3_column_text(stmt, 0)){


        for (uint8_t i = 0; i < columnNumber; i++){
            temp = temp + "{" + std::string((char *) sqlite3_column_text(stmt, i)) + "}";
        }
        data.push_back(temp);
        temp = "";

//        temp.name = std::string((char *) sqlite3_column_text(stmt, 0));
//        temp.coordinate = std::string((char *) sqlite3_column_text(stmt, 1));
//        sscanf((char *) sqlite3_column_text(stmt, 2), "%d", &temp.timestamp);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    delete record;

}




Database::~Database() {
    sqlite3_close(db);
}

/*
 * ID esp32 {
 *
 *
 */


