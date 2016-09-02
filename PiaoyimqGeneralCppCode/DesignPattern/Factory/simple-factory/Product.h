/*
 * Product.h
 *
 *  Created on: Sep 2, 2016
 *      Author: piaoyimq
 */

#ifndef PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_PRODUCT_H_
#define PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_PRODUCT_H_

class Product
{
public:
	Product();
	virtual ~Product() = 0;
};

class ConcreteProductA: public Product
{
public:
	ConcreteProductA();
	virtual ~ConcreteProductA();
};

class ConcreteProductB: public Product
{
public:
	ConcreteProductB();
	virtual ~ConcreteProductB();
};

#endif /* PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_FACTORY_SIMPLE_FACTORY_PRODUCT_H_ */
