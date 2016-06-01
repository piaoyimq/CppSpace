#ifndef __CONFiG_FILE_H__217848912894892389489238948923898598342589__
#define __CONFiG_FILE_H__217848912894892389489238948923898598342589__

#include <cstdio>
#include <iostream>
#include <string>
#include <deque>
using namespace std;

class ConfigFile
{
public:
	enum OpenMode 
	{ 
		OM_READ			= 0x00000001, 
		OM_WRITE		= 0x00000002,
		OM_CREATE		= 0x00000010
	};

protected:
	/*
	 * 配置文件条目
	 * 当_name和_comment均为空串时，表示空行；
	 * 当_name为空串，但_comment不为空串时，表示整行注释行；
	 * 其它行格式为：
	 *		_name=_value # _comment
	 * 其中_value和_comment可以省略，中间允许一个或多个空白字符；
	 * 注意：为方便起见，不允许续行；
	 */
	class Node
	{
		friend class ConfigFile;
	public:
		Node() : _next(NULL) {}
		Node(const string& name, const string& value, const string& comment)
			: _name(name), _value(value), _comment(comment), _next(NULL) {}
	private:
		string _name;
		string _value;
		string _comment;
		Node* _next;
	};

	/*
	 * 一个代理类
	 * 协助ConfigFile::operator []以区分读或写操作；
	 */
	class ValueWrap
	{
		friend class ConfigFile;
	public:
		ValueWrap(string name, ConfigFile& cf) : _name(name), _cf(cf) {}
		ValueWrap& operator =(const string& value) { _cf.set(_name, value); return *this; }
		ValueWrap& operator =(const ValueWrap& rhs) { _cf.set(_name, (string)rhs); return *this; }
		operator string() const { return _cf.get(_name); }
		friend ostream& operator <<(ostream& os, const ValueWrap& vw) { os << (string)vw; return os; }
	private:
		string _name;
		ConfigFile& _cf;
	};

public:
	/*
	 * 构造和析构
	 */
	ConfigFile() {}
	virtual ~ConfigFile();

public:
	/*
	 * 操作
	 */
	// 从某个文件加载配置信息
	bool load(const string& file, int om = OM_READ);
	// 将配置信息保存到文件
	void save();
	// 清空这个对象（不保存到文件，仅仅清空配置信息）
	void clear();

	// 获取或设置配置信息
	string get(const string& name, const string& default_val = "");
	bool get(const string& name, string& buf);
	void set(const string& name, const string& value);
	void set(const string& name, const string& value, const string& comment);
	ValueWrap operator [](const string& name) { return ValueWrap(name, *this); }

private:
	/*
	 * 禁止的操作
	 */
	ConfigFile(const ConfigFile& rhs);
	ConfigFile& operator =(const ConfigFile& rhs);

private:
	/*
	 * 内部实现
	 */
	bool do_load(FILE* fp);
	Node* find(const string& name, const string& default_val = "");

protected:
	/*
	 * 数据成员
	 */
	string _file;
	int _om;
	deque<Node*> _deque;
};

#endif // #ifndef __CONFiG_FILE_H__217848912894892389489238948923898598342589__