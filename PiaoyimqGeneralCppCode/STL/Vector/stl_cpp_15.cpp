//stl_cpp_15.cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(void)
{
    int a[10]={12,31,5,2,23,121,0,89,34,66};
    vector<int> v1(a,a+10);
    vector<int>::iterator result1,result2;//result1��result2��������ʵ�����
    result1=find(v1.begin(),v1.end(),2);
    //��v1���ҵ�2��result1ָ��v1�е�2 
    result2=find(v1.begin(),v1.end(),8);
    //��v1��û���ҵ�8��result2ָ�����v1.end() 
    cout<<result1-v1.begin()<<endl; //3��0��3��4��1��3����Ļ�����3
    cout<<result2-v1.end()<<endl;    
    int b[9]={5,2,23,54,5,5,5,2,2};
    vector<int> v2(a+2,a+8);
    vector<int> v3(b,b+4);
    result1=search(v1.begin(),v1.end(),v2.begin(),v2.end());
    cout<<*result1<<endl; 
    //��v1���ҵ�������v2��result1ָ��v2��v1�п�ʼ��λ�� 
     result1=search(v1.begin(),v1.end(),v3.begin(),v3.end());
     cout<<*(result1-1)<<endl;
    //��v1��û���ҵ�����v3��resultָ��v1.end(),��Ļ��ӡ��v1�����һ��Ԫ��66    
     vector<int> v4(b,b+9); 
     int i=count(v4.begin(),v4.end(),5);
     int j=count(v4.begin(),v4.end(),2);
     cout<<"there are "<<i<<" members in v4 equel to 5"<<endl;
     cout<<"there are "<<j<<" members in v4 equel to 2"<<endl; 
     //����v4���ж��ٸ���Ա���� 5,2
    cin.get();
    return 0;        
}
