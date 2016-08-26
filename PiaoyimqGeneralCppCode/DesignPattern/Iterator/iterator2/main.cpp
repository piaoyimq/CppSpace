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
 * operator++(), --(), *()and so on.
 * Note: next() is like operator++(), previous() is like operator--(), *() is getItem().
 * */

#include "vector-iterator2.h"

int main()
{
    ConcreteAggregate* pName = NULL;
    pName = new ConcreteAggregate();
    if(NULL != pName)
    {
        pName->Push("hello");
        pName->Push("word");
        pName->Push("cxue");
    }
    Iterator* iter = NULL;
    iter = pName->CreateIterator();
    if(NULL != iter)
    {
        string strItem = iter->First();
        while(!iter->IsEnd())
        {
            cout << iter->GetCur() << " is ok" << endl;
            iter->Next();
        }
    }
    system("pause");

    return 0;
}

