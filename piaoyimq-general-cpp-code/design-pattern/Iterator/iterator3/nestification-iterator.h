/*
 * nestification-iterator.h
 *
 *  Created on: Aug 25, 2016
 *      Author: piaoyimq
 */

#ifndef PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_ITERATOR_ITERATOR2_VECTOR_ITERATOR_H_
#define PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_ITERATOR_ITERATOR2_VECTOR_ITERATOR_H_

#include <iostream>
#include <string>
#include <vector>

class Aggregate
{
public:
    class Iterator
    {
    public:
        int index;
        Aggregate *_aggregate;
        Iterator(Aggregate* ag, int i): _aggregate(ag), index(i) {}
        Iterator& operator++()
        {
            ++index;
            return *this;
        }

        string operator*() const
        {
            return _aggregate->m_vecItems[index];
        }

        bool operator!= (Iterator it)
        {
            return index != (it.index +1 );
        }
    };


    Iterator begin()
    {
        return Iterator (this, 0);
    }

    Iterator end()
    {
           return Iterator(this, m_vecItems.size() -1);
    }

private:
    vector<string> m_vecItems = {"hello", "hi", "nihao", "good"} ;
};
#endif /* PIAOYIMQGENERALCPPCODE_DESIGNPATTERN_ITERATOR_ITERATOR2_VECTOR_ITERATOR_H_ */
