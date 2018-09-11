#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;




template <class T>//模板类
class Print
{
    public:
        void operator () (T& t)//zhuweibo ???
        {
            cout<<t<<" ";
        }
};




 
int main ()
{
    vector<int> v(10);
	
    Print<int> print;
	
    fill(v.begin(),v.end(),5);//用一给定值填充所有元素
	
    cout<<"Vector v : ";
	
    for_each(v.begin(),v.end(),print);
	//for_each算法给v.begin()和v.end()所指定的范围内的每个元素应用函数print

    cout<<endl<<"Size of v = "<<v.size()<<endl;

    cout<<"v.clear"<<endl;
    v.clear();//移除容器中所有数据
	
    cout<<"Vector v : ";
	
    for_each(v.begin(),v.end(),print);
	
    cout<<endl<<"Size of v = "<<v.size()<<endl;

	cout<<"Vector v is ";
	
    v.empty()?cout<<"":cout<<"not";//判断容器是否为空。
	
    cout<<"empty"<<endl;

    return 0;
}
// Vector v : 5 5 5 5 5 5 5 5 5 5
// Size of v = 10
// v.clear
// Vector v :
// Size of v = 0
// Vector v is empty
