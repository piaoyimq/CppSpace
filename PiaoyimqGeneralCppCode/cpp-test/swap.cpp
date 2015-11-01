#include <iostream>
using namespace std;




void swap(int &a ,int &b)
{
	int t;
	t=a;
	a=b;
	b=t;
}


int main_swap()
{
	int x(5),y(6);

	cout<<"x="<<x<<"	y="<<y<<endl;
	swap(x,y);
	cout<<"x="<<x<<"	y="<<y<<endl;
	
}
