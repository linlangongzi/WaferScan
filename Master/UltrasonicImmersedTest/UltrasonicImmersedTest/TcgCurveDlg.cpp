// TcgCurveDlg.cpp : CTcgCurveDlg 类的实现

/**
*  Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* TCG曲线面板窗体源文件
* Filename: TcgCurveDlg.cpp
* 
* @author       songchenguang  zhaoli
* @version      2.0         Date: 2013-10-29
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TcgCurveDlg.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "FeatureInfoWnd.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "GlobalFunction.h"
#include "ListCtrlEditable.h"
#include "math.h"
#include "UltrasonicImmersedTestView.h"	// wangbingfu add at 2012-11-20


const static int nAddDotNum = 0;
extern DWORD g_dwActiveChannel;
extern short g_nWaveSpeed;
// CTcgCurveDlg dialog

IMPLEMENT_DYNAMIC(CTcgCurveDlg, CDialog)

CTcgCurveDlg::CTcgCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcgCurveDlg::IDD, pParent)
	, m_fCurrentPointRange(0)
	, m_fCurrentPointAmp(0)
	, m_bSwitchFlag(TRUE)
	, m_nCurPointNum(0)
	, m_nTcgOnMode(0)	// wangbingfu add at 2012-11-19
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::CTcgCurveDlg"),"Enter");
	memset(m_stTcgCurve, 0, sizeof(m_stTcgCurve));
	m_bMakeTcgFlag = FALSE;
	m_nSelectPoint = 0;
	m_colorTcg = RGB(255, 0, 0);
	m_nCurPointNum = 0;	
	memset(m_fTcgSramGain, 0, sizeof(m_fTcgSramGain));
	m_nAllDotNum = 0;
	m_nFrontAddDotNum = 0;
	m_wSamplingInterval = 1;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::CTcgCurveDlg"),"Leave");
}

CTcgCurveDlg::~CTcgCurveDlg()
{
}

void CTcgCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::DoDataExchange"),"Enter");
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TCG_RANGE, m_fCurrentPointRange);
	DDX_Text(pDX, IDC_EDIT_TCG_AMP, m_fCurrentPointAmp);
	DDX_Radio(pDX, IDC_RADIO_TCG_ON, m_bSwitchFlag);
	DDX_Text(pDX, IDC_EDIT_TCG_CURRENT_POINT_NUM, m_nCurPointNum);
	DDX_Control(pDX, IDC_LIST_TCG, m_lstTcg);
	DDX_Radio(pDX, IDC_RADIO_TCG_SOFT, m_nTcgOnMode);	// wangbingfu add at 2012-11-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::DoDataExchange"),"Leave");
}


BEGIN_MESSAGE_MAP(CTcgCurveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TCG_CREATE, &CTcgCurveDlg::OnBnClickedButtonTcgCreate)
	ON_BN_CLICKED(IDC_BUTTON_TCG_ADD_POINT, &CTcgCurveDlg::OnBnClickedButtonTcgAddPoint)
	ON_BN_CLICKED(IDC_BUTTON_PREV_TCG_POINT, &CTcgCurveDlg::OnBnClickedButtonPrevTcgPoint)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_TCG_POINT, &CTcgCurveDlg::OnBnClickedButtonNextTcgPoint)
	ON_BN_CLICKED(IDC_BUTTON_TCG_DELETE_POINT, &CTcgCurveDlg::OnBnClickedButtonTcgDeletePoint)
	ON_BN_CLICKED(IDC_BUTTON_TCG_MODIFY_CONFIRM, &CTcgCurveDlg::OnBnClickedButtonTcgModifyConfirm)
	ON_BN_CLICKED(IDC_RADIO_TCG_ON, &CTcgCurveDlg::OnBnClickedRadioTcgOn)
	ON_BN_CLICKED(IDC_RADIO_TCG_OFF, &CTcgCurveDlg::OnBnClickedRadioTcgOff)
	ON_BN_CLICKED(IDC_BUTTON_TCG_FILE_OPEN, &CTcgCurveDlg::OnBnClickedButtonTcgFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_TCG_FILE_SAVE, &CTcgCurveDlg::OnBnClickedButtonTcgFileSave)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_TCG, &CTcgCurveDlg::OnLvnEndlabeleditListTcg)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_TCG, &CTcgCurveDlg::OnLvnBeginlabeleditListTcg)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CTcgCurveDlg::OnHdnItemchangedListTcg)
	ON_BN_CLICKED(IDC_BUTTON_TCG_SRAM_MODIFY, &CTcgCurveDlg::OnBnClickedButtonTcgSramModify)
	ON_BN_CLICKED(IDC_RADIO_TCG_SOFT, &CTcgCurveDlg::OnBnClickedRadioTcgSoft)	// wangbingfu add at 2012-11-19
	ON_BN_CLICKED(IDC_RADIO_HARDWARE, &CTcgCurveDlg::OnBnClickedRadioHardware)	// wangbingfu add at 2012-11-19
END_MESSAGE_MAP()


// CTcgCurveDlg message handlers

/** 
* 新建TCG曲线功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgCreate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgCreate"),"Enter");
	// TODO: Add your control notification handler code here
	if (IDCANCEL == MessageBox(_T("将会删除原有曲线，是否继续"),_T("提示"), MB_OKCANCEL))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgCreate"),"Leave");
		return;
	}
	::ZeroMemory(m_stTcgCurve, sizeof(m_stTcgCurve));
	// 刷新控件显示
	float fCurrentPointRange = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fRange;
	float fCurrentPointAmp = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fData / MAX_HEIGHT * 100;
	m_nCurPointNum = 0;
	CString str;
	str.Format(_T("%.1f"),fCurrentPointRange);
	m_fCurrentPointRange = static_cast<float>(_tstof(str));
	str.Format(_T("%.1f"),fCurrentPointAmp);
	m_fCurrentPointAmp = static_cast<float>(_tstof(str));
	UpdateData(FALSE);
	CalcTcgSram();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgCreate"),"Leave");
}

/** 
* 获取TCG曲线最高点
* @param     stCurveForSearch[] [in]：TCG曲线样点
* @param     nPointNum [in]：TCG曲线样点个数
* @return    CURVEPOINT_DATA_t [out]：最高点
* @since     1.0
*/
CURVEPOINT_DATA_t CTcgCurveDlg:: GetHeightestPoint(CURVEPOINT_DATA_t stCurveForSearch[], int nPointNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::GetHeightestPoint"),"Enter");
	CURVEPOINT_DATA_t stTempPoint;
	memset(&stTempPoint, 0, sizeof(CURVEPOINT_DATA_t));
	if(nPointNum > 0)
	{		
		stTempPoint = stCurveForSearch[0];
		for(int i = 0; i < nPointNum; i++)//modify wangqianfeng 20111102
		{
			if(stTempPoint.fData < stCurveForSearch[i].fData)
			{
				stTempPoint = stCurveForSearch[i];
			}
		}
	}	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::GetHeightestPoint"),"Leave");
	return stTempPoint;
}

/** 
* 添加TCG曲线样点功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgAddPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgAddPoint"),"Enter");
	// TODO: Add your control notification handler code here
	// 用闸门A加样点
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	UpdateData(TRUE);
	if(!pFeatureWnd->m_bGateOpen[0])	// 闸门1没开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先开打开闸门1"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgAddPoint"),"Leave");
		return;
	}

	if(!pFeatureWnd->m_bMPMAltitudeDisplay)	// 最大波峰幅值没有打开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先打开最大波峰幅值"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgAddPoint"),"Leave");
		return;
	}
	if(!pFeatureWnd->m_bMPMTimeDisplay)	// 最大波峰幅值没有打开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先打开最大波峰时间"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgAddPoint"),"Leave");
		return;
	}

	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = TRUE;
	bFeatureFlag[1] = pFeatureWnd->m_bFPFTimeDisplay;
	bFeatureFlag[2] = pFeatureWnd->m_bFPMTimeDisplay;
	bFeatureFlag[3] = pFeatureWnd->m_bMPFTimeDisplay;
	bFeatureFlag[4] = pFeatureWnd->m_bMPMTimeDisplay;
	bFeatureFlag[5] = pFeatureWnd->m_bFPMAltitudeDisplay;
	bFeatureFlag[6] = pFeatureWnd->m_bMPMAltitudeDisplay;

	int nTimeIndex = 0;			//最大波峰时间索引
	for(int i = 0; i < 4; i++)
	{
		if(bFeatureFlag[i])
		{
			nTimeIndex++;
		}

	}
	int nAmpIndex = 0;			//最大波峰幅值索引
	for(int i = 0; i < 6; i++)
	{
		if(bFeatureFlag[i])
		{
			nAmpIndex++;
		}
	}
	if(m_stTcgCurve[g_dwActiveChannel].nPointNum < DAC_CURVE_POINT_NUM)
	{
		m_stTcgCurve[g_dwActiveChannel].nPointNum++;
		int nPointNum = m_stTcgCurve[g_dwActiveChannel].nPointNum;
		
		float fTotalPlus = 0;
		if(nPointNum == 1)	// 第一点，记下此时增益
		{	
			fTotalPlus = GetTotalPlus(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
			m_stTcgCurve[g_dwActiveChannel].fFirstGain = fTotalPlus;
		}
		
		fTotalPlus = GetTotalPlus(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		// 制作线的点
		float fChangeGain = m_stTcgCurve[g_dwActiveChannel].fFirstGain - fTotalPlus;
		// 用闸门A加样点
		m_stTcgCurve[g_dwActiveChannel].stPointData[nPointNum - 1].fData =  CurveDataChangeByGain(pFeatureWnd->m_GateFeature.m_pfFeatures[1][nAmpIndex], fChangeGain);
		float fSyncRange = pChannelPanel->Dot2RangeChannel(pChannelPanel->m_nWaveSpeed, pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * ADFREQUENCY);// 同步声程
		m_stTcgCurve[g_dwActiveChannel].stPointData[nPointNum - 1].fRange = pFeatureWnd->m_GateFeature.m_pfFeatures[1][nTimeIndex];		

		SortCurvePointByRange(m_stTcgCurve[g_dwActiveChannel].stPointData, nPointNum);

		m_nSelectPoint = nPointNum - 1;

		DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		CalcTcgSram();
		if(m_stTcgCurve[g_dwActiveChannel].bSwitchFlag)
		{
			SendTcgCurve();
		}
	}
	else
	{
		AfxMessageBox(_T("加点失败,最多添加20个点"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgAddPoint"),"Leave");
		return;
	}
}

void CTcgCurveDlg::CalcTcgSram()
{	
	int nPointNum = m_stTcgCurve[g_dwActiveChannel].nPointNum;
	if(nPointNum > 1)
	{	
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
		CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
		::ZeroMemory(m_fTcgSramGain, sizeof(m_fTcgSramGain));
		
		int nAllDotNum = 0;
		float fSampleCalc = ADFREQUENCY * 1000000 / 50.0;
		CURVEPOINT_DATA_t stHeightestPoint = GetHeightestPoint(m_stTcgCurve[g_dwActiveChannel].stPointData, nPointNum - 1);

		float fAnaslyRange = m_stTcgCurve[g_dwActiveChannel].stPointData[0].fRange;// 第一点到同步时间的声程

       // float fAnaslyRangeTemp = m_stTcgCurve[g_dwActiveChannel].stPointData[nPointNum - 1].fRange - m_stTcgCurve[g_dwActiveChannel].stPointData[0].fRange;//最后一点到第一点的声称差
		float fAnaslyRangeTemp = m_stTcgCurve[g_dwActiveChannel].stPointData[nPointNum - 1].fRange ;//最后一点到零点的声称差
		m_wSamplingInterval = (WORD)((((fAnaslyRangeTemp * 2 / 1000) * 80 * 1000000) / pChannelPanel->m_nWaveSpeed )/ 1024) + 1;//采样间隔

		m_nAllDotNum = (((fAnaslyRangeTemp * 2 / 1000) * 80 * 1000000) / pChannelPanel->m_nWaveSpeed ) / m_wSamplingInterval;//全部采样点数

		int nAnaslyDotNum = (fAnaslyRange /fAnaslyRangeTemp) * m_nAllDotNum;// 第一点到同步时间的采样点数量
		m_nFrontAddDotNum = nAddDotNum + nAnaslyDotNum;	//第一点之前增加的点
		for(int i = 0; i < nPointNum - 1; i++)
		{
			CURVEPOINT_DATA_t stTempPoint1 = m_stTcgCurve[g_dwActiveChannel].stPointData[i];
			CURVEPOINT_DATA_t stTempPoint2 = m_stTcgCurve[g_dwActiveChannel].stPointData[i+1];
			float fRange = stTempPoint2.fRange - stTempPoint1.fRange;
             //yachang.huang rum on 2014-6-18
			//int nDotNum = (int)((fRange * 2 / 1000) * fSampleCalc / pChannelPanel->m_nWaveSpeed); //两点之间的采样点数
			int nDotNum = ((m_stTcgCurve[g_dwActiveChannel].stPointData[i+1].fRange - m_stTcgCurve[g_dwActiveChannel].stPointData[i].fRange)/fAnaslyRangeTemp) * m_nAllDotNum;

			for(int j = 0; j < nDotNum; j++)
			{
				float fData = static_cast<float>(stTempPoint1.fData + ((double)stTempPoint2.fData - (double)stTempPoint1.fData) / nDotNum * j);
				//yachang.huang rum on 2014-6-18
				//m_fTcgSramGain[nAllDotNum + j + m_nFrontAddDotNum] = (float)(20 * log10((double)(stHeightestPoint.fData / fData)));
				m_fTcgSramGain[nAllDotNum + j + m_nFrontAddDotNum] = (float)(20 * log10((double)(stHeightestPoint.fData / fData)));
			}

			nAllDotNum += nDotNum;

			if(i == 0)	//第一曲线点之前的采样点
			{
				for(int j = 0; j < m_nFrontAddDotNum; j++)
				{
					m_fTcgSramGain[j] = 0;
				}
			}

			if(i == nPointNum - 2)
			{
				for(int j = 0; j < nAddDotNum; j++)
				{
					m_fTcgSramGain[nAllDotNum + m_nFrontAddDotNum + j] = m_fTcgSramGain[nAllDotNum + m_nFrontAddDotNum - 1];
				}
			}
	
		}
		//yachang.huang rum on 2014-6-18
		//m_nAllDotNum = nAllDotNum + nAddDotNum + m_nFrontAddDotNum;
	}
	else
	{
		m_nAllDotNum = 0;
		for(int i = 0; i < TCG_SRAM_NUM; i++)
		{
			m_fTcgSramGain[i] = 0;
		}
	}
	
	UpdateList(&m_lstTcg);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::CalcTcgSram"),"Leave");
}

//前后沿水平延长8个采样点的声程，前后各补一个点
BOOL CTcgCurveDlg::BuDian(CURVEPOINT_DATA_t stCurveToBuDian[],CURVEPOINT_DATA_t stCurveAfterBuDian[], int nPointNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::BuDian"),"Enter");
	CURVEPOINT_DATA_t stTempPoint[TCG_CURVE_POINT_NUM+2];
	memset(stTempPoint, 0, sizeof(stTempPoint));
	if(nPointNum < 2)
		return FALSE;
	else
	{
	/*	for(int i = 0; i < nPointNum; i++)
		{
			stTempPoint[i + 1] = stCurveToBuDian[i];
		}
		stTempPoint[0] =  stCurveToBuDian[0];
		stTempPoint[nPointNum + 1] = stCurveToBuDian[nPointNum - 1];
		for(int i = 0; i < nPointNum + 2; i++)
		{
			stCurveAfterBuDian[i] = stTempPoint[i];
		}*/
		stTempPoint[0] =  stCurveToBuDian[0];
		stTempPoint[nPointNum + 1] = stCurveToBuDian[nPointNum - 1];
		for(int i = 0; i < nPointNum; i++)
		{
			stTempPoint[i + 1] = stCurveToBuDian[i];
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::BuDian"),"Leave");
	return TRUE;
}

/** 
* 添加增益列表数据
* @param     pListCtrl [in]：列表数据
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::UpdateList(CListCtrlEditable* pListCtrl)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::UpdateList"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pListCtrl )
	{
		MessageBox(_T("CTcgCurveDlg::UpdateList,pListCtrl = null"), _T("警告"), MB_OK|MB_ICONERROR);
		return;
	}
	// WangQianfeng add end on 2012-11-02
	pListCtrl->DeleteAllItems(); //删除所有行

	//yachang.huang rum on 2014-6-18
	for(int i = m_nFrontAddDotNum; i < m_nAllDotNum - nAddDotNum; i++) //不显示第一个曲线点和后8个补的点
//	for(int i = 0; i < m_nAllDotNum - nAddDotNum; i++) //不显示前8个和后8个补的点
	{
		LV_ITEM lvi;

		lvi.mask = LVIF_TEXT;// | LVIF_PARAM;
		lvi.iItem = pListCtrl->GetItemCount();

		lvi.iSubItem = 0;
		CString strIndex;
		strIndex.Format(_T("%d"), lvi.iItem);
		lvi.pszText = strIndex.GetBuffer(strIndex.GetLength());	// 序号
		int nRes = pListCtrl->InsertItem(&lvi);

		CString strGain;
		strGain.Format(_T("%.2f"), m_fTcgSramGain[i]);
		pListCtrl->SetItemText(lvi.iItem, 1, strGain);	// minX
	}	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::UpdateList"),"Leave");
}

BOOL CTcgCurveDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnInitDialog"),"Enter");
	CDialog::OnInitDialog();
	
	// 创建序号和增益列表
	InitResultList(&m_lstTcg);
	// TODO:  Add extra initialization here
/*	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	m_pFeatureWnd = pFrame->m_pFeatureWnd;*/
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnInitDialog"),"Leave");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/** 
* 移到上一个样点功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonPrevTcgPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonPrevTcgPoint"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	if(m_stTcgCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点"));
	}
	else
	{
		if(m_nSelectPoint == 0)
		{
			m_nSelectPoint = m_stTcgCurve[g_dwActiveChannel].nPointNum - 1;
		}
		else
		{
			m_nSelectPoint--;
		}


		// 刷新控件显示
		DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonPrevTcgPoint"),"Leave");
}

/** 
* 移到下一个样点功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonNextTcgPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonNextTcgPoint"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	if(m_stTcgCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点"));
	}
	else
	{
		if(m_nSelectPoint + 1 == m_stTcgCurve[g_dwActiveChannel].nPointNum)
		{
			m_nSelectPoint = 0;
		}
		else
		{
			m_nSelectPoint++;
		}


		// 刷新控件显示
		DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonNextTcgPoint"),"Leave");
}

/** 
* 删除当前样点功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgDeletePoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgDeletePoint"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	int nPointNum =m_stTcgCurve[g_dwActiveChannel].nPointNum;
	if(m_stTcgCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点可删"));
	}
	else
	{
		if(m_nSelectPoint == nPointNum - 1)	// 删最后一点,选中点往前移
		{
			m_nSelectPoint --;
		}
		else
		{
			for(int i = m_nSelectPoint; i < nPointNum - 1; i++)	// 选中点后面的点数据往前移
			{
				m_stTcgCurve[g_dwActiveChannel].stPointData[i] =  m_stTcgCurve[g_dwActiveChannel].stPointData[i + 1];
			}
		}

		m_stTcgCurve[g_dwActiveChannel].nPointNum--;
		nPointNum = m_stTcgCurve[g_dwActiveChannel].nPointNum;

		// 刷新控件显示
	/*	float fCurrentPointRange = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fRange;
		float fCurrentPointAmp = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fData;
		m_nCurPointNum = m_nSelectPoint + 1;
		CString str;
		str.Format(_T("%.1f"),fCurrentPointRange);
		m_fCurrentPointRange = _tstof(str);
		str.Format(_T("%.1f"),fCurrentPointAmp);
		m_fCurrentPointAmp = _tstof(str);
		UpdateData(FALSE);*/

		// 刷新控件显示
		DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);

		CalcTcgSram();
		if(m_stTcgCurve[g_dwActiveChannel].bSwitchFlag)
		{
			SendTcgCurve();		// 发送TCG
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgDeletePoint"),"Leave");
}

/** 
* 确定修改声程和幅值数值功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgModifyConfirm()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgModifyConfirm"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	UpdateData(TRUE);
	int nCurPoint = m_nSelectPoint;
	m_stTcgCurve[g_dwActiveChannel].stPointData[nCurPoint].fData = m_fCurrentPointAmp;
	m_stTcgCurve[g_dwActiveChannel].stPointData[nCurPoint].fRange = m_fCurrentPointRange;
	SortCurvePointByRange(m_stTcgCurve[g_dwActiveChannel].stPointData, m_stTcgCurve[g_dwActiveChannel].nPointNum);

/*	float fCurrentPointRange = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fRange;
	float fCurrentPointAmp = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fData;
	float fChangeGain = pChannelPanel->m_fPlus - m_stTcgCurve[g_dwActiveChannel].fFirstGain;
	fCurrentPointAmp = CurveDataChangeByGain(fCurrentPointAmp, fChangeGain);
	CString str;
	str.Format(_T("%.1f"),fCurrentPointRange);
	m_fCurrentPointRange = _tstof(str);
	str.Format(_T("%.1f"),fCurrentPointAmp);
	m_fCurrentPointAmp = _tstof(str);
	m_nCurPointNum = m_nSelectPoint + 1;
	UpdateData(FALSE);*/

	// 刷新控件显示
	DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);

	CalcTcgSram();
	if(m_stTcgCurve[g_dwActiveChannel].bSwitchFlag)
	{
		SendTcgCurve();		// 发送TCG
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgModifyConfirm"),"Leave");
}

/** 
* 开启TCG曲线功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedRadioTcgOn()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedRadioTcgOn"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	UpdateData(TRUE);
	m_stTcgCurve[g_dwActiveChannel].bSwitchFlag = TRUE;
	
	// wangbingfu add start at 2012-11-19
	if(m_nTcgOnMode == 0)	// 软件
	{
		
	}
	else if(m_nTcgOnMode == 1) // 硬件
	{
		// 发送TCG
		SendTcgCurve();
	}
	// wangbingfu add start at 2012-11-19
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedRadioTcgOn"),"Leave");
}

/** 
* 发送TCG曲线的相关信息
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::SendTcgCurve()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::SendTcgCurve"),"Enter");
	// TODO: Add your control notification handler code here
/*	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	const int DATA_SIZE = sizeof(TCG_CURVE_t);
	char szSendData[NETHEAD_SIZE + DATA_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;
	stPacketHead.dwCommand = COMMAND_TCG_SWICH;
	stPacketHead.wChannelIndex = pChannelPanel->m_nChannelNo % 2;
	stPacketHead.wGateIndex = 0;
	stPacketHead.dwSize = DATA_SIZE;

	memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
	memcpy( &szSendData[NETHEAD_SIZE], &m_stTcgCurve[pChannelPanel->m_nChannelNo], DATA_SIZE);
	pFrame->SendData( szSendData, sizeof(szSendData), pChannelPanel->m_nChannelNo);
*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;


	TCG_COMMU_DATA_t stTcgSend;
	memset( &stTcgSend, 0, sizeof(stTcgSend));
//	memcpy(&stTcgSend.stTcgCurve, &m_stTcgCurve[pChannelPanel->m_nChannelNo], sizeof(TCG_CURVE_t));
	memcpy(stTcgSend.fSramGain, m_fTcgSramGain, sizeof(m_fTcgSramGain));
/*	for(int i = 0; i < TCG_SRAM_NUM; i++)
	{
		stTcgSend.fSramGain[i] = 10 * stTcgSend.fSramGain[i];
	}*/
	stTcgSend.nSramNum = m_nAllDotNum;
	stTcgSend.bOpenFlag = m_stTcgCurve[pChannelPanel->m_nChannelNo].bSwitchFlag;
	stTcgSend.wSamplingInterval = m_wSamplingInterval;//拷贝采样间隔的值下发  yachang.huang add on 2014-06-18

	const int DATA_SIZE = sizeof(stTcgSend);
	char szSendData[NETHEAD_SIZE + DATA_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;
	stPacketHead.dwCommand = COMMAND_TCG_SWICH;
	stPacketHead.wChannelIndex = pChannelPanel->m_nChannelNo % 2;
	stPacketHead.wGateIndex = 0;
	stPacketHead.dwSize = DATA_SIZE;

	memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
	memcpy( &szSendData[NETHEAD_SIZE], &stTcgSend, sizeof(stTcgSend));
	pFrame->SendData( szSendData, sizeof(szSendData), pChannelPanel->m_nChannelNo);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::SendTcgCurve"),"Leave");
}

/** 
* 关闭TCG曲线功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedRadioTcgOff()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedRadioTcgOff"),"Enter");
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_stTcgCurve[g_dwActiveChannel].bSwitchFlag = FALSE;
	
	// wangbingfu add start at 2012-11-19
	if(m_nTcgOnMode == 0)	// 软件
	{

	}
	else if(m_nTcgOnMode == 1) // 硬件
	{
		// 发送TCG
		SendTcgCurve();
	}
	// wangbingfu add end at 2012-11-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedRadioTcgOff"),"Leave");
}

/** 
* 打开TCG曲线文件功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgFileOpen()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgFileOpen"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	TCHAR szFilters[] =
		_T("TCG文件 (*.tcg)|*.tcg||");

	CFileDialog dlg(TRUE, _T("tcg"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite), &e)
		{		
			int nRet = MessageBox(_T("当前数据将被覆盖，同意点“是”，否则点“否”。"), _T("提示"), MB_YESNO);

			if (nRet == IDYES)
			{			
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					file.Read(&m_stTcgCurve[i], sizeof(TCG_CURVE_t));
				}

				int nCurChannel = g_dwActiveChannel;
				m_nSelectPoint = 0;


				DisplayValue(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
				file.Read(&m_nAllDotNum, sizeof(int));
				file.Read(m_fTcgSramGain, sizeof(float) * TCG_SRAM_NUM);
/*
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
				float fSampleCalc = ADFREQUENCY * 1000000 / 50.0;
				float fAnaslyRange = m_stTcgCurve[g_dwActiveChannel].stPointData[0].fRange;// 第一点到同步时间的声程
				int nAnaslyDotNum = (int)((fAnaslyRange * 2 / 1000) * fSampleCalc / pChannelPanel->m_nWaveSpeed);// 第一点到同步时间的采样点数量
				m_nFrontAddDotNum = nAddDotNum + nAnaslyDotNum;	//第一点之前增加的点
*/
                int nPointNum = m_stTcgCurve[g_dwActiveChannel].nPointNum;
				float fAnaslyRange = m_stTcgCurve[g_dwActiveChannel].stPointData[0].fRange;// 第一点到同步时间的声程
				float fAnaslyRangeTemp = m_stTcgCurve[g_dwActiveChannel].stPointData[nPointNum - 1].fRange ;//最后一点到零点的声称差
				m_wSamplingInterval = (WORD)((((fAnaslyRangeTemp * 2 / 1000) * 80 * 1000000) / pChannelPanel->m_nWaveSpeed )/ 1024) + 1;//采样间隔
				int nAnaslyDotNum = (fAnaslyRange /fAnaslyRangeTemp) * m_nAllDotNum;// 第一点到同步时间的采样点数量
				m_nFrontAddDotNum = nAddDotNum + nAnaslyDotNum;	//第一点之前增加的点




			}
			file.Close();
		}
		else
		{
			e.ReportError();
		}
		UpdateList(&m_lstTcg);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgFileOpen"),"Leave");
}

/** 
* 保存TCG曲线文件功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgFileSave()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgFileSave"),"Enter");
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] =
		_T("TCG文件 (*.tcg)|*.tcg||");

	CFileDialog dlg(FALSE, _T("tcg"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		
		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite | CFile::modeCreate), &e)
		{		
			for(int i = 0; i < CHANNEL_NUM; i++)
			{
				file.Write(&m_stTcgCurve[i], sizeof(TCG_CURVE_t));
			}
			file.Write(&m_nAllDotNum, sizeof(int));
			file.Write(m_fTcgSramGain, sizeof(float) * TCG_SRAM_NUM);
			file.Close();
		}
		else
		{
			e.ReportError();
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgFileSave"),"Leave");
}

/** 
* 更新对话框显示的值
* @param     fPlus [in]：增益
* @param     fRefPlus [in]：参考增益
* @param     nChnNo [in]：通道
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::DisplayValue(float fPlus, float fRefPlus, int nChnNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::DisplayValue"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	float fTotalPlus = 0;
	fTotalPlus = GetTotalPlus(fPlus, fRefPlus, nChnNo);



	float fCurrentPointRange = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fRange;// + fSyncRange; //改成不随同步声程变化
	float fCurrentPointAmp = m_stTcgCurve[g_dwActiveChannel].stPointData[m_nSelectPoint].fData;
	float fChangeGain = fTotalPlus - m_stTcgCurve[g_dwActiveChannel].fFirstGain;
	fCurrentPointAmp = CurveDataChangeByGain(fCurrentPointAmp, fChangeGain);
	CString str;
	str.Format(_T("%.1f"),fCurrentPointRange);
	m_fCurrentPointRange = static_cast<float>(_tstof(str));
	str.Format(_T("%.1f"),fCurrentPointAmp);
	m_fCurrentPointAmp = static_cast<float>(_tstof(str));
	m_nCurPointNum = m_nSelectPoint + 1;
	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::DisplayValue"),"Leave");
}

void CTcgCurveDlg::OnClose()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnClose"),"Enter");
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
//	KillTimer(TIMER_AUTO_GAIN);
	m_bMakeTcgFlag = FALSE;
	ShowWindow(SW_HIDE);
	pFrame->m_bTcgWndDisplayed = !pFrame->m_bTcgWndDisplayed;

	int nPointNum = m_stTcgCurve[g_dwActiveChannel].nPointNum;
	for(int i = 0; i < nPointNum; i++)
	{
		float fData = m_stTcgCurve[g_dwActiveChannel].stPointData[i].fData;
		float fRange = m_stTcgCurve[g_dwActiveChannel].stPointData[i].fRange;
	}
	//	CDialog::OnClose();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnClose"),"Leave");
}

/** 
* 创建序号和增益列表
* @param     pListCtrl [in]：列表数据
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::InitResultList(CListCtrlEditable* pListCtrl)
{
	if ( NULL == pListCtrl )
	{
		MessageBox(_T("CTcgCurveDlg::InitResultList,pListCtrl = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::InitResultList"),"Leave");
		return;
	}

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pListCtrl->SetFont(this->GetFont());

	pListCtrl->InsertColumn(0, _T("序号"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 60);
	pListCtrl->InsertColumn(1, _T("增益"), CListCtrlEditable::CtrlType_Edit, 0, true, LVCFMT_CENTER, 60);

}
void CTcgCurveDlg::OnLvnEndlabeleditListTcg(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnEndlabeleditListTcg"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pNMHDR )
	{
		MessageBox(_T("CTcgCurveDlg::OnLvnEndlabeleditListTcg,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnEndlabeleditListTcg"),"Leave");
		return;
	}
	if ( NULL == pResult )
	{
		MessageBox(_T("CTcgCurveDlg::OnLvnEndlabeleditListTcg,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnEndlabeleditListTcg"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnEndlabeleditListTcg"),"Leave");
}

void CTcgCurveDlg::OnLvnBeginlabeleditListTcg(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnBeginlabeleditListTcg"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pNMHDR )
	{
		MessageBox(_T("CTcgCurveDlg::OnLvnBeginlabeleditListTcg,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnBeginlabeleditListTcg"),"Leave");
		return;
	}
	if ( NULL == pResult )
	{
		MessageBox(_T("CTcgCurveDlg::OnLvnBeginlabeleditListTcg,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnBeginlabeleditListTcg"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnLvnBeginlabeleditListTcg"),"Leave");
}

void CTcgCurveDlg::OnHdnItemchangedListTcg(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnHdnItemchangedListTcg"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pNMHDR )
	{
		MessageBox(_T("CTcgCurveDlg::OnHdnItemchangedListTcg,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnHdnItemchangedListTcg"),"Leave");
		return;
	}
	if ( NULL == pResult )
	{
		MessageBox(_T("CTcgCurveDlg::OnHdnItemchangedListTcg,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTcgCurveDlg::OnHdnItemchangedListTcg"),"Leave");
		return;
	}
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnHdnItemchangedListTcg"),"Leave");
}

/** 
* 确定修改增益数值功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedButtonTcgSramModify()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgSramModify"),"Enter");
	// TODO: Add your control notification handler code here
	if(m_stTcgCurve[g_dwActiveChannel].nPointNum > 1)
	{
		for(int i = 0; i < m_lstTcg.GetItemCount(); i++)
		{
			CString strGain = m_lstTcg.GetItemText(i, 1);
			if(i == 0)	// 第一点
			{
				//yachang.huang rum on 2014-6-18
             
				for(int j = 0; j < m_nFrontAddDotNum + 1; j++)
				{
					m_fTcgSramGain[j] = 0;
				}	
				
			}
			else if(i == m_lstTcg.GetItemCount() - 1)	// 最后一点
			{
				//yachang.huang rum on 2014-6-18
                
				for(int j = m_nAllDotNum - m_nFrontAddDotNum - 1; j < m_nAllDotNum; j++)
				{
					m_fTcgSramGain[j] = static_cast<float>(_tstof(strGain));
				}
				

			}
			else
			{
				//yachang.huang rum on 2014-6-18
				//m_fTcgSramGain[i + m_nFrontAddDotNum] = static_cast<float>(_tstof(strGain));
				m_fTcgSramGain[i + m_nFrontAddDotNum] = static_cast<float>(_tstof(strGain));
			}			
		}
	}
	if(m_stTcgCurve[g_dwActiveChannel].bSwitchFlag)
	{
		SendTcgCurve();		// 发送TCG
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::OnBnClickedButtonTcgSramModify"),"Leave");
}

// wangbingfu add start at 2012-11-20
float CTcgCurveDlg::CalcNewAmp(float fAmp, float fRange)
{
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::CalcNewAmp"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	BOOL bTcgOnFlag = m_stTcgCurve[g_dwActiveChannel].bSwitchFlag;
	float fNewAmp = 0;
	
//	if(bTcgOnFlag)
	{
//		if(m_nTcgOnMode == 0)
		{
			TCG_CURVE_t stTcgCurve;
			memcpy(&stTcgCurve, &m_stTcgCurve[g_dwActiveChannel], sizeof(stTcgCurve));
			int nPointNum = stTcgCurve.nPointNum;
			CURVEPOINT_DATA_t stHeightestPoint = GetHeightestPoint(stTcgCurve.stPointData, nPointNum - 1);
			CURVEPOINT_DATA_t stFirstPoint = stTcgCurve.stPointData[0];
			CURVEPOINT_DATA_t stLastPoint = stTcgCurve.stPointData[nPointNum-1];
			float fFirstPointDeltaDB = 0;
			float fLastPointDeltaDB = 0;
			if(stFirstPoint.fData != 0)
			{
				fFirstPointDeltaDB = (float)(20 * log10((double)(stHeightestPoint.fData / stFirstPoint.fData)));
			}
			if(stLastPoint.fData != 0)
			{
				fLastPointDeltaDB = (float)(20 * log10((double)(stHeightestPoint.fData / stLastPoint.fData)));
			}

			CUltrasonicImmersedTestView* pAScanView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
			float fXMaxRuler = pAScanView->m_fXMaxRuler;
			float fXMinRuler = pAScanView->m_fXMinRuler;

			if(fRange < stFirstPoint.fRange)	// 第一个点之前
			{
				fNewAmp = fAmp * pow(10, fFirstPointDeltaDB / 20);
			}
			else if(fRange > stLastPoint.fRange)
			{
				fNewAmp =fAmp * pow(10, fLastPointDeltaDB / 20);
			}
			else
			{
				int j = 0;
				for(; j < nPointNum - 1; j++)
				{
					if((fRange >= stTcgCurve.stPointData[j].fRange)
						&&(stTcgCurve.stPointData[j+1].fRange >= fRange))
					{
						break;
					}
				}

				float fCurCurveAmp = 0;	// 当前点对应的曲线点幅值
				if(stTcgCurve.stPointData[j].fData > stTcgCurve.stPointData[j+1].fData)
				{
					fCurCurveAmp = stTcgCurve.stPointData[j].fData + (fRange - stTcgCurve.stPointData[j].fRange)
						/ (stTcgCurve.stPointData[j + 1].fRange - stTcgCurve.stPointData[j].fRange) 
						* (stTcgCurve.stPointData[j + 1].fData - stTcgCurve.stPointData[j].fData);
				}
				else
				{
					fCurCurveAmp = stTcgCurve.stPointData[j].fData - (fRange - stTcgCurve.stPointData[j].fRange)
						/ (stTcgCurve.stPointData[j + 1].fRange - stTcgCurve.stPointData[j].fRange) 
						* (stTcgCurve.stPointData[j].fData - stTcgCurve.stPointData[j + 1].fData);
				}

			//	fCurCurveAmp = fCurCurveAmp * MAX_HEIGHT / 100;
			//	float fHeightestData = stHeightestPoint.fData * MAX_HEIGHT / 100;
				if((fCurCurveAmp != 0) && (fAmp != 0))
				{
					float fDeltaDB = static_cast<float>(20 * log10(stHeightestPoint.fData / fCurCurveAmp));
					fNewAmp = fAmp * pow(10, fDeltaDB / 20);
					if(fNewAmp > MAX_HEIGHT)
					{
						fNewAmp = MAX_HEIGHT;
					}
				}						
			}
		}
// 		else
// 		{
// 			return fAmp;
// 		}
	}
// 	else
// 	{
// 		return fAmp;
// 	}

//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CTcgCurveDlg::CalcNewAmp"),"Leave");
	return fNewAmp;
}

/** 
* 开启模式为软件功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedRadioTcgSoft()
{
	// TODO: Add your control notification handler code here
}

/** 
* 开启模式为硬件功能函数
* @param     void
* @return    void
* @since     1.0
*/
void CTcgCurveDlg::OnBnClickedRadioHardware()
{
	// TODO: Add your control notification handler code here
}
// wangbingfu add end at 2012-11-20