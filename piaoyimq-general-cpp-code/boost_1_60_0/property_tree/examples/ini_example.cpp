/*
 * ini_example.cpp
 *
 *  Created on: 2016年12月24日
 *      Author: piaoyimq
 */

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

int main()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("./over-lay.ini", pt);  // 打开读文件

	pt.put<std::string>("OVERLAY.OverlayFontName","宋体");  // 写字段
	pt.put<std::string>("OVERLAY.AuthorName","张三");
	pt.put<std::string>("Other.Name","nobody");
	pt.put<std::string>("Other.test","value");

	std::cout << pt.get<std::string>("OVERLAY.OverlayFontName") << std::endl;  // 读字段

	boost::property_tree::ini_parser::write_ini("./over-lay.ini",pt); // 写到文件

	return 0;
}


