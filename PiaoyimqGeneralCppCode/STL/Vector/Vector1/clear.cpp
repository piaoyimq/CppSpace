#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;




template <class T>//ģ����
class Print
{
    public:
        void operator () (T& t)//zhuweibo ???
        {
            cout<<t<<" ";
        }
};




 
int main ()
{
    vector<int> v(10);
	
    Print<int> print;
	
    fill(v.begin(),v.end(),5);//��һ����ֵ�������Ԫ��
	
    cout<<"Vector v : ";
	
    for_each(v.begin(),v.end(),print);
	//for_each�㷨��v.begin()��v.end()��ָ���ķ�Χ�ڵ�ÿ��Ԫ��Ӧ�ú���print

    cout<<endl<<"Size of v = "<<v.size()<<endl;

    cout<<"v.clear"<<endl;
    v.clear();//�Ƴ���������������
	
    cout<<"Vector v : ";
	
    for_each(v.begin(),v.end(),print);
	
    cout<<endl<<"Size of v = "<<v.size()<<endl;

	cout<<"Vector v is ";
	
    v.empty()?cout<<"":cout<<"not";//�ж������Ƿ�Ϊ�ա�
	
    cout<<"empty"<<endl;

    return 0;
}
// Vector v : 5 5 5 5 5 5 5 5 5 5
// Size of v = 10
// v.clear
// Vector v :
// Size of v = 0
// Vector v is empty
