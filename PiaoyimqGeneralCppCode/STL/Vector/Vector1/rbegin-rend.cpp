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
            cout.setf(ios::left);//setf()���õ�ǰ���ĸ�ʽ����־Ϊflags
            cout << setw(3) << score << name << endl;//setw(x)�������������x��������
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
    Vector::iterator Iter;//���������
	
    v.push_back(ID("Smith A",96));
    v.push_back(ID("Amstrong B.",91));
    v.push_back(ID("Watson D.",82));

    for ( Iter = v.begin(); Iter != v.end(); Iter++ )
    {
		Iter->display();
	}
    
    sort(v.begin(),v.end()); // sort algorithm
    //void sort( RandomIt first, RandomIt last )����Ϊ��������Ĭ��������
    
    cout << endl << "Sorted by Score" << endl;
    cout << "===============" << endl;
	
    for ( Iter = v.begin(); Iter != v.end(); Iter++ )
    {
		Iter->display();
	}
    cout << endl << "Reverse output" << endl;
    cout << "===============" << endl;
	
    Vector::reverse_iterator r = v.rbegin();	
	//reverse_iterator�������������Ԫ������
	//rbegin()����һ��������еĵ�һ������
	//rend()����һ��������е����һ�����ݵ���һ��λ��

	while ( r != v.rend() )
    {
		(r++)->display();//��display,��++
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
