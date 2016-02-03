#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>

using namespace std;




int main ()
{
    vector<int> v(5);
	int i;

#if 0
    iota(v.begin(),v.end(),1);
#else
	for(i=0;i<5;i++)
	{
		v.at(i)=i+1;
	}
#endif

	vector<int>::iterator It = v.begin();

/*�Ա�׼����������������ַ�ʽ,����ʹ��
whileѭ���������ÿһ��Ԫ�أ�Ҳ����ʹ��copy�������*/

#if 0	
    while ( It != v.end() )
    {
		cout << *It++ << " ";
	}
#else
	copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
#endif
	
    cout << endl;
	
    // last element of the vector
    It = v.end()-1;
	//ָ���������ĩ��Ԫ�ص���һ����ָ��һ��������Ԫ��
    cout << *It << endl;
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 5
