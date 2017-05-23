// add an element to the end of the list
#include <iostream>
#include <list>
#include <iomanip>
#include <string>

using namespace std;




template <class T>
class Name
{
    public:
        Name(T f, T l) : first(f), last(l) {}
        void print()
        {
            cout.setf(ios::left);
            cout << setw(15) << first.c_str()
                 << last << endl;
        }
    private:
        T first, last;
};




int main ()
{
    typedef Name<string> N;
    typedef list<N> L;
	
	
    L l;
    L::iterator It;

    N n1(string("Albert"),string("Johnson"));
    N n2("Lana","Vinokur");

    l.push_back(n1);//从尾部增加一个元素
    l.push_back(n2);

    // unnamed object
    l.push_back(N("Linda","Bain"));

    It = l.begin();

	cout<<"print:  "<<endl;
    while ( It != l.end() )
    {
		(It++)->print();
    }
	
	cout<<endl<<"pop_back:  "<<endl;
	It = l.begin();
	while(It != l.end())
	{
		cout<<"size="<<l.size()<<endl;
		while ( It != l.end() )
		{
			(It++)->print();
		}
		cout<<"pop\n";
		l.pop_back();
	    It = l.begin();
	}

    return 0;
}
//OUTPUT:
// Albert         Johnson
// Lana           Vinokur
// Linda          Bain
