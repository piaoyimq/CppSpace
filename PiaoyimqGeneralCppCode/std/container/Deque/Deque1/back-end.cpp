#include <iostream>
#include <deque>
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
//void Member::print()
void Member<T,D>::print()
{
    cout << name << "  " << sal << endl;
}




int main ()
{
    typedef Member<string,double> M;
    deque<M> d;
	
    d.push_back(M("Robert",60000));
    d.push_back(M("Linda",75000));
	
    deque<M>::iterator It = d.begin();
    cout << "Entire deque:" << endl;
	
    while ( It != d.end() )
    {
    	(It++)->print();
    }
    
    cout << endl;
    cout << "Return from back()" << endl;
	
    d.back().print();//back()返回容器中最后一个元素
    (d.end()-1)->print();//end()是返回容器中最后一个元素的下一个地址
	
    return 0;
}
//OUTPUT:
// Entire deque:
// Robert  60000
// Linda  75000
//
// Return from back()
// Linda  75000
