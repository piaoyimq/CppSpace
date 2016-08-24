/*
 * CppDefaultFunction.cpp
 *
 *  Created on: 01-Aug-2016
 *      Author: piaoyimq
 */

#include <iostream>
using namespace std;

class Pointer
{
public:
	Pointer(const int i, const string &n)
	{
		mptr = new int[i];
		length = i;
		name = n;
		cout << __PRETTY_FUNCTION__ << " " << name << endl;
//		showID();
	}
	Pointer() :
			mptr(nullptr), length(0)
	{
		cout << __PRETTY_FUNCTION__ << " " << name << endl;
//		showID();
	}
	virtual ~Pointer()
	{
		cout << __PRETTY_FUNCTION__ << "____" << name << endl;
		if (mptr)
			delete[] mptr;
		mptr = nullptr;

	}

	Pointer(const Pointer& s)
	{
		length = s.getlen();
		mptr = new int[length];
		name = s.name;
		cout << __PRETTY_FUNCTION__ << endl;
//		showID();
	}

	Pointer& operator=(const Pointer &s)
	{
		if (this == &s)
			return *this;
		if (mptr)
			delete[] mptr;
		length = s.getlen();
		mptr = new int[length];
		name = s.name;
		cout << __PRETTY_FUNCTION__ << endl;
//		showID();
		return *this;
	}

	//移动构造函数，参数s不能是const Pointer&& s，因为要改变s的成员数据的值
//	Pointer(Pointer&& s)=delete;
//	{
//		cout << __PRETTY_FUNCTION__ << endl;
//		length = s.getlen();
//		mptr = s.getmptr();
//		name = s.name + "-" + __PRETTY_FUNCTION__; //调用移动构造函数时，加一个标记
//		s.mptr = nullptr;
//
//		showID();
//	}

	//移动赋值运算符
	Pointer& operator=(Pointer&& s)
	{
		if (this == &s)
			return *this;
		if (mptr)
			delete[] mptr;
		length = s.getlen();
		mptr = s.mptr;
		name = s.name + "-" + __PRETTY_FUNCTION__; //调用移动赋值运算符时，加一个标记
		s.mptr = nullptr;
		cout << __PRETTY_FUNCTION__ << endl;
//		showID();
		return *this;
	}

	void showID()
	{
		cout << "length：" << length << " mptr：" << mptr << "  name：" << name << endl;
	}

	int getlen() const
	{
		return length;
	}

	int* getmptr() const
	{
		return mptr;
	}

private:
	int* mptr;
	int length;
	string name = "#NULL"; //该参数用来标记不同的对象，c++11支持直接在类的数据成员定义处初始化
};

Pointer test()
{
	Pointer a(2, "test");
	return a;
}

int main(int argc, char* argv[])
{
	//加花括号是为了观察析构函数的调用
	{
		cout << string(50, '*') << endl;
//		Pointer(4, "notname1");
		Pointer a1 = test(); //Call move constructor
//		cout << "a1.showID():\n";
//		a1.showID();

//		cout << string(50, '*') << endl;
//		Pointer a2;
//		a2 = Pointer(5, "notname2"); //调用移动赋值运算符

//		cout << string(50, '*') << endl;
//		Pointer a3(Pointer(7, "notname3")); //Call move constructor
//
//		cout << "a3.showID():\n";
//		a3.showID(); //验证a3确实是Pointer(7, "notname3")
	}

	cout << endl;
	return 0;
}
