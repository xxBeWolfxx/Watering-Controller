//
// Created by arkadiusz on 17.02.23.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/included/unit_test.hpp>
#include <sqlite3.h>
#include "Database.h"

struct testStr{
    uint32_t val1;
}testStr;

class cl{
public:
    cl(){};
    struct testStr clTest;
};

BOOST_AUTO_TEST_SUITE(DATABASE);

BOOST_AUTO_TEST_CASE(stringCheck){




    uint16_t count = Database::countWords("DD XX", ' ');

    BOOST_CHECK(count == 2);

}

BOOST_AUTO_TEST_CASE(nextTest){

    struct testStr *ptr;
    std::shared_ptr<cl> vv = std::make_shared<cl>();

    ptr = &(vv->clTest);


    BOOST_CHECK(ptr->val1 == vv->clTest.val1);

    vv.reset();

    BOOST_CHECK(ptr->val1 == NULL);
}

BOOST_AUTO_TEST_SUITE_END();

