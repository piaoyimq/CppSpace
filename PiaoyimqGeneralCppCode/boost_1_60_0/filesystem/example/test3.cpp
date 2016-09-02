/*
 * test3.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: piaoyimq
 */

#include <boost/filesystem.hpp>   // 包含所有需要的 Boost.Filesystem 声明
#include <iostream>               // 使用 std::cout
#include<fstream>
namespace fs = boost::filesystem;
// 宏FSTEST：测试f的成员函数，输出成员函数名和结果
#define FSTEST(x) std::cout << #x <<" : " << f.x << std::endl

//目录迭代器, basic_directory_iterator
void findFile(const fs::path & dir_path)
{
	if (!fs::exists(dir_path))
		return;
	fs::directory_iterator end_itr; // 缺省构造生成一个结束迭代器
	for (fs::directory_iterator itr(dir_path); itr != end_itr; ++itr)
	{
		if (fs::is_directory(itr->status()))
		{
			findFile(itr->path()); //递归查找
		}
		else
		{
			std::cout << *itr << std::endl;
		}
	}
}

//递归遍历目录的迭代器, basic_recursive_directory_iterator
void recursiveFindFile(const fs::path & dir_path)
{
	fs::recursive_directory_iterator end_itr; // 缺省构造生成一个结束迭代器
	for (fs::recursive_directory_iterator itr(dir_path); itr != end_itr; ++itr)
	{
		std::cout << itr.level() << *itr << std::endl;
	}
}

void errorControl()
{
	try
	{
		boost::filesystem::path path("C:\\src\\hdbase\\j1");
		boost::filesystem::path path2("C:\\src\\hdbase\\j2");
		boost::filesystem::rename(path, path2);
	} catch (boost::filesystem::filesystem_error e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

int main()
{
	fs::path f("./folder1/folder2/folder3/filename.txt");

	FSTEST(string());
	FSTEST(root_name());
	FSTEST(root_directory());
	FSTEST(root_path());
	FSTEST(leaf());
	FSTEST(relative_path());
	FSTEST(filename());
	FSTEST(parent_path());
	FSTEST(stem());
	FSTEST(extension());
//	FSTEST(remove_filename());
	FSTEST(replace_extension("new"));
	char buf[] = "hello";
	FSTEST(append(buf, buf+sizeof(buf)));
	FSTEST(parent_path());

	std::cout << "basename=" << fs::basename(f) << std::endl;

//	create_directory(f.parent_path());//mkdir tmp-file
	create_directories(f.parent_path());     //mkdir -p
	std::ofstream out(f.string());
	if (!out)
	{
		std::cout << "Open failed:1, " << f << std::endl;
		return 1;
	}
	out << "A test file\n";

	std::cout << "\nfindFile:\n\n";
	findFile(".");

	std::cout << "\nrecursiveFindFile:\n\n";
	recursiveFindFile(".");

	std::cout << "\nerrorControl:\n\n";
	errorControl();

	return 0;
}

