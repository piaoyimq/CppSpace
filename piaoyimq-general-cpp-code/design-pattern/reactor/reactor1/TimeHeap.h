/*
 * TimeHeap.h
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TIMEHEAP_H_
#define PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TIMEHEAP_H_


#include "ctime"
#include <sys/socket.h>
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <time.h>
//#include <assert.h>
//#include <errno.h>
//#include <sys/types.h>
//
//#include <sys/time.h>
//#include <arpa/inet.h>
//#include <string>



namespace reactor
{

using std::exception;

#define BUFFER_SIZE 64

//class TimeHeap::HeapTimer

struct ClientData
{
    sockaddr_in address;

    int sockfd;

    char buf[BUFFER_SIZE];

    TimeHeap::HeapTimer* timer;
};


class TimeHeap
{
public:

    class HeapTimer
    {
    public:
        HeapTimer(int delay)
        {
            expire = time(NULL) + delay;
        }

    public:
        time_t expire;

        void (*cb_func)(ClientData*);

        ClientData* user_data;
    };


    TimeHeap(int cap) throw (std::exception);

    TimeHeap(HeapTimer** init_array, int size, int capacity) throw (std::exception);

    ~TimeHeap();

    void add_timer(HeapTimer* timer) throw (std::exception);

    void del_timer(HeapTimer* timer);

    HeapTimer* top() const;

    void pop_timer();

    void tick();

    bool empty() const
    {
        return cur_size == 0;
    }

private:

    void percolate_down(int hole);

    void resize() throw (std::exception);

    HeapTimer** array;

    int capacity;

    int cur_size;
};

}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_DESIGN_PATTERN_REACTOR_REACTOR1_TIMEHEAP_H_ */
