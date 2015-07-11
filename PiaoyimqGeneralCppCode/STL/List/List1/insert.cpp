// insert elements into the list
#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>

using namespace std;




template <class T>
void print (list<T>& l)
{
    list<int>::iterator It = l.begin();
    while ( It != l.end() )
    {
        cout << *(It++) << " ";
    }
    cout << endl;
}




int main () 
{
    list<int> li1(10,0);
    list<int> li2(5);
    list<int>::iterator It;

#if 0
	iota(li2.begin(),li2.end(),1);
#else
	int ary[]={1,2,3,4,5};
	li2.assign(ary,ary+5);
#endif

    cout << "li1 : ";
    print(li1);
    cout << "li2 : ";
    print(li2);

    It = li1.begin();
    // value of 20 at the pos It
    li1.insert(++It,20);//在第2个元素位置插入20
    cout << "li1 : ";
    print(li1);
    
    // two value of 25 at the beginning
    li1.insert(li1.begin(),2,25);//插入2个25
    cout << "li1 : ";
    print(li1);

    // contents of li2 at the end of li1
    li1.insert(li1.end(),li2.begin(),li2.end());//在li1.end位置上插入li2从头到尾的值
    cout << "li1 : ";
    print(li1);

    return 0;
}
//OUTPUT:
// li1 : 0 0 0 0 0 0 0 0 0 0 
// li2 : 1 2 3 4 5 
// li1 : 0 20 0 0 0 0 0 0 0 0 0 
// li1 : 25 25 0 20 0 0 0 0 0 0 0 0 0 
// li1 : 25 25 0 20 0 0 0 0 0 0 0 0 0 1 2 3 4 5 
