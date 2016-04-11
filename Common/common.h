/*
 * common.h
 *
 *  Created on: Apr 6, 2016
 *      Author: ezhweib
 */

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

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
#endif

#endif /* COMMON_COMMON_H_ */
