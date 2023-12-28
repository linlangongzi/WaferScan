// IniParams.cpp: implementation of the CIniParams class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "IniParams.h"
#include "elepopular.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const WORD VERSION = 100;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString QueryExePath1()
{
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	//ASSERT(-1!=nPos);
	return p.Left(nPos+1);
}

CIniParams::CIniParams()
{
	::ZeroMemory(&m_info, sizeof(DogInfo));
}

CIniParams::~CIniParams()
{

}
// wangbingfu modify start on 2014-12-23

BOOL CIniParams::ReadIniData()
{	
	CUltrasonicImmersedTestApp *pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	unsigned short ReadDatOffset = 0x480;	//0x80为可读写数据区
	unsigned short ReadDatLength = sizeof(DogInfo);

	// 读取unsigned long 数据
	unsigned long RetVal = ElepReadMemory(pApp->m_ElepHandle,ReadDatOffset,
		(unsigned char*)&m_info,ReadDatLength);
	if(RetVal==ELEP_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("读取加密狗数据失败!"));
		return FALSE;
	}
}
/*
BOOL CIniParams::ReadIniData()
{	
	CUltrasonicImmersedTestApp *pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	unsigned short ReadDatOffset = 0x80;	//0x80为可读写数据区
	unsigned short ReadDatLength = sizeof(DogInfo);

	// 读取unsigned long 数据
	unsigned long RetVal = ElepReadMemory(pApp->m_ElepHandle,ReadDatOffset,
		(unsigned char*)&m_info,ReadDatLength);
	if(RetVal==ELEP_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("读取加密狗数据失败!"));
		return FALSE;
	}
}
*/
// wangbingfu modify end on 2014-12-23
// wangbingfu add start on 2014-12-23
BOOL CIniParams::WriteIniData()
{	
	CUltrasonicImmersedTestApp *pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	unsigned short ReadDatOffset = 0x480;	//0x80为可读写数据区
	unsigned short ReadDatLength = sizeof(DogInfo);
#if 1
	if(m_info.nCount == 0) // 5111881
	{
		CStdioFile fileParam;
		CString strFilePath = QueryExePath1();
		CString strValue;
		strFilePath += _T("YiQiConfig.ini");
		int nCount = 6;
		if (FALSE == fileParam.Open(strFilePath,CFile::modeRead))
		{
			MessageBox(NULL,_T("配置参数文件YiQiConfig.ini不存在！"),_T("提示"),MB_YESNO);
		}
		else
		{
			CString strValue;
			fileParam.ReadString(strValue);
			if (strValue.Find(_T("NUM")) != -1 && strValue.Find(_T("=")) != -1)
			{
				strValue.Delete(0,4);
				nCount = _tstoi(strValue);
			}
		}

		m_info.nCount = nCount;
	}
	else if(m_info.nCount == 1)
	{
		/*CStdioFile fileTxtData;
		ret = fileTxtData.Open(_T("C:\\JianShuBi.txt"), CFile::modeReadWrite, NULL);
		CString str;
		str.Format(_T("X=%f, Y=%f, z=%f"), m_info.fXJianSuBi, m_info.fYJianSuBi, m_info.fZJianSuBi);
		fileTxtData.WriteString(str);
		fileTxtData.Close();*/

		/*m_info.fXJianSuBi = 10;
		m_info.fYJianSuBi = 7;
		m_info.fZJianSuBi = 5;
		m_info.fZRange = 1;*/
		m_info.FieldValue4[0] = 2;
		m_info.FieldValue4[1] = 3;
		// 读取unsigned long 数据
		unsigned long RetVal = ElepWriteMemory(pApp->m_ElepHandle,ReadDatOffset,
			(unsigned char*)&m_info,ReadDatLength);
		
		return FALSE;
	}
	else if(m_info.nCount > 1)
	{
		m_info.nCount--;
	}

	// 读取unsigned long 数据
	unsigned long RetVal = ElepWriteMemory(pApp->m_ElepHandle,ReadDatOffset,
		(unsigned char*)&m_info,ReadDatLength);
	if(RetVal==ELEP_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		AfxMessageBox(_T("写加密狗数据失败!"));
		return FALSE;
	}
#endif
	return true;
}
// wangbingfu add end on 2014-12-23