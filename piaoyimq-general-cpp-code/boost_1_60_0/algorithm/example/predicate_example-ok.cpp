// Compile: g++  --std=c++11  predicate_example-ok.cpp
//  Boost string_algo library example file  ---------------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <functional>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/utility/string_ref.hpp>


using namespace std;
using namespace boost;

void test_predicate_algorithm()
{
    cout << "* Predicate Example *" << endl << endl;

    string str1("a123xXx321B");

    // Check if str1 starts with '123'
    cout << "str1 starts with \"a123\", starts_with: " << (starts_with(str1, string("a123")) ? "true" : "false") << endl;
    cout << "str1 starts with \"A123\", istarts_with: " << (istarts_with(str1, string("A123")) ? "true" : "false") << endl;

    // Check if str1 ends with '123'
    cout << "str1 ends with \"123\", end_with: " << (ends_with(str1, string("123")) ? "true" : "false") << endl;
    cout << "str1 ends with \"21B\", iend_with: " << (iends_with(str1, string("21B")) ? "true" : "false") << endl;

    // Check if str1 contains 'xxx'
    cout << "str1 contains \"xxx\", contains: " << (contains(str1, string("xxx")) ? "true" : "false") << endl;
    cout << "str1 contains \"xxx\", icontains: " << (icontains(str1, string("xxx")) ? "true" : "false") << endl;

    string str2("abc");

    // Check if str2 equals to 'abc'
    cout << "str2 equals \"abc\", equals: " << (equals(str2, string("abc")) ? "true" : "false") << endl;
    cout << "str2 equals \"aBc\", iequals: " << (iequals(str2, string("aBc")) ? "true" : "false") << endl;

    std::string str3("Abc");
    bool ret = lexicographical_compare(str3, str2);
    if (ret)
    {
        std::cout << "lexicographical_compare, " << str2 << " less than " << str3 << std::endl;
    }
    else
    {
        std::cout << "lexicographical_compare, " << str2 << " not less than " << str3 << std::endl;
    }

    ret = ilexicographical_compare(str3, str2);
    if (ret)
    {
        std::cout << "ilexicographical_compare, " << str2 << " less than " << str3 << std::endl;
    }
    else
    {
        std::cout << "ilexicographical_compare, " << str2 << " not less than " << str3 << std::endl;
    }

    // Classification functors and all predicate
    if (all(";.,", is_punct()))  // Note: no iall() function
    {
        cout << "\";.,\" are all punctuation characters" << endl;
    }

    // Classification predicates can be combined
    if (all("abcxxx", is_any_of("xabc") && !is_space()))
    {
        cout << "true" << endl;
    }

    assert(!all("Abc", boost::is_lower()));
    assert(all("abc", boost::is_lower()));
    assert(all("ABC", boost::is_upper()));

    // starts_with
    assert(boost::starts_with("boost_python-vc100-mt-1_49.dll", "boost"));
    assert(!boost::starts_with("boost_python-vc100-mt-1_49.dll", "BOOST"));
    assert(boost::istarts_with("boost_python-vc71-mt-1_33.dll", "BOOST"));

    // ends_with
    assert(boost::ends_with("boost_python-vc100-mt-1_49.dll", ".dll"));
    assert(!boost::ends_with("boost_python-vc100-mt-1_49.dll", ".DLL"));
    assert(boost::iends_with("boost_python-vc100-mt-1_49.dll", ".DLL"));

    // contains
    assert(boost::contains("boost_python-vc100-mt-1_49.dll", "python"));
    assert(!boost::contains("boost_python-vc100-mt-1_49.dll", "PYTHON"));
    assert(boost::icontains("boost_python-vc100-mt-1_49.dll", "PYTHON"));

    // equals
    assert(boost::equals("boost", "boost"));
    assert(!boost::equals("boost", "BOOST"));
    assert(boost::iequals("boost", "BOOST"));

    // Empty string test
    assert(boost::starts_with("boost_python-vc100-mt-1_49.dll", ""));
    assert(boost::ends_with("boost_python-vc100-mt-1_49.dll", ""));
    assert(boost::contains("boost_python-vc100-mt-1_49.dll", ""));

    // lexicalgrephical_compare
    assert(boost::lexicographical_compare("boost_python-vc100-mt-1_49.dll", "boost_system-vc100-mt-1_49.dll"));

    assert(boost::all("\x20\t\n\r", boost::is_any_of("\x20\t\n\r")));

}


void test_predicate_compare()
{
    const std::string str1("Samus");
    const std::string str2("samus");
    const std::string str3("SAMUS");

    assert(!boost::is_equal()(str1, str2));

//    assert(!boost::is_iequal()(str1, str2)); //TODO:  bad_cast???

    assert(boost::is_less()(str1, str2));
    assert(!boost::is_less()(str2, str1));
    assert(!boost::is_less()(str1, str1));
//    assert(!boost::is_iless()(str1, str2));

    assert(boost::is_not_greater()(str1, str2));
    assert(boost::is_not_greater()(str1, str1));
//    assert(boost::is_not_igreater()(str1, str2));

}


void test_classification()  //maily check one character if meet the requirement, used with all(str, is_xxx()) can check characters
{
    assert(boost::is_space()(' '));
    assert(all(" \t\n\r", boost::is_space()));

    assert(boost::is_alnum()('1')); // error:  assert(boost::is_alnum()("1")); or assert(boost::is_alnum()("abc"));
    assert(boost::is_alnum()('a'));
    assert(all("Good123", boost::is_alnum()));

    assert(!boost::is_alpha()('1'));
    assert(all("abcZ", boost::is_alpha()));
    assert(!all("abc3", boost::is_alpha()));

    assert(boost::is_cntrl()(static_cast<char>(1))); //^A
    assert(!boost::is_cntrl()(static_cast<char>(65))); // A

    assert(boost::is_digit()('1'));
    assert(!boost::is_digit()('_'));
    assert(all("123", boost::is_digit()));
    assert(!all("123c", boost::is_digit()));

    assert(boost::is_graph()('c'));  // graph   = _ISalpha | _ISdigit | _ISpunct;
    assert(all("Abc123,.!", boost::is_graph()));
    assert(all("#Abc123,.!/", boost::is_graph()));

    assert(boost::is_lower()('a'));
    assert(!boost::is_lower()('A'));
    assert(all("abc", boost::is_lower()));
    assert(!all("abC", boost::is_lower()));

    assert(!boost::is_print()('\n'));
    assert(!all("asdf123\t\n", boost::is_print())); // what is print character?

    assert(boost::is_punct()(','));
    assert(all("=+/,!?~__--##@", boost::is_punct()));
    assert(!all("a12=+/,!?~__--##@", boost::is_punct()));

    assert(!boost::is_upper()('a'));
    assert(boost::is_upper()('A'));
    assert(all("ABC", boost::is_upper()));
    assert(!all("abC", boost::is_upper()));

    assert(boost::is_xdigit()('a'));
    assert(all("018abcdef", boost::is_xdigit()));
    assert(!all("18fg", boost::is_xdigit()));

    assert(all("fffg", boost::is_any_of("fg")));
    assert(!all("1fffg", boost::is_any_of("fg")));

    assert(boost::all("abcde", boost::is_from_range('a','e'))); //[a,e], from <= ch <= to
    assert(boost::all("abcde", boost::is_from_range('a','z')));
    assert(!boost::all("abcde", boost::is_from_range('b','c')));
    assert(!boost::all("abc __ de", boost::is_from_range('a','z')));
    assert(boost::all("abc __ de", boost::is_from_range('a','z') || boost::is_space() || boost::is_any_of("_")));

    assert(boost::all("\x20\t\n\r", boost::is_space()));
    assert(boost::all("\x20\t\n\r", boost::is_classified(std::ctype_base::space)));

    struct is_zero_or_one  // <==> is_any_of("01")
    {
        bool operator() (char x)
        {
            return '0'==x || '1' ==x;
        }
    };


    auto is01 =[](char x){return '0'==x || '1' ==x;};

    assert(boost::all("010001", is_zero_or_one()));
    assert(boost::all("010001", is01));
    assert(!boost::all("0100301", is01));
}


int main()
{
    test_predicate_algorithm();

    test_predicate_compare();

    test_classification();

    return 0;
}
