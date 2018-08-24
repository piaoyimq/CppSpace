//Compile command: g++ find_string_example.cpp -lboost_regex

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

#include <boost/algorithm/string/regex.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;
using namespace boost;


void test_string_find_string()
{
    std::string str1("a1234_first_nth_first_nth_");
    boost::iterator_range<std::string::iterator> ir;

    ir = boost::find_first(str1, "first");

    assert(std::string(ir.begin(), ir.end()) == "first");
    assert(ir.begin() - str1.begin() == 6 && ir.end() - str1.begin() == 6 + 5);

    boost::to_upper(ir);
    assert(str1 == "a1234_FIRST_nth_first_nth_");
    boost::to_lower(ir);
    assert(str1 == "a1234_first_nth_first_nth_");

    ir = boost::find_first(str1, "no");
    assert(ir.empty());
    assert(std::string(ir.begin(), ir.end()).empty());
    std::ostringstream osstr;
    osstr << boost::find_first(str1, "_first_");
    assert(osstr.str() == "_first_");
}


void test_string_find_token()
{
    using namespace boost;

    std::string str1("ab1234_first_nth_first_nth_");
    iterator_range<std::string::iterator> ir;

    ir = find_token(str1, is_any_of("irfst"));
    assert(std::string(ir.begin(), ir.end()) == "f");

    ir = find_token(str1, is_any_of("xfirts"), token_compress_off);
    assert(std::string(ir.begin(), ir.end()) == "f");

    ir = find_token(str1, is_any_of("irfst"), token_compress_on);
    assert(std::string(ir.begin(), ir.end()) == "first");

    ir = find_token(str1, is_any_of("fitr "), token_compress_on);
    assert(std::string(ir.begin(), ir.end()) == "fir");

    ir = find_token(str1, is_lower(), token_compress_on);
    assert(std::string(ir.begin(), ir.end()) == "ab");
}


// ???????
// #include <boost/algorithm/string/regex.hpp>
// find_regex, find_all_regex
void test_string_find_regex()
{
    using namespace boost;

    std::string str1("ab1234_first_nth_first_nth_");
    iterator_range<std::string::iterator> ir;
    regex rx("b[0-9]+_");

    ir = find_regex(str1, rx);
    assert(std::string(ir.begin(), ir.end()) == "b1234_");

    std::string str2("b1_b22_b333_b4444");
    std::vector<std::string> tokens;
    find_all_regex(tokens, str2, rx);
    assert(tokens.size() == 3);
    assert(tokens[0] == "b1_");
    assert(tokens[1] == "b22_");
    assert(tokens[2] == "b333_");

    // ???????????
    std::string value = "123a1cxxxxa56c";
    regex pattern("a[0-9]+c");
    iterator_range<std::string::iterator> find_result;
    find_result = algorithm::find_regex(value, pattern);
    assert(!find_result.empty());

    std::vector<std::string> results;
    find_all_regex(results, value, pattern);
    assert("a1c" == results[0]);
    assert("a56c" == results[1]);
    assert(!results.empty());

    value = "10.10.10.10 1.1.1.1";
    boost::regex ip_pattern("(\\d{1, 3}.){3}\\d{1, 3}");
    find_all_regex(results, value, ip_pattern);
    assert("10.10.10.10" == results[0]);
    assert("1.1.1.1" == results[1]);
    assert(!results.empty());
}


void find_first_or_head_or_last()
{  
    cout << "* Find Example *" << endl << endl;

    string str1("abc___cde___efg");
    string str2("abc");

    // find "cde" substring
    iterator_range<string::iterator> range=find_first( str1, string("cde") );

    // convert a substring to upper case 
    // note that iterator range can be directly passed to the algorithm
    to_upper( range );

    cout << "str1 with upper-cased part matching cde: " << str1 << endl;

    // get a head of the string
    iterator_range<string::iterator> head=find_head( str1, 3 );
    cout << "head(3) of the str1: " << string( head.begin(), head.end() ) << endl;

    // get the tail
    head=find_tail( str2, 5 );
    cout << "tail(5) of the str2: " << string( head.begin(), head.end() ) << endl;

    // char processing
    char text[]="hello dolly!";
    iterator_range<char*> crange=find_last(text,"lly");

    // transform the range ( add 1 )
    transform( crange.begin(), crange.end(), crange.begin(), bind2nd( plus<char>(), 1 ) );
    // uppercase the range
    to_upper( crange );

    cout << text << endl;

    cout << endl;

}


int main()
{
//    test_string_find_string();
    test_string_find_token();
//    test_string_find_regex();
//    find_first_or_head_or_last();

}
