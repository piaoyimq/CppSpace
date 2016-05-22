#include <stdlib.h>
#include <assert.h>
#ifdef __UNIX
#include <ctype.h>
#endif
#include "inifile.h"

//------------------------------------------------------------------------
DIniFile::DIniFile(const char *pFileNameP)
{
    assert( pFileNameP );
    pFileName = new char[ strlen(pFileNameP) + 1];
    strcpy(pFileName, pFileNameP);
    valueBuf=new char[CONTENT_MAX_READLEN];
    assert(valueBuf);
    pProfile = new TProfile( pFileName );
    assert( pProfile );
}

DIniFile::~DIniFile()
{
	delete[] valueBuf;
	delete pProfile;
	delete[] pFileName;
}
/*char* DIniFile::ReadString(const char* pSection, const char* pKey, const char* pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if ( !ReadFile(pSection, pKey, valueBuf ) )
        return (char*) pDefault;
    else
        return valueBuf;
}*/
char * DIniFile::ReadString(const char * pSection,const char * pKey,const char * pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if(!ReadFile(pSection,pKey,valueBuf))
	return (char *)pDefault;
	else 
	return valueBuf;

}
char * DIniFile::ReadTString(const char * pSection,const char * pKey,const char * pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if(!ReadTFile(pSection,pKey,valueBuf))
	return (char *)pDefault;
	else 
	return valueBuf;
}

void  DIniFile::WriteString(const char* pSection, const char* pKey, const char* pValue)
{
    WriteFile( pSection, pKey, pValue );
}
void  DIniFile::WriteTString(const char* pSection, const char* pKey, const char* pValue)
{
    WriteFile( pSection, pKey, pValue );
}

int   DIniFile::ReadInteger(const char* pSection, const char* pKey, const int nDefault)
{
	memset((char *)valueBuf,00,CONTENT_MAX_READLEN);	
	if(!ReadFile(pSection, pKey, valueBuf))
        return nDefault;
    else
        return (atoi(valueBuf));

}

void  DIniFile::WriteInteger(const char* pSection, const char* pKey, const int nValue)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	sprintf(valueBuf,"%d",nValue);
    WriteFile(pSection, pKey, valueBuf);
}
/*
bool  DIniFile::ReadBool(const char* pSection, const char* pKey, const bool Default)
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

void  DIniFile::WriteBool(const char* pSection, const char* pKey, const bool Value)
{
    WriteFile(pSection, pKey, (Value ? "true" : "false") );
}
*/
double  DIniFile::ReadFloat(const char* pSection, const char* pKey, const double Default)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
    if ( !ReadFile(pSection, pKey, valueBuf) )
        return Default;
    else
        return atof(valueBuf);
}

void  DIniFile::WriteFloat(const char* pSection, const char* pKey, const double Value)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	sprintf(valueBuf,"%f", Value);
    WriteFile(pSection, pKey, valueBuf);
}

int  DIniFile::SectionExists(const char* pSection)
{
	return pProfile->FindSection((char*)pSection);
}

int  DIniFile::ReadSectionKeys(const char* pSection, char**  & pStr)
{
	return ( pProfile->ReadSectionKeys(pSection,pStr) );
}

int  DIniFile::ReadSections( char** &pSections)
{
	return ( pProfile->ReadSections(pSections) );
}

int  DIniFile::ReadSectionValues(const char* pSection, char** & pStr)
{
    return ( pProfile->ReadSectionValues(pSection,pStr) );
}

void  DIniFile::EraseSection(const char* pSection)
{
	pProfile->EraseSection((char*)pSection);
}
/*
bool  DIniFile::KeyExists(const char* pSection, const char* pKey)
{
	return ( (!pProfile->FindSection((char*)pSection) || 
              !pProfile->FindName((char*)pSection,(char*)pKey)) \
              ? false : true );
}
*/
void  DIniFile::DeleteKey(const char* pSection, const char* pKey)
{
	pProfile->DeleteKey((char*)pSection,(char*)pKey);
}

/*bool  DIniFile::ValueExists(const char* pSection, const char* pKey)
{
	return (pProfile->ValueExists(pSection,pKey) ? true : false ) ;
}
*/
void DIniFile::ToLower(char *pBuf, int len)
{
	for( int i = 0; i < len; i++ )
		pBuf[i] = ::tolower(pBuf[i]);
}


int DIniFile::WriteFile(const char* pSection, const char *pKey, const char *pValue)
{
    int mError = 0;
    
	if (!pProfile->FindSection((char*)pSection))
	{
	 	if (!pProfile->NewValue((char*)pSection, (char*)pKey, (char*)pValue))
			mError=-1;
	}
	/*section exist, but name not exist*/
	else if (!pProfile->FindName((char*)pSection, (char*)pKey))
	{	if (pProfile->NewNameOfSection((char*)pSection, (char*)pKey))
		{
			if (!pProfile->NewValueOfName((char*)pSection, (char*)pKey, (char*)pValue))
				mError=-3;
		}
		else
			mError=-2;	
	}
	/*section and name both exist*/
	else
	{
		if (!pProfile->WriteString((char*)pSection, \
            (char*)pKey, 1, (char*)pValue))
			mError=-4;
	}

    return mError;
}

int DIniFile::ReadFile(const char* pSection, const char *pKey, char *pValue)
{
	return pProfile->ReadString((char*)pSection, \
            (char*)pKey, 1, (char*)pValue);
}
int DIniFile::ReadTFile(const char* pSection, const char *pKey, char *pValue)
{
	return pProfile->ReadTString((char*)pSection, \
            (char*)pKey, 1, (char*)pValue);
}

char* DIniFile::encrypt(char *pBuf, int len)
{
    int mI;
	for( int i = 0; i < len; i++ )
    {
       mI=(2+ len)*i;
       if (mI>100)
            mI=100;
       	pBuf[i] = char (pBuf[i] + mI);
     }
	return pBuf;
}

char*  DIniFile::decrypt(char *pBuf, int len)
{
    int mI;
	for( int i = 0; i < len; i++ )
    {
       mI=(2+ len)*i;
       if (mI>100)
            mI=100;
       	pBuf[i] = char (pBuf[i] - mI);
 
    }

	return pBuf;
}


////////////////////////////////////
