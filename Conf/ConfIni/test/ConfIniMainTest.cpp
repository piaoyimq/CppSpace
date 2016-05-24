/*
 * ConfIniMainTest.cpp
 *
 *  Created on: 2016-05-22
 *      Author: piaoyimq
 */
#include "Conf/ConfIni/include/ConfigIni.h"
#include "Ut/googletest/include/gtest/gtest.h"
#include <stdlib.h>
#include <iostream>


TEST(ConfigIni, ReadString) {
//  Counter c;

  // EXPECT_EQ() evaluates its arguments exactly once, so they
  // can have side effects.

//  EXPECT_EQ(0, c.Increment());
//  EXPECT_EQ(1, c.Increment());
//  EXPECT_EQ(2, c.Increment());
  ConfigIni confFile("Conf/ConfIni/test/application.ini");
//  const char* varString;
//  varString = confFile.ReadString("App", "Name", "Undefined");
  EXPECT_EQ("a", confFile.ReadString("App", "Name", "Undefined"));
}
//int main(int argc, char* argv[])
//{
//	static_cast<void>(argc);
//	static_cast<void>(argv);
//	const char* variableString;
//	int variableInt;
//	double variableFloat;
//
//	ConfigIni confFile("Conf/ConfIni/test/application.ini");
//	variableString = confFile.ReadString("App", "Name", "Undefined");
//	std::cout <<  "[App]  " << "Name=" << variableString << std::endl;
//
//	variableString = confFile.ReadString("Ap", "Name", "Undefined");
//	std::cout <<  "[Ap]   " << "Name=" << variableString << std::endl;
//
//	variableString = confFile.ReadTString("App", "Vendor", "Undefined");
//	std::cout <<  "[App]  " << "Vendor=" << variableString << std::endl;
//
//	confFile.WriteString("NewSession", "key1", "yes");
//
//	variableInt = confFile.ReadInteger("XRE", "EnableProfileMigrator", 0);
//	std::cout <<  "[XRE]  " << "EnableProfileMigrator=" << variableInt << std::endl;
//
//	variableFloat = confFile.ReadFloat("Gecko", "Key2", 0.01);
//	std::cout <<  "[Gecko]  " << "Key2=" << variableFloat << std::endl;
//
//	return EXIT_SUCCESS;
//}

