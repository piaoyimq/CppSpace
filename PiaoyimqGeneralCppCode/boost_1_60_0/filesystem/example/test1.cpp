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
 *    Description:  简单C++，boost filesystem 指南
 *
 *        Version:  1.0
 *        Created:  2009年08月17日 17时16分32秒
 *       Revision:  none
 *       Compiler:  gcc -Wall -Wextra filesystem.cpp -lboost_filesystem-mt
 *
 *         Author:  lgb (LiuGuangBao), easyeagel@gmx.com
 *        Company:  easy99.org
 *
 * =====================================================================================
 */

#include<iostream>
#include<fstream>
#include<boost/filesystem.hpp>

int main()
{
	try{
	namespace bf = boost::filesystem;
	//简单别名

	//filesystem中最基本的类型
	bf::path path("./tmp");

	//对当前的目录的操作
	bf::path old_cpath = bf::current_path(); //取得当前目录
	std::cout << "old_cpath=" << old_cpath << std::endl;
	std::cout << "old_cpath.string()=" << old_cpath.string() << std::endl;

	bf::path file_path = path / "file"; //path重载了 / 运算符

	std::cout << "file_path=" << file_path << std::endl;
	//判断文件存在性
	if (bf::exists(path))
	{
		std::ofstream out(file_path.string());
		if (!out)
		{
			std::cout << "Open failed:1, " << path << std::endl;
			return 1;
		}

		out << "一个测试文件\n";
	}
	else
	{
		std::cout << path << "不存在\n";
		//目录不存在，创建
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

		out << "一个测试文件\n";
	}

	bf::current_path(path); //设置当前为/home

	if (bf::is_regular_file(file_path))
	{
		std::cout << path << "是普通文件\n";
		std::cout << path << ": 大小为" << bf::file_size(path) << '\n';
		bf::create_symlink(file_path, "/tmp/test/file-symlink");
	}

	bf::current_path(old_cpath);

//	bf::remove(file_path); //删除文件file_path
//	bf::remove_all(path); //递归地删除
	}
	catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }
	return 0;
}

