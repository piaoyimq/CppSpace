/*
 * server-main.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#include "TestTimeServer.h"
#include "TimeHeap.h"




void printHelloworld(reactor::ClientData* data)
{
    fprintf(stderr, "timertask : Hello world from timerTask!\n");
}

int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s ip port\n", argv[0]);
        return EXIT_FAILURE;
    }

    reactor::TestTimeServer server(argv[1], atoi(argv[2]));
    if (!server.Start())
    {
        fprintf(stderr, "start server failed\n");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "server started!\n");

    reactor::TimeHeap::HeapTimer* printTask = new reactor::TimeHeap::HeapTimer(5);
    printTask->cb_func = printHelloworld;

    fprintf(stderr, "register a task which will be run is five seconds!\n");
    g_reactor.RegisterTimerTask(printTask);

    while (1)
    {
        g_reactor.RegisterHandler(&server, reactor::kReadEvent);
        g_reactor.HandleEvents();
    }

    return EXIT_SUCCESS;
}
