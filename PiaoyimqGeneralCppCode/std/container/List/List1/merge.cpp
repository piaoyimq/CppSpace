// merge two lists
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

using namespace std;




int main () 
{
    int ary[] = {2,5,9,7,2,7,6,5};
    list<int> list1(ary,ary+4);
    list<int> list2(ary+4,ary+8);

    cout << "list1 : ";
    copy(list1.begin(),list1.end(),ostream_iterator<int>(cout," "));
	
    cout << endl;

    cout << "list2 : ";
    copy(list2.begin(),list2.end(),ostream_iterator<int>(cout," "));
    cout << endl << endl;

    // you have to sort data before megring it
    list1.sort();//ÅÅÐò
    list2.sort();
    list1.merge(list2);//ºÏ²¢

    cout << "After \"list1.merge(list2)\" :" << endl;
    cout << "list1 : ";
    copy(list1.begin(),list1.end(),ostream_iterator<int>(cout," "));
    cout << endl;

    cout << "size of list2 = " << list2.size()<< endl;
    cout << "list2 is " << (list2.empty() ? "" : "not ")<< "empty" << endl;

    return 0;
}
//OUTPUT:
// list1 : 2 5 9 7 
// list2 : 2 7 6 5 
// 
// After "list1.merge(list2)" :
// list1 : 2 2 5 5 6 7 7 9 
// size of list2 = 0
// list2 is empty
