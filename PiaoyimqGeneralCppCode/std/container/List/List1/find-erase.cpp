// erase an elemen
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

    list<int> li(10);
#if 0
    iota(li.begin(),li.end(),1);
#else
	int arry[10]={1,2,3,4,5,6,7,8,9,10};
	li.assign(arry,arry+10);
#endif

    print(li);
	
    list<int>::iterator It;
    It = find(li.begin(),li.end(),6);//找到第六个元素

    // erase at the pos It
    li.erase(It);//擦除该元素
    print(li);
	
    It = find(li.begin(),li.end(),5);
	
    // erase from beginning to the pos It
    li.erase(li.begin(),It);//[begin,it), 擦除从begin开始到该元素(不包括该元素)
    print(li);

    return 0;
}
//OUTPUT:
// 1 2 3 4 5 6 7 8 9 10 
// 1 2 3 4 5 7 8 9 10 
// 4 5 7 8 9 10 

