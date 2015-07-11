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
	/*
		iota算法用来创建递增序列，它先把初值给
		*first,然后用前置++操作符增长初值并赋值到下一个
		迭代器指向的元素，如
		#include <numeric>
		int a[5]={0};
		char c[3]={0};
		iota(a,a+5,10);//changes a to{10,11,12,13,14}
		iota(c,c+3,'a');//{'a','b','c'}
	*/
#else//由于iota式c++11的标准库，部分编译器暂不支持，此处用else代替其功能
	for(i=0;i<5;i++)
	{
		v.at(i)=i+1;
	}
	
#endif
	
    vector<int>::iterator It = v.begin();//begin传回迭代器中的第一个数据地址
	
    while(It != v.end())
    {
		cout << *It++ << " ";
	}
    
    cout << endl;
    // third element of the vector
    
    It = v.begin()+2;
    cout << *It <<"  "<<*(It+1)<<"  "<<*(It+2)<<endl;
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 3

