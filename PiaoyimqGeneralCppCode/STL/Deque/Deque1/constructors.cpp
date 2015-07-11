#include <iostream>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;




int main ()
{
    string str[]={"Alex","John","Robert"};
	
    // empty deque object
    deque<int> d1;
	
    // creates deque with 10 empty elements
    deque<int> d2(10);
	
    // creates deque with 10 elements,
    // and assign value 0 for each
    deque<int> d3(10,0);
	
    // creates deque and assigns
    // values from string array
    deque<string> d4(str+0,str+3);//not include str+2,the zone is  [str+1,str+3)
    deque<string>::iterator sIt = d4.begin();


    while ( sIt != d4.end() )
    {
    	cout << *sIt++ << " ";
    }
    
    cout << endl;
	
    // copy constructor
    deque<string> d5(d4);
	
    for ( int i=0; i<3; i++ )
    {
	cout << d5[i] << " ";
    }
    
    cout << endl;

    return 0;
}
//OUTPUT:
//  Alex John Robert
//  Alex John Robert
