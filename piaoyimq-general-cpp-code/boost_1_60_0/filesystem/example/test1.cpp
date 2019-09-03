// Compile with:
//    if define BOOST_ERROR_CODE_HEADER_ONLY, use:
//        g++  --std=c++11 test1.cpp -lboost_system  -lboost_filesystem
//     if not define BOOST_ERROR_CODE_HEADER_ONLY
//      g++  --std=c++11 test1.cpp -lboost_filesystem

/*
 * test1.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: piaoyimq
 */

/*
 * =====================================================================================
 *
 *       Filename:  filesystem.cpp
 *
 *    Description:  ������������������C++���������boost filesystem ������������������
 *
 *        Version:  1.0
 *        Created:  2009���������08���������17��������� 17���������16���������32���������
 *       Revision:  none
 *       Compiler:  gcc -Wall -Wextra filesystem.cpp -lboost_filesystem-mt
 *
 *         Author:  lgb (LiuGuangBao), easyeagel@gmx.com
 *        Company:  easy99.org
 *
 * =====================================================================================
 */
#define BOOST_ERROR_CODE_HEADER_ONLY    // Note: should keep before #include<boost/filesystem.hpp>
#include<iostream>
#include<fstream>
#include<boost/filesystem.hpp>


namespace bf = boost::filesystem;


int test()
{
	try{


	bf::path path("./tmp");

	bf::path old_cpath = bf::current_path();
	std::cout << "old_cpath=" << old_cpath << std::endl;
	std::cout << "old_cpath.string()=" << old_cpath.string() << std::endl;

	bf::path file_path = path / "file";

    if (bf::exists(file_path))
    {
        std::cout << file_path << " exist \n";
    }
    else
    {
        std::cout << file_path << "not exist \n";
    }

	std::cout << "file_path=" << file_path << std::endl;
	if (bf::exists(path))
	{
	    std::cout << path << " exist \n";
		std::ofstream out(file_path.string());
		if (!out)
		{
			std::cout << "Open failed:1, " << path << std::endl;
			return 1;
		}

		out << "open " << file_path.string() << std::endl;
		out.close();
	}
	else
	{
		std::cout << path << "not exist \n";

		bool ret =bf::create_directory(path);
		if(false ==ret)
		{
			std::cout << "Create directory failed" << std::endl;
		}
		std::ofstream out(file_path.string());
		if (!out)
		{
			std::cout << "Open failed:2, " << file_path << std::endl;
			return 1;
		}

		out << "open " << file_path.string() << std::endl;
	}

	bf::current_path(path);// go to path, equal to "cd path"

	std::cout << "current path=" << path.string() << std::endl;

	if (bf::exists(file_path))
	{
	    std::cout << file_path << " exist \n";
	}
	else
	{
	    std::cout << file_path << "not exist \n";
	}

	if (bf::is_regular_file(bf::path("file")))
	{
		std::cout << bf::path("file") << " is regular file\n";
		std::cout << bf::path("file") << ": size=" << bf::file_size(bf::path("file")) << '\n';
		bf::create_symlink(bf::path("file"), "/tmp/file-symlink");// should make sure /tmp exists.

		std::cout << bf::path("file") << " create symlink" << std::endl;
	}
	else
	{
	    std::cout << file_path << " is not regular file\n";
	}

	bf::path p = bf::read_symlink(bf::path("/tmp/file-symlink"));
	std::cout << "read_symlink=" << p << std::endl;
	bf::current_path(old_cpath);

	bf::remove(file_path);
//	bf::remove_all(path);
	}
	catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }
	return 0;
}


void testRemvoe()
{
    if(bf::remove("/tmp/report"))
    {
        std::cout << "removed" << std::endl;
    }
    else
    {
        std::cout << "removed failed" << std::endl;
    }
}

int main()
{
//    test();
    testRemvoe();
}
