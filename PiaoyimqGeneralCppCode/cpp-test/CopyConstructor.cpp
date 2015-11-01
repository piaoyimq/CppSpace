/*
 * CopyConstructor.cpp
 *
 *  Created on: 07-May-2015
 *      Author: ezhweib
 */

#include <iostream>
using namespace std;


class CTest {
public:
	CTest();  //构造函数
	CTest(const CTest &); //复制构造函数
	CTest & operator =(const CTest &); //赋值符
};

CTest::CTest() {
	cout << "Constructor of CTest" << endl;
}

CTest::CTest(const CTest & arg) {
	cout << "Copy Constructor of CTest" << endl;
}

CTest & CTest::operator =(const CTest & arg) {
	cout << "Assign function of CTest" << endl;
}

int main() {
	CTest a;
	CTest b(a);
	CTest c = a; //注意，这里仍然是使用复制构造函数，c是不存在（刚要建立）的对象
	a = c;       //使用赋值符

	CTest d;
	d=c;//赋值,d是一个已经存在的对象，叫做赋值

	return 0;
}

