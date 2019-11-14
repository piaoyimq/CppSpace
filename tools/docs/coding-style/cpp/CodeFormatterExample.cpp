/*
 * CodeFormatterExample.cpp
 *
 *  Created on: Nov 14, 2019
 *      Author: azhweib
 */


#include "CodeFormatterExample.h"




CodeFormatterExample::CodeFormatterExample():publicAttribuate("public"), __protectedAttribuate("protected"), _privateAttribuate("private")
{
}


CodeFormatterExample::~CodeFormatterExample()
{
}


void CodeFormatterExample::doComplexOperation()
{
    __doProtectedOperation();
    _doPrivateOperation();
}


void CodeFormatterExample::__doProtectedOperation()
{
    //do something
}


void CodeFormatterExample::_doPrivateOperation()
{
    //do something
}
