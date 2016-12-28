#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>

using namespace std;




int main ()
{
    int ary[]={1,2,3,4,5};
    deque<int> d;
	
    // assign to the "d" the contains of "ary"
    d.assign(ary,ary+5);
	
    copy(d.begin(),d.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    // replace d for 3 copies of 100
    d.assign(3,100);//3 //3 the new size of the container,100 is the value to initialize elements of the container with
    copy(d.begin(),d.end(),ostream_iterator<int>(cout," "));
	
    cout << endl;

    return 0;
}

//OUTPUT:
// 1 2 3 4 5
// 100 100 100
