/*
 * iterator.h
 *
 *  Created on: 23-Apr-2015
 *      Author: piaoyimq
 */

#ifndef _ITERATOR_H_
#define _ITERATOR_H_

class Aggregate;


typedef int Object;


class Iterator
{
public:
	virtual ~Iterator( );
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Object CurrentItem() = 0;

protected:
	Iterator();

private:
};




class ConcreteIterator:public Iterator
{
public:
	ConcreteIterator(Aggregate * ag , int idx = 0);
	~ConcreteIterator();
	void First();
	void Next();
	bool IsDone();
	Object CurrentItem();

protected:

private:
	Aggregate * _ag;
	int _idx;
};

#endif //~_ITERAT O R_H_
