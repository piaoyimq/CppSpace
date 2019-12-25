/*
 * CanNotUseBaseClass.cpp
 *
 *  Created on: Dec 25, 2019
 *      Author: azhweib
 */




class CanNotUseBaseClass
{
public:
    CanNotUseBaseClass(){}
protected:
    virtual ~CanNotUseBaseClass(){}  //define destrustor as a protected, user can not use this class define object, must inherict then use.
};


class D:public CanNotUseBaseClass
{
public:
    D(){};
    ~D(){};

};


int main()
{
//    CanNotUseBaseClass base;//compile error
    D d;
}
