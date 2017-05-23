/*
 * abstraction-imp.h
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#ifndef _ABSTRACTIONIMP_H_
#define _ABSTRACTIONIMP_H_


class AbstractionImp {
public:
	virtual ~AbstractionImp();
	virtual void Operation() = 0;
protected:
	AbstractionImp();
private:
};


class ConcreteAbstractionImpA: public AbstractionImp {
public:
	ConcreteAbstractionImpA();
	~ConcreteAbstractionImpA();
	virtual void Operation();
protected:
private:
};


class ConcreteAbstractionImpB: public AbstractionImp {
public:
	ConcreteAbstractionImpB();
	~ConcreteAbstractionImpB();
	virtual void Operation();
protected:
private:
};


#endif //~_ABSTRAC T IONIMP_H_
