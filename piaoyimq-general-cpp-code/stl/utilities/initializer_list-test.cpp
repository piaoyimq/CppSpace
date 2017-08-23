#include <iostream>
#include <vector>
#include <initializer_list>
#include <typeinfo>

template<class T>
struct S
{
    std::vector<T> v;
    S(std::initializer_list<T> l) :
            v(l)
    {
        std::cout << "constructed with a " << l.size() << "-element list\n";
    }

    void append(std::initializer_list<T> l)
    {
        v.insert(v.end(), l.begin(), l.end());
    }

    std::pair<const T*, std::size_t> c_arr() const
    {
        return {   &v[0], v.size()};  // copy list-initialization in return statement
                               // this is NOT a use of std::initializer_list, // Error until C++17, should use std::make_pair();
    }

};

template<typename T>
void templated_fn(T)
{
}

void test_initilizer_list_1()
{
    S<int> s = { 1, 2, 3, 4, 5 }; // invoke copy list-initialization
    s.append( { 6, 7, 8 });      // list-initialization in function call
    S<std::string> ss{"s1","s2","s3"}; //invoke constructor of initilizer_list
    std::cout << "The vector size is now " << s.c_arr().second << " ints:\n";

    for (auto n : s.v)
        std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "Range-for over brace-init-list: \n";

    for (int x : { -1, -2, -3 }) // the rule for auto makes this ranged-for work
        std::cout << x << ' ';
    std::cout << '\n';

    auto al = { 10, 11, 12 };   // special rule for auto
    std::cout << "typeid(al).name=" << typeid(al).name() << std::endl;
    std::cout << "typeid(s.v).name=" << typeid(s.v).name() << std::endl;

    std::cout << "The list bound to auto has size() = " << al.size() << '\n';

//    templated_fn({1, 2, 3}); // compiler error! "{1, 2, 3}" is not an expression,
    // it has no type, and so T cannot be deduced
    templated_fn<std::initializer_list<int>>( { 1, 2, 3 }); // OK
    templated_fn<std::vector<int>>( { 1, 2, 3 });           // also OK
}

void test_initilizer_list_2()
{
    std::initializer_list<int> empty_list;
    std::cout << "empty_list.size(): " << empty_list.size() << '\n';

    // create initializer lists using list-initialization
    std::initializer_list<int> digits { 1, 2, 3, 4, 5 };
    std::cout << "digits.size(): " << digits.size() << '\n';

    // special rule for auto means 'fractions' has the
    // type std::initializer_list<double>
    auto fractions = { 3.14159, 2.71828 };
    std::cout << "fractions.size(): " << fractions.size() << '\n';
}


std::initializer_list<int> return_initializer_list()
{
    return {1,2,3};
}


void test_initilizer_list_3()
{
    auto re=return_initializer_list();
    for(auto r:re)
    {
        std::cout << r <<", ";
    }
}


int main()
{
    test_initilizer_list_1();
    test_initilizer_list_2();
    test_initilizer_list_3();
}
