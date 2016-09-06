/*
 * main.cpp
 *
 *  Created on: Sep 6, 2016
 *      Author: ezhweib
 */


#include "Builder.h"
#include <iostream>

using namespace std;

int main()
{
    Director* pDirector = new Director(new ConcreteBuilder1());
    pDirector->Construct();
    pDirector->m_pBuilder->GetProduct();

    Director* pDirector1 = new Director(new ConcreteBuilder2());
    pDirector1->Construct();
    pDirector1->m_pBuilder->GetProduct();

    return 0;
}

