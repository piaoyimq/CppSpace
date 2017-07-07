// Example program
#include <iostream>
#include <list>
#include <stdexcept>
#include <deque>

namespace StdListOrDeque
{
template<class T,  class Container = std::deque<T>>
class MyQueue
{
public:

    MyQueue():qSize(0){}

    ~MyQueue(){}

    void push(const T& t);

    T pop();

    size_t size()
    {
        return qSize;
    }

private:

    Container theList;

    size_t qSize;

};

template<class T, class  Container> //the container type:  std::deque, std::list
T MyQueue<T, Container>::pop()
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

template<class T, class  Container>
void MyQueue<T, Container>::push(const T& t)
{
    ++qSize;
    theList.push_back(t);
}
}
