#include <iostream>
#include <deque>
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
    deque<int> d;
    Print<int> print;

    cout<<"push_back:"<<endl;
    for ( int i=0; i<5; i++ )
    {
	    d.push_back(i+1);//在尾部加入一个数据
    }
	
    for_each(d.begin(),d.end(),print);
    cout << endl;

    cout<<"pop_back:"<<endl;	
    while ( !d.empty() )
    {
        for_each(d.begin(),d.end(),print);
        cout << endl;
        d.pop_back();//删除最后一个数据
    }

    cout<<"d.size()"<<d.size()<<endl;
	
    cout<<"push_front:"<<endl;
    for ( int i=0; i<5; i++ )
    {
	    d.push_front(i+1);//在尾部加入一个数据
    }
    for_each(d.begin(),d.end(),print);
    cout << endl;

    cout<<"pop_front:"<<endl;	
    while ( !d.empty() )
    {
        for_each(d.begin(),d.end(),print);
        cout << endl;
        d.pop_front();
    }

    

	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 1 2 3 4
// 1 2 3
// 1 2
// 1
