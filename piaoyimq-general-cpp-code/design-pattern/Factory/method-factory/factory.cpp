/*
 * factory.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

#include "factory.h"
#include "product.h"
#include <iostream>
using namespace std;

Factory::Factory() {
}

Factory::~Factory() {
}

ConcreteFactory::ConcreteFactory() {
	cout << "ConcreteFactory....." << endl;
}

ConcreteFactory::~ConcreteFactory() {
}

Product* ConcreteFactory::CreateProduct() {
	return new ConcreteProduct();
}

