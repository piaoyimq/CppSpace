/*
 * path-file-test.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: azhweib
 */

// g++ glob-test.cpp -I /opt/gss/poco_1_6/include -L /opt/gss/poco_1_6/lib/ -lPocoFoundation -Wl,-rpath=/opt/gss/poco_1_6/lib/



#include "Poco/Path.h"
#include <iostream>

void testPath()
{
    Poco::Path pa1("/user/include");
    std::cout << pa1.parent().toString() << std::endl;
}


int main()
{
    testPath();
}
