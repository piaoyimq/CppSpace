// assign a sequence to the list
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
using namespace std;

int main ()
{
    int ary[]={1,2,3,4,5};
    list<int> l;

    // assign to l the contains of ary
    l.assign(ary,ary+5);

    copy(l.begin(),l.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    // replace l for 3 copies of 100
    l.assign(3,100);

    copy(l.begin(),l.end(),ostream_iterator<int>(cout," "));
    cout << endl;


    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 100 100 100
