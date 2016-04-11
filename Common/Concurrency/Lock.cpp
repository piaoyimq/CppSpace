

#include "Lock.h"
#include "ILockable.h"
namespace Concurrency
{

Lock::~Lock()
{
    // Don't unlock the mutex if we didn't manage to lock it in the first place
    // (Should only happen if the mutex was locked by this thread (the same thread))
    if (isLocked_)
    {
        itsLockable.unlock();
    }
}

Lock::Lock(ILockable& lockable) : itsLockable(lockable)
{
    isLocked_ = itsLockable.lock();
}
}

/*-------------------------------------------------------------------------------------------------------------------*/
