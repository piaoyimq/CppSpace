#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>

using namespace std;




int main ()
{
    vector<int> v(5);
	int i;

#if 0
    iota(v.begin(),v.end(),1);
#else
	for(i=0;i<5;i++)
	{
		v.at(i)=i+1;
	}
#endif

	vector<int>::iterator It = v.begin();

/*对标准输出可以有以下两种方式,可以使用
while循环依次输出每一个元素，也可以使用copy函数输出*/

#if 0	
    while ( It != v.end() )
    {
		cout << *It++ << " ";
	}
#else
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
#endif
	
    cout << endl;
	
    // last element of the vector
    It = v.end()-1;
	//指向迭代器中末端元素的下一个，指向一个不存在元素
    cout << *It << endl;
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 5
