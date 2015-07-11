#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>

using namespace std;




int main ()
{
    deque<int> d(5);
	
    for ( int i=0; i<5; i++ )
    {
	d[i] = i*2;
    }
        
    copy(d.begin(),d.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    d.resize(7,100);//增加到7个元素，新元素用100初始化
    copy(d.begin(),d.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    d.resize(4);//缩小到4个元素
    copy(d.begin(),d.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    return 0;
}
//OUTPUT:
// 0 2 4 6 8
// 0 2 4 6 8 100 100
// 0 2 4 6
