/*
 * CommonMainTest.cpp
 *
 *  Created on: 2016-05-21
 *      Author: piaoyimq
 */

#define BOOST_TEST_MODULE test_common
#include <boost/test/included/unit_test.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common/include/pq-common.h"

namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE (test_suite_common)

    BOOST_AUTO_TEST_CASE( test_getNameByPidTest, * utf::description("test ...."))
    {
        std::string processName;
        pid_t pid = getpid();
        processName=getNameByPid(pid);
        BOOST_TEST(processName.find("CommonTest") !=std::string::npos);
    }


    BOOST_AUTO_TEST_CASE( test_Range, * utf::description("test ...."))
    {
        using namespace std;
        using namespace pq;

        stringstream ss;
        BOOST_TEST_MESSAGE("Range(15)");
        for (int i : Range(15))
        {
            ss<<" "<<i;
        }
        BOOST_TEST(" 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14"==ss.str());


        BOOST_TEST_MESSAGE("Range(2,6)");
        ss.clear();
        ss.str("");
        for (int i : Range(2, 6))
        {
            ss<< " " << i;
        }
        BOOST_TEST(" 2 3 4 5"==ss.str());

        BOOST_TEST_MESSAGE("Range(10.5, 15.5)");
        ss.clear();
        ss.str("");
        for (float i : Range(10.5, 15.5))
        {
            ss << " " << i;
        }
        BOOST_TEST(" 10.5 11.5 12.5 13.5 14.5"==ss.str());

        BOOST_TEST_MESSAGE("Range(35,27,-1)");
        ss.clear();
        ss.str("");
        for (int i : Range(35, 27, -1))
        {
            ss << " " << i;
        }
        BOOST_TEST(" 35 34 33 32 31 30 29 28"==ss.str());

        BOOST_TEST_MESSAGE("Range(2,8,0.5)");
        ss.clear();
        ss.str("");
        for (float i : Range(2, 8, 0.5))
        {
            ss << " " << i;
        }
        BOOST_TEST(" 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5"==ss.str());

        BOOST_TEST_MESSAGE("Range(8,7,-0.1)");
        ss.clear();
        ss.str("");
        for (auto i : Range(8, 7, -0.1))
        {
            ss << " " << i;
        }
        BOOST_TEST(" 8 7.9 7.8 7.7 7.6 7.5 7.4 7.3 7.2 7.1"==ss.str());

        BOOST_TEST_MESSAGE("Range('a', 'z')");
        ss.clear();
        ss.str("");
        for (auto i : Range('a', 'z'))
        {
            ss << " " << i;
        }
        BOOST_TEST(" a b c d e f g h i j k l m n o p q r s t u v w x y"==ss.str());
    }

BOOST_AUTO_TEST_SUITE_END () //test_suite_common
