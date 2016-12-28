#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;




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
    int ary[] = {1,2,3,4,5,6,7,8,9,10};
    Print<int> print;
	
    vector<int> v1(ary,ary+7);
    vector<int> v2(ary+7,ary+10);
	
    cout << "Vector v1 : ";
    for_each(v1.begin(),v1.end(),print);
    cout << endl;
    cout << "Size of v1 = " << v1.size()<< endl << endl;

    cout << "Vector v2 : ";
    for_each(v2.begin(),v2.end(),print);
    cout << endl;
    cout << "Size of v2 = " << v2.size()<< endl << endl;
	
    v1.swap(v2);
    cout << "After swapping:" << endl;
    cout << "Vector v1 : ";
    for_each(v1.begin(),v1.end(),print);
    cout << endl;
	cout << "Size of v1 = " << v1.size()<< endl << endl;
		 
    cout << "Vector v2 : ";
    for_each(v2.begin(),v2.end(),print);
    cout << endl;
    cout << "Size of v2 = " << v2.size()<< endl << endl;
	
    return 0;
}


//OUTPUT:
// Vector v1 : 1 2 3 4 5 6 7
// Size of v1 = 7
//
// Vector v2 : 8 9 10
// Size of v2 = 3
//
// After swapping:
// Vector v1 : 8 9 10
// Size of v1 = 3
//
// Vector v2 : 1 2 3 4 5 6 7
// Size of v2 = 7

