#include <iostream>
#include <deque>
#include <iterator>
#include <numeric>

using namespace std;




int main ()
{
    deque<int> d(5);
#if 0
	iota(d.begin(),d.end(),1);
#else
	for(int i=0;i<5;i++)
	{
		//d[i]=i+1;
		d.at(i)=i+1;
	}
#endif

	
    deque<int>::iterator It = d.begin();
	
    while ( It != d.end() )
    {
	cout << *It++ << " ";
    }
        
    cout << endl;
    // third element of the deque
    It = d.begin()+2;//传回迭代器中的第一个数据地址
    cout << *It << endl;
    cout << *d.begin() << endl;	

    cout << d.front()+7 << endl;	//传回第一个数据。
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5 
// 3
// 1
// 8
