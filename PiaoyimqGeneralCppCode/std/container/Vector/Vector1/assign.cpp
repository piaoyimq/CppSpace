#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main ()
{
    int ary[]={1,2,3,4,5};
	
    vector<int> v;
	
    // assign to the "v" the contains of "ary"
    v.assign(ary,ary+5);//��[ary,ary+5 )�����е����ݸ�ֵ��v��
	
    cout<<"v.size1():"<<v.size()<< endl;
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));
	/*copy�����еĵ�һ��Ԫ�������������,
	�����������[v.begin(),v.end)���Ԫ�����θ��Ƶ����������
	ostream_iterator��,ostream_iterator�����������������������cout���
	���ݣ����������֮����" "�ַ��ָ�*/
	
	
    // replace v for 3 copies of 100
    v.assign(3,100);//��3��100���¸�ֵ��v
	
    cout<<endl<<"v.size2():"<<v.size()<< endl;	
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));

    return 0;
}
//OUTPUT:
// 1 2 3 4 5
// 100 100 100
