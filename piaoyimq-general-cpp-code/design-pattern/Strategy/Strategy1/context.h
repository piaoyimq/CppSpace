/*
 * context.h
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#ifndef _CONTEXT_H_
#define _CONTEXT_H_


class Strategy;
/**
 *这个类是Stra tegy模式的关键，也是Strate gy模式和Templa te 模式的根本
 *这个类是Stra 区别所在。
 *Stra tegy 通过“组合” （委托）方式实现算法（实现）的异构，而Template 模
 式则采取的是继承的方式
 *这两个模式的区别也是继承和组合两种实现接口重用的方式的区别
 */


class Context {
public:
	Context(Strategy* stg);
	~Context();
	void DoAction();

protected:

private:
	Strategy* _stg;
};


#endif //~_CONTEXT _H_
