#include "abstract-factory.h"
#include <iostream>
using namespace std;


int main3(int argc, char *argv[]) {
	AbstractFactory* cf1 = new ConcreteFactory1();
	AbstractProductA* a1=cf1->CreateProductA();
	AbstractProductB* b1=cf1->CreateProductB();

	AbstractFactory* cf2 = new ConcreteFactory2();
	AbstractProductA* a2 =cf2->CreateProductA();
	AbstractProductB* b2=cf2->CreateProductB();

	return 0;
}
