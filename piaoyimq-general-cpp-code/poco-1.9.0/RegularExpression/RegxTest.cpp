/*
 * RegxTest.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: azhweib
 */

//g++ RegxTest.cpp -I /opt/poco_1_6/include/ -L /opt/poco_1_6/lib/ -lPocoFoundation -Wl,-rpath,/opt/poco_1_6/lib/
#include "Poco/RegularExpression.h"
#include <iostream>



void testMatch()
{
    Poco::RegularExpression sysInfoPattern("info>>>\n(\\w+)");

    std::string fileContent("info>>>\nasdf");
    Poco::RegularExpression::MatchVec posVec;
    if(sysInfoPattern.match(fileContent,0,posVec))
    {
        std::string hostname = fileContent.substr(posVec[1].offset,posVec[1].length);
        std::cout << hostname << std::endl;
    }
}


void testSearch()
{
//    ERROR: starter: pid=25250 SystemCoreProcess handlerForDefaultTerm Received signal 6, aborting immediately
    Poco::RegularExpression originalString(".*ERROR: starter: pid=.* SystemCoreProcess handlerForDefaultTerm Received signal 6, aborting.*\n");
    std::string subject = "ERROR: starter: pid=25250 SystemCoreProcess handlerForDefaultTerm Received signal 6, aborting immediately\n";

    bool ret = originalString.match(subject);
    if(ret)
    {
        std::cout << "Search the string" << std::endl;
    }
    else
    {
        std::cout << "Can't search the string" << std::endl;
    }
}


int main()
{
    testSearch();
}
