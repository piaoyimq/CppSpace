#include "Conf/ConfIni/include/ConfigIni.h"
#include <stdlib.h>
#include <assert.h>



//------------------------------------------------------------------------
ConfigIni::ConfigIni(const char* pFileNameP)
{
	assert(pFileNameP);
	pFileName = new char[strlen(pFileNameP) + 1];
	strcpy(pFileName, pFileNameP);
	valueBuf = new char[CONTENT_MAX_READLEN];
	assert(valueBuf);
	pProfile = new TProfile(pFileName);
	assert(pProfile);
}

ConfigIni::~ConfigIni()
{
	delete[] valueBuf;
	delete pProfile;
	delete[] pFileName;
}
/*char* ConfigIni::ReadString(const char* pSection, const char* pKey, char* pDefault)
 {
 memset(valueBuf,0,CONTENT_MAX_READLEN);
 if ( !ReadFile(pSection, pKey, valueBuf ) )
 return  pDefault;
 else
 return valueBuf;
 }*/
const char * ConfigIni::ReadString(const char* pSection, const char* pKey, const char * pDefault)
{
	memset(valueBuf, 0, CONTENT_MAX_READLEN);
	if (!ReadFile(pSection, pKey, valueBuf))
		return pDefault;
	else
		return valueBuf;

}
const char * ConfigIni::ReadTString(const char* pSection, const char* pKey, const char * pDefault)
{
	memset(valueBuf, 0, CONTENT_MAX_READLEN);
	if (!ReadTFile(pSection, pKey, valueBuf))
		return pDefault;
	else
		return valueBuf;
}

void ConfigIni::WriteString(const char* pSection, const char* pKey, const char* pValue)
{
	WriteFile(pSection, pKey, pValue);
}
void ConfigIni::WriteTString(const char* pSection, const char* pKey, char* pValue)
{
	WriteFile(pSection, pKey, pValue);
}

int ConfigIni::ReadInteger(const char* pSection, const char* pKey, int nDefault)
{
	memset((char *) valueBuf, 00, CONTENT_MAX_READLEN);
	if (!ReadFile(pSection, pKey, valueBuf))
		return nDefault;
	else
		return (atoi(valueBuf));

}

void ConfigIni::WriteInteger(const char* pSection, const char* pKey, const int nValue)
{
	memset(valueBuf, 0, CONTENT_MAX_READLEN);
	sprintf(valueBuf, "%d", nValue);
	WriteFile(pSection, pKey, valueBuf);
}
/*
 bool  ConfigIni::ReadBool(const char* pSection, const char* pKey, const bool Default)
 {
 memset(valueBuf,0,CONTENT_MAX_READLEN);
 if ( !ReadFile(pSection, pKey, valueBuf) )
 return Default;

 ToLower(valueBuf,strlen(valueBuf));
 if( strcmp(valueBuf,"false") == 0 )
 return false;
 else
 if( strcmp(valueBuf,"true") == 0 )
 return true;
 if(  atoi(valueBuf) != 0 )
 return true;

 return false;
 }

 void  ConfigIni::WriteBool(const char* pSection, const char* pKey, const bool Value)
 {
 WriteFile(pSection, pKey, (Value ? "true" : "false") );
 }
 */
double ConfigIni::ReadFloat(const char* pSection, const char* pKey,  double Default)
{
	memset(valueBuf, 0, CONTENT_MAX_READLEN);
	if (!ReadFile(pSection, pKey, valueBuf))
		return Default;
	else
		return atof(valueBuf);
}

void ConfigIni::WriteFloat(const char* pSection, const char* pKey, const double Value)
{
	memset(valueBuf, 0, CONTENT_MAX_READLEN);
	sprintf(valueBuf, "%f", Value);
	WriteFile(pSection, pKey, valueBuf);
}

int ConfigIni::SectionExists(const char* pSection)
{
	return pProfile->FindSection(pSection);
}

int ConfigIni::ReadSectionKeys(const char* pSection, char** & pStr)
{
	return (pProfile->ReadSectionKeys(pSection, pStr));
}

int ConfigIni::ReadSections(char** &pSections)
{
	return (pProfile->ReadSections(pSections));
}

int ConfigIni::ReadSectionValues(const char* pSection, char** & pStr)
{
	return (pProfile->ReadSectionValues(pSection, pStr));
}

void ConfigIni::EraseSection(const char* pSection)
{
	pProfile->EraseSection(pSection);
}
/*
 bool  ConfigIni::KeyExists(const char* pSection, const char* pKey)
 {
 return ( (!pProfile->FindSection(pSection) ||
 !pProfile->FindName(pSection,pKey)) \
              ? false : true );
 }
 */
void ConfigIni::DeleteKey(const char* pSection, const char* pKey)
{
	pProfile->DeleteKey(pSection, pKey);
}

/*bool  ConfigIni::ValueExists(const char* pSection, const char* pKey)
 {
 return (pProfile->ValueExists(pSection,pKey) ? true : false ) ;
 }
 */
void ConfigIni::ToLower(char *pBuf, int len)
{
	for (int i = 0; i < len; i++)
		pBuf[i] = ::tolower(pBuf[i]);
}

int ConfigIni::WriteFile(const char* pSection, const char* pKey, const char *pValue)
{
	int mError = 0;

	if (!pProfile->FindSection(pSection))
	{
		if (!pProfile->NewValue(pSection, pKey, pValue))
			mError = -1;
	}
	/*section exist, but name not exist*/
	else
		if (!pProfile->FindName(pSection, pKey))
		{
			if (pProfile->NewNameOfSection(pSection, pKey))
			{
				if (!pProfile->NewValueOfName(pSection, pKey, pValue))
					mError = -3;
			}
			else
				mError = -2;
		}
		/*section and name both exist*/
		else
		{
			if (!pProfile->WriteString(pSection, pKey, 1, pValue))
				mError = -4;
		}

	return mError;
}

int ConfigIni::ReadFile(const char* pSection, const char*pKey, char *pValue)
{
	return pProfile->ReadString(pSection, pKey, 1, pValue);
}
int ConfigIni::ReadTFile(const char* pSection, const char*pKey, char *pValue)
{
	return pProfile->ReadTString(pSection, pKey, 1, pValue);
}

char* ConfigIni::encrypt(char *pBuf, int len)
{
	int mI;
	for (int i = 0; i < len; i++)
	{
		mI = (2 + len) * i;
		if (mI > 100)
			mI = 100;
		pBuf[i] = char(pBuf[i] + mI);
	}
	return pBuf;
}

char* ConfigIni::decrypt(char *pBuf, int len)
{
	int mI;
	for (int i = 0; i < len; i++)
	{
		mI = (2 + len) * i;
		if (mI > 100)
			mI = 100;
		pBuf[i] = char(pBuf[i] - mI);

	}

	return pBuf;
}

////////////////////////////////////
