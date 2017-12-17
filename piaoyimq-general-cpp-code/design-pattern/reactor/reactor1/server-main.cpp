/*
 * server-main.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#indlude ""




void printHelloworld(client_data* data)
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

    TimeServer server(argv[1], atoi(argv[2]));
    if (!server.Start())
    {
        fprintf(stderr, "start server failed\n");
        return EXIT_FAILURE;
    }
    fprintf(stderr, "server started!\n");

    heap_timer* printtask = new heap_timer(5);
    printtask->cb_func = printHelloworld;

    fprintf(stderr, "register a task which will be run is five seconds!\n");
    g_reactor.RegisterTimerTask(printtask);

    while (1)
    {
        g_reactor.RegisterHandler(&server, reactor::kReadEvent);
        g_reactor.HandleEvents();
    }

    return EXIT_SUCCESS;
}
