/*
 * abstraction.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include "abstraction.h"
#include "abstraction-imp.h"
#include <iostream>
using namespace std;


Abstraction::Abstraction() {
}

Abstraction::~Abstraction() {
}

RefinedAbstraction::RefinedAbstraction(AbstractionImp* imp) {
	_imp = imp;
}

RefinedAbstraction::~RefinedAbstraction() {
}

void RefinedAbstraction::Operation() {
	_imp->Operation();
}

