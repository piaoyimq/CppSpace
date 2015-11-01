/*
 * iterator.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: ezhweib
 */

#include "iterator.h"
#include "aggregate.h"
#include <iostream>
using namespace std;




Iterator::Iterator() {
}



Iterator::~Iterator() {
}



ConcreteIterator::ConcreteIterator(Aggregate* ag , int idx){
	this->_ag = ag;
	this->_idx = idx;
}



ConcreteIterator::~ConcreteIterator() {
}



Object ConcreteIterator::CurrentItem(){
	return _ag->GetItem(_idx);
}



void ConcreteIterator::First( ){
	_idx = 0;
}



void ConcreteIterator::Next() {
	if (_idx < _ag->GetSize()){
		_idx++;
	}
}



bool ConcreteIterator::IsDone() {
	cout << "GetSize:" << _ag->GetSize() << endl;
	return ( _idx == _ag ->GetSize());
}
