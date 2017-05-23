/*
 * State.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: piaoyimq
 */

#include "State.h"
#include "Context.h"
#include <iostream>

using namespace std;

State::State() {
}

State::~State() {
}

void State::OperationInterface(Context* con) {
    cout << "State::.." << endl;
}

bool State::ChangeState(Context* con, State* st) {
    con->ChangeState(st);
    return true;
}

void State::OperationChangeState(Context* con) {

}


//NotInsertMoneyState

NotInsertMoneyState::NotInsertMoneyState() {
}

NotInsertMoneyState::~NotInsertMoneyState() {
}

void NotInsertMoneyState::OperationInterface(Context* con) {
    cout << "NotInsertMoneyState..." << endl;
}

void NotInsertMoneyState::OperationChangeState(Context* con) {
    OperationInterface(con);
    this->ChangeState(con, new InsertMoneyState());
}


//InsertMoneyState

InsertMoneyState::InsertMoneyState() {
}

InsertMoneyState::~InsertMoneyState() {
}

void InsertMoneyState::OperationInterface(Context* con) {
    cout << "InsertMoneyState..." << endl;
}

void InsertMoneyState::OperationChangeState(Context* con) {
    OperationInterface(con);
    this->ChangeState(con, new SoldingState());
}


//SoldingState

SoldingState::SoldingState() {
}

SoldingState::~SoldingState() {
}

void SoldingState::OperationInterface(Context* con) {
    cout << "SoldingState..." << endl;
}

void SoldingState::OperationChangeState(Context* con) {
    OperationInterface(con);
    this->ChangeState(con, new SoldOutState());
}


//SoldOutState

SoldOutState::SoldOutState() {
}

SoldOutState::~SoldOutState() {
}

void SoldOutState::OperationInterface(Context* con) {
    cout << "SoldOutState..." << endl;
}

void SoldOutState::OperationChangeState(Context* con) {
    OperationInterface(con);
    this->ChangeState(con, new NotInsertMoneyState());
}
