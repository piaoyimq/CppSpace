#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;




template <class T>
class Print
{
    public:
        void operator () (T& t)//zhuweibo ???
        {
             cout << t << " ";
        }
};




int main ()
{
    int ary[5]={1,2,3,4,5};
    vector<int> v;
    vector<int>::iterator It;
    Print<int> print;

    //fill(ary,ary+5,1);
	
    copy(ary,ary+5,back_inserter(v));
	//back_inserter()是iterator适配器，它使得元素被插入到作为实参的某种容器的尾部，如vector等
    cout << "vector v                : ";
	
    for_each(v.begin(),v.end(),print);
    cout << endl;

    It = v.begin();
    // insert value "5" at the position "It"
    cout << "v.insert(It,5)          : ";
    v.insert(It,5);//在It位置插入一个5，传回新数据位置
	
    for_each(v.begin(),v.end(),print);
    cout << endl;


    // insert range ary+2 - ary+5 at the position "It"
    It = v.begin()+5;
    cout << "v.insert(It,ary+2,ary+5 : ";

	v.insert(It,ary+2,ary+5);
	//在It位置插入在[ary+2,ary+5)区间的数据。无返回值。
	
    for_each(v.begin(),v.end(),print);
    cout << endl;
	
    // insert 2 value of "20" at the position "It"
    It = v.end()-2;
    cout << "v.insert(It,2,20)       : ";
    v.insert(It,2,20);//在It位置插入2个20。无返回值
	
    for_each(v.begin(),v.end(),print);
    cout << endl;

    return 0;
}
//OUTPUT:
//[vector-source-code]# ./a.out 
//vector v                : 1 2 3 4 5 
//v.insert(It,5)          : 5 1 2 3 4 5 
//v.insert(It,ary+2,ary+5 : 5 1 2 3 4 3 4 5 5 
//v.insert(It,2,20)       : 5 1 2 3 4 3 4 20 20 5 5

