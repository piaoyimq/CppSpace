#include <iostream>
#include <deque>

using namespace std;




int main ()
{
    deque<int> d(10);
	
    cout << "Size of d     = "<< d.size() << endl;//����������ǰԪ�صĸ���

 //   cout << "Max_size of d = "<< d.capacity() << endl;	
 //ע��:deque��û��capacity()����
	
    cout << "Max_size of d = "<< d.max_size() << endl;
//�����������Ԫ�صĸ���



    return 0;
}
//OUTPUT:
// Size of d     = 10
// Max_size of d = 1073741823
