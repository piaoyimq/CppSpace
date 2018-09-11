#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

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
    vector<char> v(5);
    Print<char> print;
	
    cout << "Size of v = " << v.size() << endl;
	
    fill(v.begin(),v.end(),'*');
	
    for_each(v.begin(),v.end(),print);
    cout << endl;
	
    for ( int i=0; i < v.size(); i++ )
    {
		cout << v[i] << " ";
	}
    
    cout << endl;
	
    for ( int i=0; i<5; i++ )
    {
        cout << "Size of v = ";
        for_each(v.begin(),v.end(),print);
        cout << endl;
        v.pop_back();//删除最后一个数据
    }
	
    return 0;
}


//OUTPUT:
// Size of v = 5
// * * * * *
// * * * * *
// Size of v = * * * * *
// Size of v = * * * *
// Size of v = * * *
// Size of v = * *
// Size of v = *

