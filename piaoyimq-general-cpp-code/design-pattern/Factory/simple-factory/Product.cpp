/*
 * leifeng.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

/*FmEventFactory*/

#include <string>
#include <iostream>
using namespace std;

#include "Product.h"

Product::Product()
{
}

ConcreteProductA::ConcreteProductA()
{
	cout << "ConcreteProductA...." << endl;
}

ConcreteProductA::~ConcreteProductA()
{
}

ConcreteProductB::ConcreteProductB()
{
	cout << "ConcreteProductB...." << endl;
}

ConcreteProductB::~ConcreteProductB()
{
}
