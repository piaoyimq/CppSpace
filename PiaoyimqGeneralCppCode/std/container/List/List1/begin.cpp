// returns an iterator to the beginning
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;




int main ()
{
    list<int> l(5);
	
	
#if 0
	iota(l.begin(),l.end(),1);
#else
	int ary[]={1,2,3,4,5};

	l.assign(ary,ary+5);
#endif

    list<int>::iterator It = l.begin();
	
    while ( It != l.end() )
    {
		cout << *It++ << " ";
	}
	
    cout << endl;


#if 0    
    It = l.begin()+2;    // third element of the list,can not use "+" for list
#else
	 It = ++l.begin();	  // third element of the list, but can use "++" for list
#endif

	cout << *It << endl;

    return 0;
}
//OUTPUT:
// 1 2 3 4 5
