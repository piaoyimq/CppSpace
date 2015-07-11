/*
 * facade2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */


#include<iostream>
#include <string>
using namespace std;

class SubSysOne
{
public:
  void MethodOne()
  {
    cout<<"Method 1"<<endl;
  }
};

class SubSysTwo
{
public:
  void MethodTwo()
  {
    cout<< "Method 2"<<endl;
  }
};

class SubSysThree
{
public:
  void MethodThree()
  {
    cout<<"Method 3"<<endl;
  }
};

// 外观类
class Facade2
{
private:
  SubSysOne* sub1;
  SubSysTwo* sub2;
  SubSysThree* sub3;
public:
  Facade2()
  {
    sub1 = new SubSysOne();
    sub2 = new SubSysTwo();
    sub3 = new SubSysThree();
  }
  ~Facade2()
  {
    delete sub1;
    delete sub2;
    delete sub3;
  }

  void FacadeMethod()
  {
    sub1 ->MethodOne();
    sub2 ->MethodTwo();
    sub3 ->MethodThree();
  }
};

// 客户端
int main_facade2()
{
  Facade2* test = new Facade2();
  test ->FacadeMethod();
  return 0;
}



