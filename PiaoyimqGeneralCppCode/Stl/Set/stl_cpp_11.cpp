//stl_cpp_11.cpp
#include <iostream>
#include <set>
using namespace std;
int main(void)
{
    set<int> set1;
   for(int i=0; i<10; ++i)
     set1.insert(i);
    for(set<int>::iterator p=set1.begin();p!=set1.end();++p)
      cout<<*p<<"";
     if(set1.insert(3).second)//把3插入到set1中
//插入成功则set1.insert(3).second返回1，否则返回0
//此例中，集中已经有3这个元素了，所以插入将失败
       cout<<"set insert success";
     else
       cout<<"set insert failed"; 
    int a[] = {4, 1, 1, 1, 1, 1, 0, 5, 1, 0};
    multiset<int> A;
A.insert(set1.begin(),set1.end());
    A.insert(a,a+10);
    cout<<endl;
    for(multiset<int>::iterator p=A.begin();p!=A.end();++p)
    cout<<*p<<" "; 
   cin.get();
    return 0;
    }
