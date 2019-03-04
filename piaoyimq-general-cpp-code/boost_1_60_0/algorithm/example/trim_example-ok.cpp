//  Boost string_algo library example file  ---------------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
using namespace std;
using namespace boost;

void trim_test()
{
    string str1("     1x x x x1     ");
    string str2("<>trim<>");
    string str3("{[\"abs\"]}");


    trim_left(str1);
    assert("1x x x x1     " == str1);
    assert("1x x x x1     " == trim_left_copy(str1));

    trim_left_if(str2, is_any_of("<>")); // Error: trim_left_if(std::string("<>trim<>"), is_any_of("<>"));
    assert("trim<>" ==  str2);
    str2="<>trim<>";
    assert("trim<>" == trim_left_copy_if(std::string("<>trim<>"), is_any_of("<>")));

    std::cout << "____str3=" << str3 << std::endl;
    trim_if(str3, is_any_of("{[\"]}"));
    std::cout << "____str3=" << str3 << std::endl;

//trim, trim_right, trim_all, trim_fill same as trim_left, have 4 kinds of types.

}

int main()
{
    trim_test();

    return 0;
}
