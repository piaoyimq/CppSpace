//stl_cpp_14.cpp
#include <iostream>
#include <algorithm>
#include <iterator>//�����õ������������ostream_iterator
using namespace std;
int main(void)
{
    int arr[6]={1,12,3,2,1215,90};
	 int arr1[7];
    int arr2[6]={2,5,6,9,0,-56};
    copy(arr,(arr+6),arr1);//������aar���Ƶ�arr1
    cout<<"arr[6] copy to arr1[7],now arr1: "<<endl;
	for(int i=0;i<7;i++)
		cout<<" "<<arr1[i];
   reverse(arr,arr+6);//���ź����arr��ת
   cout<<'\n'<<"arr reversed ,now arr:"<<endl;
	copy(arr,arr+6,ostream_iterator<int>(cout, " "));//���Ƶ����������
swap_ranges(arr,arr+6,arr2);//����arr��arr2����
	cout<<'\n'<<"arr swaped to arr2,now arr:"<<endl;
	copy(arr,arr+6,ostream_iterator<int>(cout, " "));
	cout<<'\n'<<"arr2:"<<endl;
	copy(arr2,arr2+6,ostream_iterator<int>(cout, " "));
	cin.get();
   return 0;
}
