// Example program
#include <iostream>
#include <list>

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
        throw "QueueEmpty";
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

int main()
{
    MyQueue<int> qe;
    std::cout <<"qSize=" <<qe.size()<< std::endl;
    qe.push(3);

    qe.push(4);

    std::cout <<"qSize=" <<qe.size()<< std::endl;
    int a1=qe.pop();
    int a2=qe.pop();
    // int a3=qe.pop();
    std::cout <<"pop="<<a1 << std::endl;
    std::cout <<"pop="<<a2 << std::endl;
    std::cout <<"qSize=" <<qe.size()<< std::endl;
    return 0;
}
