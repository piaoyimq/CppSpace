/*
 * facade.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include "facade.h"
#include <iostream>
using namespace std;

Subsystem1::Subsystem1()
{
}

Subsystem1::~Subsystem1()
{
}

void Subsystem1::Operation()
{
    cout << "Subsystem1 operation.." << endl;
}

Subsystem2::Subsystem2()
{
}

Subsystem2::~Subsystem2()
{
}

void Subsystem2::Operation()
{
    cout << "Subsystem2 operation.." << endl;
}

Facade::Facade()
{
    _subs1 = new Subsystem1();
    _subs2 = new Subsystem2();
}

Facade::~Facade()
{
    delete _subs1;
    delete _subs2;
}

void Facade::OperationWrapper()
{
    _subs1->Operation();
    _subs2->Operation();
}

