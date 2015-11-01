/*
 * reference.cpp
 *
 *  Created on: 06-May-2015
 *      Author: ezhweib
 */


#include <iostream>

using namespace std;


int main_refence(){
char *s="hello",*b="nihao";
char* &name=s;
//char &name2=s;

cout<<"name="<<name<<endl;
//cout<<"name2="<<name2<<endl;
name=b;
//name2=b;
cout<<"name="<<name<<endl;
//cout<<"name2="<<name2<<endl;

}

