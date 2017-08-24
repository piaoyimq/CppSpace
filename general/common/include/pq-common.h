/*
 * pq-common.h
 *
 *  Created on: Apr 6, 2016
 *      Author: piaoyimq
 */

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_
#include <stddef.h>
#include <sys/types.h>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <list>
#include "PrintOut.h"
#include "Range.h"


extern std::string getNameByPid(pid_t pid);


#define ABORT() \
    std::cerr << "################################################" << std::endl \
              << "ABORT in function '" << __PRETTY_FUNCTION__ << "' " \
              << "File=" << __FILE__ << " line=" << __LINE__ << std::endl \
              << "################################################" << std::endl; \
    ::abort();

#define ASSERT(CONDITION) \
    if (!(CONDITION)) \
    { \
        (Assert::fail(__FILE__, __LINE__, __PRETTY_FUNCTION__, #CONDITION)); \
    }

//vector
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

//list
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

//map
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

template<class out_type, class in_value>
out_type convert(const in_value &t)
{
	out_type ans;
	std::stringstream a;
	a << t;
	a >> ans;
	return ans;
}

template<class T>
inline size_t getArrayLen(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}


#endif /* COMMON_COMMON_H_ */
