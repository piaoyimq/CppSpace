/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */


#include "builder.h"
#include "product.h"
#include "director.h"
#include <iostream>
using namespace std;


int main_builder1(int argc, char * argv[]) {
	Director* d = new Director(new ConcreteBuilder());
	d->Construct();
	return 0;
}
