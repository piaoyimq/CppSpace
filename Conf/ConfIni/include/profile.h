/////////////////////////////////////////////////////////////////////
#ifndef __Profile_H__
#define __Profile_H__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ctype.h>

/////////////////////////////////////////////////////////////////////
// Platform related issues


/////////////////////////////////////////////////////////////////////
// Constant table

// Performance
const int constLINESIZE = 2048;
const int constLINEBUFFERSIZE = constLINESIZE + 1;
//const int constTOKENSIZE         = 32;
//const int constTOKENSIZE         = 100;
const int constTOKENSIZE = 2048;
const int constTOKENBUFFERSIZE = constTOKENSIZE + 1;
#define CONTENT_MAX_READLEN	2048	//value max length

/////2000/5/30 add begin
const int constSECTIONMAX = 100; //search all section max number
const int constCONTENTMAXSIZE = 100; //section name ,key or value max size
const int constKEYVALUEMAX = 300; //search all key or value max number of specified section
/////2000/5/30 add end

// Delims
const char delimENDOFLINE = '\n';
const char delimLEFTBRACKET = '[';
const char delimRIGHTBRACKET = ']';
const char delimASSIGNMENT = '=';
const char delimSPACER = ' ';
const char delimREMARK = '#';

class TProfile
{
	std::fstream Profile;
	std::streampos posOldLine;
	char Buffer[constLINEBUFFERSIZE];
	char* bufferPtr;
	char Token[constTOKENBUFFERSIZE];

//add 
private:
	int mLesslen;

public:

	/////2000/3/30 add begin

	//return 1 is true,0 is false
	int ValueExists(const char* pSection, const char* pKey);

	//read  all value of keys in specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSectionValues(const char* pSection, char** &pStr);

	//erase specified section all content,include key ,value and specified section name itself 
	void EraseSection(const char* pSection);

	//delete specified key and it's value in specified section
	void DeleteKey(const char* pSection, const char* pKey);

	//from pContent to return section name(not incldue "[]"),if not found,pSection return NULL.
	//1 is true,0 is false
	int ReturnSection(char* pContent, char* pSection);

	//from pContent to return Key name,if not found,pKey return NULL.
	//1 is true,0 is false
	int ReturnKey(char* pContent, char* pKey);

	//from pContent to return Value,if not found,pValue return NULL.
	//1 is true,0 is false
	int ReturnValue(char* pContent, char* pValue);

	//read  all key names of specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSectionKeys(const char* pSection, char** & pStr);

	//read all sections name
	//1 is true,0 is false
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSections(char** & pSections);

	/////2000/5/30 add end

	TProfile()
	{
		mLesslen = 0;
	}
	~TProfile()
	{
		Profile.close();
		mLesslen = 0;
	}
	TProfile(char* filename) :
			Profile(filename, std::ios::in | std::ios::out)
	{
	}

	std::fstream& File()
	{
		return Profile;
	}
	std::fstream& Open(char* filename, std::ios::openmode mode = std::ios::in | std::ios::out)
	{
		Profile.open(filename, mode);
		return Profile;
	}
	void Close()
	{
		Profile.close();
	}

	// 0 - No ; 1 - Yes
	int IsEof()
	{
		return Profile.eof();
	}
	int IsGood()
	{
		return Profile.good();
	}

	// Interfaces - Read routines
	// Read 'value' from existing 'section', 'name'.
	// 0 - fail ; 1 - succeed
	int ReadChar(const char* secname, const char* name, int index, char* buf);
	int ReadString(const char* secname, const char* name, int index, char* buf);
	int ReadTString(const char* secname, const char* name, int index, char* buf);
	int ReadShort(const char* secname, const char* name, int index, short* buf);
	int ReadUshort(const char* secname, const char* name, int index, unsigned short* buf);
	int ReadInt(const char* secname, const char* name, int index, int* buf);
	int ReadUint(const char* secname, const char* name, int index, unsigned int* buf);
	int ReadLong(const char* secname, const char* name, int index, long* buf);
	int ReadUlong(const char* secname, const char* name, int index, unsigned long* buf);
	int ReadFloat(const char* secname, const char* name, int index, float* buf);
	int ReadDouble(const char* secname, const char* name, int index, double* buf);

	int ReadOct(const char* secname, const char* name, int index, unsigned short* buf);
	int ReadOct(const char* secname, const char* name, int index, unsigned int* buf);
	int ReadOct(const char* secname, const char* name, int index, unsigned long* buf);

	int ReadHex(const char* secname, const char* name, int index, unsigned short* buf);
	int ReadHex(const char* secname, const char* name, int index, unsigned int* buf);
	int ReadHex(const char* secname, const char* name, int index, unsigned long* buf);

	// Interfaces - Write routines
	// Write 'value' into existing 'section', 'name'.
	// 0 - fail ; 1 - succeed
	int WriteChar(const char* secname, const char* name, int index, char buf);
	int WriteString(const char* secname, const char* name, int index, const char* buf);
	int WriteShort(const char* secname, const char* name, int index, short buf);
	int WriteUshort(const char* secname, const char* name, int index, unsigned short buf);
	int WriteInt(const char* secname, const char* name, int index, int buf);
	int WriteUint(const char* secname, const char* name, int index, unsigned int buf);
	int WriteLong(const char* secname, const char* name, int index, long buf);
	int WriteUlong(const char* secname, const char* name, int index, unsigned long buf);
	int WriteFloat(const char* secname, const char* name, int index, float buf);
	int WriteDouble(const char* secname, const char* name, int index, double buf);

	int WriteOct(const char* secname, const char* name, int index, unsigned short buf);
	int WriteOct(const char* secname, const char* name, int index, unsigned int buf);
	int WriteOct(const char* secname, const char* name, int index, unsigned long buf);

	int WriteHex(const char* secname, const char* name, int index, unsigned short buf);
	int WriteHex(const char* secname, const char* name, int index, unsigned int buf);
	int WriteHex(const char* secname, const char* name, int index, unsigned long buf);

	// Interfaces - New & Add routine
	// 0 - fail ; 1 - succeed

	// Create an unexisting 'section' (in the end).
	int NewSection(const char* secname);

	// Create an unexisting 'name' of an existing 'section'.
	// Duplicated 'name' case may exist.
	int NewNameOfSection(const char* secname, const char* name);

	// Create unexisting 'name' and 'section' (in the end).
	int NewName(const char* secname, const char* name);

	// Create unexisting 'value', 'name', 'section' (in the end).
	int NewValue(const char* secname, const char* name, const char* value);

	// Append a 'value' to existing 'section', 'name'.
	// Duplicated 'value' case may exist.
	int AddValue(const char* secname, const char* name, const char* value);

	// Append a 'value' to existing 'section', 'name'; the same as AddValue().
	int NewValueOfName(const char* secname, const char* name,  const char* value);

	// 0 - fail; 1 - succeed
	int FindSection(const char* secname);

	/*Chen move from private*/
	int FindName(const char* secname, const char* name);
private:

	// 0 - fail; 1 - succeed
	int FindNameInSection(const char* name);

//	int FindName( const char* secname, const char* name );

	int IsName(char* buffer, const char* name);
	int IsSection(char* buffer, const char* secname);

	// 0 - fail; NOT 0 - a valid pointer of next value
	char* FirstValue(char* buffer);
	char* NextValue(char* buffer);
	char* FindValue(const char* secname, const char* name, int index);
	char* FindTValue(const char* secname, const char* name, int index);

	int GetLine(char* pch, int nCount, char delim = delimENDOFLINE);
	int GetLine(unsigned char* puch, int nCount, char delim = delimENDOFLINE);

	int PutLine(std::streampos position, char* newline);
	int PutValue(const char* secname, const char* name, char* buffer, int index, char* value);

	int Backup(char* backup);
	int Restore(char* backup);
};
#endif // __Profile_H__
