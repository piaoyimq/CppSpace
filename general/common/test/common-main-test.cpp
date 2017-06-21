/*
 * CommonMainTest.cpp
 *
 *  Created on: 2016-05-21
 *      Author: piaoyimq
 */

#include "ut/googletest/include/gtest/gtest.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "common/include/common.h"
TEST(getNameByPidTest, functionility)
{
	char task_name[50];
	pid_t pid = getpid();
	getNameByPid(pid, task_name);
}

TEST(getPidByNameTest, functionility)
{
	char task_name[50];
	getPidByName(task_name);
}

int main(int argc, char **argv)
{
	std::cout << "Running main() from " << __FILE__ << std::endl;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
