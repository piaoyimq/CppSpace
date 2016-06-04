/*
 * IpcMessageMain.cpp
 *
 *  Created on: 2016年6月4日
 *      Author: root
 */

int main()
{
	std::ostringstream archiveStream;
	boost::archive::binary_oarchive archive(archiveStream);

	Message msg;
	Message msgb;
	msg.a = 999;
	msg.serialize(archive);

	std::cout << "__________" << archiveStream.str() << "-----" << std::endl;

	std::istringstream iss(archiveStream.str());
	boost::archive::binary_iarchive iar(iss);

	msgb.unserialize(iar);

	std::cout << "____" << msgb.a << " ____" << msgb.sequenceId_.value_ << std::endl;
}
