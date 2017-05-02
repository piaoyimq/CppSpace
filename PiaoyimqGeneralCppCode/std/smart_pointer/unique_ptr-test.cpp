/*
 *unique_ptr-test.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: piaoyimq
 *      Reference: https://msdn.microsoft.com/zh-cn/library/dn439780.aspx
 */
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/make_unique.hpp>
using namespace std;
using boost::make_unique;

//unique_ptr function:
//1. as function return value.





//make_unique was imported in c++14, if upgrade to c++14, you can instead of std::make_unique.

//template<typename T, typename... Ts>
//std::unique_ptr<T> make_unique(Ts&&... params)
//{
//    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
//}




class Song
{
public:
	Song(wstring a, wstring t) :
			artist(a), title(t)
	{
	}

	void print(const char* objectName)
	{
		wcout << objectName << ": " << artist << ", " << title << endl;
	}
//private:
	wstring artist;
	wstring title;
};
unique_ptr<Song> SongFactory(const std::wstring& artist, const std::wstring& title)   //unique_ptr as function return value.
{
	// Implicit move operation into the variable that stores the result.
	return make_unique < Song > (artist, title);
}

void MakeSongs()
{
	// Create a new unique_ptr with a new object.
	auto song = make_unique < Song > (L"Mr. Children", L"Namonaki Uta");

	// Use the unique_ptr.
	vector<wstring> titles =
	{	song->title};
	song->print("song");
	if (song == nullptr)
	{
		wcout << "__1__null" << endl;
	}
	// Move raw pointer from one unique_ptr to another.
	unique_ptr<Song> song2 = std::move(song);//will call move constructor in unique_ptr, and can't use song variable, it assigen to  nullptr.
	song2->print("song2");
	if (song == nullptr)
	{
		wcout << "__2__null" << endl;
	}
	unique_ptr<Song> song4(std::move(song2));
	song4->print("song4");

	// Obtain unique_ptr from function that returns by value.
	auto song3 = SongFactory(L"Michael Jackson", L"Beat It");
	song3->print("song3");
}

void SongVector()
{
	vector < unique_ptr < Song >> songs;

	// Create a few new unique_ptr<Song> instances
	// and add them to vector using implicit move semantics.
	songs.push_back(make_unique < Song > (L"B'z", L"Juice"));// make_unique return a rvalue, so no need to use std::move().
	songs.push_back(make_unique < Song > (L"Namie Amuro", L"Funky Town"));
	songs.push_back(make_unique < Song > (L"Kome Kome Club", L"Kimi ga Iru Dake de"));
	songs.push_back(make_unique < Song > (L"Ayumi Hamasaki", L"Poker Face"));

	// Pass by const reference when possible to avoid copying.
	for (const auto& song : songs)
	{
		wcout << L"Artist: " << song->artist << L"   Title: " << song->title << endl;
	}
}

class ClassFactory
{
public:
	ClassFactory()
	{
	}
	void DoSomething()
	{
	}
};

class MyClass
{
private:
	// MyClass owns the unique_ptr.
	unique_ptr<ClassFactory> factory;
public:

	// Initialize by using make_unique with ClassFactory default constructor.
	MyClass() :
			factory(make_unique<ClassFactory>())
	{
	}

	void MakeClass()
	{
		factory->DoSomething();
	}
};

class Animal
{
public:
	std::wstring genus;
	std::wstring species;
	int age;
	double weight;
public:
	Animal(const wstring& g, const wstring& s, int a, double w):
	genus(g), species(s), age(a), weight(w)
	{
	}
	Animal() :
			genus(L"Default")
	{
	}
	Animal& operator=(const Animal&)
	{
		std::wcout <<__PRETTY_FUNCTION__ << std::endl;
	}

	Animal(Animal&& a)
	{
	genus = a.genus;
	a.genus=L"";
		std::wcout <<__PRETTY_FUNCTION__ << std::endl;
	}
	void print(const char* objectName)
	{
		wcout << objectName << ": " << genus << ", " << species << endl;
	}
};

void testParameter(unique_ptr<Animal> pt)//will call move constructor.
{
	pt->print("pt");
}

void testParameterReference(unique_ptr<Animal>& pt)//will not call move constructor
{
	pt->print("Reference...");
	pt= make_unique<Animal>(L"r2", L"b2", 2, 1);
}

unique_ptr<Animal> testParameter2(unique_ptr<Animal> pt)
{
	return pt;
}

unique_ptr<Animal> testParameter22()
{
	unique_ptr<Animal> te = make_unique<Animal>(L"aa", L"bb", 2, 1);
	return te;
}

unique_ptr<Animal> testParameter23()
{
	unique_ptr<Animal> te(new Animal(L"aa", L"bb", 2, 1));
	return te;
}

unique_ptr<Animal> testParameter233()
{
	Animal* a = new Animal(L"aa", L"bb", 2, 1);
	int c = 3, *d = &c;
	std::string s("hello");
	Animal i = Animal(L"ii", L"ee", 2, 1);

	std::wcout << __func__ << "ii=" << i.genus << std::endl;
	std::wcout << __func__ << a << std::endl;
	std::wcout << __func__ << L"c=" << c << std::endl;
	std::wcout << __func__ << L"s=" << s.c_str() << std::endl;

	unique_ptr<Animal> te(a);
	a = nullptr;	// have to assign nullptr to a;

	int bb = std::move(c);// due to int type no move copy constuctor, so c will not becaome null;
	std::string s2 = std::move(s);

	Animal e = std::move(i);

	std::wcout << __func__ << "ii=" << i.genus << std::endl;
	std::wcout << __func__ << L"c=" << c << std::endl;
	std::wcout << __func__ << L"s=" << s.c_str() << std::endl;
	std::wcout << __func__ << te.get() << std::endl;
	std::wcout << __func__ << te.get() << std::endl;
	std::wcout << __func__ << a << std::endl;
	return te;
}


unique_ptr<Animal> pGlobal = make_unique<Animal>(L"a", L"b", 20, 11);
//unique_ptr<Animal> testParameter3()
//{
//	pGlobal->print("pt");
//	return pGlobal;
//}
int c=9999;
void MakeAnimals()
{
	// Use the Animal default constructor.
	unique_ptr<Animal> p1 = make_unique<Animal>(L"F", L"C", 2, 1);
	p1->print("p1");


//	ptt = p1;//error
//	testParameter(p1);  //compile error, unique_ptr(const unique_ptr&) = delete

	unique_ptr<Animal> p0 = make_unique<Animal>(L"F", L"C", 2, 1);
	testParameterReference(p0);// use unique_ptr reference, no need to std::move.
	testParameter(move(p0));//will call move constructor


	unique_ptr<Animal> p00 = make_unique<Animal>(L"F", L"C", 2, 1);
	auto ptt = testParameter2(move(p00));//it will not call  unique_ptr(const unique_ptr&)
	ptt->print("ptt");

	auto ptt2=testParameter22();
	ptt2->print("ptt2");

	unique_ptr<Animal> ptt23= testParameter23();
	ptt23->print("ptt23");

	testParameter233();

//	testParameter3(); //error, it will call unique_ptr(const unique_ptr&)

	// Use the constructor that matches these arguments
	auto p2 = make_unique < Animal > (L"Felis", L"Catus", 12, 16.5);
	p2->print("p2");
	// Create a unique_ptr to an array of 5 Animals
	unique_ptr<Animal[]> p3 = make_unique<Animal[]>(5);

	// Initialize the elements
	p3[0] = Animal(L"Rattus", L"norvegicus", 3, 2.1);
	p3[0].print("p3[0]");
	p3[1] = Animal(L"Corynorhinus", L"townsendii", 4, 1.08);
	p3[1].print("p3[1]");

//	 auto p4 = p2; //compile error, copy constructor was delete.

	// unique_ptr overloads operator bool
	wcout <<L"p2==="<< boolalpha << (p2 == nullptr) << endl;

	// OK but now you have two pointers to the same memory location
	Animal* pAnimal = p2.get();
	pAnimal->print("Animal");
	// OK. p2 no longer points to anything
	Animal* p5 = p2.release();
	wcout << boolalpha << (p2 == nullptr) << endl;

	vector < unique_ptr < Animal >> vec;
	// vec.push_back(p2); //C2280
//	 vector<unique_ptr<Animal>> vec2 = vec; // C2280

	// OK. p2 no longer points to anything
	vec.push_back(std::move(p1)); // could not use vec.push_back(p1);

	p3[1].print("p3[1]");
	for (const auto& v : vec)
	{
		v->print("v");
	}
}

struct Vec3
{
	int x, y, z;
	Vec3() :
			x(0), y(0), z(0)
	{
	}
	Vec3(int x, int y, int z) :
			x(x), y(y), z(z)
	{
	}
	friend std::wostream& operator<<(std::wostream& os, Vec3& v)
	{
		return os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z << '}';
	}
};

void testVec3()
{
	// Use the default constructor.
	std::unique_ptr<Vec3> v1 = make_unique<Vec3>();
	// Use the constructor that matches these arguments
	std::unique_ptr<Vec3> v2 = make_unique < Vec3 > (0, 1, 2);
	// Create a unique_ptr to an array of 5 elements
	std::unique_ptr<Vec3[]> v3 = make_unique<Vec3[]>(5);
	wcout << "make_unique<Vec3>():      " << *v1 << endl << "make_unique<Vec3>(0,1,2): " << *v2 << endl << "make_unique<Vec3[]>(5):   " << endl;
	for (int i = 0; i < 5; i++)
	{
		wcout << "     " << v3[i] << endl;
	}
}


struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};

struct D {
    void operator() (Foo* p) {
        std::cout << "Calling delete for Foo object... \n";
        delete p;
    }
};

void test_reset()
{
    std::cout << "Creating new Foo...\n";
    std::unique_ptr<Foo, D> up(new Foo(), D());  // up owns the Foo pointer (deleter D)

    std::cout << "Replace owned Foo with a new Foo...\n";
    up.reset(new Foo());  // calls deleter for the old one

    std::cout << "Release and delete the owned Foo...\n";
    up.reset(nullptr);// also can use up.reset()
}

std::unique_ptr<std::vector<PcscfAddrNode> > pcscfAddrNodeVec(new std::vector<PcscfAddrNode>());

int main()
{
	MakeSongs();
	wcout << endl << wstring(50, '*') << endl << endl;
	SongVector();
	wcout << endl << wstring(50, '*') << endl << endl;
	MakeAnimals();
	testVec3();
	test_reset();
}

