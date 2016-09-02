/*
 * context.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */


#include "context.h"
#include "strategy.h"
#include <iostream>
using namespace std;


Context ::Context(Strategy* stg )
{
 _stg = stg;
}


Context::~Context()
{
 if(!_stg)
  delete  _stg;
}


void Context::DoAction()
{
 _stg-> AlgrithmInterface() ;
}

