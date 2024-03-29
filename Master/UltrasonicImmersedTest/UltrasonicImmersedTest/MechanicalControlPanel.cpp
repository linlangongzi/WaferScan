// MechanicalControlPanel.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 机械控制CMechanicalControlPanel面板窗体视图源文件
*Filename : MechanicalControlPanel.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-10 
*/


#include "stdafx.h" 
#include "UltrasonicImmersedTest.h"
#include "MechanicalStepParamDlg.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ExButton.h"
#include "MovePathView.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "UltrasonicImmersedTestDoc.h"
#include "ChannelInfoDockView.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "FeatureInfoWnd.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "GlobalFunction.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "BScanView.h"
#include <math.h>
#include "TcgCurveDlg.h"	// wangbingfu add at 2012-11-20
#include "CPrintView.h"

const double ZERO_POINT_PRECISION = 1E-2;
//extern HANDLE hSaveDMADataEvent[(CHANNEL_NUM+(PER_CARD_CHANNEL_NUM-1))/PER_CARD_CHANNEL_NUM];
//extern HANDLE *hSaveDMADataEvent;
//extern WORD g_wDMABuffer[2][DMA_COUNT];
//extern CH_FLAWDATA_CSCAN_D g_stPrimitiveData[2][PER_CH_DATA_NUM*2];
extern short g_nWaveSpeed;
const int FLAW_DATA_WORD_COUNT = sizeof(CH_FLAWDATA_CSCAN_S)/sizeof(WORD);
extern DWORD g_dwActiveChannel;
extern BOOL g_bDrawRealTime; //实时成像
// CMechanicalControlPanel
extern int GRID_X_NUM;
extern int GRID_Y_NUM;
extern CH_FLAWDATA_CSCAN_S *g_pFeatrueInfo;

//CH_FLAWDATA_CSCAN_S *gFeatrueInfo[128]; // wangbingfu add at2011-9-27

IMPLEMENT_DYNCREATE(CMechanicalControlPanel, CFormView)


typedef struct  
{
	float gridX;
	float gridY;
	int gridXSize;
	int gridYSize;
	double startX;
	double startY;
	int headSize;
	ULONG gridSize;
	int AxisSelect;
	ULONG lastIndex;
	bool isEmpty;
}my_grid_t;

//CFile mAsyncCScanFile;
// CFile mAsyncAScanFile;
// CString strTempPath;
my_grid_t m_grid_tmp;



CMechanicalControlPanel::CMechanicalControlPanel()
	: CFormView(CMechanicalControlPanel::IDD)
	, m_dXcoord(0)
	, m_dYcoord(0)
	, m_dZcoord(0)
	, m_dABcoord(0)
	, m_dWcoord(0)
	, m_dRcoord(0)
	, m_bOneStep(FALSE)
	, m_fWdrX(0)
	, m_fWdrY(0)
	, m_fWdrZ(0)
	, m_nAbsolute(0)	
	, m_dWaterDistance(0)
	, m_dABRadius(0)
	, m_dWRadius(0)
	, m_bXYReverse(FALSE)
	, m_nTracingCount(0)	// wangbingfu add on 2014-10-14
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CMechanicalControlPanel"),"Enter");
	m_fXstep = 1.0f;
	m_fYstep = 1.0f;
	m_fZstep = 1.0f;
	m_fRStep = 1.0f;
	// Song Chenguang add start on 2013-03-15
	m_fABStep = 1.0f;
	m_fWStep = 1.0f;
	// Song Chenguang add end on 2013-03-15

	m_pMovePathView = NULL;
	memset(m_dRelativeCoord,0,sizeof(m_dRelativeCoord));
	memset(m_dAcoord,0,sizeof(m_dAcoord));
	memset(m_dCurrentCoord,0,sizeof(m_dCurrentCoord));

	m_bPathExecute = FALSE;
	m_pGridCount = NULL;	//矩阵点描绘次数矩阵

	m_nMPMTimeIndex = 1;
	
	::ZeroMemory(m_dSysPara, sizeof(m_dSysPara));
	
	::ZeroMemory(m_logPara, sizeof(m_logPara));

	m_fFirstWdrX = 0.0f;		// 起点X值
	m_fFirstWdrY = 0.0f;		// 起点Y值
	m_fFirstWdrZ = 0.0f;		// 起点Z值

	int ChannelInfoID[] = {IDC_CHECK_GATE_ISOPEN,
		IDC_CHECK_MP_F_TIME,IDC_CHECK_MP_M_TIME,
		IDC_CHECK_MP_M_ALTITUDE,IDC_CHECK_FP_F_TIME,IDC_CHECK_FP_M_TIME,
		IDC_CHECK_FP_M_ALTITUDE,IDC_EDIT_WAVE_SPEED};

	int CScanInfoID[] = {
		IDC_EDIT_BOUND_MIN,IDC_EDIT_BOUND_MAX,
		IDC_RADIO_DEEPDRAW,IDC_RADIO_RANGEDRAW,IDC_RADIO_SELECT_GATE1,
		IDC_RADIO_SELECT_GATE2,IDC_RADIO_SELECT_GATE3,IDC_RADIO_SELECT_GATE4,
		IDC_RADIO_MAX_VALUE,IDC_RADIO_MIN_VALUE,IDC_RADIO_AVG_VALUE,
		IDC_EDIT_GATE1_THRESHOLD,IDC_EDIT_GATE2_THRESHOLD,IDC_EDIT_GATE3_THRESHOLD,
		IDC_EDIT_GATE4_THRESHOLD,IDC_RADIO_MAX_AMP,IDC_RADIO_FIRST_AMP,IDC_RADIO_MAX_TIME,
		IDC_RADIO_MAX_THRESHOL_TIME,IDC_RADIO_FIRST_TIME,IDC_RADIO_FIRST_THRESHOLD_TIME,
		IDC_BUTTON_READ_FILE,IDC_BUTTON_DRAW_PICTURE,IDC_BUTTON_SAVE_CSCAN_PICTURE,
		IDC_RADIO_XMODE_ANGLE,IDC_RADIO_XMODE_PERIMETER,IDC_BUTTON_SAVE_DATA,IDC_BUTTON_READ_FILE,
		IDC_BUTTON_DRAW_PICTURE,IDC_BUTTON_SAVE_CSCAN_PICTURE,IDC_BUTTON_CSCAN_PARAM_SAVE,
		IDC_BUTTON_CSCAN_PARAM_READ, IDC_RADIO_VELOCITY_PICTURE, IDC_RADIO_VELOCITY_REDUCE_PICTURE,
		IDC_RADIO_SZK_PICTURE, IDC_RADIO_MIDU_PICTURE, IDC_BUTTON_CACULATE_VELOCITY,
		IDC_BUTTON_CALCUATE_SHUAIJIAN, IDC_BUTTON_CACULATE_SZK, IDC_BUTTON_CACULATE_MIDU,
		IDC_EDIT_AVE_VELOCITY, IDC_EDIT_AVE_SHUAIJIAN, IDC_EDIT_AVE_SZK, IDC_EDIT_AVE_MIDU,
		IDC_CHECK_SURFACE_STATISTICS,IDC_BUTTON_DEFAULT_COLOR1,IDC_BUTTON_DEFAULT_COLOR2,
		 IDC_BUTTON_HANHE_RATE,IDC_BUTTON_HANHE_CACULATE,
		IDC_CHECK_REAL_SCALE};

	int MechanicalInfoID[] = {IDC_BUTTON_Y_FRONT, IDC_BUTTON_Y_BACK, IDC_BUTTON_X_LEFT,
		IDC_BUTTON_X_RIGHT, IDC_BUTTON_UP_Z, IDC_BUTTON_Z_DOWN, IDC_BUTTON_W1_POSITIVE,
		IDC_BUTTON_W1_REVERSE,IDC_BUTTON_REPOSITION,IDC_BUTTON_SYSPARAM, IDC_BUTTON_DEFINE_REPOSITION, 
		IDC_BUTTON_GET_RECT,IDC_BUTTON_DISPLAY_RECT, IDC_OPEN_PATH_FILE, IDC_SAVE_PATH_FILE, 
		IDC_FILE_SAVE_AS, IDC_BUTTON_SHANGLIAO_POS,IDC_BUTTON_SETSTEP};

	// Song Chenguang modify start on 2013-08-06
	int nSize = sizeof(ChannelInfoID) / sizeof(int);
	for (int i=0; i< nSize; i++)
	{
		m_ChannelInfoID[i] = ChannelInfoID[i];
	}

	nSize = sizeof(CScanInfoID) / sizeof(int);
	for (int i=0; i< nSize; i++)
	{
		m_CScanInfoID[i] = CScanInfoID[i];
	}

	nSize = sizeof(MechanicalInfoID) / sizeof(int);
	for (int i=0; i< nSize; i++)
	{
		m_MechanicalInfoID[i] = MechanicalInfoID[i];
	}
	// Song Chenguang modify end on 2013-08-06
	m_nMPMPeakIndex = 1;


	//存数据由临时文件夹改为存在D:\\UltrData下   yachang.huang add start on 2014-04-23
	CreateDirectory(_T("D:\\UltrData"),NULL);
	m_strTempPath = _T("D:\\UltrData\\");
	m_strTempPath += L"UTTAsync.";
	//yachang.huang add end on 2014-04-23

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CMechanicalControlPanel"),"Leave");
	m_bTaskRun = FALSE;
	m_wTaskNo = 0;
	m_wTaskItemCount = 0;
	// test
	m_fWCurPos = 0;
	//////////////////////////////////////////////////////////////////////////
}

CMechanicalControlPanel::~CMechanicalControlPanel()
{

}

void CMechanicalControlPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_X_LEFT, m_btnXLeft);
	DDX_Control(pDX, IDC_BUTTON_X_RIGHT, m_btnXRight);
	DDX_Control(pDX, IDC_BUTTON_Y_FRONT, m_btnYFront);
	DDX_Control(pDX, IDC_BUTTON_Y_BACK, m_btnYBack);
	DDX_Control(pDX, IDC_BUTTON_UP_Z, m_btnZUp);
	DDX_Control(pDX, IDC_BUTTON_Z_DOWN, m_btnZDown);	
	DDX_Control(pDX, IDC_BUTTON_AB_POSITIVE, m_btnABPositive);
	DDX_Control(pDX, IDC_BUTTON_AB_REVERSE, m_btnABReverse);
	DDX_Control(pDX, IDC_BUTTON_W_POSITIVE, m_btnWPositive);
	DDX_Control(pDX, IDC_BUTTON_W_REVERSE, m_btnWReverse);
	DDX_Control(pDX, IDC_BUTTON_R_POSITIVE, m_btnRPositive);
	DDX_Control(pDX, IDC_BUTTON_R_REVERSE, m_btnRReverse);
	DDX_Text(pDX, IDC_EDIT_XCOORD, m_dXcoord);
	DDV_MinMaxDouble(pDX, m_dXcoord, m_SysParamDlg.m_fSafeMinX, m_SysParamDlg.m_fSafeMaxX);
	DDX_Text(pDX, IDC_EDIT_YCOORD, m_dYcoord);
	DDV_MinMaxDouble(pDX, m_dYcoord, m_SysParamDlg.m_fSafeMinY, m_SysParamDlg.m_fSafeMaxY);
	DDX_Text(pDX, IDC_EDIT_ZCOORD, m_dZcoord);
	DDV_MinMaxDouble(pDX, m_dZcoord, 0.0, m_SysParamDlg.m_fSafeMaxZ);
	DDX_Control(pDX, IDC_STATICX, m_staticTx);
	DDX_Control(pDX, IDC_STATICY, m_staticTy);
	DDX_Control(pDX, IDC_STATICZ, m_staticTz);
	DDX_Check(pDX, IDC_CHECK_ONESTEP, m_bOneStep);
	DDX_Control(pDX, IDC_STATIC_Y, m_staticYCoord);
	DDX_Control(pDX, IDC_STATIC_Z, m_staticZCoord);
	DDX_Control(pDX, IDC_STATIC_X, m_staticXCoord);
	DDX_Control(pDX, IDC_BUTTON_RUN, m_btnRun);
	DDX_Text(pDX, IDC_STATIC_WDR_X, m_fWdrX);
	DDX_Text(pDX, IDC_STATIC_WDR_Y, m_fWdrY);
	DDX_Text(pDX, IDC_STATIC_WDR_Z, m_fWdrZ);
	DDX_Radio(pDX, IDC_RADIO_ABSOLUTE, m_nAbsolute);
	DDX_Control(pDX, IDC_LIST_RECT_POINTS, m_lstRectPoints);	
//	DDX_Control(pDX, IDC_STATICW1, m_staticTW1);
	DDX_Text(pDX, IDC_EDIT_ABCOORD, m_dABcoord);
	DDV_MinMaxDouble(pDX, m_dABcoord, 0, 360);
	DDX_Text(pDX, IDC_EDIT_WCOORD, m_dWcoord);
	DDV_MinMaxDouble(pDX, m_dWcoord, 0, 360);
	DDX_Text(pDX, IDC_EDIT_RCOORD, m_dRcoord);
	DDV_MinMaxDouble(pDX, m_dRcoord, 0, 360);
	DDX_Text(pDX, IDC_EDIT_WATER_DISTANCE, m_dWaterDistance);
	DDX_Text(pDX, IDC_EDIT_AB_RADIUS, m_dABRadius);
	DDX_Text(pDX, IDC_EDIT_W_RADIUS, m_dWRadius);
	DDX_Check(pDX, IDC_CHECK_XY_REVERSE, m_bXYReverse);
}

BEGIN_MESSAGE_MAP(CMechanicalControlPanel, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_REPOSITION, &CMechanicalControlPanel::OnBnClickedButtonReposition)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CMechanicalControlPanel::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_DEFINE_REPOSITION, &CMechanicalControlPanel::OnBnClickedButtonDefineReposition)
	ON_BN_CLICKED(IDC_BUTTON_SYSPARAM, &CMechanicalControlPanel::OnBnClickedButtonSysparam)
	ON_BN_CLICKED(IDC_BUTTON_SETSTEP, &CMechanicalControlPanel::OnBnClickedButtonSetstep)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SAVE_PATH_FILE, &CMechanicalControlPanel::OnBnClickedSavePathFile)
	ON_BN_CLICKED(IDC_OPEN_PATH_FILE, &CMechanicalControlPanel::OnBnClickedOpenPathFile)
	ON_BN_CLICKED(IDC_XIANGDUI_CROOD, &CMechanicalControlPanel::OnBnClickedXiangduiCrood)
	ON_BN_CLICKED(IDC_RADIO_ABSOLUTE, &CMechanicalControlPanel::OnBnClickedRadioAbsolute)
	ON_BN_CLICKED(IDC_FILE_SAVE_AS, &CMechanicalControlPanel::OnBnClickedFileSaveAs)
	ON_BN_CLICKED(IDC_BUTTON_GET_RECT, &CMechanicalControlPanel::OnBnChickedGetRect)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_RECT, &CMechanicalControlPanel::OnBnClickedButtonDisplayRect)
	ON_MESSAGE(WM_FEATURE_INFO, &CMechanicalControlPanel::OnReceiveFeatureInfo)
	ON_BN_CLICKED(IDC_BUTTON_SHANGLIAO_POS, &CMechanicalControlPanel::OnBnClickedButtonShangliaoPos)
	ON_BN_CLICKED(IDC_BUTTON_GET_WATER_DISTANCE, &CMechanicalControlPanel::OnBnClickedButtonGetWaterDistance)
	ON_BN_CLICKED(IDC_CHECK_XY_REVERSE, &CMechanicalControlPanel::OnBnClickedCheckXYReverse)
	ON_BN_CLICKED(IDC_CHECK_ONESTEP, &CMechanicalControlPanel::OnBnClickedCheckOnestep)
END_MESSAGE_MAP()


// CMechanicalControlPanel 诊断

#ifdef _DEBUG
void CMechanicalControlPanel::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMechanicalControlPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMechanicalControlPanel 消息处理程序

bool CMechanicalControlPanel::CreateView(CWnd* pParent, CCreateContext* pContext)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CreateView"),"Enter");
	CRect rect(0,0,300,300);

	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CMyFormView!\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::CreateView"),"Leave");
		return FALSE;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CreateView"),"Leave");
	return TRUE;
}

//复位
void CMechanicalControlPanel::OnBnClickedButtonReposition()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonReposition"),"Enter");

	if (IDCANCEL == MessageBox(_T("是否确定复位？"),_T("复位提示"),MB_OKCANCEL))
	{
        //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonReposition"),"Leave");
		return;
	}
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(!pFrame->m_bMoinitorDataRun)
	{
		pFrame->m_bMoinitorDataRun = TRUE;	// Song Chenguang add on 2013-12-21
/*		WPARAM wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
*/
		pFrame->SetTimer(TIMER_NET_JUDGE, NET_JUDGE_TIME, NULL);
	}
	
	short i_ws = m_pMovePathView->m_GraphMove.GetWorkStatus();


	if ( i_ws==1 || i_ws==3 || i_ws==4 || i_ws==2)
	{
//		mAsyncCScanFile.Close();
//		mAsyncAScanFile.Close();

		DeleteFile(m_strTempPath+L"acf");
		DeleteFile(m_strTempPath+L"aaf");
		DeleteFile(m_strTempPath1+L"aaf");	// wangbingfu add on 2014-09-10
	}
	

	BOOL bRet = m_pMovePathView->m_GraphMove.ReSet();

	if (bRet)
	{
		g_bDrawRealTime = FALSE;
		// 清A数据
		CUltrasonicImmersedTestDoc* pDoc = GetDocument();

		for (int i = 0; i < CHANNEL_NUM; i++)
		{
			pDoc->m_lstFlawData[i].RemoveAll();
			// Song Chenguang modfiy start on 2013-12-18
//			pDoc->m_lstAScanData[i].RemoveAll();
			// Song Chenguang modfiy end on 2013-12-18
			pDoc->m_lstFlawDataOriginal[i].RemoveAll();	// 2011-11-18
		}

		static BOOL bInitSysParam = FALSE;

		if (!bInitSysParam)
		{
			// Song Chenguang add start on 2013-09-03
			// ptp坐标x,y设为安全区最小值
			m_pMovePathView->m_GraphMove.GetPara(m_dSysPara, m_logPara);
			m_dXcoord = m_dSysPara[20];
			m_dYcoord = m_dSysPara[21];
			// Song Chenguang add start on 2013-09-03
			InitSysParamDlg();
			UpdateData(FALSE);	// Song Chenguang add on 2013-08-13 更新输入值上下限

			bInitSysParam = TRUE;

			// 检测圆柱体时取矩形对角点功能无效
			if (m_SysParamDlg.m_nAxisIndex == 3)
			{
				GetDlgItem(IDC_BUTTON_GET_RECT)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_DISPLAY_RECT)->EnableWindow(FALSE);
			} 
			else
			{
				GetDlgItem(IDC_BUTTON_GET_RECT)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_DISPLAY_RECT)->EnableWindow(TRUE);
			}
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonReposition"),"Leave");
}


//运行
void CMechanicalControlPanel::OnBnClickedButtonRun()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Enter");
	/*
	// WangQianfeng add start on 2012-11-16
	//在每次扫描时都判断LOG文件存不存在，如果存在则删除，重新生成LOG文件
	TCHAR pathbuf[260];     
	int  pathlen = GetModuleFileName(NULL,pathbuf,260); 
	CString  fName = pathbuf;
	CString FileName;
	FileName.Format(_T("%s%s"),fName.Left(fName.GetLength()-3),_T("log"));//因为当前LOG名和EXE名相同，所以只是去掉扩展名即可
	CStdioFile file;
	BOOL bRet = file.Open(FileName, CFile::modeCreate|CFile::modeReadWrite);
	if(!bRet)
	{
		AfxMessageBox(L"LOG文件创建不成功");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
	}
	file.Close();
	// WangQianfeng add end on 2012-11-16
	*/

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	pView->m_bESC = FALSE;
//	UpdateData(TRUE);	// rem by Song Chenguang on 2013-09-25
	short status=m_pMovePathView->m_GraphMove.GetWorkStatus();
	if(status>2)
	{
		MessageBox(_T("设备非扫描或停止时，不能进行此操作.","操作提示"));
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
		if (m_bTaskRun)
		{
			m_bTaskRun = FALSE;
			m_strCurrentMovePath = _T("");
			pFrame->m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);
			pFrame->m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_STATIC_RUN_STATE)->SetWindowText(_T("运行状态：状态异常未运行！"));
		}
		return;
	}

	if(!pFrame->m_bMoinitorDataRun)
	{
		pFrame->m_bMoinitorDataRun = TRUE;	// Song Chenguang add on 2013-12-21
/*		WPARAM wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
*/
		pFrame->SetTimer(TIMER_NET_JUDGE, NET_JUDGE_TIME, NULL);
	}

	if(status<1)//移动采集点
	{
		if (!m_bTaskRun)
		{
			if (IDCANCEL == MessageBox(_T("是否确定运行？"),_T("运行提示"),MB_OKCANCEL))	// Song Chenguang modified on 2013-05-03
			{
				//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
				return;
			}
		}
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		CCScanParamPanel* pCPannel1 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		CCScanParamPanel* pCPannel2 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
		if(pCPannel1->m_nFeatureIndex > 5 || pCPannel2->m_nFeatureIndex > 5)
		{
			MessageBox(_T("实时扫查成像不能选择声速、声衰减、声阻抗以及密度成像"), _T("提示"), MB_OK);
		    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
			return;
		}

		double dXStart = 0.0f;
		double dYStart = 0.0f;
		double dZStart = 0.0f;	
		// Song Chenguang modified start on 2013-04-27
		double dABStart = 0.0f;	
		double dWStart = 0.0f;
		double dRStart = 0.0f;
		m_pMovePathView->m_GraphMove.GraphStartPoint(&dXStart, &dYStart, &dZStart,
			&dABStart, &dWStart, &dRStart);
		// Song Chenguang modified end on 2013-04-27

		double dXEnd = 0.0f;
		double dYEnd = 0.0f;
		double dZEnd = 0.0f;
		// Song Chenguang modified start on 2013-04-27
		double dABEnd = 0.0f;	
		double dWEnd = 0.0f;
		double dREnd = 0.0f;
		m_pMovePathView->m_GraphMove.GraphEndPoint(&dXEnd, &dYEnd, &dZEnd, 
			&dABEnd, &dWEnd, &dREnd);
		// Song Chenguang modified end on 2013-04-27

		// Song Chenguang add start on 2013-09-18
		// 扫查区域是否在安全区，判断
		if (dXStart < m_dSysPara[20] || dYStart < m_dSysPara[21] ||
			dXEnd > m_dSysPara[22] || dYEnd > m_dSysPara[23] ||
			dZStart > m_dSysPara[25] || dZEnd > m_dSysPara[25])
		{
			MessageBox(_T("扫查区域不在安全区范围内，请重新规划扫查路径！"));
			return;
		}
		// Song Chenguang add end on 2013-09-18
		// 旋转时纵坐标设为棒材周长
		double dYStartTemp = dYStart;
		double dYEndTemp = dYEnd;
		int nAxisIndex = m_pMovePathView->m_GraphMove.GetAxisSelect();
		if(nAxisIndex == 3)
		{
			double dPipeDiameter = m_pMovePathView->m_GraphMove.GetPipR();
			dYStartTemp = 0;
			dYEndTemp = PI * dPipeDiameter;

		}
		
		double dParam[SYSPARAM1_NUM];
		::ZeroMemory(dParam, sizeof(dParam));
		long lLogParam[10];
		::ZeroMemory(lLogParam, sizeof(lLogParam));
		m_pMovePathView->m_GraphMove.GetPara(dParam, lLogParam);
		float fGridX =0.0f;
		float fGridY =0.0f;
		
		// 网格间距
		if (lLogParam[0] == 0) // 横向
		{
			fGridX = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
			fGridY = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;	
		} 
		else	// 纵向
		{
			fGridX = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;
			fGridY = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
		}

		int nMemDCXLength = static_cast<int>(fabs(dXEnd - dXStart) / fGridX);	//MemDC画工件的X像素长度
		int nMemDCYLength = static_cast<int>(fabs(dYStartTemp - dYEndTemp) / fGridY);	//MemDC画工件的Y像素长度
		if(nMemDCXLength * nMemDCYLength > 9 * 1024 *1024)
		{
			if (lLogParam[0] == 0) // 横向
			{
				MessageBox(_T("X方向长度除以扫描间距 * Y方向长度除以步进间距 的值不能大于9M"), _T("提示"), MB_OK);
			}
			else	// 纵向
			{
				if(nAxisIndex == 3)
				{
					MessageBox(_T("X方向长度除以步进间距 * 管材周长除以扫描间距 的值不能大于9M"), _T("提示"), MB_OK);
				}
				else
				{
					MessageBox(_T("X方向长度除以步进间距 * Y方向长度除以扫描间距 的值不能大于9M"), _T("提示"), MB_OK);
				}				
			}
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
			return;
		}
		
		m_pCScanView->m_bDrawSurfaceFlag = FALSE;	// 画面伤标志置为FALSE,画C扫

//		double dCurrentPos[6];
//		m_pMovePathView->m_GraphMove.GetPos(dCurrentPos);

		if ((m_SysParamDlg.m_nAxisIndex == 0 && (dXEnd - dXStart) < 1E-1 && (dYEnd - dYStart) < 1E-1) ||
			(m_SysParamDlg.m_nAxisIndex == 1 && (dXEnd - dXStart) < 1E-1 && (dZEnd - dZStart) < 1E-1) ||
			(m_SysParamDlg.m_nAxisIndex == 2 && (dYEnd - dYStart) < 1E-1 && (dZEnd - dZStart) < 1E-1) ||
			((m_SysParamDlg.m_nAxisIndex == 3 || m_SysParamDlg.m_nAxisIndex == 4) && (dXEnd - dXStart) < 1E-1 ) ||
			((m_SysParamDlg.m_nAxisIndex == 5) && (dZEnd - dZStart) < 1E-1 ))
		{
			MessageBox(_T("可能无图形数据，程序返回！"), _T("提示"), MB_OK);
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
			return;
		}
		// wangbing modify start on 2013-01-31
		if(!m_pMovePathView->m_GraphMove.PtpMove(dXStart,dYStart,dZStart, dABStart, dWStart, dRStart, 1))
		// wangbing modify end on 2013-01-31
		{
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");
			return;
		}
		KillTimer(MECHANICAL_TIMER);
		SetTimer(MECHANICAL_TIMER,10,NULL);

		m_btnRun.SetWindowText(_T("暂停(F7)"));
	}
	else
	{	
		PathExeute();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonRun"),"Leave");

}

void CMechanicalControlPanel::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnInitialUpdate"),"Enter");
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	m_pMovePathView->m_ParentWnd = this;
	m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	m_pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);

	//机械控制方向键加载位图
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	m_bXYReverse = (BOOL)pApp->GetProfileInt(_T("水浸超声探伤系统"), _T("XY_Reverse"), 0);
	SetMoveButtonIcon(TRUE);

	//给方向键添加编号
	// wangbingfu modify start on 2012-12-27
	if(pApp->m_IniParams.m_info.bXReverse == 0)
	{
		m_btnXLeft.type = 0;
		m_btnXRight.type = 1;
	}
	else
	{
		m_btnXLeft.type = 1;
		m_btnXRight.type = 0;
	}
	if(pApp->m_IniParams.m_info.bYReverse == 0)
	{
		m_btnYBack.type = 2;
		m_btnYFront.type = 3;
	}
	else
	{
		m_btnYBack.type = 3;
		m_btnYFront.type = 2;
	}
	if(pApp->m_IniParams.m_info.bZReverse == 0)
	{
		m_btnZUp.type = 4;
		m_btnZDown.type = 5;
	}
	else
	{
		m_btnZUp.type = 5;
		m_btnZDown.type = 4;
	}
	// wangbingfu modify end on 2012-12-27

	m_btnABPositive.type = 6;	// A
	m_btnABReverse.type = 7;	
	m_btnWPositive.type = 8;	// W
	m_btnWReverse.type = 9;
	m_btnRPositive.type = 10;	// R
	m_btnRReverse.type = 11;

	// 与面板关联
	m_btnXLeft.m_pParent = this;
	m_btnXRight.m_pParent = this;
	m_btnYBack.m_pParent = this;
	m_btnYFront.m_pParent = this;
	m_btnZUp.m_pParent = this;
	m_btnZDown.m_pParent = this;

	m_btnABPositive.m_pParent = this;
	m_btnABReverse.m_pParent = this;
	m_btnWPositive.m_pParent = this;
	m_btnWReverse.m_pParent = this;
	m_btnRPositive.m_pParent = this;
	m_btnRReverse.m_pParent = this;

	//初始化单步移动步进
	SetStepLabelText();

	//自定义复位原点初始化
	if(m_nAbsolute == 0)//绝对坐标
	{
		m_Xcoord=m_dAcoord[0];
		m_Ycoord=m_dAcoord[1];
		m_Zcoord=m_dAcoord[2];
		m_W1coord=m_dAcoord[3];
	}
	else//相对坐标
	{
		m_Xcoord = m_dRelativeCoord[0];
		m_Ycoord = m_dRelativeCoord[1];
		m_Zcoord = m_dRelativeCoord[2];
		m_W1coord = m_dRelativeCoord[3];
		m_staticTx.SetWindowText(_T("X偏移"));
		m_staticTy.SetWindowText(_T("Y偏移"));
		m_staticTz.SetWindowText(_T("Z偏移"));
		GetDlgItem(IDC_STATICAB)->SetWindowText(_T("AB偏移"));
	}

	//每次打开机械控制面板初始化复位坐标
	m_dXcoord = m_dAcoord[0];
	m_dYcoord = m_dAcoord[1];
	m_dZcoord = m_dAcoord[2];
	m_dRcoord = m_dAcoord[3];

	m_lstRectPoints.InsertColumn(0, _T( "序号")); 
	m_lstRectPoints.InsertColumn(1, _T( "X")); 
	m_lstRectPoints.InsertColumn(2, _T( "Y")); 
	m_lstRectPoints.InsertColumn(3, _T( "Z"));
	m_lstRectPoints.SetColumnWidth(0, 40);
	m_lstRectPoints.SetColumnWidth(1, 50);
	m_lstRectPoints.SetColumnWidth(2, 50);
	m_lstRectPoints.SetColumnWidth(3, 50);


	// 根据系统设置，设定通道使能状态
//	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	CString strProbeNum = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), _T("1"));
	int nProbeNum = _wtoi(strProbeNum.GetBuffer(strProbeNum.GetLength()));

	CString strOneProbeType = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), _T("1"));
	int nIndexOneProbeType = _wtoi(strOneProbeType.GetBuffer(strOneProbeType.GetLength()));

	CString strTwoProbeType = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), _T("1"));
	int nIndexTwoProbeType = _wtoi(strTwoProbeType.GetBuffer(strTwoProbeType.GetLength()));

	CString strThreeProbeType = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), _T("1"));
	int nIndexThreeProbeType = _wtoi(strThreeProbeType.GetBuffer(strThreeProbeType.GetLength()));

	m_SysParamDlg.SetChannelStateNew(nProbeNum, nIndexOneProbeType, nIndexTwoProbeType,nIndexThreeProbeType,m_SysParamDlg.m_wWorkMode);

	// 矩形框对角点列表初始化
	m_lstRectPoints.DeleteAllItems();
	for (int i=0; i<2; i++)
	{
		CString strIndex;
		strIndex.Format(_T("%d"), i+1);
		int nRes = m_lstRectPoints.InsertItem(i, strIndex);

		int iSubItem = 0;
		BOOL bRet = m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// X
		bRet = m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// Y
		bRet = m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// Z
	}
	
	if(m_SysParamDlg.m_nAxisIndex == 3 
		|| m_SysParamDlg.m_nAxisIndex == 4) //W1管 // wangbingfu add on 2013-01-09
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(TRUE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(TRUE);
	}
	else
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(FALSE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(FALSE);;
	}

	SetTimer(MECHANICAL_TIMER,50,NULL);

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnInitialUpdate"),"Leave");
}
/** 
* 手动开始
* @param     type [in]: 扫查架滑动方向                                                  
* @return    void
* @since     1.0
*/
void CMechanicalControlPanel::HandStart(int type)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::HandStart"),"Enter");	
	short status = 0;
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(!pFrame->m_bMoinitorDataRun)
	{
		pFrame->m_bMoinitorDataRun = TRUE;	// Song Chenguang add on 2013-12-21
/*		WPARAM wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
*/
		pFrame->SetTimer(TIMER_NET_JUDGE, NET_JUDGE_TIME, NULL);
	}

	if (!m_bOneStep)//连续运动
	{
		if(m_pMovePathView->m_GraphMove.AxisJog(type)) 
		{
			
			KillTimer(MECHANICAL_TIMER);
			SetTimer(MECHANICAL_TIMER,10,NULL);
		}
	}
	else//设置步进单步运动
	{
		// Song Chenguang add start on 2013-09-03
		// UpdateData失败就返回
// wangbingfu add start on 2014-11-03
		float fW1Diameter = pApp->m_IniParams.m_info.fW1Diameter;
#if _HAFEI
		
		CMechanicalControlPanel* pMechanicalPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		if (pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
		{
			fW1Diameter = pFrame->m_fWeiJianW1;
		}
#endif
// wangbingfu add end on 2014-11-03
		BOOL bRet = UpdateData(TRUE);
		if (!bRet)
			return;
		// Song Chenguang add end on 2013-09-03

		m_pMovePathView->m_GraphMove.GetPos(m_dCurrentCoord);
		switch (type)
		{
		case 0:
			m_dCurrentCoord[0] -= m_fXstep;
			break;
		case 1:
			m_dCurrentCoord[0] += m_fXstep;
			break;
		case 2:
			m_dCurrentCoord[1] -= m_fYstep;
			break;
		case 3:
			m_dCurrentCoord[1] += m_fYstep;
			break;
		case 4:
			m_dCurrentCoord[2] -= m_fZstep;
			break;
		case 5:
			m_dCurrentCoord[2] += m_fZstep;
			break;
		case 6:
			m_dCurrentCoord[3] -= m_fABStep;
			break;
		case 7:
			m_dCurrentCoord[3] += m_fABStep;
			break;
		case 8:
			m_dCurrentCoord[4] += m_fWStep;
			break;
		case 9:
			m_dCurrentCoord[4] -= m_fWStep;
			break;
		case 10:
			m_dCurrentCoord[5] -= m_fRStep * (m_pMovePathView->m_GraphMove.GetPipR() / fW1Diameter);// wangbingfu modify on 2014-11-03 pApp->m_IniParams.m_info.fW1Diameter改fW1Diameter
			break;
		case 11:
			m_dCurrentCoord[5] += m_fRStep * (m_pMovePathView->m_GraphMove.GetPipR() / fW1Diameter);// wangbingfu modify on 2014-11-03 pApp->m_IniParams.m_info.fW1Diameter改fW1Diameter
			break;
		default:
			break;
		}
		// wangbingfu modify start on 2013-01-31
		m_pMovePathView->m_GraphMove.PtpMove(m_dCurrentCoord[0],m_dCurrentCoord[1],m_dCurrentCoord[2],m_dCurrentCoord[3],
									m_dCurrentCoord[4],m_dCurrentCoord[5], 0);
		// wangbingfu modify end on 2013-01-31
		KillTimer(MECHANICAL_TIMER);
		SetTimer(MECHANICAL_TIMER,10,NULL);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::HandStart"),"Leave");
	
}
/** 
* 手动停止扫查运动
* @param     void                                                
* @return    void
* @since     1.0
*/
void CMechanicalControlPanel::HandStop(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::HandStop"),"Enter");
//	UpdateData(TRUE);	
	if (!m_bOneStep)//连续运动
	{
		if(m_pMovePathView->m_GraphMove.AxisStop())
		{
			KillTimer(MECHANICAL_TIMER);
			SetTimer(MECHANICAL_TIMER,50,NULL);
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::HandStop"),"Leave");

}
/** 
* 自定义复位原点按钮响应函数   改为 移动
* @param     void                                              
* @return    void
* @since     1.0
*/
void CMechanicalControlPanel::OnBnClickedButtonDefineReposition()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDefineReposition"),"Enter");
	// Song Chenguang add start on 2013-08-15
	if (!UpdateData(TRUE))
		return;
	// Song Chenguang add end on 2013-08-15
	// wangbingfu add start on 2012-12-27
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = NULL;
	pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;	
	if(pCScanParamView->m_bDisplayProbe)
	{
		CDC *pDC = m_pCScanView->GetDC();
		m_pCScanView->InvertRegion(pDC, m_pCScanView->m_DlbPoint, 0, eLINE);
		pDC = m_pCScanView2->GetDC();
		m_pCScanView2->InvertRegion(pDC, m_pCScanView2->m_DlbPoint, 0, eLINE);
	}
	// wangbingfu add end on 2012-12-27
	short status=m_pMovePathView->m_GraphMove.GetWorkStatus();
	if(status>0)
	{
		MessageBox(_T("设备非停止时，不能进行此操作.","操作提示"));
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDefineReposition"),"Leave");
		return;
	}

	if (IDCANCEL == MessageBox(_T("是否确定移动"),_T("移动提示"),MB_OKCANCEL))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDefineReposition"),"Leave");
		return;
	}

//	UpdateData(TRUE);

	if(!pFrame->m_bMoinitorDataRun)
	{
		pFrame->m_bMoinitorDataRun = TRUE;	// Song Chenguang add on 2013-12-21
/*		WPARAM wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
*/
		pFrame->SetTimer(TIMER_NET_JUDGE, NET_JUDGE_TIME, NULL);
	}

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	
	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));
	m_pMovePathView->m_GraphMove.GetPara(dParam, lLogParam);

	int nProbeNum = m_SysParamDlg.m_nIndexProbeNum + 1;
	int nScanType = m_SysParamDlg.m_nScanType;
	int nFillType = lLogParam[0];
	int nTwoProbeIndex = m_SysParamDlg.m_nIndexTwoProbeType;
	float fProbeDistance = m_SysParamDlg.m_fProbeDistance;
	float fChongDie = m_SysParamDlg.m_fChongDie;
	
	// wangbingfu add start at 2012-9-18
	int nOneProbeIndex = 0;
	int nThreeProbeIndex = 0;
	m_SysParamDlg.Group2ProbeValue(nProbeNum, nOneProbeIndex, nTwoProbeIndex, nThreeProbeIndex);
	// wangbingfu add end at 2012-9-18

#if 0
	double i_start=0;
	int i_block1=0;
	int i_block2=0;
	int nOffset=0;		//起点探头序号

	if(nProbeNum > 1)
	{
		if(nScanType == 0)
		{
			if(nFillType == 0)	// 横向
			{
				i_start=m_pCScanView->m_dYStart;
				i_block2=fProbeDistance;
				double dYcoord = m_dYcoord;
				switch(nProbeNum)
				{
				case 2:
					//wangbingfu modify start at 2012-9-18
				/*	if (nTwoProbeIndex < 3)
					{//1,2  2,3  3,4
						nOffset=nTwoProbeIndex;
						i_block1=fProbeDistance * 2;
					} 
					else if(nTwoProbeIndex > 4)
					{//1,4
						i_block1=fProbeDistance * 6;
						i_block2=fProbeDistance * 3;
					}
					else
					{//1,3  2,4
						nOffset=nTwoProbeIndex-3;
						i_block1=fProbeDistance * 4;
						i_block2=fProbeDistance * 2;
					}*/
					if(nTwoProbeIndex == 0)	//1,2
					{
						nOffset = 0;
						i_block1=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 2)	//3,4
					{
						nOffset = 1;
						i_block1=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 3)	//1,3
					{
						nOffset = 0;
						i_block1=fProbeDistance * 4;
						i_block2=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 4)	//2,4
					{
						nOffset = 1;
						i_block1=fProbeDistance * 4;
						i_block2=fProbeDistance * 2;
					}
					else
					{
						;//
					}
					//wangbingfu modify end at 2012-9-18
					break;
				case 3:
					//wangbingfu modify start at 2012-9-18
					//nOffset=nTwoProbeIndex;
					nOffset=nThreeProbeIndex;
					//wangbingfu modify end at 2012-9-18
					i_block1=fProbeDistance * 3;

					break;
				case 4:
					i_block1=fProbeDistance * 4;
					break;
				default:
					i_block1=fProbeDistance;
					break;
				}
				i_start += nOffset * fProbeDistance;

				int n=floor(fmod(dYcoord-i_start,i_block1)/i_block2);
				if (nProbeNum==2)
				{
					if (nTwoProbeIndex < 3)
					{//1,2  2,3  3,4
						n+=nOffset;
					} 
					else if (nTwoProbeIndex > 4)
					{//1,4
						if (n>0)
						{
							n=3;
						}
					}
					else
					{//1,3  2,4
						if (n>0)
						{
							n++;
						}
						n+=nOffset;
					}
				} 
				else
				{
					n+=nOffset;
				}

				dYcoord = dYcoord - n * fProbeDistance;
				m_dYcoord = dYcoord;
				
				//wangbingfu modify start at 2012-9-18
				//pChannelPanel->m_nChannelNo = n;
				pChannelPanel->m_nChannelNo = n;
				//wangbingfu modify end at 2012-9-18
				pChannelPanel->DoChangeChannel();
				pChannelPanel->UpdateData(FALSE);

			}
			else	// 纵向
			{
				i_start=m_pCScanView->m_dXStart;
				i_block2=fProbeDistance;
				double dXcoord = m_dXcoord;
				switch(nProbeNum)
				{
				case 2:
					//wangbingfu modify start at 2012-9-18
					/*	if (nTwoProbeIndex < 3)
					{//1,2  2,3  3,4
					nOffset=nTwoProbeIndex;
					i_block1=fProbeDistance * 2;
					} 
					else if(nTwoProbeIndex > 4)
					{//1,4
					i_block1=fProbeDistance * 6;
					i_block2=fProbeDistance * 3;
					}
					else
					{//1,3  2,4
					nOffset=nTwoProbeIndex-3;
					i_block1=fProbeDistance * 4;
					i_block2=fProbeDistance * 2;
					}*/
					if(nTwoProbeIndex == 0)	//1,2
					{
						nOffset = 0;
						i_block1=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 2)	//3,4
					{
						nOffset = 1;
						i_block1=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 3)	//1,3
					{
						nOffset = 0;
						i_block1=fProbeDistance * 4;
						i_block2=fProbeDistance * 2;
					}
					else if(nTwoProbeIndex == 4)	//2,4
					{
						nOffset = 1;
						i_block1=fProbeDistance * 4;
						i_block2=fProbeDistance * 2;
					}
					else
					{
						;//
					}
				case 3:
					//wangbingfu modify start at 2012-9-18
					//nOffset=nTwoProbeIndex;
					nOffset=nThreeProbeIndex;
					//wangbingfu modify end at 2012-9-18
					i_block1=fProbeDistance * 3;

					break;
				case 4:
					i_block1=fProbeDistance * 4;
					break;
				default:
					i_block1=fProbeDistance;
					break;
				}
				i_start += nOffset * fProbeDistance;

				int n=floor(fmod(dXcoord-i_start,i_block1)/i_block2);
				if (m_pCScanView->m_nProbeNum==2)
				{
					if (nTwoProbeIndex < 3)
					{//1,2  2,3  3,4
						n+=nOffset;
					} 
					else if (nTwoProbeIndex > 4)
					{//1,4
						if (n>0)
						{
							n=3;
						}
					}
					else
					{//1,3  2,4
						if (n>0)
						{
							n++;
						}
						n+=nOffset;
					}
				} 
				else
				{
					n+=nOffset;
				}


				dXcoord = dXcoord - n * fProbeDistance;
				m_dXcoord = dXcoord;

				//wangbingfu modify start at 2012-9-18
				//pChannelPanel->m_nChannelNo = n;
				pChannelPanel->m_nChannelNo = n;
				//wangbingfu modify end at 2012-9-18
				pChannelPanel->DoChangeChannel();
				pChannelPanel->UpdateData(FALSE);


			}
		}
	}
#endif	
	double dStart = 0.0f;	//起点
	int nChnNo = 0;	// 通道号
	double dJumpDistance = fProbeDistance;	// 跳间距
	double dScanDistance = fProbeDistance;	// 扫描间距
	int nOffSet = 0;// 探头起始偏移
	// wangbingfu add start on 2014-11-03
	float fW1Diameter = pApp->m_IniParams.m_info.fW1Diameter;
#if _HAFEI

	CMechanicalControlPanel* pMechanicalPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
	if (pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
	{
		fW1Diameter = pFrame->m_fWeiJianW1;
	}
#endif
	// wangbingfu add end on 2014-11-03

	//if(nProbeNum > 1)
	{
		if(nScanType == 0)
		{
			if(nFillType == 0)	// 横向
			{
				dStart = m_pCScanView->m_dYStart;
				double dYcoord = m_dYcoord;
				switch(nProbeNum)
				{
					case 2:
						if(nTwoProbeIndex == 0)	//1,2
						{
							nOffSet = 0;//
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 2)	//3,4
						{
							nOffSet = 2;//
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 3)	//1,3
						{
							nOffSet = 0;
							dScanDistance = fProbeDistance * 2;
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 4)	//2,4
						{
							nOffSet = 1;
							dScanDistance = fProbeDistance * 2;
							dJumpDistance = dScanDistance * 2;
						}
						else
						{
							;//
						}
						break;
					case 3:
						nOffSet = nThreeProbeIndex;
						dJumpDistance = fProbeDistance * 3;
						break;
					case 4:
						dJumpDistance = fProbeDistance * 4;
						break;
					default:
						dJumpDistance = fProbeDistance;
						break;
				}
			//	int nJump = (dYcoord - dStart) / dJumpDistance;	// 第几跳
				int nProbeSerial = static_cast<int>(floor(fmod(dYcoord - dStart, dJumpDistance) / dScanDistance)); // 探头顺序
				
				if(nProbeNum == 1)
				{
					if(nOneProbeIndex == 0)
					{
						nOffSet = 0;
						nChnNo = 0;
					}
					else if(nOneProbeIndex == 1)
					{
						nOffSet = 1;
						nChnNo = 1;
					}
					else if(nOneProbeIndex == 2)
					{
						nOffSet = 2;
						nChnNo = 2;
					}
					else if(nOneProbeIndex == 3)
					{
						nOffSet = 3;
						nChnNo = 3;
					}
					else
					{
						;//
					}
				}
				else if(nProbeNum == 2)
				{
					if(nTwoProbeIndex == 0)	// 1,2
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 0;
						}
						else if(nProbeSerial == 1)
						{
							nChnNo = 1;
						}
						else
						{
							;//
						}
					}
					else if(nTwoProbeIndex == 2)	// 3,4
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 2; // 通道3
						}
						else
						{
							nChnNo = 3; // 通道4
						}
					}
					else if(nTwoProbeIndex == 3)	//1,3
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 0; // 通道1
						}
						else
						{
							nChnNo = 2; // 通道3
						}
					}
					else if(nTwoProbeIndex == 4)	//2,4
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 1; // 通道2
						}
						else
						{
							nChnNo = 3; // 通道4
						}
					}
					else
					{
						;//
					}
				}
				else if(nProbeNum == 4)
				{
					if(nProbeSerial == 0)
					{
						nChnNo = 0;
					}
					else if(nProbeSerial == 1)
					{
						nChnNo = 1;
					}
					else if(nProbeSerial == 2)
					{
						nChnNo = 2;
					}
					else if(nProbeSerial == 3)
					{
						nChnNo = 3;
					}
					else
					{
						;//
					}

				}
				else
				{
					;//
				}
			/*	//dYcoord = dYcoord - nChnNo * fProbeDistance; // 控件按探头间距和起始探头有偏移（2,4或1，3时探头间距*2 就会不对）,不改控件，由这里补
				if(nOffSet > 0)
				{
					dYcoord = dYcoord - nChnNo* fProbeDistance + nOffSet * fProbeDistance;
				}
				else
				{
					dYcoord = dYcoord - nChnNo * fProbeDistance;
				}*/
				// 控件已修改
			//	dYcoord = dYcoord - nChnNo * fProbeDistance;
				
				dYcoord = dYcoord - (nChnNo - nOffSet) * fProbeDistance;//最低探头坐标

				m_dYcoord = dYcoord;

				pChannelPanel->m_nChannelNo = nChnNo;
				pChannelPanel->UpdateData(FALSE);
				pChannelPanel->DoChangeChannel();				
			}
			else	// 纵向
			{
				dStart = m_pCScanView->m_dXStart;
				double dXcoord = m_dXcoord;
				switch(nProbeNum)
				{
					case 2:
						if(nTwoProbeIndex == 0)	//1,2
						{
							nOffSet = 0;//
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 2)	//3,4
						{
							nOffSet = 2;//
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 3)	//1,3
						{
							nOffSet = 0;
							dScanDistance = fProbeDistance * 2;
							dJumpDistance = dScanDistance * 2;
						}
						else if(nTwoProbeIndex == 4)	//2,4
						{
							nOffSet = 1;
							dScanDistance = fProbeDistance * 2;
							dJumpDistance = dScanDistance * 2;
						}
						else
						{
							;//
						}
						break;
					case 3:
						nOffSet = nThreeProbeIndex;
						dJumpDistance = fProbeDistance * 3;
						break;
					case 4:
						nOffSet = 0;
						dJumpDistance = fProbeDistance * 4;
						break;
					default:
						dJumpDistance = fProbeDistance;
						break;
				}
				//	int nJump = (dYcoord - dStart) / dJumpDistance;	// 第几跳
				int nProbeSerial = static_cast<int>(floor(fmod(dXcoord - dStart, dJumpDistance) / dScanDistance)); // 探头顺序
				
				if(nProbeNum == 1)
				{
					if(nOneProbeIndex == 0)
					{
						nOffSet = 0;
						nChnNo = 0;
					}
					else if(nOneProbeIndex == 1)
					{
						nOffSet = 1;
						nChnNo = 1;
					}
					else if(nOneProbeIndex == 2)
					{
						nOffSet = 2;
						nChnNo = 2;
					}
					else if(nOneProbeIndex == 3)
					{
						nOffSet = 3;
						nChnNo = 3;
					}
					else
					{
						;//
					}
				}
				else if(nProbeNum == 2)
				{
					if(nTwoProbeIndex == 0)	// 1,2
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 0;
						}
						else if(nProbeSerial == 1)
						{
							nChnNo = 1;
						}
						else
						{
							;//
						}
					}
					else if(nTwoProbeIndex == 2)	// 3,4
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 2; // 通道3
						}
						else
						{
							nChnNo = 3; // 通道4
						}
					}
					else if(nTwoProbeIndex == 3)	//1,3
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 0; // 通道1
						}
						else
						{
							nChnNo = 2; // 通道3
						}
					}
					else if(nTwoProbeIndex == 4)	//2,4
					{
						if(nProbeSerial == 0)
						{
							nChnNo = 1; // 通道2
						}
						else
						{
							nChnNo = 3; // 通道4
						}
					}
					else
					{
						;//
					}
				}
				else if(nProbeNum == 4)
				{
					if(nProbeSerial == 0)
					{
						nChnNo = 0;
					}
					else if(nProbeSerial == 1)
					{
						nChnNo = 1;
					}
					else if(nProbeSerial == 2)
					{
						nChnNo = 2;
					}
					else if(nProbeSerial == 3)
					{
						nChnNo = 3;
					}
					else
					{
						;//
					}

				}
				else
				{
					;//
				}
			/*	//dYcoord = dYcoord - nChnNo * fProbeDistance; // 控件按探头间距和起始探头有偏移（2,4或1，3时探头间距*2 就会不对）,不改控件，由这里补
				if(nOffSet > 0)
				{
					dXcoord = dXcoord - nChnNo* fProbeDistance + nOffSet * fProbeDistance;
				}
				else
				{
					dXcoord = dXcoord - nChnNo * fProbeDistance;
				}*/
				// 控件已修改
			//	dXcoord = dXcoord - nChnNo * fProbeDistance;

				dXcoord = dXcoord - (nChnNo - nOffSet) * fProbeDistance;// 最低探头坐标

				m_dXcoord = dXcoord;				
			}
		}
		// wangbingfu add start on 2014-09-10
		if(m_pCScanView->m_bGuanLianFlag)	// 关联
		{
			int nValue = nChnNo % 2;
			if(m_pCScanView->m_nDblClickSelect == 1)	// C扫1 对应组探头的探头1
			{	
				if(nValue == 1)
				{
					nChnNo = nChnNo -1;
				}
			}
			else	// C扫2 对应组探头的探头2
			{
				if(nValue == 0)
				{
					nChnNo = nChnNo + 1;
				}
			}
		}
		// wangbingfu add start on 2014-09-10

		pChannelPanel->m_nChannelNo = nChnNo;
		pChannelPanel->UpdateData(FALSE);
		pChannelPanel->DoChangeChannel();
	}

	if (m_nAbsolute == 0)
	{
		m_dAcoord[0] = m_dXcoord;
		m_dAcoord[1] = m_dYcoord;
		m_dAcoord[2] = m_dZcoord;
		// wangbingfu add start on  2013-01-31
		m_dAcoord[3] = m_dABcoord;
		m_dAcoord[4] = m_dWcoord;
		m_dAcoord[5] = m_dRcoord * (m_pMovePathView->m_GraphMove.GetPipR() / fW1Diameter);	//驱动轮角度;	// wangbingfu modify on 2014-11-03 pApp->m_IniParams.m_info.fW1Diameter改fW1Diameter
		// wangbingfu add end on  2013-01-31
		
	}
	else
	{
		m_dRelativeCoord[0] = m_dXcoord;
		m_dRelativeCoord[1] = m_dYcoord;
		m_dRelativeCoord[2] = m_dZcoord;		
		// wangbingfu add start on  2013-01-31		
		m_dRelativeCoord[3] = m_dABcoord;
		m_dRelativeCoord[4] = m_dWcoord;
		// wangbingfu add end on  2013-01-31
		m_dRelativeCoord[5] = m_dRcoord * (m_pMovePathView->m_GraphMove.GetPipR() / fW1Diameter);	//驱动轮角度// wangbingfu modify on 2014-11-03 pApp->m_IniParams.m_info.fW1Diameter改fW1Diameter
		
	}
	double gs[6];//移动后目标坐标点
	memset(gs,0,sizeof(gs));
	m_pMovePathView->m_GraphMove.GetPos(m_dCurrentCoord);

	if (m_nAbsolute == 0)//绝对坐标
	{
		gs[0] = m_dAcoord[0];
		gs[1] = m_dAcoord[1];
		gs[2] = m_dAcoord[2];
		gs[3] = m_dAcoord[3];
		// wangbingfu add start on  2013-01-31		
		gs[4] = m_dAcoord[4];
		gs[5] = m_dAcoord[5];
		// wangbingfu add end on  2013-01-31
	}
	else	//相对坐标时，目标复位点是当前坐标与偏移量之和
	{
		gs[0] = m_dRelativeCoord[0] + m_dCurrentCoord[0];
		gs[1] = m_dRelativeCoord[1] + m_dCurrentCoord[1];
		gs[2] = m_dRelativeCoord[2] + m_dCurrentCoord[2];
		gs[3] = m_dRelativeCoord[3] + m_dCurrentCoord[3];
		// wangbingfu add start on  2013-01-31
		gs[4] = m_dRelativeCoord[4] + m_dCurrentCoord[4];
		gs[5] = m_dRelativeCoord[5] + m_dCurrentCoord[5];
		// wangbingfu add end on  2013-01-31
	}
	// wangbingfu modify start on  2013-01-31
	// Song Chenguang add start on 2013-05-10
	// 若w,a,b值与当前值不同，抬起到空走高度，再移动过去
	int nFlag = 0;
	if (fabs(m_dCurrentCoord[3] - gs[3]) > 1E-2 || fabs(m_dCurrentCoord[4] - gs[4]) > 1E-2)
	{
		nFlag = 2;
	}
	else
		nFlag = 0;
	// Song Chenguang add end on 2013-05-10

	if (m_pMovePathView->m_GraphMove.PtpMove(gs[0],gs[1],gs[2],gs[3],gs[4],gs[5], nFlag)) // 增加W1轴
	// wangbingfu modify end on  2013-01-31
	{
		KillTimer(MECHANICAL_TIMER);
		SetTimer(MECHANICAL_TIMER,10,NULL);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDefineReposition"),"Leave");
}

//相对坐标
void CMechanicalControlPanel::OnBnClickedXiangduiCrood()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedXiangduiCrood"),"Enter");
	m_nAbsolute = 1;

	UpdateData(TRUE);
	//将当前绝对坐标写入
	m_dAcoord[0] = m_Xcoord;
	m_dAcoord[1] = m_Ycoord;
	m_dAcoord[2] = m_Zcoord;
	m_dAcoord[3] = m_W1coord;

	//读出偏移量  注：目的是为了相对与绝对来回切换时能保存切换前所设置的值
	m_Xcoord = m_dRelativeCoord[0];
	m_Ycoord = m_dRelativeCoord[1];
	m_Zcoord = m_dRelativeCoord[2];
	m_W1coord = m_dRelativeCoord[3];

	UpdateData(FALSE);
	//设置静态文本显示
	m_staticTx.SetWindowText(_T("X偏移"));
	m_staticTy.SetWindowText(_T("Y偏移"));
	m_staticTz.SetWindowText(_T("Z偏移"));
	GetDlgItem(IDC_STATICAB)->SetWindowText(_T("AB偏移"));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedXiangduiCrood"),"Leave");
}
//绝对坐标
void CMechanicalControlPanel::OnBnClickedRadioAbsolute()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedRadioAbsolute"),"Enter");
	m_nAbsolute = 0;

	UpdateData(TRUE);
	//保存相对模式下设置的坐标
	m_dRelativeCoord[0] = m_Xcoord;
	m_dRelativeCoord[1] = m_Ycoord;
	m_dRelativeCoord[2] = m_Zcoord;
	m_dRelativeCoord[3] = m_W1coord;

	//显示绝对模式下的坐标
	m_Xcoord = m_dAcoord[0];
	m_Ycoord = m_dAcoord[1];
	m_Zcoord = m_dAcoord[2];
	m_W1coord = m_dAcoord[3];

	UpdateData(FALSE);
	//设置静态文本显示
	m_staticTx.SetWindowText(_T("X坐标"));
	m_staticTy.SetWindowText(_T("Y坐标"));
	m_staticTz.SetWindowText(_T("Z坐标"));
	GetDlgItem(IDC_STATICAB)->SetWindowText(_T("AB坐标"));
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedRadioAbsolute"),"Leave");
}

// 初始化系统参数对话框值
void CMechanicalControlPanel::InitSysParamDlg()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::InitSysParamDlg"),"Enter");
	BOOL bPara[4];//0：捕捉，1：刻度显示状态，2：路径显示状态，3：空走Z高度状态
	bPara[0] = m_pMovePathView->m_GraphMove.GetCaptureStatus();
	bPara[1] = m_pMovePathView->m_GraphMove.GetScaleStatus();
	bPara[2] = m_pMovePathView->m_GraphMove.GetPathStatus();
	bPara[3] = m_pMovePathView->m_GraphMove.GetUseStatus();
	m_SysParamDlg.m_nAxisIndex = m_pMovePathView->m_GraphMove.GetAxisSelect();

	m_pMovePathView->m_GraphMove.GetPara(m_dSysPara,m_logPara);//获取系统参数

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	//初始化系统参数控件变量
	m_SysParamDlg.m_dxl = pApp->m_IniParams.m_info.fXRange;
	m_SysParamDlg.m_dyl = pApp->m_IniParams.m_info.fYRange;
	m_SysParamDlg.m_dzl = pApp->m_IniParams.m_info.fZRange;
	m_SysParamDlg.m_dxacc = m_dSysPara[3];
	m_SysParamDlg.m_dyacc = m_dSysPara[4];
	m_SysParamDlg.m_dzacc = m_dSysPara[5];
	m_SysParamDlg.m_dXResetVel = m_dSysPara[6];
	m_SysParamDlg.m_dYResetVel = m_dSysPara[7];
	m_SysParamDlg.m_dZResetVel = m_dSysPara[8];
	m_SysParamDlg.m_dXMoveVel = m_dSysPara[9];
	m_SysParamDlg.m_dYMoveVel = m_dSysPara[10];
	m_SysParamDlg.m_dZMoveVel = m_dSysPara[11];
	m_SysParamDlg.m_dXKongVel = m_dSysPara[12];
	m_SysParamDlg.m_dYKongVel = m_dSysPara[13];
	m_SysParamDlg.m_dZKongVel = m_dSysPara[14];
	m_SysParamDlg.m_Time = m_dSysPara[15];
	m_SysParamDlg.m_dZHigh = m_dSysPara[16];
	m_SysParamDlg.m_dFillSpace = m_dSysPara[17];

	m_SysParamDlg.m_bCatch = bPara[0];
	m_SysParamDlg.m_bKeDu = bPara[1];
	m_SysParamDlg.m_bPath = bPara[2];
	m_SysParamDlg.m_bUse = bPara[3];

	m_SysParamDlg.m_nFillType = m_logPara[0];
	m_SysParamDlg.m_nProbeNum = m_logPara[3];
//	m_SysParamDlg.m_bUseSafeArea = m_logPara[4];
	m_SysParamDlg.m_bUseSafeArea = TRUE;

	m_SysParamDlg.m_fPerLength = static_cast<float>(m_dSysPara[18]);
//	m_SysParamDlg.m_fProbeDistance = m_dSysPara[19];	
	m_SysParamDlg.m_fSafeMinX = static_cast<float>(m_dSysPara[20]);	
	m_SysParamDlg.m_fSafeMinY = static_cast<float>(m_dSysPara[21]);
	m_SysParamDlg.m_fSafeMaxX = static_cast<float>(m_dSysPara[22]);
	m_SysParamDlg.m_fSafeMaxY = static_cast<float>(m_dSysPara[23]);
	m_SysParamDlg.m_fChongDie = static_cast<float>(m_dSysPara[24]);
	m_SysParamDlg.m_fSafeMaxZ = static_cast<float>(m_dSysPara[25]);
	m_SysParamDlg.m_nRSpeed = static_cast<unsigned int>(m_dSysPara[26]);
	// Song Chenguang add start on 2013-05-30
	m_SysParamDlg.m_nABSpeed = static_cast<unsigned int>(m_dSysPara[40]);
	m_SysParamDlg.m_nWSpeed = static_cast<unsigned int>(m_dSysPara[41]);
	// Song Chenguang add end on 2013-05-30
	// Song Chenguang add start on 2013-08-08
	// 尾减管检测标志
	int nTailDecreaseTemp = static_cast<int>(m_dSysPara[42]);
	if (nTailDecreaseTemp == 1)
		m_SysParamDlg.m_bTailDecreasePipe = TRUE;
	else
		m_SysParamDlg.m_bTailDecreasePipe = FALSE;
	// Song Chenguang add end on 2013-08-08
	// Song Chenguang add start on 2013-09-13
	// 变换Z，W轴标志（北方重工）
	int nChangeAixsTemp = static_cast<int>(m_dSysPara[44]);
	if (nChangeAixsTemp == 0 || nChangeAixsTemp == 1)
		m_SysParamDlg.m_bChangeZWAxis = FALSE;
	else
		m_SysParamDlg.m_bChangeZWAxis = TRUE;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bChangeZWAixs = m_SysParamDlg.m_bChangeZWAxis;
	// Song Chenguang add end on 2013-09-13

	CString strFillSpace = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("FillSpace2"), _T("1.0"));

	m_SysParamDlg.m_dFillSpace2 = _wtof(strFillSpace.GetBuffer(strFillSpace.GetLength()));

	// Songchenguang add start 2011-11-28

	m_SysParamDlg.m_nScanType = m_logPara[2];

	// Song chenguang modify start 2010-04-09
	// wangbingfu modify start at 2012-9-20
	m_SysParamDlg.m_nIndexProbeNum = m_logPara[3] - 1;

	// Song chenguang add start 2010-04-10
/*	if (m_logPara[3] == 1)
	{
		m_SysParamDlg.m_nIndexOneProbeType = m_logPara[5];
	}
	else if (m_logPara[3] == 2)
	{
		m_SysParamDlg.m_nIndexTwoProbeType = m_logPara[5];
	}
	else if (m_logPara[3] == 3)
	{
		m_SysParamDlg.m_nIndexThreeProbeType = m_logPara[5];
	}
	else
	{
	}*/
	CString str = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), _T("1"));
	m_SysParamDlg.m_nIndexProbeNum = static_cast<int>(_wtof(str.GetBuffer(str.GetLength())) - 1);
	str = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), _T("1"));
	m_SysParamDlg.m_nIndexOneProbeType = static_cast<int>(_wtof(str.GetBuffer(str.GetLength())));
	str = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), _T("1"));
	m_SysParamDlg.m_nIndexTwoProbeType = static_cast<int>(_wtof(str.GetBuffer(str.GetLength())));
	str = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), _T("1"));
	m_SysParamDlg.m_nIndexThreeProbeType = static_cast<int>(_wtof(str.GetBuffer(str.GetLength())));
	// wangbingfu modify end at 2012-9-20
	// Song chenguang add end 2010-04-10

	CString strShangLiaoX = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("上料X坐标"), _T("0.0"));
	m_SysParamDlg.m_fShangLiaoX = static_cast<float>(_wtof(strShangLiaoX.GetBuffer(strShangLiaoX.GetLength())));
	CString strShangLiaoY = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("上料Y坐标"), _T("0.0"));
	m_SysParamDlg.m_fShangLiaoY = static_cast<float>(_wtof(strShangLiaoY.GetBuffer(strShangLiaoY.GetLength())));
	
	// wangbingfu modify start at 2012-9-18
/*	if (m_SysParamDlg.m_nIndexProbeNum == 1)
	{
		if (m_SysParamDlg.m_nIndexTwoProbeType > 2 && m_SysParamDlg.m_nIndexTwoProbeType < 5)
		{
			m_SysParamDlg.m_fProbeDistance = m_dSysPara[19] / 2;
		}
		else if (m_SysParamDlg.m_nIndexTwoProbeType == 5)
		{
			m_SysParamDlg.m_fProbeDistance = m_dSysPara[19] / 3;
		}
		else
		{
			m_SysParamDlg.m_fProbeDistance = m_dSysPara[19];
		}
	}
	else
	{
		m_SysParamDlg.m_fProbeDistance = m_dSysPara[19];
	}*/
	
/*	CString strProbeNum = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), _T("1"));
	int nProbeNum = _wtoi(strProbeNum.GetBuffer(strProbeNum.GetLength()));
	if (nProbeNum == 2 && m_SysParamDlg.m_nScanType == 0)
	{
		m_SysParamDlg.m_fProbeDistance = m_dSysPara[19] / 2;
	}
	else
	{
		m_SysParamDlg.m_fProbeDistance = m_dSysPara[19];
	}*/
	// wangbingfu modify end at 2012-9-18

	// Songchenguang add end 2011-11-28
	// Song Chenguang add start 2011-04-17
	CString strProbeX1 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeX1"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[0][0] = static_cast<float>(_wtof(strProbeX1.GetBuffer(strProbeX1.GetLength())));

	CString strProbeX2 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeX2"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[1][0] = static_cast<float>(_wtof(strProbeX2.GetBuffer(strProbeX2.GetLength())));

	CString strProbeX3 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeX3"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[2][0] = static_cast<float>(_wtof(strProbeX3.GetBuffer(strProbeX3.GetLength())));

	CString strProbeX4 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeX4"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[3][0] = static_cast<float>(_wtof(strProbeX4.GetBuffer(strProbeX4.GetLength())));

	CString strProbeY1 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeY1"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[0][1] = static_cast<float>(_wtof(strProbeY1.GetBuffer(strProbeY1.GetLength())));

	CString strProbeY2 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeY2"), _T("50.0"));
	m_SysParamDlg.m_fProbeLocation[1][1] = static_cast<float>(_wtof(strProbeY2.GetBuffer(strProbeY2.GetLength())));
	
	CString strProbeY3 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeY3"), _T("100.0"));
	m_SysParamDlg.m_fProbeLocation[2][1] = static_cast<float>(_wtof(strProbeY3.GetBuffer(strProbeY3.GetLength())));

	CString strProbeY4 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeY4"), _T("150.0"));
	m_SysParamDlg.m_fProbeLocation[3][1] = static_cast<float>(_wtof(strProbeY4.GetBuffer(strProbeY4.GetLength())));

	CString strProbeZ1 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeZ1"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[0][2] = static_cast<float>(_wtof(strProbeZ1.GetBuffer(strProbeZ1.GetLength())));

	CString strProbeZ2 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeZ2"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[1][2] = static_cast<float>(_wtof(strProbeZ2.GetBuffer(strProbeZ2.GetLength())));

	CString strProbeZ3 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeZ3"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[2][2] = static_cast<float>(_wtof(strProbeZ3.GetBuffer(strProbeZ3.GetLength())));

	CString strProbeZ4 = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeZ4"), _T("0.0"));
	m_SysParamDlg.m_fProbeLocation[3][2] = static_cast<float>(_wtof(strProbeZ4.GetBuffer(strProbeZ4.GetLength())));

	//wangbingfu add start at 2012-9-20
	CString strProbeDistance = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeDistance"), _T("0.0"));
	m_SysParamDlg.m_fProbeDistance = static_cast<float>(_wtof(strProbeDistance.GetBuffer(strProbeDistance.GetLength())));
	//wangbingfu add end at 2012-9-20

	// Song Chenguang add end 2011-04-17

	if (m_SysParamDlg.m_nAxisIndex == 3 
		|| m_SysParamDlg.m_nAxisIndex == 4) // R管 // wangbingfu add on 2013-01-09
	{
		GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(FALSE);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::InitSysParamDlg"),"Leave");
}
/** 
* 设置系统参数按钮响应函数
* @param     void                                              
* @return    void
* @since     1.0
*/
//参数设置button按钮消息响应
void CMechanicalControlPanel::OnBnClickedButtonSysparam()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonSysparam"),"Enter");
	InitSysParamDlg();
	UpdateData(FALSE);	// Song Chenguang add on 2013-08-13 更新输入值上下限

	BOOL bPara[4] = {FALSE, FALSE, FALSE, FALSE};
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	if (m_SysParamDlg.DoModal() == IDOK)
	{
		//将当前设置的控件变量的值保存
		m_dSysPara[0] = m_SysParamDlg.m_dxl;
		m_dSysPara[1] = m_SysParamDlg.m_dyl;
		m_dSysPara[2] = m_SysParamDlg.m_dzl;
		m_dSysPara[3] = m_SysParamDlg.m_dxacc;
		m_dSysPara[4] = m_SysParamDlg.m_dyacc;
		m_dSysPara[5] = m_SysParamDlg.m_dzacc;
		m_dSysPara[6] = m_SysParamDlg.m_dXResetVel;
		m_dSysPara[7] = m_SysParamDlg.m_dYResetVel;
		m_dSysPara[8] = m_SysParamDlg.m_dZResetVel;
		m_dSysPara[9] = m_SysParamDlg.m_dXMoveVel;
		m_dSysPara[10] = m_SysParamDlg.m_dYMoveVel;
		m_dSysPara[11] = m_SysParamDlg.m_dZMoveVel;
		m_dSysPara[12] = m_SysParamDlg.m_dXKongVel;
		m_dSysPara[13] = m_SysParamDlg.m_dYKongVel;
		m_dSysPara[14] = m_SysParamDlg.m_dZKongVel;
		m_dSysPara[15] = m_SysParamDlg.m_Time;
		m_dSysPara[16] = m_SysParamDlg.m_dZHigh;
		m_dSysPara[17] = m_SysParamDlg.m_dFillSpace;

		m_logPara[0] = m_SysParamDlg.m_nFillType;
		m_logPara[3] = m_SysParamDlg.m_nProbeNum;
		m_logPara[4] = m_SysParamDlg.m_bUseSafeArea;
		m_logPara[2] = m_SysParamDlg.m_nScanType;

		m_dSysPara[18] = m_SysParamDlg.m_fPerLength;
		m_dSysPara[19] = m_SysParamDlg.m_fProbeDistance;	

		m_dSysPara[20] = m_SysParamDlg.m_fSafeMinX;
		m_dSysPara[21] = m_SysParamDlg.m_fSafeMinY;
		m_dSysPara[22] = m_SysParamDlg.m_fSafeMaxX;
		m_dSysPara[23] = m_SysParamDlg.m_fSafeMaxY;
		m_dSysPara[25] = m_SysParamDlg.m_fSafeMaxZ;
		
		m_dSysPara[24] = m_SysParamDlg.m_fChongDie;
		m_dSysPara[26] = m_SysParamDlg.m_nRSpeed;
		// Song Chenguang add start on 2013-05-30
		m_dSysPara[40] = m_SysParamDlg.m_nABSpeed;
		m_dSysPara[41] = m_SysParamDlg.m_nWSpeed;
		// Song Chenguang add end on 2013-05-30
		// Song Chenguang add start on 2013-08-08
		// 尾减管切换零点标志
 		if (m_SysParamDlg.m_bTailDecreasePipe)
 		{
 			m_dSysPara[42] = 1;
 		} 
 		else
 		{
 			m_dSysPara[42] = 0;
 		}
		// Song Chenguang add end on 2013-08-08
		// Song Chenguang add start on 2013-09-04
		// W平面步进标志
		if (m_SysParamDlg.m_nAxisIndex == 5)
		{
			m_dSysPara[43] = m_SysParamDlg.m_nFillType;
		}
		// Song Chenguang add end on 2013-09-04
		// Song Chenguang add start on 2013-09-13
		// 变换Z，W轴标志
		if (m_SysParamDlg.m_bChangeZWAxis)
			m_dSysPara[44] = 2;
#if _BEIFANG_ZHONGGONG
		else
			m_dSysPara[44] = 1;		
#else
		else
			m_dSysPara[44] = 0;
#endif

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_bChangeZWAixs = m_SysParamDlg.m_bChangeZWAxis;
		// Song Chenguang add end on 2013-09-13

		bPara[0] = m_SysParamDlg.m_bCatch;
		bPara[1] = m_SysParamDlg.m_bKeDu;
		bPara[2] = m_SysParamDlg.m_bPath;
		bPara[3] = m_SysParamDlg.m_bUse;

 		m_pMovePathView->m_GraphMove.SetCaptureStatus(bPara[0]);
 		m_pMovePathView->m_GraphMove.SetScaleStatus(bPara[1]);
 		m_pMovePathView->m_GraphMove.SetPathStatus(bPara[2]);
 		m_pMovePathView->m_GraphMove.SetUseStatus(bPara[3]);

		CString str;
		str.Format(_T("%.2f"), m_SysParamDlg.m_dFillSpace2);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("FillSpace2"), str);

		// Songchenguang add start 2011-11-28
/*		str.Format(_T("%d"), m_SysParamDlg.m_nScanType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ScanType"), str);
*/		
		str.Format(_T("%d"), m_SysParamDlg.m_nIndexProbeNum + 1);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), str);
		
		str.Format(_T("%d"), m_SysParamDlg.m_nIndexOneProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), str);
		
		str.Format(_T("%d"), m_SysParamDlg.m_nIndexTwoProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), str);

		str.Format(_T("%d"), m_SysParamDlg.m_nIndexThreeProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fShangLiaoX);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("上料X坐标"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fShangLiaoY);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("上料Y坐标"), str);
		// songchenguang add start 2012-04-18
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[0][0]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeX1"), str);
		
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[1][0]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeX2"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[2][0]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeX3"), str);
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[3][0]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeX4"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[0][1]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeY1"), str);
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[1][1]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeY2"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[2][1]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeY3"), str);
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[3][1]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeY4"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[0][2]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeZ1"), str);
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[1][2]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeZ2"), str);

		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[2][2]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeZ3"), str);
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeLocation[3][2]);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeZ4"), str);
		
		//wangbingfu add start at 2012-9-20
		str.Format(_T("%f"), m_SysParamDlg.m_fProbeDistance);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeDistance"), str);
		
		int nProbeNum = 0;
		int nOneProbeIndex = 0;
		int nTwoProbeIndex = 0;
		int nThreeProbeIndex = 0;
		m_SysParamDlg.Group2ProbeValue(nProbeNum, nOneProbeIndex, nTwoProbeIndex, nThreeProbeIndex);
		if (nProbeNum == 2 && m_SysParamDlg.m_nScanType == 0) // modify by Song chenguang on 2012-08-02
		{
			if (nTwoProbeIndex == 3 || nTwoProbeIndex == 4)
			{
				m_dSysPara[19] *= 2;
			}
			else
			{
			}
		}
		//wangbingfu add start on 2012-12-11
		// 苏晶 棒材扫查 探头竖排不改成横排时
		if((m_SysParamDlg.m_nAxisIndex == 3 //W1
			|| m_SysParamDlg.m_nAxisIndex == 4)	//W1管 // wangbingfu add on 2013-01-09
			&&(m_SysParamDlg.m_fProbeLocation[1][1] != 0))
		{
			m_logPara[6] = 1;	// 横排
		}
		else
		{
			m_logPara[6] = 0;	// 竖排
		}
		m_logPara[7] = m_SysParamDlg.m_nStepMode;
	
		m_pMovePathView->m_GraphMove.SetPara(m_dSysPara,m_logPara);
		m_SysParamDlg.SetMoveRule(m_SysParamDlg.m_nIndexProbeNum+1, m_SysParamDlg.m_nIndexOneProbeType, 
					m_SysParamDlg.m_nIndexTwoProbeType, m_SysParamDlg.m_nIndexThreeProbeType, m_SysParamDlg.m_wWorkMode);

		
	
		// wangbingfu delete end at 2012-9-20

		if (m_SysParamDlg.m_nAxisIndex == 3
			|| m_SysParamDlg.m_nAxisIndex == 4)	//W1管 // wangbingfu add on 2013-01-09
		{
			GetDlgItem(IDC_BUTTON_GET_RECT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DISPLAY_RECT)->EnableWindow(FALSE);
		} 
		else
		{
			GetDlgItem(IDC_BUTTON_GET_RECT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DISPLAY_RECT)->EnableWindow(TRUE);
		}
		m_pCScanView->Invalidate(TRUE);
		m_pCScanView2->Invalidate(TRUE);
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonSysparam"),"Leave");
}


/** 
* 设置单步运动步进
* @param     void                                              
* @return    void
* @since     1.0
*/
void CMechanicalControlPanel::OnBnClickedButtonSetstep()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonSetstep"),"Enter");
	CMechanicalStepParamDlg stepDlg;
	stepDlg.m_fXStep = m_fXstep;
	stepDlg.m_fYStep = m_fYstep;
	stepDlg.m_fZStep = m_fZstep;
	// Song Chenguang add start on 2013-03-15 
	stepDlg.m_fABStep = m_fABStep;
	stepDlg.m_fWStep = m_fWStep;
	// Song Chenguang add end on 2013-03-15
	stepDlg.m_fRStep = m_fRStep;

	if (IDOK == stepDlg.DoModal())
	{
		UpdateData(TRUE);
		m_fXstep = stepDlg.m_fXStep;
		m_fYstep = stepDlg.m_fYStep;
		m_fZstep = stepDlg.m_fZStep;
		// Song Chenguang add start on 2013-03-15 
		m_fABStep = stepDlg.m_fABStep;
		m_fWStep = stepDlg.m_fWStep;
		// Song Chenguang add end on 2013-03-15
		m_fRStep = stepDlg.m_fRStep;

		SetStepLabelText();
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonSetstep"),"Leave");


}
/** 
* 定时器响应函数
* @param     nIDEvent [in] 定时器的序号                                              
* @return    void
* @since     1.0
*/
void CMechanicalControlPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnTimer"),"Enter");
	if (nIDEvent == MECHANICAL_TIMER && m_pMovePathView->m_GraphMove.GetSafeHwnd() != NULL)
//	if(0)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		BOOL bNofinish = FALSE;
		long lMoveCheck = 0;//MoveCheck()参数，传入long 型指针，返回当前图形录序号
		short sMoveCheck = 0;//MoveCheck()参数
        /*short status*/;
		short status = m_pMovePathView->m_GraphMove.GetWorkStatus();
		if (status == 100)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("TRIO通讯故障！"));
			CFormView::OnTimer(nIDEvent);
			////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnTimer"),"Leave");
			return;
		}
		else if (status == 9)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("设备已紧急停车！"));
		}
		else if (status == 90)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("移动目标超出安全区！"));
		}
		else if (status == 101)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("移动目标超出软限位！"));
		}
		else if (status == 102)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("移动目标超出硬限位！"));
		}
		else if (status == 103)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("伺服故障！"));
		}
		else if (status == 112)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("W1轴接近开关感应故障！"));
		}
		else if (status == 119)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("X两轴不同步！"));
		}
		else if (status == 999)
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("Trio卡故障：使能关闭！"));
		}
		else
		{
			pFrame->m_wndStatusBar.SetWindowText(_T("运行正常"));
		}
		if (status > 99 && status!= 200)
		{
			CString str;
			str.Format(_T("status值异常，status = %d"), status);
			pFrame->m_wndStatusBar.SetPaneText(2, str);
			PathStop(TRUE);
		}
		if(status == 1 || (status>2&&status<9))	
		{
			//轨迹运动、手动运行、PTP运行时调用MoveCheck
			bNofinish = m_pMovePathView->m_GraphMove.MoveCheck(&lMoveCheck,&sMoveCheck);

			// Song Chenguang add start on 2013-06-29
			// 记录当前行号
			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			BOOL bRet = pApp->WriteProfileInt(_T("水浸超声探伤系统"), _T("HaltLineNum"), int(lMoveCheck));
			if (!bRet)
			{
				MessageBox(_T("行号记录失败！"), _T("提示"), MB_OK);
			}
			// Song Chenguang add end on 2013-06-29
			status = m_pMovePathView->m_GraphMove.GetWorkStatus();

			//TRACE(_T("bNofinish = %d, status = %d\r\n"), bNofinish, status);
			if(!bNofinish)
			{
				KillTimer(MECHANICAL_TIMER);
				SetTimer(MECHANICAL_TIMER,50,NULL);
			}
			else 
			{
				//采集点开始
				if(status == 4)
				{

				}
			}
		
		}
	}	
	else if (TIMER_ASYNC_SAVE_SCAN == nIDEvent)	//同步保存
	{
		short status = m_pMovePathView->m_GraphMove.GetWorkStatus();
		if (status==1)
		{
			// Song Chenguang add start on 2013-04-06
			m_AsyncCScanFile.Open(m_strTempPath+L"acf", CFile::modeWrite, NULL);
			m_AsyncAScanFile.Open(m_strTempPath+L"aaf", CFile::modeReadWrite, NULL);	
			m_AsyncAScanFile1.Open(m_strTempPath1+L"aaf", CFile::modeReadWrite, NULL); // wangbingfu add on 2014-09-10
			m_AsyncAScanPrimitiveFile.Open(m_strTempPath+L"paaf", CFile::modeWrite, NULL);	// Song Chenguang add on 2014-01-15
			//m_AscanTxtFile.Open(_T("C:\\C扫数据.txt"), CFile::modeWrite, NULL);

			m_AsyncCScanFile.SeekToEnd();
			m_AsyncAScanFile.SeekToEnd();	
			m_AsyncAScanFile1.SeekToEnd(); // wangbingfu add on 2014-09-10
			//m_AscanTxtFile.SeekToEnd();
			// Song Chenguang add end on 2013-04-06

			CUltrasonicImmersedTestDoc* pDoc = GetDocument();
			//mAsyncCScanFile	CFeatureInfo	CPicData			pDoc->m_qAScanData[i]	pDoc->m_qCScanData[i]

/*			// wangbing add output test start on 2014-10-11
			int nASize = pDoc->m_qAScanData[0].size();
			TRACE(_T("Asize before %d"), nASize);
			CString strA;
			SYSTEMTIME sysTime;
			::GetLocalTime(&sysTime);
			strA.Format(_T("%d:%d:%d Asize before %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nASize);
		//	strA.Format(_T("Asize before %d\n"), nASize);
			OutputDebugString(strA);
		//	wsprintf(strA.GetBuffer(), _T("Asize before %d\n"), nASize);

			int nCSize = pDoc->m_qCScanData[0].size();
			TRACE(_T("Csize before %d"), nCSize);
			CString strC;
			::GetLocalTime(&sysTime);
			strC.Format(_T("%d:%d:%d Csize before %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nCSize);
		//	strC.Format(_T("Csize before %d\n"), nCSize);
			OutputDebugString(strC);
		//	wsprintf(strC.GetBuffer(), _T("Csize before %d\n"), nCSize);
			//	sprintf(ca, "Asize before %d", nASize);
			// wangbing add outpu test end on 2014-10-11
*/
			for (int i = 0; i < CHANNEL_NUM; i++)
			{
				// wangbingfu modify start at 2012-10-12
				while(!pDoc->m_qCScanData[i].empty()) 
				{
					CFeatureInfo ndata=pDoc->m_qCScanData[i].front();

					ULONG dlength=sizeof(double)*7+sizeof(int)*3+sizeof(float)*ndata.m_nFeatureNum*ndata.m_nGateNum;
					m_AsyncCScanFile.Write(&i, sizeof(int));
					m_AsyncCScanFile.Write(&dlength, sizeof(ULONG));

					m_AsyncCScanFile.Write(&ndata.m_dX, sizeof(double));
					m_AsyncCScanFile.Write(&ndata.m_dY, sizeof(double));
					m_AsyncCScanFile.Write(&ndata.m_dZ, sizeof(double));
					m_AsyncCScanFile.Write(&ndata.m_dR, sizeof(double));

					m_AsyncCScanFile.Write(&ndata.m_dA, sizeof(double));
					m_AsyncCScanFile.Write(&ndata.m_dB, sizeof(double));
					m_AsyncCScanFile.Write(&ndata.m_dW, sizeof(double));

					m_AsyncCScanFile.Write(&ndata.m_nGateNum, sizeof(int));
					m_AsyncCScanFile.Write(&ndata.m_nFeatureNum, sizeof(int));
					m_AsyncCScanFile.Write(&ndata.m_nCount, sizeof(int));

					for (int j=0; j<ndata.m_nGateNum; j++)
					{
						m_AsyncCScanFile.Write(ndata.m_pfFeatures[j], sizeof(float)*ndata.m_nFeatureNum);
					}
/*
					//////////////////////////////////////////////////////////////////////////
					// C扫数据打印 2014-01-02
					CString str;
					str.Format(_T("%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\r\n"),
						ndata.m_dX, ndata.m_dY, ndata.m_dZ, ndata.m_dA, ndata.m_dB,
						ndata.m_dW, ndata.m_dR);
					m_AscanTxtFile.SeekToEnd();
					m_AscanTxtFile.WriteString(str);

					CString strAllGate;
					for (int i=0; i<ndata.m_nGateNum; i++)
					{
						CString strGate;
						for (int j=0; j<ndata.m_nFeatureNum; j++)
						{
							CString strUnit;
							strUnit.Format(_T("%.2f\t"), ndata.m_pfFeatures[i][j]);
							strGate += strUnit;
						}
						strGate += _T("\r\n");
						strAllGate += strGate;
					}

					m_AscanTxtFile.WriteString(strAllGate);
					//////////////////////////////////////////////////////////////////////////
*/
					pDoc->m_qCScanData[i].pop();
				}
				// wangbingfu modify end at 2012-10-12
				// Song Chenguang add start on 2014-01-15
				// 获取闸门成像状态
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				int nValueMode = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nValueMode;
				int nGateIndex = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nGateIndex;
				int nFeatureIndex = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nFeatureIndex;
				// Song Chenguang add end on 2014-01-15
				// wangbingfu add modify start on 2014-09-10 // 加上关联分两个文件存储的判断
				
				CFile* pFile;
				if(m_pCScanView->m_bGuanLianFlag)
				{
					int nNum = i % 2;
					if(nNum == 0)	// 卡的第一个通道
					{
						pFile = &m_AsyncAScanFile;
					}
					else	// 卡的第二个通道
					{
						pFile = &m_AsyncAScanFile1;
					}
				}
				else
				{
					pFile = &m_AsyncAScanFile;
				}
				
				while(!pDoc->m_qAScanData[i].empty()) 
				{
					CPicData_L ndata=pDoc->m_qAScanData[i].front();	// Song Chenguang modify on 2013-12-19
					m_AsyncAScanPrimitiveFile.Write(&ndata, sizeof(CPicData_L));	// Song chenguang add on 2014-01-15

					ULONG i_grid_sn;
					//m_grid_tmp
					if (m_grid_tmp.AxisSelect == 3 || m_grid_tmp.AxisSelect == 4)
					{
//						i_grid_sn=static_cast<ULONG>(floor(ndata.m_fR/m_grid_tmp.gridY));
						// wangbingfu modify start on 2014-11-04
				//		i_grid_sn=static_cast<ULONG>(ndata.m_fR / m_grid_tmp.gridY + 0.5);	// 改为四舍五入
						i_grid_sn = static_cast<ULONG>(ULONG(fabs(ndata.m_fX - m_grid_tmp.startX) / m_grid_tmp.gridX + 0.5) + 
							ULONG(fabs(ndata.m_fR) / m_grid_tmp.gridY + 0.5) * m_grid_tmp.gridXSize);
						// wangbingfu modify end on 2014-11-04
					} 
					// Song Chenguang add start on 2013-11-27
					else if (m_grid_tmp.AxisSelect == 5)
					{
//						i_grid_sn=static_cast<ULONG>(floor(fabs(ndata.m_fW - m_grid_tmp.startX)/m_grid_tmp.gridX)+floor(fabs(ndata.m_fZ - m_grid_tmp.startY)/m_grid_tmp.gridY)*m_grid_tmp.gridXSize);
						i_grid_sn = static_cast<ULONG>(ULONG(fabs(ndata.m_fW - m_grid_tmp.startX)/m_grid_tmp.gridX + 0.5) + 
							ULONG(fabs(ndata.m_fZ - m_grid_tmp.startY) / m_grid_tmp.gridY + 0.5) * m_grid_tmp.gridXSize);
					}
					// Song Chenguang add end on 2013-11-27
					else
					{
//						i_grid_sn=static_cast<ULONG>(floor(fabs(ndata.m_fX-m_grid_tmp.startX)/m_grid_tmp.gridX)+floor(fabs(ndata.m_fY-m_grid_tmp.startY)/m_grid_tmp.gridY)*m_grid_tmp.gridXSize);
						i_grid_sn = static_cast<ULONG>(ULONG(fabs(ndata.m_fX - m_grid_tmp.startX) / m_grid_tmp.gridX + 0.5) + 
							ULONG(fabs(ndata.m_fY - m_grid_tmp.startY) / m_grid_tmp.gridY + 0.5) * m_grid_tmp.gridXSize);						
					}
					
					if (i_grid_sn<m_grid_tmp.gridSize)
					{
						// Song Chenguang modify start on 2014-01-15
						pFile->Seek(i_grid_sn*sizeof(ULONG)+m_grid_tmp.headSize, CFile::begin);
						ULONG ulTempPos = 0;
						pFile->Read(&ulTempPos, sizeof(ULONG));

						if (ulTempPos == 0)
						{							
							pFile->SeekToEnd();
							ULONG i_postion=(ULONG)pFile->GetPosition();
							pFile->Seek(i_grid_sn*sizeof(ULONG)+m_grid_tmp.headSize, CFile::begin);
							pFile->Write(&i_postion, sizeof(ULONG));
							pFile->SeekToEnd();
							pFile->Write(&i, sizeof(int));
							pFile->Write(&ndata, sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
							m_grid_tmp.lastIndex=i_grid_sn;
							m_grid_tmp.isEmpty=false;
						}
						else
						{
							// 读取原值
							pFile->Seek(ulTempPos + sizeof(int), CFile::begin);
							CPicData_L tempData;
							pFile->Read(&tempData, sizeof(CPicData_L));

							// 与当前值比较，决定用哪个
							CPicData_L DataToWrite;
							CompareTwoAscanData(tempData, ndata, DataToWrite, nValueMode, nFeatureIndex, nValueMode);

							// 写值
							pFile->Seek(ulTempPos, CFile::begin);
							pFile->Write(&i, sizeof(int));
							pFile->Write(&DataToWrite, sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
							m_grid_tmp.lastIndex = i_grid_sn;
							m_grid_tmp.isEmpty = false;
						}
						// Song Chenguang modify end on 2014-01-15
					}

/*
					CString str;
					str.Format(_T("%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\t\t%.3f\r\n"),
						ndata.m_fX, ndata.m_fY, ndata.m_fZ, ndata.m_fA, ndata.m_fB,
						ndata.m_fW, ndata.m_fR);
					m_AscanTxtFile.SeekToEnd();
					m_AscanTxtFile.WriteString(str);
					//////////////////////////////////////////////////////////////////////////
					static int ascancount = 0;

					if (ascancount < 10)
					{
						CString strWave;
						for (int i=0; i<DATA_SIZE; i++)
						{
							int nTemp = i % 16;
							if (i !=0 && nTemp == 0)
							{
								strWave += _T("\r\n");
							}

							CString strTemp;
							strTemp.Format(_T("%d\t"), ndata.m_wData[i]);
							strWave += strTemp;							
						}
						strWave += _T("\r\n");
						m_AscanTxtFile.WriteString(strWave);
						CFeatureInfo info(4,5);
						PrimitiveDataChange(ndata.m_stFlawData, info, ndata.m_nCardIndex, ndata.m_nChannelIndex);

						CString strAllGate;
						for (int i=0; i<4; i++)
						{
							CString strGate;
							for (int j=0; j<5; j++)
							{
								CString strUnit;
								strUnit.Format(_T("%.2f\t"), info.m_pfFeatures[i][j]);
								strGate += strUnit;
							}
							strGate += _T("\r\n");
							strAllGate += strGate;
						}

						m_AscanTxtFile.WriteString(strAllGate);
						ascancount++;
					}
					//////////////////////////////////////////////////////////////////////////
*/
					pDoc->m_qAScanData[i].pop();
				}
				// wangbingfu modify end on 2014-09-10			
			}

/*			// wangbing test start on 2014-10-11
			int nASize1 = pDoc->m_qAScanData[0].size();
			TRACE(_T("Asize after %d"), nASize1);
			CString strA1;
			::GetLocalTime(&sysTime);
			strA1.Format(_T("%d:%d:%d Asize after %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nASize1);
		//	strA1.Format(_T("Asize after %d\n"), nASize1);
			OutputDebugString(strA1);
		//	wsprintf(strA1.GetBuffer(), _T("Asize after %d\n"), nASize1);

			int nCSize1 = pDoc->m_qCScanData[0].size();
			TRACE(_T("Csize after %d"), nCSize1);
			CString strC1;
			::GetLocalTime(&sysTime);
			strC1.Format(_T("%d:%d:%d Csize after %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nCSize1);
		//	strC1.Format(_T("Csize after %d\n"), nCSize1);
			OutputDebugString(strC1);
		//	wsprintf(strC1.GetBuffer(), _T("Csize after %d\n"), nCSize1);
			// wangbing test end on 2014-10-11
*/
			// Song Chenguang add start on 2013-04-06
			m_AsyncCScanFile.Close();
			m_AsyncAScanFile.Close();	// Song Chenguang rem on 2014-01-15
			m_AsyncAScanFile1.Close();	// wangbingfu add on 2017-09-10
			m_AsyncAScanPrimitiveFile.Close();	// Song Chenguang add on 2014-01-15
			//m_AscanTxtFile.Close();
			// Song Chenguang add end on 2013-04-06
		}
	}
	else
	{
	}
	CFormView::OnTimer(nIDEvent);
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnTimer"),"Leave");
}

// Song Chenguang add start on 2013-01-15
// 比较两个A扫数据，data3为输出要保存的值
void CMechanicalControlPanel::CompareTwoAscanData(const CPicData_L& data1, const CPicData_L& data2, CPicData_L& data3, 
						 const int& nGateIndex, const int& nFeatureIndex, const int& nValueMode)
{
	WORD wData1 = 0;
	WORD wData2 = 0;
	if (nGateIndex == 1)	// A闸门
	{
		if (nFeatureIndex == 0)	// 一次波过阈值时间
		{
			wData1 = data1.m_stFlawData.wGate2FPFirstTime;
			wData2 = data2.m_stFlawData.wGate2FPFirstTime;
		} 
		else if (nFeatureIndex == 1) // 一次波峰时间
		{
			wData1 = data1.m_stFlawData.wGate2FPTime;
			wData2 = data2.m_stFlawData.wGate2FPTime;
		}
		else if (nFeatureIndex == 3) // 最大波峰时间
		{
			wData1 = data1.m_stFlawData.wGate2MPTime;
			wData2 = data2.m_stFlawData.wGate2MPTime;
		}
		else if (nFeatureIndex == 4) // 一次波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate2FPAmp;
			wData2 = data2.m_stFlawData.wGate2FPAmp;
		}
		else if (nFeatureIndex == 5) // 最大波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate2MPAmp;
			wData2 = data2.m_stFlawData.wGate2MPAmp;
		}
		else
		{
		}
	}
	else if (nGateIndex == 2)	// B闸门
	{
		if (nFeatureIndex == 0)	// 一次波过阈值时间
		{
			wData1 = data1.m_stFlawData.wGate3FPFirstTime;
			wData2 = data2.m_stFlawData.wGate3FPFirstTime;
		} 
		else if (nFeatureIndex == 1) // 一次波峰时间
		{
			wData1 = data1.m_stFlawData.wGate3FPTime;
			wData2 = data2.m_stFlawData.wGate3FPTime;
		}
		else if (nFeatureIndex == 3) // 最大波峰时间
		{
			wData1 = data1.m_stFlawData.wGate3MPTime;
			wData2 = data2.m_stFlawData.wGate3MPTime;
		}
		else if (nFeatureIndex == 4) // 一次波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate3FPAmp;
			wData2 = data2.m_stFlawData.wGate3FPAmp;
		}
		else if (nFeatureIndex == 5) // 最大波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate3MPAmp;
			wData2 = data2.m_stFlawData.wGate3MPAmp;
		}
		else
		{
		}
	}
	else if (nGateIndex == 3)	// C闸门
	{
		if (nFeatureIndex == 0)	// 一次波过阈值时间
		{
			wData1 = data1.m_stFlawData.wGate4FPFirstTime;
			wData2 = data2.m_stFlawData.wGate4FPFirstTime;
		} 
		else if (nFeatureIndex == 1) // 一次波峰时间
		{
			wData1 = data1.m_stFlawData.wGate4FPTime;
			wData2 = data2.m_stFlawData.wGate4FPTime;
		}
		else if (nFeatureIndex == 3) // 最大波峰时间
		{
			wData1 = data1.m_stFlawData.wGate4MPTime;
			wData2 = data2.m_stFlawData.wGate4MPTime;
		}
		else if (nFeatureIndex == 4) // 一次波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate4FPAmp;
			wData2 = data2.m_stFlawData.wGate4FPAmp;
		}
		else if (nFeatureIndex == 5) // 最大波峰幅值
		{
			wData1 = data1.m_stFlawData.wGate4MPAmp;
			wData2 = data2.m_stFlawData.wGate4MPAmp;
		}
		else
		{
		}
	}
	else // I闸门
	{
	}

	if (nValueMode == 0)	// 最大值
	{
		if (wData1 < wData2)
			data3 = data2;
		else
			data3 = data1;
	}
	else if (nValueMode == 1)	// 最小值
	{
		if (wData1 < wData2)
			data3 = data1;
		else
			data3 = data2;
	}
	else if (nValueMode == 2)	// 平均值
	{
	}
}
// Song Chenguang add end on 2013-01-15

void CMechanicalControlPanel::AddScanData(CH_FLAWDATA_CSCAN_S dataArray[], const int& nDataSize, const int& nCardNo, const int& nChannelNo)
{
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AddScanData"),"Enter");
	int nRealChannelNo = nCardNo*2+ nChannelNo;
	if (m_bPathExecute && m_SysParamDlg.m_pbChannelOpen[nRealChannelNo])
	{		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		// Song Chenguang add start on 2013-08-21
		if (pFrame->m_bDuplicateDrawOnce)
		{
			pFrame->m_dwDuplicateDrawCount++;
			if(pFrame->m_dwDuplicateDrawCount > 2)
				return;
		}
		// Song Chenguang add end on 2013-08-21
		int nGateNum = pFrame->m_pFeatureWnd->m_nGateOpenNum;
		int nFeatureNum = pFrame->m_pFeatureWnd->m_nFeatureOpenNum+1;


		CH_FLAWDATA_CSCAN_S * pData = dataArray;
		CUltrasonicImmersedTestDoc* pDoc = GetDocument();

		g_pFeatrueInfo = (CH_FLAWDATA_CSCAN_S*)pData;

		float fXConvertValue = 1 / m_pCScanView->m_fGridX;
		float fYConvertValue = 1 / m_pCScanView->m_fGridY;
		COLORREF colorRGB1 = 0;			// 前一个点RGB
		COLORREF colorRGB2 = 0;			// 前两个点RGB	

		int nStartPoint = static_cast<int>(pDoc->m_lstFlawData[nRealChannelNo].GetCount());	// 链表起始位置
		int nNum = 0;		// 链表结束位置

		for (int j=0; j<nDataSize; j++)
		{
/*			// Song Chenguang add start on 2013-09-13
			// 重复扫两次只画一次过滤
			if (!pFrame->m_bDuplicateDrawOnce)
			{
				pData++;
				continue;
			}
			// Song Chenguang add end on 2013-09-13
*/
			CH_FLAWDATA_CSCAN_S data;
			memcpy(&data, pData, sizeof(CH_FLAWDATA_CSCAN_S));

			// wangbingfu add start at 2012-11-20
			BOOL bTcgOnFlag = pFrame->m_pTcgCurveDlg->m_stTcgCurve[g_dwActiveChannel].bSwitchFlag;
			if(bTcgOnFlag)
			{
				if(pFrame->m_pTcgCurveDlg->m_nTcgOnMode == 0)
				{
					float fRange = 0;
					if(pData->wGate1MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate1MPTime);
						data.wGate1MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate1MPAmp, fRange);
					}
					if(pData->wGate2MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate2MPTime);
						data.wGate2MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate2MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate2FPTime);
						data.wGate2FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate2FPAmp, fRange);
					}
					if(pData->wGate3MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate3MPTime);
						data.wGate3MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate3MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate3FPTime);
						data.wGate3FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate3FPAmp, fRange);
					}
					if(pData->wGate4MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4MPTime);
						data.wGate4MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate4MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, data.wGate4FPTime);
						data.wGate4FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(data.wGate4FPAmp, fRange);
					}
				}
			}
			// wangbingfu add end at 2012-11-20

			CFeatureInfo feature = CFeatureInfo(nGateNum, nFeatureNum);
			CFeatureInfo featureTail = CFeatureInfo(nGateNum, nFeatureNum);
			BOOL bRet = PrimitiveDataChange(data, feature, nCardNo, nChannelNo);

			// I闸门首次波过阈值时间若为0，数据丢掉
			if(!bRet)
			{
				pData ++; 
				continue;
			}

			int nXStart = 0;
			int nYStart = 0;
			float fXStep = m_pCScanView->m_fGridX;//1.0f;
			float fYStep = m_pCScanView->m_fGridY;//1.0f;						
			int x;
			int y;

			//网格化
			GridConvert(feature, fXStep, fYStep);
			int nX = (int)((feature.m_dX -  pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrX) * fXConvertValue);
			int nY = (int)((feature.m_dY -  pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrY) * fYConvertValue);

			// WangQianfeng delete start on 2013-06-28
			// 向B扫描视图发数据
			if (nRealChannelNo == g_dwActiveChannel)
			{
				CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
				CFeatureInfo * pData2 = new CFeatureInfo(nGateNum, nFeatureNum);
				// WangQianfeng add start on 2012-11-06
				if ( NULL == pData2 )
				{
					MessageBox(_T("CMechanicalControlPanel::AddScanData,pData2 = null"), _T("警告"), MB_OK|MB_ICONERROR);
					//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AddScanData"),"Leave");
					return;
				}
				// WangQianfeng add end on 2012-11-06
				*pData2 = feature;
				pBScanView->PostMessage(WM_BSCAN_DRAW,(WPARAM)pData2,0);
			}
			// WangQianfeng delete end on 2013-06-28

			if(pDoc->m_lstFlawData[nRealChannelNo].IsEmpty() == TRUE)
			{
				feature.m_nCount++;
				pDoc->m_lstFlawData[nRealChannelNo].AddTail(feature);
				// wangbingfu modify start at 2012-10-12
				pDoc->m_qCScanData[nRealChannelNo].push(feature);
//				pDoc->m_lstFlawDataTemp[nRealChannelNo].AddTail(feature);
				// wangbingfu modify end at 2012-10-12
				nNum++;
			}
			else
			{
				featureTail = pDoc->m_lstFlawData[nRealChannelNo].GetTail();
				// Song Chenguang modify start on 2013-08-15
				double dXTail = featureTail.m_dX;
				double dX = feature.m_dX;
				
				double dYTail = featureTail.m_dY;
				double dY = feature.m_dY;
				if(m_pCScanView->m_nAxisSelect == 3
					|| m_pCScanView->m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
				{
					dYTail = featureTail.m_dR;
					dY = feature.m_dR;
				}
				//? 是否加W平面处理 2013-05-04 Song Chenguang
				else if (m_pCScanView->m_nAxisSelect == 5)
				{
					dXTail = featureTail.m_dW;
					dX = feature.m_dW;
					dYTail = featureTail.m_dZ;
					dY = feature.m_dZ;
				}

				//////////////////////////////////////////////////////////////////////////
				// test
				double dcha = featureTail.m_dW - m_fWCurPos;
				if (dcha > 30.0 && m_pCScanView->m_nAxisSelect == 5)
				{
					CString str;
					str.Format(_T("cha = %.2f"), dcha);
					pFrame->m_wndStatusBar.SetPaneText(3, str);
				}
				//////////////////////////////////////////////////////////////////////////
//				if((fabs(dYTail - dY) < 0.000001) && (fabs(featureTail.m_dX - feature.m_dX) < 0.000001))
//				if((fabs(dYTail - dY) < 0.01) && (fabs(featureTail.m_dX - feature.m_dX) < 0.01))
				if((fabs(dX - dXTail) < 0.01) && (fabs(dYTail - dY) < 0.01) )
				{
					x = static_cast<int>((dX - nXStart) / fXStep);
					y = static_cast<int>((dY - nYStart) / fYStep);
					BOOL bFeatureFlag[7];
					bFeatureFlag[0] = TRUE;
					bFeatureFlag[1] = pFrame->m_pFeatureWnd->m_bFPFTimeDisplay;
					bFeatureFlag[2] = pFrame->m_pFeatureWnd->m_bFPMTimeDisplay;
					bFeatureFlag[3] = pFrame->m_pFeatureWnd->m_bMPFTimeDisplay;
					bFeatureFlag[4] = pFrame->m_pFeatureWnd->m_bMPMTimeDisplay;
					bFeatureFlag[5] = pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay;
					bFeatureFlag[6] = pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay;

					int n = 1;
					int k = 1;
					for(int m = 0; m < nGateNum; m++)
					{
						FeatureReCalculate(featureTail, feature, bFeatureFlag, m, n, k, x, y, 0);
					}
					POSITION posTail = pDoc->m_lstFlawData[nRealChannelNo].GetTailPosition();
					featureTail.m_nCount++;
					pDoc->m_lstFlawData[nRealChannelNo].SetAt(posTail, featureTail);
/*					// wangbingfu add start at 2012-10-12
					if(pDoc->m_lstFlawDataTemp[nRealChannelNo].IsEmpty())
					{
						pDoc->m_lstFlawDataTemp[nRealChannelNo].AddTail(featureTail);
					}
					else
					{
						POSITION posTailTemp = pDoc->m_lstFlawDataTemp[nRealChannelNo].GetTailPosition();
						pDoc->m_lstFlawDataTemp[nRealChannelNo].SetAt(posTailTemp, featureTail);
					}				
					// wangbingfu add end at 2012-10-12
*/
					// Song Chenguang add start on 2013-09-11
					if(pDoc->m_qCScanData[nRealChannelNo].empty())
					{
						pDoc->m_qCScanData[nRealChannelNo].push(featureTail);
					}
					else
					{
						CFeatureInfo& featureTemp = pDoc->m_qCScanData[nRealChannelNo].back();
						featureTemp = featureTail;
					}
					// Song Chenguang add end on 2013-09-11
				}
				else
				{
					feature.m_nCount++;
					pDoc->m_lstFlawData[nRealChannelNo].AddTail(feature);
					// wangbingfu modify start at 2012-10-12
					pDoc->m_qCScanData[nRealChannelNo].push(feature);
//					pDoc->m_lstFlawDataTemp[nRealChannelNo].AddTail(feature);
					// wangbingfu modify end at 2012-10-12
					nNum++;
				}
				// Song Chenguang modify end on 2013-08-15
			}

			pData ++;
		}


		if(g_bDrawRealTime)
		{
			CARD_INFO_t* pCardInfo1 = new CARD_INFO_t;
			// WangQianfeng add start on 2012-11-06
			if ( NULL == pCardInfo1 )
			{
				MessageBox(_T("CMechanicalControlPanel::AddScanData,pCardInfo1 = null"), _T("警告"), MB_OK|MB_ICONERROR);
				//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AddScanData"),"Leave");
				return;
			}
			// WangQianfeng add end on 2012-11-06
			pCardInfo1->nCardIndex = nCardNo;
			pCardInfo1->nChannelIndex = nChannelNo;
			pCardInfo1->nStartPoint = nStartPoint;
			pCardInfo1->nNum = nNum;

			m_pCScanView->SendMessage(WM_CSCAN_DRAW, (WPARAM)(g_pFeatrueInfo), LPARAM(pCardInfo1));

			CARD_INFO_t* pCardInfo2 = new CARD_INFO_t;
			// WangQianfeng add start on 2012-11-06
			if ( NULL == pCardInfo2 )
			{
				MessageBox(_T("CMechanicalControlPanel::AddScanData,pCardInfo2 = null"), _T("警告"), MB_OK|MB_ICONERROR);
				//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AddScanData"),"Leave");
				return;
			}
			// WangQianfeng add end on 2012-11-06
			pCardInfo2->nCardIndex = nCardNo;
			pCardInfo2->nChannelIndex = nChannelNo;
			pCardInfo2->nStartPoint = nStartPoint;
			pCardInfo2->nNum = nNum;

			m_pCScanView2->SendMessage(WM_CSCAN_DRAW, (WPARAM)(g_pFeatrueInfo), LPARAM(pCardInfo2));
		}

	}
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AddScanData"),"Leave");
}

void CMechanicalControlPanel::PathExeute()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathExeute"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	short status = m_pMovePathView->m_GraphMove.GetWorkStatus();
	
	if (m_pMovePathView->m_GraphMove.PathStart(&m_lGrapNum))
	{
		KillTimer(MECHANICAL_TIMER);
		SetTimer(MECHANICAL_TIMER,10,NULL);
		
		if (status == 4 || status == 2)	// 之前为到采集点或暂停状态
		{
			for (int i = 0;i<(sizeof(m_ChannelInfoID) / sizeof(int));++i)
			{
				pFrame->m_wndChannelInfo.m_pFormView->GetDlgItem(m_ChannelInfoID[i])->EnableWindow(FALSE);

			}
						
			for (int i = 0;i<(sizeof(m_CScanInfoID) / sizeof(int));++i)
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView->GetDlgItem(m_CScanInfoID[i])->EnableWindow(FALSE);
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->GetDlgItem(m_CScanInfoID[i])->EnableWindow(FALSE);
			}

			for (int i = 0;i<(sizeof(m_MechanicalInfoID) / sizeof(int));++i) 
			{
				pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(m_MechanicalInfoID[i])->EnableWindow(FALSE);
			}

			m_btnRun.SetWindowText(_T("暂停")); 

			if (status == 4)
			{		
				// 创建此次扫查目录
				// 添加工件ID目录   yachang.huang  modify start on 2012-11-21
				CProbeSet probeset1;;
				CLSID clsid;
				if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
				{
					probeset1.CreateDispatch(clsid,NULL);
				}
				CString strMaterialID;
				BSTR b = strMaterialID.AllocSysString();
				probeset1.GetMaterialID(&b);
				strMaterialID = b;
				SysFreeString(b);

				CTime time = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format(_T("%d-%d-%d"), time.GetYear(), time.GetMonth(), time.GetDay());
				m_strDataFolder = pFrame->m_strDataFolder + strTime;
				BOOL bFind = TRUE;
				bFind =PathFileExists(m_strDataFolder);
				if (!bFind)
				{
					CString strFolder = m_strDataFolder;
					if (strFolder.Right(1) == _T("\\"))
					{
						strFolder = strFolder.Left(strFolder.GetLength() - 1);
					}
					if(!CreateDirectory(strFolder, NULL)) 
					{ 
						AfxMessageBox(_T("不能创建目录：") + m_strDataFolder); 
						//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathExeute"),"Leave");
						return; 
					}
				}

				m_strDataFolder = pFrame->m_strDataFolder + strTime +_T("\\")+ strMaterialID;
                bFind =PathFileExists(m_strDataFolder);
				if (!bFind)
				{
					CString strFolder = m_strDataFolder;
					if (strFolder.Right(1) == _T("\\"))
					{
						strFolder = strFolder.Left(strFolder.GetLength() - 1);
					}
					if(!CreateDirectory(strFolder, NULL)) 
					{ 
						AfxMessageBox(_T("不能创建目录：") + m_strDataFolder); 
						//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathExeute"),"Leave");
						return; 
					}
				}
                //yachang.huang  modify start on 2012-11-21
//				SetCurrentDirectory(strFolder);
				
			}

			CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();

			// rem by Song Chenguang on 2013-07-18 
			// OnArriveStartPoint()已经清过一次了
// 			for (int i = 0; i<CHANNEL_NUM; i++)
// 			{
// 				pDoc->m_lstFlawData[i].RemoveAll();
// 				pDoc->m_lstFlawDataOriginal[i].RemoveAll();	// 2011-11-18
// 			}

			m_bPathExecute = TRUE;
			m_fOldGateBBack = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[0].m_GateInfo[2].m_fBackGate; // wangbingfu add at 2012-12-5
			g_bDrawRealTime = TRUE;	// 开始实时C扫	
			
		}
		else
		{
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathExeute"),"fei status 2 and 4");
//			g_bDrawRealTime = FALSE;	// 停止实时C扫
			int nChannelCount = sizeof(m_ChannelInfoID) / sizeof(int);
			int nCScanCount = sizeof(m_CScanInfoID) / sizeof(int);

			for (int i = 0;i<nChannelCount;++i)
			{
				pFrame->m_wndChannelInfo.m_pFormView->GetDlgItem(m_ChannelInfoID[i])->EnableWindow(TRUE);
			}

			for (int i = 0;i<nCScanCount;++i)
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView->GetDlgItem(m_CScanInfoID[i])->EnableWindow(TRUE);
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->GetDlgItem(m_CScanInfoID[i])->EnableWindow(TRUE);
			}
			for (int i = 0;i<(sizeof(m_MechanicalInfoID) / sizeof(int));++i)
			{
				pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(m_MechanicalInfoID[i])->EnableWindow(TRUE);
			}
			if(m_SysParamDlg.m_nAxisIndex == 3
				|| m_SysParamDlg.m_nAxisIndex == 4) // wangbingfu add on 2013-01-09
			{
				;//
			}
			else
			{
				pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_POSITIVE)->EnableWindow(FALSE);
				pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_REVERSE)->EnableWindow(FALSE);
			}

			pFrame->m_wndChannelInfo.m_pFormView->ALLSCanItemEnableJudge();

			m_btnRun.SetWindowText(_T("运行(F7)"));	
		}
	}
	else
	{		
	}

	m_nMPMTimeIndex = 1;
	m_nMPMPeakIndex = 1;
	//计算门内最大波幅时间
	if (pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_FP_F_TIME))
	{
		++m_nMPMTimeIndex;
		++m_nMPMPeakIndex;
	}
	if (pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_FP_M_TIME))
	{
		++m_nMPMTimeIndex;
		++m_nMPMPeakIndex;
	}
	if (pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_MP_F_TIME))
	{
		++m_nMPMTimeIndex;
		++m_nMPMPeakIndex;
	}
	if (pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_MP_M_TIME))
	{
		++m_nMPMPeakIndex;
	}
	if (pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_FP_M_ALTITUDE))
	{
		++m_nMPMPeakIndex;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathExeute"),"Leave");

}

void CMechanicalControlPanel::OnDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnDestroy"),"Enter");
	CFormView::OnDestroy();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnDestroy"),"Leave");

}

#ifdef _DEBUG
CUltrasonicImmersedTestDoc* CMechanicalControlPanel::GetDocument() const // 非调试版本是内联的
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::GetDocument"),"Enter");
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasonicImmersedTestDoc)));
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::GetDocument"),"Leave");
	return (CUltrasonicImmersedTestDoc*)m_pDocument;
}
#endif

// 保存探伤数据
BOOL CMechanicalControlPanel::SaveTestResult()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveTestResult"),"Enter");
	TCHAR szFilters[] =
		_T ("NDT Test Result files (*.uit)|*.uit||");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString stroMaterial=pFrame->m_ProbeSet.getVal(_T("oMaterial"));
	CString stroId=pFrame->m_ProbeSet.getVal(_T("oId"));
	CString stroType=pFrame->m_ProbeSet.getVal(_T("oType"));

	CString timeStr;
	CTime day;
	day = CTime::GetCurrentTime();
	int filenameday, filenamemonth, filenameyear, filehour, filemin, filesec;
	filenameday = day.GetDay();		// dd
	filenamemonth = day.GetMonth();	// mm月份
	filenameyear = day.GetYear();	// yyyy
	filehour = day.GetHour();		// hh
	filemin = day.GetMinute();		// mm分钟
	filesec = day.GetSecond();		// ss
	timeStr.Format(_T("%s_%s_%s_%04d%02d%02d%02d%02d%02d"),stroMaterial,stroId,stroType,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);

	CFileDialog dlg(FALSE, _T("uit"), timeStr, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);
	dlg.m_ofn.lpstrInitialDir = m_strDataFolder;

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		CString strFileName = dlg.GetFileName();
		CString strNotePath = _T("");
		strNotePath = strPath.Mid(0, strPath.GetLength() - 4);
		strNotePath = strNotePath + L".note";
		m_pCScanView->m_strNotePath = strNotePath;
		m_pCScanView2->m_strNotePath = strNotePath;
	//	Note_SetFilePath(strPath);
		SaveDataFile(strPath, strFileName);

		SaveFilePath(strPath);	// machao add 2013-11-18
	}
	else
	{
		CUltrasonicImmersedTestDoc* pDoc = GetDocument();
		pDoc->m_FilePath="";
		pDoc->m_FilePath1=""; // wangbingfu add on 2014-09-10
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveTestResult"),"Leave");
		return FALSE;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveTestResult"),"Leave");
	return TRUE;
}

BOOL CMechanicalControlPanel::SaveDataFile( const CString& strPath, const CString& strFileName )
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveDataFile"),"Enter");
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	CString strOtherPath;
	strOtherPath = strPath.Mid(0, strPath.GetLength() - 4);
	pDoc->m_FilePath=strOtherPath+L".aaf";
	pDoc->m_FilePath1=strOtherPath+L"1.aaf"; // waningfu add on 2014-09-10
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	//保存仪器参数
	pChannelPanel->SaveParam(strOtherPath+L".asf");
	
	//保存扫描路径
	BOOL bRet2 = m_pMovePathView->m_GraphMove.SaveFileByPath(strOtherPath+L".apf");
	if(!bRet2)
		return FALSE;

	//保存A扫描结果
	if (pFrame->m_bSaveAScan)
	{
		CopyFile(m_strTempPath+L"aaf",strOtherPath+L".aaf",true);
		CopyFile(m_strTempPath1+L"aaf",strOtherPath+L"1.aaf",true);	// wangbingfu add on 2014-09-10
		CopyFile(m_strTempPath+L"paaf",strOtherPath+L".paaf",true);		// Song Chenguang add on 2014-01-15
	}

	//保存C扫描结果
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveDataFile"),"Leave");

	// wangbingfu add start on 2013-01-09
	if(m_pCScanView->m_nAxisSelect == 4)
	{
		CString csPipeSetPath;
		csPipeSetPath = strPath.Mid(0, strPath.GetLength() - strFileName.GetLength());
		csPipeSetPath = csPipeSetPath + _T("PipeSet.ini");
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->WritePipeIni(csPipeSetPath);
	}

	// Song Chenguang modify start on 2013-06-17
	BOOL bRet = CopyFile(m_strTempPath+L"acf",strPath,true);
	if (!bRet)
		return FALSE;

	// 正常扫完完毕则删除此文件
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString strTempPath = CString(strFolder) + _T("扫描数据临时文件位置.txt");
	DeleteFile(strTempPath);

	return TRUE;
	// Song Chenguang modify end on 2013-06-17
}

void CMechanicalControlPanel::OnBnClickedSavePathFile()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedSavePathFile"),"Enter");
	if (m_strFilePath != _T(""))
	{
		m_pMovePathView->m_GraphMove.SaveFileByPath(m_strFilePath);
	}
	else
		m_pMovePathView->m_GraphMove.SaveFile();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedSavePathFile"),"Leave");

}

void CMechanicalControlPanel::OnBnClickedOpenPathFile()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedOpenPathFile"),"Enter");
	CString strFileName;
	CFileDialog fdlg(TRUE,_T("mdsx"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("路径文件(*.mdsx)|*.mdsx|AutoCAD文件(*.dxf)|*.dxf|G代码文件(*.nc)|*.nc|所有文件(*.*)|*.*||"),NULL);
	fdlg.m_ofn.lpstrTitle=_T("打开文件");
	if(fdlg.DoModal()==IDOK)
	{
		m_strFilePath = fdlg.GetPathName();
		strFileName = fdlg.GetFileName();
	}
	else
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedOpenPathFile"),"Leave");
		return;

	m_pMovePathView->m_GraphMove.OpenFileByPath(m_strFilePath);
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	pDoc->SetTitle(strFileName);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedOpenPathFile"),"Leave");
}


void CMechanicalControlPanel::OnBnClickedFileSaveAs()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedFileSaveAs"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString fstr;
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();

	fstr=pDoc->GetTitle();
	int m=fstr.Find ('.');
	if(m>-1) fstr=fstr.Mid(0,m);//去掉扩展名
	CFileDialog fdlg(FALSE,_T("mdsx"),fstr,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("路径文件(*.mdsx)|*.mdsx||"),NULL);
	fdlg.m_ofn.lpstrTitle=_T("保存文件");
	if(fdlg.DoModal()==IDOK)
	{  
		fstr=fdlg.GetPathName();
		CString strFileName = fdlg.GetFileName();
		m_pMovePathView->m_GraphMove.SaveFileByPath(fstr);
		pDoc->SetTitle(strFileName);
	}  
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedFileSaveAs"),"Leave");
}

// 分配网格各点计数内存
void CMechanicalControlPanel::AllocateGridNumMemory()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AllocateGridNumMemory"),"Enter");
	m_pGridCount = new int*[GRID_X_NUM];
	// WangQianfeng add start on 2012-11-06
	if ( NULL == m_pGridCount )
	{
		MessageBox(_T("CMechanicalControlPanel::AllocateGridNumMemory(),m_pGridCount = null"), _T("警告"), MB_OK|MB_ICONERROR);
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AllocateGridNumMemory"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	for(int i = 0; i < GRID_X_NUM; i++)
	{
		m_pGridCount[i] = new int[GRID_Y_NUM];
		// WangQianfeng add start on 2012-11-06
		if ( NULL == m_pGridCount )
		{
			MessageBox(_T("CMechanicalControlPanel::AllocateGridNumMemory(),m_pGridCount[i] = null"), _T("警告"), MB_OK|MB_ICONERROR);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::AllocateGridNumMemory"),"Leave");
			return;
		}
		// WangQianfeng add end on 2012-11-06
		memset(m_pGridCount[i], 0, GRID_Y_NUM*sizeof(int));
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::AllocateGridNumMemory"),"Leave");
}

// 释放网格各点计数内存
void CMechanicalControlPanel::FreeGridNumMemory()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::FreeGridNumMemory"),"Enter");
	for(int i = 0; i < GRID_X_NUM; i++)
		delete[] m_pGridCount[i]; 
	delete[] m_pGridCount;
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::FreeGridNumMemory"),"Leave");
}

void CMechanicalControlPanel::GridConvert(CFeatureInfo& Fdata, float fXStep, float fYStep)
{
	// 旋转
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::GridConvert"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	//if(m_SysParamDlg.m_nAxisIndex == 3)
	// Song Chenguang modify start on 2013-07-16
	if(nAxisIndex == 3 || nAxisIndex == 4) 
	{
//		Fdata.m_dR = ((int)(m_fFirstWdrX / fXStep)) * fXStep + ((int)((Fdata.m_dR - m_fFirstWdrX) / fXStep + 0.5))  * fXStep;
		Fdata.m_dR = ((int)(Fdata.m_dR / fYStep + 0.5))  * fYStep;
	}
	else if(nAxisIndex == 5)
	{
		Fdata.m_dW = ((int)(Fdata.m_dW / fXStep + 0.5))  * fXStep;
		Fdata.m_dZ = ((int)(Fdata.m_dZ / fYStep + 0.5))  * fYStep;	// Song Chenguang add on 2013-08-21
	}
	
	Fdata.m_dX = ((int)(m_fFirstWdrX / fXStep)) * fXStep + ((int)((Fdata.m_dX - m_fFirstWdrX) / fXStep + 0.5))  * fXStep;
	// Song Chenguang modify end on 2013-07-16
	Fdata.m_dY = ((int)(m_fFirstWdrY / fYStep)) * fYStep + ((int)((Fdata.m_dY - m_fFirstWdrY) / fYStep + 0.5))  * fYStep;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::GridConvert"),"Leave");

}

void CMechanicalControlPanel::FeatureReCalculate(CFeatureInfo& featureTail, const CFeatureInfo& feature,
												 BOOL bFeatureFlag[], int i, int j, int k, int x, int y, int nValueMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::FeatureReCalculate"),"Enter");
	if(bFeatureFlag[k])
	{
		if(k == 5 || k == 6)
		{
			if(nValueMode == 0)		// 最大值
			{	
				if(featureTail.m_pfFeatures[i][j] < feature.m_pfFeatures[i][j])	//取最大值点
				{
					featureTail.m_pfFeatures[i][j] = feature.m_pfFeatures[i][j];
				}
			}
			else if(nValueMode == 1)	// 最小值
			{
				if(featureTail.m_pfFeatures[i][j] > feature.m_pfFeatures[i][j])	//取最小值点
				{
					featureTail.m_pfFeatures[i][j] = feature.m_pfFeatures[i][j];
				}
			}
			else if(nValueMode == 2)	//平均值
			{
				featureTail.m_pfFeatures[i][j] = (featureTail.m_pfFeatures[i][j] *(featureTail.m_nCount - 1) + feature.m_pfFeatures[i][j]) / featureTail.m_nCount;
			}
			else
			{
				;
			}

			if(k == 6)	//最后一项特征值
			{
	            //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::FeatureReCalculate"),"Leave");
				return;
			}
		}
		else	//时间取平值
		{
			featureTail.m_pfFeatures[i][j] = (featureTail.m_pfFeatures[i][j] *(featureTail.m_nCount - 1) + feature.m_pfFeatures[i][j]) / featureTail.m_nCount;
		}
		j++;
		k++;
		FeatureReCalculate(featureTail, feature, bFeatureFlag, i, j, k, x, y, nValueMode);
	}
	else
	{	
		k++;
		FeatureReCalculate(featureTail, feature, bFeatureFlag, i, j, k, x, y, nValueMode);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::FeatureReCalculate"),"Leave");

}

// 保存数据到数据库
void CMechanicalControlPanel::SaveResultToDB()
{
	// 连接数据库
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveResultToDB"),"Enter");
	_ConnectionPtr	pConnection = NULL;
	HRESULT hr = pConnection.CreateInstance(__uuidof(Connection));

	if (FAILED(hr))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveResultToDB"),"Leave");
		return;
	}

	_bstr_t strCnn(g_strDBConnection);
	try                 
	{	
		// 打开本地Access库Demo.mdb
		hr = pConnection->Open(strCnn,"","",adConnectUnspecified);
	}
	catch(_com_error e)
	{
		CString strError;
		strError.Format(_T("数据库连接失败:%s"), e.ErrorMessage());
		AfxMessageBox(strError, MB_ICONSTOP);
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveResultToDB"),"Leave");
		return;
	}

	// 保存通道数据
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	for (int i=0; i<CHANNEL_NUM; i++)
	{
		CChannelInfo channelInfo = pFrame->GetChannelInfo(i);
		CString strSQL;
		strSQL.Format(_T("insert ars_pc_tdcs(channelid, kg1, qy1, kd1, yz1, tbfs1, tbqz1,\
						 ys1, kg2, qy2, kd2, yz2, tbfs2, tbqz2, ys2, kg3, qy3, kd3, yz3, \
						 tbfs3, tbqz3, ys3, kg4, qy4, kd4, yz4, tbfs4, tbqz4, ys4, ld, \
						 yc, fw, zy, cyjg, pjcys, ttpl, jfdy, jbms, zk, lb) value('%d', \
						 '%d','%.1f','%.1f','%.1f', '%d', '%.1f', '%d',\
						 '%d','%.1f','%.1f','%.1f', '%d', '%.1f', '%d',\
						 '%d','%.1f','%.1f','%.1f', '%d', '%.1f', '%d',\
						 '%d','%.1f','%.1f','%.1f', '%d', '%.1f', '%d',\
						 '%.1f','%.1f','%.1f','%.1f','%d','%d','%d','%d','%d',\
						 ,'%d','%d','%d','%d','%d','%d','%d','%d')"), i, 
						 channelInfo.m_GateInfo[0].m_bOpen, channelInfo.m_GateInfo[0].m_fFrontGate, channelInfo.m_GateInfo[0].m_fBackGate,
						 channelInfo.m_GateInfo[0].m_fPeakGate, channelInfo.m_GateInfo[0].m_nMtpMode, channelInfo.m_GateInfo[0].m_fMTPPeak,
						 channelInfo.m_GateInfo[0].m_colorGate, 
						 channelInfo.m_GateInfo[1].m_bOpen, channelInfo.m_GateInfo[1].m_fFrontGate, channelInfo.m_GateInfo[1].m_fBackGate,
						 channelInfo.m_GateInfo[1].m_fPeakGate, channelInfo.m_GateInfo[1].m_nMtpMode, channelInfo.m_GateInfo[1].m_fMTPPeak,
						 channelInfo.m_GateInfo[1].m_colorGate,
						 channelInfo.m_GateInfo[2].m_bOpen, channelInfo.m_GateInfo[2].m_fFrontGate, channelInfo.m_GateInfo[2].m_fBackGate,
						 channelInfo.m_GateInfo[2].m_fPeakGate, channelInfo.m_GateInfo[2].m_nMtpMode, channelInfo.m_GateInfo[2].m_fMTPPeak,
						 channelInfo.m_GateInfo[2].m_colorGate,
						 channelInfo.m_GateInfo[3].m_bOpen, channelInfo.m_GateInfo[3].m_fFrontGate, channelInfo.m_GateInfo[3].m_fBackGate,
						 channelInfo.m_GateInfo[3].m_fPeakGate, channelInfo.m_GateInfo[3].m_nMtpMode, channelInfo.m_GateInfo[3].m_fMTPPeak,
						 channelInfo.m_GateInfo[3].m_colorGate,
						 channelInfo.m_fZeroPoint, channelInfo.m_fChannelDelay, channelInfo.m_fChannelRange, channelInfo.m_fChannelPlus,
						 channelInfo.m_nChSI, channelInfo.m_nChSavg, channelInfo.m_ProbeInfo.m_eProbeFreq, channelInfo.m_ProbeInfo.m_eInterruptVoltage,
						 channelInfo.m_ProbeInfo.m_eDisplayMode, channelInfo.m_ProbeInfo.m_eImpedance, channelInfo.m_ProbeInfo.m_eLvBoType);

		_CommandPtr		m_pCommand;
		m_pCommand.CreateInstance(__uuidof(Command));
		m_pCommand->ActiveConnection = pConnection;  // 将库连接赋于它
		m_pCommand->CommandText = _bstr_t(strSQL);  // SQL语句
		
		m_pCommand->Execute(NULL, NULL,adCmdText); // 执行SQL语句，返回记录集
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SaveResultToDB"),"Leave");

}

void CMechanicalControlPanel::OnBnChickedGetRect()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnChickedGetRect"),"Enter");
	double x = 0;
	double y = 0;
	double z = 0;

	int nIndex = m_lstRectPoints.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

	if (nIndex == -1)
	{
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnChickedGetRect"),"Leave");
		return;
	}

	if (nIndex == 0)
	{
		m_pMovePathView->m_GraphMove.AutoAddRect(1, &x, &y, &z);	
	}
	else if (nIndex == 1)
	{
		m_pMovePathView->m_GraphMove.AutoAddRect(2, &x, &y, &z);
	}
	
	CString strX;
	strX.Format(_T("%.2f"), x);
	CString strY;
	strY.Format(_T("%.2f"), y);
	CString strZ;
	strZ.Format(_T("%.2f"), z);

	int iSubItem = 0;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	BOOL bRet = FALSE;
	if (nAxisIndex == 0)
	{
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strX);	// X
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strY);	// Y
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strZ);	// Z
	}
	else if (nAxisIndex == 1)
	{
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strX);	// X		
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strZ);	// Z
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strY);	// Y
	}
	else if (nAxisIndex == 2)
	{		
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strY);	// Y
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strZ);	// Z
		bRet = m_lstRectPoints.SetItemText(nIndex, ++iSubItem, strX);	// X
	}
	else
	{
	}
	
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnChickedGetRect"),"Leave");
}

// 单卡两个通道
void CMechanicalControlPanel::SetChannelOpenFlag(BOOL bOpen[], const int& nCardNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SetChannelOpenFlag"),"Enter");
	int nProbeNum = m_SysParamDlg.m_nIndexProbeNum + 1;

	if (nProbeNum == 1)
	{
		int nProbeIndex = m_SysParamDlg.m_nIndexOneProbeType;
		int nIndex1 = nProbeIndex / 2;
		int nIndex2 = nProbeIndex % 2;
		if (nIndex1 == nCardNo)
		{
			if (nIndex2 == 0)
			{
				bOpen[0] = TRUE;
				bOpen[1] = FALSE;
			}
			else
			{
				bOpen[0] = FALSE;
				bOpen[1] = TRUE;
			}
		}
		else
		{
			bOpen[0] = FALSE;
			bOpen[1] = FALSE;
		}
	}
	else if (nProbeNum == 2)	// {_T("1, 2"), _T("2, 3"), _T("3, 4"), _T("1, 3"), _T("2, 4"), _T("1, 4")}
	{
		int nIndex = m_SysParamDlg.m_nIndexTwoProbeType;

		if (nIndex == 0 && nCardNo == 0)
		{
			bOpen[0] = TRUE;
			bOpen[1] = TRUE;
		}
		else if (nIndex == 1)
		{
			if (nCardNo == 0)
			{
				bOpen[0] = FALSE;
				bOpen[1] = TRUE;
			} 
			else
			{
				bOpen[0] = TRUE;
				bOpen[1] = FALSE;
			}
		}
		else if (nIndex == 2 && nCardNo == 1)
		{
			bOpen[0] = TRUE;
			bOpen[1] = TRUE;
		}
		else if (nIndex == 3)
		{
			bOpen[0] = TRUE;
			bOpen[1] = FALSE;
		}
		else if (nIndex == 4)
		{
			bOpen[0] = FALSE;
			bOpen[1] = TRUE;
		}
		else if (nIndex == 5)
		{
			if (nCardNo == 0)
			{
				bOpen[0] = TRUE;
				bOpen[1] = FALSE;
			} 
			else
			{
				bOpen[0] = FALSE;
				bOpen[1] = TRUE;
			}			
		}
		else
		{
			bOpen[0] = FALSE;
			bOpen[1] = FALSE;
		}
	}
	else if (nProbeNum == 3)
	{
		int nIndex = m_SysParamDlg.m_nIndexThreeProbeType;
		if (m_SysParamDlg.m_nScanType == 0)
		{
			if (nIndex == 0)  // _T("1,2,3")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				} 
				else
				{
					bOpen[0] = TRUE;
					bOpen[1] = FALSE;
				}
			} 
			else	 // _T("2,3,4")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = FALSE;
					bOpen[1] = TRUE;
				} 
				else
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				}
			}
			
		} 
		else
		{
			if (nIndex == 0)	// _T("1, 2, 3")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				} 
				else
				{
					bOpen[0] = TRUE;
					bOpen[1] = FALSE;
				}
			} 
			else if (nIndex == 1)	// _T("1, 2, 4")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				} 
				else
				{
					bOpen[0] = FALSE;
					bOpen[1] = TRUE;
				}
			}
			else if (nIndex == 2)	// _T("1, 3, 4")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = TRUE;
					bOpen[1] = FALSE;
				} 
				else
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				}
			}
			else if (nIndex == 3)	// _T("2, 3, 4")
			{
				if (nCardNo == 0)
				{
					bOpen[0] = FALSE;
					bOpen[1] = TRUE;
				} 
				else
				{
					bOpen[0] = TRUE;
					bOpen[1] = TRUE;
				}
			}
			else
			{
			}
		}
	}
	else if (nProbeNum == 4)
	{
		bOpen[0] = TRUE;
		bOpen[1] = TRUE;
	}
	else
	{
		bOpen[0] = FALSE;
		bOpen[1] = FALSE;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SetChannelOpenFlag"),"Leave");
}

void CMechanicalControlPanel::OnBnClickedButtonDisplayRect()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDisplayRect"),"Enter");
	if (m_lstRectPoints.GetItemCount() == 2)
	{
		double x = 0;
		double y = 0;
		double z = 0;
		m_pMovePathView->m_GraphMove.AutoAddRect(3, &x, &y, &z);
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);

	pCScanView->bFreePictureMemoy = TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonDisplayRect"),"Leave");
}
/** 
* C扫视图相关设置
* @param     type [in]: 视图窗口指针                                    
* @return    void
* @since     1.0
*/
BOOL CMechanicalControlPanel::CScanViewSet(CCScanView* pCScanView)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CScanViewSet"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCScanView == NULL)
	{
		MessageBox(_T("CMechanicalControlPanel::CScanViewSet(CCScanView* pCScanView)"), _T("警告"), MB_OK|MB_ICONERROR);
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::CScanViewSet"),"Leave");
 		return FALSE;
	}
   // yachang.huang  add end on 2012-11-02


	pCScanView->m_MemDC.SetViewportOrg(0, 0);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = NULL;
	if(pCScanView == m_pCScanView)
	{
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;	
	}
	else
	{
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;		
	}

	pCScanView->GetCscanBackColor();

	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));
	m_pMovePathView->m_GraphMove.GetPara(dParam, lLogParam);
	// 网格间距(暂时用默认值)
	if (lLogParam[0] == 0) // 横向
	{
		pCScanView->m_fGridX = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
		pCScanView->m_fGridY = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;	
	} 
	else	// 纵向
	{
		pCScanView->m_fGridX = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;
		pCScanView->m_fGridY = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
	}


	double dXStart = 0;
	double dYStart = 0;
	double dZStart = 0;
	double dABStart = 0.0f;	
	double dWStart = 0.0f;
	double dRStart = 0.0f;
	double dXEnd = 0;
	double dYEnd = 0;
	double dZEnd = 0;
	double dABEnd = 0.0f;	
	double dWEnd = 0.0f;
	double dREnd = 0.0f;
	m_pMovePathView->m_GraphMove.GraphStartPoint(&dXStart, &dYStart	, &dZStart,
		&dABStart, &dWStart, &dRStart);
	// Song Chenguang test on 2013-12-11
//	CString strWStart;
//	strWStart.Format(_T("dWStart = %.2f"), dWStart);
//	pFrame->m_wndStatusBar.SetPaneText(3, strWStart);
	/////end//////////////////////////////////////////////////////////////////
	m_pMovePathView->m_GraphMove.GraphEndPoint(&dXEnd, &dYEnd, &dZEnd,
		&dABEnd, &dWEnd, &dREnd);

	// 旋转
	int nAxisIndex = m_pMovePathView->m_GraphMove.GetAxisSelect();
	if(nAxisIndex == 3 
		|| nAxisIndex == 4) // wangbingfu add start on 2013-01-09 
	{
		pCScanView->m_dPipeDiameter = m_pMovePathView->m_GraphMove.GetPipR();
		dYStart = 0;
		{
			dYEnd = PI * pCScanView->m_dPipeDiameter;
		}
	}
	// Song Chenguang add start on 2013-05-04
	if(nAxisIndex == 5) // wangbingfu add start on 2013-01-09 
	{
		pCScanView->m_dPipeDiameter = m_pMovePathView->m_GraphMove.Get_WallDiameter();	// machao modify on 2013-12-4
		dXStart = 0;
		{
//			dXEnd = PI * pCScanView->m_dPipeDiameter;
			dXEnd = 360.0f;
		}
		dYStart = dZStart;
		dYEnd = dZEnd;
	}
	// Song Chenguang add end on 2013-05-04
	else if(nAxisIndex == 1)	// XZ
	{
		dYStart = dZStart;
		dYEnd = dZEnd;
	}
	else if(nAxisIndex == 2)	// YZ
	{
		dXStart = dYStart;
		dXEnd = dYEnd;

		dYStart = dZStart;
		dYEnd = dZEnd;
	}
	else
	{
		;//
	}

	pCScanView->m_dXStart = dXStart;
	pCScanView->m_dYStart = dYStart;
	pCScanView->m_dZStart = dZStart;
	pCScanView->m_dWStart = dWStart;	// Song Chenguang add on 2013-05-04
	pCScanView->m_dXEnd = dXEnd;
	pCScanView->m_dYEnd = dYEnd;
	pCScanView->m_dZEnd = dZEnd;
	pCScanView->m_fXMove = static_cast<float>(dXStart);
	pCScanView->m_fYMove = static_cast<float>(dYStart);	
	pCScanView->m_fYMoveForZuoBiao = static_cast<float>(pCScanView->m_dYStart);

	m_fFirstWdrX = static_cast<float>(pCScanView->m_dXStart);
	m_fFirstWdrY = static_cast<float>(pCScanView->m_dYStart);
	m_fFirstWdrZ = static_cast<float>(pCScanView->m_dZStart);

	//修改断电恢复后，继续扫查造成原有图像消失的问题  yachang.huang modify start on 2014-04-29
	if (pCScanView->bFreePictureMemoy)
	{
		pCScanView->FreePictureMemory();
		BOOL bRet = pCScanView->MallocPictureMemory();

		if(!bRet)
			return FALSE;
	}
	//yachang.huang modify end on 2014-04-29

	pCScanView->m_fScaleFactor = 1;
	CRect rect;
	pCScanView->GetClientRect(&rect);
	CSize csize;
	csize.cx =  rect.right - rect.left;
	csize.cy =  rect.bottom - rect.top;
	
	csize.cx = static_cast<LONG>(csize.cx * pCScanView->m_fScaleFactor);
	csize.cy = static_cast<LONG>(csize.cy * pCScanView->m_fScaleFactor);
	pCScanView->SetScrollSizes(MM_TEXT, csize);
	// 放大、缩小
	pCScanView->m_displaySize.cx = int(pCScanView->m_layout.cx * pCScanView->m_fScaleFactor);
	pCScanView->m_displaySize.cy = int(pCScanView->m_layout.cy * pCScanView->m_fScaleFactor);
	
	//用于点击成像按钮DrawPicture()成像
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for (int j=0; j< GATE_NUM; j++)
		{
			pCScanView->m_bFileGateFlag[i][j] = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[i].m_GateInfo[j].m_bOpen;
			if (pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[i].m_GateInfo[j].m_bOpen)
			{
				nGateNum[i]++;
			}
		}
	}
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		pCScanView->m_nFileGateNum[i] = nGateNum[i];
	}
	int nFeatureNum = 0;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay)
		nFeatureNum++;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay)
		nFeatureNum++;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay)
		nFeatureNum++;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay)
		nFeatureNum++;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay)
		nFeatureNum++;
	if(pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay)
		nFeatureNum++;
	nFeatureNum = nFeatureNum + 1;
	pCScanView->m_nFileFeatureNum = nFeatureNum;
	pCScanView->m_bFileFeatureFlag[1] = pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay;
	pCScanView->m_bFileFeatureFlag[2] = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay;
	pCScanView->m_bFileFeatureFlag[3] = pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay;
	pCScanView->m_bFileFeatureFlag[4] = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay;
	pCScanView->m_bFileFeatureFlag[5] = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay;
	pCScanView->m_bFileFeatureFlag[6] = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay;

	pCScanView->m_nProbeNum = m_SysParamDlg.m_nIndexProbeNum + 1;
	pCScanView->m_nScanType = m_SysParamDlg.m_nScanType;
	pCScanView->m_nFillType = lLogParam[0];
	pCScanView->m_nTwoProbeIndex = m_SysParamDlg.m_nIndexTwoProbeType;
	pCScanView->m_fProbeDistance = m_SysParamDlg.m_fProbeDistance;
	pCScanView->m_fChongDie = m_SysParamDlg.m_fChongDie;
	
	WORD wWorkMode = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode;
	//成像时是否关联已经确定,不随修改模式而变化
	if(wWorkMode == 4 || wWorkMode == 5 ||
		wWorkMode == 6 ||wWorkMode == 7 ||
		wWorkMode == 8)
	{
		pCScanView->m_bGuanLianFlag = TRUE;
	}
	else
	{
		pCScanView->m_bGuanLianFlag = FALSE;
	}

	pCScanView->Invalidate(TRUE);	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::CScanViewSet"),"Leave");
	return TRUE;	// Song Chenguang add on 2013-09-11
}

LRESULT CMechanicalControlPanel::OnReceiveFeatureInfo(WPARAM wParam, LPARAM lParam)
{
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnReceiveFeatureInfo"),"Enter");
	CH_FLAWDATA_CSCAN_S * pData = (CH_FLAWDATA_CSCAN_S*)wParam;
	CARD_INFO_t* pCardInfo = (CARD_INFO_t*)lParam;

	double dX = 0.0;
	double dY = 0.0;
	double dZ = 0.0;
	double dA = 0.0;
	double dB = 0.0;
	double dW = 0.0;
	double dR = 0.0;

	int nAxisIndex = m_pMovePathView->m_GraphMove.GetAxisSelect();
	GetCScanDataCoordinate(*pData, dX, dY, dZ, dR, dB, dW, dA, 1, nAxisIndex, pCardInfo->nCardIndex, pCardInfo->nChannelIndex);

	//解决坐标yachang.huang add start on 2014-04-29
	double dXDisplay = dX;
	double dYDisplay = dY;
	double dZDisplay = dZ;
	double dADisplay = dA;
	double dBDisplay = dB;
	double dWDisplay = dW;
	double dRDisplay = dR;

	if(nAxisIndex == 1)	// XZ
	{
		double dTemp = dZDisplay;
		dZDisplay = dYDisplay;
		dYDisplay = dTemp;
	}
	else if(nAxisIndex == 2)	// YZ
	{
		double dTempZ = dZDisplay;
		dZDisplay = dYDisplay;
		dYDisplay = dXDisplay;
		dXDisplay = dTempZ;
	}
	else
	{
		;//
	}

	CString strXCoord;
	strXCoord.Format(_T("X = %.2f"), dXDisplay);
	m_staticXCoord.SetWindowText(strXCoord);
	CString strYCoord;
	strYCoord.Format(_T("Y = %.2f"),dYDisplay);
	m_staticYCoord.SetWindowText(strYCoord);
	CString strZCoord;
	strZCoord.Format(_T("Z = %.2f"),dZDisplay);
	m_staticZCoord.SetWindowText(strZCoord);
	CString strRCoord;
	strRCoord.Format(_T("R = %.1f"), dRDisplay);
	GetDlgItem(IDC_STATIC_W1)->SetWindowText(strRCoord);
	CString strWCoord;
	strWCoord.Format(_T("W = %.1f"), dWDisplay);
	GetDlgItem(IDC_STATIC_W)->SetWindowText(strWCoord);
	CString strBCoord;
	strBCoord.Format(_T("B = %.1f"), dBDisplay);
	GetDlgItem(IDC_STATIC_BVALUE)->SetWindowText(strBCoord);

	//yachang.huang add end on 2014-04-29



	//////////////////////////////////////////////////////////////////////////
	// test
	m_fWCurPos = dW;
	//////////////////////////////////////////////////////////////////////////

	m_pCScanView->m_dX = dX;
	m_pCScanView->m_dY = dY;
	m_pCScanView->m_dZ = dZ;
	// wangbingfu modify start on 2014-10-14
	//m_pCScanView->m_dR = dR / 360.0f * (PI * m_pMovePathView->m_GraphMove.GetPipR());	// Song Chenguang modify on 2013-09-10
	m_pCScanView->m_dR = dR;	// 在GetCScanDataCoordinate()里面如果是周长的话会换算成周长
	// wangbingfu modify end on 2014-10-14

	m_pCScanView2->m_dX = dX;
	m_pCScanView2->m_dY = dY;
	m_pCScanView2->m_dZ = dZ;
	// wangbingfu modify start on 2014-10-14
//	m_pCScanView2->m_dR = dR / 360.0f * (PI * m_pMovePathView->m_GraphMove.GetPipR());	// Song Chenguang modify on 2013-09-10
	m_pCScanView2->m_dR = dR;	// 在GetCScanDataCoordinate()里面如果是周长的话会换算成周长
	// wangbingfu modify end on 2014-10-14
	//AfxMessageBox(_T("已进入CMechanicalControlPanel::OnReceiveFeatureInfo"));


	if (m_bPathExecute)
	{
		// wangbingfu modify start on 2014-10-14 降低m_GraphMove.PathTracing的调用频率，否则
		// 扫较大工件时会卡（400*300 0.2*0.2 开闸门A和B，开了A扫存储时，过段时间会卡，不开A扫存储开		//始就卡，把发给该类的消息注释掉就不卡）
		m_nTracingCount++;
		if(m_nTracingCount == 10)
		{
			double dAcoord[7];
			::ZeroMemory(dAcoord, sizeof(dAcoord));
			dAcoord[0] = dX;
			dAcoord[1] = dY;
			dAcoord[2] = dZ;
			dAcoord[3] = dA;
			dAcoord[4] = dB;
			dAcoord[5] = dW;
			dAcoord[6] = dR;
			//CString strAcoord;
			//strAcoord.Format(_T("%.1lf,%.1lf,%.1lf,%.1lf"),dX,dY,dZ,dW1);
			//MessageBox(_T("当前坐标值：") + strAcoord,_T("运行提示"),MB_OKCANCEL);
			//AfxMessageBox(_T("当前坐标值：") + strAcoord);
			// wangbingfu add output test start on 2014-10-14
		/*	SYSTEMTIME sysTime;
			CString strC;
			::GetLocalTime(&sysTime);
			int nTime = GetTickCount();*/
			// wangbingfu add output test end on 2014-10-14
			// wangbingfu add output test start on 2014-10-14
			m_pMovePathView->m_GraphMove.PathTracing(dAcoord);
		/*	nTime = GetTickCount() - nTime;
			strC.Format(_T("%d:%d:%d PathTracing time %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nTime);
			OutputDebugString(strC);*/
			// wangbingfu add output test end on 2014-10-14
			m_nTracingCount = 0;
			// wangbingfu modify start on 2014-10-14
		}
		
	}

	delete pData;
	pData = NULL;

	delete pCardInfo;
	pCardInfo = NULL;
    ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnReceiveFeatureInfo"),"Leave");
	return 0;
}

void CMechanicalControlPanel::PathStop( BOOL p_flag )
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathStop"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (m_bPathExecute)
	{
		m_btnRun.SetWindowText(_T("运行(F7)"));

		g_bDrawRealTime = FALSE;

		KillTimer(TIMER_ASYNC_SAVE_SCAN);

		if (p_flag)
		{
			m_bPathExecute = FALSE;

			// 保存数据文件			

			// rem by Song Chenguang start on 2013-03-29
			// 解决扫描完未当时保存，后点保存探伤数据按钮保存文件失败的问题
//			DeleteFile(strTempPath+L"acf");
//			DeleteFile(strTempPath+L"aaf");
			// rem by Song Chenguang end on 2013-03-29
			// Song Chenguang add start on 2013-04-26			
			if (!m_bTaskRun)
			{
				SaveTestResult();
			}
			else
			{
				// 自动保存数据文件
				int nIndex1 = m_strCurrentMovePath.ReverseFind('.');
				CString strDataPath = m_strCurrentMovePath.Left(nIndex1) + _T(".uit");
				int nIndex2 = strDataPath.ReverseFind('\\');

				CString strFileName = strDataPath.Right(strDataPath.GetLength() - nIndex2 - 1);
				CString strNotePath = _T("");
				strNotePath = strDataPath.Mid(0, strDataPath.GetLength() - 4);
				strNotePath = strNotePath + L".note";
				m_pCScanView->m_strNotePath = strNotePath;
				m_pCScanView2->m_strNotePath = strNotePath;
				SaveDataFile(strDataPath, strFileName);

				// Song Chenguang add start on 2013-07-01
				// 改变列表状态
				pFrame->m_wndTaskDockBar.m_pFormView->m_lstTaskItems.SetItemText(m_wTaskNo, 2, _T("运行完毕"));
				// Song Chenguang add start on 2013-07-01

				m_wTaskNo++;
				if (m_wTaskNo < m_wTaskItemCount)
				{
					CStdioFile file;
					file.Open(m_strTaskListPath, CFile::modeRead);
					CString strPath;
					for (int i=0; i<m_wTaskNo+1; i++)
					{
						file.ReadString(strPath);
					}

					file.Close();
					m_strCurrentMovePath = strPath;

					int nIndex = strPath.ReverseFind('.');
					CString strSubPath = strPath.Left(nIndex);
					CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
					// 读通道参数文件
					CString strChannelParaPath = strSubPath +_T(".par");
					pFrame->m_wndChannelInfo.m_pFormView->ReadParam(strChannelParaPath);

					// 读C扫参数文件
					CString strCScanParaPath1 = strSubPath +_T("1.cpar");
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView->ReadCScanPara(strCScanParaPath1, TRUE);

					CString strCScanParaPath2 = strSubPath +_T("2.cpar");
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->ReadCScanPara(strCScanParaPath2, TRUE);

					// 读机械扫描轨迹文件
					m_pMovePathView->m_GraphMove.OpenFileByPath(strPath);

					// 显示当前扫描任务子项
					int nIndex2 = strPath.ReverseFind('\\');
					CString strTaskItem = strPath.Mid(nIndex2+1, (nIndex - nIndex2 - 1));
					CString strInfo;
					strInfo.Format(_T("正在运行：%s"), strTaskItem);
					pFrame->m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_STATIC_RUN_STATE)->SetWindowText(strInfo);

					// 运行
					OnBnClickedButtonRun();

					return;
				} 
				else
				{
					m_bTaskRun = FALSE;
					m_strCurrentMovePath = _T("");
					MessageBox(_T("扫查任务执行完毕！"));
					CTaskPanel* pPanel = pFrame->m_wndTaskDockBar.m_pFormView;
					pPanel->GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);
					pPanel->GetDlgItem(IDC_STATIC_RUN_STATE)->SetWindowText(_T("运行结束"));
				}				
			}
			// Song Chenguang add end on 2013-04-26
		}
	
		pFrame->m_wndChannelInfo.m_pFormView->UpdateData(TRUE);

		for (int i = 0;i<(sizeof(m_ChannelInfoID) / sizeof(int));++i)
		{
			pFrame->m_wndChannelInfo.m_pFormView->GetDlgItem(m_ChannelInfoID[i])->EnableWindow(TRUE);
		}
		
		for (int i = 0;i<(sizeof(m_CScanInfoID) / sizeof(int));++i)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->GetDlgItem(m_CScanInfoID[i])->EnableWindow(TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->GetDlgItem(m_CScanInfoID[i])->EnableWindow(TRUE);
		}

		for (int i = 0;i<(sizeof(m_MechanicalInfoID) / sizeof(int));++i)
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(m_MechanicalInfoID[i])->EnableWindow(TRUE);
		}
		int nAxisIndex = m_pMovePathView->m_GraphMove.GetAxisSelect();
		if(nAxisIndex == 3)
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_POSITIVE)->EnableWindow(TRUE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_REVERSE)->EnableWindow(TRUE);
		}
		else
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_POSITIVE)->EnableWindow(FALSE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W1_REVERSE)->EnableWindow(FALSE);
		}

		pFrame->m_wndChannelInfo.m_pFormView->ALLSCanItemEnableJudge();

		// Song chenguang modified on 2013-04-26
		// 走轨迹才刷新图（从下面括号外挪到括号里）
		m_pCScanView->Invalidate(TRUE);
		m_pCScanView2->Invalidate(TRUE);
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::PathStop"),"Leave");
}

void CMechanicalControlPanel::OnArriveStartPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnArriveStartPoint"),"Enter");
#ifdef _DEBUG
	CString string;
	string.Format(_T("%.3f"),m_fFirstWdrX);
	//	MessageBox(string, _T("提示"), MB_OK);
#endif
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();

	//KillTimer(TIMER_ASYNC_SAVE_SCAN);
	//断电恢复后开始检测时不清除伤点数据  yachang.huang modify on 2014-4-29
	if (m_pCScanView->bFreePictureMemoy)
	{
		for (int i = 0; i<CHANNEL_NUM; i++)
		{
			pDoc->m_lstFlawData[i].RemoveAll();
			//		pDoc->m_lstAScanData[i].RemoveAll();	// rem by Song Chenguang on 2013-12-18
			pDoc->m_lstFlawDataOriginal[i].RemoveAll();	// 2011-11-18
			// wangbing test start on 2014-10-11
			int nCSize = pDoc->m_qCScanData[i].size();
			int nASize = pDoc->m_qAScanData[i].size();
			TRACE(_T("Asize free before %d"), nASize);
			CString strA;
			SYSTEMTIME sysTime;
			::GetLocalTime(&sysTime);
			strA.Format(_T("%d:%d:%d Asize free before %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nASize);
			OutputDebugString(strA);
	//		wsprintf(strA.GetBuffer(), _T("Asize free before %d\n"), nASize);

			TRACE(_T("Csize free before %d"), nCSize);
			CString strC;
			::GetLocalTime(&sysTime);
			strC.Format(_T("%d:%d:%d Csize free before %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nCSize);
	//		strC.Format(_T("Csize free before %d\n"), nCSize);
			OutputDebugString(strC);
	//		wsprintf(strC.GetBuffer(), _T("Csize free before %d\n"), nCSize);
			// wangbing test end on 2014-10-11
			while(!pDoc->m_qAScanData[i].empty()) {pDoc->m_qAScanData[i].pop();}
			while(!pDoc->m_qCScanData[i].empty()) {pDoc->m_qCScanData[i].pop();}
			pDoc->m_lstFlawDataTemp[i].RemoveAll(); // wangbingfu add at 2012-10-12
			// wangbing test start on 2014-10-11
			int nCSize1 = pDoc->m_qCScanData[i].size();
			int nASize1 = pDoc->m_qAScanData[i].size();
			TRACE(_T("Asize free after %d"), nASize1);
			CString strA1;
			::GetLocalTime(&sysTime);
			strA1.Format(_T("%d:%d:%d Asize free after %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nASize1);
			OutputDebugString(strA1);
		//	wsprintf(strA1.GetBuffer(), _T("Asize free after %d\n"), nASize1);

			TRACE(_T("Csize free after %d"), nCSize1);
			CString strC1;
			::GetLocalTime(&sysTime);
			strC1.Format(_T("%d:%d:%d Csize free after %d\n"),
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nCSize1);
		//	strC1.Format(_T("Csize free after %d\n"), nCSize1);
			OutputDebugString(strC1);
		//	wsprintf(strC1.GetBuffer(), _T("Csize free after %d\n"), nCSize1);
			// wangbing test end on 2014-10-11
		}
	}

	m_pCScanView->m_Offset_Coordinate.x=0;
	m_pCScanView->m_Offset_Coordinate.y=0;	//-1024;
	m_pCScanView2->m_Offset_Coordinate.x=0;
	m_pCScanView2->m_Offset_Coordinate.y=0;	//-1024;
	
	BOOL bRet = CScanViewSet(m_pCScanView);
	if (!bRet)
		return;
	bRet = CScanViewSet(m_pCScanView2);
	if (!bRet)
		return;

	TCHAR szTempPath [MAX_PATH];
	DWORD dwSize = MAX_PATH;
	DWORD dwResult = GetTempPath(dwSize, szTempPath);//获取临时文件夹路径 
	m_strTempPath = szTempPath ;
	m_strTempPath += L"UTTAsync.";
	m_strTempPath1 = szTempPath;
	m_strTempPath1 += L"UTTAsync1.";
	if (m_pCScanView->bFreePictureMemoy)
	{
		m_AsyncCScanFile.Open(m_strTempPath+L"acf", CFile::modeCreate | CFile::modeWrite, NULL);
		m_AsyncAScanFile.Open(m_strTempPath+L"aaf", CFile::modeCreate| CFile::modeWrite, NULL);
		m_AsyncAScanFile1.Open(m_strTempPath1+L"aaf", CFile::modeCreate| CFile::modeWrite, NULL);// wangbingfu add on 2014-09-10
		m_AsyncAScanPrimitiveFile.Open(m_strTempPath + L"paaf", CFile::modeCreate| CFile::modeWrite, NULL);	
		m_AsyncAScanPrimitiveFile.Close();
	}
	else
	{
		m_AsyncCScanFile.Open(m_strTempPath+L"acf",  CFile::modeWrite, NULL);
		m_AsyncAScanFile.Open(m_strTempPath+L"aaf",  CFile::modeWrite, NULL);
		m_AsyncAScanFile1.Open(m_strTempPath1+L"aaf",  CFile::modeWrite, NULL);	// wangbingfu add on 2014-09-10
		m_AsyncAScanPrimitiveFile.Open(m_strTempPath + L"paaf", CFile::modeWrite, NULL);	
		m_AsyncAScanPrimitiveFile.Close();
	}

/*
	m_AscanTxtFile.Open(_T("C:\\C扫数据.txt"), CFile::modeCreate| CFile::modeWrite, NULL);
	m_AscanTxtFile.WriteString(_T("X\t\tY\t\tZ\t\tA\t\tB\t\tW\t\tR\r\n"));
	m_AscanTxtFile.Close();
*/
	CCScanFileHeader fileHeader;

	// 扫查架范围
	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));
	m_pMovePathView->m_GraphMove.GetPara(dParam, lLogParam);


	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	fileHeader.m_fRangeX = (float)pApp->m_IniParams.m_info.fXRange;
	fileHeader.m_fRangeY = (float)pApp->m_IniParams.m_info.fYRange;

	// 闸门数据
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	for(int i=0; i<CHANNEL_NUM; i++)
	{
		CChannelInfo info = pFrame->GetChannelInfo(i);

		for(int j=0; j<4; j++)
			fileHeader.m_GateInfo[i][j] = info.m_GateInfo[j];
	}

	// 特征值开关
	fileHeader.m_bFPFTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay;
	fileHeader.m_bFPMTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay;
	fileHeader.m_bFPMAltitudeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay;
	fileHeader.m_bMPFTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay;
	fileHeader.m_bMPMTimeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay;
	fileHeader.m_bMPMAltitudeDisplay = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay;
	// 工件厚度
	fileHeader.m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;
	fileHeader.m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fThick;
	// 网格间距(暂时用默认值)
	if (lLogParam[0] == 0) // 横向
	{
		fileHeader.m_fGridX = (float)m_SysParamDlg.m_dFillSpace2;
		fileHeader.m_fGridY = (float)m_SysParamDlg.m_dFillSpace;
	} 
	else	// 纵向
	{
		fileHeader.m_fGridX = (float)m_SysParamDlg.m_dFillSpace;
		fileHeader.m_fGridY = (float)m_SysParamDlg.m_dFillSpace2;
	}

	// 声速、通道支持个数
	fileHeader.m_wWaveSpeed = pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed;
	fileHeader.m_wChannelNum = CHANNEL_NUM;

	double dXStart = m_pCScanView->m_dXStart;
	double dYStart = m_pCScanView->m_dYStart;
	double dZStart = m_pCScanView->m_dZStart;

	double dXEnd = m_pCScanView->m_dXEnd;
	double dYEnd = m_pCScanView->m_dYEnd;
	double dZEnd = m_pCScanView->m_dZEnd;

	fileHeader.m_dXStart = static_cast<float>(dXStart);
	fileHeader.m_dYStart =static_cast<float>(dYStart);
	fileHeader.m_dZStart =static_cast<float>(dZStart);
	fileHeader.m_dXEnd =static_cast<float>(dXEnd);
	fileHeader.m_dYEnd =static_cast<float>(dYEnd);
	fileHeader.m_nAxisSelect = m_pMovePathView->m_GraphMove.GetAxisSelect();
	fileHeader.m_dPipeDiameter = m_pMovePathView->m_GraphMove.GetPipR();
	// machao add start on 2013-12-4
	if (fileHeader.m_nAxisSelect == 5)
	{
		fileHeader.m_dPipeDiameter = m_pMovePathView->m_GraphMove.Get_WallDiameter();
	}
	// machao add end on 2013-11-4
	
	// wangbingfu modify start at 2012-9-25
	fileHeader.m_nProbeNum = m_SysParamDlg.m_nIndexProbeNum;
	fileHeader.m_nScanType = m_SysParamDlg.m_nScanType;
	fileHeader.m_nFillType = lLogParam[0];
//	fileHeader.m_nTwoProbeIndex = m_SysParamDlg.m_nIndexTwoProbeType;
	fileHeader.m_fProbeDistance = m_SysParamDlg.m_fProbeDistance;
	fileHeader.m_fChongDie = m_SysParamDlg.m_fChongDie;
	fileHeader.m_nWorkMode = m_SysParamDlg.m_wWorkMode;
	
	if( m_SysParamDlg.m_nIndexProbeNum == 0)	// 1块卡
	{
		fileHeader.m_nTwoProbeIndex =  m_SysParamDlg.m_nIndexOneProbeType;
	}
	else if( m_SysParamDlg.m_nIndexProbeNum == 1)	// 2块卡
	{
		fileHeader.m_nTwoProbeIndex =  m_SysParamDlg.m_nIndexTwoProbeType;
	}
	else if( m_SysParamDlg.m_nIndexProbeNum == 2)	// 3块卡
	{
		fileHeader.m_nTwoProbeIndex =  m_SysParamDlg.m_nIndexThreeProbeType;
	}
	else
	{
		fileHeader.m_nTwoProbeIndex = 0;
	}
	// wangbingfu modify end at 2012-9-25

	m_AsyncCScanFile.Write(&fileHeader, sizeof(CCScanFileHeader));

	// 伤点总数
	//	CUltrasonicImmersedTestDoc* pDoc = GetDocument();

	for (int i= 0; i<fileHeader.m_wChannelNum; i++)
	{
		DWORD dwCount= 0;
		m_AsyncCScanFile.Write(&dwCount, sizeof(DWORD));
	}
	// wangbingfu modify start on 2014-09-10 // 封成函数，并加上m_AsyncAScanFile1
/*	m_AsyncAScanFile.Write(&dXStart,sizeof(double));
	m_AsyncAScanFile.Write(&dYStart,sizeof(double));
	m_AsyncAScanFile.Write(&dXEnd,sizeof(double));
	m_AsyncAScanFile.Write(&dYEnd,sizeof(double));
	m_AsyncAScanFile.Write(&fileHeader.m_dPipeDiameter,sizeof(double));
	m_AsyncAScanFile.Write(&fileHeader.m_nAxisSelect,sizeof(int));
	m_AsyncAScanFile.Write(&fileHeader.m_fGridX,sizeof(float));
	m_AsyncAScanFile.Write(&fileHeader.m_fGridY,sizeof(float));

	m_grid_tmp.headSize=sizeof(double)*5+sizeof(float)*2+sizeof(int)*2+sizeof(ULONG) + sizeof(CGateInfo) * GATE_NUM * CHANNEL_NUM;
	m_grid_tmp.gridX=fileHeader.m_fGridX;
	m_grid_tmp.gridY=fileHeader.m_fGridY;
	m_grid_tmp.AxisSelect=fileHeader.m_nAxisSelect;
	m_grid_tmp.lastIndex=0;
	m_grid_tmp.isEmpty=true;

	if (fileHeader.m_nAxisSelect == 3 ||
		fileHeader.m_nAxisSelect == 4)	// Song Chenguang modify on 2014-01-16
	{
		m_grid_tmp.startX=0;
		m_grid_tmp.startY=0;

		m_grid_tmp.gridXSize=1;
		m_grid_tmp.gridYSize=static_cast<int>(ceil(fileHeader.m_dPipeDiameter*PI*(ceil(fabs(dXEnd-dXStart)/fileHeader.m_fGridX))/fileHeader.m_fGridY));
		m_grid_tmp.gridSize=static_cast<int>(m_grid_tmp.gridXSize*m_grid_tmp.gridYSize);
	} 
	else
	{
		m_grid_tmp.startX=dXStart;
		m_grid_tmp.startY=dYStart;

		m_grid_tmp.gridXSize=static_cast<int>(ceil(fabs(dXEnd-dXStart)/fileHeader.m_fGridX));
		m_grid_tmp.gridYSize=static_cast<int>(ceil(fabs(dYEnd-dYStart)/fileHeader.m_fGridY));
		m_grid_tmp.gridSize=m_grid_tmp.gridXSize*m_grid_tmp.gridYSize;
	}
	m_AsyncAScanFile.Write(&m_grid_tmp.gridXSize,sizeof(int));
	m_AsyncAScanFile.Write(&m_grid_tmp.gridSize,sizeof(ULONG));
	// Song chenguang add start on 2013-12-19
	// 保存闸门信息
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	for (int i=0; i<CHANNEL_NUM; i++)
	{
		m_AsyncAScanFile.Write(pChannelPanel->m_ChannelInfo[i].m_GateInfo, sizeof(CGateInfo)*GATE_NUM);
	}
	// Song chenguang add end on 2013-12-19

	ULONG *i_grid=new ULONG[m_grid_tmp.gridSize];
	// WangQianfeng add start on 2012-11-06
	if ( NULL == i_grid )
	{
		MessageBox(_T("CMechanicalControlPanel::OnArriveStartPoint(),i_grid = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnArriveStartPoint"),"Leave");
		return;
	}

	memset(i_grid,0,sizeof(ULONG)*m_grid_tmp.gridSize);
	m_AsyncAScanFile.Write(i_grid,sizeof(ULONG)*m_grid_tmp.gridSize);
	*/
	SaveAScanHeadData(fileHeader, &m_AsyncAScanFile);
	SaveAScanHeadData(fileHeader, &m_AsyncAScanFile1);
	// wangbingfu modify end on 2014-09-10

	m_AsyncCScanFile.Close();	
	m_AsyncAScanFile.Close();	
	m_AsyncAScanFile1.Close(); // wangbingfu add on 2014-09-10

	SetTimer(TIMER_ASYNC_SAVE_SCAN,1000,NULL);
	//*********同步保存文件初始化完成************
	
	// wangbingfu add start on 2013-01-09
	pFrame->ReadPipeIni();
	m_pCScanView->m_fPipeProbeAngle = pFrame->m_fPipeProbeAngle;
	m_pCScanView2->m_fPipeProbeAngle = pFrame->m_fPipeProbeAngle;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		m_pCScanView->m_nPipeChannelSelect[i] = pFrame->m_nPipeChannelSelect[i];
		m_pCScanView2->m_nPipeChannelSelect[i] = pFrame->m_nPipeChannelSelect[i];
	}
	m_pCScanView->m_nCScanSelect = pFrame->m_nCScanSelect[0];
	m_pCScanView2->m_nCScanSelect = pFrame->m_nCScanSelect[1];
	// wangbingfu add end on 2013-01-09
	

	//清空缓存及C扫视图
/*	m_pCScanView->m_MemDC.SetViewportOrg(0, 0);
	m_pCScanView->m_MemDC.FillSolidRect(0, 0, m_pCScanView->m_CScanSize.cx, m_pCScanView->m_CScanSize.cy, m_pCScanView->m_cscan_back_color);
	m_pCScanView->Invalidate(TRUE);
	m_pCScanView2->m_MemDC.SetViewportOrg(0, 0);
	m_pCScanView2->m_MemDC.FillSolidRect(0, 0, m_pCScanView->m_CScanSize.cx, m_pCScanView->m_CScanSize.cy, m_pCScanView->m_cscan_back_color);
	m_pCScanView2->Invalidate(TRUE);
*/

	PathExeute();

	// 更新C扫面板上闸门阈值
	CChannelInfo info = pFrame->GetChannelInfo((WORD)g_dwActiveChannel);
	CCScanParamPanel* pCPannel = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	CCScanParamPanel* pCPannel2 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;

	CString str;
	float fGatePeak;
	fGatePeak = static_cast<float>(info.m_GateInfo[0].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel->m_fGate1Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[1].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel->m_fGate2Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[2].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel->m_fGate3Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[3].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel->m_fGate4Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[0].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel2->m_fGate1Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[1].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel2->m_fGate2Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[2].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel2->m_fGate3Threshold = static_cast<float>(_tstof(str));

	fGatePeak = static_cast<float>(info.m_GateInfo[3].m_fPeakGate);					
	str.Format(_T("%.1f"),fGatePeak);
	pCPannel2->m_fGate4Threshold = static_cast<float>(_tstof(str));

	pCPannel->UpdateData(FALSE);
	pCPannel2->UpdateData(FALSE);

	// Song Chenguang add start on 2013-06-17
	// 创建文件记录临时文件路径
	CStdioFile file;
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString strPath = CString(strFolder) + _T("扫描数据临时文件位置.txt");
	file.Open(strPath, CFile::modeCreate | CFile::modeReadWrite);
	CString strContent = m_strTempPath + _T("acf");
	file.WriteString(strContent);
	file.Close();
	// Song Chenguang add end on 2013-06-17
	// Song Chenguang add start on 2013-06-19
	// 备份扫查路径、仪器参数
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	//保存仪器参数
	pChannelPanel->SaveParam(m_strTempPath+L"asf");

	//保存扫描路径
	m_pMovePathView->m_GraphMove.SaveFileByPath(m_strTempPath+L"apf");
	// Song Chenguang add end on 2013-06-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnArriveStartPoint"),"Leave");
}
// wangbingfu add start on 2014-09-10
void CMechanicalControlPanel::SaveAScanHeadData(const CCScanFileHeader &fileHeader, CFile* file)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	double dXStart = m_pCScanView->m_dXStart;
	double dYStart = m_pCScanView->m_dYStart;
	double dXEnd = m_pCScanView->m_dXEnd;
	double dYEnd = m_pCScanView->m_dYEnd;

	file->Write(&dXStart,sizeof(double));
	file->Write(&dYStart,sizeof(double));
	file->Write(&dXEnd,sizeof(double));
	file->Write(&dYEnd,sizeof(double));
	file->Write(&fileHeader.m_dPipeDiameter,sizeof(double));
	file->Write(&fileHeader.m_nAxisSelect,sizeof(int));
	file->Write(&fileHeader.m_fGridX,sizeof(float));
	file->Write(&fileHeader.m_fGridY,sizeof(float));

	m_grid_tmp.headSize=sizeof(double)*5+sizeof(float)*2+sizeof(int)*2+sizeof(ULONG) + sizeof(CGateInfo) * GATE_NUM * CHANNEL_NUM;
	m_grid_tmp.gridX=fileHeader.m_fGridX;
	m_grid_tmp.gridY=fileHeader.m_fGridY;
	m_grid_tmp.AxisSelect=fileHeader.m_nAxisSelect;
	m_grid_tmp.lastIndex=0;
	m_grid_tmp.isEmpty=true;

	if (fileHeader.m_nAxisSelect == 3 ||
		fileHeader.m_nAxisSelect == 4)	// Song Chenguang modify on 2014-01-16
	{
		m_grid_tmp.startX=0;
		m_grid_tmp.startY=0;
		
		// wangbingfu modify start on 2014-11-04
//		m_grid_tmp.gridXSize=1;
//		m_grid_tmp.gridYSize=static_cast<int>(ceil(fileHeader.m_dPipeDiameter*PI*(ceil(fabs(dXEnd-dXStart)/
//.m_fGridX))/fileHeader.m_fGridY));
		m_grid_tmp.gridXSize=static_cast<int>(ceil(fabs(dXEnd-dXStart)/fileHeader.m_fGridX));
		m_grid_tmp.gridYSize=static_cast<int>(ceil(fileHeader.m_dPipeDiameter*PI/fileHeader.m_fGridY));
		// wangbingfu modify end on 2014-11-04
		m_grid_tmp.gridSize=static_cast<int>(m_grid_tmp.gridXSize*m_grid_tmp.gridYSize);
	} 
	else
	{
		m_grid_tmp.startX=dXStart;
		m_grid_tmp.startY=dYStart;

		m_grid_tmp.gridXSize=static_cast<int>(ceil(fabs(dXEnd-dXStart)/fileHeader.m_fGridX));
		m_grid_tmp.gridYSize=static_cast<int>(ceil(fabs(dYEnd-dYStart)/fileHeader.m_fGridY));
		m_grid_tmp.gridSize=m_grid_tmp.gridXSize*m_grid_tmp.gridYSize;
	}
	file->Write(&m_grid_tmp.gridXSize,sizeof(int));
	file->Write(&m_grid_tmp.gridSize,sizeof(ULONG));
	// Song chenguang add start on 2013-12-19
	// 保存闸门信息
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	for (int i=0; i<CHANNEL_NUM; i++)
	{
		file->Write(pChannelPanel->m_ChannelInfo[i].m_GateInfo, sizeof(CGateInfo)*GATE_NUM);
	}
	// Song chenguang add end on 2013-12-19

	ULONG *i_grid=new ULONG[m_grid_tmp.gridSize];
	// WangQianfeng add start on 2012-11-06
	if ( NULL == i_grid )
	{
		MessageBox(_T("CMechanicalControlPanel::OnArriveStartPoint(),i_grid = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnArriveStartPoint"),"Leave");
		return;
	}

	memset(i_grid,0,sizeof(ULONG)*m_grid_tmp.gridSize);
	file->Write(i_grid,sizeof(ULONG)*m_grid_tmp.gridSize);
	delete[] i_grid; // wangbingfu add on 2014-10-23 解决每次扫查都比上次扫查内存增涨的内存泄漏问题
}
// wangbingfu add end on 2014-09-10

void CMechanicalControlPanel::OnResetLowerMachine()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnResetLowerMachine"),"Enter");
	Sleep(1000);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// songchenguang add start 2012-01-16
	// 复位
	char szSendData[NETHEAD_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;
	::ZeroMemory(&stPacketHead, sizeof(PACKET_HEAD_t));
	stPacketHead.dwCommand = COMMAND_RESET;

	memcpy(szSendData, &stPacketHead, sizeof(PACKET_HEAD_t));
	
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i=0; i< nCardNum; i++)
	{
		int nChannelIndex = i*2;
		pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
	}
	
	// songchenguang add end 2012-01-16

	CCScanView* m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	m_pCScanView->Invalidate(TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnResetLowerMachine"),"Leave");
}

void CMechanicalControlPanel::SetStepLabelText()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SetStepLabelText"),"Enter");
	CString strXstep;
	strXstep.Format(_T("X轴的步进： %.1f mm"),m_fXstep);
	GetDlgItem(IDC_STATIC_STEP_X)->SetWindowText(strXstep);

	CString strYstep;
	strYstep.Format(_T("Y轴的步进： %.1f mm"),m_fYstep);
	GetDlgItem(IDC_STATIC_STEP_Y)->SetWindowText(strYstep);

	CString strZstep;
	strZstep.Format(_T("Z轴的步进： %.1f mm"),m_fZstep);
	GetDlgItem(IDC_STATIC_STEP_Z)->SetWindowText(strZstep);

	// Song Chenguang add start on 2013-03-15
	CString strABStep;
	strABStep.Format(_T("A(B)轴的步进：%.1f 度"), m_fABStep);
	GetDlgItem(IDC_STATIC_STEP_AB)->SetWindowText(strABStep);

	CString strWStep;
	strWStep.Format(_T("W轴的步进：%.1f 度"), m_fWStep);
	GetDlgItem(IDC_STATIC_STEP_W)->SetWindowText(strWStep);
	// Song Chenguang add end on 2013-03-15
	CString strRStep;
	strRStep.Format(_T("R轴的步进：%.1f 度"), m_fRStep);
	GetDlgItem(IDC_STATIC_STEP_R)->SetWindowText(strRStep);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::SetStepLabelText"),"Leave");
}
void CMechanicalControlPanel::OnBnClickedButtonShangliaoPos()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonShangliaoPos"),"Enter");
	int nRet = MessageBox(_T("移动到上料位置？"), _T("提示"), MB_OKCANCEL);

	if(nRet == IDOK)
	{
		double dX = m_SysParamDlg.m_fShangLiaoX;
		double dY = m_SysParamDlg.m_fShangLiaoY;
		double dCurrentPos[6];
		m_pMovePathView->m_GraphMove.GetPos(dCurrentPos);

		m_pMovePathView->m_bULDPtp = TRUE;
		// wangbingfu modify start on 2013-01-31
		m_pMovePathView->m_GraphMove.PtpMove(dX, dY, 0.0, dCurrentPos[3], dCurrentPos[4], dCurrentPos[5], 1);
		// wangbingfu modify end on 2013-01-31
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMechanicalControlPanel::OnBnClickedButtonShangliaoPos"),"Leave");
}

// Song Chenguang add start on 2013-04-26
void CMechanicalControlPanel::RunTask(CString strListPath)
{
	m_bTaskRun = TRUE;
	m_wTaskNo = 0;
	m_strTaskListPath = strListPath;

	CStdioFile file;

	file.Open(strListPath, CFile::modeRead);
	CString strFirstPath;
	file.ReadString(strFirstPath);
	file.Close();

	m_strCurrentMovePath = strFirstPath;

	int nIndex = strFirstPath.ReverseFind('.');
	CString strSubPath = strFirstPath.Left(nIndex);	

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	// 读通道参数文件
	CString strChannelParaPath = strSubPath +_T(".par");
	pFrame->m_wndChannelInfo.m_pFormView->ReadParam(strChannelParaPath);

	// 读C扫参数文件
	CString strCScanParaPath1 = strSubPath +_T("1.cpar");
	pFrame->m_wndCScanParamDockBar.m_pCScanParamView->ReadCScanPara(strCScanParaPath1, TRUE);

	CString strCScanParaPath2 = strSubPath +_T("2.cpar");
	pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->ReadCScanPara(strCScanParaPath2, TRUE);

	// 读机械扫描轨迹文件
	m_pMovePathView->m_GraphMove.OpenFileByPath(strFirstPath);

	// 显示当前扫描任务子项
	int nIndex2 = strFirstPath.ReverseFind('\\');
	CString strTaskItem = strFirstPath.Mid(nIndex2+1, (nIndex - nIndex2 - 1));
	CString strInfo;
	strInfo.Format(_T("正在运行：%s"), strTaskItem);
	pFrame->m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_STATIC_RUN_STATE)->SetWindowText(strInfo);

	// 运行
	OnBnClickedButtonRun();
}
// Song Chenguang add end on 2013-04-26
void CMechanicalControlPanel::OnBnClickedButtonGetWaterDistance()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelInfo info = pFrame->GetChannelInfo((WORD)g_dwActiveChannel);
	if (pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] < 1E-1)
	{
		MessageBox(_T("请检查I闸门是否框住界面波"));
		return;
	}

	int nDotNum = int(pFrame->m_wndChannelInfo.m_pFormView->m_fRange * 2 / (pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384 + 1;

	m_dWaterDistance = (pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * nBeiShu + info.m_fZeroPoint) / 1000000.0f / 2 *
		WATER_SPEED * 1000;
	
	UpdateData(FALSE);
}

void CMechanicalControlPanel::OnBnClickedCheckXYReverse()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	SetMoveButtonIcon(FALSE);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	pApp->WriteProfileInt(_T("水浸超声探伤系统"), _T("XY_Reverse"), int(m_bXYReverse));
}

void CMechanicalControlPanel::SetMoveButtonIcon(const BOOL& bIsInit)
{
	if (m_bXYReverse)
	{
		// 换图
		HICON Hicon;
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_X_DECREASE_REVERSE);
		m_btnXLeft.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_X_INCREASE_REVERSE);
		m_btnXRight.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Y_INCREASE_REVERSE);
		m_btnYFront.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Y_DECREASE_REVERSE);
		m_btnYBack.SetIcon(Hicon);
		// 移动位置
		CRect rectXDecrease;
		m_btnXLeft.GetWindowRect(&rectXDecrease);
		ScreenToClient(&rectXDecrease);
		CRect rectXIncrease;
		m_btnXRight.GetWindowRect(&rectXIncrease);
		ScreenToClient(&rectXIncrease);
		CRect rectYDecrease;
		m_btnYBack.GetWindowRect(&rectYDecrease);
		ScreenToClient(&rectYDecrease);
		CRect rectYIncrease;
		m_btnYFront.GetWindowRect(&rectYIncrease);
		ScreenToClient(&rectYIncrease);

		m_btnXLeft.MoveWindow(&rectYDecrease);
		m_btnXRight.MoveWindow(&rectYIncrease);
		m_btnYBack.MoveWindow(&rectXIncrease);
		m_btnYFront.MoveWindow(&rectXDecrease);
	}
	else
	{		
		// 换图
		HICON Hicon;
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_X_LEFT);
		m_btnXLeft.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_X_RIGHT);
		m_btnXRight.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Y_FRONT);
		m_btnYFront.SetIcon(Hicon);
		Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Y_BACK);
		m_btnYBack.SetIcon(Hicon);
		// 移动位置
		CRect rectXDecrease;
		m_btnXLeft.GetWindowRect(&rectXDecrease);
		ScreenToClient(&rectXDecrease);
		CRect rectXIncrease;
		m_btnXRight.GetWindowRect(&rectXIncrease);
		ScreenToClient(&rectXIncrease);
		CRect rectYDecrease;
		m_btnYBack.GetWindowRect(&rectYDecrease);
		ScreenToClient(&rectYDecrease);
		CRect rectYIncrease;
		m_btnYFront.GetWindowRect(&rectYIncrease);
		ScreenToClient(&rectYIncrease);

		if(!bIsInit)
		{
			m_btnXLeft.MoveWindow(&rectYIncrease);
			m_btnXRight.MoveWindow(&rectYDecrease);
			m_btnYBack.MoveWindow(&rectXDecrease);
			m_btnYFront.MoveWindow(&rectXIncrease);
		}		
	}

	HICON Hicon;
	Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Z_UP);
	m_btnZUp.SetIcon(Hicon);
	Hicon = AfxGetApp()->LoadIcon(IDI_ICON_Z_DOWN);
	m_btnZDown.SetIcon(Hicon);
}
void CMechanicalControlPanel::OnBnClickedCheckOnestep()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
// machao add start on 2013-11-18
void CMechanicalControlPanel::SaveFilePath(const CString& strPath)
{
	CString strOtherPath;
	strOtherPath = strPath.Mid(0, strPath.GetLength() - 4);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	CString str;
	str = strOtherPath + _T(".apf");
	if (PathFileExists(str))
	{
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("apfFilePath"),str);
	}

	str = strOtherPath + _T(".asf");
	if (PathFileExists(str))
	{
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("asfFilePath"),str);
	}
}
// machao add end on 2013-11-18
