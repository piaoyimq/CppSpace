//------------------------------------------------------------------------
// inifile.h is used for declaration of class DInifile
//------------------------------------------------------------------------
/*attention:
 at end of ini file,must end with empty line,or the end data can not read!
 */
#ifndef DInifileH
#define DInifileH

#include <stdlib.h>
#include <assert.h>
#include "profile.h"
//#define CONTENT_MAX_READLEN 200
class DIniFile
{
private:
	TProfile *pProfile;
	char *pFileName;
	char *valueBuf;
	char **pStrBuf;
	int nStrCount;
//	char Buf[CONTENT_MAX_READLEN];
public:
	DIniFile(const char *pFileNameP);
	~DIniFile();

	void ToLower(char *pBuf, int len);
	const char* ReadString(const char* pSection, const char* pKey, const char* pDefault);
	//read total string
	const char* ReadTString(const char* pSection, const char* pKey, const char* pDefault);
	void WriteString(const char* pSection, const char* pKey, const char* pValue);
	void WriteTString(const char* pSection, const char* pKey, char* pValue);

	int ReadInteger(const char* pSection, const char* pKey, int nDefault);
	void WriteInteger(const char* pSection, const char* pKey, const int nValue);

//	bool  ReadBool(const char* pSection, const char* pKey, const bool Default);
	//void  WriteBool(const char* pSection, const char* pKey, const bool Value);
	//write: if ture write "true",if false write "false"

	double ReadFloat(const char* pSection, const char* pKey,  double Default);
	void WriteFloat(const char* pSection, const char* pKey, const double Value);

	//exist return 1,else return 0;
	int SectionExists(const char* pSection);

	//read  all key names of specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSectionKeys(const char* pSection, char** & pStr);

	//read all sections name
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSections(char** &pSections);

	//read  all value of keys in specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! --no 
	int ReadSectionValues(const char* pSection, char** & pStr);

	//erase specified section all content,include key ,value and specified section name itself 
	void EraseSection(const char* pSection);

//	bool  KeyExists(const char* pSection, const char* pKey);

	//delete specified key and it's value in specified section
	void DeleteKey(const char* pSection, const char* pKey);

//	bool  ValueExists(const char* pSection, const char* pKey);

	char* encrypt(char *pBuf, int len);
	char* decrypt(char *pBuf, int len);

private:
	int WriteFile(const char* pSection, const char* pKey, const char*pValue);
	int ReadFile(const char* pSection, const char* pKey, char *pValue);
	int ReadTFile(const char* pSection, const char* pKey, char *pValue);
};
#endif

