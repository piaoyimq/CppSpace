#include <iostream>
#include <iomanip>
#include <vector>
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
            cout.setf(ios::left);//setf()设置当前流的格式化标志为flags
            cout << setw(3) << score << name << endl;//setw(x)设置域宽，当大于x不起作用
        }
		
    private:
        string name; int score;
};




// comperation function for sorting
bool operator < ( const ID& a, const ID& b )
{
    return a.score < b.score;
}




typedef vector<ID> Vector; // new name for existing datatype

int main () 
{
    Vector v;
    Vector::iterator Iter;//正向迭代器
	
    v.push_back(ID("Smith A",96));
    v.push_back(ID("Amstrong B.",91));
    v.push_back(ID("Watson D.",82));

    for ( Iter = v.begin(); Iter != v.end(); Iter++ )
    {
		Iter->display();
	}
    
    sort(v.begin(),v.end()); // sort algorithm
    //void sort( RandomIt first, RandomIt last )函数为链表排序，默认是升序。
    
    cout << endl << "Sorted by Score" << endl;
    cout << "===============" << endl;
	
    for ( Iter = v.begin(); Iter != v.end(); Iter++ )
    {
		Iter->display();
	}
    cout << endl << "Reverse output" << endl;
    cout << "===============" << endl;
	
    Vector::reverse_iterator r = v.rbegin();	
	//reverse_iterator逆向迭代器，是元素逆向
	//rbegin()传回一个逆向队列的第一个数据
	//rend()传回一个逆向队列的最后一个数据的下一个位置

	while ( r != v.rend() )
    {
		(r++)->display();//先display,再++
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
