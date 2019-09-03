/*
 * ConstReference.cpp
 *
 *  Created on: Sep 3, 2019
 *      Author: azhweib
 */


//g++ -fno-elide-constructors ConstReference.cpp
#include <iostream>




class Test
{
public:
    Test()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    ~Test()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Test(const Test& t)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    Test& operator=(const Test& t)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

};


void passTest(const Test& t=Test())
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}


void testConstReference()
{
    std::cout << __LINE__ << std::endl;
    Test t1;
    passTest(t1);


    std::cout << __LINE__ << std::endl;
    const Test t2;
    passTest(t2);

    std::cout << __LINE__ << std::endl;
    passTest(Test());

    std::cout << __LINE__ << std::endl;
    passTest();
    std::cout << __LINE__ << std::endl;
}


int main()
{
    testConstReference();
}
