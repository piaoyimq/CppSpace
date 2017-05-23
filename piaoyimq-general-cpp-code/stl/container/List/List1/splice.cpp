// merge two lists 
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;
/*
list::splice实现list拼接的功能。将源list的内容部分或全部元素删除，拼插入到目的list。

函数有以下三种声明：

void splice ( iterator position, list<T,Allocator>& x );  // 

void splice ( iterator position, list<T,Allocator>& x, iterator i );

void splice ( iterator position, list<T,Allocator>& x, iterator first, iterator last );

函数说明：在list间移动元素：

将x的元素移动到目的list的指定位置，高效的将他们插入到目的list并从x中删除。

目的list的大小会增加，增加的大小为插入元素的大小。x的大小相应的会减少同样的大小。

前两个函数不会涉及到元素的创建或销毁。第三个函数会。

指向被删除元素的迭代器会失效。

参数：

position

目的list的位置，用来标明 插入位置

x

源list、

first,last

x里需要被移动的元素的迭代器。区间为[first, last).

包含first指向的元素，不包含last指向的元素。



*/



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
    list<int> li1, li2, li3, li4;
    Print<int> print;

    for ( int i=0; i<5; i++ )
    {
        li1.push_back(i);
        li2.push_back(i+5);
        li3.push_back(i+10);
        li4.push_back(i+15);
    }

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl;

    cout << "li2 : ";
    for_each(li2.begin(),li2.end(),print);
    cout << endl;

    cout << "li3 : ";
    for_each(li3.begin(),li3.end(),print);
    cout << endl;

    cout << "li4 : ";
    for_each(li4.begin(),li4.end(),print);
    cout << endl << endl;

    li1.splice(li1.end(),li2);//把li2中的元素加到li1的末尾

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl << endl;

    li1.splice(li1.end(),li3,li3.begin(),li3.end());

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl << endl;

    list<int>::iterator It;
    It = find(li4.begin(),li4.end(),18);

    li1.splice(li1.begin(),li4,It);//在begin位置插入18这个元素
    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl;

    cout << "li4 : ";
    for_each(li4.begin(),li4.end(),print);
    cout << endl;

    return 0;
}
//OUTPUT:
// li1 : 0 1 2 3 4 
// li2 : 5 6 7 8 9 
// li3 : 10 11 12 13 14 
// li4 : 15 16 17 18 19 
// 
// li1 : 0 1 2 3 4 5 6 7 8 9 
// 
// li1 : 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 
// 
// li1 : 18 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 
// li4 : 15 16 17 19 
