/*
 * State.h
 *
 *  Created on: Apr 21, 2016
 *      Author: piaoyimq
 */

#ifndef PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_STATE_H_
#define PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_STATE_H_

class Context;


class State {
public:
    State();
    virtual ~State();
    virtual void OperationInterface(Context*) = 0;
    virtual void OperationChangeState(Context*) = 0;
protected:
    bool ChangeState(Context* con, State* st);
private:
    //bool ChangeState( Context*  con,State *  st);
};


class NotInsertMoneyState: public State {
public:
    NotInsertMoneyState();
    virtual ~NotInsertMoneyState();
    virtual void OperationInterface(Context*);
    virtual void OperationChangeState(Context*);

protected:
private:
};


class InsertMoneyState: public State {
public:
    InsertMoneyState();
    virtual ~InsertMoneyState();
    virtual void OperationInterface(Context*);
    virtual void OperationChangeState(Context*);
protected:
private:
};


class SoldingState: public State {
public:
    SoldingState();
    virtual ~SoldingState();
    virtual void OperationInterface(Context*);
    virtual void OperationChangeState(Context*);
protected:
private:
};


class SoldOutState: public State {
public:
    SoldOutState();
    virtual ~SoldOutState();
    virtual void OperationInterface(Context*);
    virtual void OperationChangeState(Context*);
protected:
private:
};
#endif /* PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_STATE_STATE_H_ */
