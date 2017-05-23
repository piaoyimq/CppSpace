#include <iostream>
#include <vector>

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
		throw "Empty";
	}

	T t = theVector[0];
	theVector.erase(theVector.begin());
	--qSize;
	return t;
}

int main()
{
	MyQueue<int> qe;
	std::cout << "qSize=" << qe.size() << std::endl;
	qe.push(3);

	qe.push(4);

	std::cout << "qSize=" << qe.size() << std::endl;
	int a1 = qe.pop();
	int a2 = qe.pop();
	// int a3=qe.pop();
	std::cout << "pop=" << a1 << std::endl;
	std::cout << "pop=" << a2 << std::endl;
	std::cout << "qSize=" << qe.size() << std::endl;
//	qe.pop();

	MyQueue<int> q;
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
			q.push(i);
		std::cout << "size=" << q.size() << std::endl;
		while (q.size())
		{
			int a = q.pop();
			std::cout << "a=" << a << std::endl;
		}
	}
	 q.pop();
	return 0;
}

