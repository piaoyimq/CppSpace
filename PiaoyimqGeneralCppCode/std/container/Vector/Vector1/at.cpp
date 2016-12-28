#include <iostream>
#include <vector>
using namespace std;

int main ()
{
#if 1
    vector<int> v(3,99);
	
    v[0] = 100;
    v.at(1) = 200;//��������idx��ָ�����ݣ����idxԽ�磬�׳�out_of_range
	
    for ( int i=0; i<3; i++ )
    {
		cout << v.at(i) << " ";
	}
	
    cout << endl;
    return 0;
#else
	vector<int> v;

	v.reserve(10);//�����ʵ�������

	for(int i=0; i<7; i++) 
	{
		v.push_back(i);//push_back��β������һ������
	}

	try 
	{
	#if 0
		int iVal1 = v[7];
		// not bounds checked - will not throw
		cout<<"iVal1: "<<iVal1<<endl;
	#else
		int iVal2 = v.at(7);
	
		cout<<"iVal2: "<<iVal2<<endl;
		// bounds checked - will throw if out of range,���Ҫ������at���ã�������ʽ�ķ���
	#endif
	} 
	catch(const exception& e) 
	{
		cout << e.what();
	}

	return 0;

#endif
}

//OUTPUT:
// 100 200 99 


