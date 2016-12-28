// erase an elemen
#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>

using namespace std;




template <class T>
void print (list<T>& l)
{
    list<int>::iterator It = l.begin();
    while ( It != l.end() )
    {
        cout << *(It++) << " ";
    }
    cout << endl;
}




int main () 
{

    list<int> li(10);
#if 0
    iota(li.begin(),li.end(),1);
#else
	int arry[10]={1,2,3,4,5,6,7,8,9,10};
	li.assign(arry,arry+10);
#endif

    print(li);
	
    list<int>::iterator It;
    It = find(li.begin(),li.end(),6);//�ҵ�������Ԫ��

    // erase at the pos It
    li.erase(It);//������Ԫ��
    print(li);
	
    It = find(li.begin(),li.end(),5);
	
    // erase from beginning to the pos It
    li.erase(li.begin(),It);//[begin,it), ������begin��ʼ����Ԫ��(��������Ԫ��)
    print(li);

    return 0;
}
//OUTPUT:
// 1 2 3 4 5 6 7 8 9 10 
// 1 2 3 4 5 7 8 9 10 
// 4 5 7 8 9 10 

