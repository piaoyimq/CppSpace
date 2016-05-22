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
	 * �����ļ���Ŀ
	 * ��_name��_comment��Ϊ�մ�ʱ����ʾ���У�
	 * ��_nameΪ�մ�����_comment��Ϊ�մ�ʱ����ʾ����ע���У�
	 * �����и�ʽΪ��
	 *		_name=_value # _comment
	 * ����_value��_comment����ʡ�ԣ��м�����һ�������հ��ַ���
	 * ע�⣺Ϊ������������������У�
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
	 * һ��������
	 * Э��ConfigFile::operator []�����ֶ���д������
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
	 * ���������
	 */
	ConfigFile() {}
	virtual ~ConfigFile();

public:
	/*
	 * ����
	 */
	// ��ĳ���ļ�����������Ϣ
	bool load(const string& file, int om = OM_READ);
	// ��������Ϣ���浽�ļ�
	void save();
	// ���������󣨲����浽�ļ����������������Ϣ��
	void clear();

	// ��ȡ������������Ϣ
	string get(const string& name, const string& default_val = "");
	bool get(const string& name, string& buf);
	void set(const string& name, const string& value);
	void set(const string& name, const string& value, const string& comment);
	ValueWrap operator [](const string& name) { return ValueWrap(name, *this); }

private:
	/*
	 * ��ֹ�Ĳ���
	 */
	ConfigFile(const ConfigFile& rhs);
	ConfigFile& operator =(const ConfigFile& rhs);

private:
	/*
	 * �ڲ�ʵ��
	 */
	bool do_load(FILE* fp);
	Node* find(const string& name, const string& default_val = "");

protected:
	/*
	 * ���ݳ�Ա
	 */
	string _file;
	int _om;
	deque<Node*> _deque;
};

#endif // #ifndef __CONFiG_FILE_H__217848912894892389489238948923898598342589__