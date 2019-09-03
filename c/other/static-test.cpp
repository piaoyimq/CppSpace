/*
 * static-test.cpp
 *
 *  Created on: Aug 2, 2019
 *      Author: azhweib
 */




#include <iostream>

void test_static(int t)
{
    static int a = 10;
//    a = 1;
    a +=t;
    std::cout << "a=" << a << std::endl;
}


int main()
{
    test_static(1);
    test_static(2);
    test_static(3);
}
