//stl_cpp_12.cpp
#include <iostream>
#include <map>
using namespace std;
int main(void)
{
   map<char,int,less<char> > map1;
   map<char,int,less<char> >::iterator mapIter;
    //char �Ǽ������ͣ�int��ֵ������
    //�����ǳ�ʼ��������������
//Ҳ������map1.insert(map<char,int,less<char> >::value_type('c',3)); 
map1['c']=3;
    map1['d']=4; 
    map1['a']=1;
    map1['b']=2; 
    for(mapIter=map1.begin();mapIter!=map1.end();++mapIter)
      cout<<" "<<(*mapIter).first<<": "<<(*mapIter).second;
   //first��Ӧ�����е�char����second��Ӧ�����е�intֵ  
   //������Ӧ��d����ֵ���������ģ�
   map<char,int,less<char> >::const_iterator ptr; 
   ptr=map1.find('d');
   cout<<'\n'<<" "<<(*ptr).first<<" ����Ӧ��ֵ��"<<(*ptr).second; 
   cin.get();
      return 0;
  }
