#include <iostream>
#include <stdexcept>
#include <cassert>


namespace MyLinkedList
{
template<class T>
class MyQueue
{
public:
	MyQueue() :
			qHead(nullptr), qTail(nullptr), qSize(0)
	{
	}

	~MyQueue();

	size_t size()
	{
		return qSize;
	}

	void push(const T& t);

	T pop();

private:
	struct LinkedList
	{
		LinkedList(T element) :
				value(element), next(nullptr)
		{
		}

		~LinkedList()
		{
			next=nullptr;
		}

		T value;
		LinkedList* next;
	};

	LinkedList* qHead;
	LinkedList* qTail;
	size_t qSize;
};

template<class T>
MyQueue<T>::~MyQueue()
{
	while (qSize)
	{
		pop();
	}
}

template<class T>
void MyQueue<T>::push(const T& t)
{
	++qSize;
	if (nullptr == qHead)
	{
		qHead = new LinkedList(t);
		qTail = qHead;
		return;
	}

	qTail->next = new LinkedList(t);
	qTail = qTail->next;
}

template<class T>
T MyQueue<T>::pop()
{
	if (0 == qSize)
	{
		throw std::underflow_error("queue is empty, can't pop.");
	}

	--qSize;
	T t = qHead->value;
	LinkedList* temp = qHead->next;
	delete qHead;
	qHead = temp;

	return t;
}
}
