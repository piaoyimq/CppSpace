#include <iostream>
#include <vector>
using namespace std;

int main ()
{
#if 1
    vector<int> v(3,99);
	
    v[0] = 100;
    v.at(1) = 200;//传回索引idx所指的数据，如果idx越界，抛出out_of_range
	
    for ( int i=0; i<3; i++ )
    {
		cout << v.at(i) << " ";
	}
	
    cout << endl;
    return 0;
#else
	vector<int> v;

	v.reserve(10);//保留适当的容量

	for(int i=0; i<7; i++) 
	{
		v.push_back(i);//push_back在尾部加入一个数据
	}

	try 
	{
	#if 0
		int iVal1 = v[7];
		// not bounds checked - will not throw
		cout<<"iVal1: "<<iVal1<<endl;
	#else
		int iVal2 = v.at(7);
	
		cout<<"iVal2: "<<iVal2<<endl;
		// bounds checked - will throw if out of range,因此要尽量用at不用，数组型式的访问
	#endif
	} 
	catch(const exception& e) 
	{
		cout << e.what();
	}

	return 0;

#endif
}

//OUTPUT:
// 100 200 99 


