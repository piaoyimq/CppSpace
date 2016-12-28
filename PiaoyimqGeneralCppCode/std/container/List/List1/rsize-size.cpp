// change the size of the list
#include <iostream>
#include <list>

using namespace std;




int main () 
{
    list<int> l(10);

    cout << "Size of list l = "<< l.size();

    l.resize(100);
    cout << "After l.resize(100)" << endl;
    cout << "Size of list l = "<< l.size();
	
    l.resize(5);
    cout << "After l.resize(5)" << endl;
    cout << "Size of list l = "<< l.size();

    return 0;
}

//OUTPUT:
// Size of list l = 10After l.resize(100)
// Size of list l = 100After l.resize(5)
// Size of list l = 5
