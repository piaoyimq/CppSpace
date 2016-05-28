/*
 * test.cpp
 *
 *  Created on: 2016年5月28日
 *      Author: piaoyimq
 */
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "Serialization.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
using namespace std;
template<typename T>
void EpollServerSocket<T>::ProcessMessage(Socket&clientSocket)
{
	std::string message;
	ReceiveMessage(clientSocket, message);
	T msgStruct = Serialization::DeSerialize(message);
	switch (msgStruct.option)
	{
		case Login:
			SendMessage(clientSocket, "Login_Success");
			break;
		case Logout:
			SendMessage(clientSocket, "Logout_Success");
			DeleteClient(clientSocket.GetSocketfd());
			break;
		case SendToAll:
			SendToAllUsers(msgStruct.message);
			break;
			//messages of other type……
		default:
			break;
	}
}

int main(int argc, char* argv[])
{

}

