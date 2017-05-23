/*
 * main.cpp
 *
 *  Created on: Sep 2, 2016
 *      Author: piaoyimq
 */

#include "SimpleFactory.h"

int main()
{
	SimpleFactory sf;
	Product *p = sf.createProduct("A");
}
