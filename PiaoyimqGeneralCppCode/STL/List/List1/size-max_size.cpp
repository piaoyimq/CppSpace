// returns the maximum number of elements the list can hold
#include <iostream>
#include <list>

using namespace std;




int main () 
{
    list<int> li(10);

    cout << "size() of li = "<< li.size() << endl;
    cout << "max_size     = "<< li.max_size() << endl;
	
    return 0;
}
//OUTPUT:
// size() of li = 10
// max_size     = 4294967295
