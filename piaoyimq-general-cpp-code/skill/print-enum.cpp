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

#define ENUM_NAME Level


class Test
{
public:

    enum ENUM_NAME
    {
        ELEMENTS
    };
    #define IN_CLASS 1
    #include "print-enum.h"

};

//
//#define CHANGE(a) a,
//enum Level
//{
//    ELEMENTS
//};
//
//#undef CHANGE
//
//
//#define CHANGE(a) #a,
////const char logLevelString[][20] = {ELEMENTS };
//const char *logLevelString[] = {ELEMENTS };
//#undef CHANGE
//
//template <class T>
//inline size_t getArrayLen(T& array)
//{
//    return (sizeof(array) / sizeof(array[0]));
//}
//
//inline const char* getLogLevelString(Level logLevel)
//{
//    return logLevel < (static_cast<Level>(getArrayLen(logLevelString))) ? logLevelString[logLevel] : "Undefined";
//}
//
//std::ostream& operator<<(std::ostream& os, Level logLevel)
//{
//    os << (logLevel < (static_cast<Level>(getArrayLen(logLevelString))) ? logLevelString[logLevel] : "Undefined");
//    return os;
//}

int main()
{
    std::cout << Emergency << std::endl;
    std::cout << LastLevel << std::endl;
}
