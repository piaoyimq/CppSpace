// merge two lists 
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;
/*
list::spliceʵ��listƴ�ӵĹ��ܡ���Դlist�����ݲ��ֻ�ȫ��Ԫ��ɾ����ƴ���뵽Ŀ��list��

��������������������

void splice ( iterator position, list<T,Allocator>& x );  // 

void splice ( iterator position, list<T,Allocator>& x, iterator i );

void splice ( iterator position, list<T,Allocator>& x, iterator first, iterator last );

����˵������list���ƶ�Ԫ�أ�

��x��Ԫ���ƶ���Ŀ��list��ָ��λ�ã���Ч�Ľ����ǲ��뵽Ŀ��list����x��ɾ����

Ŀ��list�Ĵ�С�����ӣ����ӵĴ�СΪ����Ԫ�صĴ�С��x�Ĵ�С��Ӧ�Ļ����ͬ���Ĵ�С��

ǰ�������������漰��Ԫ�صĴ��������١������������ᡣ

ָ��ɾ��Ԫ�صĵ�������ʧЧ��

������

position

Ŀ��list��λ�ã��������� ����λ��

x

Դlist��

first,last

x����Ҫ���ƶ���Ԫ�صĵ�����������Ϊ[first, last).

����firstָ���Ԫ�أ�������lastָ���Ԫ�ء�



*/



template <class T>
class Print
{
    public:
        void operator () (T& t)
        {
            cout << t << " ";
        }
};




int main ()
{
    list<int> li1, li2, li3, li4;
    Print<int> print;

    for ( int i=0; i<5; i++ )
    {
        li1.push_back(i);
        li2.push_back(i+5);
        li3.push_back(i+10);
        li4.push_back(i+15);
    }

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl;

    cout << "li2 : ";
    for_each(li2.begin(),li2.end(),print);
    cout << endl;

    cout << "li3 : ";
    for_each(li3.begin(),li3.end(),print);
    cout << endl;

    cout << "li4 : ";
    for_each(li4.begin(),li4.end(),print);
    cout << endl << endl;

    li1.splice(li1.end(),li2);//��li2�е�Ԫ�ؼӵ�li1��ĩβ

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl << endl;

    li1.splice(li1.end(),li3,li3.begin(),li3.end());

    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl << endl;

    list<int>::iterator It;
    It = find(li4.begin(),li4.end(),18);

    li1.splice(li1.begin(),li4,It);//��beginλ�ò���18���Ԫ��
    cout << "li1 : ";
    for_each(li1.begin(),li1.end(),print);
    cout << endl;

    cout << "li4 : ";
    for_each(li4.begin(),li4.end(),print);
    cout << endl;

    return 0;
}
//OUTPUT:
// li1 : 0 1 2 3 4 
// li2 : 5 6 7 8 9 
// li3 : 10 11 12 13 14 
// li4 : 15 16 17 18 19 
// 
// li1 : 0 1 2 3 4 5 6 7 8 9 
// 
// li1 : 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 
// 
// li1 : 18 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 
// li4 : 15 16 17 19 
