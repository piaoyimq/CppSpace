/*
 * builder.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include <string>
#include <iostream>
#include <vector>
using namespace std;


// 最终的产品类
class Product2 {
private:
	vector<string> m_product;
public:
	void Add(string strtemp)
	{
		m_product.push_back(strtemp);
	}
	void Show() {
		vector<string>::iterator p = m_product.begin();
		while (p != m_product.end()) {
			cout << *p << endl;
			p++;
		}
	}
};


// 建造者基类
class Builder2 {
public:
	virtual void BuilderA()=0;
	virtual void BuilderB()=0;
	virtual Product2* GetResult()=0;
};


// 第一种建造方式
class ConcreteBuilder1: public Builder2 {
private:
	Product2* m_product;
public:
	ConcreteBuilder1() {
		m_product = new Product2();
	}

	virtual void BuilderA() {
		m_product->Add("one");
	}

	virtual void BuilderB() {
		m_product->Add("two");
	}

	virtual Product2* GetResult() {
		return m_product;
	}
};


// 第二种建造方式
class ConcreteBuilder2: public Builder2 {
private:
	Product2 * m_product;
public:
	ConcreteBuilder2() {
		m_product = new Product2();
	}

	virtual void BuilderA() {
		m_product->Add("A");
	}

	virtual void BuilderB() {
		m_product->Add("B");
	}

	virtual Product2* GetResult() {
		return m_product;
	}
};


// 指挥者类
class Direct2 {
public:
	void Construct(Builder2* temp) {
		temp->BuilderA();
		temp->BuilderB();
	}
};


// 客户端
int main_builder2() {
	Direct2 *p = new Direct2();
	Builder2 *b1 = new ConcreteBuilder1();
	Builder2 *b2 = new ConcreteBuilder2();

	p->Construct(b1);     // 调用第一种方式

	Product2 *pb1 = b1->GetResult();
	pb1->Show();

	p->Construct(b2);           // 调用第二种方式

	Product2 *pb2 = b2->GetResult();
	pb2->Show();
	return 0;
}
