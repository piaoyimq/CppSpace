#include <iostream>
#include <vector>
using namespace std;


int main ()
{
#if 0
    vector<int> v(10);
	
    cout << "Size     of v = "<<v.size()<< endl;
    cout << "Capacity of v = "<<v.capacity()<< endl;
	
    v.resize(100);
    cout << "After resizing:" << endl;
	
    cout << "Size     of v = "<< v.size()<< endl;
    cout << "Capacity of v = "<< v.capacity() << endl;
	
    return 0;

#else
	
	vector<int> v(5,0); // 5 elements, each - value 0

    cout << "Size of v  = " << v.size() << endl;
    cout << "Capacity v = " << v.capacity() << endl;
    cout << "Value of each element is - ";
    cout << "sizeof v   = " << sizeof(v) << endl;// how big is vector itself
    
    for ( int i = 0; i < v.size(); i++ )
    {
		cout << v[i] << "  ";
	}

    cout << endl;
	
    v[0] = 5;       // new value for first element
    v[1] = 8;

	//	push_back在尾部加入一个数据。
 	v.push_back(3); // creates new (6th) element of vector,
    v.push_back(7); // automatically increases size  

    cout << endl;   // capacity of vector v
    cout << "Size of v  = " << v.size() << endl;//size返回容器中实际数据的个数。
    cout << "Capacity v = " << v.capacity() << endl;
    cout << "sizeof v   = " << sizeof(v) << endl;// how big is vector itself
	//capacity返回容器中数据个数,容积，在之前的容积扩大一倍
	
    cout << "Value of each element is - ";
	
    for( int i = 0; i < v.size(); i++ )
    {
		cout << v[i] << "  ";
	}
	
    cout << endl << endl;

    v.reserve(100); // increase capacity to 100
    cout << "Size of v1_int  = " << v.size() << endl;
    cout << "Capacity v1_int = " << v.capacity() << endl;
    cout << "sizeof v   = " << sizeof(v) << endl;// how big is vector itself
	 
    return 0;

#endif
}
//OUTPUT 1:
// Size of v = 10
// Capacity of v = 10
// After resizing:
// Size of v = 100
// Capacity of v = 100


//OUTPUT 2:
// Size of v  = 5
// Capacity v = 5
// Value of each element is - 0  0  0  0  0  
// 
// Size of v  = 7
// Capacity v = 10
// Value of each element is - 5  8  0  0  0  3  7  
// 
// Size of v  = 7
// Capacity v = 100
// sizeof v   = 12

