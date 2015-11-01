/*
 * main.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include "abstraction.h"
#include "abstraction-imp.h"
#include <iostream>
using namespace std;


int main_bridge1(int argc, char *argv[]) {
	AbstractionImp* imp = new ConcreteAbstractionImpA();
	Abstraction* abs = new RefinedAbstraction(imp);
	abs->Operation();

	return 0;
}

