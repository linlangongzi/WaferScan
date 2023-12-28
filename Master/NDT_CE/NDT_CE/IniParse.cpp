#include "StdAfx.h"
#include "IniParse.h"

#include <deque> 
#include <algorithm> 



CIniParse::CIniParse(): 
m_stOffset(0) 
{} 

CIniParse::~CIniParse() 
{} 

BOOL CIniParse::Open(TSTRING strFile) 
{ 
	HANDLE hFile = CreateFile(strFile.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL); 
	if(hFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE; 
	} 

	m_strFilePath = strFile; 

	DWORD dwSize = GetFileSize(hFile,NULL); 

	//Read the file to buffer 
	std::vector<BYTE> vtBuf(dwSize,0); 
	DWORD dwRead = 0; 
	ReadFile(hFile,&vtBuf[0],vtBuf.size(),&dwRead,NULL); 
	vtBuf.resize(dwRead); 

	CloseHandle(hFile); 

#ifdef UNICODE 

	m_strFileBuf.clear(); 

	if(vtBuf.size() >= 2) 
	{ 
		if(vtBuf[0] == 0xFF && vtBuf[1] == 0xFE) 
		{ 
			//Unicode file 

			//Convert the read buffer to the unicode.The TCHAR here is equal to wchar_t 
			std::deque<TCHAR> dqBuf; 
			for(std::vector<BYTE>::size_type i = 2; i < vtBuf.size(); i += 2) 
			{ 
				dqBuf.push_back(vtBuf[i] + (vtBuf[i+1] << 8)); 
			} 


			m_strFileBuf.insert(m_strFileBuf.end(),dqBuf.begin(),dqBuf.end()); 
		} 
		else 
		{ 
			std::vector<TCHAR> vtBufT; 
			vtBufT.resize(MultiByteToWideChar (CP_ACP, 0, reinterpret_cast<char *>(&vtBuf[0]), vtBuf.size(), NULL, 0)); 
			MultiByteToWideChar (CP_ACP, 0, reinterpret_cast<char *>(&vtBuf[0]), vtBuf.size(), &vtBufT[0], vtBufT.size());   

			m_strFileBuf.insert(m_strFileBuf.end(),vtBufT.begin(),vtBufT.end()); 
		} 
	} 

#else 

#error "Unfortunately! The source code donesn't complete in the ASCII environment" 

#endif 




	Parse(m_strFileBuf); 

	return TRUE; 
} 

TSTRING CIniParse::GetLine() 
{ 
	TSTRING strReturn; 

	if(m_stOffset >= m_strFileBuf.size()) 
	{ 
		return TSTRING(); 
	} 

	TSTRING::size_type stPos = m_strFileBuf.find(TEXT("/r/n"),m_stOffset); 
	if(stPos == TSTRING::npos) 
	{ 
		strReturn.insert(0,m_strFileBuf,m_stOffset,m_strFileBuf.size() - m_stOffset + 1);  

		//Move the offset position to end of the file 
		m_stOffset = m_strFileBuf.size() + 1; 
	} 
	else 
	{ 
		strReturn.insert(0,m_strFileBuf,m_stOffset,stPos - m_stOffset);  

		//Move the offset position to the back of the "/r/n"  
		m_stOffset = stPos + 2; 

	} 

	return strReturn; 
} 

BOOL CIniParse::IsCommentLine(const TSTRING &strLine) 
{ 
	if(strLine.empty() != FALSE || strLine[0] == ';') 
	{ 
		return TRUE; 
	} 
	else 
	{ 
		return FALSE; 
	} 
} 

BOOL CIniParse::IsSectionLine(const TSTRING &strLine) 
{ 
	TSTRING::size_type stLeft = strLine.find(TEXT("[")); 
	TSTRING::size_type stRight = strLine.find(TEXT("]")); 
	if(strLine.empty() == FALSE && stLeft != TSTRING::npos && stRight != TSTRING::npos && stRight > stLeft) 
	{ 
		return TRUE; 
	} 
	else 
	{ 
		return FALSE; 
	} 
} 

TSTRING CIniParse::GetKeyValueFromLine(const TSTRING &strLine) 
{ 
	TSTRING::size_type stPosEqual = strLine.find(TEXT("=")); 
	if(stPosEqual == TSTRING::npos) 
	{ 
		return TSTRING(); 
	} 



	TSTRING strReturn; 

	TSTRING::size_type stPosQuoteLeft = strLine.find(TEXT("/"),stPosEqual); 
		if(stPosQuoteLeft != TSTRING::npos) 
		{ 
			TSTRING::size_type stPosQuoteRight = strLine.find(TEXT("/"),stPosQuoteLeft + 1); 
				if(stPosQuoteRight != TSTRING::npos && stPosQuoteLeft + 1 != stPosQuoteRight) 
				{ 
					strReturn.insert(0,strLine,stPosQuoteLeft + 1,stPosQuoteRight - stPosQuoteLeft - 1); 
					return strReturn; 
				} 
		} 


		//Store the string to the buffer 
		if(stPosEqual + 1 > strLine.size()) 
		{ 
			return TSTRING(); 
		} 
		TSTRING strBuf(strLine,stPosEqual + 1,strLine.size() - stPosEqual);; 
		return RemoveSpace(strBuf); 
} 


TSTRING CIniParse::GetKeyNameFromLine(const TSTRING &strLine) 
{ 
	TSTRING::size_type stPosEqual = strLine.find(TEXT("=")); 
	if(stPosEqual == 0) 
	{ 
		return TSTRING(); 
	} 

	TSTRING strBuf; 
	if(stPosEqual == TSTRING::npos) 
	{ 
		//All of the string line is the key name 
		strBuf = strLine; 
	} 
	else 
	{ 
		strBuf.clear(); 
		strBuf.insert(0,strLine,0,stPosEqual); 
	} 

	return RemoveSpace(strBuf); 

} 

TSTRING CIniParse::GetSectionNameFromLine(const TSTRING &strLine) 
{ 
	TSTRING::size_type stLeft = strLine.find(TEXT("[")); 
	TSTRING::size_type stRight = strLine.find(TEXT("]")); 
	if(!(strLine.empty() == FALSE && stLeft != TSTRING::npos && stRight != TSTRING::npos && stRight > stLeft)) 
	{ 
		return TSTRING(); 
	} 

	TSTRING strBuf(strLine,stLeft + 1,stRight - stLeft - 1);  
	return RemoveSpace(strBuf); 

} 

TSTRING CIniParse::RemoveSpace(const TSTRING &strBuf) 
{ 
	if(strBuf.find(TEXT(" ")) != TSTRING::npos) 
	{ 
		//Remove the space 
		TSTRING strReturn; 
		for(TSTRING::size_type stPos = 0; stPos < strBuf.size(); stPos ++) 
		{ 
			if(strBuf[stPos] != ' ') 
			{ 
				strReturn.push_back(strBuf[stPos]); 
			} 
		} 
		return strReturn; 
	} 
	else 
	{ 
		//No space 
		return strBuf; 
	} 
} 

void CIniParse::Parse(const TSTRING &strBuf) 
{ 
	//Reset the file pointer to the begin 
	ResetOffset(); 

	std::map<TSTRING,TSTRING> mpKey; 
	TSTRING strSection; 

	while(TRUE) 
	{ 
		TSTRING strLine = GetLine(); 

		if(strLine.empty() != FALSE || IsCommentLine(strLine) != FALSE) 
		{ 
			if(IsOffsetEnd()) 
			{ 
				break; 
			} 
			else 
			{ 
				continue; 
			} 
		} 

		if(IsSectionLine(strLine) != FALSE) 
		{    
			if(strSection.empty() != FALSE) 
			{ 
				//It's the first section 

				strSection = GetSectionNameFromLine(strLine); 
				continue; 
			} 

			//Store the last section value 
			m_mpValue.insert(std::make_pair(strSection,mpKey)); 

			strSection = GetSectionNameFromLine(strLine); 
			mpKey.clear(); 
		} 
		else 
		{ 
			if(strSection.empty() != FALSE) 
			{ 
				//The section name is empty, so needn't store the key value 
				continue; 
			} 

			//Store the key value and name 
			TSTRING strKeyName = GetKeyNameFromLine(strLine); 
			if(strKeyName.empty() == FALSE) 
			{ 
				mpKey.insert(std::make_pair(strKeyName,GetKeyValueFromLine(strLine))); 
			} 
		} 
	} 

	//Store the last section value 
	if(strSection.empty() == FALSE) 
	{ 
		m_mpValue.insert(std::make_pair(strSection,mpKey)); 
	} 
} 


TSTRING CIniParse::ConvertToLowercase(const TSTRING &strBuf) 
{ 
	std::vector<TCHAR> vtBuf(strBuf.length() + 1,0); 
	_tcscpy(&vtBuf[0],strBuf.c_str()); 
	return _tcslwr(&vtBuf[0]); 
} 

BOOL CIniParse::ResetOffset() 
{ 
	m_stOffset = 0; 
	return TRUE; 
} 

BOOL CIniParse::IsOffsetEnd() 
{ 
	if(m_stOffset >= m_strFileBuf.size()) 
	{ 
		return TRUE; 
	} 
	else 
	{ 
		return FALSE; 
	} 
} 

TSTRING CIniParse::GetPrivateProfileString(const TSTRING &strSection,const TSTRING &strKey) 
{ 
	TSTRING strSection1 = strSection;
	TSTRING strKey1 = strKey;
	if(m_mpValue.empty() != FALSE) 
	{ 
		return TSTRING(); 
	} 

	//Ignoring the character case to find the specified key  
	for(std::map<TSTRING, std::map<TSTRING,TSTRING>>::iterator iterSection = m_mpValue.begin(); iterSection != m_mpValue.end(); iterSection ++) 
	{ 
		if(ConvertToLowercase(iterSection->first) == ConvertToLowercase(strSection)) 
		{ 
			for(std::map<TSTRING,TSTRING>::iterator iterKey = iterSection->second.begin(); iterKey != iterSection->second.end(); iterKey ++) 
			{ 
				if(ConvertToLowercase(iterKey->first) == ConvertToLowercase(strKey)) 
				{ 
					return iterKey->second; 
				} 
			} 
		} 
	} 

	return TSTRING(); 
} 

int CIniParse::GetPrivateProfileInt(const TSTRING &strSection,const TSTRING &strKey) 
{ 
	return _ttoi(GetPrivateProfileString(strSection,strKey).c_str()); 
} 

BOOL CIniParse::SetPrivateProfileString(const TSTRING &strSection,const TSTRING &strKey,TSTRING strSet) 
{ 


	//Ignoring the character case to find the specified key  
	for(std::map<TSTRING, std::map<TSTRING,TSTRING>>::iterator iterSection = m_mpValue.begin(); iterSection != m_mpValue.end(); iterSection ++) 
	{ 
		if(ConvertToLowercase(iterSection->first) == ConvertToLowercase(strSection)) 
		{ 
			for(std::map<TSTRING,TSTRING>::iterator iterKey = iterSection->second.begin(); iterKey != iterSection->second.end(); iterKey ++) 
			{ 
				if(ConvertToLowercase(iterKey->first) == ConvertToLowercase(strKey)) 
				{ 
					iterKey->second = strSet; 
					return TRUE; 
				} 
			} 

			//Add the new key value 
			iterSection->second.insert(std::make_pair(strKey,strSet)); 

			return TRUE; 
		} 
	} 


	//Add the new section and key value 
	std::map<TSTRING,TSTRING> mpKey; 
	mpKey.insert(std::make_pair(strKey,strSet)); 
	m_mpValue.insert(std::make_pair(strSection,mpKey)); 

	return TRUE; 
} 

BOOL CIniParse::SetPrivateProfileInt(const TSTRING &strSection,const TSTRING &strKey,int iSet) 
{ 
	std::vector<TCHAR> vtBuf(MAX_PATH,0); 
	TSTRING strSet = _itot(iSet,&vtBuf[0],10); 
	return SetPrivateProfileString(strSection,strKey,strSet); 
} 

BOOL CIniParse::Flush() 
{ 
	TSTRING strWrite; 
	//strWrite.reserve(m_mpValue.size()); 

	//Store the string value to the buffer 
	for(std::map<TSTRING, std::map<TSTRING,TSTRING>>::iterator iterSection = m_mpValue.begin(); iterSection != m_mpValue.end(); iterSection ++) 
	{ 
		strWrite += TEXT("/r/n["); 
		strWrite += iterSection->first; 
		strWrite += TEXT("]/r/n"); 

		for(std::map<TSTRING,TSTRING>::iterator iterKey = iterSection->second.begin(); iterKey != iterSection->second.end(); iterKey ++) 
		{ 
			strWrite += iterKey->first; 
			strWrite += TEXT("="); 
			strWrite += iterKey->second; 
			strWrite += TEXT("/r/n"); 
		} 
	} 


	//Write to the file 
	HANDLE hFile = CreateFile(m_strFilePath.c_str(),GENERIC_WRITE,NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	if(INVALID_HANDLE_VALUE != hFile) 
	{ 

		std::vector<BYTE> vtWrite; 
		vtWrite.reserve(sizeof(TCHAR) * strWrite.size()); 

#ifdef UNICODE 
		vtWrite.push_back(0xFF); 
		vtWrite.push_back(0xFE); 
		for(TSTRING::iterator iter = strWrite.begin(); iter != strWrite.end(); iter++) 
		{ 
			vtWrite.push_back(static_cast<BYTE>(*iter)); 
			vtWrite.push_back((*iter)>>8); 
		} 
#else 
		vtWrite.assign(strWrite.begin(),strWrite.end()); 
#endif 

		DWORD dwWrite = 0; 
		WriteFile(hFile,&vtWrite[0],vtWrite.size(),&dwWrite,NULL); 
		CloseHandle(hFile); 

		return TRUE; 
	} 
	else 
	{ 
		return FALSE; 
	}  
} 
