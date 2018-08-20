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

extern std::string get_process_name(pid_t pid);  //only can get the process first 15 charaters name.

inline pid_t get_tid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

//vector print
template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
	s.put('[');
	char comma[3] = { '\0', ' ', '\0' };
	for(typename std::vector<T>::const_iterator it=v.begin(); it!=v.end(); ++it)
	{
		s << comma << *it;
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
    for(typename std::list<T>::const_iterator it=v.begin(); it!=v.end(); ++it)
    {
        s << comma << *it;
        comma[0] = ',';
    }
	return s << ']';
}

//map
template<typename TK, typename TV>
std::ostream& operator<<(std::ostream& s, const std::map<TK, TV>& m)
{
	s.put('{');
	char comma[3] = { '\0', ' ', '\0' };
	for(typename std::map<TK, TV>::const_iterator it = m.begin(); it !=m.end(); ++it)
	{
		s << comma << it->first << ": " << it->second;
		comma[0] = ',';
	}
	return s << '}';
}
std::tolower()
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
