/*
 * AssignmentOperator.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: piaoyimq
 */

#include <iostream>
#include <string.h>
#include <assert.h>

#define     ARRAY_SIZE      10

class A {
public:
    A& operator=(const A& that);

    ~A() {
        delete[] _name;
        _name = NULL;
    }
    ;

    void print() {
        std::cout << _name << std::endl;
    }

    void setA(char *name) {
        _name = name;
    }

private:
    char* _name;
};

#if 0   // not copy-safe or exceptionsafe.  when self-assignment occur _name point to a unknown address

A& A::operator=(const A& that) {
    delete[] _name;
    _name = new char[strlen(that._name) + 1];
    strcpy(_name, that._name);
    return *this;
}

#elif 0     // only copy-safe, but not exceptionsafe ???

A& A::operator=(const A& that) {
    if (this != &that) {
        delete[] _name;

#if 0
        _name = new char[strlen(that._name) + 1];
#else   // new throw exception
        _name = new char[100000000000000000000];
#endif

        strcpy(_name, that._name);
    }
    return *this;
}

#else   // copy-safe and exceptionsafe. ???

A& A::operator=(const A& that) {
    char *temp = NULL;

#if 0
    temp = new char[strlen(that._name) + 1];
#else   // new throw exception
    temp = new char[1000000000000000000000000];
#endif

    strcpy(temp, that._name);
    char* old = _name;
    _name = temp;
    delete[] old;
    return *this;
}

#endif

int main() {
char *pa = new char[ARRAY_SIZE];
strncpy(pa, "hello", ARRAY_SIZE);

char *pb = new char[ARRAY_SIZE];
strncpy(pb, "Good", ARRAY_SIZE);

A a;
A b;

a.setA(pa);
b.setA(pb);

std::cout << "Self-assignment:\n";
a = a;
a.print();

std::cout << "Non self-assignment:\n";
a = b;
a.print();

}
