#include <iostream>
#include <stdint.h>
#include <stdexcept>


namespace MyStackArray
{

#ifndef MYQUEUE_MAX_SIZE
#define MYQUEUE_MAX_SIZE 5	 //if want to change the size, must define the macro before the head file.
#endif

template<class T>
class MyQueue
{
public:
	MyQueue() :
			head(0), tail(0), qSize(0)
	{
	}

	size_t size()
	{
		return qSize;
	}

	bool isFull()
	{
		return qSize == MYQUEUE_MAX_SIZE;
	}

	void push(const T& t);

	T pop();

private:
	size_t head;
	size_t tail;
	size_t qSize;
	T theQueue[MYQUEUE_MAX_SIZE];
};

template<class T>
void MyQueue<T>::push(const T& t)
{
	if (isFull())
	{
		throw std::overflow_error("queue is full, can't push.");
	}

	if (MYQUEUE_MAX_SIZE == tail)
	{
		tail = 0;
	}

	theQueue[tail] = t;
	++tail;
	++qSize;
}

template<class T>
T MyQueue<T>::pop()
{
	if (qSize == 0)
	{
		throw std::underflow_error("queue is empty, can't pop.");
	}

	T t = theQueue[head];
	++head;
	if (MYQUEUE_MAX_SIZE == head)
	{
		head = 0;
	}
	--qSize;
	return t;
}
}
