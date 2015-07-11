// exchange two lists 
#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>

using namespace std;




void print (list<int>& l)
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
    int ary[]={1,2,3,4,5,6,7,8,9,10};
    list<int> li1(ary,ary+5), li2(ary+4,ary+9);
	
    //iota(li1.begin(),li1.end(),1);
    //iota(li2.begin(),li2.end(),5);

    cout << "li1 : ";
    print(li1);
    cout << "li2 : ";
    print(li2);

    li1.swap(li2);
	
    cout << endl <<"After swapping:" << endl;
    cout << "li1 : ";
    print(li1);
    cout << "li2 : ";
    print(li2);

    return 0;
}
//OUTPUT:
// li1 : 1 2 3 4 5 
// li2 : 5 6 7 8 9 
//
// After swapping:
// li1 : 5 6 7 8 9 
// li2 : 1 2 3 4 5 
