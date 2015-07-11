#include <iostream>
#include <deque>

using namespace std;





int main ()
{
	deque<int> d(3,0);
	d[0] = 100;
	d.at(1) = 200;
	
	for ( int i=0; i<3; i++ )
	{
		cout << d.at(i) << " ";
    	 }

	cout << endl;
	return 0;
}
//OUTPUT:
// 100 200 0
