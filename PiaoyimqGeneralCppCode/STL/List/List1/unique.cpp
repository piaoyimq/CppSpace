// removes duplicate elements
#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;




template <class T>
class Member
{
    public:
        Member(T f, T l) :first_n(f), last_n(l) {}
        void print();
    private:
        string last_n, first_n;
    // for sort function
    friend bool operator < (Member& m1,Member& m2)
    { return m1.last_n < m2.last_n; }

    // for merge and unique functions 
    friend bool operator == (Member& m1,Member& m2)
    { return m1.last_n == m2.last_n; }
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
    list<M> li1;
    li1.push_back(M("Linda","Smith"));
    li1.push_back(M("Robert","Frost"));
    li1.push_back(M("Alex","Amstrong"));

    list<M> li2;
    li2.push_back(M("Linda","Smith"));
    li2.push_back(M("John","Wood"));
    li2.push_back(M("Alex","Amstrong"));

    li1.sort();
    li2.sort();
    li1.merge(li2);
    
    cout << "li1 after sorting and mergin"<< endl;

    list<M>::iterator It = li1.begin();
    while ( It != li1.end() )
    {
        (It++)->print();
    }
    cout << endl;
    
    li1.unique();

    cout << "After li1.unique()" << endl;

    It = li1.begin();
    while ( It != li1.end() )
    {
        (It++)->print();
    }
    cout << endl;
    
    return 0;
}
//OUTPUT:
// li1 after sorting and mergin
// Amstrong       Alex
// Amstrong       Alex
// Frost          Robert
// Smith          Linda
// Smith          Linda
// Wood           John
// 
// After li1.unique()
// Amstrong       Alex
// Frost          Robert
// Smith          Linda
// Wood           John
