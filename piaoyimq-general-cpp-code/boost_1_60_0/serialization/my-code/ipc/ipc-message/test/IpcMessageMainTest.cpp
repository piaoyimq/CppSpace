/*
 * IpcMessageMain.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */
#include "TestReq.h"
#include <gtest/gtest.h>


std::string str = "get sysinfo";
TestReq testReq(str);

TEST(TestReq, getId)
{
	EXPECT_EQ(InterfaceMessage::ReqId, testReq.getId());
}



//
//	std::cout << "____" << testReq << std::endl;
//	std::ostringstream archiveStream;
//	boost::archive::binary_oarchive archive(archiveStream);
//
//	Message msg;
//	Message msgb;
////	msg.a = 999;
//	msg.serialize(archive);
//
//	std::cout << "__________" << archiveStream.str() << "-----" << std::endl;
//
//	std::istringstream iss(archiveStream.str());
//	boost::archive::binary_iarchive iar(iss);
//
//	msgb.unserialize(iar);
//
//	std::cout << "____" << msgb.a << " ____" << msgb.sequenceId_.value_ << std::endl;


int main(int argc, char **argv)
{
	std::cout << "Running main() from " << __FILE__ << std::endl;

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
