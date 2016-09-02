/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

#include "singleton.h "
#include <iostream>
using namespace std;


int main_singleton(int argc, char *argv[]) {
	Singleton *sgn = Singleton::Instance();
	return 0;
}
