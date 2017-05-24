/*
 * with-my-heap-array.hpp
 *
 *  Created on: May 24, 2017
 *      Author: piaoyimq
 */
#include <stdexcept>

namespace MyHeapArray
{
template<class T>
class MyQueue
{
public:
	MyQueue(size_t maxQSize): maxSize(maxQSize), qSize(0),head(0),tail(0)
	{
		theArray = new T[maxSize];
	}

	~MyQueue()
	{
		delete []theArray;
	}

	size_t size()
	{
		return qSize;
	}

	bool isFull()
	{
		return qSize==maxSize;
	}

	void push(const T& t);

	T pop();

private:
	T* theArray;
	size_t maxSize;
	size_t qSize;
	size_t head;
	size_t tail;
};

template<class T>
void MyQueue<T>::push(const T& t)
{
	if(isFull())
	{
		throw std::overflow_error("queue is full, can't push.");
	}

	theArray[tail]=t;
	++qSize;
	++tail;
	if(tail>=maxSize)
	{
		tail=0;
	}
}

template<class T>
T MyQueue<T>::pop()
{
	if(0==size())
	{
		throw std::underflow_error("queue is empty, can't pop.");
	}

	T t = theArray[head];
	--qSize;
	++head;
	if(head>=maxSize)
	{
		head=0;
	}

	return t;
}
}
