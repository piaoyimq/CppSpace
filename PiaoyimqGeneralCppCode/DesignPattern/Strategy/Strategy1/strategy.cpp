/*
 * strategy.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include "strategy.h"
#include <iostream>
using namespace std;

Strategy::Strategy() {
}


Strategy::~Strategy() {
	cout << "~ Strategy....." << endl;
}


void Strategy::AlgrithmInterface() {
}


ConcreteStrategyA::ConcreteStrategyA() {
}
ConcreteStrategyA::~ConcreteStrategyA() {
	cout << "~ C oncreteStrategyA....." << endl;
}


void ConcreteStrategyA::AlgrithmInterface() {
	cout << "test ConcreteStrategyA....." << endl;
}


ConcreteStrategyB::ConcreteStrategyB() {
}


ConcreteStrategyB::~ConcreteStrategyB() {
	cout << "~ ConcreteStrategyB....." << endl;
}


void ConcreteStrategyB::AlgrithmInterface() {
	cout << "test ConcreteStrategyB....." << endl;
}

