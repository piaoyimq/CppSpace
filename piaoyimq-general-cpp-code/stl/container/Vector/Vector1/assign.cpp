#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main ()
{
    int ary[]={1,2,3,4,5};
	
    vector<int> v;
	
    // assign to the "v" the contains of "ary"
    v.assign(ary,ary+5);//将[ary,ary+5 )区间中的数据赋值给v。
	
    cout<<"v.size1():"<<v.size()<< endl;
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	/*copy从序列的第一个元素其进行正向复制,
	把输入迭代器[v.begin(),v.end)间的元素依次复制到输出迭代器
	ostream_iterator中,ostream_iterator创建流输出迭代器，用来向cout输出
	数据，输出的数据之间用" "字符分隔*/
	
	
    // replace v for 3 copies of 100
    v.assign(3,100);//将3个100重新赋值给v
	
    cout<<endl<<"v.size2():"<<v.size()<< endl;	
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));

    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 100 100 100
