// sorts the list
#include <iostream>
#include <list>
#include <algorithm>
#include <functional>
#include <string>
#include <iomanip>



using namespace std;



#if 0
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
    int ary[] = {3,2,5,7,3,6,7,2,4,5};
    list<int> li(ary,ary+10);
    Print<int> print;

    cout << "Before sorting\nli : ";
    for_each(li.begin(),li.end(),print);
    cout << endl << endl;

    li.sort(greater<int>());//升序ASC

    cout << "After li.sort(greater())\nli : ";
    for_each(li.begin(),li.end(),print);
    cout << endl << endl;

    li.sort(less<int>());//降序DSC

    cout << "After li.sort(less())\nli : ";
    for_each(li.begin(),li.end(),print);
    cout << endl;
	
    li.sort();//默认ASC
    //sort(li.begin(),li.end()); // 不可以用这种方法


    cout << "After li.sort()\nli : ";
    for_each(li.begin(),li.end(),print);
    cout << endl;

    return 0;
}
//OUTPUT:
// Before sorting
// li : 3 2 5 7 3 6 7 2 4 5 
// 
// After li.sort(greater<int>())
// li : 7 7 6 5 5 4 3 3 2 2 
// 
// After li.sort(less<int>())
// li : 2 2 3 3 4 5 5 6 7 7 

#else

template <class T>
class Member
{
    public:
        Member(T f, T l) :first_n(f), last_n(l) {}
        void print();
    private:
        string last_n, first_n;
    // for sort() list member function
#if 1    
    friend bool operator < (Member& m1,Member& m2)
    { return m1.last_n < m2.last_n; }
#else
    friend bool operator < (Member& m1,Member& m2)
    { return m1.first_n < m2.first_n; }
#endif
};




template <class T>
void Member<T>::print()
{
    cout.setf(ios::left);
    cout << setw(15) << last_n.c_str()<< first_n << endl;
}

typedef Member<string> M;




int main () 
{
    list<M> li;
    li.push_back(M("BLinda","Cmith"));
    li.push_back(M("Frost","Bobert"));
    li.push_back(M("Clex","Amstrong"));

    cout << "Before sorting by last name:\n"<< "============================"<< endl;

    list<M>::iterator It = li.begin();
    while ( It != li.end() )
    {
        (It++)->print();
    }
    cout << endl;

    li.sort();

    cout << "After sorting by last name:\n"<< "============================"<< endl;

    It = li.begin();
    while ( It != li.end() )
    {
        (It++)->print();
    }

	return 0;
}
//OUTPUT:
// Before sorting by last name:
// ============================
// Smith          Linda
// Robert         Frost
// Amstrong       Alex
// 
// After sorting by last name:
// ============================
// Amstrong       Alex
// Robert         Frost
// Smith          Linda
#endif
