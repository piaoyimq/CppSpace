#include <iostream>
#include <vector>
using namespace std;




int main ()
{
    vector<int> v;
	
    cout << "Vector is ";
    v.empty() ? cout << "" : cout << "not ";
    cout << "empty  " <<v.size()<<endl;
	
    v.push_back(100);//��β������һ������
    cout << "Vector is ";
    v.empty() ? cout << "" : cout << "not ";
    cout << "empty  " <<v.size()<<endl;
    return 0;
}
// Vector is empty
// Vector is not empty

