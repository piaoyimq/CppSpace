/*
 * main.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include "template.h"
#include <iostream>
using namespace std;


int main_template1(int argc,char* argv[])
{
	AbstractClass* p1 = new ConcreteClass1();
	AbstractClass* p2 = new ConcreteClass2();

	p1->TemplateMethod();
	p2->TemplateMethod();
	return 0;
}

