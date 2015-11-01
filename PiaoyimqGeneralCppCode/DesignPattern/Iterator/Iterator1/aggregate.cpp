/*
 * aggregate.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: ezhweib
 */

#include "aggregate.h"
#include "iterator.h"
#include <iostream>
using namespace std;




Aggregate::Aggregate() {
}



Aggregate::~Aggregate() {
}



ConcreteAggregate::ConcreteAggregate() {
	for(int i = 0; i < SIZE;i++)
		_objs[i] = i;
	}



ConcreteAggregate::~ConcreteAggregate(){
}



Iterator* ConcreteAggregate ::CreateIterator(){
	cout << "CreateIterator ..." << endl;
	return new ConcreteIterator(this);//???
}



Object ConcreteAggregate::GetItem(int idx){
	if(idx < this->GetSize()){
		return _objs[idx];
	}
	else{
		return -1;
	}
}



int ConcreteAggregate::GetSize(){
	return SIZE;
}

