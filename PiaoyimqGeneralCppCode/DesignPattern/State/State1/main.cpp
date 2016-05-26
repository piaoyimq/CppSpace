/*
 * main.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: ezhweib
 */

#include "Context.h"
#include "State.h"
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
    State* st = new NotInsertMoneyState();

    Context* con = new Context(st);

    con->OperationChangState(); // No state->NotInsertMoneyState
    con->OperationChangState(); // NotInsertMoneyState->InsertMoneyState
    con->OperationChangState(); // InsertMoneyState->SoldingState
    con->OperationChangState(); // SoldingState->SoldOutState

    delete con;
    st = NULL;

    return 0;
}
