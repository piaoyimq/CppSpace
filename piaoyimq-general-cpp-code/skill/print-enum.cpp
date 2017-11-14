#include <iostream>

#define ELEMENTS \
    CHANGE(Emergency) \
    CHANGE(Alert) \
    CHANGE(Critical) \
    CHANGE(Error) \
    CHANGE(Warning) \
    CHANGE(Notice) \
    CHANGE(Info) \
    CHANGE(Debug) \
    CHANGE(LastLevel) \


#define CHANGE(a) a,
enum Level
{
    ELEMENTS
};
#undef CHANGE


#define CHANGE(a) #a,
const char logLevelString[][20] = {ELEMENTS };
#undef CHANGE

template <class T>
inline size_t getArrayLen(T& array)
{
    return (sizeof(array) / sizeof(array[0]));
}

inline const char* getLogLevelString(Level logLevel)
{
    return logLevel < (static_cast<Level>(getArrayLen(logLevelString))) ? logLevelString[logLevel] : "Undefined";
}

int main()
{
    std::cout << getLogLevelString(Emergency) << std::endl;
    std::cout << getLogLevelString(LastLevel) << std::endl;
}
