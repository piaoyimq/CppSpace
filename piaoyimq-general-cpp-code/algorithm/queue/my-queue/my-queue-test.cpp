
/*
 * my-queue-test.cpp
 *
 *  Created on: May 23, 2017
 *      Author: piaoyimq
 */
#define BOOST_TEST_MODULE test_MyQueue
#include <boost/test/included/unit_test.hpp>

#define MYQUEUE_MAX_SIZE 10
#include "with-my-stack-array.hpp"
#include "with-my-heap-array.hpp"
#include "with-my-linked-list.hpp"
#include "with-std-vector.hpp"
#include "with-std-list-or-deque.hpp"

namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE( test_suite_implimented_by_my_container )

	BOOST_AUTO_TEST_CASE( test_MyQueue_by_MyArray, * utf::description("I impliment it with a stack array"))
	{
	    BOOST_TEST_MESSAGE( "Testing initialization :" << "1" ); // test

	    MyStackArray::MyQueue<int> myQueue;

		BOOST_TEST(0==myQueue.size());

		myQueue.push(1);
		myQueue.push(2);
		myQueue.push(3);

		BOOST_TEST(3==myQueue.size(), "size=" <<myQueue.size()); // test

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
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

		myQueue.push(7);
		myQueue.push(8);
		myQueue.push(9);
		myQueue.push(10);
		myQueue.push(11);
		BOOST_TEST(5==myQueue.size());
		BOOST_TEST(7==myQueue.pop());

		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (!myQueue.isFull())
				{
					myQueue.push(i);
				}
			}

			BOOST_TEST(MYQUEUE_MAX_SIZE==myQueue.size());

			while (myQueue.size())
			{
				myQueue.pop();
			}

			BOOST_TEST(0==myQueue.size());
		}
		BOOST_TEST(0==myQueue.size());
	}

	BOOST_AUTO_TEST_CASE( test_MyQueue_by_MyHeapArray, * utf::description("I impliment it with a heap array"))
	{
		MyHeapArray::MyQueue<int> myQueue(10);

		BOOST_TEST(0==myQueue.size());

		myQueue.push(1);
		myQueue.push(2);
		myQueue.push(3);

		BOOST_TEST(3==myQueue.size(), "size=" <<myQueue.size()); // test

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
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);
		BOOST_CHECK_THROW(myQueue.pop(), std::underflow_error);

		myQueue.push(7);
		myQueue.push(8);
		myQueue.push(9);
		myQueue.push(10);
		myQueue.push(11);
		BOOST_TEST(5==myQueue.size());
		BOOST_TEST(7==myQueue.pop());

		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (!myQueue.isFull())
				{
					myQueue.push(i);
				}
			}

			BOOST_TEST(MYQUEUE_MAX_SIZE==myQueue.size());

			while (myQueue.size())
			{
				myQueue.pop();
			}

			BOOST_TEST(0==myQueue.size());
		}
		BOOST_TEST(0==myQueue.size());
	}


	BOOST_AUTO_TEST_CASE( test_MyQueue_by_MyLinkedList )
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

BOOST_AUTO_TEST_SUITE_END() //test_suite_implimented_by_my_container


BOOST_AUTO_TEST_SUITE( test_suite_implimented_by_std_container )

	BOOST_AUTO_TEST_CASE( test_MyQueue_by_StdListOrDeque)
	{
		StdListOrDeque::MyQueue<int, std::list<int>> myQueue;

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


	BOOST_AUTO_TEST_CASE( test_MyQueue_by_StdVector )
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

BOOST_AUTO_TEST_SUITE_END() //test_suite_implimented_by_std_container
