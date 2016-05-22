/////////////////////////////////////////////////////////////////////
#ifndef __Profile_H__
#define __Profile_H__

#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <ctype.h>

#define __UNIX
/////////////////////////////////////////////////////////////////////
// Platform related issues
#ifdef __UNIX
 #include <unistd.h>
#endif

/////////////////////////////////////////////////////////////////////
// Constant table

// Performance
const int constLINESIZE          = 2048;
const int constLINEBUFFERSIZE    = constLINESIZE + 1;
//const int constTOKENSIZE         = 32;
//const int constTOKENSIZE         = 100;
const int constTOKENSIZE         = 2048;
const int constTOKENBUFFERSIZE   = constTOKENSIZE + 1;
#define CONTENT_MAX_READLEN	2048	//value max length

/////2000/5/30 add begin
const int constSECTIONMAX=100; //search all section max number 
const int constCONTENTMAXSIZE=100; //section name ,key or value max size
const int constKEYVALUEMAX=300; //search all key or value max number of specified section
/////2000/5/30 add end

// Delims
const char delimENDOFLINE    = '\n';
const char delimLEFTBRACKET  = '[' ;
const char delimRIGHTBRACKET = ']' ;
const char delimASSIGNMENT   = '=' ;
const char delimSPACER       = ' ' ;
const char delimREMARK       = '#' ;

class TProfile
{
	fstream   Profile;
	streampos posOldLine;
	char      Buffer[constLINEBUFFERSIZE];
	char*     bufferPtr;
	char      Token[constTOKENBUFFERSIZE];

//add 
private:
	int mLesslen;

public:

	/////2000/3/30 add begin

	//return 1 is true,0 is false
	int  ValueExists(const char* pSection, const char* pKey);

	//read  all value of keys in specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int  ReadSectionValues(const char* pSection, char** &pStr);

	//erase specified section all content,include key ,value and specified section name itself 
	void  EraseSection(char* pSection);

	//delete specified key and it's value in specified section
   	void  DeleteKey(char* pSection,char* pKey);

	//from pContent to return section name(not incldue "[]"),if not found,pSection return NULL.
	//1 is true,0 is false
	int ReturnSection(char* pContent,char*  pSection);

	//from pContent to return Key name,if not found,pKey return NULL.
	//1 is true,0 is false
	int ReturnKey(char* pContent,char*  pKey);

	//from pContent to return Value,if not found,pValue return NULL.
	//1 is true,0 is false
	int ReturnValue(char* pContent,char*  pValue);

	//read  all key names of specified section
	//return value is the number of char array, 
	//caller must free alloced memory!! 
    int  ReadSectionKeys(const char* pSection, char** & pStr) ;

	//read all sections name
	//1 is true,0 is false
	//return value is the number of char array, 
	//caller must free alloced memory!! 
	int ReadSections(char** & pSections);

	/////2000/5/30 add end

	TProfile () { mLesslen=0;}
	~TProfile() { Profile.close(); mLesslen=0;}
	TProfile( char* filename ) : Profile( filename, ios::in|ios::out ) { }

	fstream& File() { return Profile; }
	fstream& Open( char* filename, int mode = ios::in|ios::out )
	{
		Profile.open( filename, mode );
		return Profile;
	}
	void Close() { Profile.close(); }

	// 0 - No ; 1 - Yes
	int IsEof() { return Profile.eof(); }
	int IsGood() { return Profile.good(); }

	// Interfaces - Read routines
	// Read 'value' from existing 'section', 'name'.
	// 0 - fail ; 1 - succeed
	int ReadChar  ( char* section, char* name, int index, char* buf );
	int ReadString( char* section, char* name, int index, char* buf );
	int ReadTString( char* section, char* name, int index, char* buf );
	int ReadShort ( char* section, char* name, int index, short* buf );
	int ReadUshort( char* section, char* name, int index, unsigned short* buf );
	int ReadInt   ( char* section, char* name, int index, int* buf );
	int ReadUint  ( char* section, char* name, int index, unsigned int* buf );
	int ReadLong  ( char* section, char* name, int index, long* buf );
	int ReadUlong ( char* section, char* name, int index, unsigned long* buf );
	int ReadFloat ( char* section, char* name, int index, float* buf );
	int ReadDouble( char* section, char* name, int index, double* buf );

	int ReadOct( char* section, char* name, int index, unsigned short* buf );
	int ReadOct( char* section, char* name, int index, unsigned int* buf );
	int ReadOct( char* section, char* name, int index, unsigned long* buf );

	int ReadHex( char* section, char* name, int index, unsigned short* buf );
	int ReadHex( char* section, char* name, int index, unsigned int* buf );
	int ReadHex( char* section, char* name, int index, unsigned long* buf );

	// Interfaces - Write routines
	// Write 'value' into existing 'section', 'name'.
	// 0 - fail ; 1 - succeed
	int WriteChar  ( char* section, char* name, int index, char buf );
	int WriteString( char* section, char* name, int index, char* buf );
	int WriteShort ( char* section, char* name, int index, short buf );
	int WriteUshort( char* section, char* name, int index, unsigned short buf );
	int WriteInt   ( char* section, char* name, int index, int buf );
	int WriteUint  ( char* section, char* name, int index, unsigned int buf );
	int WriteLong  ( char* section, char* name, int index, long buf );
	int WriteUlong ( char* section, char* name, int index, unsigned long buf );
	int WriteFloat ( char* section, char* name, int index, float buf );
	int WriteDouble( char* section, char* name, int index, double buf );

	int WriteOct( char* section, char* name, int index, unsigned short buf );
	int WriteOct( char* section, char* name, int index, unsigned int buf );
	int WriteOct( char* section, char* name, int index, unsigned long buf );

	int WriteHex( char* section, char* name, int index, unsigned short buf );
	int WriteHex( char* section, char* name, int index, unsigned int buf );
	int WriteHex( char* section, char* name, int index, unsigned long buf );

	// Interfaces - New & Add routine
	// 0 - fail ; 1 - succeed

	// Create an unexisting 'section' (in the end).
	int NewSection( char* section );

	// Create an unexisting 'name' of an existing 'section'.
	// Duplicated 'name' case may exist.
	int NewNameOfSection( char* section, char* name );

	// Create unexisting 'name' and 'section' (in the end).
	int NewName( char* section, char* name );

	// Create unexisting 'value', 'name', 'section' (in the end).
	int NewValue( char* section, char* name, char* value );

	// Append a 'value' to existing 'section', 'name'.
	// Duplicated 'value' case may exist.
	int AddValue( char* section, char* name, char* value );

	// Append a 'value' to existing 'section', 'name'; the same as AddValue().
	int NewValueOfName( char* section, char* name, char* value );

	// 0 - fail; 1 - succeed
	int FindSection( char* section );

/*Chen move from private*/
	int FindName( char* section, char* name );
private:

	// 0 - fail; 1 - succeed
	int FindNameInSection( char* name );

//	int FindName( char* section, char* name );

	int IsName( char* buffer, char* name );
	int IsSection( char* buffer, char* secname );

	// 0 - fail; NOT 0 - a valid pointer of next value
	char* FirstValue( char* buffer );
	char* NextValue ( char* buffer );
	char* FindValue ( char* section, char* name, int index );
	char* FindTValue ( char* section, char* name, int index );

	int GetLine( char* pch, int nCount, char delim = delimENDOFLINE );
	int GetLine( unsigned char* puch, int nCount, char delim = delimENDOFLINE );
	
	int PutLine ( streampos position, char* newline );
	int PutValue( char* section, char* name, char* buffer, int index, char* value );

	int Backup( char* backup );
	int Restore( char* backup );
};
#endif // __Profile_H__
