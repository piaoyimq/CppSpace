/*
 * startegy2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */

#include<iostream>
#include <string>
using namespace std;


class COperation {
public:
	int m_nFirst;
	int m_nSecond;

	virtual double GetResult() {
		double dResult = 0;
		return dResult;
	}
};


// 策略具体类—加法类
class AddOperation: public COperation {
public:
	AddOperation(int a, int b) {
		m_nFirst = a;
		m_nSecond = b;
	}

	virtual double GetResult() {
		return m_nFirst + m_nSecond;
	}
};

class Context {
private:
	COperation* op;
public:
	Context(COperation* temp) {
		op = temp;
	}

	double GetResult() {
		return op->GetResult();
	}
};

// 客户端
int main_strategy2() {
	int a, b;
	char c;
	cout << "a=";
	cin >> a;
	cout << "b=";
	cin >> b;
	cout << "Please input operator:";
	cin >> c;

	switch (c) {
		case '+':
			Context *context;
			context = new Context(new AddOperation(a,b));
			cout << context->GetResult() << endl;
			break;
		default:
			break;
	}

	return 0;
}

