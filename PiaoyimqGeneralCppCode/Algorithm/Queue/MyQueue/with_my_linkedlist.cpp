#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <cassert>
template<class T>
class MyQueue
{
public:

	struct LinkedList
	{
		LinkedList(T element) :
				value(element), next(nullptr)
		{
		}
		T value;
		LinkedList* next;
	};

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

int main()
{
	MyQueue<int> qe;
	std::cout << "qSize=" << qe.size() << std::endl;
	qe.push(1);
	qe.push(2);
	qe.push(3);
	std::cout << "qSize=" << qe.size() << std::endl;
	qe.push(4);
	std::cout << "qSize=" << qe.size() << std::endl;

	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;

	std::cout << "qSize=" << qe.size() << std::endl;

	qe.push(4);
	qe.push(5);
	qe.push(6);
	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;
	std::cout << "pop=" << qe.pop() << std::endl;


	std::cout << "qSize=" << qe.size() << std::endl;

	MyQueue<int> q;
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			std::cout << "push=" << i << std::endl;
			q.push(i);
		}

		while (q.size())
		{
			std::cout << q.pop() << std::endl;
		}
	}

	return 0;
}
