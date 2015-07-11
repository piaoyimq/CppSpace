#include <iostream>
#include <vector>
#include <string>
#include <iterator>

using namespace std;




template <class T>
class Name
{
    public:
        Name(T t) : name(t) {}
        void print()
        {
            cout << name << " ";
        }
    private:
        T name;
};





int main ()
{
    typedef Name<string> N;
    typedef vector<N> V;
	
    V v;
    N n1("Robert");
    N n2("Alex");
	
    v.push_back(n1);//��β������һ�����ݡ�
    v.push_back(n2);
	
    // unnamed object of the type Name
    v.push_back(N("Linda"));
    V::iterator It = v.begin();
	
    while ( It != v.end() )
    {
	#if 0
		(It++)->print();
	#else
		(*It++).print();//��ִ��*It.print()����++
	#endif
	}
    
    cout << endl;
	
    return 0;
}
//OUTPUT:
// Robert Alex Linda
