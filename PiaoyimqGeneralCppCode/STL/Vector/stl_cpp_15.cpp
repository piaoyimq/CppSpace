//stl_cpp_15.cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(void)
{
    int a[10]={12,31,5,2,23,121,0,89,34,66};
    vector<int> v1(a,a+10);
    vector<int>::iterator result1,result2;//result1和result2是随机访问迭代器
    result1=find(v1.begin(),v1.end(),2);
    //在v1中找到2，result1指向v1中的2 
    result2=find(v1.begin(),v1.end(),8);
    //在v1中没有找到8，result2指向的是v1.end() 
    cout<<result1-v1.begin()<<endl; //3－0＝3或4－1＝3，屏幕结果是3
    cout<<result2-v1.end()<<endl;    
    int b[9]={5,2,23,54,5,5,5,2,2};
    vector<int> v2(a+2,a+8);
    vector<int> v3(b,b+4);
    result1=search(v1.begin(),v1.end(),v2.begin(),v2.end());
    cout<<*result1<<endl; 
    //在v1中找到了序列v2，result1指向v2在v1中开始的位置 
     result1=search(v1.begin(),v1.end(),v3.begin(),v3.end());
     cout<<*(result1-1)<<endl;
    //在v1中没有找到序列v3，result指向v1.end(),屏幕打印出v1的最后一个元素66    
     vector<int> v4(b,b+9); 
     int i=count(v4.begin(),v4.end(),5);
     int j=count(v4.begin(),v4.end(),2);
     cout<<"there are "<<i<<" members in v4 equel to 5"<<endl;
     cout<<"there are "<<j<<" members in v4 equel to 2"<<endl; 
     //计算v4中有多少个成员等于 5,2
    cin.get();
    return 0;        
}
