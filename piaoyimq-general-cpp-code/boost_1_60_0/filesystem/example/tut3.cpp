//  filesystem tut3.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <iostream>
#include <boost/filesystem.hpp>
using std::cout;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut3 path\n";
    return 1;
  }

  path p (argv[1]);

  try
  {
    if (exists(p))
    {
      if (is_regular_file(p))
        cout << p << " size is " << file_size(p) << '\n';

      else if (is_directory(p))
      {
        cout << p << " is a directory containing:\n";

#if 0
        for (directory_entry& x : directory_iterator(p))
        	cout << "    " << x.path() << '\n';
#else
            directory_iterator end;
            /**无参构造函数是最后那个iterator的value 摘抄如下
           *If the end of the directory elements is reached, the iterator becomes equal to the end iterator value.
           *If  The constructor directory_iterator() with no arguments always constructs an end iterator object,
           *If  which is the only legitimate iterator to be used for the end condition. The result of operator* on an end iterator is not defined.
           *If   For any other iterator value a const directory_entry& is returned. The result ofoperator-> on an end iterator is not defined.
           *If    For any other iterator value a const directory_entry* is returned.
           *
           **/
            for (directory_iterator pos(p); pos != end; pos++)
            	cout << "    " << pos->path() << '\n';
#endif

      }
      else
        cout << p << " exists, but is not a regular file or directory\n";
    }
    else
      cout << p << " does not exist\n";
  }

  catch (const filesystem_error& ex)
  {
    cout << ex.what() << '\n';
  }

  return 0;
}
