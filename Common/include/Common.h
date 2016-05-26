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
