/*
 * thread-test.cpp
 *
 *  Created on: Jul 18, 2019
 *      Author: azhweib
 */



#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>



inline pid_t get_tid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void handle(int sig)
{
    std::cout << "____subthread tid=" << get_tid()<< ", receive signal =" << sig << std::endl;
    pthread_exit(0);
}


void *thread(void *str)
{
    signal(SIGQUIT, handle);

    int i;
    for (i = 0; i < 10; ++i)
    {
        std::cout << "____subthread tid=" << get_tid() << std::endl;
        sleep(20);
    }
    return NULL;
}

int main()
{
    pthread_t pth;
    int i;
    int ret = pthread_create(&pth, NULL, thread, (void *)(i));

#ifdef DETACH
    int retTh = pthread_detach(pth);
    if(0 != retTh)
    {
        std::cout << "____detach failed, retTh=" << retTh << std::endl;
    }
#endif
    sleep(5);
    std::cout << "____main thread tid=" << get_tid() << std::endl;

    int n = pthread_kill(pth, SIGQUIT);
    if(n != 0)
    {
        std::cout << "____kill failed, n=" << n << std::endl;
    }

#ifndef DETACH
    pthread_join(pth, NULL);
#endif

    sleep(50);
    std::cout << "____main thread tid=" << get_tid() << ", main thread will exit" << std::endl;

    return 0;
}


