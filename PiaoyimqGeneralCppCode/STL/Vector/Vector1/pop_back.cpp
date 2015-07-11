#include <iostream>
#include <vector>
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
    vector<int> v;
    Print<int> print;
	
    for ( int i=0; i<5; i++ )
    {
		v.push_back(i+1);//在尾部加入一个数据。
	}
    
    while ( !v.empty() )
    {
    
		cout<<"size: "<<v.size()<<"     ";
        for_each(v.begin(),v.end(),print);

    	cout<<endl; 
     
        v.pop_back();//删除最后一个数据
    }

	cout<<"size: "<<v.size()<<endl;
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 1 2 3 4
// 1 2 3
// 1 2
// 1
