/*
 * main.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: piaoyimq
 */

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
		if (instance_ == NULL)
		{
			instance_ = new Singleton;
		}
		return instance_;
	}

	~Singleton()
	{
		cout <<__PRETTY_FUNCTION__ << endl;
	}

	class Garbo //资源回收机制
	{
		/*
		 * 这种方式提供的处理方式显然要比Singleton1的Free(), 来的要便捷，因为它依靠内部提供的Garbo嵌套类来提供服务，
		 * 当Singleton类生命周期结束时，Garbo的类对象garbo_也要销毁， 它将调用析构函数,
		 * 而在析构函数中又自动地释放了Singleton单例类申请的一些资源，这种实现就比较智能化。不需要手动释放资源。这是它的优势。
		 * */
	public:
		~Garbo()
		{
			delete instance_;
			instance_ = nullptr;
			cout <<__PRETTY_FUNCTION__ << endl;
		}
	};

private:
	Singleton(const Singleton& other);	//no copy
	Singleton & operator=(const Singleton & other);	// no assign
	Singleton()
	{
		cout <<__PRETTY_FUNCTION__ << endl;
	}

	static Singleton *instance_; //引用性声明
	static Garbo garbo_;  //引用性声明
};

Singleton * Singleton::instance_ = nullptr; //定义性声明
Singleton::Garbo Singleton::garbo_; //定义性声明

int main(void)
{
	Singleton *s1 = Singleton::GetInstance();
	Singleton *s2 = Singleton::GetInstance();  //s2的地址等于s1，即指向同一对象

	//Singleton s3(*s1); //既然是单例模式，是不允许被拷贝的。编译会出错

	return 0;
}

