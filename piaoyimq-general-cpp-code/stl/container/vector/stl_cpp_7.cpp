//stl_cpp_7.cpp
//���򣺳�ʼ����ʾ
#include <cstring>  
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;
int ar[10] = {  12, 45, 234, 64, 12, 35, 63, 23, 12, 55  };
char* str = "Hello World";
int main(void)
{
  vector <int> vec1(ar, ar+10);          //first=ar,last=ar+10,������ar+10
  vector <char> vec2(str, str+strlen(str));  //first=str,last= str+strlen(str),���������һ��
  cout<<"vec1:"<<endl;   
//��ӡvec1��vec2��const_iterator�ǵ�����������ὲ��
//��Ȼ��Ҳ������for (int i=0; i<vec1.size(); i++)cout << vec[i];���
//size()��vector��һ����Ա����
  for(vector<int>::const_iterator p=vec1.begin();p!=vec1.end(); ++p)
     cout<<*p;
  cout<<'\n'<<"vec2:"<<endl;
  for(vector<char>::const_iterator p1=vec2.begin();p1!=vec2.end(); ++p1)
      cout<<*p1;
  getchar();
  return 0;
}  
