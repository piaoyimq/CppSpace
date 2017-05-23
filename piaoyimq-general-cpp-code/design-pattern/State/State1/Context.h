/*
 * Context.h
 *
 *  Created on: Apr 21, 2016
 *      Author: piaoyimq
 */

#ifndef PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_CONTEXT_H_
#define PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_CONTEXT_H_
class State;
/**
 *
 **/
class Context {
public:
    Context();
    Context(State* state);
    ~Context();
    void OprationInterface();
    void OperationChangState();
protected:
private:
    friend class State; //表明在Sta t e 类中可以访问Context 类的priv at e 字段
    bool ChangeState(State* state);
private:
    State* _state;
};

#endif /* PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_CONTEXT_H_ */
