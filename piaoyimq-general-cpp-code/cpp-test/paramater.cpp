/*
 * paramater.cpp
 *
 *  Created on: 06-May-2015
 *      Author: piaoyimq
 */

#include <iostream>
#include <cstring>
using namespace std;

int change1(char* name) {
	name = "alter";
	return 1;
}

int change2(char* &name) {
	name = "alter";
	return 1;
}
//
//int change3(char &name){ //error
//    name="alter";
//    return 1;
//}

int change4(char **name) {
	*name = "alter";
	return 1;
}
//
//int change5(char* &name) {
//	strcpy(name,"alter");
//	return 1;
//}

//
//int change6(char* (&name)[10]) {
//
//	strcpy(name,"alter");
//	return 1;
//}

int main_1() {
	char* string = "original!";
	char s[10] = "Hello";

	change1(string);
	cout << string << endl;

	change2(string);
	cout << string << endl;

	change1(s);
	cout << s << endl;

#if 0
	change2(s);
	cout << s << endl;
#else

#endif

//    change3(string);
//    cout<<string<<endl;

	change4(&string);
	cout << string << endl;

}
