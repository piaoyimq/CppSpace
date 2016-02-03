//stl_cpp_13.cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;
int main(void)
{
    multimap<string,string,less<string> >mulmap;
    multimap<string,string,less<string> >::iterator p;
    //初始化多重映射mulmap: 
    typedef multimap<string,string,less<string> >::value_type vt;
    typedef string s;
    mulmap.insert(vt(s("Tom "),s("is a student")));
    mulmap.insert(vt(s("Tom "),s("is a boy")));
    mulmap.insert(vt(s("Tom "),s("is a bad boy of blue!")));
    mulmap.insert(vt(s("Jerry "),s("is a student")));
    mulmap.insert(vt(s("Jerry "),s("is a beatutiful girl")));
    mulmap.insert(vt(s("DJ "),s("is a student")));
    //输出初始化以后的多重映射mulmap: 
    for(p=mulmap.begin();p!=mulmap.end();++p)
       cout<<(*p).first<<(*p).second<<endl;
    //检索并输出Jerry键所对应的所有的值
    cout<<"find Jerry :"<<endl;
    p=mulmap.find(s("Jerry "));
    while((*p).first=="Jerry ")
    { 
        cout<<(*p).first<<(*p).second<<endl;
        ++p;
    }    
    cin.get();
    return 0;
    }  
