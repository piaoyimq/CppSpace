//stl_cpp_7.cpp
//程序：初始化演示
#include <cstring>  
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;
int ar[10] = {  12, 45, 234, 64, 12, 35, 63, 23, 12, 55  };
char* str = "Hello World";
int main(void)
{
  vector <int> vec1(ar, ar+10);          //first=ar,last=ar+10,不包括ar+10
  vector <char> vec2(str, str+strlen(str));  //first=str,last= str+strlen(str),不包括最后一个
  cout<<"vec1:"<<endl;   
//打印vec1和vec2，const_iterator是迭代器，后面会讲到
//当然，也可以用for (int i=0; i<vec1.size(); i++)cout << vec[i];输出
//size()是vector的一个成员函数
  for(vector<int>::const_iterator p=vec1.begin();p!=vec1.end(); ++p)
     cout<<*p;
  cout<<'\n'<<"vec2:"<<endl;
  for(vector<char>::const_iterator p1=vec2.begin();p1!=vec2.end(); ++p1)
      cout<<*p1;
  getchar();
  return 0;
}  
