// true if the list is empty 
#include <iostream>
#include <list>

using namespace std;



int main ()
{
    list<int> l;

    cout << "List is ";
    l.empty() ? cout << "" : cout << "not ";
    cout << "empty" << endl;

    l.push_back(100);

    cout << "List is ";
    l.empty() ? cout << "" : cout << "not ";
    cout << "empty" << endl;

    return 0;
}
//OUTPUT:
// List is empty
// List is not empty
