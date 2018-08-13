/*
 * thread-demo.cpp
 *
 *  Created on: Jul 27, 2018
 *      Author: azhweib
 */




#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadTarget.h"
#include "Poco/RunnableAdapter.h"

using Poco::Thread;
using Poco::Runnable;
using Poco::ThreadTarget;
using Poco::RunnableAdapter;

class MyRunnable:public Runnable
{
public:
    void run() { std::cout << "hello MyRunnable." << std::endl; }
};

void gFun4Td()
{
    std::cout << "hello gFun4Td" << std::endl;
}

class staticFun4Td
{
public:
    static void staticFun() { std::cout << "hello static fun." << std::endl; }
};

class commFun4Td
{
public:
    void commFun() { std::cout << "hello common function." << std::endl; }
};

int main()
{
    Thread t1("MyRun");
    Thread t2("global");
    Thread t3("static");
    Thread t4("comm");

    MyRunnable rMy;
    ThreadTarget rg(gFun4Td);
    ThreadTarget rs(&staticFun4Td::staticFun);
    commFun4Td com;
    RunnableAdapter<commFun4Td> rc(com,&commFun4Td::commFun);

    t1.start(rMy);
    Thread::sleep(100);
    t2.start(rg);
    Thread::sleep(100);
    t3.start(rs);
    Thread::sleep(100);
    t4.start(rc);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
