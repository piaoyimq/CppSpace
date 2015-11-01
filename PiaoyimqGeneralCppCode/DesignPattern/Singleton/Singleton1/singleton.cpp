/*
 * singleton.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: ezhweib
 */

#include "singleton.h "
#include <iostream>
using namespace std;


Singleton* Singleton::_instance = 0;


Singleton::Singleton() {
	cout << "Singleton...." << endl;
}


Singleton* Singleton::Instance() {
	if (_instance == 0) {
		_instance = new Singleton();
	}
	return _instance;
}
