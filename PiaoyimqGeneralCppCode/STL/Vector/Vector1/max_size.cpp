#include <iostream>
#include <vector>

using namespace std;




int main ()
{
    vector<int> v(10);
	
    cout << "Size of v     = "<< v.size() << endl;//����������ʵ�ʴ�����ݵ���Ŀ

    cout << "Capacity of v     = "<< v.capacity() << endl;//���������ݻ�
	
    cout << "Max_size of v = "<< v.max_size() << endl;//����������������ݵ�����
	
    return 0;
}

//OUTPUT:
// Size of v     = 10
// Max_size of v = 1073741823
