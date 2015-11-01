/*
 * singleton.h
 *
 *  Created on: 21-Apr-2015
 *      Author: ezhweib
 */
#ifndef _SINGLETON_H_
#define  _SINGLETON _H_

#include <iostream>
using namespace std;


class Singleton {
public:
	static Singleton * Instance();
protected:
	Singleton();//构造函数声明为protected，目的是让该类只能在内部和子类里创建，不能在外部创建。
private:
	static Singleton * _instance;
};
#endif //~_SINGLETON_H_
