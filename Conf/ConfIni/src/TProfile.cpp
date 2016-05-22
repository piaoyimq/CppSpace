#include "Conf/ConfIni/include/TProfile.h"
#include "Conf/ConfIni/include/DIniFile.h"

//-----------------------function define ----------------------------------
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// See if it is '[section]'-like line.
int TProfile::FindName(const char* section, const char* name)
{
	if (FindSection(section))
		return FindNameInSection(name);
	return 0;
}
int TProfile::GetLine(char* pch, int nCount, char delim)
{
	posOldLine = Profile.tellp();
	Profile.getline(pch, nCount, delim);
	return Profile.good();
}

int TProfile::IsSection(char* buffer, const char* secname)
{
	if (delimLEFTBRACKET == buffer[0])
	{
		const char* s = secname;
		char* t = &buffer[1];
		while (*s++ == *t++)
			;
		if (!*--s && delimRIGHTBRACKET == *--t)
		{
			return 1;
		}
	}
	return 0;
}

// See if it is 'name = '-like line.
int TProfile::IsName(char* buffer, const char* name)
{
	const char* s = name;
	char* t = buffer;

//correct by ChenHuaJie at 2000/4/1
	while (*s == *t)
	{
		s++;
		t++;
	}
	while (*t == ' ' && *t)
		t++;

	if (delimASSIGNMENT == *t && !*s)
		return 1;
	else
		return 0;

//old :
	/*	while( *s++==*t++ );
	 for( t--; (delimASSIGNMENT!=*t && *t); t++ );
	 if ( !*--s && delimASSIGNMENT==*t )
	 {
	 return 1;
	 }
	 return 0;
	 */

}

int TProfile::FindSection(const char* secname)
{
	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::beg);
	if (!Profile)
		return 0;
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE))
		if (IsSection(Buffer, secname))
			return 1;
	return 0;
}

int TProfile::FindNameInSection(const char* name)
{
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE) && delimLEFTBRACKET != Buffer[0] && !Profile.eof())
	{
		if (IsName(Buffer, name))
			return 1; //Obtain a valid value-ptr after d'call
	}
	return 0;
}

// Just suppose it should be some value there
char* TProfile::FirstValue(char* buffer)
{
	char* t = buffer;
	for (; delimASSIGNMENT != *t && *t; t++)
		;
	if (delimASSIGNMENT == *t)
	{
		return ++t;
	}
	return 0;
}

// Check the previous and move to the next
char* TProfile::NextValue(char* buffer)
{
	for (; delimSPACER == *buffer; buffer++)
		if (!*buffer)
			return 0;
	for (char* t = buffer;; t++)
	{
		switch (*t)
		{
		case delimSPACER:
			return t;
		case 0:
			return 0;
		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////

char* TProfile::FindValue(const char* section, const char* name, int index)
{
	char* buf = 0;
	if (!FindName(section, name))
		return 0;
	for (int i = 0; i < index; i++)
	{
		if (i)
		{ // Find next one
			if (!(buf = NextValue(buf)))
				return 0;
		}
		else
		{ // First time
			if (!(buf = FirstValue(Buffer)))
				return 0;
		}
	}
	int retval = sscanf(buf, "%s", Token);
	if (!retval || EOF == retval)
		return 0;
	else
		return Token;
}
char* TProfile::FindTValue(const char* section, const char* name, int index)
{
	char* buf = 0;
	if (!FindName(section, name))
		return 0;
	for (int i = 0; i < index; i++)
	{
		if (i)
		{ // Find next one
			if (!(buf = NextValue(buf)))
				return 0;
		}
		else
		{ // First time
			if (!(buf = FirstValue(Buffer)))
				return 0;
		}
	}
	memset(Token, '\0', sizeof(Token));
	strncpy(Token, buf, sizeof(Token) - 1);
	return Token;
	/*
	 int retval = sscanf( buf, "%c", Token );
	 if ( !retval || EOF==retval )
	 return 0;
	 else
	 return Token;
	 */

}

int TProfile::Backup(char* backup)
{
	std::ofstream backupfile(backup);
	char buf;
	if (Profile.good() && backupfile.good())
	{
		while (Profile.get(buf) && EOF != buf)
			backupfile.put(buf);
		backupfile.close();
		return 1;
	}
	backupfile.close();
	return 0;
}

int TProfile::Restore(char* backup)
{
	std::ifstream backupfile(backup);
	char buf;
	if (Profile.good() && backupfile.good())
	{
		while (backupfile.get(buf) && EOF != buf)
			Profile.put(buf);

		//new add 
		//put less charater,don't do that,put file data wrong!
		int i = 0, mCount;
		mCount = mLesslen <= constLINESIZE ? mLesslen : constLINESIZE;
		while (!Profile.eof() && i < mCount)
		{
			Profile.put(' ');
			i++;
		}

		backupfile.close();
		return 1;
	}
	backupfile.close();
	return 0;
}

// Interface
int TProfile::NewSection(const char* section)
{
	if (FindSection(section))
		return 0;
	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::end);
	Profile << std::endl << '[' << section << ']' << std::endl;
	return 1;
}

// Interface
int TProfile::NewName(const char* section, const char* name)
{
	if (NewSection(section))
	{
		Profile << name << '=';
		return 1;
	}
	return 0;
}

// Interface
int TProfile::NewValue(const char* section, const char* name, const char* value)
{
	if (NewName(section, name))
	{
		Profile << value << std::endl;
		return 1;
	}
	return 0;
}

// Interface
int TProfile::AddValue(const char* section, const char* name, const char* value)
{
	if (FindName(section, name))
	{
		int lenOldline = strlen(Buffer);
		int lenNewline = lenOldline + strlen(value) + 1;
		if ((lenNewline) <= constLINESIZE)
		{
			Buffer[lenOldline] = ' ';
			strcpy(&Buffer[lenOldline + 1], value);
			return PutLine(posOldLine, Buffer);
		}
	}
	return 0;
}

// Interface
int TProfile::NewNameOfSection(const char* section, const char* name)
{
	if (FindSection(section))
	{
		posOldLine = Profile.tellp();
		sprintf(Buffer, "%s=", name);
		return PutLine(posOldLine, Buffer);
	}
	return 0;
}

int TProfile::PutLine(std::streampos position, char* newline)
{
	char tempFilename[L_tmpnam];

	if (-1 == mkstemp(tempFilename))  //piaoyimq modify tmpnam
		return 0;
	if (Backup(tempFilename))
	{
		if (Profile.eof())
			Profile.clear();
		Profile.seekp(position);
		Profile << newline << std::endl;
		if (Restore(tempFilename))
			return (-1 == unlink(tempFilename) ? 0 : 1);
	}
	unlink(tempFilename);
	return 0;
}

int TProfile::PutValue(const char* section, const char* name, char* buffer, int index, char* value)
{
	char* curr = 0;
	if (!FindName(section, name))
		return 0;
	for (int i = 0; i < index; i++)
	{
		if (i)
		{		// Find next one
			if (!(curr = NextValue(curr)))
				return 0;
		}
		else
		{		// First time
			if (!(curr = FirstValue(buffer)))
				return 0;
		}
	}
	char* next = NextValue(curr);
	char backup[constLINEBUFFERSIZE];
	if (curr)
	{
		int len1 = curr - buffer;
		int len2 = strlen(value);
		int len3 = (next ? strlen(next) : 0);

		//add
		mLesslen = strlen(curr) - strlen(value);

		if ((len1 + len2 + len3) <= constLINESIZE)
		{
			if (next)
				strcpy(backup, next);
			else
				backup[0] = 0;
			strcpy(curr, value);
			strcpy(curr + len2, backup);
			return PutLine(posOldLine, buffer);
		}
	}
	return 0;
}

////////
int TProfile::ReturnSection(char* pContent, char* pSection)
{	//purpose:from pContent to return section name(not incldue "[]"),if not found,pSection return NULL.
	char *mP;
	int mCount = 0, i = 0;
	if (pContent[0] == delimLEFTBRACKET)
	{
		mP = pContent + 1;
		mCount = strlen(pContent) - 1;
		i = 0;
//		while (mP && *mP!=delimRIGHTBRACKET) //judge section
		while ((i < mCount) && *mP != delimRIGHTBRACKET)
		{	//judge section
			mP++;
			i++;
		}
		if (*mP == delimRIGHTBRACKET)
		{ //copy sections
			mP = pContent + 1;
//			while (mP && *mP!=delimRIGHTBRACKET)
			mCount = mCount - 1;
			i = 0;
			while (i < mCount)
			{
				if (*mP == delimRIGHTBRACKET)
					break;
				*pSection++ = *mP++;
				i++;
			}
			*pSection++ = 0;  //string end
			return 1;
		}
	}
	return 0;  //1 is true,0 is false
}
////////
int TProfile::ReadSections(char** & pSections)
{	//purpose:read all sections name,result section name each end with '\0'
//if not found ,pSections=NULL,not neccessary to deal memory point
	char *pResult, *pResultBuf, **pSectionsArray;
	int mI = 0, mCount = 0, i = 0;

	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::beg);
	if (!Profile)
	{
		return 0;	//1 is true,0 is false
	}
	pResultBuf = new char[constLINESIZE];

	//SectionsArray init
	pSectionsArray = new char*[constSECTIONMAX];
	for (mI = 0; mI < constSECTIONMAX; mI++)
		*(pSectionsArray + mI) = NULL;

	//search section
	mCount = 0;
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE))
	{
		memset(pResultBuf, 0, constLINESIZE);
		if (ReturnSection(Buffer, pResultBuf))
		{ //find section
			if (pResultBuf)
			{ //find ok
				pResult = new char[constCONTENTMAXSIZE];
				*(pSectionsArray + mCount) = pResult;
				pSectionsArray[mCount] = pResult;
				memset(pResult, 0, constCONTENTMAXSIZE);
				i = 0;
				while (*(pResultBuf + i) && i < constCONTENTMAXSIZE)
				{
					*(pResult + i) = *(pResultBuf + i); //copy section name
					i++;
				}
				mCount++;
			}
		}
	}
	if (mCount > 0)
		pSections = pSectionsArray;
	else
	{ //not found deal
		pSections = NULL;
		delete[] pSectionsArray;
	}
	delete[] pResultBuf;
	return mCount;
}
/////////////
int TProfile::ReturnKey(char* pContent, char* pKey)
{
	//from pContent to return Key name,if not found,pKey return NULL.
	//1 is true,0 is false
	char *mP, *mResult;
	int mFound = 0, i = 0, mTotal = 0, mCount = 0;
	if (pContent[0] == delimASSIGNMENT)
		return 0;
	mResult = new char[constCONTENTMAXSIZE];
	memset(mResult, 0, sizeof(constCONTENTMAXSIZE));
	mResult[0] = pContent[0];
	mP = pContent + 1;
	i = 1;

	mTotal = strlen(pContent) - 1;	//total string char number
	mTotal = constCONTENTMAXSIZE >= mTotal ? mTotal : constCONTENTMAXSIZE;
	mCount = 0;

//	while (mP && i<constCONTENTMAXSIZE) {
	while (mCount < mTotal)
	{
		if (*mP == delimASSIGNMENT)
		{
			mFound = 1;
			break;
		}
		mResult[i] = *mP;
		mP++;
		i++;
		mCount++;
	}
	if (mFound)
	{
		//	strcpy(pKey,mResult);
		i = i <= constCONTENTMAXSIZE ? i : constCONTENTMAXSIZE;
		for (int jj = 0; jj < i; jj++)
			pKey[jj] = mResult[jj];

		if (i < constCONTENTMAXSIZE) //string with'\0' end
			pKey[i] = 0;
	}
	else
		pKey = NULL;
	delete[] mResult;
	return mFound;
}
/////////////
int TProfile::ReturnValue(char* pContent, char* pValue)
{
	//from pContent to return Value,if not found,pValue return NULL.
	//1 is true,0 is false
	char *mP, *mResult;
	int mFound = 0, i = 0, mStartCopy = 0, mTotal = 0, mCount = 0;	//,mSum=0;
	if (pContent[0] == delimASSIGNMENT)
		return 0;
	mResult = new char[constCONTENTMAXSIZE];
	memset(mResult, 0, sizeof(constCONTENTMAXSIZE));

	mTotal = strlen(pContent) - 1;	//total string char number
	mTotal = constCONTENTMAXSIZE >= mTotal ? mTotal : constCONTENTMAXSIZE;
	i = 0;
	mCount = 0;
	mP = pContent + 1;
	while (mCount < mTotal)
	{
		if (*mP == delimASSIGNMENT)
		{
			if (!mStartCopy)
				mStartCopy = 1; //Start Copy value
			else
				break;
		}
		if (mStartCopy && (*mP != delimASSIGNMENT))
		{
			mResult[i] = *mP;
			i++;
			mFound = 1;
		}
		mP++;
		mCount++;
	}
	if (mFound)
	{
		i = i <= constCONTENTMAXSIZE ? i : constCONTENTMAXSIZE;
		for (int jj = 0; jj < i; jj++)
			pValue[jj] = mResult[jj];

		if (i < constCONTENTMAXSIZE) //string with'\0' end
			pValue[i] = 0;
	}
	else
		pValue = NULL;
	delete[] mResult;
	return mFound;
}

///////////
int TProfile::ReadSectionKeys(const char* pSection, char** & pStr)
{
	/*read  all key names of specified section
	 return value is the number of char array,
	 caller must free alloced memory!! */

	char *pResultBuf = NULL, **pSectionsArray, *pIsSection = NULL;
	char *pKeyBuf = NULL;
	int mI = 0, mCount = 0, mStartCopy = 0;

	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::beg);
	if (!Profile)
	{
		return 0; //1 is true,0 is false
	}
	pResultBuf = new char[constLINESIZE];
	pIsSection = new char[constCONTENTMAXSIZE];
	//SectionsArray init
	pSectionsArray = new char*[constKEYVALUEMAX];
	for (mI = 0; mI < constSECTIONMAX; mI++)
		*(pSectionsArray + mI) = NULL;

	//search section
	mCount = 0;
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE))
	{
		if (!mStartCopy)
		{
			if (IsSection(Buffer, pSection))
				mStartCopy = 1;
		}
		else
		{
			if (ReturnSection(Buffer, pIsSection))
				break;
			else
			{
				//key copy		
				pKeyBuf = new char[constCONTENTMAXSIZE];
				memset(pKeyBuf, 0, sizeof(constCONTENTMAXSIZE));
				/*				ReturnKey(Buffer,pKeyBuf);
				 if (pKeyBuf) {*/
				if (ReturnKey(Buffer, pKeyBuf))
				{
					pSectionsArray[mCount] = pKeyBuf;
					mCount++;
				}
				else
					delete[] pKeyBuf;
			}
		}
	}
	if (mCount > 0)
		pStr = pSectionsArray;
	else
	{ //not found deal
		pStr = NULL;
		delete[] pSectionsArray;
	}
	delete[] pResultBuf;
	delete[] pIsSection;
	return mCount;
}
////////////
int TProfile::ReadSectionValues(const char* pSection, char** &pStr)
{
	/*read  all value of keys in specified section
	 return value is the number of char array,
	 caller must free alloced memory!! */
	char *pResultBuf = NULL, **pArray, *pIsSection = NULL;
	char *pBuf = NULL;
	int mI = 0, mCount = 0, mStartCopy = 0;

	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::beg);
	if (!Profile)
	{
		return 0; //1 is true,0 is false
	}
	pResultBuf = new char[constLINESIZE];
	pIsSection = new char[constCONTENTMAXSIZE]; //is judge
	//SectionsArray init
	pArray = new char*[constKEYVALUEMAX];
	for (mI = 0; mI < constSECTIONMAX; mI++)
		*(pArray + mI) = NULL;

	//search section
	mCount = 0;
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE))
	{
		if (!mStartCopy)
		{
			if (IsSection(Buffer, pSection))
				mStartCopy = 1;
		}
		else
		{
			if (ReturnSection(Buffer, pIsSection))
				break;
			else
			{
				//result copy		
				pBuf = new char[constCONTENTMAXSIZE];
				memset(pBuf, 0, sizeof(constCONTENTMAXSIZE));
				if (ReturnValue(Buffer, pBuf))
				{
					pArray[mCount] = pBuf;
					mCount++;
				}
				else
					delete[] pBuf;
			}
		}
	}
	if (mCount > 0)
		pStr = pArray;
	else
	{ //not found deal
		pStr = NULL;
		delete[] pArray;
	}
	delete[] pResultBuf;
	delete[] pIsSection;
	return mCount;

}
////////////
int TProfile::ValueExists(const char* pSection, const char* pKey)
{
	//return 1 is true,0 is false
	char *pIsSection = NULL;
	char *pBuf = NULL;
	int mFound = 0, mStartCopy = 0;

	if (Profile.eof())
		Profile.clear();
	Profile.seekg(0, std::ios::beg);
	if (!Profile)
	{
		return 0; //1 is true,0 is false
	}
	pBuf = new char[constCONTENTMAXSIZE];
	memset(pBuf, 0, sizeof(constCONTENTMAXSIZE));
	pIsSection = new char[constCONTENTMAXSIZE]; //is judge
	mFound = 0;
	//read each line  in file
	while (GetLine(Buffer, constLINEBUFFERSIZE, delimENDOFLINE))
	{
		if (!mStartCopy)
		{
			if (IsSection(Buffer, pSection)) //find section
				mStartCopy = 1;
		}
		else
		{
			if (ReturnSection(Buffer, pIsSection))  //end flag
				break;
			else
				//judge value
				if (ReturnValue(Buffer, pBuf))
					if (!strcmp(pBuf, pKey))
					{
						mFound = 1;
						break;
					}
		}
	}
	delete pBuf;
	delete pIsSection;
	return mFound;
}

//////////////
void TProfile::DeleteKey(const char* pSection, const char* pKey)
{
//purpose:delete specified key and it's value in specified section
	int mLen = 0;
	if (FindName(pSection, pKey))
	{
		mLen = strlen(Buffer);
		for (int i = 0; i < mLen; i++)
			Buffer[i] = ' ';
		PutLine(posOldLine, Buffer);
	}
}
//////////////
void TProfile::EraseSection(const char* pSection)
{	//purpose:erase specified section all content,include key ,value and specified section name itself 
	int mCount = 0, i = 0;
	char ** pSections = NULL;
	//ReadSectionKeys
	if ((mCount = ReadSectionKeys(pSection, pSections)))
	{
		if (pSections)
		{
			//follow key ,erase key,values of section
			for (i = 0; i < mCount; i++)
			{
				if (pSections[i])
					DeleteKey(pSection, pSections[i]);
			}
			//free
			for (i = 0; i < mCount; i++)
				if (pSections[i])
					delete[] pSections[i];
			delete[] pSections;
		}
	}
	//delete section name
	int mLen = 0;
	if (FindSection(pSection))
	{
		mLen = strlen(Buffer);
		for (int i = 0; i < mLen; i++)
			Buffer[i] = ' ';
		PutLine(posOldLine, Buffer);
	}
}
///////////////////////////////////////////////////////
int TProfile::ReadString(const char* section, const char* name, int index, char* buf)
{
	char* source = FindValue(section, name, index);
	if (!source)
		return 0;
	int retval = sscanf(source, "%s", buf);
	if (!retval || EOF == retval)
		return 0;
	else
		return 1;
}
int TProfile::ReadTString(const char* section, const char* name, int index, char* buf)
{
	char* source = FindTValue(section, name, index);
	if (!source)
		return 0;
	else
	{
		strncpy(buf, source, CONTENT_MAX_READLEN - 1);
		return 1;
	}
	/*
	 int retval = sscanf( source, "%s", buf );
	 if ( !retval || EOF==retval )
	 return 0;
	 else
	 return 1;
	 */
}
int TProfile::NewValueOfName(const char* section, const char* name, const char* value)
{
//old:
	return AddValue(section, name, value);
	/*      char *mLocate;

	 if ( FindName( section, name ) )
	 {
	 int lenOldline = strlen(Buffer);
	 int lenNewline = lenOldline + strlen(value) + 1;
	 if ( (lenNewline) <= constLINESIZE )
	 {
	 if ((mLocate=strstr(Buffer,&delimASSIGNMENT))==NULL)
	 {
	 Buffer[lenOldline] = '=';
	 strcpy( &Buffer[lenOldline+1],value );
	 }
	 else
	 strcpy( mLocate+1, value );
	 return PutLine( posOldLine, Buffer );
	 }
	 }
	 */
	/*      if ( FindName( section, name ) )
	 {
	 int lenOldline = strlen(Buffer);
	 int lenNewline = lenOldline + strlen(value) + 1;
	 if ( (lenNewline) <= constLINESIZE )
	 {
	 Buffer[lenOldline] = ' ';
	 strcpy( &Buffer[lenOldline+1], value );
	 return PutLine( posOldLine, Buffer );
	 }
	 }
	 return 0;
	 */
}

int TProfile::WriteString(const char* section, const char* name, int index, const char* buf)
{
	sprintf(Token, "%s", buf);
	return PutValue(section, name, Buffer, index, Token);
}
