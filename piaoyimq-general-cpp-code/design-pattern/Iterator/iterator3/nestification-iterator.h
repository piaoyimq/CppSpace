/*
 * nestification-iterator.h
 *
 *  Created on: Aug 25, 2016
 *      Author: piaoyimq
 */

/*// std::iterator example
#include <iostream>     // std::cout
#include <iterator>     // std::iterator, std::input_iterator_tag

class MyIterator : public std::iterator<std::input_iterator_tag, int>
{
  int* p;
public:
  MyIterator(int* x) :p(x) {}
  MyIterator(const MyIterator& mit) : p(mit.p) {}
  MyIterator& operator++() {++p;return *this;}
  MyIterator operator++(int) {MyIterator tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator& rhs) const {return p==rhs.p;}
  bool operator!=(const MyIterator& rhs) const {return p!=rhs.p;}
  int& operator*() {return *p;}
};

int main () {
  int numbers[]={10,20,30,40,50};
  MyIterator from(numbers);
  MyIterator until(numbers+5);
  for (MyIterator it=from; it!=until; it++)
    std::cout << *it << ' ';
  std::cout << '\n';

  return 0;
}*/
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
