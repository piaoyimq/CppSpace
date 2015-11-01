/*
 * observer2.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: ezhweib
 */

#include <string>
#include <iostream>
#include <vector>
using namespace std;





class SecretaryBase;


// 抽象观察者
class CObserverBase {
protected:
	string name;
	SecretaryBase* sub;

public:
	CObserverBase(string strname, SecretaryBase* strsub) {
		name = strname;
		sub = strsub;
	}

	virtual void Update()=0;
};




// 具体的观察者，看股票的
class StockObserver: public CObserverBase {
public:
	StockObserver(string strname, SecretaryBase* strsub) :CObserverBase(strname, strsub) {
	}

	virtual void Update();
};




// 具体观察者，看 NBA的
class NBAObserver : public CObserverBase
{
public:
	NBAObserver(string strname,SecretaryBase* strsub) :CObserverBase(strname,strsub) {
	}

	virtual void Update();
};




// 抽象通知者
class SecretaryBase {
public:
	string action;
	vector<CObserverBase*> observers;

public:
	virtual void Attach(CObserverBase* observer)=0;
	virtual void Notify()=0;
};




// 具体通知者
class Secretary: public SecretaryBase {
public:
	void Attach(CObserverBase* ob) {
		observers.push_back(ob);
	}

	void Notify() {
		vector<CObserverBase*>::iterator p = observers.begin();
		while (p != observers.end()) {
			(*p)->Update();
			p++;
		}
	}
};




void StockObserver::Update() {
	cout << name << ":" << sub->action << " don't watch stock, it's time to work." << endl;
}



void NBAObserver::Update() {
	cout << name << ":" << sub->action << " don't watch NBA, Boss is coming." << endl;
}




//客户端：
int main_observer2() {
	SecretaryBase *p = new Secretary();    // 创建观察者

	CObserverBase *s1 = new NBAObserver("Li", p);// 被观察的对象
	CObserverBase *s2 = new StockObserver("Zhao", p);

	p->Attach(s1);// 加入观察队列
	p->Attach(s2);

	// 事件
	p->action = " Boss coming ,";

	// 通知
	p->Notify();

	return 0;
}

