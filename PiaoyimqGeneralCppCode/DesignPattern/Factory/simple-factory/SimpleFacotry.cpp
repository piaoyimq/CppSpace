/*
 * SimpleFacotry.cpp
 *
 *  Created on: Sep 2, 2016
 *      Author: piaoyimq
 */

#include "SimpleFactory.h"

SimpleFactory::SimpleFactory()
{
}

SimpleFactory::~SimpleFactory()
{
}

Product* SimpleFactory::createProduct(const std::string& proname)
{
	if ("A" == proname)
	{
		return new ConcreteProductA();
	}
	else
	{
		if ("B" == proname)
		{
			return new ConcreteProductB();
		}
	}

	return nullptr;
}
