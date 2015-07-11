/*
 * factory-strategy.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

/*
 * startegy2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include<iostream>
#include <string>
using namespace std;

class COperation2 {
public:
	int m_nFirst;
	int m_nSecond;

	virtual double GetResult() {
		double dResult = 0;
		return dResult;
	}
};

// 策略具体类—加法类
class AddOperation2: public COperation2 {
public:
	AddOperation2(int a, int b) {
		m_nFirst = a;
		m_nSecond = b;
	}

	AddOperation2() {
	}

	virtual double GetResult() {
		return m_nFirst + m_nSecond;
	}
};


class Context2 {
private:
	COperation2* op;
public:
	Context2(char cType) {
		switch (cType) {
		case '+':
			op = new AddOperation2(3, 8);
			break;
		default:
			op = new AddOperation2();
			break;
		}
	}
	double GetResult() {
		return op->GetResult();
	}
};


// 客户端
int main_fast() {
	int a, b;
	cout << "a=";
	cin >> a;
	cout << "b=";
	cin >> b;

	Context2 *test = new Context2('+');
	cout << test->GetResult() << endl;
	return 0;
}

