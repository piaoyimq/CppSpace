#include <stdlib.h>
#include <assert.h>
#ifdef __UNIX
#include <ctype.h>
#endif
#include "Conf/ConfIni/include/inifile.h"

//------------------------------------------------------------------------
DIniFile::DIniFile(char* pFileNameP)
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
/*char* DIniFile::ReadString(char* pSection, char* pKey, char* pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if ( !ReadFile(pSection, pKey, valueBuf ) )
        return  pDefault;
    else
        return valueBuf;
}*/
char * DIniFile::ReadString(char* pSection,char* pKey, char * pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if(!ReadFile(pSection,pKey,valueBuf))
	return pDefault;
	else 
	return valueBuf;

}
char * DIniFile::ReadTString(char* pSection,char* pKey, char * pDefault)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	if(!ReadTFile(pSection,pKey,valueBuf))
	return pDefault;
	else 
	return valueBuf;
}

void  DIniFile::WriteString(char* pSection, char* pKey, char* pValue)
{
    WriteFile( pSection, pKey, pValue );
}
void  DIniFile::WriteTString(char* pSection, char* pKey, char* pValue)
{
    WriteFile( pSection, pKey, pValue );
}

int   DIniFile::ReadInteger(char* pSection, char* pKey, const int nDefault)
{
	memset((char *)valueBuf,00,CONTENT_MAX_READLEN);	
	if(!ReadFile(pSection, pKey, valueBuf))
        return nDefault;
    else
        return (atoi(valueBuf));

}

void  DIniFile::WriteInteger(char* pSection, char* pKey, const int nValue)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	sprintf(valueBuf,"%d",nValue);
    WriteFile(pSection, pKey, valueBuf);
}
/*
bool  DIniFile::ReadBool(char* pSection, char* pKey, const bool Default)
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

void  DIniFile::WriteBool(char* pSection, char* pKey, const bool Value)
{
    WriteFile(pSection, pKey, (Value ? "true" : "false") );
}
*/
double  DIniFile::ReadFloat(char* pSection, char* pKey, const double Default)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
    if ( !ReadFile(pSection, pKey, valueBuf) )
        return Default;
    else
        return atof(valueBuf);
}

void  DIniFile::WriteFloat(char* pSection, char* pKey, const double Value)
{
	memset(valueBuf,0,CONTENT_MAX_READLEN);
	sprintf(valueBuf,"%f", Value);
    WriteFile(pSection, pKey, valueBuf);
}

int  DIniFile::SectionExists(char* pSection)
{
	return pProfile->FindSection(pSection);
}

int  DIniFile::ReadSectionKeys(char* pSection, char**  & pStr)
{
	return ( pProfile->ReadSectionKeys(pSection,pStr) );
}

int  DIniFile::ReadSections( char** &pSections)
{
	return ( pProfile->ReadSections(pSections) );
}

int  DIniFile::ReadSectionValues(char* pSection, char** & pStr)
{
    return ( pProfile->ReadSectionValues(pSection,pStr) );
}

void  DIniFile::EraseSection(char* pSection)
{
	pProfile->EraseSection(pSection);
}
/*
bool  DIniFile::KeyExists(char* pSection, char* pKey)
{
	return ( (!pProfile->FindSection(pSection) ||
              !pProfile->FindName(pSection,pKey)) \
              ? false : true );
}
*/
void  DIniFile::DeleteKey(char* pSection, char* pKey)
{
	pProfile->DeleteKey(pSection, pKey);
}

/*bool  DIniFile::ValueExists(char* pSection, char* pKey)
{
	return (pProfile->ValueExists(pSection,pKey) ? true : false ) ;
}
*/
void DIniFile::ToLower(char *pBuf, int len)
{
	for( int i = 0; i < len; i++ )
		pBuf[i] = ::tolower(pBuf[i]);
}


int DIniFile::WriteFile(char* pSection, char *pKey, char *pValue)
{
    int mError = 0;
    
	if (!pProfile->FindSection(pSection))
	{
	 	if (!pProfile->NewValue(pSection, pKey, pValue))
			mError=-1;
	}
	/*section exist, but name not exist*/
	else if (!pProfile->FindName(pSection, pKey))
	{	if (pProfile->NewNameOfSection(pSection, pKey))
		{
			if (!pProfile->NewValueOfName(pSection, pKey, pValue))
				mError=-3;
		}
		else
			mError=-2;	
	}
	/*section and name both exist*/
	else
	{
		if (!pProfile->WriteString(pSection, \
            pKey, 1, pValue))
			mError=-4;
	}

    return mError;
}

int DIniFile::ReadFile(char* pSection, char*pKey, char *pValue)
{
	return pProfile->ReadString(pSection, \
            pKey, 1, pValue);
}
int DIniFile::ReadTFile(char* pSection, char*pKey, char *pValue)
{
	return pProfile->ReadTString(pSection, \
            pKey, 1, pValue);
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
