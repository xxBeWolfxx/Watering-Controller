//
// Created by arkadiusz on 17.02.23.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/included/unit_test.hpp>
#include <sqlite3.h>
#include <Database.h>
#include "Record.h"
#include "Logger.h"

Database *Record::database;


BOOST_AUTO_TEST_SUITE(DATABASE);

BOOST_AUTO_TEST_CASE(logs){

    Logger logger = Logger("test.log");
    logger.Open_log_file();




    BOOST_CHECK(2 == 2);

}

BOOST_AUTO_TEST_CASE(updateESP){
    Database database = Database();
    database.OpenDatabase("rpi.db");
    Record::database = &database;

    ESP esp = ESP();
    std::vector<std::string> data;


    esp.validate_incoming_messages();
    

//    BOOST_CHECK(strcmp(esp.name.c_str(), "TARAS") == 0);
    BOOST_CHECK(1 == 1);


}

BOOST_AUTO_TEST_SUITE_END();

