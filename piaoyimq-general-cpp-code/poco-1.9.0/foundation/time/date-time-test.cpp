/*
 * date-time-test.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: azhweib
 */



#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"


void test_1()
{
    Poco::LocalDateTime now;
    std::string timestamp = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT);
}

