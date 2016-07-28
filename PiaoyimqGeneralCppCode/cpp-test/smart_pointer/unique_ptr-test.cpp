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
unique_ptr<Song> SongFactory(const std::wstring& artist, const std::wstring& title)
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
	unique_ptr<Song> song2 = std::move(song);
	song2->print("song2");
	if (song == nullptr)
	{
		wcout << "__2__null" << endl;
	}

	// Obtain unique_ptr from function that returns by value.
	auto song3 = SongFactory(L"Michael Jackson", L"Beat It");
	song3->print("song3");
}

void SongVector()
{
	vector < unique_ptr < Song >> songs;

	// Create a few new unique_ptr<Song> instances
	// and add them to vector using implicit move semantics.
	songs.push_back(make_unique < Song > (L"B'z", L"Juice"));
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
private:
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
	void print(const char* objectName)
	{
		wcout << objectName << ": " << genus << ", " << species << endl;
	}
};

void MakeAnimals()
{
	// Use the Animal default constructor.
	unique_ptr<Animal> p1 = make_unique<Animal>();
	p1->print("p1");

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

	// auto p4 = p2; //C2280
	//当你看到带unique_ptr的连接错误C2280，几乎可以肯定，因为你正试图调用它的拷贝构造函数，这是一个已删除的功能。

	// unique_ptr overloads operator bool
	wcout << boolalpha << (p2 == nullptr) << endl;	// Prints "true"

	// OK but now you have two pointers to the same memory location
	Animal* pAnimal = p2.get();
	pAnimal->print("Animal");
	// OK. p2 no longer points to anything
	Animal* p5 = p2.release();
	wcout << boolalpha << (p2 == nullptr) << endl;	// Prints "true"

	vector < unique_ptr < Animal >> vec;
	// vec.push_back(p2); //C2280
	// vector<unique_ptr<Animal>> vec2 = vec; // C2280

	// OK. p2 no longer points to anything
	vec.push_back(std::move(p1));
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
	friend std::ostream& operator<<(std::ostream& os, Vec3& v)
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

	wcout << L"make_unique<Vec3>():      " << *v1 << L'\n' << L"make_unique<Vec3>(0,1,2): " << *v2 << L'\n' << L"make_unique<Vec3[]>(5):   " << L'\n';
	for (int i = 0; i < 5; i++)
	{
		wcout << L"     " << v3[i] << L'\n';
	}
}
int main()
{
	MakeSongs();
	wcout << endl << wstring(50, '*') << endl << endl;
	SongVector();
	wcout << endl << wstring(50, '*') << endl << endl;
	MakeAnimals();
	testVec3();
}

