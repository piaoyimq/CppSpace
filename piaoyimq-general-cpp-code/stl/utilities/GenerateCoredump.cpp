/*
 * GenerateCoredump.cpp
 *
 *  Created on: Aug 30, 2019
 *      Author: azhweib
 */


#include <csignal>
#include <iostream>
#include <cstdlib>

void signal_handler(int signal)
{
    if (signal == SIGABRT)
    {
        std::cerr << "SIGABRT received\n";
    }
    else
    {
        std::cerr << "Unexpected signal " << signal << " received\n";
    }
//    std::_Exit (EXIT_FAILURE);// uncoment it, this process will core dump
}


void testAbort()
{
    sighandler_t previous_handler = std::signal(SIGABRT, signal_handler);
    if (previous_handler == SIG_ERR)
    {
        std::cerr << "Setup failed\n";
        return ;
    }

    std::abort();  // Raise SIGABRT
    std::cout << "This code is unreachable\n";
}

int main()
{
    testAbort();
}
