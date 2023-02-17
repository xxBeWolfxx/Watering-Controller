//
// Created by arkadiusz on 17.02.23.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/included/unit_test.hpp>
#include "Database.h"

static uint16_t countWords(std::string str, char separator){
    if (str.size() == 0) {
        return 0;
    }

    uint16_t count = 1;

    for (uint16_t i = 0; i < str.size(); i++) {

        if (str[i] == separator) {
            count++;
        }

    }

    return count;
}


BOOST_AUTO_TEST_CASE(stringCheck){

    BOOST_CHECK(countWords("Andrzej POKA ROR", ' ') == 3);

}