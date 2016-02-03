// returns the first element
#include <iostream>
#include <list>

using namespace std;




int main () 
{
	list<int> li(10);
    int ary[] = {1,2,3,4,5};

	list<int>::iterator It;

	cout<<"push_front:"<<endl;
    for ( int i=0; i<5; i++ )
    {
        li.push_front(ary[i]);	
        cout <<li.front() <<"\t";
    }

	It= li.begin();
	cout<<endl<<"It++: "<<endl;
	for ( int i=0; i<5; i++ )
    {
		cout<<*It++<<'\t';
    }

	cout<<endl<<"pop_front:"<<endl;
    for ( int i=0; i<5; i++ )
    {
		cout<<li.front()<<'\t';
		li.pop_front();
    }

	


	cout<<endl;

    return 0;
}
//OUTPUT:
// front() : 1
// front() : 2
// front() : 3
// front() : 4
// front() : 5
