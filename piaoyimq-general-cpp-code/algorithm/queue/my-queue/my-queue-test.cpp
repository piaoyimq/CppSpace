/*
 * my-queue-test.cpp
 *
 *  Created on: May 23, 2017
 *      Author: piaoyimq
 */
#define BOOST_TEST_MODULE test_MyQueue
#include <boost/test/included/unit_test.hpp>
#include "with-my-array.hpp"
#include "with-my-linked-list.hpp"
#include "with-std-vector.hpp"
#include "with-std-list-or-deque.hpp"

BOOST_AUTO_TEST_CASE( test_with_my_array )
{
	MyArray::MyQueue<int> myQueue;

	BOOST_TEST(0==myQueue.size());

  	myQueue.push(1);
  	myQueue.push(2);
  	myQueue.push(3);

  	BOOST_TEST(3==myQueue.size());

  	myQueue.push(4);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(1==myQueue.pop());
  	BOOST_TEST(2==myQueue.pop());
  	BOOST_TEST(3==myQueue.pop());

  	BOOST_TEST(1==myQueue.size());

  	myQueue.push(4);
  	myQueue.push(5);
  	myQueue.push(6);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(5==myQueue.pop());
  	BOOST_TEST(6==myQueue.pop());

  	BOOST_TEST(0==myQueue.size());
  	BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			if (!myQueue.isFull())
			{
				myQueue.push(i);
			}
		}

		BOOST_TEST(max_size==myQueue.size());

		while (myQueue.size())
		{
			myQueue.pop();
		}

		BOOST_TEST(0==myQueue.size());
	}
	BOOST_TEST(0==myQueue.size());
}

BOOST_AUTO_TEST_CASE( with_my_linked_list )
{
	MyLinkedList::MyQueue<int> myQueue;

	BOOST_TEST(0==myQueue.size());

  	myQueue.push(1);
  	myQueue.push(2);
  	myQueue.push(3);

  	BOOST_TEST(3==myQueue.size());

  	myQueue.push(4);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(1==myQueue.pop());
  	BOOST_TEST(2==myQueue.pop());
  	BOOST_TEST(3==myQueue.pop());

  	BOOST_TEST(1==myQueue.size());

  	myQueue.push(4);
  	myQueue.push(5);
  	myQueue.push(6);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(5==myQueue.pop());
  	BOOST_TEST(6==myQueue.pop());

  	BOOST_TEST(0==myQueue.size());
  	BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i++)
		{
				myQueue.push(i);
		}

		BOOST_TEST(10==myQueue.size());

		while (myQueue.size())
		{
			myQueue.pop();
		}

		BOOST_TEST(0==myQueue.size());
	}
	BOOST_TEST(0==myQueue.size());
}


BOOST_AUTO_TEST_CASE( with_std_list_or_deque )
{
	StdListOrDeque::MyQueue<int> myQueue;

	BOOST_TEST(0==myQueue.size());

  	myQueue.push(1);
  	myQueue.push(2);
  	myQueue.push(3);

  	BOOST_TEST(3==myQueue.size());

  	myQueue.push(4);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(1==myQueue.pop());
  	BOOST_TEST(2==myQueue.pop());
  	BOOST_TEST(3==myQueue.pop());

  	BOOST_TEST(1==myQueue.size());

  	myQueue.push(4);
  	myQueue.push(5);
  	myQueue.push(6);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(5==myQueue.pop());
  	BOOST_TEST(6==myQueue.pop());

  	BOOST_TEST(0==myQueue.size());
  	BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i++)
		{
				myQueue.push(i);
		}

		BOOST_TEST(10==myQueue.size());

		while (myQueue.size())
		{
			myQueue.pop();
		}

		BOOST_TEST(0==myQueue.size());
	}
	BOOST_TEST(0==myQueue.size());
}

BOOST_AUTO_TEST_CASE( with_std_vector )
{
	StdVector::MyQueue<int> myQueue;

	BOOST_TEST(0==myQueue.size());

  	myQueue.push(1);
  	myQueue.push(2);
  	myQueue.push(3);

  	BOOST_TEST(3==myQueue.size());

  	myQueue.push(4);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(1==myQueue.pop());
  	BOOST_TEST(2==myQueue.pop());
  	BOOST_TEST(3==myQueue.pop());

  	BOOST_TEST(1==myQueue.size());

  	myQueue.push(4);
  	myQueue.push(5);
  	myQueue.push(6);

  	BOOST_TEST(4==myQueue.size());

  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(4==myQueue.pop());
  	BOOST_TEST(5==myQueue.pop());
  	BOOST_TEST(6==myQueue.pop());

  	BOOST_TEST(0==myQueue.size());
  	BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 10; i++)
		{
				myQueue.push(i);
		}

		BOOST_TEST(10==myQueue.size());

		while (myQueue.size())
		{
			myQueue.pop();
		}

		BOOST_TEST(0==myQueue.size());
	}
	BOOST_TEST(0==myQueue.size());
}
