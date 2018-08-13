/*
 * test-1.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: azhweib
 */


#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>




namespace bpt = boost::posix_time;


int main()
{
    bpt::ptime now(bpt::microsec_clock::local_time());
    const std::string timestamp = bpt::to_iso_extended_string(now);

    std::cout << timestamp << std::endl;
}
