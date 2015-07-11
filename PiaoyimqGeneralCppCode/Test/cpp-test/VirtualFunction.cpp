/*
 * VirtualFunction.cpp
 *
 *  Created on: 09-May-2015
 *      Author: ezhweib
 */

#include <iostream>
#include <string>
using namespace std;

class A {
public:
	void output() {
		cout << "A output" << endl;
	}


	virtual int display() {
		cout << "A display" << endl;
	}

	int display(int a){
			cout<<"A overloaded function"<<endl;
		}
};

class B: public A {
public:
	void output() {
		cout << "B output" << endl;
	}
#define B_OK
//#undef B_OK
#ifdef B_OK

	int display(int a) {//注意此处该函数并非虚函数，因为虚函数必须和基类中的原型完全相同
		cout << "B display overloaded function" << endl;
	}
#else
	/*一个成员函数被声明为虚函数后，在同一类族中的类就不能再定义一个非virtual的
	 * 但与该虚函数具有相同的参数(包括个数和类型)和函数返回值类型的同名函数。
	 * 所以，此处display()虽无virtual修饰，但实际还是虚函数*/
	 int display(){
			cout<<"B dispaly"<<endl;
		}
#endif
};

class C: public B {
public:
	void output() {
		cout << "C output" << endl;
	}

	virtual int display() {
		cout << "C display" << endl;
	}
};

class Test: public C {
public:
	void output() {
		cout << "Test output" << endl;
	}

	virtual int display() {//virtual可以省略，因为只要基类该同名函数定义为virtual,其子类的同名，不过建议保留，这样可以提高可读性
		cout << "Test display" << endl;
	}

	int show(){
		cout<<"Test show"<<endl;
	}

	int display(int a){
		cout<<"Test overloaded function"<<endl;
	}
};




int main(int argc, char* argv[]) {
	A a, *pa=NULL;
	B b, *pb=NULL;
	C c, *pc=NULL;
	Test t, *pt=NULL;

#ifdef B_OK
	pa=&a;
	pa->display();
	pa=&b;
//	b.display();//error,class B 没有这个函数，因为被同名隐藏了
	b.display(1);
	pa->display();//由于class B没有虚函数，故不能实现多态，只能是类型兼容规则，调用基类的函数
	pa->display(1);//由于class B没有虚函数，故不能实现多态，只能是类型兼容规则，调用基类的函数

#else
	cout<<"_____pa____"<<endl<<endl;;
	pa=&a;
	pa->output();
	pa->display();

	pa=&b;
	pa->output();
	pa->display();

	pa=&t;
	pa->output();
	pa->display();

	cout<<"_____pb____"<<endl<<endl;
//	pb=&a;//error ,不可以用基类初始化子类，违反类型兼容规则

	pb=&b;
	pb->output();
	pb->display();

	cout<<"_____pt____"<<endl<<endl;
	pb=&t;
	pt=&t;
	pb->output();//调用基类B的output()
	cout<<"1-1"<<endl;
	pt->output();//调用派生类Test的output()
	pb->display();
	pt->display();

	pt->show();
	pt->display(3);
//	pb->show();//error,基类B没有show函数，所以不可调用

//	pb->display(3);//error,被该函数被同名隐藏了，只要子类有一个函数和基类的同名，就可以隐藏基类所有的同名函数
#endif
	return 1;
}

