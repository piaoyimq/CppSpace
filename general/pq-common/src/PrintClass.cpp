/*
 * PrintClass.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

#include "pq-common/include/PrintClass.h"

PrintClass::PrintClass()
{
}

PrintClass::~PrintClass()
{
}

std::string PrintClass::toString() const
{
    std::stringstream ss;
    print(ss);

    return ss.str();
}

std::ostream & operator<<(std::ostream& os, const PrintClass& PrintClass)
{
	PrintClass.print(os);
    return os;
}
