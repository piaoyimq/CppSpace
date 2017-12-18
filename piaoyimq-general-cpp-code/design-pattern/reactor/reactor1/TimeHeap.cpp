/*
 * TimeHeap.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: ezhweib
 */


#include "TimeHeap.h"




namespace reactor
{

TimeHeap::TimeHeap(int cap) throw (std::exception) :
        capacity(cap), cur_size(0)
{
    array = new TimeHeap::HeapTimer*[capacity];
    if (!array)
    {
        throw std::exception();
    }
    for (int i = 0; i < capacity; ++i)
    {
        array[i] = NULL;
    }
}

TimeHeap::TimeHeap(TimeHeap::HeapTimer** init_array, int size, int capacity) throw (std::exception) :
        cur_size(size), capacity(capacity)
{
    if (capacity < size)
    {
        throw std::exception();
    }
    array = new TimeHeap::HeapTimer*[capacity];
    if (!array)
    {
        throw std::exception();
    }
    for (int i = 0; i < capacity; ++i)
    {
        array[i] = NULL;
    }
    if (size != 0)
    {
        for (int i = 0; i < size; ++i)
        {
            array[i] = init_array[i];
        }
        for (int i = (cur_size - 1) / 2; i >= 0; --i)
        {
            percolate_down(i);
        }
    }
}

TimeHeap::~TimeHeap()
{
    for (int i = 0; i < cur_size; ++i)
    {
        delete array[i];
    }
    delete[] array;
}

void TimeHeap::add_timer(TimeHeap::HeapTimer* timer) throw (std::exception)
{
    if (!timer)
    {
        return;
    }
    if (cur_size >= capacity)
    {
        resize();
    }
    int hole = cur_size++;
    int parent = 0;
    for (; hole > 0; hole = parent)
    {
        parent = (hole - 1) / 2;
        if (array[parent]->expire <= timer->expire)
        {
            break;
        }
        array[hole] = array[parent];
    }
    array[hole] = timer;
}


void TimeHeap::del_timer(TimeHeap::HeapTimer* timer)
{
    if (!timer)
    {
        return;
    }
    // lazy delelte
    timer->cb_func = NULL;
}


TimeHeap::HeapTimer* TimeHeap::top() const
{
    if (empty())
    {
        return NULL;
    }
    return array[0];
}


void TimeHeap::pop_timer()
{
    if (empty())
    {
        return;
    }

    if (array[0])
    {
        delete array[0];
        array[0] = array[--cur_size];
        percolate_down(0);
    }
}


void TimeHeap::tick()
{
    TimeHeap::HeapTimer* tmp = array[0];
    time_t cur = time(NULL);
    while (!empty())
    {
        if (!tmp)
        {
            break;
        }
        if (tmp->expire > cur)
        {
            break;
        }
        if (array[0]->cb_func)
        {
            array[0]->cb_func(array[0]->user_data);
        }
        pop_timer();
        tmp = array[0];
    }
}


void TimeHeap::percolate_down(int hole)
{
    TimeHeap::HeapTimer* temp = array[hole];
    int child = 0;
    for (; ((hole * 2 + 1) <= (cur_size - 1)); hole = child)
    {
        child = hole * 2 + 1;
        if ((child < (cur_size - 1)) && (array[child + 1]->expire < array[child]->expire))
        {
            ++child;
        }
        if (array[child]->expire < temp->expire)
        {
            array[hole] = array[child];
        }
        else
        {
            break;
        }
    }
    array[hole] = temp;
}


void TimeHeap::resize() throw (std::exception)
{
    TimeHeap::HeapTimer** temp = new TimeHeap::HeapTimer*[2 * capacity];
    for (int i = 0; i < 2 * capacity; ++i)
    {
        temp[i] = NULL;
    }
    if (!temp)
    {
        throw std::exception();
    }
    capacity = 2 * capacity;
    for (int i = 0; i < cur_size; ++i)
    {
        temp[i] = array[i];
    }
    delete[] array;
    array = temp;
}

}
