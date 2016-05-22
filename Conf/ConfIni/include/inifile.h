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
	int  nStrCount;
//	char Buf[CONTENT_MAX_READLEN];
public:
    DIniFile(char *pFileNameP);
	~DIniFile();

	void ToLower(char *pBuf, int len);
	char* ReadString(char* pSection, char* pKey, char* pDefault);
	//read total string
	char* ReadTString(char* pSection, char* pKey, char* pDefault);
	void  WriteString(char* pSection, char* pKey, char* pValue);
	void  WriteTString(char* pSection, char* pKey, char* pValue);

	int   ReadInteger(char* pSection, char* pKey, const int nDefault);
	void  WriteInteger(char* pSection, char* pKey, const int nValue);

//	bool  ReadBool(char* pSection, char* pKey, const bool Default);
    //void  WriteBool(char* pSection, char* pKey, const bool Value);
	//write: if ture write "true",if false write "false"


	double  ReadFloat(char* pSection, char* pKey, const double Default);
    void  WriteFloat(char* pSection, char* pKey, const double Value);

	//exist return 1,else return 0;
	int  SectionExists(char* pSection);

	//read  all key names of specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int  ReadSectionKeys(char* pSection, char**  & pStr);

	//read all sections name
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int  ReadSections( char** &pSections); 
	
	//read  all value of keys in specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! --no 
	int  ReadSectionValues(char* pSection, char** & pStr);

	//erase specified section all content,include key ,value and specified section name itself 
    void  EraseSection(char* pSection);

//	bool  KeyExists(char* pSection, char* pKey);

	//delete specified key and it's value in specified section
	void  DeleteKey(char* pSection, char* pKey);

//	bool  ValueExists(char* pSection, char* pKey);

    char* encrypt(char *pBuf, int len);
    char* decrypt(char *pBuf, int len);
    

private:
    int WriteFile(char* pSection, char*pKey, char*pValue);
    int ReadFile(char* pSection, char*pKey, char *pValue);
    int ReadTFile(char* pSection, char*pKey, char *pValue);
};
#endif

