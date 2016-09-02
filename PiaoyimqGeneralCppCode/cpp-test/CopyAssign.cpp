/*
 * CopyAssign.cpp
 *
 *  Created on: 07-May-2015
 *      Author: piaoyimq
 */

#include <iostream>
using namespace std;

class Food {
public:
	Food(int type = 10) {
		m_type = 10;
		cout << "Food constructor" << endl;
	}

	Food(Food &other)                 //拷贝构造函数
			{
		m_type = other.m_type;
		cout << "Food copy constructor" << endl;
	}

	Food & operator =(Food &other)      //重载赋值=函数
			{
		m_type = other.m_type;
		cout << "Food assignment" << endl;
		return *this;
	}
private:
	int m_type;
};



class Dog: public Food {/*构造函数调用顺序，先基类，然后内嵌对象，最后自己构造函数体内的东西*/
public:
#if 0//注意：此处是构造函数，并非拷贝构造函数
	Dog(Food &food)//通过函数体内赋值的方式初始化的构造函数
	{
		m_food = food;               //调用赋值函数初始化内嵌对象
		cout << "Dog assignment" << endl;
	}
#else
	Dog(Food &food):m_food(food)//通过初始化列表方式初始化的构造函数，调用拷贝构造函数，初始化内嵌对象
	{
		cout << "Dog assignment" << endl;
	}
#endif

	Dog(){}//当定义了带参的构造函数时，还想要定义不带参的对象时，就必须定义不带参的构造函数

	int test(Food &food)//这里会调用food 的构造函数???
	{
		cout<<"test"<<endl;
	}

private:
	Food m_food;
};

int main() {

	//使用
	Food fd;
	cout << "1-1" << endl;

	Dog dog(fd);//子类调用带形参的构造函数，
//
//	Dog dog;
////	dog.test(fd);

}
