/*
 * singleton2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Singelton2 {
private:
	Singelton2() {}//构造函数声明为private，目的是让该类只能在内部创建，不能在外部创建。

	static Singelton2 *single;

public:
	static Singelton2* GetInstance() {//此处必须声明为static，否则，外部也无法调用，因为声明了static，该函数就是一个用类名就可以调用的函数了
		if (single == NULL) {
			single = new Singelton2();//此处调用构造函数
		}
		return single;
	}
};


Singelton2* Singelton2::single = NULL; // 注意静态变量类外初始化

//客户端：
int main_singleton2() {
	Singelton2* s1 = Singelton2::GetInstance();//通过内部的函数创建类的对象
	Singelton2* s2 = Singelton2::GetInstance();

	if (s1 == s2){
		cout << "ok" << endl;
	}
	else{
		cout << "no" << endl;
	}

	return 0;
}

