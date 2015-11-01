#include <iostream>
#include <deque>

using namespace std;




int main ()
{
    deque<int> d(10);
	
    cout << "Size of d     = "<< d.size() << endl;//返回容器当前元素的个数

 //   cout << "Max_size of d = "<< d.capacity() << endl;	
 //注意:deque中没有capacity()方法
	
    cout << "Max_size of d = "<< d.max_size() << endl;
//返回容器最大元素的个数



    return 0;
}
//OUTPUT:
// Size of d     = 10
// Max_size of d = 1073741823
