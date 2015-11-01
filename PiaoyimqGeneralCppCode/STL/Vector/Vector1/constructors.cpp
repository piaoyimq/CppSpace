#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int main ()
{
    string str[]={"Alex","John","Robert"};
   
    // empty vector object
    vector<int> v1;
    
    // creates vector with 10 empty elements
    vector<int> v2(10);
    
    // creates vector with 10 elements,and assign value 0 for each
    vector<int> v3(10,0);
    
    // creates vector and assigns values from string array
    vector<string> v4(str+0,str+3);
    
    vector<string>::iterator sIt=v4.begin();//begin���ص������е�һ������ ��ַ
    	
    while(sIt != v4.end())//ָ���������ĩ��Ԫ�ص���һ����ָ��һ��������Ԫ��
    {
		cout<<*sIt++<<" ";
	}
    
	
    cout << endl;
	
    // copy constructor
    vector<string> v5(v4);
	cout<<"v5.size():"<<v5.size()<<endl;//����������ʵ�����ݵĸ�����
    for ( int i=0; i<v5.size(); i++ )
    {
		cout << v5[i] << " ";
	}
    
    cout << endl;
    return 0;
}

//OUTPUT:
//  Alex John Robert
//  Alex John Robert
