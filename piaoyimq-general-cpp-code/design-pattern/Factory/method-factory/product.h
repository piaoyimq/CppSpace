/*
 * product.h
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */
#ifndef _PRODUCT_H_
#define _PRODUCT_H_

class Product {
public:
	virtual ~Product() =0;
protected:
	Product();
private:
};


class ConcreteProduct: public Product {
public:
	~ConcreteProduct();
	ConcreteProduct();

protected:
private:
};
#endif //~_PRODUCT_H_
