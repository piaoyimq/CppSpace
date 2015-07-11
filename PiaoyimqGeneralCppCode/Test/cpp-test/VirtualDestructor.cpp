/*
 * DerivedClass.cpp
 *
 *  Created on: 07-May-2015
 *      Author: ezhweib
 */

#include <iostream>
#include <string>
using namespace std;


class A {
public:
	A() {
		cout << "A constructor" << endl;
	}

	virtual ~A(){//感受 ~A是否为virtual,对派生类析构时的影响
		cout<<"A Destructor"<<endl;
	}
};



class B:public A
{
public:
//	B() {
//		cout << "B constructor" << endl;
//	}

	~B(){
		cout<<"B Destructor"<<endl;
	}
};



class C:public B {
public:
//	C() {
//		cout << "C constructor" << endl;
//	}

	~C(){
		cout<<"C Destructor"<<endl;
	}
};



/*基类声明了不带形参的构造函数,派生类可以不用声明构造函数*/
class Test: public C {
public:
	Test() {
		cout << "Test Constructor" << endl;
	}
	~Test(){
		cout << "Test Donstructor" << endl;
	}
};



/*本例说明，将基类的析构函数声明为虚函数时，由该基类所派生的所有派生类的析构函数也都自动成为虚函数，
 * 即使派生类的析构函数与基类的析构函数名字不相同。
 * 当基类的析构函数为虚函数时，无论指针指的是同一类族中的哪一个类对象，系统会采用动态关联，
 * 调用相应的析构函数，对该对象进行清理工作。*/

int main(int argc, char* argv[]) {
	A *a=new Test();
	delete a;	//用new申请的一定要用ｄｅｌｅｔｅ释放，否则会造成内存泄漏

	cout<<"1-1"<<endl<<endl;

	B *b=new Test();
	delete b;

	cout<<"1-2"<<endl<<endl;
	C *c=new Test();
	delete c;//此处，若~A（）不为virtual的话，则会调用 C，B，A的析构函数，而不会调用Test的析构函数。

	cout<<"1-3"<<endl<<endl;
	B *b2=new C();
	delete b2;

	return 1;
}

