/*
 * main.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: piaoyimq
 */


/*iterator implementation key point:
 * Reference std::vector::iterator.
 * the aggregate class should contain the following functions:
 * begin(), end(), size()
 *the iterator class contain the following functions:
 * operator++(), --(), and so on.
 * Note: next() is like operator++(), previous() is like operator--()
 * */

#include "nestification-iterator.h"

int main()
{
   Aggregate a;

   Aggregate::Iterator it = a.begin();

   std::cout << *it << std::endl;
   it = a.end();
   std::cout << *it << std::endl;

   for(Aggregate::Iterator ite = a.begin(); ite != a.end(); ++ite)
   {
       std::cout << *ite << std::endl;
   }

    return 0;
}

