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
	/*
		iota�㷨���������������У����Ȱѳ�ֵ��
		*first,Ȼ����ǰ��++������������ֵ����ֵ����һ��
		������ָ���Ԫ�أ���
		#include <numeric>
		int a[5]={0};
		char c[3]={0};
		iota(a,a+5,10);//changes a to{10,11,12,13,14}
		iota(c,c+3,'a');//{'a','b','c'}
	*/
#else//����iotaʽc++11�ı�׼�⣬���ֱ������ݲ�֧�֣��˴���else�����书��
	for(i=0;i<5;i++)
	{
		v.at(i)=i+1;
	}
	
#endif
	
    vector<int>::iterator It = v.begin();//begin���ص������еĵ�һ�����ݵ�ַ
	
    while(It != v.end())
    {
		cout << *It++ << " ";
	}
    
    cout << endl;
    // third element of the vector
    
    It = v.begin()+2;
    cout << *It <<"  "<<*(It+1)<<"  "<<*(It+2)<<endl;
	
    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 3

