/*
 * UnInheritClass.cpp
 *
 *  Created on: Dec 6, 2019
 *      Author: azhweib
 */




#include <iostream>
using namespace std;
class C;

class BASE
{
        private:
                BASE(){}
                friend class C;
};


class C: public virtual BASE
{
    // Class C is not inheritable
};

class D:public C
{
//    D():C(), BASE(){} //the default D() will invoke BASE() directly, because BASE is a virtual base class.
};

int main()
{
        C c;
//        D d;
}
