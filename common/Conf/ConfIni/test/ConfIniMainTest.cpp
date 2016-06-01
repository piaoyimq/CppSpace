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

ConfigIni confFile("Conf/ConfIni/test/application.ini");

TEST(ConfigIni, ReadString) {
   EXPECT_TRUE(strcmp("Firefox", confFile.ReadString("App", "Name", "Undefined")) == 0);
   EXPECT_STREQ("Firefox", confFile.ReadString("App", "Name", "Undefined"));
}

TEST(ConfigIni, ReadTString) {
   EXPECT_STREQ("Mozilla", confFile.ReadTString("App", "Vendor", "Undefined"));
   EXPECT_STREQ("Undefine", confFile.ReadTString("App", "Vndor", "Undefined"));
}

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

