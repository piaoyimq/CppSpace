#include <iostream>
#include <deque>
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
    deque<int> d(10);
    Print<int> print;

#if 0
    fill(d.begin(),d.end(),5);
#else
    for(int i=0;i<10;i++)
    {
	d[i]=i+1;
    }
#endif
	
	
    cout << "Deque d : ";
    for_each(d.begin(),d.end(),print);
    cout << endl;
	
    cout << "Size of d = " << d.size()<< endl;

    deque<int>::iterator It = d.begin()+2;

/*erase*/	
    cout << "erase third element" << endl;	
    d.erase(It);//从容器中清除一个或几个元素

    cout << "Deque d : ";
    for_each(d.begin(),d.end(),print);
    cout << endl;
    cout << "Size of d = " << d.size()<< endl;

/*clear*/
    cout << "d.clear" << endl;	
    d.clear();//清除容器中所有元素

    cout << "Deque d : ";
    for_each(d.begin(),d.end(),print);
    cout << endl;
	
    cout << "Size of d = " << d.size()<< endl;
    cout << "Deque d is ";
	
    d.empty() ? cout << "" : cout << "not ";
    cout << "empty" << endl;
	
    return 0;
}
/*
OUT:
Deque d : 1 2 3 4 5 6 7 8 9 10 
Size of d = 10
erase third element
Deque d : 1 2 4 5 6 7 8 9 10 
Size of d = 9
d.clear
Deque d : 
Size of d = 0
Deque d is empty

*/

