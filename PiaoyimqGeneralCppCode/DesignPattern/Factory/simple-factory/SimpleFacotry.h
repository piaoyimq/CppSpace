/*
 * Facotry.h
 *
 *  Created on: Sep 2, 2016
 *      Author: ezhweib
 */

#ifndef PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_SIMPLEFACOTRY_H_
#define PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_SIMPLEFACOTRY_H_
#include "Product.h"


class SimpleFactory
{
public:
	SimpleFactory();

	~SimpleFactory();

	Product* createProduct(const std::string& proname);
};



#endif /* PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_SIMPLEFACOTRY_H_ */
