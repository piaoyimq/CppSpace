/*
 * glob-test.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: azhweib
 */

// g++ glob-test.cpp -I /opt/gss/poco_1_6/include -L /opt/gss/poco_1_6/lib/ -lPocoFoundation -Wl,-rpath=/opt/gss/poco_1_6/lib/

#include "Poco/Glob.h"
#include "Poco/String.h"
#include <iostream>
#include <fstream>
using Poco::Glob;


int test1()
{
    std::set<std::string> files;
    //Glob::glob("%WINDIR%\\system32\\*.exe", files);
    Glob::glob("/usr/include/*/*.h", files);
    std::set<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    return 0;
}


int test2(const std::string& processCmdLine)
{
    std::set<std::string> files;
//    std::string processCmd(processCmdLine);
//    processCmd.replace(processCmd.begin(), processCmd.end(), 1, 'A');
    std::string processCmd = Poco::replace(processCmdLine, " ", "\0x00");//^@
    std::cout << "____processCmd=" << processCmd << std::endl;
    Glob::glob("/proc/*/cmdline", files);
    std::set<std::string>::iterator it = files.begin();
    for (; it != files.end(); ++it)
    {
        std::cout << *it << std::endl;
        std::ifstream cmdLineFile((*it).c_str());

        std::string cmdLine;
        std::getline(cmdLineFile, cmdLine);
        std::cout << "____cmdLine" << cmdLine << std::endl;
        if (std::string::npos != cmdLine.find(processCmd))
        {
            std::cout << cmdLine << std::endl;
            break;
        }
    }

    return 0;
}


int main()
{
//    test1();

    test2("/bin/java -Dosgi.requiredJavaVersion=1.8");
}
