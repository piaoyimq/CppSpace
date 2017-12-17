/*
 * client-main.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */




#include "TestTimeClient.h"




int main(int argc, char ** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s ip port\n", argv[0]);
        return EXIT_FAILURE;
    }

    TestTimeClient client;
    if (!client.ConnectServer(argv[1], atoi(argv[2])))
    {
        fprintf(stderr, "connect remote server failed\n");
        return EXIT_FAILURE;
    }

    g_reactor.RegisterHandler(&client, reactor::kWriteEvent);
    while (1)
    {
        g_reactor.HandleEvents();
        sleep(1);
    }
    g_reactor.RemoveHandler(&client);
    return EXIT_SUCCESS;
}
