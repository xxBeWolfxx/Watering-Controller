//
// Created by arkadiusz on 17.02.23.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/included/unit_test.hpp>
#include <sqlite3.h>
#include "Database.h"

BOOST_AUTO_TEST_SUITE(DATABASE);

BOOST_AUTO_TEST_CASE(stringCheck){


    Flower x1 = {};
    x1.timestamp = 1;

    uint16_t count = Database::countWords("DD XX", ' ');

    BOOST_CHECK(count == 2);

}

BOOST_AUTO_TEST_CASE(nextTest){
    BOOST_CHECK(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END();

