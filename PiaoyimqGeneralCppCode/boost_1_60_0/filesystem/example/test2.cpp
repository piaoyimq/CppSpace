/*
 * test2.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: ezhweib
 */

#include<iostream>
#include<boost/filesystem.hpp>

int main()
{
	namespace fs = boost::filesystem;
	try
	{
		fs::path currentPath = fs::current_path();	//Get current path.
		std::cout << "current_path= " << currentPath << std::endl;

		fs::create_directory(currentPath / "tmp");
		fs::create_directories(currentPath / "a/c/d");



	} catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

