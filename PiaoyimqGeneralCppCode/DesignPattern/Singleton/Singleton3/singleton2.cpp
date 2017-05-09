/*
 * singleton2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Singleton
{
public:
	static Singleton& GetInstance()
	{
		static Singleton instance_;
		return instance_;
	}

	~Singleton()
	{
		cout << "~Singleton" << endl;
	}
private:
	Singleton()
	{
		cout << "Singleton " << endl;
		cout << "this=" << this <<  endl;
	}
	Singleton(const Singleton &other);
	Singleton & operator=(const Singleton &other);
};


/*
 * 这种实现方式利用了static修改函数内部的变量，当第一次调用GetInstance函数时，系统将构造一个Singleton对象，在后续再次调用这个函数时，
 * 系统不再执行这个语句，也就不会再构造Singleton类对象了，而直接返回instance_本身。另外，当Singleton类对象生命周期结束时，
 * 对象会自动调用析构函数销毁，这里也就不存在申请资源之类的了，需要注意的是GetInstance()函数返回的是引用，这样就不会调用拷贝构造函数了，
 * 使用时也应该声明Singleton的引用，如下：
 * */

int main()
{
	//Singleton s1 = Singleton::GetInstance(); //it will call copy constructor, compile error.
	Singleton &s1 = Singleton::GetInstance();
	Singleton &s2 = Singleton::GetInstance(); //s1与s2是同一对象的引用

	return 0;
}
