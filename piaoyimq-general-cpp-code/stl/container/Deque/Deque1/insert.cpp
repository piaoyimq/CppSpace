#include <iostream>
#include <deque>
#include <iterator>
#include <algorithm>

using namespace std;





template <class T>
class Print
{
    public:
        void operator () (T& t)
        {
             cout << t << " ";
        }
};





int main ()
{
    int ary[5]={1,2,3,4,5};
   // fill(ary,ary+5,1);

    deque<int> d;
    deque<int>::iterator It;
    Print<int> print;
	
    copy(ary,ary+5,back_inserter(d));
	/*back_inserter()是iterator适配器，它使得元素被插
	入到作为实参的某种容器的尾部*/
	
    cout << "deque d                : ";
    for_each(d.begin(),d.end(),print);
    cout << endl;
	
    It = d.begin();
    // insert value "5" at the position "It"
    cout << "d.insert(It,5)          : ";
    d.insert(It,5);
    for_each(d.begin(),d.end(),print);
    cout << endl;
	
    // insert range ary+2 - ary+5 at the position "It"
    It = d.begin()+5;
    cout << "d.insert(It,ary+2,ary+5 : ";
    d.insert(It,ary+2,ary+5);
    for_each(d.begin(),d.end(),print);
    cout << endl;
	
    // insert 2 value of "20" at the position "It"
    It = d.end()-2;
    cout << "d.insert(It,2,20)       : ";
    d.insert(It,2,20);
    for_each(d.begin(),d.end(),print);
    cout << endl;

    return 0;
}
//OUTPUT:
// deque d                : 1 1 1 1 1
// d.insert(It,5)          : 5 1 1 1 1 1
// d.insert(It,ary+2,ary+5 : 5 1 1 1 1 1 1 1 1
// d.insert(It,2,20)       : 5 1 1 1 1 1 1 20 20 1 1
