#include<iostream>
using namespace std;

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

//	HasPtrMem(HasPtrMem && h) :
//			d(h.d)
//	{
//		h.d = nullptr;
//		cout<<"Move construct: "<<++n_mvtr << endl;
//	}

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
};

int HasPtrMem::n_cstr = 0;
int HasPtrMem::n_dstr = 0;
int HasPtrMem::n_cptr = 0;
int HasPtrMem::n_mvtr = 0;

HasPtrMem get_temp()
{
//	HasPtrMem h;
//	cout << "Resource from " <<__FUNCTION__ << ": " << hex << h.d << endl;
//	return h;
	return HasPtrMem();
}

int main()
{
	HasPtrMem a = get_temp();
//	HasPtrMem b =a;
	cout << "Resource from " << __FUNCTION__ << ": " << hex << a.d << endl;
}

