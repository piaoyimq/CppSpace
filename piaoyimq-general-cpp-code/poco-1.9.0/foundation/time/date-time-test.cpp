/*
 * date-time-test.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: azhweib
 */


//g++ date-time-test.cpp -I /opt/poco_1_6/include/ -L /opt/poco_1_6/lib/ -lPocoFoundation -Wl,-rpath,/opt/poco_1_6/lib/

#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "Poco/Timezone.h"
#include <iostream>
#include <unistd.h>



void test_1()
{
    Poco::LocalDateTime now;

    std::cout << "timezone=" << now.tzd() << std::endl;
//    tzdISO

    std::string timestamp = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT);
    std::cout << "timestamp=" << timestamp << std::endl;

    timestamp = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::SORTABLE_FORMAT);
    std::cout << "timestamp=" << timestamp << std::endl;

    const std::string timeFormat("%Y-%m-%d %H:%M:%S.%i");
    timestamp = Poco::DateTimeFormatter::format(now, timeFormat);
    std::cout << "timestamp=" << timestamp << std::endl;


}


void test_time_interval()
{
    Poco::DateTime now1;
    sleep(5);
    Poco::DateTime now2;

    Poco::Timespan timeInterval = now2 -now1;
    std::cout << "____totalSeconds= " << timeInterval.totalSeconds() << std::endl;
}

#include "Poco/NumberFormatter.h"
std::string myTimezone()
{
    std::string timezone;
    int timeZoneDifferential = Poco::LocalDateTime().tzd();

    if (timeZoneDifferential != Poco::DateTimeFormatter::UTC)
    {
        if (timeZoneDifferential >= 0)
        {
            timezone += "East_";
            Poco::NumberFormatter::append0(timezone, timeZoneDifferential/3600, 2);
            timezone += ".";
            Poco::NumberFormatter::append0(timezone, (timeZoneDifferential%3600)/60, 2);
        }
        else
        {
            timezone += "West_";
            Poco::NumberFormatter::append0(timezone, -timeZoneDifferential/3600, 2);
            timezone += ".";
            Poco::NumberFormatter::append0(timezone, (-timeZoneDifferential%3600)/60, 2);
        }
    }
    else
    {
        timezone = "Middle_time_zone";
    }

    return timezone;
}

void getTimeZone()
{
    std::string name = Poco::Timezone::name();
    std::string stdName = Poco::Timezone::standardName();
    std::string dstName = Poco::Timezone::dstName();

    std::cout << "Timezone Names: " << name << ", " << stdName << ", " << dstName << std::endl;

    int utcOffset = Poco::Timezone::utcOffset();
    std::cout << "UTC Offset: " << utcOffset << std::endl;

    int dst = Poco::Timezone::dst();
    std::cout << "DST Offset: " << dst << std::endl;

    std::cout << "tzd : " << Poco::Timezone::tzd() << std::endl;


    Poco::LocalDateTime now;

    std::cout << "timezone=" << now.tzd() << std::endl;


    std::string zone = Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%Z, %z");

    std::cout << "____timezone=" << zone << std::endl;

    std::cout << "____timezone=" << myTimezone() << std::endl;
}

int main()
{
//    test_1();
//    test_time_interval();
    getTimeZone();
}
