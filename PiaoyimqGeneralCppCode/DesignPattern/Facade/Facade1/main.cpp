/*
 * main.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include "facade.h"
#include <iostream>
using namespace std;


int main_facade1(int argc, char* argv[]) {
	Facade* f = new Facade();
	f->OperationWrapper();

	return 0;
}

