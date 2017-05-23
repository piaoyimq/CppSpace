// Example program
#include <iostream>
#include <list>
#include <stdexcept>


namespace StdListOrDeque
{
template<class T>
class MyQueue
{
public:

    MyQueue():qSize(0)
    {
    }


    void push(const T& t);
    T pop();
    size_t size()
    {
        return qSize;
    }

private:

    std::list<T> theList;//also can use std::deque<T> theList;
    size_t qSize;

};

template<class T>
T MyQueue<T>::pop()
{
    if(qSize<=0)
    {
    	throw std::underflow_error("queue is empty, can't pop.");
    }

    --qSize;
    T t=theList.front();
    theList.pop_front();
    return t;
}

template<class T>
void MyQueue<T>::push(const T& t)
{
    ++qSize;
    theList.push_back(t);
}
}
