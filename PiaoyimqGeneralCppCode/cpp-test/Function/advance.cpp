#include <vector>
#include <iostream>     // std::cout
#include <iterator>     // std::front_inserter
#include <list>         // std::list
#include <algorithm>    // std::copy

#if 1
int main () {
  std::list<int> foo,bar;
  for (int i=1; i<=5; i++)
  { foo.push_back(i); bar.push_back(i*10); }

  std::list<int>::iterator it = foo.begin();
  advance (it,3);

  std::copy (bar.begin(),bar.end(),std::inserter(foo,it));

  std::cout << "foo contains:";
  for ( std::list<int>::iterator it = foo.begin(); it!= foo.end(); ++it )
	  std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}

#else
int main() 
{
    std::vector<int> v{ 3, 1, 4 };

    auto vi = v.begin();

    std::advance(vi, 1);

    std::cout << *vi << '\n';
    
    for(std::vector<int>::iterator it=v.begin(); it != v.end(); ++it){
        std::cout << *it;
    }
    std::cout << std::endl;
    for(; vi != v.end(); ++vi){
        std::cout << *vi;
    }
}
#endif