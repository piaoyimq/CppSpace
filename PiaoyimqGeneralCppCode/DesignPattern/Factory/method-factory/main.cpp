/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

#include <iostream>

#include "../method-factory/factory.h"
#include "../method-factory/product.h"
using namespace std;

int main1(int argc, char * argv[]) {
	Factory* fac = new ConcreteFactory();
	Product * p = fac->CreateProduct();

	return 0;
}

