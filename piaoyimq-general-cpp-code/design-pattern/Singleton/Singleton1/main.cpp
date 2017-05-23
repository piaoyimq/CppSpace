/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

#include "singleton.h "
#include <iostream>
using namespace std;
/*
 * 在单例模式的实现过程中，需要注意如下三点：

单例类的构造函数为私有；
提供一个自身的静态私有成员变量；
提供一个公有的静态工厂方法。
 *
 * */

class Singleton
{
public:
    //全局访问接口
    static Singleton *GetInstance()
    {
        if( instance_ == NULL )
        {
            instance_ = new Singleton;
        }
        return instance_;
    }

    ~Singleton()
    {
        cout << "~Singleton"<< endl;
    }

    //提供一个回收接口，在应用中显示调用回收资源
        static void Free()
        {
            delete instance_;
            instance_ = nullptr;
        }

private:
    Singleton(const Singleton& other);	//no copy
    Singleton & operator=(const Singleton & other);	// no assign
    Singleton()
    {
          cout << "Singleton"<<endl;
    }

    static Singleton *instance_; //引用性声明
};

Singleton * Singleton::instance_ = nullptr; //定义性声明

int main(void)
{
    Singleton *s1 = Singleton::GetInstance();
    Singleton *s2 = Singleton::GetInstance();  //s2的地址等于s1，即指向同一对象

    //Singleton s3(*s1); //既然是单例模式，是不允许被拷贝的。编译会出错

    return 0;
}

