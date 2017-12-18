#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Reactor.h"
#include "Singleton.h"




namespace reactor
{

class Reactor;


class Global : public Singleton<Global>
{
public:
    Global(void);
    ~Global(void);

    Reactor* g_reactor_ptr;
};

#define sGlobal reactor::Global::instance()

}


#endif
