//stl_cpp_10.cpp
#include <iostream>
#include <string>
#include <list>
using namespace std;
void PrintIt(list<int> n)
{
    for(list<int>::iterator iter=n.begin(); iter!=n.end(); ++iter)
      cout<<*iter<<" ";//�õ������������ѭ�� 
    }
int main(void)
{
    list<int> listn1,listn2;
    //��listn1,listn2��ʼ�� 
    listn1.push_back(123);
    listn1.push_back(0);
    listn1.push_back(34);
    listn1.push_back(1123);
    //now listn1:123,0,34,1123 
    listn2.push_back(100);
    listn2.push_back(12);
    //now listn2:12,100
    listn1.sort();
    listn2.sort();
    //��listn1��listn2����
    //now listn1:0,34,123,1123         listn2:12,100 
    PrintIt(listn1);
    cout<<endl;
    PrintIt(listn2);
    listn1.merge(listn2);
    //�ϲ����������б��,listn1:0��12��34��100��123��1123 
    cout<<endl;
    PrintIt(listn1);
    cin.get();
}
