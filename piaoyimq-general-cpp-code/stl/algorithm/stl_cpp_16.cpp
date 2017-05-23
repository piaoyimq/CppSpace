//stl_cpp_16.cpp
#include <iostream>
#include <algorithm>
using namespace std;
int main(void)
{
    int a[10]={12,0,5,3,6,8,9,34,32,18};
    int b[5]={5,3,6,8,9};
    int d[15];
    sort(a,a+10);
    for(int i=0;i<10;i++)
      cout<<" "<<a[i];
    sort(b,b+5);
    if(includes(a,a+10,b,b+5))
       cout<<'\n'<<"sorted b members are included in a."<<endl;
    else
       cout<<"sorted a dosn`t contain sorted b!";
     merge(a,a+10,b,b+5,d);
    for(int j=0;j<15;j++)
       cout<<" "<<d[j]; 
    cin.get();
    return 0;
}
