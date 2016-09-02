/*
 * product.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */


#include "product.h"
#include <iostream>
using namespace std;


Builder_Product::Builder_Product() {
	ProducePart();
	cout << "return a product" << endl;
}


Builder_Product::~Builder_Product() {
}


void Builder_Product::ProducePart() {
	cout << "build  part of product.." << endl;
}


ProductPart::ProductPart() {
//	cout<<" build productpart.."<<endl;
}


ProductPart::~ProductPart() {
}


ProductPart* ProductPart::BuildPart() {
	return new ProductPart;
}
