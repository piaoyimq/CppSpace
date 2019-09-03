#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>


sighandler_t registerSignal (sighandler_t action)
{
    std::signal(SIGINT, action);
    std::signal(SIGQUIT, action);
    std::signal(SIGSEGV, action);
    std::signal(SIGTERM, action);
    std::signal(SIGABRT, action);
}


void handleSignal(int signal)
{
    std::cout << "Received xxx signal, process aborting immediately" << std::endl;
    switch(signal)
    {
        case SIGINT:
            std::cout << "Received SIGINT signal, process aborting immediately" << std::endl;
            break;
        case SIGQUIT:
            std::cout << "Received SIGQUIT signal, process aborting immediately" << std::endl;
            break;

        case SIGABRT:
            std::cout << "Received SIGABRT signal, process aborting immediately" << std::endl;
            break;

        case SIGSEGV:
            std::cout << "Received SIGSEGV signal, process aborting immediately" << std::endl;
            break;

        case SIGTERM:
            std::cout << "Received SIGTERM signal, process aborting immediately" << std::endl;
            break;

        default:
            std::cout << "Received unkonw signal, process aborting immediately" << std::endl;
    }
}

void my_func(int sign_no)
{
	if(sign_no==SIGINT)
		printf("I have get SIGINT\n");
	else if(sign_no==SIGQUIT)
		printf("I have get SIGQUIT\n");
}
int main()
{
	printf("Waiting for signal SIGINT or SIGQUIT \n ");
	
	/*����������������*/
//	signal(SIGINT, my_func);
//	signal(SIGQUIT, my_func);
	
	registerSignal(handleSignal);


	pause();
	exit(0);
}

