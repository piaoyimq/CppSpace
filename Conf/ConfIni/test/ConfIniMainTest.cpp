/*
 * ConfIniMainTest.cpp
 *
 *  Created on: 2016-05-22
 *      Author: piaoyimq
 */
#include "Conf/ConfIni/include/DIniFile.h"
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[])
{
	static_cast<void>(argc);
	static_cast<void>(argv);
	const char* variableString;
	int variableInt;
	double variableFloat;

	DIniFile confFile("Conf/ConfIni/test/application.ini");
	variableString = confFile.ReadString("App", "Name", "Undefined");
	std::cout <<  "[App]  " << "Name=" << variableString << std::endl;

	variableString = confFile.ReadString("Ap", "Name", "Undefined");
	std::cout <<  "[Ap]   " << "Name=" << variableString << std::endl;

	variableString = confFile.ReadTString("App", "Vendor", "Undefined");
	std::cout <<  "[App]  " << "Vendor=" << variableString << std::endl;

	confFile.WriteString("NewSession", "key1", "yes");

	variableInt = confFile.ReadInteger("XRE", "EnableProfileMigrator", 0);
	std::cout <<  "[XRE]  " << "EnableProfileMigrator=" << variableInt << std::endl;

	variableFloat = confFile.ReadFloat("Gecko", "Key2", 0.01);
	std::cout <<  "[Gecko]  " << "Key2=" << variableFloat << std::endl;

	return EXIT_SUCCESS;
}

