// reverse the list
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;




int main () 
{
    list<int> l(10);

#if 0
    iota(l.begin(),l.end(),1);
#else
	int ary[]={1,2,3,4,5,6,7,8,9,10};
	l.assign(ary,ary+10);
#endif

    copy(l.begin(),l.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    l.reverse();
    copy(l.begin(),l.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    return 0;
}
//OUTPUT:
// 1 2 3 4 5 6 7 8 9 10 
// 10 9 8 7 6 5 4 3 2 1 
