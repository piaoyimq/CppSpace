#include<iostream>
#include <cassert>
using namespace std;
//compile with: g++  -std=c++0x -fno-elide-constructors HasPtrMem.cpp


class HasPtrMem
{
public:
	HasPtrMem() :
			d(new int(3))
	{
		cout << "Construct: " << ++n_cstr << endl;
	}

	HasPtrMem(const HasPtrMem& h) :
			d(new int(*h.d))
	{
		cout<<"Copy construct: "<<++n_cptr << endl;
	}

	HasPtrMem(HasPtrMem && h) :
			d(h.d)
	{
		h.d = nullptr;
		cout<<"Move construct: "<<++n_mvtr << endl;
	}

	HasPtrMem& operator = (HasPtrMem&& t)
	{
	    assert(this != &t);

	    this->d = nullptr;
	    this->d = move(t.d);
	    t.d = nullptr;
	    cout<<"Move assign: "<<++n_mva << endl;
	    return *this;
	}

	~HasPtrMem()
	{
		delete d;
		cout << "Destruct: " << ++n_dstr <<endl;
	}

	int *d;

	static int n_cstr;
	static int n_dstr;
	static int n_cptr;
	static int n_mvtr;
	static int n_mva;
};

int HasPtrMem::n_cstr = 0;
int HasPtrMem::n_dstr = 0;
int HasPtrMem::n_cptr = 0;
int HasPtrMem::n_mvtr = 0;
int HasPtrMem::n_mva = 0;

HasPtrMem get_temp()
{
//	HasPtrMem h;
//	cout << "Resource from " <<__FUNCTION__ << ": " << hex << h.d << endl;
//	return h;
	return HasPtrMem();
}

void set_temp(HasPtrMem c)
{
	cout << "Resource from " <<__FUNCTION__ << ": " << hex << c.d << endl;
}

int main()
{
	HasPtrMem a = get_temp();
	cout << "Resource from " << __FUNCTION__ << ": " << hex << a.d << endl;

	HasPtrMem b ;
	b = get_temp();

	set_temp(a);
}

