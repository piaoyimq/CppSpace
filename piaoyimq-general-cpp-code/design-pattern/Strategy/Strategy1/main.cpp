/*
 * main.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include "context.h"
#include "strategy.h"
#include <iostream>
using namespace std;


int main_strategy1(int argc, char * argv[]) {
	Strategy* ps = new ConcreteStrategyA();
	Context* pc = new Context(ps);
	pc->DoAction();
	if (NULL != pc)
		delete pc;
	return 0;
}

