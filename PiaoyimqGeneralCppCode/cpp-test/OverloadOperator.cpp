/*
 * OverloadOperator.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: ezhweib
 */

#include <iostream>
#include <cstring>
#include <iomanip>
#include <cassert>
#include <utility> //move
using namespace std;

class String {
public:
    String();
    String(const char *);
    String(const String &);
    String(String&& str);

    ~String();

    String& operator=(const char *);
    String& operator=(const String &);

    bool operator==(const char *);
    bool operator==(const String &);

    char &operator[](int);

    String operator+(const char *);
    String operator+(const char);
    String operator+(const String &);

    String &operator +=(const char *);
    String &operator +=(const char);
    String &operator +=(const String &);

    int size() {
        return _size;
    }
    char *c_str() {
        return _string;
    }

    friend istream &operator>>(istream &cin, String &str);
private:
    int _size;
    char *_string;
};

String::String() {
    _size = 0;
    _string = NULL;

    cout << "String() call" << endl;
}

String::String(const char *str) {
    if (!str) {
        _size = 0;
        _string = NULL;
    } else {
        _size = strlen(str);
        _string = new char[_size + 1];
        strcpy(_string, str);
        _string[_size] = 0;
    }

    cout << "String(const char *str) call" << endl;
}

String::String(String && str) {
    _size = str._size;
    _string = str._string;
    str._string = NULL;
}

String::String(const String &str) {
    if (!str._size) {
        _size = 0;
        _string = NULL;
    } else {
        _size = str._size;
        _string = new char[_size + 1];
        strcpy(_string, str._string);
        _string[_size] = 0;
    }

    cout << "String(const String &str) call" << endl;
}

String::~String() {
    if (_string)
        delete _string;

    cout << "~String() call" << endl;
}

String &String::operator=(const char *str) {
    //Èç¹ûstrÓëµ±Ç°×Ö·û´®ÊÇÏàÍ¬µÄ
    if (_string == str)
        return *this;
    if (!str) {
        _size = 0;
        if (_string) {
            delete[] _string;
            _string = NULL;
        }
    } else {
        _size = strlen(str);
        if (_string)
            delete[] _string;
        _string = new char[_size + 1];
        strcpy(_string, str);
        _string[_size] = 0;
    }
    return *this;
}

String &String::operator=(const String &str) {
    cout << "operator =(const String &str) call" << endl;
    if (_string == str._string)
        return *this;
    if (!str._size) {
        _size = 0;
        if (_string) {
            delete[] _string;
            _string = NULL;
        }
    } else {
        _size = str._size;
        if (_string)
            delete[] _string;
        _string = new char[_size + 1];
        strcpy(_string, str._string);
        _string[_size] = 0;
    }
    return *this;
}

bool String::operator==(const char *str) {
    if (strcmp(str, _string) == 0)
        return true;
    return false;
}

bool String::operator ==(const String &str) {
    if (strcmp(str._string, _string) == 0)
        return true;
    return false;
}

char &String::operator[](int index) {
    assert(index >= 0 && index < _size);
    return _string[index];
}

String String::operator+(const char *str) {
    assert(_string && str);

    String str_temp(*this);

    str_temp += str;
    return std::move(str_temp);
}

String String::operator+(const char x) {
    assert(_string);

    String str_temp(*this);

    str_temp += x;
    return std::move(str_temp);
}

String String::operator+(const String &str) {
    assert(_string && str._string);

    String str_temp(*this);

    str_temp += str;
    return std::move(str_temp);
}

String &String::operator+=(const char x) {
    assert(_string);

    char *temp_string = new char[_size + 2];
    strcpy(temp_string, _string);
    temp_string[_size] = x;
    _size += 1;
    temp_string[_size] = 0;
    delete[] _string;
    _string = temp_string;
    return *this;
}

String &String::operator+=(const char *str) {
    assert(_string && str);

    int d_size = strlen(str);
    char *temp_string = new char[_size + d_size + 1];
    strcpy(temp_string, _string);
    strcpy(temp_string + _size, str);
    _size += d_size;
    temp_string[_size] = 0;
    delete[] _string;
    _string = temp_string;
    return *this;
}

String &String::operator+=(const String &str) {
    assert(_string && str._string);

    int d_size = str._size;
    char *temp_string = new char[_size + d_size + 1];
    strcpy(temp_string, _string);
    strcpy(temp_string + _size, str._string);
    _size += d_size;
    temp_string[_size] = 0;
    delete[] _string;
    _string = temp_string;
    return *this;
}

istream &operator>>(istream &cin, String &str) {
    const int limit_string_size = 4096;

    str._string = new char[limit_string_size];

    cin >> setw(limit_string_size) >> str._string;
    str._size = strlen(str._string);
    return cin;
}

ostream &operator<<(ostream &cout, String &str) {
    return cout << str.c_str();
}

int main(int argc, char *argv[]) {
    //int ia[7] = {0, 1, 1, 2, 3, 5, 8};

    ////vector<int> ivec = {0, 1, 1, 2, 3, 5, 8}; ³ö´í
    //vector<int> ivec(ia, ia + 7);
    ////vector<string> svec = ivec;
    //vector<string> svec(10, string("null"));
    //for (int i = 0; i < svec.size(); i ++)
    //  cout << svec[i] << endl;

    String str1("Huang"), str2("Chen");

    cout << "Please input a string:\n";
    cin >> str1;
    str1 = str1 + 'c' + "CC";
    str1 += 'a';
    cout << str1 << " " << str2 << endl;
    cout << (str2 == "Chen") << endl;
    return 0;
}

//String(const char *str)  call
//String(const char *str)  call
//String() call
//String(const String &str)  call
//~String()
//String() call
//String(const String &str)  call
//~String()
//~String()
//~String()
//~String()
//~String()



#if 0


#include <iostream>
#include <string.h>
using namespace std;

class MyString
{
public:
    MyString();
    ~MyString(){}
    MyString(const char* str);
    MyString(MyString & str);
    MyString & operator=(const MyString & str);
    MyString & operator=(const char * c_str);

    char operator[](const unsigned int index);

    MyString operator+(const MyString & str);
    MyString operator+(const char *);
    MyString operator+=(const MyString & str);
    MyString operator+=(const char * c_str);
    bool operator>(const MyString &str);
    bool operator>(const char * c_str);
    bool operator>=(const MyString &str);
    bool operator>=(const char * c_str);
    bool operator<(const MyString &str);
    bool operator<(const char * c_str);
    bool operator<=(const MyString & str);
    bool operator<=(const char * c_str);
    bool operator==(const MyString & str);
    bool operator==(const char * c_str);
    bool operator!=(const MyString & str);
    bool operator!=(const char * c_str);

    friend ostream & operator<<(ostream &ostr, MyString & str);
    friend istream & operator>>(istream &istr, MyString & str);
    friend MyString operator+(char * c_str, MyString & str);
    friend MyString operator!=(char * c_str, MyString & str);
    friend bool operator>(const char * c_str, const MyString & str);
    friend bool operator>=(const char * c_str, const MyString &str);
    friend bool operator<(const char * c_str, const MyString &str);
    friend bool operator<=(const char *c_str, const MyString & str);
    friend bool operator==(const char * c_str, const MyString & str);
    friend bool operator!=(const char * c_str, const MyString & str);

public:
    char* getString() const;
    int getSize() const;

private:
    char * m_string;
    unsigned int m_size;
};

int MyString::getSize() const
{
    return m_size;
}

char * MyString::getString() const
{
    return m_string;
}

MyString::MyString()
{
    m_string = new char[1];
    m_string[0] = '';
    m_size = 0;
}

MyString::MyString(const char * c_str)
{
    m_size = (unsigned int)strlen(c_str);
    m_string = new char[m_size+1];
    strcpy(m_string, c_str);
}

MyString::MyString(MyString &str)
{
    m_size = str.getSize();
    m_string = new char (m_size + 1);
    strcpy(m_string, str.getString());
}

MyString & MyString::operator =(const char *str)
{
    if (strcmp(str, m_string) == 0)
        return *this;
    if (m_string)
        delete [] m_string;
    m_size = (unsigned int)strlen(str);
    m_string = new char(m_size + 1);
    strcpy(m_string, str);
    return *this;
}

MyString & MyString::operator=(const MyString & str)
{
    if (strcmp(str.getString(), m_string) == 0)
    {
        return * this;
    }
    if (m_string)
        delete m_string;
    m_size = str.getSize();
    m_string = new char (m_size + 1);
    strcpy(m_string, str.getString());
    return * this;
}


char MyString::operator [](unsigned int index)
{
    if (index > m_size)
    {
        index = m_size - 1;
    }
    return m_string[index];
}

MyString MyString::operator+(const MyString & str)
{
    int i = m_size + str.getSize() + 1;
    char * p = new char (i);
    strcpy(p, m_string);
    strcat(p, str.getString());
    return MyString(p);
}

MyString MyString::operator +(const char * c_str)
{
    size_t i = strlen(c_str) + m_size + 1;
    char * p = new char ((int)i);
    strcpy(p, m_string);
    strcat(p, c_str);
    return MyString(p);
}

MyString MyString::operator +=(const char *str)
{
    strcpy(m_string, str);
    return * this;
}

MyString MyString::operator +=(const MyString &str)
{
    strcpy(m_string, str.getString());
    return * this;
}

bool MyString::operator >(const char *str)
{
    return (strcmp(m_string, str) < 0);
}

bool MyString::operator>(const MyString & str)
{
    return (strcmp(m_string, str.getString())>0);
}

bool MyString::operator>=(const char * c_str)
{
    return (strcmp(m_string, c_str)>=0);
}

bool MyString::operator>=(const MyString & str)
{
    return (strcmp(m_string, str.getString())>=0);
}

bool MyString::operator<(const char * c_str)
{
    return (strcmp(m_string, c_str)<0);
}

bool MyString::operator<(const MyString & str)
{
    return (strcmp(m_string, str.getString()) < 0);
}

bool MyString::operator<=(const char * c_str)
{
    return (strcmp(m_string, c_str)<=0);
}

bool MyString::operator<=(const MyString &str)
{
    return (strcmp(m_string, str.getString()) == 0);
}

bool MyString::operator==(const char * c_str)
{
    return (strcmp(m_string, c_str)==0);
}

bool MyString::operator==(const MyString & str)
{
    return (strcmp(m_string, str.getString()) == 0);
}

bool MyString::operator !=(const char * c_str)
{
    return (strcmp(m_string, c_str) != 0);
}

bool MyString::operator !=(const MyString &str)
{
    return (strcmp(m_string, str.getString()) != 0);
}

// Friend Function
ostream & operator<<(ostream &ostr, MyString & str)
{
    ostr<<str.getString();
    return ostr;
}

istream & operator>>(istream & istr, MyString &str)
{
    char tmp[255];
    istr>>tmp;
    str = tmp;
    return istr;
}

MyString operator+(char * c_str, MyString & str)
{
    int new_size = strlen(c_str) + str.getSize() + 1;
    char * newStr = new char [new_size];
    strcpy (newStr, c_str);
    strcat (newStr, str.getString());

    return MyString(newStr);
}

bool operator!=(const char * c_str, MyString & str)
{
    return (strcmp(c_str, str.getString()) != 0);
}

bool operator>(const char * c_str, const MyString & str)
{
    return (strcmp(c_str, str.getString()) > 0);
}
bool operator>=(const char * c_str, const MyString &str)
{
    return (strcmp(c_str, str.getString()) > 0);
}
bool operator<(const char * c_str, const MyString &str)
{
    return (strcmp(c_str, str.getString()) < 0);
}
bool operator<=(const char *c_str, const MyString & str)
{
    return (strcmp(c_str, str.getString()) <= 0);
}
bool operator==(const char * c_str, const MyString & str)
{
    return (strcmp(c_str, str.getString()) == 0);
}
bool operator!=(const char * c_str, const MyString & str)
{
    return (strcmp(c_str, str.getString()) != 0);
}

bool operator>(char * c_str, const MyString & str)
{
    return (strcmp(c_str, str.getString()) > 0);
}

#endif
