/*
 * common.h
 *
 *  Created on: Apr 6, 2016
 *      Author: piaoyimq
 */

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_
#include <stddef.h>
#include <sys/types.h>
#include <sys/syscall.h>	//SYS_gettid
#include <unistd.h>	//syscall
#include <dirent.h>	//opendir
#include "../src/PrintOut.h"
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <list>


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

inline pid_t getTid()
{   // Must a static function, it called by a static funcion flushLogThread().
	return syscall(SYS_gettid);
}

inline bool isDirPathExist(const char* dirpPath)
{
	DIR *dirPtr = opendir(dirpPath);
	if (NULL == dirpPath || NULL == dirPtr)
	{
		return false;
	}
	else
	{
		closedir(dirPtr);
		dirPtr = NULL;
		return true;
	}
}

extern void getPidByName(char* task_name);
extern void getNameByPid(pid_t pid, char *processName);
#endif /* COMMON_COMMON_H_ */
