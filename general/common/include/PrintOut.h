/*
 * PrintOut.h
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#ifndef GENERAL_COMMON_INCLUDE_PRINTOUT_H_
#define GENERAL_COMMON_INCLUDE_PRINTOUT_H_
#include <ostream>
#include <sstream>

class PrintOut
{
public:
	PrintOut();
    virtual ~PrintOut();

    // Print the contents of the class into the specified ostream
    virtual void print(std::ostream& os) const = 0;

    std::string toString() const;
};

#endif /* GENERAL_COMMON_INCLUDE_PRINTOUT_H_ */
