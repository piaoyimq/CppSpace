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

	//���������������������������s���������const Pointer&& s������������������s���������������������
//	Pointer(Pointer&& s)=delete;
//	{
//		cout << __PRETTY_FUNCTION__ << endl;
//		length = s.getlen();
//		mptr = s.getmptr();
//		name = s.name + "-" + __PRETTY_FUNCTION__; //���������������������������������������������
//		s.mptr = nullptr;
//
//		showID();
//	}

	//���������������������
	Pointer& operator=(Pointer&& s)
	{
		if (this == &s)
			return *this;
		if (mptr)
			delete[] mptr;
		length = s.getlen();
		mptr = s.mptr;
		name = s.name + "-" + __PRETTY_FUNCTION__; //������������������������������������������������
		s.mptr = nullptr;
		cout << __PRETTY_FUNCTION__ << endl;
//		showID();
		return *this;
	}

	void showID()
	{
		cout << "length���" << length << " mptr���" << mptr << "  name���" << name << endl;
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
	string name = "#NULL"; //���������������������������������������c++11���������������������������������������������������
};

Pointer test()
{
	Pointer a(2, "test");
	return a;
}

//TODO: To verify invokeing which constructor when existing copy assignment function and move assignment funciton
int main(int argc, char* argv[])
{
	//������������������������������������������������
	{
		cout << string(50, '*') << endl;
//		Pointer(4, "notname1");
		Pointer a1 = test(); //Call move constructor
//		cout << "a1.showID():\n";
//		a1.showID();

//		cout << string(50, '*') << endl;
//		Pointer a2;
//		a2 = Pointer(5, "notname2"); //���������������������������

//		cout << string(50, '*') << endl;
//		Pointer a3(Pointer(7, "notname3")); //Call move constructor
//
//		cout << "a3.showID():\n";
//		a3.showID(); //������a3���������Pointer(7, "notname3")
	}

	cout << endl;
	return 0;
}
