/*
 * greedy-match.cpp
 *
 *  Created on: Aug 3, 2017
 *      Author: ezhweib
 */




#include <iostream>
#include <string>
#include <regex>

int main()
{
    std::string str = "zzxayyzz";
    std::regex re1(".*(a|xayy)"); // ECMA， 贪婪匹配
    std::regex re2(".*?(a|xayy)"); // ECMA， 非贪婪匹配


    std::cout << "Searching for .*(a|xayy) in zzxayyzz:\n";
    std::smatch m;
    std::regex_search(str, m, re1);
    std::cout << " ECMA (\".*(a|xayy)\") match: " << m[0] << '\n';
    std::regex_search(str, m, re2);
    std::cout << " ECMA (\".*?(a|xayy)\") match: " << m[0] << '\n';

}
