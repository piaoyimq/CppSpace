/*
 * PrintOut.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#include "PrintOut.h"

PrintOut::PrintOut()
{
}

PrintOut::~PrintOut()
{
}

std::string PrintOut::toString() const
{
    std::stringstream ss;
    print(ss);

    return ss.str();
}

std::ostream & operator<<(std::ostream& os, const PrintOut& printOut)
{
	printOut.print(os);
    return os;
}
