/*
 * stack-with-std-vector.hpp
 *
 *  Created on: Jul 6, 2017
 *      Author: piaoyimq
 */

#ifndef PIAOYIMQ_GENERAL_CPP_CODE_ALGORITHM_STACK_MY_STACK_STACK_WITH_STD_HPP_
#define PIAOYIMQ_GENERAL_CPP_CODE_ALGORITHM_STACK_MY_STACK_STACK_WITH_STD_HPP_
#include <cstddef>
#include <stdexcept>
#include <deque>

namespace WithStd
{
template<class T, class Container = std::deque<T>>
class MyStack
{
public:
	MyStack() :
			_size(0)
	{
	}

	~MyStack()
	{
	}

	void push(const T& t);

	T pop();

	size_t size()
	{
		return _size;
	}

private:
	Container it;
	size_t _size;
};

template<class T, class Container>
void MyStack<T, Container>::push(const T& t)
{
	it.push_back(t);
	++_size;
}

template<class T, class Container>
T MyStack<T, Container>::pop()
{
	if (0 == _size)
	{
		throw std::underflow_error("stack is empty, can't pop.");
	}
	T tmp = it.back();
	it.pop_back();
	--_size;
	return tmp;
}
}
#endif /* PIAOYIMQ_GENERAL_CPP_CODE_ALGORITHM_STACK_MY_STACK_STACK_WITH_STD_HPP_ */
