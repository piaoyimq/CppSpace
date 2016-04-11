
#ifndef Lock_H
#define Lock_H

#include <string>
#include <algorithm>
namespace Concurrency
{
class ILockable;


// Purpose:
// Provides a "Resource Acquisition is Initialisation" interface to the mutex class.
//
// Description:
// The Lock works as a stack variable that aquires a mutex in its constructor and releases in the destructor.
// See TC++PL. (Stroustrup)
//
//
class Lock
{
public:

    virtual ~Lock();

    explicit Lock(ILockable& lockable);

private:

    // No dynamic allocation allowed, this would defeat the purpose
    void* operator new(size_t size);

    // No dynamic allocation allowed, this would defeat the purpose
    void* operator new[](size_t size);


    bool isLocked_;

    ////    Relations and components    ////

    ILockable& itsLockable;
};
}

#endif
/*-------------------------------------------------------------------------------------------------------------------*/
