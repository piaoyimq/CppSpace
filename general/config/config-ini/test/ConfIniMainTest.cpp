/*
 * ConfIniMainTest.cpp
 *
 *  Created on: 2016-05-22
 *      Author: piaoyimq
 */
#include "config/config-ini/include/ConfigIni.h"
#include <gtest/gtest.h>
#include <stdlib.h>
#include <iostream>

ConfigIni confFile("general/config/config-ini/test/application.ini");

TEST(ConfigIni, ReadString)
{
	EXPECT_TRUE(strcmp("Firefox", confFile.ReadString("App", "Name", "Undefined")) == 0);
	EXPECT_STREQ("Firefox", confFile.ReadString("App", "Name", "Undefined"));
}

TEST(ConfigIni, ReadTString)
{
	EXPECT_STREQ("Mozilla", confFile.ReadTString("App", "Vendor", "Undefined"));
	EXPECT_STREQ("Undefined", confFile.ReadTString("App", "Vndor", "Undefined"));
}

int main(int argc, char **argv)
{
	std::cout << "Running main() from " << __FILE__ << std::endl;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
