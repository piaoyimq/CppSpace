/*
 * ofstream-test1.cpp
 *
 *  Created on: Nov 9, 2018
 *      Author: azhweib
 */

// ofstream::open / ofstream::close
#include <fstream>      // std::ofstream
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sstream>

void testOfstream()
{

    std::ofstream ofs("test.txt", std::ofstream::trunc);
    if (!ofs.is_open())
    {
        std::cout << "Open test.txt failed:" << std::strerror(errno) << std::endl;
    }

    ofs << " more lorem ipsum";
    std::cout << "____tellp=" << ofs.tellp() << std::endl;

}

void testOfstream2()
{
    std::stringstream ss("test...");
    std::ofstream ofs("test.txt", std::ofstream::app);
    if (!ofs.is_open())
    {
        std::cout << "Open test.txt failed:" << std::strerror(errno) << std::endl;
    }

//    ofs << ss;
    ofs << ss.str();
    std::cout << "____tellp=" << ofs.tellp() << std::endl;

}

int main()
{
//    testOfstream();
    testOfstream2();
}
