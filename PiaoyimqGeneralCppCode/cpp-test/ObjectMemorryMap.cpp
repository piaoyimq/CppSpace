/*
 * ObjectMemorryMap.cpp
 *
 *  Created on: 2015Äê11ÔÂ21ÈÕ
 *      Author: vicky
 */

#include <iostream>

class Animal {
public:
	Animal() {
		std::cout << "Animal constructor\n";
	}

	virtual ~Animal() {
	}

	virtual void walkSpeed(int a) = 0;
};

class Person: public Animal {
public:
	enum Sex {
		Male, Femal
	};

	Person(Sex sex, std::string className) :
			_sex(sex), _className(className) {
		std::cout << "Person constructor\n";
	}

	virtual ~Person() {
	}

	virtual void walkSpeed(int a) {
		std::cout << "Person walkSpeed is: " << a << std::endl;
	}

	virtual void eatFood(std::string& food) = 0;

	void printPerson() {
		std::cout << "Person" << std::endl;
	}

	virtual Sex getSex() = 0;

protected:
	Sex _sex;

private:
	std::string _className;
};

class Student: public Person {
public:
	Student(Sex sex, std::string className, int sleepTime, int stuendtId) :
			Person(sex, className), _sleepTime(sleepTime), _studentId(stuendtId) {
		std::cout << "Student constructor\n";
	}

	void walkSpeed(int a) {
		std::cout << "Student walkSpeed is: " << a << std::endl;
	}

	virtual void eatFood(std::string& food) {
		std::cout << "eatFood is: " << food << std::endl;
	}

	virtual Sex getSex() {
		return _sex;
	}

	void getSleep() {
		std::cout << "sleep time: " << _sleepTime << std::endl;
	}

protected:
	int _sleepTime;

private:
	int _studentId;
};

class PrimaryStudent: public Student {
public:
	PrimaryStudent(Sex sex, std::string className, int sleepTime, int stuendtId,
			int age) :
			 Student(sex, className, sleepTime,
					stuendtId), _age(age) {
		std::cout << "PrimaryStudent constructor\n";
	}

	void getAge() {
		std::cout << "age is: " << _age << std::endl;
	}

private:
	int _age;
};


int main(){
//	Animal animal;
//	Person person(Person::Male, "Person");
	Student student(Person::Male, "Person", 9, 198);
	PrimaryStudent primaryStudent(Person::Male, "Person", 9, 198, 15);

	Animal* pAnimal = new Student(Person::Femal, "Student", 10, 200);

	pAnimal->walkSpeed(88);
}
