/*
 * leifeng.cpp
 *
 *  Created on: 21-Apr-2015
 *      Author: ezhweib
 */

/*FmEventFactory*/

#include <string>
#include <iostream>
using namespace std;

//Base Class
class LeiFeng {
public:
	virtual void Sweep() {
		cout << "LeiFeng sweep the floor." << endl;
	}
};

//the undergraduate who learn from LeiFeng
class Student: public LeiFeng {
public:
	virtual void Sweep() {
		cout << "The undergraduate sweep the floor." << endl;
	}
};


//// the volunteer who learn from LeiFeng
//class Volunteer: public LeiFeng {
//public:
//	virtual void Sweep() {
//		cout << "Volunteer" << endl;
//	}
//};

// Factory Base Class
class LeiFengFactory {
public:
	virtual LeiFeng* CreateLeiFeng() {
		return new LeiFeng();
	}
};
// factory concreate class
class StudentFactory: public LeiFengFactory {
public:
	virtual LeiFeng* CreateLeiFeng() {
	return new Student();
}
};


//class VolenterFactory: public LeiFengFactory {
//public:
//	virtual LeiFeng* CreateLeiFeng() {
//		return new Volunteer();
//	}
//};

//Client
int main2(int argc, char * argv[]) {
	LeiFengFactory *sf = new StudentFactory();/*use the derived class assign to the farther class's pointer, compliant with type compatibility rules */
	LeiFeng *s = sf->CreateLeiFeng();
	s->Sweep();
	delete s;
	delete sf;
	return 0;
}

