//stl_cpp_12.cpp
#include <iostream>
#include <map>
using namespace std;
int main(void)
{
	map<char, int, less<char> > map1;
	map<char, int, less<char> >::iterator mapIter;
//	map1['c'] = 3;
//	map1['d'] = 4;
//	map1['a'] = 1;
//	map1['b'] = 2;
	for (mapIter = map1.begin(); mapIter != map1.end(); ++mapIter)
	{
		cout << " " << (*mapIter).first << ": " << (*mapIter).second;
	}

	map<char, int, less<char> >::const_iterator ptr;
	ptr = map1.find('d');
	if (ptr == map1.begin())
	{
		std::cout << "begin\n";
//		std::cout << "ptr=" << ptr<< ", begin=" << map1.begin() << ", end=" << map1.end() << std::endl;
	}

	if (ptr == map1.end())
	{
		std::cout << "end\n";
		//		std::cout << "ptr=" << ptr<< ", begin=" << map1.begin() << ", end=" << map1.end() << std::endl;
	}
	cout << '\n' << " Key: " << (*ptr).first << " , Value: " << (*ptr).second;
	return 0;
}
