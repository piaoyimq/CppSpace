/*
 * abstraction.h
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#ifndef _ABSTRACTION_H_
#define _ABSTRACTION_H_


class AbstractionImp;


class Abstraction {
public:
	virtual ~Abstraction();
	virtual void Operation() = 0;
protected:
	Abstraction();
private:
};


class RefinedAbstraction: public Abstraction {
public:
	RefinedAbstraction(AbstractionImp* imp);
	~RefinedAbstraction();
	void Operation();
protected:
private:
	AbstractionImp* _imp;
};


#endif //~_ABSTRAC T ION_H_
