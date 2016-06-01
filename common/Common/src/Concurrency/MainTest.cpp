/*
 * MainTest.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: ezhweib
 */

#include <iostream>
#include "Mutex.h"

int main(int argc, char* argv[]) {
    pthread_t id;
    int ret;

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_create(&id, NULL, f, NULL);
        if (0 != ret) {
            printf("pthread_create error\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        ret = pthread_join(id, NULL);
        if (ret < 0) {
            printf("pthread_join error\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
