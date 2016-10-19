/*
 * tokenizer.cpp
 *
 *  Created on: Oct 18, 2016
 *      Author: piaoyimq
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <boost/tokenizer.hpp>
using namespace std;

int main()
{
	std::string str = ";;Hello|world||-foo--bar;yow;baz|"; //	std::string str = "apn, apn1.com,*,apn2.com,   apn4.com,,,apn5.com";

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("-;|"); //boost::char_separator<char> sep(", ");

	tokenizer tokens(str, sep);

	std::vector<std::string>* indexType = new std::vector<std::string>();

	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		std::cout << "<" << *tok_iter << "> ";
	}

	std::cout << "\n";

	return 0;
}

