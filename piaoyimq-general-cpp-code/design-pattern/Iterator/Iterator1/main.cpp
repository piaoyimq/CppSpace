/*
 * main.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: piaoyimq
 */

/*
 * Aggregate is a array.
 *
 * */

#include "iterator.h"
#include "aggregate.h"
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	Aggregate *ag = new ConcreteAggregate();
	Iterator* it = new ConcreteIterator(ag);

	for (; !(it->IsDone()); it->Next())
	{
		cout << it->CurrentItem() << endl;
	}

	return 0;
}
