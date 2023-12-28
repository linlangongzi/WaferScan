// FeatureInfoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "FeatureInfoWnd.h"
#include "GlobalFunction.h"
#include "const.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "CScanDlgBar.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "BScanView.h"
#include "SysParam.h"				// wangbingfu add at 2012-12-5
#include "MechanicalControlPanel.h"	// wangbingfu add at 2012-12-5
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"		// wangbingfu add at 2012-12-5
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"				// wangbingfu add at 2012-12-5
// CFeatureInfoWnd
const WORD FEATURE_INTERVAL = 120;
extern short g_nWaveSpeed;
extern DWORD g_dwActiveChannel;
static int s_nHeightCount = 0; // 厚度计数 wangbingfu add at 2012-12-5
static float s_fHeight = 0;	// 工件厚度 wangbingfu add at 2012-12-5
extern BOOL g_bDrawRealTime;	// wangbingfu add at 2012-12-5
IMPLEMENT_DYNAMIC(CFeatureInfoWnd, CWnd)

CFeatureInfoWnd::CFeatureInfoWnd()
: m_bMPFTimeDisplay(FALSE), m_bMPMTimeDisplay(FALSE), m_bMPMAltitudeDisplay(FALSE),
  m_bFPFTimeDisplay(FALSE), m_bFPMTimeDisplay(FALSE), m_bFPMAltitudeDisplay(FALSE),
  m_nGateOpenNum(0), m_nFeatureOpenNum(0)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CFeatureInfoWnd"),"Enter");
	for (int i=0; i<4; i++)
	{
		m_bGateOpen[i] = FALSE;
	}	

	m_pOldBmp = NULL;	// Song Chenguang add on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CFeatureInfoWnd"),"Leave");
}

CFeatureInfoWnd::~CFeatureInfoWnd()
{
}

BEGIN_MESSAGE_MAP(CFeatureInfoWnd, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE(WM_FEATURE_INFO, &CFeatureInfoWnd::OnReceiveFeatureInfo)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
//Machao add start on 2013-07-1
	ON_WM_CLOSE()
//Machao add end on 2013-07-1
END_MESSAGE_MAP()



// CFeatureInfoWnd message handlers

void CFeatureInfoWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	DrawInfo(&dc);
}

//Machao add start on 2013-07-1
void CFeatureInfoWnd::OnClose()
{
	CMainFrame* pFrame1 = (CMainFrame*)AfxGetMainWnd();
	pFrame1->OnFeatureInfo();
}
//Machao add end on 2013-07-1

/** 
* 显示数据
* @return    void
* @since     1.0
*/
void CFeatureInfoWnd::DrawInfo(CDC* pDC)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::DrawInfo"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CFeatureInfoWnd::DrawInfo  pDC"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CFeatureInfoWnd::DrawInfo"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	
	// 特征值
	CString strTitles[7] = {_T("同步时间(us)"),_T("FP_FTime(mm)"),_T("FP_MTime(mm)"),_T("MP_FTime(mm)"),
		_T("MP_MTime(mm)"),_T("FP_MAltitude(%)"), _T("MP_MAltitude(%)")};

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	long lCharHeight = tm.tmHeight + tm.tmExternalLeading;

	int yGate = lCharHeight;
	CString strGates[4] = {_T("闸门I"), _T("闸门A"), _T("闸门B"), _T("闸门C")};
	for (int i=0; i<4; i++)
	{
		if(m_bGateOpen[i])
		{
			pDC->TextOut(0, yGate, strGates[i]);

			yGate += lCharHeight;
		}
	}

	BOOL bFeatureOpen[7] = {TRUE, m_bFPFTimeDisplay, m_bFPMTimeDisplay, m_bMPFTimeDisplay, 
		m_bMPMTimeDisplay, m_bFPMAltitudeDisplay, m_bMPMAltitudeDisplay};
	
	int k = 0;
	for (int i=0; i<7; i++)
	{
		if(bFeatureOpen[i])
		{
			for (int j=0; j < (m_nGateOpenNum + 1); j++)
			{
				int y = lCharHeight * j;

				if (j==0)
				{
					pDC->TextOut((k+1) * FEATURE_INTERVAL, y, strTitles[i]);
				}
				else
				{
					CString str;
					if ( j - 1 == 0 && k == 0)
					{
						CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
						int nDotNum = int(pFrame->m_wndChannelInfo.m_pFormView->m_fRange * 2 / (pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
						int nBeiShu = nDotNum / 16384;
						str.Format(_T("%.2f"), m_GateFeature.m_pfFeatures[j-1][k] * (nBeiShu + 1));
					}
					else
					{
						str.Format(_T("%.2f"), m_GateFeature.m_pfFeatures[j-1][k]);
					}				
					pDC->TextOut((k+1) * FEATURE_INTERVAL, y, str);
				}
			}

			k++;
		}
		else
			continue;
		
	}

	CString strX;
	strX.Format(_T("X = %.2f mm"), m_GateFeature.m_dX);
	pDC->TextOut(0, lCharHeight*5, strX);
	CString strY;
	strY.Format(_T("Y = %.2f mm"), m_GateFeature.m_dY);
	pDC->TextOut(0, lCharHeight*6, strY);
	CString strZ;
	strZ.Format(_T("Z = %.2f mm"), m_GateFeature.m_dZ);
	pDC->TextOut(0, lCharHeight*7, strZ);
	CString strW1;
	strW1.Format(_T("R = %.2f 度"), m_GateFeature.m_dR);
	pDC->TextOut(0, lCharHeight*8, strW1);
	CString strA;
	strA.Format(_T("A = %.2f mm"), m_GateFeature.m_dA);
	pDC->TextOut(0, lCharHeight*9, strA);
	CString strB;
	strB.Format(_T("B = %.2f mm"), m_GateFeature.m_dB);
	pDC->TextOut(0, lCharHeight*10, strB);
	CString strW2;
	strW2.Format(_T("W = %.2f 度"), m_GateFeature.m_dW);
	pDC->TextOut(0, lCharHeight*11, strW2);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::DrawInfo"),"Leave");
}

/** 
* 接收数据消息处理函数
* @param     wParam [in]: 扫查数据指针
* @param     lParam [in]: 卡信息（卡号、卡内通道序号）指针
* @return    void
* @since     1.0
*/
LRESULT CFeatureInfoWnd::OnReceiveFeatureInfo(WPARAM wParam, LPARAM lParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnReceiveFeatureInfo"),"Enter");
	CH_FLAWDATA_CSCAN_S * pData = (CH_FLAWDATA_CSCAN_S*)wParam;
	CARD_INFO_t* pCardInfo = (CARD_INFO_t*)lParam;

	// 获取闸门开关状态、特征值开关状态
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	for (int i=0; i<4; i++)
	{
		m_bGateOpen[i] = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_GateInfo[i].m_bOpen;
	}

	m_bMPFTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay;
	m_bMPMTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay;
	m_bMPMAltitudeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay;
	m_bFPFTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay;
	m_bFPMTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay;
	m_bFPMAltitudeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay;

	// 统计闸门开的个数
	m_nGateOpenNum = CountGateOpenNum();
	// 统计特征值开的个数
	m_nFeatureOpenNum = CountFeatureOpenNum();

	const int nGateNum = m_nGateOpenNum;
	const int nFeatureNum = m_nFeatureOpenNum+1;

	m_GateFeature = CFeatureInfo(nGateNum, nFeatureNum);

	GetCScanDataCoordinate(*pData, m_GateFeature.m_dX, m_GateFeature.m_dY, m_GateFeature.m_dZ, 
								m_GateFeature.m_dR, m_GateFeature.m_dB, m_GateFeature.m_dW, 
								m_GateFeature.m_dA, 1, 0, pCardInfo->nCardIndex, pCardInfo->nChannelIndex);
	int i = 0;
	CChannelInfo info = pFrame->GetChannelInfo((WORD)g_dwActiveChannel);
	BOOL bMTPmode[4] = {FALSE, FALSE, FALSE, FALSE};
	float fDelay[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	DWORD dwDelayDot[4] = {0, 0, 0, 0};
	
	for (int i=0; i<4; i++)
	{
		bMTPmode[i] = info.m_GateInfo[i].m_nMtpMode;
		fDelay[i] = info.m_fChannelDelay;
		dwDelayDot[i] = MicroSecond2Dot(fDelay[i]);
	}	
	
	// wangbingfu add start at 2012-12-5
	float fBiaoMianBoRange = 0;	// 表面波
	float fFirstDiBoRange = 0;	// 底波
	// wangbingfu add end at 2012-12-5

	if(m_bGateOpen[0])
	{
		int j = 1;

		m_GateFeature.m_pfFeatures[i][0] = float(pData->wGate1SyncTime) / ADFREQUENCY + fDelay[0] ;
	
		if (m_bFPFTimeDisplay)
		{
			j++;
		}

		if (m_bFPMTimeDisplay)
		{
			j++;
		}

		if(m_bMPFTimeDisplay)
		{
			j++;
		}

		if (m_bMPMTimeDisplay)
		{
			// wangbingfu modify start at 2012-12-5
			float fRange = 0;
			if(bMTPmode[0])
			{
				//Dot2Range改为Dot2RangeChannel  yachang.huang modify on 2014-6-5
				fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate1MPTime);
				m_GateFeature.m_pfFeatures[i][j] = fRange;
			}
			else
			{
				//Dot2Range改为Dot2RangeChannel  yachang.huang modify on 2014-6-5
				fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate1MPTime + dwDelayDot[0]);
				m_GateFeature.m_pfFeatures[i][j] = fRange;
			}

			j++;
			fBiaoMianBoRange = fRange;	// wangbingfu add at 2012-12-5
			// wangbingfu modify end at 2012-12-5
		}
		
		if (m_bFPMAltitudeDisplay)
		{
			j++;
		}

		if (m_bMPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate1MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}
		
		i++;
	}

	if (m_bGateOpen[1])
	{
		int j = 1;
		if (m_bFPFTimeDisplay)
		{
			if (bMTPmode[1])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2FPFirstTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2FPFirstTime + dwDelayDot[1]);

			j++;
		}

		if (m_bFPMTimeDisplay)
		{
			if (bMTPmode[1])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2FPTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2FPTime + dwDelayDot[1]);

			j++;
		}

		if(m_bMPFTimeDisplay)
		{
			j++;
		}

		if (m_bMPMTimeDisplay)
		{
			// wangbingfu modify start at 2012-12-5
			float fRange = 0;
			if(bMTPmode[1])
			{
				fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2MPTime);
				m_GateFeature.m_pfFeatures[i][j] = fRange;
			}
			else
			{
				fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2MPTime + dwDelayDot[1]);
				m_GateFeature.m_pfFeatures[i][j] = fRange;
			}

			j++;
	//		fFirstDiBoRange = m_GateFeature.m_pfFeatures[i][j];	// wangbingfu add at 2012-12-5,值不对
			fFirstDiBoRange = fRange;
			// wangbingfu modify end at 2012-12-5
		}

		if (m_bFPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate2FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (m_bMPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate2MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if(m_bGateOpen[2])
	{
		int j = 1;
		if (m_bFPFTimeDisplay)
		{
			if(bMTPmode[2])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3FPFirstTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3FPFirstTime + dwDelayDot[2]);

			j++;
		}

		if (m_bFPMTimeDisplay)
		{
			if(bMTPmode[2])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3FPTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3FPTime + dwDelayDot[2]);

			j++;
		}

		if(m_bMPFTimeDisplay)
		{
			j++;
		}

		if (m_bMPMTimeDisplay)
		{
			if(bMTPmode[2])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3MPTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3MPTime + dwDelayDot[2]);

			j++;
		}

		if (m_bFPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate3FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (m_bMPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate3MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if (m_bGateOpen[3])
	{
		int j = 1;
		if (m_bFPFTimeDisplay)
		{
			if(bMTPmode[3])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4FPFirstTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4FPFirstTime + dwDelayDot[3]);

			j++;
		}

		if (m_bFPMTimeDisplay)
		{
			if(bMTPmode[3])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4FPTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4FPTime + dwDelayDot[3]);

			j++;
		}

		if(m_bMPFTimeDisplay)
		{
			j++;
		}

		if (m_bMPMTimeDisplay)
		{
			if(bMTPmode[3])
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4MPTime);
			else
				m_GateFeature.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4MPTime +dwDelayDot[3]);

			j++;
		}

		if (m_bFPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate4FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (m_bMPMAltitudeDisplay)
		{
			m_GateFeature.m_pfFeatures[i][j] = (pData->wGate4MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

 	delete pData;
 	pData = NULL;
	delete pCardInfo;
	pCardInfo = NULL;

	Invalidate(FALSE);

	// wangbingfu add start at 2012-12-5
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bGateAdjust && g_bDrawRealTime)
	{
		CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		int nGroupNum = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexProbeNum  + 1;
		WORD wWorkMode = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode;
		if(nGroupNum == 1 && wWorkMode == 6)
		{
			float fHeight = fFirstDiBoRange - fBiaoMianBoRange;
			if(s_nHeightCount < 5)
			{
				s_fHeight += fHeight;
				s_nHeightCount++;
			}
			else
			{
				s_nHeightCount = 0;
				s_fHeight = s_fHeight / 5.0f;
				float fStandartHeight = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;	// 标称厚度
				float fChaZhi = s_fHeight - fStandartHeight;
				float fNewGateBack = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fOldGateBBack + fChaZhi;
				CString str = _T("");
				str.Format(_T("%.1f"),fNewGateBack);
				fNewGateBack = _tstof(str);
				if(fNewGateBack > 0)
				{
				//	pFrame->m_wndChannelInfo.m_pFormView->m_fGateBack = fNewGateBack;
					pFrame->m_wndChannelInfo.m_pFormView->DoModifyGateWidth(2, fNewGateBack);
				}
				s_fHeight = 0;
				
			}
		}
	}
	
	// wangbingfu add end at 2012-12-5
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnReceiveFeatureInfo"),"Leave");
	return 0;
}

BOOL CFeatureInfoWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnEraseBkgnd"),"Enter");
	CRect rect;
	GetClientRect(&rect);
	static int nCount = 0;
	if(nCount == 0)
	{
		BOOL bRet = m_bmpBack.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		if (bRet)
		{
			m_pOldBmp = pDC->SelectObject(&m_bmpBack);
			nCount++;
		}		
	}
	
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnEraseBkgnd"),"Leave");

	return TRUE;
}

int CFeatureInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnCreate"),"Enter");
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnCreate"),"Leave");
		return -1;
	}

	// TODO:  Add your specialized creation code here
	// 禁止关闭操作
	//Machao delete start on 2013-07-1
// 	CMenu* pSysMenu = GetSystemMenu(FALSE);
// 	if (pSysMenu != NULL)
// 	{
// 		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_DISABLED);
// 	}
	//Machao delete end on 2013-07-1
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnCreate"),"Leave");
	return 0;
}

void CFeatureInfoWnd::OnDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnDestroy"),"Enter");
	CWnd::OnDestroy();
	// TODO: Add your message handler code here
	// Song Chenguang add start on 2012-11-05
	CDC* pDC = GetDC();
	pDC->SelectObject(m_pOldBmp);
	DeleteObject(m_bmpBack);
	// Song Chenguang add end on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::OnDestroy"),"Leave");
}

/** 
* 计算打开的闸门数                                            
* @return    void
* @since     1.0
*/
int CFeatureInfoWnd::CountGateOpenNum()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CountGateOpenNum"),"Enter");
	int nCount = 0;

	for (int i=0; i<4; i++)
	{
		if (m_bGateOpen[i])
			nCount++;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CountGateOpenNum"),"Leave");
	return nCount;
}

/** 
* 计算打开的特征值数                                            
* @return    void
* @since     1.0
*/
int CFeatureInfoWnd::CountFeatureOpenNum()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CountFeatureOpenNum"),"Enter");
	int nCount = 0;

	if(m_bMPFTimeDisplay)
		nCount++;

	if(m_bMPMTimeDisplay)
		nCount++;

	if(m_bMPMAltitudeDisplay)
		nCount++;

	if(m_bFPFTimeDisplay)
		nCount++;

	if(m_bFPMTimeDisplay)
		nCount++;

	if(m_bFPMAltitudeDisplay)
		nCount++;
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CFeatureInfoWnd::CountFeatureOpenNum"),"Leave");
	return nCount;
}
