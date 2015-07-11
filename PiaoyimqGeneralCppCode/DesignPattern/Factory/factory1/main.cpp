/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: ezhweib
 */

#include "factory.h"
#include "product.h"
#include <iostream>
using namespace std;

int main1(int argc, char * argv[]) {
	Factory* fac = new ConcreteFactory();
	Product * p = fac->CreateProduct();

	return 0;
}

