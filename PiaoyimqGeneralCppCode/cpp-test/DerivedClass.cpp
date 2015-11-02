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
	G(int gg)
	{
        g = gg;
		cout<<"Class G(g) constructor"<<endl;
	}

    int g;
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
	M(int mm)
	{
        m = mm;
		cout<<"Class M(m) constructor"<<endl;
	}

    int m;
};

//普通类N
class N
{
public:
	N()
	{
		cout<<"Class N constructor"<<endl;
	}
	N(int nn)
	{
       n == nn; 
		cout<<"Class N(n) constructor"<<endl;
	}

    int n;
};

/*class A and B is abstract class, others is generla class */
class Test: public E,public B,virtual public C,public A,virtual public D,public F
{
public:
	Test():B(),A(),D(),C(),F(),E(),g(4),n(5),m1(6)
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
    N n;
    M m1;
	static H h;
	static M m;//not initialize, won't invoke the constructor function
};

H Test::h;

int main(int argc, char* argv[])
{
	Test test;
	return 1;
}

