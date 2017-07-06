/*
 * my-stack-test.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: piaoyimq
 */

#define BOOST_TEST_MODULE test_MyStack
#include <boost/test/included/unit_test.hpp>
#include "stack-with-std.hpp"

namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE (test_suite_implimented_by_std_container)

	BOOST_AUTO_TEST_CASE( test_MyStack_by_std_vector, * utf::description("I impliment it with a std"))
	{
		WithStd::MyStack<int> s;

		for( int i = 0; i < 10; i++ )
		{
			s.push( i );
		}

		BOOST_TEST(10 == s.size());

		for(int i=9;i >=0; i--)
		{
			BOOST_TEST(s.pop()==i);
		}

		BOOST_TEST(0 == s.size());

		BOOST_CHECK_THROW(s.pop(), std::underflow_error);
	}

BOOST_AUTO_TEST_SUITE_END () //test_suite_implimented_by_std_container
