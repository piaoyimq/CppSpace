#include <iostream>
#include <stdint.h>
#include <stdexcept>
namespace MyArray
{
#define max_size 10


#ifndef max_size
#define max_size 3
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
		return qSize == max_size;
	}

	void push(const T& t);

	T pop();

private:
	size_t head;
	size_t tail;
	size_t qSize;
	T theQueue[max_size];
};

template<class T>
void MyQueue<T>::push(const T& t)
{
	if (isFull())
	{
		throw std::overflow_error("queue is full, can't push.");
	}

	if (max_size == tail)
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
	if (max_size == head)
	{
		head = 0;
	}
	--qSize;
	return t;
}
}
//int main()
//{
//	MyQueue<int> qe;
//	std::cout << "qSize=" << qe.size() << std::endl;
//	qe.push(1);
//	qe.push(2);
//	qe.push(3);
//	std::cout << "qSize=" << qe.size() << std::endl;
//	qe.push(4);
//	std::cout << "qSize=" << qe.size() << std::endl;
//
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "pop=" << qe.pop() << std::endl;
//
//	std::cout << "qSize=" << qe.size() << std::endl;
//
//	qe.push(4);
//	qe.push(5);
//	qe.push(6);
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "pop=" << qe.pop() << std::endl;
//	std::cout << "qSize=" << qe.size() << std::endl;
//
//	MyQueue<int> q;
//	for (int j = 0; j < 5; j++)
//	{
//		for (int i = 0; i < 10; i++)
//		{
//			if (!q.isFull())
//			{
//				std::cout <<"push=" <<i << std::endl;
//				q.push(i);
//			}
//		}
//
//		while (q.size())
//		{
//			std::cout << q.pop() << std::endl;
//		}
//	}
//
//	return 0;
//}

