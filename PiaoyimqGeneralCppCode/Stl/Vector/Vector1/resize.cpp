#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;



int main ()
{
    vector<int> v(5);
	
    for ( int i=0; i<5; i++ )
    {
		v[i] = i*2;
	}
    
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	
    cout << endl;
    v.resize(7,100);//7为元素的新大小，100为新元素的初始值
	
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    v.resize(4);//调整容器大小为4,把容器末端多出来的元素去掉。
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
    cout << endl;
	
    return 0;
}

//OUTPUT:
// 0 2 4 6 8
// 0 2 4 6 8 100 100
// 0 2 4 6
