#include <iostream>
#include <vector>
#include <stdexcept>


namespace StdVector
{
template<class T>
class MyQueue
{
public:
	MyQueue() :
			qSize(0)
	{
	}

	void push(const T& t);

	T pop();

	size_t size()
	{
		return qSize;
	}

private:
	std::vector<T> theVector;
	size_t qSize;
};

template<class T>
void MyQueue<T>::push(const T& t)
{
	theVector.push_back(t);
	++qSize;
}

template<class T>
T MyQueue<T>::pop()
{
	if (qSize <= 0)
	{
		throw std::underflow_error("queue is empty, can't pop.");
	}

	T t = theVector[0];
	theVector.erase(theVector.begin());
	--qSize;
	return t;
}
}

