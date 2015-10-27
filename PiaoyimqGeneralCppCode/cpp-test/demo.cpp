/*
 * demo.cpp
 *
 *  Created on: 07-May-2015
 *      Author: ezhweib
 */

#include <iostream>

using namespace std;



class A
{
public:
	A(){std::cout << "A ctor" << std::endl;}
	A(const A& a){std::cout << "A ctor 2" << std::endl;}
	~A(){std::cout << "A dtor" << std::endl;}
};

class B
{
public:
	B(int a){std::cout << "B ctor" << std::endl;}
	B(){}//如果定义了带形参的构造函数，还想要无参构造函数，就必须自己定义，因为定义了带形参的构造函数就不会调用默认无参构造函数。
	B(const B& b){std::cout << "B ctor 2" << std::endl;}
	~B(){std::cout << "B dtor" << std::endl;}
};


void func(A a, B b)
{
	std::cout << "&a=" << &a << std::endl;
	std::cout << "&b=" << &b << std::endl;
}


int main()
{
	A a;
	B b(3);
	B c;

	std::cout << "Call func()" << std::endl;
	func(a, b);

	std::cout << "Exit program" << std::endl;

	return 0;
}

