/*
 * main.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include "facade.h"
#include <iostream>
using namespace std;


int main(int argc, char* argv[]) {
	Facade* f = new Facade();
	f->OperationWrapper();

	return 0;
}

