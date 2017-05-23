/*
 * json-example2.cpp
 *
 *  Created on: 2016年12月24日
 *      Author: piaoyimq
 */


#include <string>
#include <sstream>
#include <vector>
#include <boost/progress.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
class sms_info
{
	sms_info()
	{
		index_ = 0;
	}
public:
	static std::string INDEX;
	static std::string TO;
	static std::string FROM;
	static std::string MSG;
	static std::string SPLITTER;
	static std::string TAG;
private:
	int index_;
	std::string to_;
	std::string from_;
	std::string msg_;

public:
	sms_info(int index, const std::string& to, const std::string& from, const std::string& msg)
	{
		index_ = index;
		to_ = to;
		from_ = from;
		msg_ = msg;
	}
	int index()
	{
		return index_;
	}
	std::string to()
	{
		return to_;
	}
	std::string from()
	{
		return from_;
	}
	std::string msg()
	{
		return msg_;
	}
};

class json_parser
{
private:
	static std::string ROOTNAME;
public:
	static std::string generate(const std::vector<sms_info>& smss);
	static bool parse(const std::string& s, std::vector<sms_info>& smss);
public:
	static void tester();
};


//
//  json_parser.cpp
//  mongoose
//
//  Created by Alex on 5/26/14.
//  Copyright (c) 2014 Cenbong. All rights reserved.
//

std::string sms_info::INDEX = "index";
std::string sms_info::TO = "to";
std::string sms_info::FROM = "from";
std::string sms_info::MSG = "msg";
std::string sms_info::SPLITTER = ",";
std::string sms_info::TAG = "SMSInfo";

std::string json_parser::ROOTNAME = "smss";
void json_parser::tester()
{
	std::vector<sms_info> smss1;
	for(int i = 0; i < 5; i++)
	{
		int index = i;
		std::string to = "1860000" ;
		std::string from = "1880000" ;
		std::string msg = "这个短信发给Alex，   谢谢。 ";
		smss1.push_back(sms_info(index, to, from, msg));
	}
	std::string s = generate(smss1);
	std::vector<sms_info> smss2;
	parse(s, smss2);
	assert(smss1.size() == smss2.size());
}
std::string json_parser::generate(const std::vector<sms_info>& smss)
{
	boost::property_tree::ptree pt_root;
	boost::property_tree::ptree children;
	boost::property_tree::ptree child;

	for(size_t i = 0; i < smss.size(); i++)
	{
		sms_info sms = smss.at(i);
		child.put(sms_info::INDEX, sms.index());
		child.put(sms_info::TO, sms.to());
		child.put(sms_info::FROM, sms.from());
		child.put(sms_info::MSG, sms.msg());
		children.push_back(std::make_pair("", child));
	}

	pt_root.add_child(ROOTNAME, children);
	std::stringstream ss;
	boost::property_tree::write_json(ss, pt_root);
	std::string s = ss.str();
	std::cout << "s=" << s << std::endl;
	return s;
}
bool json_parser::parse(const std::string& s, std::vector<sms_info>& smss)
{
	std::istringstream iss;
	iss.str(s.c_str());
	boost::property_tree::ptree parser;
	boost::property_tree::json_parser::read_json(iss, parser);
	boost::property_tree::ptree sms_array = parser.get_child(ROOTNAME);
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, sms_array)
	{
		boost::property_tree::ptree p = v.second;
		int index = p.get<int>(sms_info::INDEX);
		std::cout << "index=" << index << std::endl;
		std::string to = p.get<std::string>(sms_info::TO);
		std::string from = p.get<std::string>(sms_info::FROM);
		std::string msg = p.get<std::string>(sms_info::MSG);

		smss.push_back(sms_info(index, to, from, msg));
	}
	return true;
}

int main()
{
	json_parser jo;

	jo.tester();
}
