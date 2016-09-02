/*
 * main.cpp
 *
 *  Created on: 23-Apr-2015
 *      Author: piaoyimq
 */

#include "subject.h"
#include "observer.h"
#include <iostream>
using namespace std;




int main_observer1(int argc, char *argv[]) {
	ConcreteSubject* sub = new ConcreteSubject();
	Observer* o1 = new ConcreteObserverA(sub);
	Observer* o2 = new ConcreteObserverB(sub);

	sub->SetState("old");
	sub->SetState("new"); // 也可以由Observer调用
	sub->Notify();

	return 0;
}

