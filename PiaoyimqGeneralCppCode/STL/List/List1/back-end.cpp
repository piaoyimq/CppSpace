// returns the last element
#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>

using namespace std;




template<class T, class D>
class Member
{
    public:
        Member(T t, D d) : name(t), sal(d) {}
        void print();
    private:
        T name;
        D sal;
};

template<class T, class D>
void Member<T,D>::print()
{
    cout << name << "  " << sal << endl;
}




int main ()
{
    typedef Member<string,double> M;
    list<M> l;

    l.push_back(M("Robert",60000));
    l.push_back(M("Linda",75000));

    list<M>::iterator It = l.begin();
    cout << "Entire list:" << endl;

    while ( It != l.end() )
    {
		(It++)->print();
	}
    
    cout << endl;

    cout << "Return from back()" << endl;
	
    l.back().print();
	
    return 0;
}

//OUTPUT:
// Entire list:
// Robert  60000
// Linda  75000
//
// Return from back()
// Linda  75000
