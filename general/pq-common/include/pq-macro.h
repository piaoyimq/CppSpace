/*
 * pq-macro.h
 *
 *  Created on: Aug 25, 2017
 *      Author: piaoyimq
 */

#ifndef GENERAL_COMMON_INCLUDE_PQ_MACRO_H_
#define GENERAL_COMMON_INCLUDE_PQ_MACRO_H_
#include <iostream>

#define ABORT() \
    std::cerr << "################################################" << std::endl \
              << "ABORT in function '" << __PRETTY_FUNCTION__ << "' " \
              << "File=" << __FILE__ << " line=" << __LINE__ << std::endl \
              << "################################################" << std::endl; \
    ::abort();



#define ASSERT(CONDITION) \
    if (!(CONDITION)) \
    { \
        (Assert::fail(__FILE__, __LINE__, __PRETTY_FUNCTION__, #CONDITION)); \
    }



//colorful print
#define RED                  "\x1b[31m"
#define GREEN             "\x1b[32m"
#define YELLOW           "\x1b[33m"
#define BLUE                 "\x1b[34m"
#define MAGENTA        "\x1b[35m"
#define CYAN                "\x1b[36m"
#define RESET              "\x1b[0m"

#define RED_PRINT(msg)              std::cout<<RED << msg << RESET;

#define GREEN_PRINT(msg)          std::cout<<GREEN<< msg << RESET;

#define YELLOW_PRINT(msg)        std::cout<<YELLOW<< msg << RESET;

#define BLUE_PRINT(msg)             std::cout<<BLUE<< msg << RESET;

#define MAGENTA_PRINT(msg)     std::cout<<MAGENTA<< msg << RESET;

#define CYAN_PRINT(msg)             std::cout<<CYAN<< msg << RESET;





#endif /* GENERAL_COMMON_INCLUDE_PQ_MACRO_H_ */
