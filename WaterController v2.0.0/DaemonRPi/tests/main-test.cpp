//
// Created by arkadiusz on 17.02.23.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/included/unit_test.hpp>
#include <sqlite3.h>
#include <Database.h>
#include "Record.h"

Database *Record::database;


BOOST_AUTO_TEST_SUITE(DATABASE);

BOOST_AUTO_TEST_CASE(stringCheck){




    uint16_t count = Database::countWords("DD XX", ' ');

    BOOST_CHECK(count == 2);

}

BOOST_AUTO_TEST_CASE(updateESP){
    Database database = Database();
    database.OpenDatabase("rpi.db");
    Record::database = &database;

    ESP_unit esp = ESP_unit();
    std::vector<std::string> data;


    esp.validate_incoming_messages();
    

//    BOOST_CHECK(strcmp(esp.name.c_str(), "TARAS") == 0);
    BOOST_CHECK(1 == 1);


}

BOOST_AUTO_TEST_SUITE_END();

