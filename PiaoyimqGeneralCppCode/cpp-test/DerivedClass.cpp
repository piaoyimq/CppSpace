/*
 * DerivedClass.cpp
 *
 *  Created on: 07-May-2015
 *      Author: ezhweib
 */


#include <iostream>
#include <string>
using namespace std;

//抽象类A
class A
{
public:
	A()
	{
		cout<<"Abstract Class A constructor"<<endl;
	}
	//纯虚函数fun
	virtual void fun1() = 0;
};

//抽象类B
class B
{
public:
	B()
	{
		cout<<"Abstract Class B constructor"<<endl;
	}
	//纯虚函数fun
	virtual void fun2() = 0;
};

//普通类C
class C
{
public:
	C()
	{
		cout<<"Class C constructor"<<endl;
	}
};

//普通类D
class D
{
public:
	D()
	{
		cout<<"Class D constructor"<<endl;
	}
};

//普通类C
class E
{
public:
	E()
	{
		cout<<"Class E constructor"<<endl;
	}
};

//普通类D
class F
{
public:
	F()
	{
		cout<<"Class F constructor"<<endl;
	}
};

//普通类D
class G
{
public:
	G()
	{
		cout<<"Class G constructor"<<endl;
	}
};

//普通类D
class H
{
public:
	H()
	{
		cout<<"Class H constructor"<<endl;
	}
};
//普通类D
class M
{
public:
	M()
	{
		cout<<"Class M constructor"<<endl;
	}
};

/*class A and B is abstract class, others is generla class */
class Test: public E,public B,virtual public C,public A,virtual public D,public F
{
public:
	Test():B(),A(),D(),C(),F(),E()
	{
		cout<<"Class Test constructor"<<endl;
	}
	void fun1()
	{
	}
	void fun2()
	{
	}
private:
	G g;
	static H h;
	static M m;//没有初始化，不调用构造函数
};

H Test::h;

int main(int argc, char* argv[])
{
	Test test;
	return 1;
}

