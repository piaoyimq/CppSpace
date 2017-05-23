#include <iostream>
#include <iomanip>
#include <deque>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;




class ID
{
    friend bool operator < ( const ID&, const ID& );
        public:
        ID(string name,int score) : name(name), score(score) {}
        void display ()
        {
            cout.setf(ios::left);
            cout << setw(3) << score << name << endl;
        }
    private:
        string name; int score;
};

// comperation function for sorting
bool operator < ( const ID& a, const ID& b )
{
    return a.score < b.score;
}

typedef deque<ID> Deque; // new name for existing datatype




int main () 
{
    Deque d;
    Deque::iterator Iter;
	
    d.push_back(ID("Smith A",96));
    d.push_back(ID("Amstrong B.",91));
    d.push_back(ID("Watson D.",82));
	
    for ( Iter = d.begin(); Iter != d.end(); Iter++ )
    {
     	Iter->display();
    }
	
    sort(d.begin(),d.end()); // sort algorithm 按升序(ASC)排序
    
    cout << endl << "Sorted by Score" << endl;
    cout << "===============" << endl;
	
    for ( Iter = d.begin(); Iter != d.end(); Iter++ )
    {
     	Iter->display();
    }
    
    cout << endl << "Reverse output" << endl;
    cout << "===============" << endl;
	
    Deque::reverse_iterator r = d.rbegin();	
	//传回一个逆向队列的第一个元素的地址
    while ( r != d.rend() )//传回一个逆向队列的最后一个元素的下一个地址
    {
     	r++->display();
    }
    
    cout << endl;
	
    return 0;
}
//OUTPUT:
// 96 Smith A.
// 91 Amstrong B.
// 82 Watson D.
//
// Sorted by Score
// ===============
// 82 Watson D.
// 91 Amstrong B.
// 96 Smith A.
//
// Reverse output
// ===============
// 96 Smith A.
// 91 Amstrong B.
// 82 Watson D.
