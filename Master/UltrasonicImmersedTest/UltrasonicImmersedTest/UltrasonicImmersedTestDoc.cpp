/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* CUltrasonicImmersedTestDoc ���ʵ��
*Filename : CUltrasonicImmersedTestDoc.cpp
*
*@author       Song Chenguang
*@version      1.0         Date: 2011-7-1
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "UltrasonicImmersedTestDoc.h"
#include "GlobalFunction.h"

#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "CScanDlgBar.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern CDatabase g_sqlDB;

int g_nADCFreq;	// AD�ظ�Ƶ��
int g_nADCaiYangShu;	// AD������
int g_nSpeed;	// ����

// CUltrasonicImmersedTestDoc

IMPLEMENT_DYNCREATE(CUltrasonicImmersedTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CUltrasonicImmersedTestDoc, CDocument)
	ON_COMMAND(ID_SAVE_A_SCAN, &CUltrasonicImmersedTestDoc::OnSaveAScan)
END_MESSAGE_MAP()


// CUltrasonicImmersedTestDoc ����/����

CString QueryExePath()
{
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	//ASSERT(-1!=nPos);
	return p.Left(nPos+1);
}

CUltrasonicImmersedTestDoc::CUltrasonicImmersedTestDoc()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::CUltrasonicImmersedTestDoc"),"Enter");

	// TODO: �ڴ����һ���Թ������

	CStdioFile fileParam;

	CString strFilePath = QueryExePath();
	CString strValue;
	strFilePath += _T("Param.ini");
	if (FALSE == fileParam.Open(strFilePath,CFile::modeRead))
	{
		MessageBox(NULL,_T("���ò����ļ�Param.ini�����ڣ�"),_T("��ʾ"),MB_YESNO);
	}
	else
	{
		for (int j=0;j < 45;j++)
		{
			fileParam.ReadString(strValue);

			if (strValue.Find(_T("X")) != -1 && strValue.Find(_T("=")) != -1)
			{
				CString strPinDai;
				strPinDai = strValue ;
				strPinDai.Left(1);
				int nPinDai = _ttoi(strPinDai);
				if(strValue.Find(_T("X11")) != -1)
				{
					strValue.Delete(0,5);
					fPlusEqualize[nPinDai - 1][0] = _tstof(strValue);
				}
				else if(strValue.Find(_T("X12")) != -1)
				{
					strValue.Delete(0,5);
					fPlusEqualize[nPinDai - 1][1] = _tstof(strValue);
				}
				else if(strValue.Find(_T("X21")) != -1)
				{
					strValue.Delete(0,5);
					fPlusEqualize[nPinDai - 1][2] = _tstof(strValue);
				}
				else if(strValue.Find(_T("X22")) != -1)
				{
					strValue.Delete(0,5);
					fPlusEqualize[nPinDai - 1][3] = _tstof(strValue);
				}
			}
			else if (strValue.Find(_T("MidValue1")) != -1)
			{
				strValue.Delete(0,10);
				m_stAllCH_CONFIG[0].stConfig.wValueMiddle = _ttoi(strValue);
			}
			else if (strValue.Find(_T("MidValue2")) != -1)
			{
				strValue.Delete(0,10);
				m_stAllCH_CONFIG[1].stConfig.wValueMiddle = _ttoi(strValue);
			}
			else if (strValue.Find(_T("MidValue3")) != -1)
			{
				strValue.Delete(0,10);
				m_stAllCH_CONFIG[2].stConfig.wValueMiddle = _ttoi(strValue);
			}
			else if (strValue.Find(_T("MidValue4")) != -1)
			{
				strValue.Delete(0,10);
				m_stAllCH_CONFIG[3].stConfig.wValueMiddle = _ttoi(strValue);
			}
		}
	}
	for(int i=0; i<CHANNEL_NUM;i++)
	{
		m_stAllCH_CONFIG[i].stCon.wMaiKuan = 5;
		m_stAllCH_CONFIG[i].stCon.wRepeatCounter = 390;
		m_stAllCH_CONFIG[i].stCon.wReset = 0;

		m_stAllCH_CONFIG[i].stConfig.wDACEnable = 0;
		m_stAllCH_CONFIG[i].stConfig.wdBControl = 1;
		m_stAllCH_CONFIG[i].fPlus = 0;
		m_stAllCH_CONFIG[i].fRefPlus = 0;
		m_stAllCH_CONFIG[i].stConfig.wDetectMode = 2;
		m_stAllCH_CONFIG[i].stConfig.wMonitorZeroCounter = 0;
		m_stAllCH_CONFIG[i].stConfig.wSuspend = 0;
		m_stAllCH_CONFIG[i].stGate.wGate1Enable = 1;
		m_stAllCH_CONFIG[i].stGate.wGate1SyncMode = 0;
		m_stAllCH_CONFIG[i].stGate.wGate1SyncThreshold = 400;
		m_stAllCH_CONFIG[i].stGate.wGate1Start = 750;//600;
		m_stAllCH_CONFIG[i].stGate.wGate1Width = 1050;//1200;
		m_stAllCH_CONFIG[i].stGate.wGate1Amp = 30;
		m_stAllCH_CONFIG[i].stGate.wGate2Enable = 1;
		m_stAllCH_CONFIG[i].stGate.wGate2SyncMode = 1;
		m_stAllCH_CONFIG[i].stGate.wGate2SyncThreshold = 400;
		m_stAllCH_CONFIG[i].stGate.wGate2Start = 750;//1700;
		m_stAllCH_CONFIG[i].stGate.wGate2Width = 1050;//2000;
		m_stAllCH_CONFIG[i].stGate.wGate2Amp = 30;
		m_stAllCH_CONFIG[i].stGate.wGate3Enable = 0;
		m_stAllCH_CONFIG[i].stGate.wGate3SyncMode = 1;
		m_stAllCH_CONFIG[i].stGate.wGate3SyncThreshold = 400;
		m_stAllCH_CONFIG[i].stGate.wGate3Start = 750;
		m_stAllCH_CONFIG[i].stGate.wGate3Width =1050;
		m_stAllCH_CONFIG[i].stGate.wGate3Amp = 30;
		m_stAllCH_CONFIG[i].stGate.wGate4Enable = 0;
		m_stAllCH_CONFIG[i].stGate.wGate4SyncMode = 1;
		m_stAllCH_CONFIG[i].stGate.wGate4SyncThreshold = 400;
		m_stAllCH_CONFIG[i].stGate.wGate4Start = 750;
		m_stAllCH_CONFIG[i].stGate.wGate4Width = 1050;
		m_stAllCH_CONFIG[i].stGate.wGate4Amp = 30;
		m_stAllCH_CONFIG[i].stGate.wDelay = 3;
		m_stAllCH_CONFIG[i].fRange = 100;
		m_stAllCH_CONFIG[i].nVelocity = 6000;
		m_stAllCH_CONFIG[i].stGate.wSimpleInterval = 0;
		m_stAllCH_CONFIG[i].stGate.wSimpleAverage = 1;//ͨ������ƽ������ʼ��Ϊ1��  yachang.huang modify on 2014-06-26

		m_stAllCH_CONFIG[i].wProbeBand = 3;	// 10M
		m_stAllCH_CONFIG[i].wVoltage = 400;
		if (i == 0)	// Xinbo.Lin modified 2014-05-08 �迹����ʼ������
		{
			m_stAllCH_CONFIG[i].wImpedance = 0;	// ͨ��1: 0��1��
		}
		else
		{
			m_stAllCH_CONFIG[i].wImpedance = 2;	// ͨ��2: 2��0��
		}
		m_stAllCH_CONFIG[i].wEnerge = 1;  //���� yachang.huang add on 2013-11-25
		m_stAllCH_CONFIG[i].wRevAndSendMode = 1;//�շ�ģʽ  yachang.huang add on 2013-11-25
		m_stAllCH_CONFIG[i].wSPIAddr = 0;//SPI��ַ yachang.huang add on 2013-11-25
		m_stAllCH_CONFIG[i].wSPIData = 0;//SPI���� yachang.huang add on 2013-11-25
		m_stAllCH_CONFIG[i].fPlusAddX11 = fPlusEqualize[3][0];
		m_stAllCH_CONFIG[i].fPlusAddX12 = fPlusEqualize[3][1];
		m_stAllCH_CONFIG[i].fPlusAddX21 = fPlusEqualize[3][2];
		m_stAllCH_CONFIG[i].fPlusAddX22 = fPlusEqualize[3][3];



//		ZeroMemory(fPlusEqualize,32 * sizeof(float));
// 		fileParam.Open()

	}

	// temp Song Chenguang 2013-12-11Ӧ��ԽҪ���޸�
	//������λ���Ѿ�д��Ϊ511 yachang.huang 2014-05-16
// 	m_stAllCH_CONFIG[0].stConfig.wValueMiddle = 502;
// 	m_stAllCH_CONFIG[1].stConfig.wValueMiddle = 500;


	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::CUltrasonicImmersedTestDoc"),"Leave");
}

CUltrasonicImmersedTestDoc::~CUltrasonicImmersedTestDoc()
{
}

BOOL CUltrasonicImmersedTestDoc::OnNewDocument()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::OnNewDocument"),"Enter");

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	SetTitle(_T("������"));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::OnNewDocument"),"Leave");
	return TRUE;
}




// CUltrasonicImmersedTestDoc ���л�

void CUltrasonicImmersedTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CUltrasonicImmersedTestDoc ���

#ifdef _DEBUG
void CUltrasonicImmersedTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUltrasonicImmersedTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUltrasonicImmersedTestDoc ����
// ��ʼ��ͨ������
BOOL CUltrasonicImmersedTestDoc::InitChannel()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::InitChannel"),"Enter");
	// �����ݿ��ȡ��������
//	INIT_CONFIG(m_stAllCH_CONFIG, &ch_config_global);

	// ����
	::memcpy(m_stAllCH_CONFIG_BackUp, m_stAllCH_CONFIG, sizeof(CH_CONFIG) * CHANNEL_NUM);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::InitChannel"),"Leave");
	return TRUE;
}



// ����Aɨ
void CUltrasonicImmersedTestDoc::OnSaveAScan()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::InitChannel"),"Enter");

	TCHAR szFilters[] =
		_T("NDT Instrument Parameter files (*.aaf)|*.aaf|All files (*.*)|*.*||");


	CFileDialog dlg(FALSE, _T("aaf"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		CMainFrame* pFrame =(CMainFrame*)AfxGetMainWnd();

		int nCurChannelNum = pFrame->m_wndChannelInfo.m_pFormView->m_nChannelNo;
#if _DEBUG
//////////////////////////////////////////////////////////////////////////
		CString str = strPath.Left(strPath.GetLength() - 4);
		str = str + _T(".txt");
		CStdioFile file2(str, CFile::modeCreate | CFile::modeReadWrite);

		POSITION pos2 = m_lstAScanData[nCurChannelNum].GetHeadPosition();

		while (pos2)
		{
			CPicData data = m_lstAScanData[nCurChannelNum].GetNext(pos2);

			for (int i = 0; i< 512; i++)
			{
				CString strContent;
				strContent.Format(_T("%d\n"), data.m_wData[i]);
				file2.WriteString(strContent);
			}

			file2.WriteString(_T("****************************************************************\n"));
		}

		file2.Close();

//////////////////////////////////////////////////////////////////////////
#endif
		// machao modify start on 2013-11-26
		CMechanicalControlPanel* pPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		CString strAScanPath = pPanel->m_AsyncAScanFile.GetFilePath();
		// Song Chenguang add start on 2014-01-16
		CString strPath2 = strPath.Left(strPath.GetLength() - 4) +_T(".paaf");
		CString strAScanPrimitivePath = pPanel->m_AsyncAScanPrimitiveFile.GetFilePath();
		// Song Chenguang add end on 2014-01-16
		// wangbingfu add start on 2014-09-10
		CString strAScanPath1 = pPanel->m_AsyncAScanFile1.GetFilePath();
		CString strPath1 = strPath.Left(strPath.GetLength() - 4) +_T("1.aaf");
		// wangbingfu add end on 2014-09-10
		if (PathFileExists(strAScanPath))
		{
			CopyFile(strAScanPath, strPath, true);
			CopyFile(strAScanPath1, strPath1, true); // wangbingfu add on 2014-09-10
			CopyFile(strAScanPrimitivePath, strPath2, true);	// Song Chenguang add on 2014-01-16
		}
		else
		{
			AfxMessageBox(_T("ɨ��ʱδ�洢Aɨ����"));
		}
// 		CFile file;
// 		CFileException e;
//
// 		if (file.Open(strPath, CFile::modeCreate | CFile::modeReadWrite, &e))
// 		{
// 			POSITION pos = m_lstAScanData[nCurChannelNum].GetHeadPosition();
//
// 			while (pos)
// 			{
// 				CPicData data = m_lstAScanData[nCurChannelNum].GetNext(pos);
//
// 				file.Write(&data, sizeof(CPicData));
// 			}
//
// 			file.Close();
// 		}
// 		else
// 		{
// 			e.ReportError();
// 		}
		// machao modify end on 2013-11-26
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestDoc::InitChannel"),"Leave");
}
