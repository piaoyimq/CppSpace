#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include "string_ex.h"
#include "config_file.h"
using namespace std;

const int MAX_BUF_SIZE = 1024;

bool ConfigFile::load(const string& file, int om /*= OM_READ*/)
{
	if (file.empty())
	{
		cerr << "Error: ConfigFile::load(): the file name is null string!" << endl;
		return false;
	}

	_file = file;
	_om = om;
	clear();

	if (_om & OM_CREATE)
	{
		FILE* fp = fopen(file.c_str(), "a+b"); // 不存在就创建；然后打开
		if (fp == NULL)
		{
			cerr << "Error: ConfigFile::load(): create file failed!" << endl;
			return false;
		}
		fclose(fp);
	}
	
	FILE* fp = fopen(file.c_str(), "rb");
	if (fp == NULL)
	{
		cerr << "Error: ConfigFile::load(): open file to read failed!" << endl;
		return false;
	}
	do_load(fp);
	fclose(fp);
	return true;
}

ConfigFile::~ConfigFile()
{
	clear();
}

string ConfigFile::get(const string& name, const string& default_val /*= ""*/)
{
	Node* p = find(name, default_val);
	assert(p != NULL);
	p->_value = default_val;
	return p->_value;
}

bool ConfigFile::get(const string& name, string& buf)
{
	deque<Node*>::iterator it;
	for (it = _deque.begin(); it != _deque.end(); ++it)
	{
		if ((*it)->_name == name)
		{
			buf = (*it)->_value;
			return true;
		}
	}
	return false;
}

void ConfigFile::set(const string& name, const string& value)
{
	Node* p = find(name);
	assert(p != NULL);
	p->_value = value;
}

void ConfigFile::set(const string& name, const string& value, const string& comment)
{
	Node* p = find(name);
	assert(p != NULL);
	p->_value = value;
	p->_comment = comment;
}

void ConfigFile::save()
{
	if (!(_om & OM_WRITE))
	{
		cerr << "Error: ConfigFile::save(): the config file not be saved for only OM_READ mode!" << endl;
		return;
	}

	FILE* fp = fopen(_file.c_str(), "w+b"); // 不存在就创建，存在就截短为0长度；然后打开
	if (fp == NULL)
	{
		cerr << "Error: ConfigFile::save(): open file to write failed!" << endl;
		return;
	}
	
	deque<Node*>::iterator it;
	for (it = _deque.begin(); it != _deque.end(); ++it)
	{
		Node* p = *it;

		string_ex buf;
		if (p->_name == "" && p->_comment == "")
		{
			buf = "\r\n";
		}
		else if (p->_name == "")
		{
			buf.format("# %s\r\n",
				p->_comment.c_str()
			);
		}
		else if (p->_comment == "")
		{
			string_ex s(p->_value.c_str());
			s.alltrim();
			if (s.length() != p->_value.length())
			{
				buf.format("%s=\"%s\"\r\n",
					p->_name.c_str(),
					p->_value.c_str()
				);
			}
			else
			{
				buf.format("%s=%s\r\n",
					p->_name.c_str(),
					p->_value.c_str()
				);
			}
		}
		else
		{
			string_ex s(p->_value.c_str());
			s.alltrim();
			if (s.length() != p->_value.length())
			{
				buf.format("%s=\"%s\" # %s\r\n",
					p->_name.c_str(),
					p->_value.c_str(),
					p->_comment.c_str()
				);
			}
			else
			{
				buf.format("%s=%s # %s\r\n",
					p->_name.c_str(),
					p->_value.c_str(),
					p->_comment.c_str()
				);
			}
		}

		size_t written = fwrite(buf.c_str(), 1, buf.length(), fp);
		if (written < buf.length())
		{
			cerr << "Error: ConfigFile::save(): write the config file error!" << endl;
			return;
		}
	}

	fclose(fp);
}

void ConfigFile::clear()
{
	deque<Node*>::iterator it;
	for (it = _deque.begin(); it != _deque.end(); ++it)
	{
		delete (*it);
	}
	_deque.clear();
}

bool ConfigFile::do_load(FILE* fp)
{
	assert(fp != NULL);
	char buf[MAX_BUF_SIZE];

	while (fgets(buf, sizeof(buf), fp))
	{
		Node* node = new Node();
		string_ex st;
		string_ex s(buf);

		s.alltrim();
		size_t pos = s.find('#');
		if (pos != string_ex::npos) 
		{
			st = string(s, pos + 1, s.length() - pos - 1);
			st.alltrim();
			node->_comment = st;
			s = s.left(pos);
			s.alltrim();
		}

		pos = s.find('=');
		if (pos == string_ex::npos) // 此行没有=
		{
			if (!s.empty())
			{
				cerr << "Error: ConfigFile::do_load(): no = in the non-empty line!" << endl;
			}
		}
		else
		{
			if (pos == 0)
			{
				cerr << "Error: ConfigFile::do_load(): no name in the non-empty line!" << endl;
			}
			else
			{
				st = s.left(pos);
				st.alltrim();
				node->_name = st;
				st = s.right(s.length() - pos - 1);
				st.alltrim();
				if (st.length() >= 2 && st[0] == '\"' && st[st.length() - 1] == '\"')
				{
					st = st.substr(1, st.length() - 2);
				}
				node->_value = st;
			}	
		}

		_deque.push_back(node);
	}

	return true;
}

ConfigFile::Node* ConfigFile::find(const string& name, const string& default_val /*= ""*/)
{
	deque<Node*>::iterator it;
	for (it = _deque.begin(); it != _deque.end(); ++it)
	{
		if ((*it)->_name == name)
		{
			return *it;
		}
	}

	Node* p = new Node(name, default_val, "");
	_deque.push_back(p);
	return p;
}