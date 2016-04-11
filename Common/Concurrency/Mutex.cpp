

#include "Mutex.h"
#include <sstream>
namespace Concurrency
{



Mutex::Mutex()
{
    memset(&mutex_, 0, sizeof(pthread_mutex_t));
    int ret;

    // On Linux, we need to set the ERRORCHECK_NP attribute explicitly.
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);

    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK_NP);
    ret = pthread_mutex_init(&mutex_, &mutexAttr);
    if (ret != 0)
    {
        std::cout << "Error from pthread_mutex_init error id: " << ret << std::endl;
        ABORT();
    }

    // Clean up
    ret = pthread_mutexattr_destroy(&mutexAttr);
    if (ret != 0)
    {
        std::cout << "Failed to pthread_mutexattr_destroy, error id: " << ret << std::endl;
        ABORT();
    }
}

Mutex::~Mutex()
{
    int ret;
    if ((ret = pthread_mutex_destroy(&mutex_)) != 0)
    {
        std::cout << "pthread_mutex_destroy failed! error: " << ret << std::endl;
        ASSERT(false);
    }
}

pthread_mutex_t* Mutex::getMutex()
{
    return &mutex_;
}



SingletonMutex::ScopedLock::ScopedLock()
{
    SingletonMutex::lock();
}

SingletonMutex::ScopedLock::~ScopedLock()
{
    SingletonMutex::unlock();
}

SingletonMutex::~SingletonMutex()
{
}
}

/*-------------------------------------------------------------------------------------------------------------------*/
