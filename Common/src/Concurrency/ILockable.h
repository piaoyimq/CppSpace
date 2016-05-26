
#ifndef ILockable_H
#define ILockable_H

#include <string>
#include <algorithm>
namespace Concurrency
{

// Purpose:
// Provide a generic interface for "Lockable" objects, such as pthread mutexes, spinlocks, etc..
//
// Description:
// N/A
class ILockable
{
public:

    virtual bool lock()   = 0;

    virtual void unlock() = 0;

    inline virtual ~ILockable()
    {
    }
};
}

#endif
/*-------------------------------------------------------------------------------------------------------------------*/
