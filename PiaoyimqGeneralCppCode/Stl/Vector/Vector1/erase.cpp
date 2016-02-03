#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main ()
{
    vector<int> v(10);
    vector<int>::iterator It;
	
    for( int i=0; i<10; i++ )
    {
		v[i] = i+1;
	}
    
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    It = v.begin()+2;
    // remove third element
    v.erase(It);//删除It位置的数据，传回下一个数据的位置
	
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    It = v.begin();
    // remove 2 elements from beginning fo v
    v.erase(It,It+2);//删除[It,It+2)区间的数据，传回下一个数据的位置。
	
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    return 0;
}


//OUTPUT:
// 1 2 3 4 5 6 7 8 9 10
// 1 2 4 5 6 7 8 9 10
// 4 5 6 7 8 9 10
