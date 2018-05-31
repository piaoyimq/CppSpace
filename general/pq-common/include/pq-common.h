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
#include <sys/syscall.h>
#include <utility>
#include <unistd.h>
#include <iomanip>
#include <map>
#include <vector>
#include <list>
#include "PrintClass.h"
#include "RangeImpl.h"
#include "pq-macro.h"

extern std::string get_name_by_pid(pid_t pid);

inline pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

//vector print
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
	s.put('[');
	char comma[3] = { '\0', ' ', '\0' };
	for(int i=0; i< v.size(); ++i)
	{
		s << comma << e[i];
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
//	for (const auto& e : v)
    for(int i=0; i< v.size(); ++i)
	{
		s << comma << e[i];
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
		s << comma << e.first << ": " << e.second;
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
inline size_t get_array_len(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}

#endif /* COMMON_COMMON_H_ */
