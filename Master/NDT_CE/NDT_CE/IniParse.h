#pragma once 

#include "windows.h" 
#include <string> 
#include <vector> 
#include <map> 

#ifdef UNICODE 
#ifndef TSTRING 
#define TSTRING std::wstring 
#endif 
#else 
#ifndef TSTRING 
#define TSTRING std::string 
#endif 
#endif 


class CIniParse 
{ 
public: 
	//------------------------------------------------------------------------------- 
	//Description: 
	// Open the ini file. 
	//------------------------------------------------------------------------------- 
	BOOL Open(TSTRING strFile); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the profile value as string type 
	//------------------------------------------------------------------------------- 
	TSTRING GetPrivateProfileString(const TSTRING &strSection,const TSTRING &strKey); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the profile value as int type 
	//------------------------------------------------------------------------------- 
	int GetPrivateProfileInt(const TSTRING &strSection,const TSTRING &strKey); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Set the profile value as string type.The function wouldn't save the data to file 
	//but memory. If you want to save to the file, you must call Flush function 
	//------------------------------------------------------------------------------- 
	BOOL SetPrivateProfileString(const TSTRING &strSection,const TSTRING &strKey,TSTRING strSet); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Set the profile value as int type.The function wouldn't save the data to file 
	//but memory. If you want to save to the file, you must call Flush function 
	//------------------------------------------------------------------------------- 
	BOOL SetPrivateProfileInt(const TSTRING &strSection,const TSTRING &strKey,int iSet); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Flush the memory buffer to the file 
	//------------------------------------------------------------------------------- 
	BOOL Flush(); 

public: 
	CIniParse(); 
	virtual ~CIniParse(); 


private: 
	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the line value base on the current offset. After calling, the offset value 
	//would move to the head of next line 
	//------------------------------------------------------------------------------- 
	TSTRING GetLine(); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Check the string value of one line whether is comment or not 
	//------------------------------------------------------------------------------- 
	static BOOL IsCommentLine(const TSTRING & strLine); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Check the string value of one line whether is the section. 
	//Parameters: 
	// strLine : [in] The string value buffer. 
	//------------------------------------------------------------------------------- 
	static BOOL IsSectionLine(const TSTRING & strLine); 


	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the key value from one line 
	//Parameters: 
	// strLine : [in] The buffer to find 
	//------------------------------------------------------------------------------- 
	static TSTRING GetKeyValueFromLine(const TSTRING & strLine); 


	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the key name from one line 
	//Parameters: 
	// strLine : [in] The buffer to find 
	//------------------------------------------------------------------------------- 
	static TSTRING GetKeyNameFromLine(const TSTRING & strLine); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Get the section name from one line 
	//Parameters: 
	// strLine : [in] The buffer to find 
	//------------------------------------------------------------------------------- 
	static TSTRING GetSectionNameFromLine(const TSTRING & strLine); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Remove the space from the string 
	//Parameters: 
	// strBuf : [in] The buffer to remove 
	//Return Value: 
	// Return the string without space 
	//------------------------------------------------------------------------------- 
	static TSTRING RemoveSpace(const TSTRING &strBuf); 


	//------------------------------------------------------------------------------- 
	//Description: 
	// Parse the ini file 
	//------------------------------------------------------------------------------- 
	void Parse(const TSTRING &strBuf); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Convert the string to  lowercase 
	//------------------------------------------------------------------------------- 
	static TSTRING ConvertToLowercase(const TSTRING &strBuf); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Reset the offset point to the begin 
	//------------------------------------------------------------------------------- 
	BOOL ResetOffset(); 

	//------------------------------------------------------------------------------- 
	//Description: 
	// Check whether the offset arrived the end of the buffer. 
	//------------------------------------------------------------------------------- 
	BOOL IsOffsetEnd(); 


private: 
	TSTRING m_strFileBuf; 
	TSTRING::size_type m_stOffset; 
	TSTRING m_strFilePath; 
	std::map<TSTRING, std::map<TSTRING,TSTRING> > m_mpValue; //The first TSTRING is section name, the second is key name and last is the value. 
}; 
