/*
 * template2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */
#include<iostream>
#include <vector>
#include <string>
using namespace std;


class AbstractClass2 {
public:
	void Show() {
		cout << "I am " << GetName() << endl;
	}
protected:
	virtual string GetName()=0;
};


class Naruto: public AbstractClass2 {
protected:
	virtual string GetName() {
		return "Naruto ...";
	}
};


class OnePice: public AbstractClass2 {
protected:
	virtual string GetName() {
		return "OnePice ...";
	}
};


// 客户端
int main_template2() {
	Naruto* man = new Naruto();
	man->Show();

	OnePice* man2 = new OnePice();
	man2->Show();

	return 0;
}
