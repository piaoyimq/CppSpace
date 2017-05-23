#include <iostream>
#include <vector>

using namespace std;




int main ()
{
    vector<int> v(10);
	
    cout << "Size of v     = "<< v.size() << endl;//返回容器中实际存放数据的数目

    cout << "Capacity of v     = "<< v.capacity() << endl;//返回容器容积
	
    cout << "Max_size of v = "<< v.max_size() << endl;//返回容器中最大数据的数量
	
    return 0;
}

//OUTPUT:
// Size of v     = 10
// Max_size of v = 1073741823
