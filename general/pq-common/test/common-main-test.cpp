/*
 * CommonMainTest.cpp
 *
 *  Created on: 2016-05-21
 *      Author: piaoyimq
 */

#define BOOST_TEST_MODULE test_pq
#include <boost/test/included/unit_test.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq-common/include/pq-common.h"

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE (test_suite_common_function)
    BOOST_AUTO_TEST_CASE( test_get_name_by_pid, * utf::description("test function"))
    {
        BOOST_TEST_MESSAGE("--------------- test_get_name_by_pid ---------------");
        std::string processName;
        pid_t pid = getpid();
        processName=get_name_by_pid(pid);
        BOOST_TEST_MESSAGE("processName=" << processName);
        BOOST_TEST(processName.find("CommonTest") !=std::string::npos);
    }

    BOOST_AUTO_TEST_CASE( test_vector_print)
    {
        BOOST_TEST_MESSAGE("--------------- test_vector_print ---------------");
        std::vector<int> v={1, 2,3, 8, 9};
        std::stringstream ss;
        ss << v;
//        MAGENTA_PRINT("vector=" <<v <<std::endl);
        BOOST_TEST("[1, 2, 3, 8, 9]"==ss.str());
    }

    BOOST_AUTO_TEST_CASE( test_list_print)
    {
        BOOST_TEST_MESSAGE("--------------- test_list_print ---------------");
        std::list<std::string> l={"abc", "xyz", "hello"};
        std::stringstream ss;
        ss << l;
//        MAGENTA_PRINT("list="<<l<< std::endl);
        BOOST_TEST("[abc, xyz, hello]"==ss.str());
    }

    BOOST_AUTO_TEST_CASE( test_map_print)
    {
        BOOST_TEST_MESSAGE("--------------- test_map_print ---------------");
        std::map<int, std::string> m={{1, "hello"}, {2, "hi"}, {3, "morning"}};
        std::stringstream ss;
        ss << m;
//        MAGENTA_PRINT("map=" << m<< std::endl);
        BOOST_TEST("{1: hello, 2: hi, 3: morning}"==ss.str());


        std::list<std::map<int, std::vector<std::string>>> lmv={{{1, {"v1", "v11"}}, {2, {"v2", "v22", "v222"}}}, {{3, {"v3", "v33"}}, {4, {"v4", "v44", "v444"}}}};
        ss.clear();
        ss.str("");
        ss<<lmv;
//        MAGENTA_PRINT("lmv="<< lmv<< std::endl);
         BOOST_TEST("[{1: [v1, v11], 2: [v2, v22, v222]}, {3: [v3, v33], 4: [v4, v44, v444]}]"==ss.str());

    }
BOOST_AUTO_TEST_SUITE_END () //test_suite_common_function


BOOST_AUTO_TEST_SUITE (test_suite_common_class)
    BOOST_AUTO_TEST_CASE( test_PrintClass)
    {
        BOOST_TEST_MESSAGE("--------------- test_PrintClass ---------------");

        class Student:  public PrintClass
        {
            public:
                virtual void print(std::ostream& os) const
                {
                    os<<"["<<
                            "num="<< num << ", " <<
                            "students=" <<  students
                            << "]";
                }

            private:
                int num=3;
                std::vector<std::string> students={"LiLei", "XiaoLi", "WangEr"};
        };

//         std::cout << Student()<< std::endl;
         BOOST_TEST("[num=3, students=[LiLei, XiaoLi, WangEr]]" == Student().toString());

    }

    BOOST_AUTO_TEST_CASE( test_RangeImpl, * utf::description("test Range"))
    {
        BOOST_TEST_MESSAGE("--------------- test_RangeImpl ---------------");

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
BOOST_AUTO_TEST_SUITE_END () //test_suite_common_class


BOOST_AUTO_TEST_SUITE (test_suite_common_others)
    BOOST_AUTO_TEST_CASE( test_colorful_print, * utf::description("test ...."))
    {
        BOOST_TEST_MESSAGE("--------------- test_colorful_print ---------------");
        std::ostringstream ss;
        std::streambuf* old_buf;

        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        RED_PRINT("RED");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(RED)+std::string("RED")+std::string(RESET) == ss.str());

        ss.clear();
        ss.str("");
        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        GREEN_PRINT("GREEN");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(GREEN)+std::string("GREEN")+std::string(RESET) == ss.str());

        ss.clear();
        ss.str("");
        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        YELLOW_PRINT("YELLOW");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(YELLOW)+std::string("YELLOW")+std::string(RESET) == ss.str());

        ss.clear();
        ss.str("");
        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        BLUE_PRINT("BLUE");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(BLUE)+std::string("BLUE")+std::string(RESET) == ss.str());

        ss.clear();
        ss.str("");
        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        MAGENTA_PRINT("MAGENTA");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(MAGENTA)+std::string("MAGENTA")+std::string(RESET) == ss.str());

        ss.clear();
        ss.str("");
        old_buf = std::cout.rdbuf(ss.rdbuf()); //redirection cout stream to ss
        CYAN_PRINT("CYAN");
        std::cout.rdbuf( old_buf);  // restore cout stream
        BOOST_TEST_MESSAGE(ss.str());
        BOOST_TEST(std::string(CYAN)+std::string("CYAN")+std::string(RESET) == ss.str());
    }
BOOST_AUTO_TEST_SUITE_END () //test_suite_common_others
