#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <boost/algorithm/string/case_conv.hpp>

using namespace std;
using namespace boost;

void test_to_upper()
{
    string str1("AbCdEfG");
    std::string strUpper = boost::to_upper_copy(str1);

    std::cout << "strUpper=" << strUpper << std::endl;

    boost::to_upper(str1);

    std::cout << "str1, to_upper=" << str1 << std::endl;
}


void test_to_lower()
{  
    cout << "* Case Conversion Example *" << endl << endl;

    string str1("AbCdEfG");
    std::cout << "original string=" << str1 << std::endl;
    vector<char> vec1( str1.begin(), str1.end() );
    
    // Convert vector of chars to lower case
    cout << "lower-cased copy of vec1: ";
    to_lower_copy( ostream_iterator<char>(cout, "-"), vec1 );
    cout << endl;

    std::stringstream ss;
    to_lower_copy( ostream_iterator<char>(ss, "_"), vec1 );

    std::cout << "stringstream, to_lower_copy="<< ss.str() << std::endl;


    // Conver string str1 to upper case ( copy the input )
    cout << "upper-cased copy of str1: " << to_upper_copy( str1 ) << endl;

    // Inplace conversion
    to_lower( str1 );
    cout << "lower-cased str1: " << str1 << endl;

    cout << endl;

}


int main()
{
    test_to_upper();
    test_to_lower();
}
