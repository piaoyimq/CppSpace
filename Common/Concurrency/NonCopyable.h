
#ifndef NonCopyable_H
#define NonCopyable_H

#include <string>
#include <algorithm>

// Derive from this class to make your class non-copyable.
// This class will make copy-constructor and assignment operator private.
class NonCopyable
{
protected:

    explicit inline NonCopyable()
    {
    }

    inline ~NonCopyable()
    {
    }

private:

    explicit NonCopyable(const NonCopyable& other);

    NonCopyable& operator=(const NonCopyable& other);
};

#endif
/*-------------------------------------------------------------------------------------------------------------------*/
