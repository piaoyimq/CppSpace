/*
 * Protected.cpp
 *
 *  Created on: 08-May-2015
 *      Author: piaoyimq
 */

#include <iostream>
using namespace std;

class A {
private://将构造函数和析构函数声明为protected，这样可以确保创建的对象只在堆上，不在栈上。
	A(){
		cout << "A constructor" << endl;
	}

	~A(){
		cout << "A destructor" << endl;
	}

public:
	static A* create() {
		return new A(); //调用保护的构造函数
	}

	void destroy()
	{
		delete this; //调用保护的析构函数
	}
};


int main(){
	A* pa= A::create();
	pa->destroy() ;
}


