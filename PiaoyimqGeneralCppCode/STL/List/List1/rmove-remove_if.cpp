// removes elements from the list
#include <iostream>
#include <list>
#include <algorithm>
#include <string>

using namespace std;




template <class T, class D>
class Salary
{
    public:
        Salary(T t) : id(t) {}
        Salary(T t,D d) : id(t), sal(d) {}
        void print ()
        { cout << id << "  " << sal << endl; }
    private:
        T id;
        D sal;
    friend bool operator == 
        (const Salary& s1,const Salary& s2)
    { return s1.id == s2.id; }
};





int main () 
{
#if 1
    typedef Salary<string,double> S;
    typedef list<S> L;

    L l;
    l.push_back(S("012345",70000.0));
    l.push_back(S("012346",60000.0));
    l.push_back(S("012347",72000.0));

    L::iterator It = l.begin();
	
    while ( It != l.end() )
    {
		(It++)->print();
	}
    
    cout << endl;

    S s1("012346");
    l.remove(s1);//删除符合条件的元素

    S s2("012345",70000.0);
    l.remove(s2);//删除符合条件的元素
        
    It = l.begin();
    while ( It != l.end() )
    {
		(It++)->print();
	}
    
    cout << endl;
#else
	string str = "Text with some   spaces";

    remove(str.begin(), str.end(), ' ');
    cout << str << '\n';

	str.erase(remove(str.begin(), str.end(), ' '),str.end());
    cout << str << '\n';

#endif

    return 0;
}
//OUTPUT:
// 012345  70000
// 012346  60000
// 012347  72000
// 
// 012346  60000
// 012347  72000
