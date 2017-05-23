/*
 * outputStlContainer.hpp
 *
 *  Created on: May 23, 2017
 *      Author: ezhweib
 */

#ifndef GENERAL_COMMON_INCLUDE_OUTPUTSTLCONTAINER_HPP_
#define GENERAL_COMMON_INCLUDE_OUTPUTSTLCONTAINER_HPP_
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <list>

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
	s.put('[');
	char comma[3] = { '\0', ' ', '\0' };
	for (const auto& e : v)
	{
		s << comma << e;
		comma[0] = ',';
	}
	return s << ']';
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::list<T>& v)
{
	s.put('[');
	char comma[3] = { '\0', ' ', '\0' };
	for (const auto& e : v)
	{
		s << comma << e;
		comma[0] = ',';
		// std::cout << s <<std::endl;
	}
	return s << ']';
}

template<typename TK, typename TV>
std::ostream& operator<<(std::ostream& s, const std::map<TK, TV>& m)
{
	s.put('{');
	char comma[3] = { '\0', ' ', '\0' };
	for (const auto& e : m)
	{
		s << comma << e.first << ':' << e.second;
		comma[0] = ',';
	}
	return s << '}';
}


#endif /* GENERAL_COMMON_INCLUDE_OUTPUTSTLCONTAINER_HPP_ */
