/*
 * GenerateCoredump.cpp
 *
 *  Created on: Aug 30, 2019
 *      Author: azhweib
 */


#include <iostream>


void signal_handler(int signal)
{
    if (signal == SIGABRT) {
        std::cerr << "SIGABRT received\n";
    } else {
        std::cerr << "Unexpected signal " << signal << " received\n";
    }
    std::_Exit(EXIT_FAILURE);
}

int main()
{
    Tester automatic_tester; // Destructor not called

    // Setup handler

}


void testAbort()
{
    auto previous_handler = std::signal(SIGABRT, signal_handler);
    if (previous_handler == SIG_ERR) {
        std::cerr << "Setup failed\n";
        return EXIT_FAILURE;
    }

    std::abort();  // Raise SIGABRT
    std::cout << "This code is unreachable\n";
}


int main()
{
    testAbort();
}
