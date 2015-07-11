#include <iostream>
using namespace std;

int display(const int &r)
{
//	r=99;//error
	cout<<"r="<<r<<endl;
}

int main_const()
{
	int c=10;
	display(c);
}
