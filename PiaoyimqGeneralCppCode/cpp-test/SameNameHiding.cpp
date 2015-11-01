/*
 * SameNameHiding.cpp
 *
 *  Created on: 09-May-2015
 *      Author: ezhweib
 */



#include<iostream>
using namespace std;
class A
{
	public:


	void print2(){
		cout<<"A print2 !"<<endl;
	}

	void print2(int x){
			cout<<"A print2 overloaded function !"<<x<<endl;
	}
};


class B:public A
{

	public:

	void print2(int x){//子类的一个同名函数就可以隐藏父类所有同名函数
		cout<<"B print2 !"<<x<<endl;
	}
};


int main(){
	B b;
	/*如果派生类中声明了与基类同名的新函数，
	 * 即使函数的参数表不同，从基类继承的同名函数的所有重载形式也都被隐藏*/
	//b.print2();//error

	b.A::print2();
	b.A::print2(3);
	return 0;
}
