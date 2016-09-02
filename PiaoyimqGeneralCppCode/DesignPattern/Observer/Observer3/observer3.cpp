/*
 * observer3.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: piaoyimq
 */

#include <string>
#include <iostream>
#include <vector>
using namespace std;




class Secretary3;


// 看股票的同事类（观察对象，观察者）
class StockObserver3 {
private:
	string name;
	Secretary3* sub;

public:
	StockObserver3(string strname,Secretary3* strsub){
		name=strname;
		sub=strsub;
	}

	void Update();
};




// 秘书类（主题对象，通知者）
class Secretary3 {
private:
	vector<StockObserver3> observers;

public:
	string action;

	void Add(StockObserver3 ob) {
		observers.push_back(ob);
	}

	void Notify() {
		vector<StockObserver3>::iterator p = observers.begin();
		while (p != observers.end()) {
			(*p).Update();
			p++;
		}
	}
};




void StockObserver3::Update() {
	cout << name << ":" << sub->action << " don't watch NBA, Boss is coming." << endl;
}



// 客户端
int main_observer3() {
	Secretary3 *p = new Secretary3();    // 创建通知者

	// 观察者
	StockObserver3 *s1 = new StockObserver3("Li", p);
	StockObserver3 *s2 = new StockObserver3("Zhao", p);

	// 加入通知队列
	p->Add(*s1);
	p->Add(*s2);

	// 事件
	p->action = " boss coming";

	// 通知
	p->Notify();

	return 0;
}

