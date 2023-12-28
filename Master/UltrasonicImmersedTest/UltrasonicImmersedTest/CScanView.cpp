/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* Cɨ��ͼ��ʾִ���ļ�
* Filename : CScanView.cpp
*
* @author       Song Chenguang
* @version      1.0         Date: 2011-08-01 
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "ExButton.h"
#include "UltrasonicImmersedTestDoc.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "ZoomView.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "MovePathView.h"
#include "GeomRotaDlg.h"
#include "math.h"
#include "OverViewWnd.h"
#include "SmoothDlg.h"
#include "MidFilterDlg.h"
#include "MicroscopeWnd.h"
#include "math.h"
#include "GlobalFunction.h"
#include "MovePathView.h"
#include "FeatureInfoWnd.h"
#include "UltrasonicImmersedTestView.h"
#include "MemDC.h"
#include "RulerWnd.h"
#include <vector>
#include "EdgeDBSetDlg.h"
#include "DropFaultDlg.h"	//machao add on 2013-07-12

//Machao add start on 2013-07-12
#using "..\\release\\DFClass.dll"
using namespace DropFault;
//Machao add start on 2013-07-12
using namespace std;
extern DWORD g_dwActiveChannel;
// CCScanView
const float MIN_FACTOR = 0.3f;	// ��С�Ŵ����
const float MAX_FACTOR = 10;	// ���Ŵ����
const UINT ID_OVERVIEW_WND = 1235;
const UINT ID_MICROSCOPE_WND = 1236;
const UINT REPORT_ITEM_OFFSET = 50;
const float g_fUnfoldHeight = 360;	// machao add on 2013-08-8

#define MOVEPROBE 4
// ������
#define PI 3.1415926535
#define FILE_DEBUG 0
//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0) 

extern BOOL g_bDrawRealTime; //ʵʱ����
extern BOOL g_bPictureReadFile;	//��ȡ�ļ�����
int g_nSCanMaxXLength = 1200;
int g_nSCanMaxYLength = 800;
int GRID_X_NUM = 1200;
int GRID_Y_NUM = 800;
BOOL gbCScanOnSizeFlag = FALSE;
int CCScanView::m_nObjectCount = 0;

const int MARGIN_X = 200;
const int MARGIN_Y = 300;
// Song Chenguang add start on 2013-08-13
#if _HAFEI
	float g_fDBZeroAmp = 800.0f;
#else
	float g_fDBZeroAmp = 100.0f;
#endif
// Song Chenguang add end on 2013-08-13
const int GATE_STRUCT_SIZE = sizeof(BOOL) + sizeof(double)*4 + sizeof(COLORREF) + sizeof(int) + 4; // Song chenguang add on 2013-12-19 բ�����С

IMPLEMENT_DYNCREATE(CCScanView, CScrollView)


CCScanView::CCScanView()
: m_fXMove(0)
, m_fYMove(0)
, m_nColorMode(1)
//Machao add start on 2013-07-9
, m_bNoiseReduction(FALSE)
, m_bImage(FALSE)
//Machao add end on 2013-07-9
#if _HAFEI
, m_bTailDecreasePipe(FALSE)	// machao add on 2013-08-12
#endif
// machao add start on 2013-08-16
, m_bCompensateRange(FALSE)	// machao add on 2013-08-16
, m_fWStart(0)
, m_fWStop(0)
, m_fZStart(0)
, m_fZStop(0)
// machao add end on 2013-08-16
, m_bMouseGainRangeSelect(FALSE)	// machao add on 2013-08-23
, m_bDrawGainRange(FALSE)	// machao add on 2013-08-23
, m_nAllCount(0)	// machao add on 2013-09-3
, m_bEdgePoint(TRUE)	// machao add on 2013-09-18
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Enter");
	m_bISRotateCenterExist = FALSE;     //yachang.huang add on 2012-8-14 
	m_ptRotateCenter = 0;  //yachang.huang add on 2012-8-14
	m_fRateX = 1.0f;
	m_fRateY = 1.0f;
	//m_eType = NORMAL;
	m_fScaleFactor = 1.0f;
	m_bRota = FALSE;
	m_fRotaAngle = 0;     //yachang.huang modify on 2012-8-14
//	m_nXRange = 100;
//	m_nYRange = 100;
//	m_Size = CSize(100, 100);
	m_bOverViewDisplay = FALSE;
	m_pOverViewWnd = NULL;
	m_CScanSize = CSize(100, 100);

	m_bMicroscopeDisplay = FALSE;
	m_pMicroScopeWnd = NULL;

	m_bMeasureOn = FALSE;
	m_bTracking = FALSE;
	m_eMeasureType = eRECT;

	m_nGrayMax = 255;
	m_nGrayMin = 0;
	m_nValueMode = 0;
	m_nGateIndex = 0;
	m_nAmpFeatureIndex = 0;
	m_nTimeFeatureIndex = 0;
	m_nDrawType = 0;
	m_fBoundMin = 0;
	// WangQianfeng add start on 2012-11-21
	m_nDefectNum = -1;
	// WangQianfeng add end on 2012-11-21
	//m_fBoundMin = 100;
	m_fBoundMax = 100; //zhaoli  2011 11 04
	m_cspen[0].CreatePen(PS_SOLID,1,RGB(0,255,0));//
	m_cspen[1].CreatePen(PS_SOLID,1,RGB(0,240,14));//
	m_cspen[2].CreatePen(PS_SOLID,1,RGB(0,209,41));//
	m_cspen[3].CreatePen(PS_SOLID,1,RGB(0,191,68));//
	m_cspen[4].CreatePen(PS_SOLID,1,RGB(0,160,91));//
	m_cspen[5].CreatePen(PS_SOLID,1,RGB(0,132,118));//
	m_cspen[6].CreatePen(PS_SOLID,1,RGB(0,107,135));//
	m_cspen[7].CreatePen(PS_SOLID,1,RGB(0,74,179));//
	m_cspen[8].CreatePen(PS_SOLID,1,RGB(0,48,218));//
	m_cspen[9].CreatePen(PS_SOLID,1,RGB(0,35,213));//
	m_cspen[10].CreatePen(PS_SOLID,1,RGB(0,0,255));//
	m_cspen[11].CreatePen(PS_SOLID,1,RGB(20,0,235));//
	m_cspen[12].CreatePen(PS_SOLID,1,RGB(32,0,210));//
	m_cspen[13].CreatePen(PS_SOLID,1,RGB(70,0,186));//
	m_cspen[14].CreatePen(PS_SOLID,1,RGB(97,0,167));//
	m_cspen[15].CreatePen(PS_SOLID,1,RGB(130,0,124));//
	m_cspen[16].CreatePen(PS_SOLID,1,RGB(143,0,106));//
	m_cspen[17].CreatePen(PS_SOLID,1,RGB(170,0,88));//
	m_cspen[18].CreatePen(PS_SOLID,1,RGB(197,0,46));//
	m_cspen[19].CreatePen(PS_SOLID,1,RGB(226,0,38));//
	m_cspen[20].CreatePen(PS_SOLID,1,RGB(255,0,0));//
	m_cspen[21].CreatePen(PS_SOLID,1,RGB(0,0,0));//��ɫ ������
	for(int i = 0; i < GATE_NUM; i++)
	{
		m_fGateThreshold[i] = 0;
	}
	m_clrPlatte[0] = RGB(0,255,0);//
	m_clrPlatte[1] = RGB(0,240,14);//
	m_clrPlatte[2] = RGB(0,209,41);//
	m_clrPlatte[3] = RGB(0,191,68);//
	m_clrPlatte[4] = RGB(0,160,91);//
	m_clrPlatte[5] = RGB(0,132,118);//
	m_clrPlatte[6] = RGB(0,107,135);//
	m_clrPlatte[7] = RGB(0,74,179);//
	m_clrPlatte[8] = RGB(0,48,218);//
	m_clrPlatte[9] = RGB(0,35,213);//
	m_clrPlatte[10] = RGB(0,0,255);//
	m_clrPlatte[11] = RGB(20,0,235);//
	m_clrPlatte[12] = RGB(32,0,210);//
	m_clrPlatte[13] = RGB(70,0,186);//
	m_clrPlatte[14] = RGB(97,0,167);//
	m_clrPlatte[15] = RGB(130,0,124);//
	m_clrPlatte[16] = RGB(143,0,106);//
	m_clrPlatte[17] = RGB(170,0,88);//
	m_clrPlatte[18] = RGB(197,0,46);//
	m_clrPlatte[19] = RGB(226,0,38);//
	m_clrPlatte[20] = RGB(255,0,0);//

	// Song Chenguang add start on 2013-07-12
	::ZeroMemory(m_arrayPlatte, sizeof(PLATTE) * PLATTE_NUM);
	m_nPlatteSize = 0;
#if _HAFEI
	m_nPlatteSize = 5;
	m_arrayPlatte[0].color = RGB(0, 0, 0);
	m_arrayPlatte[0].fMaxPercent = 100;
	m_arrayPlatte[0].fMinPercent = 80;
	m_arrayPlatte[1].color = RGB(64, 64, 64);
	m_arrayPlatte[1].fMaxPercent = 80;
	m_arrayPlatte[1].fMinPercent = 60;
	m_arrayPlatte[0].color = RGB(128, 128, 128);
	m_arrayPlatte[0].fMaxPercent = 60;
	m_arrayPlatte[0].fMinPercent = 40;
	m_arrayPlatte[0].color = RGB(192, 192, 192);
	m_arrayPlatte[0].fMaxPercent = 40;
	m_arrayPlatte[0].fMinPercent = 20;
	m_arrayPlatte[0].color = RGB(255, 255, 255);
	m_arrayPlatte[0].fMaxPercent = 20;
	m_arrayPlatte[0].fMinPercent = 0;
#endif
	// Song Chenguang add start on 2013-07-12

	m_bIsDrawFlag = FALSE;
	m_bDrawSurfaceFlag = FALSE;

	for(int i = 0; i < 21; i++)
	{
		m_fSurfacePixeRate[i] = 0;
	}

	for(int i = 0; i < 21; i++)
	{
		m_fHanHeRate[i] = 0;
	}
	
	m_nMemXLENGTH = 1024;
	m_nMemYLENGTH = 1024;

	m_pPixel = new float*[m_nMemXLENGTH];
	// WangQianfeng add start on 2012-11-06
	if (NULL == m_pPixel)
	{
		MessageBox(_T("CCScanView::CCScanView(),m_pPixel = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		m_pPixel[i] = new float[m_nMemYLENGTH];
		// WangQianfeng add start on 2012-11-06
		if (NULL == m_pPixel[i])
		{
			MessageBox(_T("CCScanView::CCScanView(),m_pPixel[i] = null"), _T("����"), MB_OK|MB_ICONERROR);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
			return;
		}
		// WangQianfeng add end on 2012-11-06
		memset(m_pPixel[i], 0, m_nMemYLENGTH*sizeof(float));
	}	
	//���ص�����������
	m_pPixelCount = new int*[m_nMemXLENGTH];
	// WangQianfeng add start on 2012-11-06
	if (NULL == m_pPixel)
	{
		MessageBox(_T("CCScanView::CCScanView(),m_pPixelCount = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		m_pPixelCount[i] = new int[m_nMemYLENGTH];
		// WangQianfeng add start on 2012-11-06
		if (NULL == m_pPixelCount[i])
		{
			MessageBox(_T("CCScanView::CCScanView(),m_pPixel[i] = null"), _T("����"), MB_OK|MB_ICONERROR);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
			return;
		}
		// WangQianfeng add end on 2012-11-06
		memset(m_pPixelCount[i], 0, m_nMemYLENGTH*sizeof(int));
	}

	// machao add start on 2013-08-6
	m_pfSrcData = new float*[m_nMemXLENGTH];
	if (NULL == m_pfSrcData)
	{
		MessageBox(_T("CCScanView::CCScanView(),m_pfSrcData = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
		return;
	}
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		m_pfSrcData[i] = new float[m_nMemYLENGTH];
		if (NULL == m_pfSrcData[i])
		{
			MessageBox(_T("CCScanView::CCScanView(),m_pfSrcData[i] = null"), _T("����"), MB_OK|MB_ICONERROR);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
			return;
		}
		memset(m_pfSrcData[i], 0, m_nMemYLENGTH*sizeof(float));
	}
	// machao add end on 2013-08-6

	m_fXLength = 0;
	m_fYLength = 0;
	m_fGridX = 1.0f;
	m_fGridY = 1.0f;
	m_dXStart = 0;
	m_dYStart = 0;
	m_dZStart = 0;
	m_dWStart = 0;	// Song Chenguang add on 2013-05-04
	m_dXEnd = m_nMemXLENGTH;
	m_dYEnd = m_nMemYLENGTH;
	m_dZEnd = 0;
	m_bMouseLeave = FALSE;
	m_nEnterCount = 0;
	m_fZuobiaoXmin = 0;
	m_fZuobiaoXmax = 0;
	m_fZuobiaoYmax = 100;
	m_fZuobiaoYmin = 0;
	m_fYMoveForZuoBiao = 0;
	m_nOnSizeCount = 0;
	m_nObjectCount++;
	m_nObjectNum = m_nObjectCount;
	
	m_dPipeDiameter = DRIVE_WHEEL_DIAMETER;
	m_nPipeXMode =0;
	m_nAxisSelect = 0;
	m_SelArea=0;

	InitReportItems();

	for(int i = 0; i < 7; i++)
	{
		m_bFileFeatureFlag[i] = FALSE;
	}
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		m_nFileGateNum[i] = 1;
		for(int j = 0; j < GATE_NUM; j++)
		{
			m_bFileGateFlag[i][j] = FALSE;
		}
	}	
	m_nFileFeatureNum = 1;
	m_strPictureMode = _T("");
	m_strPictureGate = _T("");
	m_fThick = 0.0f;
	m_bIsDrawSyncFlag = FALSE;

//	ZeroMemory(m_Poly,sizeof(m_Poly));  //����ζ������������ʼ��  yachang.huang add start on 2012-6-1
	for (int i=0; i<POLY_VERTEX_NUM; i++)
	{
		m_Poly[i] = CPoint(0, 0);
	}
	m_nPolyNum = -1;  //����ζ�������������ʼ��  yachang.huang add start on 2012-6-1
    m_fPolygonArea = 0.0;  //����������ʼ��  yachang.huang add start on 2012-6-1


	m_pHRuler = new CRulerWnd; //yachang.huang add on 2012-8-14
	m_pVRuler = new CRulerWnd; //yachang.huang add on 2012-8-14
	m_bEnterFlag = FALSE;
	m_bDblClk = FALSE;
	m_cscan_back_color = RGB(180,180,180);
	m_bGuanLianFlag = FALSE;
	m_bNotEditFlag = FALSE;
	m_dbFlawArea = 0;
	m_strNotePath = _T("");
	// wangbingfu add start on 2013-01-09
	memset(m_nPipeChannelSelect, 0, sizeof(m_nPipeChannelSelect));
	m_nCScanSelect = 0;
	bRealTimeDrawMode = 0;
	// wangbingfu add end on 2013-01-09
	// Song Chenguang add start on 2013-06-27
	m_bSelSurfaceCenter = FALSE;
	m_bSurfaceCenterSelected = FALSE;
	m_bSelSurfaceEdge = FALSE;

	m_pcCustumFlawBuffer = NULL;
	m_lCustumFlawBufferSize = 0;
	m_fCenterDB = 0.0f;
	m_fEdgeDB = 0.0f;
	// Song Chenguang add end on 2013-06-27

	m_pPartGainCompensateDlg = NULL;
	bFreePictureMemoy = TRUE;//Ĭ�ϻ�ͼǰ������ͼ����ڴ�  yachang.huang add on 2014-04-24
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
}

CCScanView::~CCScanView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::~CCScanView"),"Enter");
	if(m_pOverViewWnd != NULL)
	{
		delete m_pOverViewWnd;
		m_pOverViewWnd = NULL;
	}

	if(m_pMicroScopeWnd != NULL)
	{
		delete m_pMicroScopeWnd;
		m_pMicroScopeWnd = NULL;
	}
	
	// wangbingfu modify start on 2012-11-03
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		if(m_pPixel[i] != NULL)
		{
			delete[] m_pPixel[i];
			m_pPixel[i] = NULL;
		}
	}
	if(m_pPixel != NULL)
	{
		delete[] m_pPixel;
		m_pPixel = NULL;
	}
	
	// machao add start on 2013-07-25
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		if(m_pfSrcData[i] != NULL)
		{
			delete[] m_pfSrcData[i];
			m_pfSrcData[i] = NULL;
		}
	}
	if(m_pfSrcData != NULL)
	{
		delete[] m_pfSrcData;
		m_pfSrcData = NULL;
	}
	// machao add end on 2013-07-25

	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		if(m_pPixelCount[i] != NULL)
		{
			delete[] m_pPixelCount[i];
			m_pPixelCount[i] = NULL;
		}
	}
	if(m_pPixelCount != NULL)
	{
		delete[] m_pPixelCount;
		m_pPixelCount = NULL;
	}
	// wangbingfu modify end on 2012-11-03

	//yachang.huang add start on 2012-8-14
	if (m_pHRuler != NULL)
	{
		delete m_pHRuler;
		m_pHRuler = NULL;
	}
	if (m_pVRuler != NULL)
	{
		delete m_pVRuler;
		m_pVRuler = NULL;
	}
	// machao add start on 2013-09-6
	if (m_pfCustomHanheRate != NULL)
	{
		delete m_pfCustomHanheRate;
		m_pfCustomHanheRate = NULL;
	}
	// machao add end on 2013-09-6
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::~CCScanView"),"Leave");
	//yachang.huang add end on 2012-8-14
	// Song Chenguang add start on 2012-11-05
	m_MemDC.SelectObject(m_pOldBitmap);
	DeleteObject(m_Membitmap);
	// Song Chenguang add start on 2012-11-05
}

void CCScanView::InitReportItems()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InitReportItems"),"Enter");
// 	for (int i=0; i<30; i++)
// 	{
// 	m_S1_ReportItems[i].strKey = _T (" ");
// 	m_S2_ReportItems[i].strKey = _T(" ");
// 	m_S3_ReportItems[i].strKey = _T(" ");
// 	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InitReportItems"),"Leave");
}

BEGIN_MESSAGE_MAP(CCScanView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_ROTATE_IMAGE, &CCScanView::OnRotateImage)
	ON_WM_DESTROY()
//	ON_COMMAND(ID_C_SCAN_OVERVIEW, &CCScanView::OnCScanOverview)
//	ON_UPDATE_COMMAND_UI(ID_C_SCAN_OVERVIEW, &CCScanView::OnUpdateCScanOverview)
	ON_MESSAGE(WM_MOVE_OVERVIEW, &CCScanView::OnMoveOverView)
	ON_COMMAND(ID_CSCAN_SMOOTH, &CCScanView::OnCscanSmooth)
	ON_COMMAND(ID_CSCAN_MF, &CCScanView::OnCscanMf)
	ON_COMMAND(ID_C_SCAN_MICROSCOPE, &CCScanView::OnCScanMicroscope)
	ON_UPDATE_COMMAND_UI(ID_C_SCAN_MICROSCOPE, &CCScanView::OnUpdateCScanMicroscope)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MEASURE_ON_OFF, &CCScanView::OnMeasureOnOff)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_ON_OFF, &CCScanView::OnUpdateMeasureOnOff)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_RULER_MEASURE, &CCScanView::OnRulerMeasure)
	ON_COMMAND(ID_TRIANGLE_MEASURE, &CCScanView::OnRectMeasure)
	ON_COMMAND(ID_ELLIPSE_MEASURE, &CCScanView::OnEllipseMeasure)
	ON_COMMAND(ID_POLYGON_MEASURE, &CCScanView::OnPolygonMeasure)      //yachang.huang add start on 2012-5-31
	ON_UPDATE_COMMAND_UI(ID_RULER_MEASURE, &CCScanView::OnUpdateRulerMeasure)
	ON_UPDATE_COMMAND_UI(ID_TRIANGLE_MEASURE, &CCScanView::OnUpdateTriangleMeasure)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE_MEASURE, &CCScanView::OnUpdateEllipseMeasure)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_MEASURE, &CCScanView::OnUpdatePolygonMeasure) //yachang.huang add start on 2012-5-31
	ON_MESSAGE(WM_CSCAN_DRAW, &CCScanView::DrawCScanRealTime)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()  // yachang.huang add on 2012-8-16
	ON_UPDATE_COMMAND_UI(ID_ROTATE_IMAGE,OnUpdateIDMROTATE)  // yachang.huang add on 2012-8-16
	ON_WM_TIMER()
	ON_COMMAND(ID_NOTE_EDIT, &CCScanView::OnNoteEdit)
	ON_COMMAND(ID_NOTE_DELETE, &CCScanView::OnNoteDelete)
	ON_COMMAND(ID_SEL_SURFACE_CENTER, &CCScanView::OnSelSurfaceCenter)
	ON_COMMAND(ID_SEL_SURFACE_EDGE, &CCScanView::OnSelSurfaceEdge)
	ON_COMMAND(ID_CUSTUM_SURFACE_DEL, &CCScanView::OnCustumSurfaceDel)
	ON_COMMAND(ID_NOISE_REDUCTION, &CCScanView::OnNoiseReduction)	//machao add on 2013-07-15
	ON_UPDATE_COMMAND_UI(ID_SEL_SURFACE_EDGE, &CCScanView::OnUpdateSelSurfaceEdge)
	ON_UPDATE_COMMAND_UI(ID_NOISE_REDUCTION, &CCScanView::OnUpdateNoiseReduction)	//machao add on 2013-07-22
#if _HAFEI	
	ON_COMMAND(IDC_TAIL_DECREASE_PIPE_UNFOLD, &CCScanView::OnTailDecreasePipeUnfold)	// machao add on 2013-08-12
	ON_UPDATE_COMMAND_UI(IDC_TAIL_DECREASE_PIPE_UNFOLD, &CCScanView::OnUpdateTailDecreasePipeUnfold)	// machao add on 2013-08-12
#endif
	ON_COMMAND(ID_PART_GAIN_COMPENSATE_SITE, &CCScanView::OnPartGainCompensateSite)
	ON_MESSAGE (WM_USER_DIALOG_DESTROYED, OnDialogDestroyed)	// machao add on 2013-08-16
END_MESSAGE_MAP()

BOOL CCScanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPreparePrinting"),"Enter");
	// TODO:  ���� DoPreparePrinting �Ե��á���ӡ���Ի���
	//pInfo->SetMaxPage(4);
	BOOL bRet = DoPreparePrinting(pInfo);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPreparePrinting"),"Leave");	
	return bRet;
}

void CCScanView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnBeginPrinting"),"Enter");
	// TODO: �ڴ����ר�ô����/����û���   /**    20120326
	// ȷ��ҳ��
	CUltrasonicImmersedTestDoc* pDoc = GetDocument(); 

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;	//�ַ��߶�
	int nPrintXRange = pDC->GetDeviceCaps(HORZRES);
	int nPrintYRange = pDC->GetDeviceCaps(VERTRES);
	GetPrintMessage();//��.ini�ж�ȡ����
	
	//ȷ�����ҳ��
	int nDrawWidth = nPrintXRange - MARGIN_X * 2;
	int nDrawHeight = nPrintYRange -  MARGIN_Y * 2;

	m_uCScanLength = static_cast<UINT>(nDrawWidth / ( m_dXEnd - m_dXStart ) * ( m_dYEnd - m_dYStart ) + nDrawWidth / 10);
	m_uMaxPageNum=((MARGIN_Y + nDrawWidth / 10 + m_uCScanLength + cy * ( m_nS3_NUM / 2 + m_nS2_NUM / 2 + m_nS1_NUM / 2 + m_nFlawPrintRow_NUM + 3 )) + m_nFlawPrintRow_NUM * (nPrintXRange / 2 + 2 * (tm.tmHeight +tm.tmExternalLeading + 10)) + nDrawHeight - 1 ) /nDrawHeight;

	m_uMaxPageNum = max(1,m_uMaxPageNum);
	pInfo->SetMaxPage(m_uMaxPageNum);
	CScrollView::OnBeginPrinting(pDC, pInfo);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnBeginPrinting"),"Leave");
}

void CCScanView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnEndPrintPreview"),"Enter");
	// TODO: �ڴ����ר�ô����/����û���

	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnEndPrintPreview"),"Leave");
}

// CCScanView drawing

void CCScanView::OnDraw(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnDraw"),"Enter");
	if(m_nOnSizeCount == 0)
	{
		BOOL  bRet = m_MemDC.CreateCompatibleDC(pDC);
		if(!bRet)
		{
			MessageBox(_T("DC���洴��ʧ�ܣ�"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnDraw"),"Leave");
			return;
		}

		m_CScanSize = CSize(m_nMemXLENGTH, m_nMemYLENGTH);

		bRet = m_Membitmap.CreateCompatibleBitmap(pDC, m_CScanSize.cx, m_CScanSize.cy);
		if(!bRet)
		{
			MessageBox(_T("Membmp����ʧ�ܣ�"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnDraw"),"Leave");
			return;
		}

		m_pOldBitmap = m_MemDC.SelectObject(&m_Membitmap);
		
		m_MemDC.FillSolidRect(0, 0, m_CScanSize.cx, m_CScanSize.cy, m_cscan_back_color);
	}
	m_nOnSizeCount++;
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	// TODO: add draw code here
	// ����Cɨ����
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	m_pCScanParamView->UpdateData(TRUE);
	m_fGateThreshold[0] = m_pCScanParamView->m_fGate1Threshold;
	m_fGateThreshold[1] = m_pCScanParamView->m_fGate2Threshold;
	m_fGateThreshold[2] = m_pCScanParamView->m_fGate3Threshold;
	m_fGateThreshold[3] = m_pCScanParamView->m_fGate4Threshold;
	m_nColorMode = m_pCScanParamView->m_nColorMode;
	m_nValueMode = m_pCScanParamView->m_nValueMode;
	m_nGateIndex = m_pCScanParamView->m_nGateIndex;
	m_nDrawType = m_pCScanParamView->m_nDrawType;
	m_fBoundMin = m_pCScanParamView->m_fBoundMin;
	m_fBoundMax = m_pCScanParamView->m_fBoundMax;
	m_nAmpFeatureIndex = m_pCScanParamView->m_nAmpFeatureIndex;
	m_nTimeFeatureIndex = m_pCScanParamView->m_nTimeFeatureIndex;
	m_nPipeXMode = m_pCScanParamView->m_nPipeXMode;
	
	CRect rect;
	GetClientRect(&rect);
	float fRate = (float)rect.Width()/rect.Height();

	if (m_bRota && m_bISRotateCenterExist)
	{
		RotateImage(pDC,m_fRotaAngle,m_ptRotateCenter);
		m_ptObliqueStartPoint = 0;
		m_ptObliqueEndPoint = 0;
		m_ptRotateCenter = 0;
		m_bISRotateCenterExist = FALSE;
	}

	DistributeRegion(pDC,rect,m_rectAngle, m_rectCScan, m_rectScaleX, m_rectScaleY, m_rectGrid);

	int nRectWidth  = 0;
	int nRectHeight = 0;
	
	// Song Chenguang modify start on 2013-07-10
//	if(g_bDrawRealTime)	//ʵʱCɨ
	if(!m_pCScanParamView->m_bRealScale)	// ��Ϊ�������
	// Song Chenguang modify end on 2013-07-10
	{
		nRectWidth = m_rectGrid.Width();
		nRectHeight = m_rectGrid.Height();
	}
	else
	{
		nRectWidth = m_rectCScan.Width();
		nRectHeight = m_rectCScan.Height();
	}

	// machao add start on 2013-11-27
	if (m_nAxisSelect == 5)
	{
		if (m_nPipeXMode == 0)
		{
			m_dXEnd = 360;
		}
		else
		{
			m_dXEnd = PI * m_dPipeDiameter;
		}
	}
	// machao add end on 2013-11-27
	float fXLength = fabs(static_cast<float>(m_dXEnd - m_dXStart));	// ����X�᳤��
	float fYLength = fabs(static_cast<float>(m_dYEnd - m_dYStart)); // ����Y�᳤��
	int nXLength = static_cast<int>(nRectWidth * m_fScaleFactor);	// ����X���س���
	int nYLength = static_cast<int>(nRectHeight * m_fScaleFactor);	// ����Y���س���

	int nMemDCXLength = int(fXLength / m_fGridX);	//MemDC��������X���س���
	int nMemDCYLength = int(fYLength / m_fGridY);	//MemDC��������Y���س���
 
	//�ı�ԭ��,Cɨ�������µ�
	pDC->SetViewportOrg(m_rectGrid.left, m_rectGrid.top);
	//Machao modify start on 2013-07-4
//	pDC->FillSolidRect(0, 0, m_rectGrid.Width(), m_rectGrid.Height(), m_cscan_back_color);
	pDC->FillSolidRect(0, 0, m_rectGrid.Width(), m_rectGrid.Height(), m_pCScanParamView->m_clrBack2);
	//Machao modify end on 2013-07-4
	m_MemDC.SetViewportOrg(0, m_nMemYLENGTH - nMemDCYLength);
	
	// ��X����
	m_fZuobiaoXmin = m_fXMove;
	m_fZuobiaoXmax = 0;
//	m_fZuobiaoYmax = m_fYMove + fabs(m_dYEnd - m_dYStart);
	m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart));
	m_fZuobiaoYmin = 0;

	// ��ת
 	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
	{
//		if(g_bDrawRealTime) //if(g_bDrawRealTime) ��ת  wangbingfu modify at 2012-1-6
		if(!m_pCScanParamView->m_bRealScale)	// Song Chenguang modify on 2013-07-19
		{
			if(m_nPipeXMode == 0) // �Ƕ�
			{
				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(360 - m_dYStart));
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor;
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(360 - m_dYStart)) / m_fScaleFactor;
			}
			else	// �ܳ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor;
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs( static_cast<float>(PI * m_dPipeDiameter - m_dYStart)) / m_fScaleFactor;
			}			
		}
		else
		{
			if(m_nPipeXMode == 0) // �Ƕ�
			{
				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(360 - m_dYStart));
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor* ((float)m_rectGrid.Width() / m_rectCScan.Width());
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(360 - m_dYStart)) / m_fScaleFactor* ((float)m_rectGrid.Height() / m_rectCScan.Height());
			}
			else	// �ܳ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor* ((float)m_rectGrid.Width() / m_rectCScan.Width());
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(PI * m_dPipeDiameter - m_dYStart)) / m_fScaleFactor* ((float)m_rectGrid.Height() / m_rectCScan.Height());
			}		
		}
	}
	// Song Chenguang add start on 2013-05-04
	else if(m_nAxisSelect == 5)	
	{
//		if(g_bDrawRealTime)
		if(!m_pCScanParamView->m_bRealScale)	// Song Chenguang modify on 2013-07-19
		{
	// machao modify start on 2013-11-24
			if(m_nPipeXMode == 0) // �Ƕ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(360 - m_dXStart));
				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor;
				m_fZuobiaoXmin = m_fZuobiaoXmax - fabs(static_cast<float>(360 - m_dXStart)) / m_fScaleFactor;
			}
			else	// �ܳ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(PI * m_dPipeDiameter - m_dXStart));
				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor;
				m_fZuobiaoXmin = m_fZuobiaoXmax - fabs(static_cast<float>(PI * m_dPipeDiameter - m_dXStart)) / m_fScaleFactor;
// 				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor;
// 				m_fZuobiaoXmin = m_fZuobiaoXmax - fabs( static_cast<float>(PI * m_dPipeDiameter - m_dXStart)) / m_fScaleFactor;
			}			
		}
		else
		{
			if(m_nPipeXMode == 0) // �Ƕ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(360 - m_dXStart)) / m_fScaleFactor * ((float)m_rectGrid.Width() / m_rectCScan.Width());
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor * ((float)m_rectGrid.Height() / m_rectCScan.Height());
				
//				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor * ((float)m_rectGrid.Height() / m_rectCScan.Height());
//				m_fZuobiaoXmin = m_fXMove;
			}
			else	// �ܳ�
			{
				m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(PI * m_dPipeDiameter - m_dXStart)) / m_fScaleFactor * ((float)m_rectGrid.Width() / m_rectCScan.Width());
				m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor * ((float)m_rectGrid.Height() / m_rectCScan.Height());
// 				m_fZuobiaoYmax = m_fYMoveForZuoBiao + fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor* ((float)m_rectGrid.Height() / m_rectCScan.Height());
// 				m_fZuobiaoXmin = m_fZuobiaoXmax - fabs(static_cast<float>(PI * m_dPipeDiameter - m_dXStart)) / m_fScaleFactor* ((float)m_rectGrid.Width() / m_rectCScan.Width());
			}
	// machao modify end on 2013-11-24
		}
	}
	// Song Chenguang add end on 2013-05-04
	else
	{
//		if(g_bDrawRealTime) //if(g_bDrawRealTime) ��ת  wangbingfu modify at 2012-1-6
		if(!m_pCScanParamView->m_bRealScale)	// Song Chenguang modify on 2013-07-15
		{
			m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor;
			m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor;
		}
		else
		{
			m_fZuobiaoXmax = m_fXMove + fabs(static_cast<float>(m_dXEnd - m_dXStart)) / m_fScaleFactor* ((float)m_rectGrid.Width() / m_rectCScan.Width());
			m_fZuobiaoYmin = m_fZuobiaoYmax - fabs(static_cast<float>(m_dYEnd - m_dYStart)) / m_fScaleFactor* ((float)m_rectGrid.Height() / m_rectCScan.Height());
		}
	}
	
	
	float fXStart = static_cast<float>(m_dXStart - m_fXMove);
	float fYStart = static_cast<float>(m_dYStart - m_fYMove);
	int nXStart = static_cast<int>(fXStart / (m_fZuobiaoXmax - m_fZuobiaoXmin) * m_rectGrid.Width());
	int nYStart = static_cast<int>(fYStart / (m_fZuobiaoYmax - m_fZuobiaoYmin) * m_rectGrid.Height());

	pDC->StretchBlt(nXStart, nYStart, nXLength, nYLength, &m_MemDC, 
		0, 0, nMemDCXLength, nMemDCYLength, SRCCOPY);
	
	DrawCScanBackGround(pDC, rect, pDoc->m_lstFlawData[g_dwActiveChannel]);

	if (m_bIsDrawFlag)
	{
	//	DrawDlbFlg(pDC,m_DlbPoint);
		m_bIsDrawFlag = FALSE;
	}
	if (m_bIsDrawSyncFlag)
	{
		CPoint point = Logical2Client(m_probePoint);
		DrawSyncDlbFlg(pDC,point);
		m_bIsDrawSyncFlag = FALSE;
	}

	//�����˳�����˵�
	if(m_pCScanParamView->m_pSurfaceDeatailDlg != NULL)
	{
		CPoint point = Logical2Client(m_stLogicalFlaw);
		DrawDlbFlg(pDC,point);
	}
	if(m_nEnterCount != 0)
	{
		if(!m_bDblClk)
		{
			InvertRegion(pDC, m_ptCurPos, 0, eLINE);
		}
	}
	m_bDblClk = FALSE;

	if(m_pCScanParamView->m_bDisplayProbe)
	{
		InvertRegion(pDC, m_ptProbeCurPos, 0, eLINE);
	}
#if _HAFEI
	// machao add start on 2013-08-9
	// β����ʾ��ͼ
	if (m_bTailDecreasePipe)
	{
		DrawTailDecreasePipeUnfoldMap(pDC);
	}
	// machao add end on 2013-08-9
#endif
	// machao add start on 2013-08-15
	// �ֲ����油��
	if (m_bCompensateRange)
	{
		DrawPartGainCompensateRange(pDC, m_fWStart, m_fZStart, m_fWStop, m_fZStop);
	}
	// machao add end on 2013-08-15
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnDraw"),"Leave");
}

void CCScanView::DrawCScanBackGround(CDC* pDC, const CRect& rect, CList<CFeatureInfo, CFeatureInfo&>& lstFlawData)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCScanBackGround"),"Enter");
	pDC->SetViewportOrg(0, 0);
	
	//�����������հ�����
// 	FillSpace(pDC,CRect(rect.left,rect.bottom-CSCAN_SCALE_Y_LENGTH,CSCAN_SCALE_X_LENGTH,rect.bottom));  //yachang.huang rum on 2012-8-16

	// ��ͼ������߿�
	pDC->SelectObject(&m_cspen[21]);
	pDC->Rectangle(&m_rectAngle);	
	
	float fXmin;
	float fXmax;
	float fYmax;
	float fYmin;
	
	// ��X����
	if (m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0 )
	{
		fXmin = m_fZuobiaoXmin - m_Offset_Coordinate.x;
		fXmax = m_fZuobiaoXmax - m_Offset_Coordinate.x;
		fYmax = m_fZuobiaoYmax - m_Offset_Coordinate.y;
		fYmin = m_fZuobiaoYmin - m_Offset_Coordinate.y;
	} 
	else
	{
		fXmin = m_fZuobiaoXmin;
		fXmax = m_fZuobiaoXmax;
		fYmax = m_fZuobiaoYmax;
		fYmin = m_fZuobiaoYmin;
	}

	m_pHRuler->MoveWindow(&m_rectScaleX);     //yachang.huang add on 2012-8-14  
	m_pVRuler->MoveWindow(&m_rectScaleY);     //yachang.huang add on 2012-8-14 


//yachang.huang rum on 2012-8-16
// 	float fStep  = (fXmax - fXmin) / 10;
// 	DrawScale(pDC, m_rectScaleX, fXmin, fXmax, fStep, 3, TRUE);
// 	// ��Y����
// 	
// 	fStep  = (fYmax - fYmin) / 10;
// 	DrawScale(pDC, m_rectScaleY, fYmin, fYmax, fStep, 4, TRUE);

	// ������
//	DrawGrid(pDC, m_rectGrid);
	
	m_MemDC.SetViewportOrg(0, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCScanBackGround"),"Leave");
}

LRESULT CCScanView::DrawCScanRealTime(WPARAM wParam, LPARAM lParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCScanRealTime"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	// wangbingfu add start on 2012-11-03
	if((CH_FLAWDATA_CSCAN_S*)wParam == NULL)
	{
		MessageBox(_T("CScanView  DrawCScanRealTime(), WPARAM wParam is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawCScanRealTime"),"Leave");
		return 0;
	}
	// wangbingfu add end on 2012-11-03

	CH_FLAWDATA_CSCAN_S *featrueInfo = (CH_FLAWDATA_CSCAN_S*)wParam;
	
	// wangbingfu add start on 2012-11-03
	if((CARD_INFO_t*)lParam == NULL)
	{
		MessageBox(_T("CScanView  DrawCScanRealTime(), LPARAM lParam is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawCScanRealTime"),"Leave");
		return 0;
	}
	// wangbingfu add end on 2012-11-03

	CARD_INFO_t * pCardInfo = (CARD_INFO_t*)lParam;
	
	// wangbingfu add start on 2012-11-03
	if ( ::IsBadReadPtr( pCardInfo, sizeof(CARD_INFO_t) ))
	{
		MessageBox(_T("CScanView  DrawCScanRealTime(), pCardInfo can not read"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawCScanRealTime"),"Leave");
		return 0;
	}
	// wangbingfu add end on 2012-11-03

	CARD_INFO_t info;
	info.nCardIndex = pCardInfo->nCardIndex;
	info.nChannelIndex = pCardInfo->nChannelIndex;
	info.nStartPoint = pCardInfo-> nStartPoint;
	info.nNum = pCardInfo-> nNum;
	
	// wangbingfu modify start on 2012-11-03
	if(pCardInfo != NULL)
	{
		delete pCardInfo;
		pCardInfo = NULL;
	}
	// wangbingfu modify end on 2012-11-03
	// Song Chenguang modify start on 2013-07-15
	CRect rect;
	if(!m_pCScanParamView->m_bRealScale)
		rect = m_rectGrid;
	else
		rect = m_rectCScan;
	
	DrawPictureRealTime(&m_MemDC, rect, featrueInfo, info.nCardIndex, 
						info.nChannelIndex, info.nStartPoint, info.nNum);
	// Song Chenguang modify end on 2013-07-15
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCScanRealTime"),"Leave");
	return 0;
}
// CCScanView diagnostics

#ifdef _DEBUG
void CCScanView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CCScanView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

void CCScanView::DistributeRegion(CDC* pDC,
								  const CRect& rectAll, 
								  CRect& rectAngle,
								  CRect& rectCScan,
								  CRect& rectScaleX,
								  CRect& rectScaleY,
								  CRect& rectGrid)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DistributeRegion"),"Enter");
	// x��������
	POINT XScaleTop;
	POINT XScaleBottom;
	if(pDC->IsPrinting())
	{
		XScaleTop.x = static_cast<LONG>(CSCAN_SCALE_X_LENGTH*m_fRateX);
		XScaleTop.y = static_cast<LONG>(rectAll.bottom - CSCAN_SCALE_Y_LENGTH*m_fRateX);
	}
	else
	{
		XScaleTop.x = CSCAN_SCALE_X_LENGTH;
		XScaleTop.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;

	}
	XScaleBottom.x = rectAll.right;
	XScaleBottom.y = rectAll.bottom;
	rectScaleX.SetRect(XScaleTop,XScaleBottom);

	// y��������
	POINT YScaleTop;
	POINT YScaleBottom;
	YScaleTop.x = rectAll.left;
	YScaleTop.y = rectAll.top;

	if(pDC->IsPrinting())
	{
		YScaleBottom.x = static_cast<LONG>(CSCAN_SCALE_X_LENGTH*m_fRateX);
		YScaleBottom.y = static_cast<LONG>(rectAll.bottom - CSCAN_SCALE_Y_LENGTH*m_fRateX);
	}
	else
	{
		YScaleBottom.x = CSCAN_SCALE_X_LENGTH;
		YScaleBottom.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
	}
	rectScaleY.SetRect(YScaleTop,YScaleBottom);

	// ��������
	POINT GridTop;
	POINT GridBottom;
	GridTop.x = XScaleTop.x;
	GridTop.y = rectAll.top;
	GridBottom.x = rectAll.right;

	GridBottom.y = YScaleBottom.y;
	rectGrid.SetRect(GridTop,GridBottom);

	// Cɨ����
	POINT CScanTop;
	POINT CScanBottom;
	int nCScanYLength = 0;	//Cɨ������
	int nCScanXLength = 0;	//Cɨ���򳤶�
	// machao add start on 2013-11-27
	if (m_nAxisSelect == 5)
	{
		if (m_nPipeXMode == 0)
		{
			m_dXEnd = 360;
		}
		else
		{
			m_dXEnd = PI * m_dPipeDiameter;
		}
	}
	// machao add end on 2013-11-27
	float fXLength = fabs(static_cast<float>(m_dXEnd - m_dXStart));	// ����X�᳤��
	float fYLength = fabs(static_cast<float>(m_dYEnd - m_dYStart)); // ����Y�᳤��
	float X_Y_SCALE = fXLength / fYLength;

	if(rectGrid.Width() > rectGrid.Height() * X_Y_SCALE)
	{
		nCScanYLength = rectGrid.Height();
		nCScanXLength = static_cast<int>(nCScanYLength * X_Y_SCALE);
	}
	else
	{	
		nCScanXLength = rectGrid.Width();
		nCScanYLength = static_cast<int>(nCScanXLength / X_Y_SCALE);
	}

	CScanTop.x = rectGrid.left;
//	CScanTop.y = rectGrid.bottom - nCScanYLength;	// Song Chenguang modified on 2013-05-10
	CScanTop.y = rectGrid.top;

	CScanBottom.x = rectGrid.left + nCScanXLength;
//	CScanBottom.y = rectGrid.bottom;
	CScanBottom.y = rectGrid.top + nCScanYLength;
	rectCScan.SetRect(CScanTop, CScanBottom);
    rectScaleX.left = 0;  //yachang.huang add on 2012-8-15   ˮƽ����������Ϊ0
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DistributeRegion"),"Leave");
}
// ����ʱ���˵�
void CCScanView::DrawFlawPoints(CDC* pDC, const CRect& rect, CList<CFeatureInfo, CFeatureInfo&>& lstFlawData)
{

}

// CCScanView message handlers

void CCScanView::OnSize(UINT nType, int cx, int cy)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnSize"),"Enter");
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

// 	SetScrollSizes(MM_TEXT, m_displaySize);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnSize"),"Leave");
}

BOOL CCScanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseWheel"),"Enter");
	// TODO: Add your message handler code here and/or call default
	// �Ե�ǰ��ͼ���ĵ�Ϊ���ģ����зŴ�
	// ����Cɨ����ͼ����ʾ��Χ
	if(!g_bDrawRealTime)
	{
		CRect rect;
		GetClientRect(&rect);
		CSize csize;
		csize.cx =  rect.right - rect.left;
		csize.cy =  rect.bottom - rect.top;
		if (zDelta > 0)
		{
			//m_fScaleFactor *= (1 + double(zDelta) / (double(WHEEL_DELTA)*10));
			m_fScaleFactor = m_fScaleFactor - 0.2f;
		}
		else
		{
			//m_fScaleFactor /= (1 + double(-1*zDelta) / (double(WHEEL_DELTA)*10));
			m_fScaleFactor = m_fScaleFactor + 0.2f;
		}

		if (m_fScaleFactor < MIN_FACTOR)
		{
			m_fScaleFactor = MIN_FACTOR;
		}
		else if (m_fScaleFactor > MAX_FACTOR)
		{
			m_fScaleFactor = MAX_FACTOR;
		}
		csize.cx = static_cast<LONG>(csize.cx * m_fScaleFactor);
		csize.cy = static_cast<LONG>(csize.cy * m_fScaleFactor);
		SetScrollSizes(MM_TEXT, csize);
		
		//m_Offset_Coordinate.x *= m_fScaleFactor;
		//m_Offset_Coordinate.y *= m_fScaleFactor;

		// �Ŵ���С
		m_displaySize.cx = int(m_layout.cx * m_fScaleFactor);
		m_displaySize.cy = int(m_layout.cy * m_fScaleFactor);

		Invalidate(TRUE);
		
		//ͬ��
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCScanParamPanel* pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		if(pCScanParamView->m_bCscanSync)
		{
			CCScanView* pCScanView = NULL;
			if(m_nObjectNum == 1)
			{
				pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);			
			}
			else
			{
				pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
			}

			pCScanView->GetClientRect(&rect);
			csize.cx =  rect.right - rect.left;
			csize.cy =  rect.bottom - rect.top;
			csize.cx = static_cast<LONG>(csize.cx * m_fScaleFactor);
			csize.cy = static_cast<LONG>(csize.cy * m_fScaleFactor);

			pCScanView->m_fScaleFactor = m_fScaleFactor;
			pCScanView->SetScrollSizes(MM_TEXT, csize);
			pCScanView->m_displaySize = m_displaySize;
			pCScanView->Invalidate(TRUE);
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseWheel"),"Leave");
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CCScanView::DrawGrids(CDC* pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawGrids"),"Enter");
	CPen pen;

	pen.CreatePen(PS_DOT ,1,RGB(255,255,255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//	pDC->SetBkColor(TRANSPARENT);

	//������������
	for(int i = 1;i<5;++i)
	{
		pDC->MoveTo(rect.left,rect.top + static_cast<int>(2.0/10.0*rect.Height()*i));
		pDC->LineTo(rect.right,rect.top + static_cast<int>(2.0/10.0*rect.Height()*i));
	}
	//������������
	for(int i = 1;i< 10;++i)
	{ 
		pDC->MoveTo(static_cast<int>(rect.left + 1.0/10.0*rect.Width()*i),rect.top);
		pDC->LineTo(static_cast<int>(rect.left + 1.0/10.0*rect.Width()*i),rect.bottom);
	}


	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawGrids"),"Leave");
}
void CCScanView::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnInitialUpdate"),"Enter");
	CScrollView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	if(m_nObjectNum == 1)
	{
		m_pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	}
	else
	{
		m_pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	}

	m_layout.cx = int(pApp->m_IniParams.m_info.fXRange);
	m_layout.cy = int(pApp->m_IniParams.m_info.fYRange);

	GRID_X_NUM = m_layout.cx;
	GRID_Y_NUM = m_layout.cy;

	CRect rect;
	GetClientRect(&rect);
	CSize csize;
	csize.cx =  rect.right - rect.left;
	csize.cy =  rect.bottom - rect.top;
	
	m_page.cx = 0;
	m_page.cy = 0;
	m_line.cx = 0;
	m_line.cy = 0;
	SetScrollSizes(MM_TEXT, csize, m_page, m_line);

	m_displaySize = m_layout;

	// ȫ��Ԥ������
	m_pOverViewWnd = new COverViewWnd;
	// WangQianfeng add start on 2012-11-06
	if (NULL == m_pOverViewWnd)
	{
		MessageBox(_T("CCScanView::OnInitialUpdate(),m_pOverViewWnd = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnInitialUpdate"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	m_pOverViewWnd->Create(NULL, _T("OverView"), WS_OVERLAPPEDWINDOW | WS_CHILD, 
		CRect(0, 0, OVERVIEW_WIDTH, OVERVIEW_HEIGHT), this, ID_OVERVIEW_WND);
	m_pOverViewWnd->m_pDCMemory = &m_MemDC;
	m_pOverViewWnd->m_SourceSize = m_CScanSize;

	m_bOverViewDisplay = FALSE;

	// ��΢������
	m_pMicroScopeWnd = new CMicroscopeWnd;
	// WangQianfeng add start on 2012-11-06
	if (NULL == m_pMicroScopeWnd)
	{
		MessageBox(_T("CCScanView::OnInitialUpdate(),m_pMicroScopeWnd = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnInitialUpdate"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	m_pMicroScopeWnd->Create(NULL, _T("Microscope"), WS_CHILD, 
		CRect(0, 0, 100, 100), this, ID_MICROSCOPE_WND);

	m_pMovePath = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);

	m_Offset_Coordinate.x=0;
	m_Offset_Coordinate.y=0;	//-1024;
	m_ToolTip.Create(this,TTS_ALWAYSTIP);
	m_ToolTip.AddTool(this,LPSTR_TEXTCALLBACK);
	m_ToolTip.SetTipBkColor(RGB(0,255,0));
	m_ToolTip.SetDelayTime(1000);
	m_ToolTip.SetMaxTipWidth(200);

	SetTimer(TIMER_DRAW_PROBE_POSITTION, 10, NULL);

	// wangbingfu add start on 2013-01-09
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		m_nPipeChannelSelect[i] = pFrame->m_nPipeChannelSelect[i];
	}
	m_nAxisSelect = pFrame->m_nAxisSelect;
	if(m_nObjectNum == 1)
	{
		m_nCScanSelect = pFrame->m_nCScanSelect[0];
	}
	else if(m_nObjectNum == 2)
	{
		m_nCScanSelect = pFrame->m_nCScanSelect[1];
	}
	else
	{
		;//
	}
	m_fPipeProbeAngle = pFrame->m_fPipeProbeAngle;
	// wangbingfu add end on 2013-01-09
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnInitialUpdate"),"Leave");
}

void CCScanView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPrepareDC"),"Enter");
	// TODO: Add your specialized code here and/or call the base class

	CScrollView::OnPrepareDC(pDC, pInfo);

	if(pDC->IsPrinting()) ///////////////////////////////////////////////////////////////////////////////////////////////
	{
		int nMarginHeight = pInfo->m_rectDraw.Height() / 20;
		int nMarginWidth = pInfo->m_rectDraw.Width() / 20;
		int pageHeight = pDC->GetDeviceCaps(VERTRES);
		int originY=pageHeight*(pInfo->m_nCurPage-1);
		pDC->SetViewportOrg(0,-originY);
	}

      CView::OnPrepareDC(pDC,pInfo);
	  /////////////////////////////////////////////////

	  ///////////////////////////////////////////////
//	if(pInfo != NULL)
//		pDC->SetMapMode(MM_ANISOTROPIC);
/*	int nFullWidth=GetSystemMetrics(SM_CXSCREEN);     
	int nFullHeight=GetSystemMetrics(SM_CYSCREEN); 

	CSize size = CSize(nFullWidth,nFullHeight);
	pDC->SetWindowExt(size);
	//��ӡ��ÿӢ������ص�
	int xlog = pDC->GetDeviceCaps(LOGPIXELSX);
	int ylog = pDC->GetDeviceCaps(LOGPIXELSY);
	float fwidth = xlog/96.0;
	float fheight = ylog/96.0;

	long xext = (long)size.cx*xlog/96.0;
	long yext = (long)size.cy*ylog/96.0;

	pDC->SetViewportExt((int)xext,(int)yext);
*/
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPrepareDC"),"Leave");
}

void CCScanView::OnRotateImage()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRotateImage"),"Enter");
	// TODO: �ڴ���������������
// 	CGeomRotaDlg rotaDlg;
// 	rotaDlg.m_nRotateAngle = m_nRotaAngle;
// 	if (IDOK == rotaDlg.DoModal())
// 	{
// 		m_nRotaAngle =  rotaDlg.m_nRotateAngle;
// 		m_bRota = TRUE;
// 		Invalidate(TRUE);
// 		
// 	}
     //yachang.huang modify start on 2012-8-16
	if (FALSE == m_bRota)
	{
		m_bRota = TRUE;
	}
	else
	{
		m_bRota = FALSE;
		Invalidate(TRUE);
		
	}
	//yachang.huang modify end on 2012-8-16
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRotateImage"),"Leave");
}


void CCScanView::OnDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnDestroy"),"Enter");
	CScrollView::OnDestroy();
	// TODO: Add your message handler code here	
	if (m_pcCustumFlawBuffer != NULL)
	{
		delete []m_pcCustumFlawBuffer;
		m_pcCustumFlawBuffer = NULL;
		m_lCustumFlawBufferSize = 0;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnDestroy"),"Leave");
}

#ifdef _DEBUG
CUltrasonicImmersedTestDoc* CCScanView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasonicImmersedTestDoc)));
	return (CUltrasonicImmersedTestDoc*)m_pDocument;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCScanView::PrintPageHeader(CDC   *pDC,   CPrintInfo   *pInfo) 
{  
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintPageHeader"),"Enter");
	CPoint   point(2500,-600); 
	pDC-> TextOut(point.x,point.y, _T("����������")); 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintPageHeader"),"Leave");
} 
void CCScanView::PrintPageFooter(CDC   *pDC) 
{ 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintPageFooter"),"Enter");
	CPoint   point(1300,-16000); 
	pDC-> TextOut(point.x,point.y,_T("�绰")); 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintPageFooter"),"Leave");

} 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCScanView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPrint"),"Enter");
	// TODO: Add your specialized code here and/or call the base class
//	CRect rectDraw; //�õ�view��Ҫ��ͼ������(����������CScrollView,���Բ�һ��С��GetDrawRect(rectDraw); //��Ļ��С)
//	GetClientRect(&rectDraw);
 
	//PrintPageHeader(pDC,pInfo);//��ӡҳü 
	//PrintPageFooter(pDC);//��ӡҳ�� */

	int nMarginHeight = pInfo->m_rectDraw.Height() / 20;
	int nMarginWidth = pInfo->m_rectDraw.Width() / 20;

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;	//�ַ��߶�
	pDC->TextOut(nMarginWidth, nMarginHeight - cy, _T("ˮ��Cɨ��ͼ"));	//��ӡҳü

	pDC->MoveTo(nMarginWidth, nMarginHeight);
	pDC->LineTo(pInfo->m_rectDraw.right - nMarginWidth, nMarginHeight);

	CRect rectDraw(nMarginWidth, nMarginHeight, pInfo->m_rectDraw.right - nMarginWidth, pInfo->m_rectDraw.bottom - nMarginHeight);
	PrintDraw(pDC, rectDraw);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPrint"),"Leave");
}
/*
// rem by songchenguang 2012-05-03
void CCScanView::PrintDraw(CDC* pDC, const CRect& rect)
{
	int nXPos = rect.left;
	int nYPos = rect.top + 10;

	// �߿�
	CPen penBorder(PS_SOLID, 3, RGB(0, 0, 0));

	CPen *pOldPen = pDC->SelectObject(&penBorder);

	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 1200;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Times New Roman"));

	CFont fontTitle;
	fontTitle.CreatePointFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&fontTitle);

	TEXTMETRIC tm;
	::ZeroMemory(&tm, sizeof(TEXTMETRIC));
	pDC->GetTextMetrics(&tm);
	pDC->SelectObject(pOldFont);
	pDC->GetTextMetrics(&tm);

	UINT nOldAlign = pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextAlign(TA_TOP | TA_LEFT);

	m_nYPos = nYPos;
	PrintMessageTableOne(pDC,rect); //��ӡ�������һ����
	nYPos = m_nYPos;

	// Cɨ��ͼ
	nYPos += (tm.tmHeight +tm.tmExternalLeading);
	CRect rectCScan(rect);
	rectCScan.left += rect.Width() / 5;
	rectCScan.right -= rect.Width() / 5;
	rectCScan.top = nYPos;
	rectCScan.bottom = float(rectCScan.right - rectCScan.left) / (m_dXEnd - m_dXStart) * (m_dYEnd - m_dYStart) + rectCScan.top;

	int nMemDCXLength = int((m_dXEnd - m_dXStart) / m_fGridX);	//MemDC��������X���س���
	int nMemDCYLength = int((m_dYEnd - m_dYStart) / m_fGridY);	//MemDC��������Y���س���
	m_MemDC.SetViewportOrg(0, m_nMemYLENGTH - nMemDCYLength);
	pDC->StretchBlt(rectCScan.left, rectCScan.top, rectCScan.Width(), rectCScan.Height(), &m_MemDC, 0, 0, nMemDCXLength, nMemDCYLength, SRCCOPY);

	CRect YScaleRect;
	YScaleRect.left = rectCScan.left - rect.Width() / 10;
	YScaleRect.right = rectCScan.left;
	YScaleRect.top = rectCScan.top;
	YScaleRect.bottom = rectCScan.bottom;

	CRect XScaleRect;
	XScaleRect.left = rectCScan.left;
	XScaleRect.right = rectCScan.right;
	XScaleRect.top = rectCScan.bottom;
	XScaleRect.bottom = rectCScan.bottom + rect.Width() / 10;

	DrawScale(pDC, XScaleRect, m_dXStart, m_dXEnd, (m_dXEnd - m_dXStart) / 10, 3, TRUE);
	DrawScale(pDC, YScaleRect, m_dYStart, m_dYEnd, (m_dYEnd - m_dYStart) / 10, 4, TRUE);

	nYPos = rectCScan.bottom + 10 + XScaleRect.Height();
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->MoveTo(rect.left, rect.top + 10);
	pDC->LineTo(rect.left, nYPos);
	pDC->MoveTo(rect.right, rect.top + 10);
	pDC->LineTo(rect.right, nYPos);
	pDC->SetTextAlign(nOldAlign);
	pDC->SelectObject(pOldPen);
	pDC->SetTextAlign(TA_TOP | TA_CENTER);

	//����̽�˱��
	m_nYPos = nYPos;
	DrawDefaultResultTable(pDC,rect); //����ӡ���ȱ���������
	nYPos = m_nYPos;

	//̽�˱�ͷ
	pDC->TextOut(rect.left + 4 * tm.tmAveCharWidth, nYPos + 10, _T("���� X"));
	pDC->TextOut(rect.left + 11 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T(" ���� Y"));
	pDC->TextOut(rect.left + 20 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�ײ�λ��"));
	pDC->TextOut(rect.left + 27 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("���"));
	pDC->TextOut(rect.left + 35 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("ȱ�����"));
	pDC->TextOut(rect.left + 43 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET - 20, nYPos + 10, _T("Aͼ"));
	pDC->TextOut(rect.left + 51 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("λ��Σ����"));
	pDC->TextOut(rect.left + 63 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�������"));
	pDC->TextOut(rect.left + 77 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�������"));
	pDC->TextOut(rect.left + 91 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("��ע"));
	nYPos += (tm.tmHeight + tm.tmExternalLeading) + 10;
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nYPos += (tm.tmHeight + tm.tmExternalLeading) + 10;
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nYPos= nYPos - 2 * (tm.tmHeight + tm.tmExternalLeading + 10);
	//������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	// ��ȡ������Ϣ
	VARIANT Result = pCScanParam->m_PlateDrawCtrl.GetPlaneFlawPoints2();
	PlaneFlawHeader stPFH = {0};
	long lLBound = 0;
	long lUBound = 0;
	::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//һά�����±�
	::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//һά�����ϱ�

	char* buf = NULL;
	SafeArrayAccessData(Result.parray,   (void   **)&buf);   

	UINT Size = (lUBound-lLBound+1);	
	int nZoomScale = pCScanParam->m_nZoomScale;
	int nPos = 0;
	int i = 0;
	int ImageNum1 = 0;

	CList<LOGICAL_POINT_t, LOGICAL_POINT_t&> lstSurfacePoint;	// ������С������
	float fMinSurfaceArea = pCScanParam->m_fMinSurfaceArea * nZoomScale * nZoomScale;
	while(nPos + sizeof(PlaneFlawHeader) < Size)
	{	
		memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));		
		if (stPFH.area > fMinSurfaceArea)
		{
			CString strMinX;
			
			strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale);
			CString strMinY;
			strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale);
			CString strMaxX;
			strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale);
			CString strMaxY;
			strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale);
			CString strZ;
			strZ.Format(_T("%.2f"), stPFH.z);
			CString strArea;
			strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));
			CString csStrMark;
			csStrMark.Format(_T("%d"), i);
			double dMaxArea = pCScanParam->m_PlateDrawCtrl.GetBodyMaxQueXianArea();

			//////////////////////////////////////////////////////////////////////////
			LOGICAL_POINT_t pt;
			pt.x = (stPFH.min_x + stPFH.max_x) / 2 / nZoomScale;
			pt.y = (stPFH.min_y + stPFH.max_y) / 2 / nZoomScale;
			lstSurfacePoint.AddTail(pt);
			//////////////////////////////////////////////////////////////////////////
			if ( fabs (dMaxArea/(nZoomScale*nZoomScale) - (stPFH.area / (nZoomScale * nZoomScale))) < 1)
			{
				ImageNum1 = i;
				m_DefaultMax_X = stPFH.max_x / nZoomScale;
				m_DefaultMax_Y = stPFH.max_y / nZoomScale;
				m_DefaultMax_Z = stPFH.z;
			}

			//̽���������
			for (int i = 0; i < m_uMaxPageNum + 1; i++)
			{
				int nPrintYRange = pDC->GetDeviceCaps(VERTRES);
				if (nYPos > (i + 1) * nPrintYRange - 2 * MARGIN_Y  && nYPos < (i + 1) * nPrintYRange - 2 * MARGIN_Y + (tm.tmHeight + tm.tmExternalLeading + 10) + 1)
				{
					nYPos = ((int)(nYPos + 1800) / nPrintYRange) * nPrintYRange + MARGIN_Y;
				}//end if
			}//end for
			nYPos += (tm.tmHeight + tm.tmExternalLeading + 10);
			pDC->TextOut(rect.left + 4 * tm.tmAveCharWidth, nYPos, strMaxX);
			pDC->TextOut(rect.left + 11 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strMaxY);
			pDC->TextOut(rect.left + 27 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strZ);
			pDC->TextOut(rect.left + 35 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strArea);
			pDC->TextOut(rect.left + 43 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET - 20, nYPos + 10, csStrMark);
			//pDC->TextOut(rect.left + 20 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�ײ�λ��"));

			m_nYPos = nYPos;
			DrawDefaultResultTable1( pDC,rect);//����
			nYPos = m_nYPos;			
			i++;
			nXPos++;
		}

		int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT) * stPFH.PointsCount;
		nPos += sizetmp;
	}//end while
	m_nFlawPrintRow_NUM = i;
	nXPos++;
	nYPos++;
	SafeArrayUnaccessData(Result.parray);
	SafeArrayDestroy(Result.parray);

	// ��ȱ�ݲ���ͼ
	int nPrintXRange = pDC->GetDeviceCaps(HORZRES);
	int nPrintYRange = pDC->GetDeviceCaps(VERTRES);

	POSITION pos = lstSurfacePoint.GetHeadPosition();
	int k = 0;
	while (pos)
	{
		LOGICAL_POINT_t pt = lstSurfacePoint.GetNext(pos);

		if ((int)nYPos / nPrintYRange  < (int)(nYPos + nPrintXRange / 2 + 2 * (tm.tmHeight +tm.tmExternalLeading + 10))/ nPrintYRange )
		{
			nYPos = ((int)((nYPos + nPrintXRange / 2 + 3 * (tm.tmHeight +tm.tmExternalLeading + 10)) / nPrintYRange))* nPrintYRange + MARGIN_Y;
		}
		nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
		CString csStrMark;
		csStrMark.Format(_T("%d"), k);
 		pDC->TextOut(nXPos + rect.Width()/10, nYPos + 10, (csStrMark + _T("������ͼ")));
 		nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);

		CRect RectMaxDefault;
		RectMaxDefault.bottom = nYPos + nPrintXRange / 2;
		RectMaxDefault.left = rect.left;
		RectMaxDefault.right = rect.right;
		RectMaxDefault.top = nYPos;

		PrintDefaultWave(pDC,RectMaxDefault, pt);
		nYPos = RectMaxDefault.bottom;
		nXPos = RectMaxDefault.left;

		k++;
	}
	
}
*/

void CCScanView::PrintDraw(CDC* pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintDraw"),"Enter");
	int nXPos = rect.left;
	int nYPos = rect.top + 10;

	// �߿�
	CPen penBorder(PS_SOLID, 3, RGB(0, 0, 0));

	CPen *pOldPen = pDC->SelectObject(&penBorder);

	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 1200;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Times New Roman"));

	CFont fontTitle;
	fontTitle.CreatePointFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&fontTitle);

	TEXTMETRIC tm;
	::ZeroMemory(&tm, sizeof(TEXTMETRIC));
	pDC->GetTextMetrics(&tm);
	pDC->SelectObject(pOldFont);
	pDC->GetTextMetrics(&tm);

	UINT nOldAlign = pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextAlign(TA_TOP | TA_LEFT);

	m_nYPos = nYPos;
	PrintMessageTableOne(pDC,rect); //��ӡ�������һ����
	nYPos = m_nYPos;

	// Cɨ��ͼ
	nYPos += (tm.tmHeight +tm.tmExternalLeading);
	CRect rectCScan(rect);
	rectCScan.left += rect.Width() / 5;
	rectCScan.right -= rect.Width() / 5;
	rectCScan.top = nYPos;
	rectCScan.bottom = static_cast<LONG>(float(rectCScan.right - rectCScan.left) / (m_dXEnd - m_dXStart) * (m_dYEnd - m_dYStart) + rectCScan.top);

	int nMemDCXLength = int((m_dXEnd - m_dXStart) / m_fGridX);	//MemDC��������X���س���
	int nMemDCYLength = int((m_dYEnd - m_dYStart) / m_fGridY);	//MemDC��������Y���س���
	m_MemDC.SetViewportOrg(0, m_nMemYLENGTH - nMemDCYLength);
	pDC->StretchBlt(rectCScan.left, rectCScan.top, rectCScan.Width(), rectCScan.Height(), &m_MemDC, 0, 0, nMemDCXLength, nMemDCYLength, SRCCOPY);

	CRect YScaleRect;
	YScaleRect.left = rectCScan.left - rect.Width() / 10;
	YScaleRect.right = rectCScan.left;
	YScaleRect.top = rectCScan.top;
	YScaleRect.bottom = rectCScan.bottom;

	CRect XScaleRect;
	XScaleRect.left = rectCScan.left;
	XScaleRect.right = rectCScan.right;
	XScaleRect.top = rectCScan.bottom;
	XScaleRect.bottom = rectCScan.bottom + rect.Width() / 10;

	DrawScale(pDC, XScaleRect, static_cast<float>(m_dXStart), 
		static_cast<float>(m_dXEnd), static_cast<float>((m_dXEnd - m_dXStart) / 10), 3, TRUE);
	DrawScale(pDC, YScaleRect, static_cast<float>(m_dYStart), static_cast<float>(m_dYEnd), 
		static_cast<float>((m_dYEnd - m_dYStart) / 10), 4, TRUE);

	nYPos = rectCScan.bottom + 10 + XScaleRect.Height();
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->MoveTo(rect.left, rect.top + 10);
	pDC->LineTo(rect.left, nYPos);
	pDC->MoveTo(rect.right, rect.top + 10);
	pDC->LineTo(rect.right, nYPos);
	pDC->SetTextAlign(nOldAlign);
	pDC->SelectObject(pOldPen);
	pDC->SetTextAlign(TA_TOP | TA_CENTER);

	//����̽�˱��
	m_nYPos = nYPos;
	DrawDefaultResultTable(pDC,rect); //����ӡ���ȱ���������
	nYPos = m_nYPos;

	//̽�˱�ͷ
	pDC->TextOut(rect.left + 4 * tm.tmAveCharWidth, nYPos + 10, _T("���� X"));
	pDC->TextOut(rect.left + 11 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T(" ���� Y"));
	pDC->TextOut(rect.left + 20 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�ײ�λ��"));
	pDC->TextOut(rect.left + 27 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("���"));
	pDC->TextOut(rect.left + 35 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("ȱ�����"));
	pDC->TextOut(rect.left + 43 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET - 20, nYPos + 10, _T("Aͼ"));
	pDC->TextOut(rect.left + 51 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("λ��Σ����"));
	pDC->TextOut(rect.left + 63 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�������"));
	pDC->TextOut(rect.left + 77 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�������"));
	pDC->TextOut(rect.left + 91 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("��ע"));
	nYPos += (tm.tmHeight + tm.tmExternalLeading) + 10;
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nYPos += (tm.tmHeight + tm.tmExternalLeading) + 10;
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nYPos= nYPos - 2 * (tm.tmHeight + tm.tmExternalLeading + 10);
	//������
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	// ��ȡ������Ϣ
	int i = 0;
	int k = 0;

	//CList<LOGICAL_POINT_t, LOGICAL_POINT_t&> lstSurfacePoint;	// ������С������
	POSITION posNode = pCScanParam->m_lstSurfaceDetail.GetHeadPosition();
	while(posNode)	
	{		
		FlawNode node = pCScanParam->m_lstSurfaceDetail.GetNext(posNode);
		if (node.m_strSave != _T("����"))
		{
			continue;
		}

		LOGICAL_POINT_t pt;
		pt.x = (node.m_fMinX + node.m_fMaxX) / 2 ;
		pt.y = (node.m_fMinY + node.m_fMaxY) / 2 ;

		CString strMinX;
		strMinX.Format(_T("%.2f"), pt.x);//node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), pt.y);//node.m_fMinY);
		CString strMaxX;
		strMaxX.Format(_T("%.2f"), node.m_fMaxX);
		CString strMaxY;
		strMaxY.Format(_T("%.2f"), node.m_fMaxY);
		CString strZ;
		strZ.Format(_T("%.2f"), node.m_fZ);
		CString strArea;
		strArea.Format(_T("%.2f"), node.m_fArea);
		
		
		/*
		CString strPosition;
		strPosition.Format(_T("%.2f"), node.m_strPosition);
		CString strResult;
		strResult.Format(_T("%.2f"), node.m_strResult);
		CString strProc;
		strProc.Format(_T("%.2f"), node.m_strProc);
		CString strMemo;
		strMemo.Format(_T("%.2f"), node.m_strMemo);
		*/
		//CString csStrMark;
		//csStrMark.Format(_T("%d"), i);
		CString strAIndex;
		//////////////////////////////////////////////////////////////////////////
		if (node.m_strAscan == _T("����") && node.m_boolHasAscan)
		{
			//lstSurfacePoint.AddTail(pt);
			strAIndex.Format(_T("%d"), k);
			k++;
		}
		else
		{
			strAIndex = "";
		}
		//////////////////////////////////////////////////////////////////////////

		//̽���������
		for (int j = 0; j < static_cast<int>(m_uMaxPageNum) + 1; j++)
		{
			int nPrintYRange = pDC->GetDeviceCaps(VERTRES);
			if (nYPos > (j + 1) * nPrintYRange - 2 * MARGIN_Y  && nYPos < (j + 1) * nPrintYRange - 2 * MARGIN_Y + (tm.tmHeight + tm.tmExternalLeading + 10) + 1)
			{
				nYPos = ((int)(nYPos + 1800) / nPrintYRange) * nPrintYRange + MARGIN_Y;
			}//end if
		}//end for
		nYPos += (tm.tmHeight + tm.tmExternalLeading + 10);
		pDC->TextOut(rect.left + 4 * tm.tmAveCharWidth, nYPos, strMinX);
		pDC->TextOut(rect.left + 11 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strMinY);
		pDC->TextOut(rect.left + 27 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strZ);
		pDC->TextOut(rect.left + 35 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strArea);
		pDC->TextOut(rect.left + 43 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, strAIndex);
		pDC->TextOut(rect.left + 51 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, node.m_strPosition);
		pDC->TextOut(rect.left + 63 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, node.m_strResult);
		pDC->TextOut(rect.left + 77 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, node.m_strProc);
		pDC->TextOut(rect.left + 91 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos, node.m_strMemo);
		//pDC->TextOut(rect.left + 43 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET - 20, nYPos + 10, csStrMark);
		//pDC->TextOut(rect.left + 20 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 10, _T("�ײ�λ��"));

		m_nYPos = nYPos;
		DrawDefaultResultTable1( pDC,rect);//����
		nYPos = m_nYPos;			
		i++;
		nXPos++;

	}//end while
	m_nFlawPrintRow_NUM = i;
	nXPos++;
	nYPos++;

	// ��ȱ�ݲ���ͼ		2012-05-08 by lijunwei
	k = 0;
	int nPrintXRange = pDC->GetDeviceCaps(HORZRES);
	int nPrintYRange = pDC->GetDeviceCaps(VERTRES);
	posNode = pCScanParam->m_lstSurfaceDetail.GetHeadPosition();
	while(posNode)	
	{		
		FlawNode node = pCScanParam->m_lstSurfaceDetail.GetNext(posNode);
		if (node.m_strAscan == _T("����") && node.m_boolHasAscan)
		{
			LOGICAL_POINT_t pt;
			pt.x = (node.m_fMinX + node.m_fMaxX) / 2 ;
			pt.y = (node.m_fMinY + node.m_fMaxY) / 2 ;

			if ((int)nYPos / nPrintYRange  < (int)(nYPos + nPrintXRange / 2 + 2 * (tm.tmHeight +tm.tmExternalLeading + 10))/ nPrintYRange )
			{
				nYPos = ((int)((nYPos + nPrintXRange / 2 + 3 * (tm.tmHeight +tm.tmExternalLeading + 10)) / nPrintYRange))* nPrintYRange + MARGIN_Y;
			}
			nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
			CString csStrMark;
			csStrMark.Format(_T("%d"), k);
			pDC->TextOut(nXPos + rect.Width()/10, nYPos + 10, (csStrMark + _T("������ͼ")));
			nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);

			CRect RectMaxDefault;
			RectMaxDefault.bottom = nYPos + nPrintXRange / 2;
			RectMaxDefault.left = rect.left;
			RectMaxDefault.right = rect.right;
			RectMaxDefault.top = nYPos;

			PrintDefaultWave(pDC,RectMaxDefault, pt, node.m_aScan );
			nYPos = RectMaxDefault.bottom;
			nXPos = RectMaxDefault.left;

			k++;
		}
	}

/*

	// ��ȱ�ݲ���ͼ
	int nPrintXRange = pDC->GetDeviceCaps(HORZRES);
	int nPrintYRange = pDC->GetDeviceCaps(VERTRES);

	POSITION pos = lstSurfacePoint.GetHeadPosition();
	int k = 0;
	while (pos)
	{
		LOGICAL_POINT_t pt = lstSurfacePoint.GetNext(pos);

		if ((int)nYPos / nPrintYRange  < (int)(nYPos + nPrintXRange / 2 + 2 * (tm.tmHeight +tm.tmExternalLeading + 10))/ nPrintYRange )
		{
			nYPos = ((int)((nYPos + nPrintXRange / 2 + 3 * (tm.tmHeight +tm.tmExternalLeading + 10)) / nPrintYRange))* nPrintYRange + MARGIN_Y;
		}
		nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
		CString csStrMark;
		csStrMark.Format(_T("%d"), k);
		pDC->TextOut(nXPos + rect.Width()/10, nYPos + 10, (csStrMark + _T("������ͼ")));
		nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);

		CRect RectMaxDefault;
		RectMaxDefault.bottom = nYPos + nPrintXRange / 2;
		RectMaxDefault.left = rect.left;
		RectMaxDefault.right = rect.right;
		RectMaxDefault.top = nYPos;

		PrintDefaultWave(pDC,RectMaxDefault, pt);
		nYPos = RectMaxDefault.bottom;
		nXPos = RectMaxDefault.left;

		k++;
	}
*/
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintDraw"),"Leave");
}

/*
// ����X���п�������
void CCScanView::QuickSortX(FeaturesData* SeqList, int low, int high)
{
	int pivotpos; //���ֺ�Ļ�׼��¼��λ��
	if(low < high)
	{
		//�������䳤�ȴ���1ʱ��������
		pivotpos = PartitionX(SeqList, low, high); //��SeqList[low..high]������
		QuickSortX(SeqList, low, pivotpos-1); //��������ݹ�����
		QuickSortX(SeqList, pivotpos+1, high); //��������ݹ�����
	}
}

//�����㷨 ���
int CCScanView::PartitionX(FeaturesData* SeqList, int low, int high)
{
	int rnd = rand()%(high-low+1) + low;
	//�����ػ�׼��¼��λ��
	FeaturesData pivot = SeqList[rnd]; //������ĵ�1����¼��Ϊ��׼
	while(low < high)
	{ //���������˽������м�ɨ�裬ֱ��i=jΪֹ
		while(low < high && SeqList[high].x > pivot.x) //pivot�൱����λ��i�� ��>=�޸ĳ�> �����ڽӽ�������Сʱʱ��Ͳ����ǳ���
			high--; //��������ɨ�裬���ҵ�1���ؼ���С��pivot.key�ļ�¼SeqList[j]

		if( low < high ) //��ʾ�ҵ���SeqList[j]�Ĺؼ���<pivot.x
			SeqList[low++] = SeqList[high];

		while(low < high && SeqList[low].x < pivot.x) //pivot�൱����λ��j��	��<=�޸ĳ�< �����ڽӽ�������Сʱʱ��Ͳ����ǳ���
			low++; //��������ɨ�裬���ҵ�1���ؼ��ִ���pivot.key�ļ�¼SeqList[i]

		if( low < high )	//��ʾ�ҵ���SeqList[j]�Ĺؼ���>pivot.x
			SeqList[high--] = SeqList[low];
	}
	SeqList[low] = pivot; //��׼��¼�ѱ����λ

	return low;
}

// ��X��������ٶ�X��ͬ�ĵ����Y��С�����������
void CCScanView::SortYAfterSortX(FeaturesData* SeqList, int low, int high)
{
	int k = 0;
	int i = low;
	for(; i < high; i++)
	{
		if(fabs(SeqList[i].x - SeqList[i+1].x) < 0.000001)	// ����Xֵ��ͬʱ��¼����
		{
			k++;
		}
		else	//����Xֵ��ͬʱ
		{
			if( k > 1 )	// ������������ͬ��Xֵ,����Yֵ��С��������
			{
				QuickSortY(SeqList, low, low + k);
			}
			break;	//�˳�����ѭ��
		}
	}
	if( i < high )	// �������ڶ���ʱ�����ݹ�
	{
		SortYAfterSortX(SeqList, i+1, high);
	}
}

// ����Y���п�������
void CCScanView::QuickSortY(FeaturesData* SeqList, int low, int high)
{
	int pivotpos; //���ֺ�Ļ�׼��¼��λ��
	if(low < high)
	{
		//�������䳤�ȴ���1ʱ��������
		pivotpos = PartitionY(SeqList, low, high); //��SeqList[low..high]������
		QuickSortY(SeqList, low, pivotpos-1); //��������ݹ�����
		QuickSortY(SeqList, pivotpos+1, high); //��������ݹ�����
	}
}

// ����Yֵ��������
int CCScanView::PartitionY(FeaturesData* SeqList, int low, int high)
{
	int rnd = rand()%(high-low+1) + low;
	//�����ػ�׼��¼��λ��
	FeaturesData pivot = SeqList[rnd]; //������ĵ�1����¼��Ϊ��׼
	while(low < high)
	{ //���������˽������м�ɨ�裬ֱ��i=jΪֹ
		while(low < high && SeqList[high].y > pivot.y) //pivot�൱����λ��i�� ��>=�޸ĳ�> �����ڽӽ�������Сʱ�Ͳ�������ǳ�
			high--; //��������ɨ�裬���ҵ�1���ؼ���С��pivot.key�ļ�¼SeqList[j]

		if( low < high ) //��ʾ�ҵ���SeqList[j]�Ĺؼ���<pivot.x
			SeqList[low++] = SeqList[high];

		while(low < high && SeqList[low].y < pivot.y) //pivot�൱����λ��j��	��<=�޸ĳ�< �����ڽӽ�������Сʱ�Ͳ�������ǳ�
			low++; //��������ɨ�裬���ҵ�1���ؼ��ִ���pivot.key�ļ�¼SeqList[i]

		if( low < high )	//��ʾ�ҵ���SeqList[j]�Ĺؼ���>pivot.x
			SeqList[high--] = SeqList[low];
	}
	SeqList[low] = pivot; //��׼��¼�ѱ����λ

	return low;
}
*/
float CCScanView::getScaleFactor()
{
	return m_fScaleFactor;
}
CSize CCScanView::getLayout()
{
	return m_layout;
}
CSize CCScanView::getDisplaySize()
{
	return m_displaySize;
}

CSize CCScanView::setDisplaySize(CSize size)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::setDisplaySize"),"Enter");
	m_displaySize = size;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::setDisplaySize"),"Leave");	
	return m_displaySize;
}

// ��ʾȫ��Cɨ����
void CCScanView::OnCScanOverview()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCScanOverview"),"Enter");
	// TODO: Add your command handler code here
	if (m_pOverViewWnd == NULL)
	{
		return;
	}

	if(!m_bOverViewDisplay)
	{		
		CRect rect;
		GetClientRect(&rect);

		if(rect.Height() > OVERVIEW_HEIGHT && rect.Width() > OVERVIEW_WIDTH)
		{
			// �Ŵ���ͼ�����ʱ���
			m_fScaleFactor = 4;

			m_displaySize.cx = int(m_layout.cx * m_fScaleFactor);
			m_displaySize.cy = int(m_layout.cy * m_fScaleFactor);

			SetScrollSizes(MM_TEXT, m_displaySize);

			// ��������Ӧλ��
			int nXPos = 0;
			int nYPos = 0;
			m_pOverViewWnd->GetScrollPos(m_displaySize, nXPos, nYPos);
			POINT pt;
			pt.x = nXPos;
			pt.y = nYPos;
			ScrollToPosition(pt);

			m_pOverViewWnd->SetWindowPos(&wndTop, rect.Width() - OVERVIEW_WIDTH, 
				0, OVERVIEW_WIDTH, OVERVIEW_HEIGHT, SWP_SHOWWINDOW | SWP_NOSIZE);
			m_pOverViewWnd->ShowWindow(TRUE);

			Invalidate(TRUE);

		}
		else
		{
			m_bOverViewDisplay = TRUE;
		}
	}
	else
	{
		m_pOverViewWnd->ShowWindow(FALSE);
	}

	m_bOverViewDisplay = !m_bOverViewDisplay;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCScanOverview"),"Leave");
}

// ȫ��Cɨ���ڲ˵�״̬
void CCScanView::OnUpdateCScanOverview(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateCScanOverview"),"Enter");
	// TODO: Add your command update UI handler code here
	if (m_bOverViewDisplay)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	else
		pCmdUI->SetCheck(BST_UNCHECKED);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateCScanOverview"),"Leave");		
}

LRESULT CCScanView::OnMoveOverView(WPARAM wParam, LPARAM lParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMoveOverView"),"Enter");
	CRect rect;
	GetClientRect(&rect);

	m_pOverViewWnd->MoveWindow(rect.Width() - OVERVIEW_WIDTH*2, OVERVIEW_HEIGHT, OVERVIEW_WIDTH, OVERVIEW_HEIGHT, TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMoveOverView"),"Leave");	
	return 0;
}
//
/** 
* ͼ��ƽ������
* @param     pDC [in]:	ָ����ͼ�����豸������
* @param     nWidth [in]:ͼ��Ŀ��
* @param     nHeight [in]:ͼ��ĸ߶�
* @param     nTempW [in]:ģ��Ŀ��
* @param     nTempH [in]:ģ��ĸ߶�
* @param     nTempMX [in]:ģ�������Ԫ�ص�x����
* @param     nTempMY [in]:ģ�������Ԫ�ص�Y����
* @param     fpArray [in]:ָ��ģ��Ԫ�������ָ��
* @param     fCoef [in]:ģ��ϵ��
* @return    void
* @since     1.00
*/
void CCScanView::SmoothCScanImage(CDC* pDC,//ָ����ͼ�����豸������
								  int nWidth,
								  int nHeight,
								  int nTempW,
								  int nTempH,
								  int nTempMX,
								  int nTempMY,
								  float* fpArray,
								  float fCoef)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::SmoothCScanImage"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int n = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nColorMode;//0:�Ҷ�ͼ��1����ɫͼ
	int RGrayValue = 0;
	int GGrayValue = 0;
	int BGrayValue = 0;
	int nRValueResult = 0;
	int nGValueResult = 0;
	int nBValueResult = 0;

	int  nResult = 0;//������

	int i = 0;
	int j  = 0;
	//��ȥ��Ե����
// 	for( i = nTempMY;i<nHeight - nTempH + nTempMY + 1;++i)
// 	{	
// 		//��ȥ��Ե����
// 		for ( j = nTempMX;j<nWidth - nTempW + nTempMX + 1;++j)
// 		{

	for( i = m_rectCScan.left + 1;i<m_rectCScan.right;++i)
	{	
		//��ȥ��Ե����
		for ( j = m_rectCScan.top + 1;j<m_rectCScan.bottom;++j)
		{


			//�����(i,j)�ĻҶ�ֵ
			int nGrayValue;
			//COLORREF color = pDC->GetPixel(i,j);

			if (n == 0)//�Ҷȴ���
			{
				//�Ҷ�ֵ����ģ�����

				for (int k = 0;k<nTempH;++k)
				{
					for (int m = 0;m<nTempW;++m)
					{
						//��i- nTempMY + K�У���j-nTempMX+m�����ص�ָ��
						COLORREF clr = pDC->GetPixel(i- nTempMY + k+2,(j-nTempMX+m+2));
						int RGray = GetRValue(clr);
						int GGray = GetGValue(clr);
						int BGray = GetBValue(clr);
						int nGray = (RGray +GGray+BGray)/3;

						nResult += static_cast<int>(nGray*fpArray[k*nTempW+m]);
					}
				}
				nResult*= static_cast<int>(fCoef);
				//ȡ����ֵ
				nResult = static_cast<int>(fabs(static_cast<float>(nResult)));
				//�ж��Ƿ񳬹�255
				if (nResult > 255)
				{
					nGrayValue = 255;
				}
				else
				{
					//(i,j)���ص���»Ҷ�ֵ
					nGrayValue = static_cast<int>(nResult+0.5);
				}

				pDC->SetPixel(i,j,RGB(nGrayValue,nGrayValue,nGrayValue));
				//pDC->SetPixel(i,j,RGB(255,0,0));
			}
			else//��ɫͼ
			{
				for (int k = 0;k<nTempH;++k)
				{
					for (int m = 0;m<nTempW;++m)
					{
						//��i- nTempMY + K�У���j-nTempMX+m�����ص�ָ��
						COLORREF clr = pDC->GetPixel(i- nTempMY + k+2,(j-nTempMX+m+2));
						int RGray = GetRValue(clr);
						int GGray = GetGValue(clr);
						int BGray = GetBValue(clr);
						//int nGray = (RGray +GGray+BGray)/3;
						int t = static_cast<int>(fpArray[k*nTempW+m]);

						nRValueResult += static_cast<int>(RGray*fpArray[k*nTempW+m]);
						nGValueResult += static_cast<int>(GGray*fpArray[k*nTempW+m]);
						nBValueResult += static_cast<int>(BGray*fpArray[k*nTempW+m]);
					}
				}
				nRValueResult*= static_cast<int>(fCoef);
				nGValueResult*= static_cast<int>(fCoef);
				nBValueResult*= static_cast<int>(fCoef);
				//ȡ����ֵ
				nRValueResult = static_cast<int>(fabs(static_cast<float>(nRValueResult)));/////////////////////////////////
				nGValueResult = static_cast<int>(fabs(static_cast<float>(nGValueResult)));
				nBValueResult = static_cast<int>(fabs(static_cast<float>(nBValueResult)));

				//�ж��Ƿ񳬹�255
				if (nRValueResult > 255)
				{
					nRValueResult = 255;
				}
				else
				{
					//(i,j)���ص���»Ҷ�ֵ
					nRValueResult = static_cast<int>(nRValueResult+0.5);
				}
				if (nGValueResult > 255)
				{
					nGValueResult = 255;
				}
				else
				{
					//(i,j)���ص���»Ҷ�ֵ
					nGValueResult = static_cast<int>(nGValueResult+0.5);
				}
				if (nBValueResult > 255)
				{
					nBValueResult = 255;
				}
				else
				{
					//(i,j)���ص���»Ҷ�ֵ
					nBValueResult = static_cast<int>(nBValueResult+0.5);
				}

				pDC->SetPixel(i,j,RGB(nRValueResult,nGValueResult,nBValueResult));

			}

		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::SmoothCScanImage"),"Leave");
}
/** 
* ͼ���˲�����
* @param     pDC [in]:	ָ����ͼ�����豸������
* @param     nWidth [in]:ͼ��Ŀ��
* @param     nHeight [in]:ͼ��ĸ߶�
* @param     nTempW [in]:ģ��Ŀ��
* @param     nTempH [in]:ģ��ĸ߶�
* @param     nTempMX [in]:ģ�������Ԫ�ص�x����
* @param     nTempMY [in]:ģ�������Ԫ�ص�Y����
* @return    void
* @since     1.00
*/
void CCScanView::MedianFilter(CDC* pDC,int nWidth,int nHeight,int nFilterH,int nFilterW,int nFilterMX,int nFilterMY)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MedianFilter"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int n = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nColorMode;//��ȡ��ͼģʽ��0���Ҷ�ͼ��1����ɫͼ

	int RGrayValue = 0;
	int GGrayValue = 0;
	int BGrayValue = 0;

	//unsigned char* nValue;
	int nValue[9];
	int nRValue[9];
	int nGValue[9];
	int nBValue[9];

	//float fResult;//������
	//��ȥ��Ե����
// 	for (int i = nFilterMY;i<nHeight - nFilterH + nFilterMY +1;++i )
// 	{
// 		//��ȥ��Ե����
// 		for (int j = nFilterMX;j<nWidth - nFilterW+nFilterMX+1;++j)
// 		{

	for (int i = m_rectCScan.left +1;i<m_rectCScan.right;++i )
	{
		//��ȥ��Ե����
		for (int j = m_rectCScan.top + 1;j<m_rectCScan.bottom;++j)
		{
			//(i,j)���ص�ĻҶ�ֵ
			int nGrayValue;
			//(i,j)���ص������ֵ
			//COLORREF color = pDC->GetPixel(i,j);

			if (n == 0)//�Ҷ�ͼ
			{
				//ȡ�˲�������
				for (int k =0;k<nFilterH;++k)
				{
					for (int m = 0;m<nFilterW;++m)
					{
						COLORREF color = pDC->GetPixel(i - nFilterMY + k+2,(j - nFilterMX + m+2));
						RGrayValue = GetRValue(color);
						GGrayValue = GetGValue(color);
						BGrayValue = GetBValue(color);

						nGrayValue = (RGrayValue+GGrayValue+BGrayValue)/3;

						nValue[k*nFilterW+m] = nGrayValue;//????????????????
					}
				}
				//��ȡ��ֵ,
				nGrayValue = GetMedianNum(nValue,nFilterH*nFilterW);
				pDC->SetPixel(i,j,RGB(nGrayValue,nGrayValue,nGrayValue));
				//pDC->SetPixel(i,j,RGB(255,0,0));
			}
			else//��ɫͼ n == 1;
			{
				//ȡ�˲�������
				for (int k =0;k<nFilterH;++k)
				{
					for (int m = 0;m<nFilterW;++m)
					{
						COLORREF color = pDC->GetPixel(i - nFilterMY + k+2,(j - nFilterMX + m+2));
						RGrayValue = GetRValue(color);
						GGrayValue = GetGValue(color);
						BGrayValue = GetBValue(color);

						//nGrayValue = (RGrayValue+GGrayValue+BGrayValue)/3;

						//nValue[k*nFilterW+m] = nGrayValue;
						nRValue[k*nFilterW+m] = RGrayValue;
						nGValue[k*nFilterW+m] = GGrayValue;
						nBValue[k*nFilterW+m] = BGrayValue;


					}
				}
				//��ȡ��ֵ,
				//nGrayValue = GetMedianNum(nValue,nFilterH*nFilterW);
				int t  = nFilterH*nFilterW;
				RGrayValue = GetMedianNum(nRValue,nFilterH*nFilterW);
				GGrayValue = GetMedianNum(nGValue,nFilterH*nFilterW);
				BGrayValue = GetMedianNum(nBValue,nFilterH*nFilterW);

				pDC->SetPixel(i,j,RGB(RGrayValue,GGrayValue,BGrayValue));

			}
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MedianFilter"),"Leave");
}
/*************************************************************************
* �������ƣ�
*   GetMedianNum()
* ����:
*   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
*   int   iFilterLen			- ���鳤��
* ����ֵ:
*   unsigned char      - ����ָ���������ֵ��
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
************************************************************************/
unsigned char CCScanView::GetMedianNum(int bArray[], int iFilterLen)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetMedianNum"),"Enter");
	int		i,j;			// ѭ������
	unsigned char bTemp;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetMedianNum"),"Leave");
	return bTemp;
}
//��ת���豸��ص�ͼƬ
/*************************************************************************
* �������ƣ�
*   RotateImage()
* ����:
*   CDC* pDC		- ָ��ͼ���豸������
*   double angle	- ƫת�Ƕȣ����ȣ�
* ����ֵ:
*  void
* ˵��:
*   ��ת���豸��ص�ͼƬ
************************************************************************/
void CCScanView::RotateImage(CDC* pDC,double angle,CPoint centerPt)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::RotateImage"),"Enter");
	HDC hDc = pDC->GetSafeHdc();
/*	POINT centerPt;*/   
// 	centerPt.x = (rect.right - rect.left)/2 ;
// 	centerPt.y = (rect.bottom - rect.top)/2 ;  

    centerPt.x -= CSCAN_SCALE_X_LENGTH;
	int nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	XFORM xform;
	if ( angle != 0 )
	{
		double fangle = RADIAN(angle);
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(centerPt.x - cos(fangle)*centerPt.x + sin(fangle)*centerPt.y);
		xform.eDy = (float)(centerPt.y - cos(fangle)*centerPt.y - sin(fangle)*centerPt.x);

		BOOL bRet = SetWorldTransform(hDc, &xform);
		{
			int a = 0;
		}
       HDC hDc = pDC->GetSafeHdc();
	   SetGraphicsMode(hDc, nGraphicsMode); 
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::RotateImage"),"Leave");
}
//ƽ��
void CCScanView::OnCscanSmooth()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanSmooth"),"Enter");
	// TODO: �ڴ���������������

	FLOAT	aValue[25];			// ģ��Ԫ������
	// wangbingfu delete start on 2012-11-03
// 	int		iTempH;				// ģ��߶�	
// 	int		iTempW;				// ģ����	
// 	FLOAT	fTempC;				// ģ��ϵ��	
// 	int		iTempMX;			// ģ������Ԫ��X����	
// 	int		iTempMY;			// ģ������Ԫ��Y����
	// wangbingfu delete end on 2012-11-03
	CSmoothDlg dlgPara;	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	//dlgPara.m_intType = 0;
	dlgPara.m_nTempH  = 3;
	dlgPara.m_nTempW  = 3;
	dlgPara.m_nTempMX = 1;
	dlgPara.m_nTempMY = 1;
	dlgPara.m_fTempC  = (FLOAT) (1.0 / 9.0);
	for (int i = 0;i<25;++i)
	{
		dlgPara.m_fpArray[i] = aValue[i];
	}
	//dlgPara.m_fpArray = aValue;//���鲻��ֱ��������ֵ

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanSmooth"),"Leave");
		return;
	}

	CDC* pCScanDC = this->GetDC();

	// wangbingfu add start on 2012-11-03
	if(pCScanDC == NULL)
	{
		MessageBox(_T("CScanView  OnCscanSmooth(), pCScanDC is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnCscanSmooth"),"Leave");
		return;
	}
	// wangbingfu add start on 2012-11-03

	SmoothCScanImage(pCScanDC,m_displaySize.cx,m_displaySize.cy,dlgPara.m_nTempH,dlgPara.m_nTempW,dlgPara.m_nTempMX,
		dlgPara.m_nTempMY,dlgPara.m_fpArray,dlgPara.m_fTempC);

	ReleaseDC(pCScanDC);	// wangbingfu add start on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanSmooth"),"Leave");
}
//��ֵ�˲�
void CCScanView::OnCscanMf()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanMf"),"Enter");
	// TODO: �ڴ���������������

	CMidFilterDlg dlgPara;
	//dlgPara.m_nFilterType = 0;
	dlgPara.m_nFilterH = 3;
	dlgPara.m_nFilterW = 3;
	dlgPara.m_nFilterMX = 1;
	dlgPara.m_nFilterMY = 1;	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanMf"),"Leave");
		return;
	}
	CDC* pCScanDC = this->GetDC();
	// wangbingfu add start on 2012-11-03
	if(pCScanDC == NULL)
	{
		MessageBox(_T("CScanView  OnCscanMf(), pCScanDC is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnCscanMf"),"Leave");
		return;
	}
	// wangbingfu add end on 2012-11-03
 	MedianFilter(pCScanDC,m_displaySize.cx,m_displaySize.cy,dlgPara.m_nFilterH,dlgPara.m_nFilterW,
 		dlgPara.m_nFilterMX,dlgPara.m_nFilterMY);

	ReleaseDC(pCScanDC);	// wangbingfu add start on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCscanMf"),"Leave");
}

void CCScanView::OnCScanMicroscope()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCScanMicroscope"),"Enter");
	// TODO: Add your command handler code here
	if (m_pMicroScopeWnd == NULL)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCScanMicroscope"),"Leave");
		return;
	}

	if(!m_bMicroscopeDisplay)
	{
		// ��ʾ����
		CRect rect;
		GetClientRect(&rect);

		if(rect.Height() > 100 && rect.Width() > 100)
		{
			m_pMicroScopeWnd->SetWindowPos(&wndTop, 0, 
				0, 100, 100, SWP_SHOWWINDOW | SWP_NOSIZE);
			m_pMicroScopeWnd->ShowWindow(TRUE);
		}
		else
		{
			m_bMicroscopeDisplay = TRUE;
		}
	}
	else
	{
		//���ش���
		m_pMicroScopeWnd->ShowWindow(FALSE);
	}

	m_bMicroscopeDisplay = !m_bMicroscopeDisplay;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCScanMicroscope"),"Leave");
}

void CCScanView::OnUpdateCScanMicroscope(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateCScanMicroscope"),"Enter");
	// TODO: Add your command update UI handler code here
	if(m_bMicroscopeDisplay)
		pCmdUI->SetCheck(BST_CHECKED);
	else
		pCmdUI->SetCheck(BST_UNCHECKED);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateCScanMicroscope"),"Leave");
}

void CCScanView::OnMouseMove(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseMove"),"Enter");
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView;
	
	// wangbingfu add start on 2012-11-03
	CDC* pDC = GetDC();	
	if(pDC == NULL)
	{
		MessageBox(_T("CScanView  OnMouseMove(), pDC is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnMouseMove"),"Leave");
		return;
	}
	// wangbingfu add end on 2012-11-03

	if(m_nObjectNum == 1)
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	else
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;

	if (pCScanParamView->m_bSurfaceFlag)	// ����ͳ��
	{
		// ����
		if (m_bTracking)
		{
			if (pCScanParamView->m_nSurfaceType == 0)	// ����
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eRECT);
				InvertRegion(pDC, m_ptFrom, point, eRECT);
			} 
			else if(pCScanParamView->m_nSurfaceType == 1)	// Բ
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eCIRCLE);
				InvertRegion(pDC, m_ptFrom, point, eCIRCLE);
			}
			else if (pCScanParamView->m_nSurfaceType == 2)	// ��Բ
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eELLIPSE);
				InvertRegion(pDC, m_ptFrom, point, eELLIPSE);
			}
			else if (pCScanParamView->m_nSurfaceType == 3)	// �����
			{
				InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 

				m_Poly[m_nPolyNum] = point;
				InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 
			}
			m_ptTo = point;
		}
	}
	else if(pCScanParamView->m_bNoteFlag)	// ע��
	{	
		// ����
		if (m_bTracking)
		{
			if (pCScanParamView->m_nNoteType == 0)	// ����
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eRECT);
				InvertRegion(pDC, m_ptFrom, point, eRECT);
			} 
			else if(pCScanParamView->m_nNoteType == 1)	// Բ
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eCIRCLE);
				InvertRegion(pDC, m_ptFrom, point, eCIRCLE);
			}
			else if (pCScanParamView->m_nNoteType == 2)	// ��Բ
			{
				InvertRegion(pDC, m_ptFrom, m_ptTo, eELLIPSE);
				InvertRegion(pDC, m_ptFrom, point, eELLIPSE);
			}
			else if (pCScanParamView->m_nNoteType == 3)	// �����
			{
				InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 

				m_Poly[m_nPolyNum] = point;
				InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 
			}
			m_ptTo = point;
		}
	}
	else if(m_bMicroscopeDisplay && m_pMicroScopeWnd != NULL)	// ��΢��
	{
		// ����ͼ����΢������		
		CDC* pMicroScopeDC = m_pMicroScopeWnd->GetDC();
		// wangbingfu add start on 2012-11-03
		if(pMicroScopeDC == NULL)
		{
			MessageBox(_T("CScanView  OnMouseMove(), pMicroScopeDC is null"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnMouseMove"),"Leave");
			return;
		}
		// wangbingfu add end on 2012-11-03
		pMicroScopeDC->StretchBlt(0, 0, 100, 100, pDC, point.x - 20, point.y - 20,
			40, 40, SRCCOPY);

		m_pMicroScopeWnd->ReleaseDC(pMicroScopeDC);
	}
	else if (m_bMeasureOn)	// ����
	{
		// ����
		if (m_bTracking && m_eMeasureType != ePolygon)
		{
			InvertRegion(pDC, m_ptFrom, m_ptTo, m_eMeasureType);
			InvertRegion(pDC, m_ptFrom, point, m_eMeasureType);
			m_ptTo = point;

			// ������Ϣ
			
		}
		//yachang.huang add start on 2012-6-25 
		else if (m_bTracking && m_eMeasureType == ePolygon)
		{
			InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 

			m_Poly[m_nPolyNum] = point;
			InvertPolygonRegion(pDC, m_Poly, m_nPolyNum + 1); 
		}
		//yachang.huang add end on 2012-6-25 
	}
	// machao add start on 2013-08-23
	else if (m_bMouseGainRangeSelect)
	{
		if (m_bDrawGainRange)
		{
			DrawPartGainCompensateRange(pDC, m_fWStart, m_fZStart, m_fWStop, m_fZStop);
			DrawPartGainCompensateRange(pDC, m_fWStart, m_fZStart, Client2Logical(point).x,  Client2Logical(point).y);
			LOGICAL_POINT_t ptStop =  Client2Logical(point);
			m_fWStop = ptStop.x;
			m_fZStop = ptStop.y;
		}
	}
	// machao add end on 2013-08-23
	else
	{
		// ʮ�ֽ�����
		BOOL bRun = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_bPathExecute;
		if (!bRun)
		{
			if (m_bMouseLeave)
			{
				if(m_nEnterCount != 0)
					InvertRegion(pDC, m_ptCurPos, m_ptTo, eLINE);
				InvertRegion(pDC, point, m_ptTo, eLINE);
				m_ptCurPos = point;

				m_nEnterCount++;
			} 
			else
			{
				TRACKMOUSEEVENT tme; 
				tme.cbSize = sizeof(tme); 
				tme.hwndTrack = m_hWnd; 
				tme.dwFlags = TME_LEAVE; 
				tme.dwHoverTime = 0; 
				m_bMouseLeave = _TrackMouseEvent(&tme); 
			}
		}		
	}
	ReleaseDC(pDC); // wangbingfu add on 2012-11-03

	if (pFrame->m_wndCScanDlgBar.GetSafeHwnd()!=NULL && m_bMeasureOn)
	{
		CRect rect;
		GetClientRect(&rect);
		CString str;
		float fPointCX = 0.0f;
		float fPointCY = 0.0f;

		LOGICAL_POINT_t logicalPoint;
		logicalPoint = Client2Logical(point);

		fPointCX = logicalPoint.x;
		fPointCY = logicalPoint.y;
		if(point.x < m_rectScaleY.right)
		{
			fPointCX = m_fXMove;
		}
		if(point.y > m_rectScaleX.top)
		{
			fPointCY = m_fYMove;

		}

		str.Format(_T("���꣺%.1f,%.1f"), fPointCX, fPointCY);
		CWnd* pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_COORD);//��ʾ����
		pWnd->SetWindowText(str);

		//����DBֵ
		float fDBValue = 0.0f;//��ǰ���DBֵ
		float fSrcDBValue = 0.0f;//����DBֵ
		float fChiZhi = 0.0f;

		fDBValue  = CalculateDBValue(point);
		str.Format(_T("��ǰDB��%.1f"),fDBValue);
		pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_STATIC_CURRENTDB);//��ʾ��ǰdbֵ
		pWnd->SetWindowText(str);

		//����Ŵ���
		float CScanfactor = m_fScaleFactor*100;
		str.Format(_T("Zoom:%.1f"),CScanfactor);
		pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_STATIC_ZOOM);
		pWnd->SetWindowText(str);

		LOGICAL_POINT_t pBegin, pEnd;//��㣬�յ�λ��
		pBegin = Client2Logical(m_ptFrom);
		pEnd = Client2Logical(point);		

		if (m_bTracking)//����갴�»�����ͼʱ
		{
			if (m_eMeasureType == eTRIANGLE)//������ͼ��Ϊ������ʱ
			{
				fSrcDBValue = CalculateDBValue(m_ptFrom);	
				fChiZhi = fDBValue - fSrcDBValue;

				//����ֱ�߲����Ŀ��
				float DXValue = pBegin.x - pEnd.x;
				DXValue = fabs(DXValue);

				//����֧�ֲ����ĸ߶�
				float DYValue = pBegin.y - pEnd.y;
				DYValue = fabs(DYValue);
				

				//����֧�ֲ����ĳ���
				float fLong = 0.0f;
				fLong = sqrt(DXValue*DXValue + DYValue*DYValue);

				str.Format(_T("Length:%.1f mm Ruler: Width:%.1fmm Height:%.1f mm Difference:%.1f Decibel"),
					fLong, DXValue,DYValue,fChiZhi);
				pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);

			}
			else if (m_eMeasureType == eRECT)//������ͼ��Ϊ����ʱ
			{
				float fWidth = pBegin.x - pEnd.x;//���εĿ�
				fWidth = fabs(fWidth);
				float fHeight = pBegin.y- pEnd.y;//���εĸ�
				fHeight = fabs(fHeight);
 				float fRectArea = fWidth*fHeight;//���ε����

				str.Format(_T("Selected: %.1fmm * %.1fmm = %.1fmm^2"),fWidth,fHeight,fRectArea);
				pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);

			}
			else if(m_eMeasureType == eELLIPSE)//������ͼ��ΪԲʱ
			{
				float fHZhou = pBegin.x - pEnd.x;//ˮƽ��ĳ���
				fHZhou = fabs(fHZhou);
				float fVZhou = pBegin.y - pEnd.y;//��ֱ��ĳ���
				fVZhou = fabs(fVZhou);

				float fEllipseArea = static_cast<float>(PI*(fHZhou/2.0)*(fVZhou/2.0));//��Բ�����
				str.Format(_T("Selected: %.1fmm * %.1fmm = %.1fmm^2"),fHZhou,fVZhou,fEllipseArea);
				pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);
			}
			//yachang.huang add start on 2012-6-25  
			else if(m_eMeasureType == ePolygon)//������ͼ��Ϊ�����ʱ
			{
				this->CalculatePolygonArea();
				str.Format(_T("Selected:  %.1fmm^2"),m_fPolygonArea);
				pWnd = pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);
			}
			//yachang.huang add end on 2012-6-25 
		}
	}

	LOGICAL_POINT_t stLogicalPoint;
	stLogicalPoint = Client2Logical(point);

	int nPixelX = (int)((stLogicalPoint.x - m_dXStart) / m_fGridX + 0.5);
	int nPixelY = (int)((stLogicalPoint.y - m_dYStart) / m_fGridY + 0.5);
/*
	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// ��תW1	wangbingfu add on 2013-01-09
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			nPixelY = (int)((stLogicalPoint.y * PI * m_dPipeDiameter / 360.0f - m_dYStart) / m_fGridY);
		}
	}
	// Song Chenguang add start on 2013-05-04
	else if (m_nAxisSelect == 5) // Wƽ��
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			nPixelX = (int)((stLogicalPoint.x * PI * m_dPipeDiameter / 360.0f - m_dXStart) / m_fGridX);
		}
	}
*/	// Song Chenguang add end on 2013-05-04
	float fValue = 0.0f; //����ֵ
	if((nPixelX >= 0) && (nPixelX < m_nMemXLENGTH) && (nPixelY < m_nMemYLENGTH) && (nPixelY >= 0))
	{
		fValue = m_pPixel[nPixelX][nPixelY];
	}
	else
	{
		fValue = 0;
	}
	int nFeatureIndex = pCScanParamView->m_nFeatureIndex;
	CString strTip2;
	if(nFeatureIndex == 0 || nFeatureIndex == 1 || nFeatureIndex == 2 || nFeatureIndex == 3)
	{
		strTip2.Format(_T("H = %.1f"),fValue);
	}
	else if(nFeatureIndex == 4 || nFeatureIndex == 5)
	{
		float fdB = 0.0f;
		if (fabs(fValue) > 1E-2)
			fdB = 0 - 20 * log10(g_fDBZeroAmp / fValue);
		else
			fdB = 0.0f;

		strTip2.Format(_T("A = %.1f %%\nA = %0.1f dB"),fValue, fdB);
	}
	else if(nFeatureIndex == 6)
	{
		strTip2.Format(_T("V = %.1f"),fValue);
	}
	else if(nFeatureIndex == 7)
	{
		strTip2.Format(_T("�� = %.1f"),fValue);
	}
	else if(nFeatureIndex == 8)
	{
		strTip2.Format(_T("Z = %.1f"),fValue);
	}
	else if(nFeatureIndex == 9)
	{
		strTip2.Format(_T("�� = %.1f"),fValue);
	}
	else
	{
		;
	}
	
	CString strTip1;
	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// ��תW1 wangbingfu add on 2013-01-09
	{	
		float fX = stLogicalPoint.x;
		float fR = 0;
		if (m_pCScanParamView->m_nPipeXMode == 0)
			fR = stLogicalPoint.y / (PI * m_dPipeDiameter) * 360.0f;
		else
			fR = stLogicalPoint.y;

		if(m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0)
		{
			fX = stLogicalPoint.x - m_Offset_Coordinate.x;	//�ı�����ԭ��
			fR = fR - m_Offset_Coordinate.y;	//�ı�����ԭ��
		}
		strTip1.Format(_T("X = %.1f\nR = %.1f\n"),fX, fR);
		
	}
	// Song Chenguang add start on 2013-05-04
	else if(m_nAxisSelect == 5)	// Wƽ��
	{	
		float fW = stLogicalPoint.x;
		float fZ = stLogicalPoint.y;
		if(m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0)
		{
			fW = stLogicalPoint.x - m_Offset_Coordinate.x;	//�ı�����ԭ��
			fZ = stLogicalPoint.y - m_Offset_Coordinate.y;	//�ı�����ԭ��
		}
		strTip1.Format(_T("Z = %.1f\nW = %.1f\n"),fZ, fW);

	}
	// Song Chenguang add end on 2013-05-04
	else if(m_nAxisSelect == 1)	// XZ
	{
		float fX = stLogicalPoint.x;
		float fZ = stLogicalPoint.y;
		if(m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0)
		{
			fX = stLogicalPoint.x - m_Offset_Coordinate.x;	//�ı�����ԭ��
			fZ = stLogicalPoint.y - m_Offset_Coordinate.y;	//�ı�����ԭ��
		}
		strTip1.Format(_T("X = %.1f mm\nZ = %.1f mm\n"),fX, fZ);
	}
	else if(m_nAxisSelect == 2)	// YZ
	{	
		float fY = stLogicalPoint.x;
		float fZ = stLogicalPoint.y;
		if(m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0)
		{
			fY = stLogicalPoint.x - m_Offset_Coordinate.x;	//�ı�����ԭ��
			fZ = stLogicalPoint.y - m_Offset_Coordinate.y;	//�ı�����ԭ��
		}
		strTip1.Format(_T("Y = %.1f mm\nZ = %.1f mm\n"),fY, fZ);
	}
	else
	{
		float fX = stLogicalPoint.x;
		float fY = stLogicalPoint.y;
		if(m_Offset_Coordinate.y>=0 || m_Offset_Coordinate.x>=0)
		{
			fX = stLogicalPoint.x - m_Offset_Coordinate.x;	//�ı�����ԭ��
			fY = stLogicalPoint.y - m_Offset_Coordinate.y;	//�ı�����ԭ��
		}
		strTip1.Format(_T("X = %.1f mm\nY = %.1f mm\n"),fX, fY);
	}
	// Song Chenguang modified start on 2013-07-11
#if 0//_HAFEI
	// Ĭ��Cɨ1Ϊ���Cɨ2Ϊ�ֲ�
	float fFenCengAmp = 0.0f;
	float fThickness = 0.0f;
	if (m_nObjectNum == 1)	// ��ǰΪCɨ1
	{
		if((nPixelX >= 0) && (nPixelX < m_nMemXLENGTH) && (nPixelY < m_nMemYLENGTH) && (nPixelY >= 0))
		{
			fFenCengAmp = m_pCScanParamView->m_pCScanView2->m_pPixel[nPixelX][nPixelY];
		}
		else
		{
			fFenCengAmp = 0;
		}

		fThickness = fValue;
	}
	else if (m_nObjectNum == 2)	// ��ǰΪCɨ2
	{
		fFenCengAmp = fValue;

		if((nPixelX >= 0) && (nPixelX < m_nMemXLENGTH) && (nPixelY < m_nMemYLENGTH) && (nPixelY >= 0))
		{
			fThickness = m_pCScanParamView->m_pCScanView2->m_pPixel[nPixelX][nPixelY];
		}
		else
		{
			fThickness = 0;
		}
	}

	float fdBHafei = 0.0f;
	if (fabs(fFenCengAmp) > 1E-2)
		fdBHafei = 0 - 20 * log10(g_fDBZeroAmp / fFenCengAmp);
	else
		fdBHafei = 0.0f;
	// �����϶��
	float fKongXiLv = CalcKongXiLv(fdBHafei, fThickness);

	strTip2.Format(_T("A = %.1f %%\nA = %0.1f dB\n��϶�� = %.1f %%"),fFenCengAmp, fdBHafei, fKongXiLv);
#endif
	// Song Chenguang modified end on 2013-07-11
	CString strTip = strTip1 + strTip2;
	m_ToolTip.UpdateTipText(strTip,this);
    //yachang.huang add start on 2012-8-16
	if (TRUE == m_bISRotateCenterExist)
	{
		CClientDC dc(this);
		dc.SetROP2(R2_NOT);
		dc.MoveTo(m_ptObliqueStartPoint);
		dc.LineTo(m_ptObliqueEndPoint);
		dc.MoveTo(m_ptRotateCenter);
		dc.LineTo(point);
		double k;
//		float Angle;
		if (point.x != m_ptRotateCenter.x)
		{
			k = static_cast<double>(point.y - m_ptRotateCenter.y)/static_cast<double>(point.x - m_ptRotateCenter.x);
			m_fRotaAngle = static_cast<float>(0 - atan(k)*180/3.141516);
		}
		m_ptObliqueStartPoint = m_ptRotateCenter;
		m_ptObliqueEndPoint = point;

	} 
	 //yachang.huang add end on 2012-8-16
	
	CScrollView::OnMouseMove(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseMove"),"Leave");
}

void CCScanView::OnMeasureOnOff()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMeasureOnOff"),"Enter");
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->ShowControlBar(&pFrame->m_wndCScanDlgBar,!pFrame->m_wndCScanDlgBar.IsWindowVisible(),FALSE,FALSE);
	m_bMeasureOn = pFrame->m_wndCScanDlgBar.IsWindowVisible();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMeasureOnOff"),"Leave");
}

void CCScanView::OnUpdateMeasureOnOff(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateMeasureOnOff"),"Enter");
	// TODO: Add your command update UI handler code here
	if (m_bMeasureOn)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateMeasureOnOff"),"Leave");	
}

void CCScanView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonDown"),"Enter");
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = NULL;
	if (m_nObjectNum == 1)
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	else
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	// songchenguang add start on 2012-05-21
	m_pCScanParamView->UpdateData(TRUE);
	// songchenguang add end on 2012-05-21
	// Song Chenguang modify start on 2013-08-08
//	CButton* i_button = (CButton*)pCScanParamView->GetDlgItem(IDC_CHECK_ORIGIN);
//	if (i_button)
//	{
		
		if ( pCScanParamView->m_bChangeOrigin )
		{
			LOGICAL_POINT_t pt = Client2Logical(point);
			CMechanicalControlPanel* pMechanicalPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
			if (!pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
				m_Offset_Coordinate = pt;
			else
			{
/*				// �޸�memDC��ˢ��
				CMemDC dcMem(&m_MemDC, CRect(0,0,m_nMemXLENGTH, m_nMemYLENGTH));
				CPoint ptMemOffset = Logical2MemDC(pt);
				int tenp = abs(ptMemOffset.y);
				dcMem.BitBlt(0, 0, m_nMemXLENGTH, tenp, &m_MemDC, 0, m_nMemYLENGTH - abs(ptMemOffset.y)+1, SRCCOPY);
				dcMem.BitBlt(0, m_nMemYLENGTH - ptMemOffset.y + 1, m_nMemXLENGTH, ptMemOffset.y, &m_MemDC, 0, 0, SRCCOPY);
				m_MemDC.FillSolidRect(0, 0, m_nMemXLENGTH, m_nMemYLENGTH, RGB(0,0,0));
				m_MemDC.BitBlt(0, 0, m_nMemXLENGTH, m_nMemYLENGTH, &dcMem, 0, 0, SRCCOPY);
*/
				float fTotalY = PI * m_dPipeDiameter;
				float fYOffset = 0;
				if (pCScanParamView->m_nPipeXMode == 0)
					fYOffset = (pt.y/360.f) * fTotalY;
				else
					fYOffset = pt.y;
				
				float fYu = fTotalY - fYOffset;
				CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
				CUltrasonicImmersedTestDoc* pDoc = GetDocument();
				int nCardNum = pApp->GetCardNum();

				for (int i=0; i<nCardNum; i++)
				{
					for (int j=0; j<2; j++)
					{
						int nChannelIndex = i*2+j;
						POSITION pos = pDoc->m_lstFlawData[nChannelIndex].GetHeadPosition();

						while(pos)
						{
							POSITION oldPos = pos;
							CFeatureInfo data = pDoc->m_lstFlawData[nChannelIndex].GetNext(pos);

							if (data.m_dR > fYOffset)
								data.m_dR -= fYOffset;
							else
								data.m_dR += fYu;
							
							pDoc->m_lstFlawData[nChannelIndex].SetAt(oldPos, data);
						}
					}
				}

				DrawPicture(&m_MemDC);
			}			

			Invalidate(TRUE);
			pCScanParamView->m_bChangeOrigin = FALSE;
			pCScanParamView->UpdateData(FALSE);
			
			// wangbingfu add start on 2013-01-08
			CCScanView* pCScanView = NULL;
			if(m_nObjectNum == 1)
			{
				pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);			
			}
			else if(m_nObjectNum == 2)
			{
				pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
			}
			else
			{
				;//
			}
			pCScanView->m_Offset_Coordinate = m_Offset_Coordinate;
/*			if (pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
			{
				// �޸�memDC��ˢ��
				CMemDC dcMem(&(pCScanView->m_MemDC), CRect(0,0,m_nMemXLENGTH, m_nMemYLENGTH));
				CPoint ptMemOffset = Logical2MemDC(pt);
				dcMem.BitBlt(0, 0, m_nMemXLENGTH, m_nMemYLENGTH - ptMemOffset.y, &(pCScanView->m_MemDC), 0, ptMemOffset.y+1, SRCCOPY);
				dcMem.BitBlt(0, m_nMemYLENGTH - ptMemOffset.y + 1, m_nMemXLENGTH, ptMemOffset.y, &(pCScanView->m_MemDC), 0, 0, SRCCOPY);
				pCScanView->m_MemDC.BitBlt(0, 0, m_nMemXLENGTH, m_nMemYLENGTH, &dcMem, 0, 0, SRCCOPY);
			}
*/			pCScanView->Invalidate(TRUE);
			// wangbingfu add end on 2013-01-08
			
		}
//	}
	// Song Chenguang modify end on 2013-08-08
	if (m_bMeasureOn || pCScanParamView->m_bSurfaceFlag || pCScanParamView->m_bNoteFlag)
	{
		if(pCScanParamView->m_bSurfaceFlag)
		{
			CClientDC dc(this);
			if (pCScanParamView->m_nSurfaceType == 0)
			{
				InvertRegion(&dc, m_ptFrom, m_ptTo, eRECT);
			} 
			else if(pCScanParamView->m_nSurfaceType == 1)
			{
				InvertRegion(&dc, m_ptFrom, m_ptTo, eCIRCLE);
			}
			else if (pCScanParamView->m_nSurfaceType == 2)
				InvertRegion(&dc, m_ptFrom, m_ptTo, eELLIPSE);
	         // yachang.huang add start on 2012-5-31 
			else if (pCScanParamView->m_nSurfaceType == 3)
			{
//				InvertPolygonRegion(&dc, m_Poly, m_nPolyNum + 1);
			}
	         // yachang.huang add end on 2012-5-31 							
		}
		else if(pCScanParamView->m_bNoteFlag)
		{
			if(!m_bNotEditFlag)	//����ɫ
			{
				CClientDC dc(this);
				if (pCScanParamView->m_nNoteType == 0)
				{
					InvertRegion(&dc, m_ptFrom, m_ptTo, eRECT);
				} 
				else if(pCScanParamView->m_nNoteType == 1)
				{
					InvertRegion(&dc, m_ptFrom, m_ptTo, eCIRCLE);
				}
				else if (pCScanParamView->m_nNoteType == 2)
				{
					InvertRegion(&dc, m_ptFrom, m_ptTo, eELLIPSE);
				}
				else
				{
					;
				}
			}
			m_bNotEditFlag = FALSE;			
		}

		//yachang.huang add start on 2012-6-1  ��ȡ����ζ���
		if ((TRUE == m_bMeasureOn && ePolygon == m_eMeasureType) ||
			(pCScanParamView->m_bSurfaceFlag && pCScanParamView->m_nSurfaceType == 3))
		{
			CClientDC dc(this);
			// machao add start on 2013-12-24
			int nTemp = POLY_VERTEX_NUM - 2;
			if (m_nPolyNum > nTemp)
			{
				CString str;
				str.Format(_T("����ζ���������%d����"), POLY_VERTEX_NUM);
				MessageBox(str);
				return;
			}
			// machao add end on 2013-12-24
			InvertPolygonRegion(&dc, m_Poly, m_nPolyNum+1);    	//����֮ǰ���Ķ����  yachang.huang add  on 2012-6-25
			m_nPolyNum++;	
			m_Poly[m_nPolyNum] = point;

			InvertPolygonRegion(&dc, m_Poly, m_nPolyNum+1);

			if (m_bMeasureOn && ePolygon == m_eMeasureType)
			{
				this->CalculatePolygonArea();
				CString str;
				str.Format(_T("Selected:  %.1fmm^2"),m_fPolygonArea);
				CStatic* pWnd = (CStatic*)pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);
			}

		}
		//yachang.huang add end on 2012-6-1
		m_ptFrom = point;
		m_ptTo = point;
		m_bTracking = TRUE;

		SetCapture();
	}
	// machao add start on 2013-08-23
	if (m_bMouseGainRangeSelect)
	{
		m_bDrawGainRange = TRUE;
		CClientDC dc(this);
		DrawPartGainCompensateRange(&dc,m_fWStart, m_fZStart, m_fWStop,m_fZStop);
		m_fWStart = Client2Logical(point).x;
		m_fWStop = Client2Logical(point).x;
		m_fZStop = Client2Logical(point).y;
		m_fZStart = Client2Logical(point).y;
		SetCapture();
	}
	// machao add end on 2013-08-23
    //yachang.huang add start on 2012-8-16
	if (TRUE == m_bRota )
	{
		if (FALSE == m_bISRotateCenterExist)
		{
			m_ptRotateCenter = point;
			m_ptHorizontalEndPoint.x = m_ptRotateCenter.x + 200;
			m_ptHorizontalEndPoint.y = m_ptRotateCenter.y;
			CClientDC dc(this);
			dc.SetROP2(R2_NOT);
			dc.MoveTo(m_ptRotateCenter);
			dc.LineTo(m_ptHorizontalEndPoint);
			m_ToolTip.EnableToolTips(FALSE);
			m_bISRotateCenterExist = TRUE;
		}
		else
		{
            Invalidate(TRUE);
/*			m_bISRotateCenterExist = FALSE;*/
		}
	}
	//yachang.huang add end on 2012-8-16
	CScrollView::OnLButtonDown(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonDown"),"Leave");
}

void CCScanView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonUp"),"Enter");
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = NULL;

	if(m_nObjectNum == 1)
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	else
		pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	
	if (pCScanParamView->m_bSurfaceFlag || pCScanParamView->m_bNoteFlag)	// ����ͳ��
	{
		if (m_bTracking)
		{
			m_bTracking = FALSE;

			if (GetCapture() == this)
				::ReleaseCapture();

			// ��Ϊ��ѡ���ˣ���ѡ��Χ��һ���ڿ�
			if (pCScanParamView->m_bSurfaceFlag && pCScanParamView->m_nSurfaceSelType == 0)
			{				
				// ��Χ��������
 				if (!m_bSelSurfaceCenter && !m_bSelSurfaceEdge && m_bEdgePoint)		// machao modify on 2013-09-18
				{
					//�ı�ԭ��
					m_MemDC.SetViewportOrg(0, m_nMemYLENGTH);

					if (m_pCScanParamView->m_nSurfaceType == 0)
					{
						DrawMemDCRegion(&m_MemDC, m_ptFrom, m_ptTo, eRECT, FALSE);
					} 
					else if(m_pCScanParamView->m_nSurfaceType == 1)
					{
						DrawMemDCRegion(&m_MemDC, m_ptFrom, m_ptTo, eCIRCLE, FALSE);
					}
					else if (m_pCScanParamView->m_nSurfaceType == 2)
					{
						DrawMemDCRegion(&m_MemDC, m_ptFrom, m_ptTo, eELLIPSE, FALSE);
					}
					else if (m_pCScanParamView->m_nSurfaceType == 3)
					{
						DrawMemDCRegion(&m_MemDC, m_ptFrom, m_ptTo, ePolygon, FALSE);
					}
					else
					{
						;//
					}

					m_MemDC.SetViewportOrg(0, m_nMemYLENGTH * -1);

					if (m_pCScanParamView->m_nSurfaceType == 1)	// Բ
					{
						CPoint ptRightTop;
						ptRightTop.x = m_ptFrom.x > m_ptTo.x ? m_ptFrom.x : m_ptTo.x;
						ptRightTop.y = (m_ptFrom.y + m_ptTo.y) / 2 - abs(m_ptTo.y - m_ptFrom.y) / 2;

						CPoint ptLeftBottom;
						ptLeftBottom.x = m_ptFrom.x < m_ptTo.x ? m_ptFrom.x : m_ptTo.x;
						ptLeftBottom.y = (m_ptFrom.y + m_ptTo.y) / 2 + abs(m_ptTo.y - m_ptFrom.y) / 2;

						m_ptFrom = ptLeftBottom;
						m_ptTo = ptRightTop;
					} 
					else if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
					{
						// ��ȡ����X����С��Y
						int nMinX = m_Poly[0].x;
						int nMaxX = m_Poly[0].x;
						int nMinY = m_Poly[0].y;
						int nMaxY = m_Poly[0].y;

						for (int i=1; i<m_nPolyNum+1; i++)
						{
							if (m_Poly[i].x < nMinX)
							{
								nMinX = m_Poly[i].x;
							}

							if (m_Poly[i].x > nMaxX)
							{
								nMaxX = m_Poly[i].x;
							}

							if (m_Poly[i].y < nMinY)
							{
								nMinY = m_Poly[i].y;
							}

							if (m_Poly[i].y > nMaxY)
							{
								nMaxY = m_Poly[i].y;
							}
						}
						m_ptFrom = CPoint(nMinX, nMaxY);
						m_ptTo = CPoint(nMaxX, nMinY);
					}
					else
					{
					}

					if (m_ptFrom != m_ptTo)		// machao add on 2013-09-22
					{
						LOGICAL_REGION stRegion;
						stRegion.m_stFrom = Client2Logical(m_ptFrom);
						stRegion.m_stTo = Client2Logical(m_ptTo);
						m_lstCustumSurfaceRegion.AddTail(stRegion);
						m_bEdgePoint = FALSE;		// machao add on 2013-09-18
					}
				}
				
				if (nFlags & MK_CONTROL)
				{
					// Song Chenguang add start on 2013-06-28
					LOGICAL_POINT_t stLogicalPoint;
					stLogicalPoint = Client2Logical(point);
					
					// �жϵ��Ƿ��ڷ�Χ��
					LOGICAL_REGION stJudgeRegion = m_lstCustumSurfaceRegion.GetTail();

					if (!(stLogicalPoint.x > stJudgeRegion.m_stFrom.x && stLogicalPoint.x < stJudgeRegion.m_stTo.x &&
						stLogicalPoint.y < stJudgeRegion.m_stFrom.y && stLogicalPoint.y > stJudgeRegion.m_stTo.y))
					{
						MessageBox(_T("��ѡ�㲻�ڿ�ѡ��Χ�ڣ������¿�ѡ��Χ��ѡ�㣡"), _T("��ʾ"), MB_OK);
						CScrollView::OnLButtonUp(nFlags, point);
						return;
					}
					m_pCScanParamView->UpdateData(TRUE);

					if (m_pCScanParamView->m_bSurfaceFlag && m_pCScanParamView->m_nSurfaceSelType == 0)
					{
						int nPixelX = (int)((stLogicalPoint.x - m_dXStart) / m_fGridX);
						int nPixelY = (int)((stLogicalPoint.y - m_dYStart) / m_fGridY);

						float fValue = 0.0f; //����ֵ
						if((nPixelX >= 0) && (nPixelX < m_nMemXLENGTH) && (nPixelY < m_nMemYLENGTH) && (nPixelY >= 0))
						{
							fValue = m_pPixel[nPixelX][nPixelY];
						}
						else
						{
							fValue = 0;
						}

						float fdB = 0.0f;
						if (fabs(fValue) > 1E-2)
							fdB = 0 - 20 * log10(g_fDBZeroAmp / fValue);
						else
							fdB = 0.0f;

						static int nAddCenterCount = 0;	// Song Chenguang add on 2013-09-22
						if (m_bSelSurfaceCenter)
						{
							// ��ӵ���Ϣ
							int nCount = m_pCScanParamView->m_lstctrlCustumFlaw.GetItemCount();
							CString strIndex;
							strIndex.Format(_T("%d"), nCount + 1);
							if(nAddCenterCount == 0)	// �����¼�¼
								int nRes = m_pCScanParamView->m_lstctrlCustumFlaw.InsertItem(nCount, strIndex);
							else	// �޸�
								nCount--;

							int iSubItem = 0;
							CString strCoordinate;
							strCoordinate.Format(_T("%.1f, %.1f"), stLogicalPoint.x, stLogicalPoint.y);
							BOOL bRet = m_pCScanParamView->m_lstctrlCustumFlaw.SetItemText(nCount, ++iSubItem, strCoordinate);	
							CString strCenterDB;
							strCenterDB.Format(_T("%.1f"), fdB);
							m_fCenterDB = fdB;
							bRet = m_pCScanParamView->m_lstctrlCustumFlaw.SetItemText(nCount, ++iSubItem,strCenterDB);	// DB

							m_bSelSurfaceCenter = FALSE;
							m_bSurfaceCenterSelected = TRUE;
							// machao add start on 2013-09-22
							stJudgeRegion.m_nInstallEdgeDB = 1;
							POSITION pos = m_lstCustumSurfaceRegion.GetTailPosition();
							m_lstCustumSurfaceRegion.SetAt(pos,stJudgeRegion);
							// machao add end on 2013-09-22
						}
						else if (m_bSelSurfaceEdge)
						{		
							nAddCenterCount = 0;	// Song Chenguang add on 2013-09-22
							// ����ԵdB���öԻ���
							CEdgeDBSetDlg dlg;
							dlg.m_fEdgeDB = fdB;
							dlg.m_fCenterDB = m_fCenterDB;

							if (dlg.DoModal() == IDOK)
							{
								fdB = dlg.m_fEdgeDB;
								int nCount = m_pCScanParamView->m_lstctrlCustumFlaw.GetItemCount();
								CString strEdgeDB;
								strEdgeDB.Format(_T("%.1f"), fdB);
								m_pCScanParamView->m_lstctrlCustumFlaw.SetItemText(nCount-1, 3, strEdgeDB);	// DB

								float fThreshold = 100.0f / powf(10.0f, fabs(fdB) / 20.0f);

								// ��������
								LOGICAL_REGION stRegion = m_lstCustumSurfaceRegion.GetTail();
								CalcOneCustumFlaw(stRegion, fThreshold);

								// ��ȡ������棬��Ŀǰ����ӽ�ȥ
								VARIANT Result = m_pCScanParamView->m_PlateDrawCtrl.GetPlaneFlawPoints2();

								PlaneFlawHeader PFH = {0};
								long lLBound = 0;
								long lUBound = 0;

								::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//һά�����±�
								::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//һά�����ϱ�

								char* pBuf;
								SafeArrayAccessData(Result.parray,   (void   **)&pBuf);   

								UINT Size = (lUBound-lLBound+1);
								
								if (m_pcCustumFlawBuffer != NULL)
								{
									char* pTemp = new char[m_lCustumFlawBufferSize + Size];
									memcpy(pTemp, m_pcCustumFlawBuffer, m_lCustumFlawBufferSize);
									memcpy(pTemp + m_lCustumFlawBufferSize, pBuf, Size);
									delete []m_pcCustumFlawBuffer;
									m_pcCustumFlawBuffer = pTemp;
									m_lCustumFlawBufferSize = m_lCustumFlawBufferSize + Size;
									// machao add start on 2013-09-17
									long lSize = (long)Size;
									m_lstCustumFlawBufferSize.AddTail(lSize);
									stJudgeRegion.m_nInstallEdgeDB = 2;
									POSITION pos = m_lstCustumSurfaceRegion.GetTailPosition();
									m_lstCustumSurfaceRegion.SetAt(pos,stJudgeRegion);
									// machao add end on 2013-09-17
								}
								else
								{
									m_pcCustumFlawBuffer = new char[Size];
									memcpy(m_pcCustumFlawBuffer, pBuf, Size);
									m_lCustumFlawBufferSize = Size;
									// machao add start on 2013-09-18
									m_lstCustumFlawBufferSize.AddTail(m_lCustumFlawBufferSize);
									stJudgeRegion.m_nInstallEdgeDB = 2;
									POSITION pos = m_lstCustumSurfaceRegion.GetTailPosition();
									m_lstCustumSurfaceRegion.SetAt(pos,stJudgeRegion);
									// machao add end on 2013-09-18
								}
								m_bEdgePoint = TRUE;	// machao add on 2013-09-18

								SafeArrayUnaccessData(Result.parray);
								SafeArrayDestroy(Result.parray);

								// ��ʾ��ϸ���
								m_pCScanParamView->OnBnClickedButtonSurfaceDetail();

								m_bSelSurfaceEdge = FALSE;
								m_bSurfaceCenterSelected = FALSE;
							}
						}
					}
					// Song Chenguang add end on 2013-06-28
				}

				Invalidate(TRUE);
			}
		}
	}
	else if (m_bMeasureOn)
	{
		if (m_bTracking)
		{
			m_bTracking = FALSE;

			if (GetCapture() == this)
				::ReleaseCapture();

			CClientDC dc(this);			
			
			if (m_eMeasureType == ePolygon)
			{
	//			InvertPolygonRegion(&dc, m_Poly, m_nPolyNum + 1);
				this->CalculatePolygonArea();
				CString str;
				str.Format(_T("Selected:  %.1fmm^2"),m_fPolygonArea);
				CStatic* pWnd = (CStatic*)pFrame->m_wndCScanDlgBar.GetDlgItem(IDC_SCAN_INFO);
				pWnd->SetWindowText(str);
			}
			else
				InvertRegion(&dc, m_ptFrom, m_ptTo, m_eMeasureType,FALSE);
		}

	}
	// machao add start on 2013-08-23
	if (m_bMouseGainRangeSelect)
	{
		if (m_bDrawGainRange)
		{
			m_bDrawGainRange = FALSE;
			if (GetCapture() == this)
				::ReleaseCapture();
		}
	}
	// machao add end on 2013-08-23
	CScrollView::OnLButtonUp(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonUp"),"Leave");
}
void CCScanView::DrawMemDCRegion(CDC* pDC, CPoint ptFrom, CPoint ptTo, const MEASURE_TYPE& eType, BOOL bFillFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawMemDCRegion"),"Enter");
	float fXConvertValue = 1 / m_fGridX;
	float fYConvertValue = 1 / m_fGridY;

	LOGICAL_POINT_t ptLogicalFrom;
	LOGICAL_POINT_t ptLogicalTo;
	ptLogicalFrom = Client2Logical(ptFrom);
	ptLogicalTo = Client2Logical(ptTo);
/*	// Song Chenguang add start on 2013-09-06
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
	int nAxisIndex = pMovePathView->m_GraphMove.GetAxisSelect();
	if(m_pCScanParamView->m_nPipeXMode == 0 && (nAxisIndex == 3 || nAxisIndex == 4))
	{
		ptLogicalFrom.y = ptLogicalFrom.y / 360.0f * PI * m_dPipeDiameter;
		ptLogicalTo.y = ptLogicalTo.y / 360.0f * PI * m_dPipeDiameter;
	}
	// Song Chenguang add end on  2013-09-06
*/
	CPoint ptMemDCFrom;
	CPoint ptMemDCTo;

	ptMemDCFrom.x = (int)((ptLogicalFrom.x - m_dXStart) * fXConvertValue);
	ptMemDCFrom.y = -(int)((ptLogicalFrom.y - m_dYStart) * fYConvertValue);
	ptMemDCTo.x = (int)((ptLogicalTo.x - m_dXStart) * fXConvertValue);
	ptMemDCTo.y = -(int)((ptLogicalTo.y - m_dYStart) * fYConvertValue);

	CPen pen;
	CPen *pOldPen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));

	CBrush br;
	CBrush *pOldBr;
	br.CreateStockObject(NULL_BRUSH);
	if(!bFillFlag)
	{
		pOldPen = pDC->SelectObject(&pen);
		pOldBr = pDC->SelectObject(&br);
	}

	if (eType == eRECT)
	{
		pDC->Rectangle(CRect(ptMemDCFrom, ptMemDCTo));
	} 
	else if(eType == eELLIPSE)
	{
		pDC->Ellipse(CRect(ptMemDCFrom, ptMemDCTo));
	}

	else if (eType == ePolygon)
	{		
		// machao modify start on 2013-12-24
		CPoint ptMemDCPoly[POLY_VERTEX_NUM];  //����ζ�������
		LOGICAL_POINT_t ptLogicalPoly[POLY_VERTEX_NUM];  //����ζ�������
		// machao modify end on 2013-12-24
		for (int i = 0; i < m_nPolyNum + 1; i++)
		{
			ptLogicalPoly[i] = Client2Logical(m_Poly[i]);
			ptMemDCPoly[i].x = (int)((ptLogicalPoly[i].x - m_dXStart) * fXConvertValue);
			ptMemDCPoly[i].y = -(int)((ptLogicalPoly[i].y - m_dYStart) * fYConvertValue);
		}

		for (int i = 0; i < m_nPolyNum + 1; i++)
		{
			if(i == 0)
			{
				pDC->BeginPath();
				pDC->MoveTo(ptMemDCPoly[i]);
			}
			else
			{
				pDC->LineTo(ptMemDCPoly[i]);
			}

			if(i == m_nPolyNum)
			{
				pDC->EndPath();
				pDC->FillPath();
			}		
		}

		for (int i = 0; i < m_nPolyNum + 1; i++)
		{
			pDC->MoveTo(ptMemDCPoly[i]);
			if(i == m_nPolyNum)
			{
				pDC->LineTo(ptMemDCPoly[0]);
			}
			else
			{
				pDC->LineTo(ptMemDCPoly[i + 1]);
			}			
		}
	}
	else if (eType == eCIRCLE)
	{
		int nLeft = ptMemDCFrom.x < ptMemDCTo.x ? ptMemDCFrom.x : ptMemDCTo.x;
		int nTop = (ptMemDCTo.y + ptMemDCFrom.y)/2 - abs(ptMemDCTo.x - ptMemDCFrom.x)/2;
		int nRight = ptMemDCFrom.x < ptMemDCTo.x ? ptMemDCTo.x : ptMemDCFrom.x;
		int nBottom = (ptMemDCTo.y + ptMemDCFrom.y)/2 + abs(ptMemDCTo.x - ptMemDCFrom.x)/2;

		pDC->Ellipse(CRect(nLeft, nTop, nRight,nBottom));
	}
	if (eType == eLINE)
	{
		pDC->MoveTo(ptMemDCFrom.x,ptMemDCFrom.y);//ֱ��������б��
		pDC->LineTo(ptMemDCTo.x,ptMemDCTo.y);
	} 
	else
	{
		;//
	}
	
	if(!bFillFlag)
	{
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBr);
	}

	pen.DeleteObject();
	br.DeleteObject();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawMemDCRegion"),"Leave");
}
/** 
* ����ɫ����
* @param     pDC [in]: DCָ��
* @param     ptFrom [in]: ���
* @param     ptTo [in]: �յ�
* @return    void
* @since     1.00
*/
void CCScanView::InvertRegion(CDC* pDC, CPoint ptFrom, CPoint ptTo, const MEASURE_TYPE& eType, BOOL bErase)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertRegion"),"Enter");
	int nOldMode = pDC->SetROP2(R2_NOT);


	if (eType == eRECT)
	{
		pDC->Rectangle(CRect(ptFrom, ptTo));
	} 
	else if(eType == eELLIPSE)
	{
		pDC->Ellipse(CRect(ptFrom, ptTo));
	}

	//yachang.huang add start on 2012-5-31    
	else if (eType == ePolygon)
	{		
		if (1 == m_nPolyNum)
		{
			pDC->MoveTo(m_Poly[1]);
			pDC->LineTo(m_Poly[0]);
		}
		else if (m_nPolyNum > 1)
		{
			pDC->BeginPath();
			CPoint pts[POLY_VERTEX_NUM];
			::memcpy(pts, m_Poly, sizeof(CPoint)*30);
			pts[m_nPolyNum+1] = m_Poly[0];
			pDC->Polygon(pts, m_nPolyNum+2);

			pDC->EndPath();//����·��
			pDC->FillPath();
		}

	}
	//yachang.huang add end on 2012-6-1    


	else if(eType == eTRIANGLE)
	{
		
		if(!bErase)
		{
			//�ı�ԭ��
			m_MemDC.SetViewportOrg(0, m_nMemYLENGTH);
			int nMode = m_MemDC.SetROP2(R2_NOT);			
			DrawMemDCRegion(&m_MemDC, ptFrom, ptTo, eLINE, FALSE);
			m_MemDC.SetROP2(nOldMode);
			LINE_T line;
			line.p1 = ptFrom;
			line.p2 = ptTo;
			m_vecLine.push_back(line);
		}
		else
		{
			pDC->MoveTo(ptFrom.x,ptFrom.y);//ֱ��������б��
			pDC->LineTo(ptTo.x,ptTo.y);
		}		

		pDC->MoveTo(ptFrom.x,ptFrom.y);
		pDC->LineTo(ptTo.x,ptFrom.y);

		pDC->MoveTo(ptTo.x,ptFrom.y);
		pDC->LineTo(ptTo.x,ptTo.y);
	}
	else if (eType == eLINE)
	{
		CRect rect;
		GetClientRect(&rect);

		pDC->MoveTo(ptFrom.x, 0);
		pDC->LineTo(ptFrom.x, rect.Height() - CSCAN_SCALE_Y_LENGTH);

		pDC->MoveTo(CSCAN_SCALE_X_LENGTH, ptFrom.y);
		pDC->LineTo(rect.Width(), ptFrom.y);
	}
	else if (eType == eCIRCLE)
	{
		int nLeft = ptFrom.x < ptTo.x ? ptFrom.x : ptTo.x;
		int nTop = (ptTo.y + ptFrom.y)/2 - abs(ptTo.x - ptFrom.x)/2;
		int nRight = ptFrom.x < ptTo.x ? ptTo.x : ptFrom.x;
		int nBottom = (ptTo.y + ptFrom.y)/2 + abs(ptTo.x - ptFrom.x)/2;

		pDC->Ellipse(CRect(nLeft, nTop, nRight,nBottom));
	}

	pDC->SetROP2(nOldMode);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertRegion"),"Leave");
}


void CCScanView::InvertPolygonRegion(CDC* pDC, CPoint* ptArray, const int& nSize)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertPolygonRegion"),"Enter");
	if (nSize > POLY_VERTEX_NUM)
	{
		CString str;
		str.Format(_T("����ζ���������%d����"), POLY_VERTEX_NUM);
		MessageBox(str);
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertPolygonRegion"),"Leave");
		return;
	}

	int nOldMode = pDC->SetROP2(R2_NOT);

	if (2 == nSize)
	{
		pDC->MoveTo(m_Poly[1]);
		pDC->LineTo(m_Poly[0]);
	}
	else if (nSize > 2)
	{
		pDC->BeginPath();
		CPoint pts[POLY_VERTEX_NUM];	// machao modify start on 2013-12-24
		::memcpy(pts, ptArray, nSize*sizeof(CPoint));
		pts[nSize] = ptArray[0];
		pDC->Polygon(pts, nSize + 1);

		pDC->EndPath();//����·��
		pDC->FillPath();
	}
	pDC->SetROP2(nOldMode);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertPolygonRegion"),"Leave");
}

void CCScanView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnVScroll"),"Enter");
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;
	GetScrollRange(SB_VERT, &minpos,&maxpos);

	//�õ���ǰ��������λ��
	int curpos = GetScrollPos(SB_VERT);
	//�õ���ǰ����������Ϣ
	SCROLLINFO	info;
	GetScrollInfo(SB_VERT, &info, SIF_ALL);
	//������Ӧ���ѡ���µĹ�����λ��
	switch(nSBCode)
	{
	case SB_LINELEFT://�����ߵļ�ͷ
		if(curpos>minpos)
			curpos--;
		break;
	case SB_LINERIGHT://����ұߵļ�ͷ
		if(curpos<maxpos)
			curpos++;//o.nTrackPos;
		break;
	case SB_PAGELEFT:
		if(curpos>minpos)
			curpos=max(minpos,curpos-(int)info.nPage);
		break;
	case SB_PAGERIGHT:
		if(curpos<maxpos)
			curpos=min(maxpos,curpos+(int)info.nPage);
		break;
// 	case SB_ENDSCROLL:
// 		{		
// 			Invalidate(TRUE);
// 		}
// 		break;
		//������ק�ƶ�
	case SB_THUMBPOSITION:
		curpos=nPos;
		break;
	case SB_THUMBTRACK:
		curpos=nPos;
		break;
	}
	//���ù���������λ��
	SetScrollPos(SB_VERT, curpos, TRUE);

	m_fYMove = static_cast<float>(GetScrollPos(SB_VERT) + m_dYStart);
	m_fYMoveForZuoBiao = static_cast<float>(m_dYStart - GetScrollPos(SB_VERT));
	Invalidate(TRUE);

	//ͬ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	if(pCScanParamView->m_bCscanSync)
	{
		CCScanView* pCScanView = NULL;
		if(m_nObjectNum == 1)
		{
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);			
		}
		else
		{
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		}
		pCScanView->SetScrollPos(SB_VERT, curpos, TRUE);
		pCScanView->m_fYMove = m_fYMove;
		pCScanView->m_fYMoveForZuoBiao = m_fYMoveForZuoBiao;
		pCScanView->Invalidate(TRUE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnVScroll"),"Leave");
	//CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCScanView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnHScroll"),"Enter");
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;
	GetScrollRange(SB_HORZ, &minpos,&maxpos);

	//�õ���ǰ��������λ��
	int curpos = GetScrollPos(SB_HORZ);
	//�õ���ǰ����������Ϣ
	SCROLLINFO	info;
	GetScrollInfo(SB_HORZ, &info, SIF_ALL);

	//������Ӧ���ѡ���µĹ�����λ��
	switch(nSBCode)
	{
	case SB_LINELEFT://�����ߵļ�ͷ
		if(curpos>minpos)
			curpos--;
		break;
	case SB_LINERIGHT://����ұߵļ�ͷ
		if(curpos<maxpos)
			curpos++;//o.nTrackPos;
		break;
	case SB_PAGELEFT:
		if(curpos>minpos)
			curpos=max(minpos,curpos-(int)info.nPage);
		break;
	case SB_PAGERIGHT:
		if(curpos<maxpos)
			curpos=min(maxpos,curpos+(int)info.nPage);
		break;
// 	case SB_ENDSCROLL:
// 		{			
// 			Invalidate(TRUE);
// 		}
//		break;
		//������ק�ƶ�
	case SB_THUMBPOSITION:
		curpos=nPos;
		break;
	case SB_THUMBTRACK:
		curpos=nPos;
		break;
	}
	//���ù���������λ��
	SetScrollPos(SB_HORZ, curpos, TRUE);

	m_fXMove = static_cast<float>(GetScrollPos(SB_HORZ) + m_dXStart);
	Invalidate(TRUE);
	
	//ͬ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	if(pCScanParamView->m_bCscanSync)
	{
		CCScanView* pCScanView = NULL;
		if(m_nObjectNum == 1)
		{
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);			
		}
		else
		{
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		}
		pCScanView->SetScrollPos(SB_HORZ, curpos, TRUE);
		pCScanView->m_fXMove = m_fXMove;
		pCScanView->Invalidate(TRUE);
	}
	//CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnHScroll"),"Leave");
}

void CCScanView::OnRulerMeasure()
{
	// TODO: �ڴ���������������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRulerMeasure"),"Enter");
	m_eMeasureType = eTRIANGLE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRulerMeasure"),"Leave");
}

void CCScanView::OnRectMeasure()
{
	// TODO: �ڴ���������������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRectMeasure"),"Enter");

	m_eMeasureType = eRECT;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRectMeasure"),"Leave");
}

void CCScanView::OnEllipseMeasure()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnEllipseMeasure"),"Enter");
	// TODO: �ڴ���������������

	m_eMeasureType = eELLIPSE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnEllipseMeasure"),"Leave");	
}

//yachang.huang add start on 2012-5-31
void CCScanView::OnPolygonMeasure()
{
	// TODO: �ڴ���������������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPolygonMeasure"),"Enter");
	m_eMeasureType = ePolygon;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnPolygonMeasure"),"Leave");	
}
//yachang.huang add end on 2012-5-31

void CCScanView::OnUpdateRulerMeasure(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateRulerMeasure"),"Enter");
	// TODO: �ڴ������������û����洦��������
// 	CMenu menu;
// 	menu.LoadMenu(IDR_MAINFRAME);
// 	menu.CheckMenuRadioItem(ID_RULER_MEASURE,ID_ELLIPSE_MEASURE,ID_RULER_MEASURE,MF_BYCOMMAND);

	if (m_eMeasureType == eTRIANGLE)
	{
		//pCmdUI->SetRadio(TRUE);
		if(pCmdUI->m_pMenu != NULL)
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_RULER_MEASURE,ID_POLYGON_MEASURE,ID_RULER_MEASURE,MF_BYCOMMAND);  //yachang.huang modify start on 2012-5-31
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateRulerMeasure"),"Leave");
}

void CCScanView::OnUpdateTriangleMeasure(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateTriangleMeasure"),"Enter");
	// TODO: �ڴ������������û����洦��������
// 	CMenu menu;
// 	menu.LoadMenu(IDR_MAINFRAME);
// 	menu.CheckMenuRadioItem(ID_RULER_MEASURE,ID_ELLIPSE_MEASURE,ID_TRIANGLE_MEASURE,MF_BYCOMMAND);
	if (m_eMeasureType == eRECT)
	{
		//pCmdUI->SetRadio(TRUE);
		if(pCmdUI->m_pMenu != NULL)
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_RULER_MEASURE,ID_POLYGON_MEASURE,ID_TRIANGLE_MEASURE,MF_BYCOMMAND); //yachang.huang modify start on 2012-5-31
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateTriangleMeasure"),"Leave");
}

void CCScanView::OnUpdateEllipseMeasure(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateEllipseMeasure"),"Enter");
	// TODO: �ڴ������������û����洦��������
// 	CMenu menu;
// 	menu.LoadMenu(IDR_MAINFRAME);
// 	menu.CheckMenuRadioItem(ID_RULER_MEASURE,ID_ELLIPSE_MEASURE,ID_ELLIPSE_MEASURE,MF_BYCOMMAND);

	if (m_eMeasureType == eELLIPSE)
	{
		//pCmdUI->SetRadio(TRUE);
		if(pCmdUI->m_pMenu != NULL)
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_RULER_MEASURE,ID_POLYGON_MEASURE,ID_ELLIPSE_MEASURE,MF_BYCOMMAND);  //yachang.huang modify start on 2012-5-31
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateEllipseMeasure"),"Leave");
}


//yachang.huang add start on 2012-5-31
void CCScanView::OnUpdatePolygonMeasure(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdatePolygonMeasure"),"Enter");
	// TODO: �ڴ������������û����洦��������
	// 	CMenu menu;
	// 	menu.LoadMenu(IDR_MAINFRAME);
	// 	menu.CheckMenuRadioItem(ID_RULER_MEASURE,ID_ELLIPSE_MEASURE,ID_ELLIPSE_MEASURE,MF_BYCOMMAND);

	if (m_eMeasureType == ePolygon)
	{
		//pCmdUI->SetRadio(TRUE);
		if(pCmdUI->m_pMenu != NULL)
			pCmdUI->m_pMenu->CheckMenuRadioItem(ID_RULER_MEASURE,ID_POLYGON_MEASURE,ID_POLYGON_MEASURE,MF_BYCOMMAND);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdatePolygonMeasure"),"Leave");
}
//yachang.huang add end on 2012-5-31

float CCScanView::CalculateDBValue(CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalculateDBValue"),"Enter");
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();

	CRect rect;
	GetClientRect(&rect);
	float fPointCX = 0.0f;
	float fPointCY = 0.0f;

	// 1200��800Ӧ���Ǵ���ֵ Song Chenguang finde on 2013-05-04
	fPointCX = ((float)(point.x - m_rectScaleY.Width())*1200/m_rectCScan.Width())/m_fScaleFactor + m_fXMove;
	float y1  =static_cast<float>(rect.Height() - m_rectScaleX.Height() - point.y);
	fPointCY = (y1*800/m_rectCScan.Height())/m_fScaleFactor +m_fYMove;

	//����DBֵ
	float fDBValue = 0.0f;//��ǰ���DBֵ
	//��������
	for(POSITION pos = pDoc->m_lstFlawData[0].GetHeadPosition();pos; )
	{
		CFeatureInfo ndata = pDoc->m_lstFlawData[0].GetNext(pos);

		if(fabs(ndata.m_dX - fPointCX) < 0.01 && fabs(ndata.m_dY - fPointCY) < 0.01)
		{
			//�ж�բ��(Ĭ��һբ��)
			fDBValue = 20*log10(ndata.m_pfFeatures[0][3]*MAX_HEIGHT/100.0f);
			break;
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalculateDBValue"),"Leave");
	return fDBValue;
}

void CCScanView::GetCscanBackColor()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetCscanBackColor"),"Enter");
	if (m_pCScanParamView->m_nColorMode == 0)//ʹ�ûҶ�ͼ��
	{
		m_cscan_back_color = RGB(255, 255, 255);
	}
	else
	{
		//Machao modify start on 2013-07-4
//		m_cscan_back_color = RGB(100, 100, 100);
		m_cscan_back_color = m_pCScanParamView->m_clrBack1;
		//Machao modify end on 2013-07-4
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetCscanBackColor"),"Leave");
}
//���������
void CCScanView::DrawPictureErrorAlarm()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Enter");
	int nCFeatureIndex = m_pCScanParamView->m_nFeatureIndex;
	int nFeatureIndex = m_pCScanParamView->m_nFeatureIndex + 1;	//����ѡ��
	if(nCFeatureIndex > 5) //��������
	{
		if(m_bFileGateFlag[g_dwActiveChannel][1] == FALSE || m_bFileGateFlag[g_dwActiveChannel][2] == FALSE)
		{
			MessageBox(_T("���ܳ�����ȷ��ɨ��ʱբ��1��բ��2���ѿ���"), _T("��ʾ"), MB_OK);
		}

		if(nCFeatureIndex == 6)
		{
			if(m_bFileFeatureFlag[4] == FALSE)
			{
				MessageBox(_T("��󲨷�ʱ��MP_MTimeû�п���������ѡ��ó���"), _T("��ʾ"), MB_OK);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
				return;
			}
		}
		else if(nCFeatureIndex == 7 || nCFeatureIndex == 8 || nCFeatureIndex == 9 )
		{
			if(m_bFileFeatureFlag[4] == FALSE)
			{
				MessageBox(_T("��󲨷�ʱ��MP_MTimeû�п���������ѡ��ó���"), _T("��ʾ"), MB_OK);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
				return;
			}
			else if(m_bFileFeatureFlag[6] == FALSE)
			{
				MessageBox(_T("��󲨷��ֵMP_MAltitudeû�п���������ѡ��ó���"), _T("��ʾ"), MB_OK);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
				return;
			}
		}
	}
	else
	{
		if(!m_bFileGateFlag[g_dwActiveChannel][m_nGateIndex])
		{
			CString strText = _T("����ѡ���բ�ţ�բ����ѡ��:");
			PictureGateText(1, strText);
			CString strText1 = _T("���е�һ��բ�� ");
			CString strText2 = _T("���л�ͨ��");
			m_strPictureGate = m_strPictureGate + strText1 + strText2;
			MessageBox(m_strPictureGate);
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
			return;
		}
		if(!m_bFileFeatureFlag[nFeatureIndex])
		{
			CString strText = _T("����ѡ��˳���ģʽ������ģʽ��ѡ��:");
			PictureModeText(1, strText);
			CString strText1 = _T("���е�һ��");
			m_strPictureMode = m_strPictureMode + strText1;
			MessageBox(m_strPictureMode, _T("��ʾ"), MB_OK);
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
			return;
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureErrorAlarm"),"Leave");
}

int CCScanView::GetPictureMode(int nCFeatureIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetPictureMode"),"Enter");
	int nMode = 0;
	if(nCFeatureIndex == 0 || nCFeatureIndex == 1 || nCFeatureIndex == 2 || nCFeatureIndex == 3)	// ���
	{
		nMode = 0;
	}
	else if(nCFeatureIndex == 4 || nCFeatureIndex == 5)	// ����
	{
		nMode = 1;
	}
	else if(nCFeatureIndex == 6)	//����
	{
		nMode = 2;
	}
	else if(nCFeatureIndex == 7)	//��˥��
	{
		nMode = 3;
	}
	else if(nCFeatureIndex == 8)	//���迹
	{
		nMode = 4;
	}
	else if(nCFeatureIndex == 9)	//�ܶ�
	{
		nMode = 5;
	}
	else
	{
		;//
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetPictureMode"),"Leave");
	return nMode;
}

float CCScanView::CalcPictureValue(CFeatureInfo ndata, int nMode,int nGateIndexTemp,int nFeatureIndexTemp, 
								   int nMaxTimeIndexTemp, int nMaxAmpIndexTemp)
{
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcPictureValue"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	float fCalcValue = 0;
	if(nMode == 0)	// ���
	{
		fCalcValue = ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp];
	}
	else if(nMode == 1)	// ����
	{	
		fCalcValue = ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp];
	}
	else if(nMode == 2)	// ����
	{	
		float fRange = ndata.m_pfFeatures[2][nMaxTimeIndexTemp] - ndata.m_pfFeatures[1][nMaxTimeIndexTemp];
		if(fRange > 0)
		{
			fCalcValue = 2 * m_fThick * 1000 / Range2Dot(pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed, fRange) * 100;
		}
		else
		{
			fCalcValue = -1;
		}
	}
	else if(nMode == 3)	// ��˥��
	{
		double a = 0;
		int n1 = 1;
		int n2 = 2;
		double	z1=1.5;//ˮ�����迹
		if(ndata.m_pfFeatures[1][nMaxAmpIndexTemp] > 0 && ndata.m_pfFeatures[2][nMaxAmpIndexTemp] > 0)
		{
			a=((double)ndata.m_pfFeatures[2][nMaxAmpIndexTemp])/((double)ndata.m_pfFeatures[1][nMaxAmpIndexTemp]);
		}
		else
		{
			fCalcValue = -1;
		}
		//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
		if (a>=1)
		{
			fCalcValue = -1;
		}
		else if(a <= 0)
		{
			fCalcValue = -1;
		}
		else
		{
			float fRange = ndata.m_pfFeatures[2][nMaxTimeIndexTemp] - ndata.m_pfFeatures[1][nMaxTimeIndexTemp];
			if(fRange > 0)
			{
				fCalcValue = static_cast<float>(log((double)ndata.m_pfFeatures[1][nMaxAmpIndexTemp] / (double)ndata.m_pfFeatures[2][nMaxAmpIndexTemp]) / (2 * fRange));
			}
			else
			{
				fCalcValue = -1;
			}
			if(fCalcValue < 0)
			{
				fCalcValue = -1;
			}

		}
	}
	else if(nMode == 4)	// ���迹
	{
		double a = 0;
		double	z1=1.5;//ˮ�����迹

		if(ndata.m_pfFeatures[1][nMaxAmpIndexTemp] > 0)
		{
			a=((double)ndata.m_pfFeatures[2][nMaxAmpIndexTemp])/((double)ndata.m_pfFeatures[1][nMaxAmpIndexTemp]);
		}
		else
		{
			fCalcValue = -1;
		}
		//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
		if (a>=1)
		{
			fCalcValue = -1;
		}
		fCalcValue=static_cast<float>(z1*(-2*(a+1)-4*sqrt(a))/(2*(a-1)));
	}
	else if(nMode == 5)	///�ܶ�
	{
		double a = 0;
		int n1 = 1;
		int n2 = 2;
		double	z1=1.5;//ˮ�����迹
		if(ndata.m_pfFeatures[1][nMaxAmpIndexTemp] > 0)
			a=((double)ndata.m_pfFeatures[2][nMaxAmpIndexTemp])/((double)ndata.m_pfFeatures[1][nMaxAmpIndexTemp]);
		else
		{
			fCalcValue = -1;
		}
		//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
		if (a>=1)
		{
			fCalcValue = -1;
		}
		double szk=z1*(-2*(a+1)-4*sqrt(a))/(2*(a-1));
		float fRange = ndata.m_pfFeatures[2][nMaxTimeIndexTemp] - ndata.m_pfFeatures[1][nMaxTimeIndexTemp];
		if(fRange > 0)
		{
			double wavevelocity=2 * m_fThick * 1000 / Range2Dot(pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed, fRange) * 100;
			fCalcValue=static_cast<float>(szk/(wavevelocity / 1000) * 1000);
		}
		else
		{
			fCalcValue = -1;
		}
	}
	// wangbingf add start on 2013-01-11
	else if(nMode == 6)	// ���
	{
		float fHeight = ndata.m_pfFeatures[2][nMaxTimeIndexTemp] - ndata.m_pfFeatures[1][nMaxTimeIndexTemp];
		if(fHeight > 0)
		{
			fCalcValue = fHeight;
		}
		else
		{
			fCalcValue = -1;
		}
	}
	else
	{
		;//
	}
	// wangbingf add end on 2013-01-11
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcPictureValue"),"Leave");
	return fCalcValue;
}
//����
void CCScanView::DrawPicture(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPicture"),"Enter");
	GetCscanBackColor();
	//�ж�bFreePictureMemoyֵ��ΪTRUEʱ�����ڴ�  yachang.huang modify start on 2014-04-24
	if (bFreePictureMemoy)
	{
		FreePictureMemory();
	}
	//yachang.huang modify end on 2014-04-24
	
	BOOL bRet = MallocPictureMemory();
	if (!bRet)
		return;

	m_bDrawSurfaceFlag = FALSE;

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;

	int nFeatureIndex = 0;	//����ѡ��
	int nMaxAmpIndex = 6;
	int nGateIndexTemp = 0;
	int nFeatureIndexTemp = 0;
	int nMaxAmpIndexTemp = 6;//��󲨷��ֵ����
	int nMaxTimeIndex = 4;	
	int nMaxTimeIndexTemp = 4;//��󲨷�ʱ������
	
	// ����Cɨ����
	m_pCScanParamView->UpdateData(TRUE);
	m_fGateThreshold[0] = m_pCScanParamView->m_fGate1Threshold;
	m_fGateThreshold[1] = m_pCScanParamView->m_fGate2Threshold;
	m_fGateThreshold[2] = m_pCScanParamView->m_fGate3Threshold;
	m_fGateThreshold[3] = m_pCScanParamView->m_fGate4Threshold;
	m_nColorMode = m_pCScanParamView->m_nColorMode;
	m_nValueMode = m_pCScanParamView->m_nValueMode;
	m_nGateIndex = m_pCScanParamView->m_nGateIndex;
	m_nDrawType = m_pCScanParamView->m_nDrawType;
	m_fBoundMin = m_pCScanParamView->m_fBoundMin;
	m_fBoundMax = m_pCScanParamView->m_fBoundMax;
	m_nAmpFeatureIndex = m_pCScanParamView->m_nAmpFeatureIndex;
	m_nTimeFeatureIndex = m_pCScanParamView->m_nTimeFeatureIndex;

	nFeatureIndex = m_pCScanParamView->m_nFeatureIndex + 1;	//����ѡ��
	m_nPipeXMode = m_pCScanParamView->m_nPipeXMode;

	m_fThick = m_pCScanParamView->m_fThick;	//�������
	m_nColorDefine = m_pCScanParamView->m_nColorDefine;

	if(m_nGateIndex == -1)	//ûѡ���ĸ�բ��ʱ��������
	{
		m_pCScanParamView->OneItemSetCheck(IDC_RADIO_SELECT_GATE1, TRUE);
		m_nGateIndex = 0;
	}

	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = FALSE;
	for(int i= 1; i < 7; i++)
	{
		bFeatureFlag[i] = m_bFileFeatureFlag[i];
	}
	
	DrawPictureErrorAlarm();

	//��ջ��漰Cɨ��ͼ
	pDC->SetViewportOrg(0, 0);
	pDC->FillSolidRect(0, 0, m_CScanSize.cx, m_CScanSize.cy, m_cscan_back_color);
	
//	m_nAxisSelect = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();

	int nFillType = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nFillType;	//��䷽ʽ:��������

	float fXStep = 1; // X���
	float fYStep = 1; // Y���
	// ������(��ʱ��Ĭ��ֵ)
	if (nFillType == 0) // ����
	{
		fXStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
		fYStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;	
	} 
	else	// ����
	{
		fXStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;
		fYStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
	}
	// TODO: �ڴ���������������

	// machao add start on 2013-11-27
	if (m_nAxisSelect == 5)
	{
		if (m_nPipeXMode == 0)
		{
			m_dXEnd = 360;
		}
		else
		{
			m_dXEnd = PI * m_dPipeDiameter;
		}
	}
	// machao add end on 2013-11-27
	float fXLength = static_cast<float>(fabs(m_dXEnd - m_dXStart));	// ����X�᳤��
	float fYLength = static_cast<float>(fabs(m_dYEnd - m_dYStart)); // ����Y�᳤��

	int nXLength = int(fXLength / fXStep);	//��������X���س���
	int nYLength = int(fYLength / fYStep);	//��������Y���س���

	//��ʼ��ͼ
	int	x,y;//�������λ��

//	float fGrayScale; //��ɫɫ�ȱ���
//	int nGrayValue;	//��ɫֵ
//	int nColorIndex; //��ɫ����
	float tempAmp;
	if(m_nValueMode == 1)
	{
		tempAmp = 100;
	}
	else
	{
		tempAmp = 0;
	}
	float fCalcValue = 0.0f;	//����ֵ

	//�ı�ԭ��,�������µ�
	pDC->SetViewportOrg(0, m_nMemYLENGTH);

	nGateIndexTemp = m_nGateIndex;
	nFeatureIndexTemp = nFeatureIndex;

	for(int j = 1; j < nFeatureIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nFeatureIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxAmpIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxAmpIndexTemp--;
		}
	}

	for(int j = 1; j < nMaxTimeIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxTimeIndexTemp--;
		}
	}

	float fXConvertValue = 1 / fXStep;
	float fYConvertValue = 1 / fYStep;

	float fXPixelLength = fXStep;
	float fYPixelLength = fYStep;

	int nMode = GetPictureMode(nFeatureIndex - 1);

	// test start 2011-11-18
	CStdioFile file;
	if(FILE_DEBUG)
	{
		file.Open(_T("c:\\test.txt"), CFile::modeCreate | CFile::modeReadWrite);
		CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument(); 
		CFeatureInfo ndataOriginal(nGateNum[0], nFeatureNum);//��������
		POSITION posOriginal;
		posOriginal = pDoc->m_lstFlawDataOriginal[g_dwActiveChannel].GetHeadPosition();
	}
	// test end 2011-11-18
	
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		// wangbingfu modify start on 2013-01-09
		if(m_bGuanLianFlag && m_nAxisSelect != 4)	// ����ģʽ
		// wangbingfu modify end on 2013-01-09
		{
			if(m_nObjectNum == 1)	// ֻ���ײ� ��ͨ���ײ� ˫ͨ���˲�
			{
				if(i == 1 || i == 3 || i == 5 || i == 7)
				{
					continue;
				}
			}
			else if(m_nObjectNum == 2)	// ֻ���˲�
			{
				if(i == 0 || i == 2 || i == 4 || i == 6)
				{
					continue;
				}
			}
			else
			{
				;//
			}
		}

		// wangbingfu add start on 2013-01-09
		BOOL bPipeGuanLian = FALSE;	// ��ĳһ���2ͨ��ѡ����+�ֲ�ʱ������Ϊ����
		if(m_nAxisSelect == 4) // W1��
		{
			if((i % 2) == 0)	// ����1ͨ��
			{
				if(m_nPipeChannelSelect[i+1] == 3) // ����2ͨ������ѡ��A���+�ֲ�ʱ��C1�����C2���˲�(�ֲ�)
				{
					bPipeGuanLian = TRUE;
					if(m_nObjectNum == 1)
					{
						if(m_nCScanSelect == (i + 1))	// Cɨ1�����1ͨ��,�����
						{
							continue;
						}
					}
					else if(m_nObjectNum == 2)
					{
						if(m_nCScanSelect == i)	// Cɨ2�����2ͨ��,���ֲ�(�˲�)
						{
							continue;
						}
					}
					else
					{
						;//
					}
				}
			}

			if(!bPipeGuanLian
				&& (i != m_nCScanSelect)) // ����ѡ�е�ͨ���˳�����
			{
				continue;
			}

			nMode = GetPictureMode(nFeatureIndex - 1);
			if((i % 2) == 0)
			{
				if(m_nPipeChannelSelect[i+1] == 3)
				{
					if(m_nObjectNum == 1)	// Cɨ1
					{
						nMode = 6; //  ���
					}
				}
				else if(m_nPipeChannelSelect[i] == 1)
				{
					nMode = 6; //  ���
				}
			}
			else if((i % 2) == 1)
			{
				if(m_nPipeChannelSelect[i] == 1)
				{
					nMode = 6; //  ���
				}
			}
			else
			{
				;//
			}
		}
		
		// wangbingfu add end on 2013-01-09

		nGateIndexTemp = m_nGateIndex;

		for(int j = 0; j < m_nGateIndex; j++)
		{
			if(!m_bFileGateFlag[i][j])
			{
				nGateIndexTemp--;
			}
		}

		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			//��ȡ���ݵ㣬���Զ����е���һ����
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);
			x = (int)((ndata.m_dX - m_dXStart) * fXConvertValue + 0.5);	// ��Ϊ��������
			y = (int)((ndata.m_dY - m_dYStart) * fYConvertValue + 0.5);

			if(m_nAxisSelect == 3
				|| m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09	
			{
				y = (int)((ndata.m_dR - m_dYStart) * fYConvertValue);		
			}
			// Song Chenguang add start on 2013-05-04
			else if (m_nAxisSelect == 5)
			{
				x = (int)((ndata.m_dW - m_dXStart) * fXConvertValue);
				// machao add start on 2013-11-27
				if (m_nPipeXMode == 1)
				{
					x = (int)(x * PI * m_dPipeDiameter / 360);
				}
				// machao add end on 2013-11-27
				y = (int)((ndata.m_dZ - m_dYStart) * fYConvertValue);
			}
			// Song Chenguang add end on 2013-05-04

			// test start 2011-11-18
			//	if(FILE_DEBUG)
			//	{
			// 		if(posOriginal)
			// 		{
			// 			ndataOriginal = pDoc->m_lstFlawDataOriginal[g_dwActiveChannel].GetNext(posOriginal);
			// 		}
			//	}
			// test end 2011-11-18

			if (!(m_pCScanParamView->bHanHeFlag))
			{
				//����Cɨ����ȳ���
				if(m_pCScanParamView->m_nGateType == 0)
				{
					if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[m_nGateIndex])
					{
//						if((x >= 0 && x <= m_nMemXLENGTH) && (y >= 0 && y <= m_nMemYLENGTH))
						if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
						{	
							// test start 2011-11-18
							// 			if(FILE_DEBUG)
							// 			{
							// 				CString strInfo;
							// 				strInfo.Format(_T("%d\t\t%d\t%.3f\t\t%.3f\t\n"), x, y,  ndataOriginal.m_dX, ndataOriginal.m_dY);
							// 				file.WriteString(strInfo);
							// 			}
							// test end 2011-11-18
							//machao modify start on 2013-07-15
							if (m_bNoiseReduction)
							{
								fCalcValue = m_pfSrcData[x][y];
							}
							else
							{
								fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
								m_pfSrcData[x][y] = fCalcValue;		// machao delete on 2013-08-21
							}
							//fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
							//machao modify end on 2013-07-15

							Draw2DCAsValueMode(pDC, x, y, fCalcValue, fXPixelLength, fYPixelLength);
						}
					}
					else
					{
						//	if(!m_pCScanParamView->bHanHeFlag)	// ���Ǻ���,���Ϸ�Χ�ⲻ����
						{
							if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
							{
								fCalcValue = -1;
								m_pPixel[x][y] = fCalcValue;
								m_pPixelCount[x][y] = m_pPixelCount[x][y] + 1;
								draw2DC(pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);
							}
						}
					}
				}
				else
				{
					if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < m_fGateThreshold[m_nGateIndex])
					{
						if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
						{	
							// test start 2011-11-18
							// 			if(FILE_DEBUG)
							// 			{
							// 				CString strInfo;
							// 				strInfo.Format(_T("%d\t\t%d\t%.3f\t\t%.3f\t\n"), x, y,  ndataOriginal.m_dX, ndataOriginal.m_dY);
							// 				file.WriteString(strInfo);
							// 			}
							// test end 2011-11-18
							//machao modify start on 2013-07-15
							if (m_bNoiseReduction)
							{
								fCalcValue = m_pfSrcData[x][y];
							}
							else
							{
								fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
//								m_pfSrcData[x][y] = fCalcValue;		// machao delete on 2013-08-21
							}
							//fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
							//machao modify end on 2013-07-15

							Draw2DCAsValueMode(pDC, x, y, fCalcValue, fXPixelLength, fYPixelLength);
						}
					}
					else
					{
						//	if(!m_pCScanParamView->bHanHeFlag)	// ���Ǻ���,���Ϸ�Χ�ⲻ����
						{
							if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
							{
								fCalcValue = -1;
								m_pPixel[x][y] = fCalcValue;
								m_pPixelCount[x][y] = m_pPixelCount[x][y] + 1;
								draw2DC(pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);
							}
						}
					}
				}
			}
			else
			{
				//����Cɨ����ȳ���
				if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
				{	
					//machao modify start on 2013-07-15
					if (m_bNoiseReduction)
					{
						fCalcValue = m_pfSrcData[x][y];
					}
					else
					{
						fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
//						m_pfSrcData[x][y] = fCalcValue;		// machao delete on 2013-08-21
					}
					//fCalcValue = CalcPictureValue(ndata, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, nMaxAmpIndexTemp);
					//machao modify end on 2013-07-15

					Draw2DCAsValueMode(pDC, x, y, fCalcValue, fXPixelLength, fYPixelLength);
				}
				
			}
		}
	}

	// ����
	if(m_pCScanParamView->bHanHeFlag)
	{
		DrawCalcRegion(pDC);
	}
	m_pCScanParamView->bHanHeFlag = FALSE;

	if(m_pCScanParamView->m_bNoteFlag)
	{
		Note_ReadFile(m_strNotePath);

		Note_DrawAllNote(&m_MemDC, FALSE, m_nObjectNum);
	}

	// Song Chenguang add start on 2013-07-02
	int nCount = m_lstCustumSurfaceRegion.GetCount();
	if (nCount > 0)
	{
		POSITION pos = m_lstCustumSurfaceRegion.GetHeadPosition();

		while(pos)
		{
			LOGICAL_REGION region = m_lstCustumSurfaceRegion.GetNext(pos);
// 			CPoint ptLeftBottom = Logical2MemDC(region.m_stFrom);
// 			CPoint ptRightTop = Logical2MemDC(region.m_stTo);
// 
// 			DrawMemDCRegion(pDC, CPoint(ptLeftBottom.x, ptRightTop.y * -1), 
// 				CPoint(ptRightTop.x, ptLeftBottom.y * -1), eRECT, FALSE);
			CPoint ptLeftBottom = Logical2Client(region.m_stFrom);
			CPoint ptRightTop = Logical2Client(region.m_stTo);

			DrawMemDCRegion(pDC, ptLeftBottom, 
				ptRightTop, eRECT, FALSE);
		}
	}

	int nOldMode = m_MemDC.SetROP2(R2_NOT);
	for(int i = 0; i < m_vecLine.size(); i++)
	{
		DrawMemDCRegion(pDC, m_vecLine[i].p1, 
			m_vecLine[i].p2, eLINE, FALSE);
	}
	m_MemDC.SetROP2(nOldMode);
	// Song Chenguang add end on 2013-07-02

	Invalidate(TRUE);	// 2012-2-21�ĳɷŵ�������
	
	//	SaveDC�ú���ͨ������������ѡ�������ͼ��ģʽ����λͼ�����ʡ���ɫ�塢���塢�ʡ����򡢻�ͼģʽ��ӳ��ģʽ��
	//	�������������ջ��Ϊ���棬ָ���豸�����Ļ����ĵ�ǰ״̬
//	m_nSavedDC = pDC->SaveDC();

	// test start 2011-11-18
	if(FILE_DEBUG)
	{ 			
		file.Close();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPicture"),"Leave");
}
// ����������Χ
void CCScanView::DrawCalcRegion(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCalcRegion"),"Enter");
/*	LOGICAL_POINT_t* ptArray = NULL;

	if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
	{
		ptArray = new LOGICAL_POINT_t[m_nPolyNum+1];
		for (int i = 0; i < m_nPolyNum + 1; i++)
		{
			ptArray[i] = Client2Logical(m_Poly[i]);
		}
	}*/
	LOGICAL_POINT_t ptLogicalFrom;
	LOGICAL_POINT_t ptLogicalTo;
	ptLogicalFrom = Client2Logical(m_ptFrom);
	ptLogicalTo = Client2Logical(m_ptTo);
	if(ptLogicalFrom.x < ptLogicalTo.x)
	{
		m_pCScanParamView->m_fLogicalFromX = ptLogicalFrom.x;
		m_pCScanParamView->m_fLogicalToX = ptLogicalTo.x;
	}
	else
	{
		m_pCScanParamView->m_fLogicalFromX = ptLogicalTo.x;
		m_pCScanParamView->m_fLogicalToX = ptLogicalFrom.x;
	}

	if(ptLogicalFrom.y < ptLogicalTo.y)
	{
		m_pCScanParamView->m_fLogicalFromY = ptLogicalFrom.y;
		m_pCScanParamView->m_fLogicalToY = ptLogicalTo.y;
	}
	else
	{
		m_pCScanParamView->m_fLogicalFromY = ptLogicalTo.y;
		m_pCScanParamView->m_fLogicalToY = ptLogicalFrom.y;
	}

	if (m_pCScanParamView->m_nSurfaceType == 0)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo,eRECT, FALSE);
	} 
	else if(m_pCScanParamView->m_nSurfaceType == 1)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, eCIRCLE, FALSE);
	}
	else if (m_pCScanParamView->m_nSurfaceType == 2)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, eELLIPSE, FALSE);
	}
	else if (m_pCScanParamView->m_nSurfaceType == 3)
	{

		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, ePolygon, FALSE);
	}
	else
	{
		;//
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawCalcRegion"),"Leave");
}
void CCScanView::Draw2DCAsValueMode(CDC *pDC, int x, int y, float fCalcValue, float fXPixelLength, float fYPixelLength)
{
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Draw2DCAsValueMode"),"Enter");
	float tempAmp;
	if(m_nValueMode == 1)
	{
		tempAmp = 100;
	}
	else
	{
		tempAmp = 0;
	}
	if(m_nValueMode == 0)
	{
		tempAmp = m_pPixel[x][y];		
		if(tempAmp < fCalcValue)	//ȡ���ֵ��
		{
			;//
		}
		else
		{
			fCalcValue = tempAmp;
		}

		m_pPixel[x][y] = fCalcValue;
		m_pPixelCount[x][y] = m_pPixelCount[x][y] + 1;

		draw2DC(pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);

	}
	else if(m_nValueMode == 1)
	{	
		if(m_pPixelCount[x][y] == 0)
		{
			tempAmp = 100;
		}
		else
		{
			tempAmp = m_pPixel[x][y];
		}

		if(tempAmp > fCalcValue)	//ȡ��Сֵ��
		{
			;//
		}
		else
		{
			fCalcValue = tempAmp;
		}

		m_pPixel[x][y] = fCalcValue;
		m_pPixelCount[x][y] = m_pPixelCount[x][y] + 1;

		draw2DC(pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);
	}
	else if(m_nValueMode == 2)	//ȡƽ��ֵ
	{
		m_pPixelCount[x][y] = m_pPixelCount[x][y] + 1;
		m_pPixel[x][y] = (m_pPixel[x][y]*(m_pPixelCount[x][y] - 1)  + fCalcValue) / m_pPixelCount[x][y];

		draw2DC(pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);
	}
	else
	{
		;
	}
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Draw2DCAsValueMode"),"Leave");
}


void CCScanView::BuDian(CFeatureInfo feature, int nGateNum, int nFeatureNum, int nChannelNum, 
						int nFillType, CDC *pDC, float fCalcValue, int x, int y, float fXPixelLength, float fYPixelLength)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::BuDian"),"Enter");
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	CFeatureInfo featureTail(nGateNum, nFeatureNum);//��������
	COLORREF colorRGB1 = 0;			// ǰһ����RGB
	COLORREF colorRGB2 = 0;			// ǰ������RGB	
	if(nFillType == 0) // ����
	{
		if(y > 1)
		{
			colorRGB1 = m_MemDC.GetPixel(x, -(y - 1));
			colorRGB2 = m_MemDC.GetPixel(x, -(y - 2));
			WORD wRvalue1 = GetRValue(colorRGB1);
			WORD wGvalue1 = GetGValue(colorRGB1);
			WORD wBvalue1 = GetBValue(colorRGB1);
			WORD wRvalue2 = GetRValue(colorRGB2);
			WORD wGvalue2 = GetGValue(colorRGB2);
			WORD wBvalue2 = GetBValue(colorRGB2);

			if(wRvalue1 == 255 && wGvalue1 == 255 && wBvalue1 == 255
				&& (wRvalue2 != 255 || wGvalue2 != 255 || wBvalue2 != 255))
			{
				draw2DC(pDC, fCalcValue, x, y - 1, fXPixelLength, fYPixelLength);
				if(pDoc->m_lstFlawData[nChannelNum].GetCount() > 0)
				{
					featureTail = pDoc->m_lstFlawData[nChannelNum].GetTail();
					feature.m_nCount = featureTail.m_nCount;
					feature.m_dY = feature.m_dY - m_fGridY;
					feature.m_nCount++;
					pDoc->m_lstFlawData[nChannelNum].AddTail(feature);
					// wangbingfu modify start at 2012-10-12
					pDoc->m_qCScanData[nChannelNum].push(feature);
//					pDoc->m_lstFlawDataTemp[nChannelNum].AddTail(feature);
					// wangbingfu modify end at 2012-10-12
				}
			}
		}
	}
	else	// ����
	{
		if(x > 1)
		{
			colorRGB1 = m_MemDC.GetPixel(x - 1, -y);
			colorRGB2 = m_MemDC.GetPixel(x - 2, -y);
			WORD wRvalue1 = GetRValue(colorRGB1);
			WORD wGvalue1 = GetGValue(colorRGB1);
			WORD wBvalue1 = GetBValue(colorRGB1);
			WORD wRvalue2 = GetRValue(colorRGB2);
			WORD wGvalue2 = GetGValue(colorRGB2);
			WORD wBvalue2 = GetBValue(colorRGB2);

			if(wRvalue1 == 255 && wGvalue1 == 255 && wBvalue1 == 255
				&& (wRvalue2 != 255 || wGvalue2 != 255 || wBvalue2 != 255))
			{
				draw2DC(pDC, fCalcValue, x - 1, y, fXPixelLength, fYPixelLength);
				if(pDoc->m_lstFlawData[nChannelNum].GetCount() > 0)
				{
					featureTail = pDoc->m_lstFlawData[nChannelNum].GetTail();
					feature.m_nCount = featureTail.m_nCount;
					feature.m_dX = feature.m_dX - m_fGridX;
					feature.m_nCount++;
					pDoc->m_lstFlawData[nChannelNum].AddTail(feature);
					// wangbingfu modify start at 2012-10-12
					pDoc->m_qCScanData[nChannelNum].push(feature);
//					pDoc->m_lstFlawDataTemp[nChannelNum].AddTail(feature);
					// wangbingfu modify end at 2012-10-12
				}
			}
		}

	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::BuDian"),"Leave");
}
void CCScanView::DrawPictureRealTime(CDC* pDC, const CRect& rect, CH_FLAWDATA_CSCAN_S* pFeatureInfo,  const int& nCardIndex, 
									 const int& nChannelIndex, const int& nStartPoint, const int& nNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Enter");
	// TODO: �ڴ���������������
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	int nFeatureIndex = 0;	//����ѡ��
	// wangbingfu add start 2013-01-09
	int nMaxAmpIndex = 6;
	int nMaxTimeIndex = 4;
	int nMaxTimeIndexTemp = 4;//��󲨷�ʱ������ 
	// wangbingfu add end 2013-01-09
	int nGateIndexTemp = m_nGateIndex;
	int nFeatureIndexTemp = nFeatureIndex;
	int nMaxAmpIndexTemp = nMaxAmpIndex;

	// ����Cɨ����
//	pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(TRUE);
	m_fGateThreshold[0] = m_pCScanParamView->m_fGate1Threshold;
	m_fGateThreshold[1] = m_pCScanParamView->m_fGate2Threshold;
	m_fGateThreshold[2] = m_pCScanParamView->m_fGate3Threshold;
	m_fGateThreshold[3] = m_pCScanParamView->m_fGate4Threshold;
	m_nColorMode = m_pCScanParamView->m_nColorMode;
	m_nValueMode = m_pCScanParamView->m_nValueMode;
	m_nGateIndex = m_pCScanParamView->m_nGateIndex;
	m_nDrawType = m_pCScanParamView->m_nDrawType;
	m_fBoundMin = m_pCScanParamView->m_fBoundMin;
	m_fBoundMax = m_pCScanParamView->m_fBoundMax;
	m_nAmpFeatureIndex = m_pCScanParamView->m_nAmpFeatureIndex;
	m_nTimeFeatureIndex = m_pCScanParamView->m_nTimeFeatureIndex;
	nFeatureIndex = m_pCScanParamView->m_nFeatureIndex + 1;	//����ѡ��
	m_nPipeXMode = m_pCScanParamView->m_nPipeXMode;
	m_nColorDefine = m_pCScanParamView->m_nColorDefine;
	
	nFeatureIndexTemp = nFeatureIndex;

	int nFillType = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nFillType;	//��䷽ʽ:��������
	// ��ת
	// wangbingfu delete start on 2013-01-09
//	m_nAxisSelect = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	// wangbingfu delete end on 2013-01-09
	float fXStep = m_fGridX; // X���
	float fYStep = m_fGridY; // Y���

	float fXLength = static_cast<float>(fabs(m_dXEnd - m_dXStart));	// ����X�᳤��
	float fYLength = static_cast<float>(fabs(m_dYEnd - m_dYStart)); // ����Y�᳤��

	int nXLength = int(fXLength / fXStep);	//��������X���س���
	int nYLength = int(fYLength / fYStep);	//��������Y���س���

	//��ʼ��ͼ
	int	x,y;//�������λ��
	//��ȡ���ݣ�����
//	CH_FLAWDATA_CSCAN_S ndata;//��������
//	float fGrayScale; //��ɫɫ�ȱ���
//	int nGrayValue;	//��ɫֵ
//	int nColorIndex; //��ɫ����
	
	float fCalcValue = 0.0f;	//����ֵ
	//�ı�ԭ��,�������µ�
	pDC->SetViewportOrg(0, m_nMemYLENGTH);

	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = FALSE;
	for(int i= 1; i < 7; i++)
	{
		bFeatureFlag[i] = m_bFileFeatureFlag[i];
	}

	for(int i = 0; i < m_nGateIndex; i++)
	{
		if(!pFrame->m_pFeatureWnd->m_bGateOpen[i])
		{
			nGateIndexTemp--;
		}
	}
	for(int j = 1; j < nFeatureIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nFeatureIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxAmpIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxAmpIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxTimeIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxTimeIndexTemp--;
		}
	}

	int nGateNum = pFrame->m_pFeatureWnd->m_nGateOpenNum;
	int nFeatureNum = pFrame->m_pFeatureWnd->m_nFeatureOpenNum+1;

	float fXConvertValue = 1 / fXStep;
	float fYConvertValue = 1 / fYStep;

	float fXPixelLength = fXStep;
	float fYPixelLength = fYStep;

	if(m_pPixel == NULL)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Leave");
		return;
	}

	int nMode = GetPictureMode(nFeatureIndex - 1);
	if(nMode > 1)
		nMode = 0;

	int nChannelNum = nCardIndex * 2 + nChannelIndex;
	
	int nMemDCXMin = 100000;		//����DC X��Сֵ
	int nMemDCXMax = 0;				//����DC X���ֵ
	int nMemDCYMin = 100000;		//����DC Y��Сֵ
	int nMemDCYMax = 0;				//����DC Y���ֵ
	CFeatureInfo feature(nGateNum, nFeatureNum);//��������
	int nCount = 1;
	// wangbingfu modify start on 2013-01-09
	if(m_bGuanLianFlag && m_nAxisSelect != 4)	// ����ģʽ
	// wangbingfu modify start on 2013-01-09
	{
		if(m_nObjectNum == 1)	// ֻ���ײ� ��ͨ���ײ� ˫ͨ���˲�
		{
			if(nChannelNum == 1 || nChannelNum == 3 || nChannelNum == 5 || nChannelNum == 7)
			{
				//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Leave");
				return;
			}
		}
		else if(m_nObjectNum == 2)	// ֻ���˲�
		{
			if(nChannelNum == 0 || nChannelNum == 2 || nChannelNum == 4 || nChannelNum == 6)
			{
				
				//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Leave");
				return;
			}
		}
		else
		{
			;//
		}
	}

	//	SaveDC�ú���ͨ������������ѡ�������ͼ��ģʽ����λͼ�����ʡ���ɫ�塢���塢�ʡ����򡢻�ͼģʽ��ӳ��ģʽ��
	//	�������������ջ��Ϊ���棬ָ���豸�����Ļ����ĵ�ǰ״̬
//	m_nSavedDC = pDC->SaveDC();
	
	// wangbingfu add start on 2013-01-09
	if(m_nAxisSelect == 4) // W1��
	{
		BOOL bPipeGuanLian = FALSE;	// ��ĳһ�鿨��2ͨ��ѡ����+�ֲ�ʱ������Ϊ����
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			i++;
			if(m_nPipeChannelSelect[i] == 3) // ����2ͨ������ѡ��A���+�ֲ�ʱ��C1�����C2���˲�(�ֲ�)
			{
				bPipeGuanLian = TRUE;
				if((nChannelNum == i)
					||(nChannelNum == (i - 1)))
				{
					if(m_nObjectNum == 1)
					{
						if(nChannelNum == i)	// Cɨ1�����1ͨ��,�����
						{
							return;
						}
					}
					else if(m_nObjectNum == 2)
					{
						if(nChannelNum == (i - 1))	// Cɨ2�����2ͨ��,���ֲ�(�˲�)
						{
							return;
						}
					}
					else
					{
						;//
					}
				}
				else
				{
					return;
				}
				
			}
		}

		if(!bPipeGuanLian
			&& (nChannelNum != m_nCScanSelect))	//����ѡ��ͨ��ʱ�˳�����
		{
			return;
		}		
		//	BOOL bCeHouFlag = FALSE;	// ����־
		if(nChannelIndex == 0)
		{
			if(m_nPipeChannelSelect[nChannelNum+1] == 3)
			{
				if(m_nObjectNum == 1)	// Cɨ1
				{
					//	bCeHouFlag = TRUE;
					nMode = 6; //  ���
				}
			}
			else if(m_nPipeChannelSelect[nChannelNum] == 1)
			{
				//bCeHouFlag = TRUE;
				nMode = 6; //  ���
			}
		}
		else if(nChannelIndex == 1)
		{
			if(m_nPipeChannelSelect[nChannelNum] == 1)
			{
				//	bCeHouFlag = TRUE;
				nMode = 6; //  ���
			}
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

	bRealTimeDrawMode = nMode;
	// wangbingfu add end on 2013-01-09
	
	for(POSITION pos = pDoc->m_lstFlawData[nChannelNum].FindIndex(nStartPoint); pos; )
	{
		//��ȡ���ݵ㣬���Զ����е���һ����
		feature = pDoc->m_lstFlawData[nChannelNum].GetNext(pos);
		
		if(nCount > nNum)
		{
			break;
		}
		nCount++;
		// ��ת
		if(m_nAxisSelect == 3 || m_nAxisSelect == 4)	
		{
			x = (int)((feature.m_dX -  pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrX) * fXConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15
			y = (int)((feature.m_dR - pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrY) * fYConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15
			// wangbingfu add start on 2013-01-09
			if(m_nAxisSelect == 4)	// W1�ܲ�
			{
				if(m_nPipeChannelSelect[m_nCScanSelect] == 4)	//����B1
				{
					;
				}
				else if(m_nPipeChannelSelect[m_nCScanSelect] == 5)	//����B2
				{
					;
				}
				else if(m_nPipeChannelSelect[m_nCScanSelect] == 6)	//����C1
				{
					;
				}
				else if(m_nPipeChannelSelect[m_nCScanSelect] == 7)	//����C2	
				{
					;
				}
				else
				{
					;//
				}
			}
			// wangbingfu add end on 2013-01-09
		}
		// Song Chenguang add start on 2013-05-04
		else if (m_nAxisSelect == 5)	//?
		{
			x = (int)((feature.m_dW -  m_dWStart) *fXConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15
			y = (int)((feature.m_dZ - pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrZ) * fYConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15
		}
		// Song Chenguang add end on 2013-05-04
		else
		{
			x = (int)((feature.m_dX -  pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrX) * fXConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15
			y = (int)((feature.m_dY - pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrY) * fYConvertValue + 0.5);	// Song Chenguang modify on 2014-01-15			
		}

		if(x < nMemDCXMin)
		{
			nMemDCXMin = x;
		}
		if(x > nMemDCXMax)
		{
			nMemDCXMax = x;
		}
		if(y < nMemDCYMin)
		{
			nMemDCYMin = y;
		}
		if(y > nMemDCYMax)
		{
			nMemDCYMax = y;
		}
		//		if(feature.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[nGateIndexTemp])
		{
			//����Cɨ�����
			if((x >= 0 && x < m_nMemXLENGTH) && (y >= 0 && y < m_nMemYLENGTH))
			{
				//machao modify start on 2013-07-15
				if (m_bNoiseReduction)
				{
					fCalcValue = m_pfSrcData[x][y];
				}
				else
				{
					fCalcValue = CalcPictureValue(feature, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, 0);
//					m_pfSrcData[x][y] = fCalcValue;		// machao delete on 2013-08-21
				}
// 				// wangbingfu modify start on 2013-01-09
// 		//		fCalcValue = CalcPictureValue(feature, nMode,nGateIndexTemp, nFeatureIndexTemp, 0, 0);
// 				fCalcValue = CalcPictureValue(feature, nMode,nGateIndexTemp, nFeatureIndexTemp, nMaxTimeIndexTemp, 0);
// 				// wangbingfu modify end on 2013-01-09
				//machao modify end on 2013-07-15

				Draw2DCAsValueMode(pDC, x, y, fCalcValue, fXPixelLength, fYPixelLength);

				// rem by Song Chenguang on 2013-12-20,ȥ���˹��ܣ�©�㲹����
//				BuDian(feature, nGateNum, nFeatureNum, nChannelNum, nFillType, pDC, fCalcValue, x, y, fXPixelLength, fYPixelLength);
			}
		}		
	}

	if(nCount > 2)
	{	
		CDC *pDC1 = this->GetDC();
		// wangbingfu add start on 2012-11-03
		if(pDC1 == NULL)
		{
			MessageBox(_T("CScanViwe  DrawPictureRealTime(), pDC1 is null"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Leave");
			return;
		}
		// wangbingfu add end on 2012-11-03
		//�ı�ԭ��,Cɨ�������µ�
		pDC1->SetViewportOrg(rect.left, rect.bottom);
		
 		nMemDCXMin -= 3; 
 		nMemDCXMax += 3;
 		nMemDCYMin -= 3;
 		nMemDCYMax += 3;


		if(nMemDCXMin < 0)
		{
			nMemDCXMin = 0;
		}
		if(nMemDCXMax < 0)
		{
			nMemDCXMax = 0;
		}
		if(nMemDCYMin < 0)
		{
			nMemDCYMin = 0;
		}
		if(nMemDCYMax < 0)
		{
			nMemDCYMax = 0;
		}
		int nDCXMin = nMemDCXMin * rect.Width() / nXLength;		//DC X��Сֵ
		int nDCXMax = nMemDCXMax * rect.Width() / nXLength;		//DC X���ֵ
		int nDCYMin = nMemDCYMin * rect.Height() / nYLength;		//DC Y��Сֵ
		int nDCYMax = nMemDCYMax * rect.Height() / nYLength;		//DC Y���ֵ
		
		pDC1->StretchBlt(nDCXMin, -nDCYMin, nDCXMax, -nDCYMax, pDC, 
			nMemDCXMin, -nMemDCYMin, nMemDCXMax, -nMemDCYMax, SRCCOPY);
		ReleaseDC(pDC1);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawPictureRealTime"),"Leave");
}
void CCScanView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonDblClk"),"Enter");
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bDblClk = TRUE; //OnDraw ������ɫ
	LOGICAL_POINT_t stLogicalPoint;
	stLogicalPoint = Client2Logical(point);
	RepositionProcess(point, stLogicalPoint);
	CScrollView::OnLButtonDblClk(nFlags, point);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnLButtonDblClk"),"Leave");
}

// Song Chenguang add start on 2013-06-28
void CCScanView::CalcOneCustumFlaw(LOGICAL_REGION& stRegion, const float& fThreshold)
{
	m_pCScanParamView->m_nZoomScale = 1;
	LOGICAL_POINT_t* ptArray = NULL;
	if (m_pCScanParamView->m_nFeatureIndex > 5)
	{
		AfxMessageBox(_T("���˼��㲻֧����������"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Leave");
		return;
	}

	UpdateData(TRUE);

	// ��ȡɨ�鷶Χ
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	m_pCScanParamView->m_PlateDrawCtrl.clear();

	if (m_pCScanParamView->m_nSurfaceType == 1)	// Բ
	{
		m_pCScanParamView->m_PlateDrawCtrl.SetPlateInfo(stRegion.m_stTo.x * m_pCScanParamView->m_nZoomScale, 
			stRegion.m_stTo.y * m_pCScanParamView->m_nZoomScale, m_fThick);
	}
	else if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
	{
		m_pCScanParamView->m_PlateDrawCtrl.SetPlateInfo(stRegion.m_stTo.x * m_pCScanParamView->m_nZoomScale, 
			stRegion.m_stTo.y * m_pCScanParamView->m_nZoomScale, m_fThick);

		ptArray = new LOGICAL_POINT_t[m_nPolyNum+1];
		if (NULL != ptArray)  //add by wangqianfeng on 20121106
		{
			for (int i=0; i<m_nPolyNum+1; i++)
			{
				ptArray[i] = Client2Logical(m_Poly[i]);
			}
		}
	}
	else
 	{
 		float fLength = stRegion.m_stTo.x > stRegion.m_stFrom.x ? stRegion.m_stTo.x : stRegion.m_stFrom.x;
 		float fWidth = stRegion.m_stTo.y > stRegion.m_stFrom.y ? stRegion.m_stTo.y : stRegion.m_stFrom.y;
 
 		m_pCScanParamView->m_PlateDrawCtrl.SetPlateInfo(fLength * m_pCScanParamView->m_nZoomScale, 
 			fWidth * m_pCScanParamView->m_nZoomScale, m_fThick);
 	}

	// ��ȡɨ����
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);

	float fDistanceX = m_fGridX;
	float fDistanceY = m_fGridY;
	m_pCScanParamView->m_PlateDrawCtrl.SetDistanceX(fDistanceX * m_pCScanParamView->m_nZoomScale, fDistanceX * m_pCScanParamView->m_nZoomScale);
	m_pCScanParamView->m_PlateDrawCtrl.SetDistanceY(fDistanceY * m_pCScanParamView->m_nZoomScale, fDistanceY * m_pCScanParamView->m_nZoomScale);
	m_pCScanParamView->m_PlateDrawCtrl.SetMaxZDistance(1);

	m_pCScanParamView->m_PlateDrawCtrl.SetZhiShiLengthType(1);
	m_pCScanParamView->m_PlateDrawCtrl.SetShangWidthType(2);
	m_pCScanParamView->m_PlateDrawCtrl.SetAreaType(1);
	m_pCScanParamView->m_PlateDrawCtrl.SetMergeShangType(2, 1.0);
	m_pCScanParamView->m_PlateDrawCtrl.SetEdgeWidth(0);

	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseBodyMinArea(m_pCScanParamView->m_fMinSurfaceArea * m_pCScanParamView->m_nZoomScale * m_pCScanParamView->m_nZoomScale);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseBodyMinShangZhiShiLength(0);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseBodyMinShangLength(0);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseBodyMinShangWidth(0);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseEdgeMinArea(m_pCScanParamView->m_fMinSurfaceArea * m_pCScanParamView->m_nZoomScale * m_pCScanParamView->m_nZoomScale);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseEdgeMinShangZhiShiLength(0);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseEdgeMinShangLength(0);
	m_pCScanParamView->m_PlateDrawCtrl.SetNotUseEdgeMinShangWidth(0);

	// �ӵ�
	int nFeatureIndex = 4;	//����ѡ��,ѡ������ֵʱ��
	int nMaxAmpIndex = 6;	//��������ֵ����
	int nMaxAmpIndexTemp = 0;	//��������ֵ����
	int nGateIndexTemp = 0;
	int nFeatureIndexTemp = 0;
	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = FALSE;
	for(int i= 1; i < 7; i++)
	{
		bFeatureFlag[i] = m_bFileFeatureFlag[i];
	}

	nFeatureIndexTemp = nFeatureIndex;
	nMaxAmpIndexTemp = nMaxAmpIndex;

	for(int j = 1; j < nFeatureIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nFeatureIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxAmpIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxAmpIndexTemp--;
		}
	}

	CUltrasonicImmersedTestDoc* pDoc = GetDocument();

	m_pCScanParamView->m_fLogicalFromX = min(stRegion.m_stFrom.x, stRegion.m_stTo.x);
	m_pCScanParamView->m_fLogicalToX = max(stRegion.m_stFrom.x, stRegion.m_stTo.x);

	m_pCScanParamView->m_fLogicalFromY = min(stRegion.m_stFrom.y, stRegion.m_stTo.y);
	m_pCScanParamView->m_fLogicalToY = max(stRegion.m_stFrom.y, stRegion.m_stTo.y);	

	long lCount = 0;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		if(m_bGuanLianFlag)	// ����ģʽ
		{
			if(m_nObjectNum == 1)	// ֻ���ײ� ��ͨ���ײ� ˫ͨ���˲�
			{
				if(i == 1 || i == 3 || i == 5 || i == 7)
				{
					continue;
				}
			}
			else if(m_nObjectNum == 2)	// ֻ���˲�
			{
				if(i == 0 || i == 2 || i == 4 || i == 6)
				{
					continue;
				}
			}
			else
			{
				;//
			}
		}

		nGateIndexTemp = m_nGateIndex;

		for(int j = 0; j < m_nGateIndex; j++)
		{
			if(!m_bFileGateFlag[i][j])
			{
				nGateIndexTemp--;
			}
		}
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition();pos; )
		{
			if (lCount > MAX_POINTS_NUM)
			{
				MessageBox(_T("�˵��������500,000��"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Leave");
				return;
			}
			CFeatureInfo ndata = pDoc->m_lstFlawData[i].GetNext(pos); 

			if(m_nAxisSelect == 3
				|| m_nAxisSelect == 4)	//�ܲ� wangbingfu add on 2013-01-09
			{
				ndata.m_dY = ndata.m_dR;
			}
			// Song Chenguang add start on 2013-05-04
			else if (m_nAxisSelect == 5)
			{
				ndata.m_dX = ndata.m_dW;
				ndata.m_dY = ndata.m_dZ;
			}
			// Song Chenguang add end on 2013-05-04

			// songchenguang modify start on 2012-05-22
			// wangbingfu modify stat on 2012-9-13 �򻯴��벢���Ӹ��ڻ������ֵ���ж�
			if (m_pCScanParamView->m_nSurfaceType == 0 || m_pCScanParamView->m_nSurfaceType == 2)	// ���Ρ���Բ
			{
				if(!((ndata.m_dX >= m_pCScanParamView->m_fLogicalFromX) && (ndata.m_dX <= m_pCScanParamView->m_fLogicalToX)
					&& (ndata.m_dY >= m_pCScanParamView->m_fLogicalFromY) && (ndata.m_dY <= m_pCScanParamView->m_fLogicalToY)))
				{
					continue;
				}
			}
			else if(m_pCScanParamView->m_nSurfaceType == 2)
			{
				BOOL bRet = m_pCScanParamView->IsInEllipse(ndata, m_pCScanParamView->m_fLogicalFromX, 
					m_pCScanParamView->m_fLogicalToX, m_pCScanParamView->m_fLogicalFromY, m_pCScanParamView->m_fLogicalToY);
				if (!bRet)
					continue;
			}
			else if (m_pCScanParamView->m_nSurfaceType == 1)	// Բ
			{
				BOOL bRet = m_pCScanParamView->IsInCircle(ndata, m_pCScanParamView->m_fLogicalFromX, 
					m_pCScanParamView->m_fLogicalToX, m_pCScanParamView->m_fLogicalFromY, m_pCScanParamView->m_fLogicalToY);
				if (!bRet)
					continue;

			}		
			else if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
			{				
				BOOL bRet = m_pCScanParamView->IsInPolygon(ndata, ptArray, m_nPolyNum + 1);				

				if (!bRet)
					continue;
			}

			if(m_pCScanParamView->m_nGateType == 0)
			{
				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= fThreshold)
				{
					m_pCScanParamView->m_PlateDrawCtrl.AddPoint2(ndata.m_dX * m_pCScanParamView->m_nZoomScale, 
						ndata.m_dY * m_pCScanParamView->m_nZoomScale, ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp], 2,ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] , 0, 0, 0, 0);

					lCount++;
				}
			}
			else
			{
				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < fThreshold)
				{
					m_pCScanParamView->m_PlateDrawCtrl.AddPoint2(ndata.m_dX * m_pCScanParamView->m_nZoomScale, ndata.m_dY * m_pCScanParamView->m_nZoomScale, ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp], 2, ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp], 0, 0, 0, 0);

					lCount++;
				}
			}
		}
		// wangbingfu modify end on 2012-9-13
		// songchenguang modify end on 2012-05-22

		if (lCount > MAX_POINTS_NUM)
		{			
			break;
		}
	}
	if (m_pCScanParamView->m_nSurfaceType == 3)
	{
		delete []ptArray;
		ptArray = NULL;
	}

	// ����
	m_pCScanParamView->m_PlateDrawCtrl.go();
	// ��ͼ
	m_pCScanParamView->m_nSurfceColorMode = m_nColorMode;
	m_bDrawSurfaceFlag = TRUE;
//	pCScanView->m_MemDC.FillSolidRect(0, 0, pCScanView->m_CScanSize.cx, pCScanView->m_CScanSize.cy, pCScanView->m_cscan_back_color);
//	DrawPicture(&m_MemDC);

//	DrawSurfaceFlaw(&m_MemDC);
	Invalidate(TRUE);	// ������
}
// Song Chenguang add end on 2013-06-28

// ��λ����
void CCScanView::RepositionProcess(CPoint point, LOGICAL_POINT_t stLogicalPoint)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::RepositionProcess"),"Enter");
	m_bIsDrawFlag = TRUE;
	m_DlbPoint = point;

	LOGICAL_POINT_t i_pot;
	m_probePoint = stLogicalPoint;

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
#if !_OUTLINE
	double dbXRange = pApp->m_IniParams.m_info.fXRange;
	double dbYRange = pApp->m_IniParams.m_info.fYRange;
#else
	double dbXRange = 4000;
	double dbYRange = 4000;
#endif
	// machao add start on 2013-11-15
	if (m_nAxisSelect == 5)
	{
		// machao modify start on 2013-11-24
		if (m_nPipeXMode == 0)	// �Ƕ�
			dbXRange = 360;
		else
			dbXRange = PI * m_dPipeDiameter;
		// machao modify end on 2013-11-24
#if !_OUTLINE
		dbYRange = pApp->m_IniParams.m_info.fZRange;		
#else
		dbYRange = 2000;
#endif
	}
	// machao add end on 2013-11-15

	if (m_probePoint.x > dbXRange || m_probePoint.y > dbYRange)
	{
		MessageBox(_T("���곬��ɨ�鷶Χ��"), _T("��ʾ"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::RepositionProcess"),"Leave");
		return;
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();

	WORD wAECNum = pApp->GetCardNum();
	double dStartXValue = 0;
	double dStartYValue = 0;
	double dStartZValue = 0;
	
	// wangbingfu add start on 2013-01-31	
	double dStartABValue = 0;
	double dStartWValue = 0;
	// wangbingfu add end on 2013-01-31
	double dStartRValue = 0;

	for (int i=0; i<wAECNum*2; i++)
	{
		if (pDoc->m_lstFlawData[i].GetCount() > 0)
		{
			CFeatureInfo pFeatureInfo = pDoc->m_lstFlawData[i].GetTail();
			dStartXValue = pFeatureInfo.m_dX;
			dStartYValue = pFeatureInfo.m_dY;
			dStartZValue = pFeatureInfo.m_dZ;
			dStartRValue = pFeatureInfo.m_dR;	// ��ʼ����Rֵ
			dStartWValue = pFeatureInfo.m_dW;	// ��ʼ����Wֵ
			dStartABValue = pFeatureInfo.m_dB;	// ��ʼ����ABֵ

			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord = dStartXValue;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dYcoord = dStartYValue;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dZcoord = dStartZValue;
			// wangbingfu add start on 2013-01-31
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dABcoord = dStartABValue;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dWcoord = dStartWValue;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord = dStartRValue;			
			// wangbingfu add end on 2013-01-31
			break;
		}
	}

	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// ��תW1 wangbingfu add on 2013-01-09	
	{	
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord = m_probePoint.x;
		if(pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nPipeXMode == 1)		// �ܳ�		Song Chenguang modify on 2013-09-09
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord = m_probePoint.y;	//�����Ƕ�
			i_pot.y=static_cast<float>(PI * m_dPipeDiameter * m_probePoint.y/360);	//?
		}
		else	// �Ƕ�
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord = (m_probePoint.y / (PI * m_dPipeDiameter) * 360);	//�����Ƕ�
			i_pot.y=m_probePoint.y;	//?
		}
	}
	// Song Chenguang add start on 2013-05-04
	else if(m_nAxisSelect == 5)	// Wƽ��
	{	
		//?
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dZcoord = m_probePoint.y;

			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dWcoord = m_probePoint.x;	//�����Ƕ�
		// Song Chenguang add start on 2013-11-27
		i_pot.x = m_probePoint.x;
		i_pot.y = m_probePoint.y;
		// Song Chenguang add end on 2013-11-27
	}
	// Song Chenguang add end on 2013-05-04
	else if(m_nAxisSelect == 1)	// XZ
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord = m_probePoint.x;
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dYcoord = dStartZValue;
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dZcoord = m_probePoint.y;
		i_pot.x=m_probePoint.x;
		i_pot.y=m_probePoint.y;
	}
	else if(m_nAxisSelect == 2)	// YZ
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord = dStartZValue;
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dYcoord = m_probePoint.x;
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dZcoord = m_probePoint.y;
		i_pot.x=m_probePoint.x;
		i_pot.y=m_probePoint.y;
	}
	else
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord = m_probePoint.x;//˫����꽫��ǰ̽ͷ������ʾ�ڻ�е���������
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dYcoord = m_probePoint.y;
		i_pot.x=m_probePoint.x;
		i_pot.y=m_probePoint.y;
	}


	CString str;
	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dXcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_XCOORD,str);

	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dYcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_YCOORD,str);

	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dZcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_ZCOORD,str);

	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dABcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_ABCOORD,str);
	// wangbingfu add start on 2013-01-31
	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dWcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_WCOORD,str);

	str.Format(_T("%.1f"),pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord);
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_RCOORD,str);
	// wangbingfu add end on 2013-01-31

	//��ʾAɨ����

	if (PathFileExists(pDoc->m_FilePath))
	{
		pFrame->KillTimer(TIMER_NET_JUDGE);

		// Song Chenguang modify start on 2013-12-19
		CPicData_L *PicData = getAScanDataByPos(i_pot);

		pFrame->m_bMoinitorDataRun = FALSE;
/*		WPARAM wParam = MAKEWPARAM(ID_STOP_MONITOR_DATA, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
*/
		CPicData *pPicData2 = new CPicData;
		memcpy(pPicData2->m_wData, PicData->m_wData, sizeof(WORD) * DATA_SIZE);
		// ����բ��
		pFrame->SetGateFromFile(PicData->m_nCardIndex, PicData->m_nChannelIndex);

		CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
		pView->PostMessage(WM_A_SCAN_DATA, (WPARAM)pPicData2, 0);

		CH_FLAWDATA_CSCAN_S * pData = new CH_FLAWDATA_CSCAN_S;
		CARD_INFO_t * pCardInfo = new CARD_INFO_t;

		if (pData == NULL)
		{
			AfxMessageBox(_T("CARSSocket::OnReceive new pData2ʧ�ܣ�"));
			return;
		}

		if (pCardInfo == NULL)
		{
			AfxMessageBox(_T("CARSSocket::OnReceive new pCardInfo2ʧ�ܣ�"));
			return;
		}

		pCardInfo->nCardIndex = PicData->m_nCardIndex;
		pCardInfo->nChannelIndex = PicData->m_nChannelIndex;

		*pData = PicData->m_stFlawData;
		pFrame->m_pFeatureWnd->PostMessage(WM_FEATURE_INFO, (WPARAM)pData, (LPARAM)pCardInfo);

		delete PicData;
		PicData = NULL;
		// Song Chenguang modify end on 2013-12-19
	}

	if (m_bMeasureOn)	// ����ʱ��˫�������ʮ��
	{
		m_bIsDrawSyncFlag = TRUE;
	}

	Invalidate(TRUE);

	//ͬ��
	CCScanParamPanel* pCScanParamView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	if(pCScanParamView->m_bCscanSync)
	{
		if(!m_pCScanParamView->m_bSurfaceFlag)
		{
			if(m_pCScanParamView->m_pSurfaceDeatailDlg == NULL)		//����ʱ����λ
			{
				CCScanView* pCScanView = NULL;
				if(m_nObjectNum == 1)
				{
					pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);			
				}
				else
				{
					pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
				}

				pCScanView->m_bIsDrawSyncFlag = TRUE;
				pCScanView->m_probePoint = m_probePoint;
				pCScanView->Invalidate(TRUE);
			}
		}
	}
	// wangbingfu add start on 2014-09-10
	// ����ʱ�л�����Ӧͨ��
	if(m_bGuanLianFlag)	// ����ģʽ
	{
		CCScanView* pCScanView = NULL;
		if(m_nObjectNum == 1)
		{
			m_nDblClickSelect = 1;
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
			pCScanView->m_nDblClickSelect = 1;
		}
		else
		{
			m_nDblClickSelect = 2;
			pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
			pCScanView->m_nDblClickSelect = 2;
		}
	}
	// wangbingfu add end on 2014-09-10
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::RepositionProcess"),"Leave");
}



CPoint CCScanView::MPoint2SPoint( MPoint point )	//?
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MPoint2SPoint"),"Enter");
	CPoint i_point;
	
	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// ��תW1	// wangbingfu add on 2013-01-09
	{	
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if(pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nPipeXMode == 1)		// �Ƕ�		Song Chenguang modify on 2013-09-10
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord = m_probePoint.y;	//�����Ƕ�
			i_point.y = static_cast<LONG>(point.w1);
		}
		else	// �ܳ�
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_dRcoord = (m_probePoint.y / (PI * m_dPipeDiameter) * 360);	//�����Ƕ�
			i_point.y = static_cast<LONG>(PI * m_dPipeDiameter * point.w1 / 360);
		}
	}
	else if(m_nAxisSelect == 1)	// XZ
	{
		i_point.x=static_cast<LONG>(point.x);
		i_point.y=static_cast<LONG>(point.z);
	}
	else if(m_nAxisSelect == 2)	// YZ
	{
		i_point.x=static_cast<LONG>(point.y);
		i_point.y=static_cast<LONG>(point.z);
	}
	else
	{
		i_point.x=static_cast<LONG>(point.x);
		i_point.y=static_cast<LONG>(point.y);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MPoint2SPoint"),"Leave");
	return i_point;
}

// wangbingfu modify start on 2014-09-10
CPicData_L* CCScanView::getAScanDataByPos(LOGICAL_POINT_t p_point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::getAScanDataByPos"),"Enter");
	CPicData_L *PicData = new CPicData_L;	// Song Chenguang modify on 2013-12-19
	// WangQianfeng add start on 2012-11-06
	if ( NULL == PicData )
	{
		MessageBox(_T(" CCScanView::getAScanDataByPos,PicData = null"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::getAScanDataByPos"),"Leave");
		return NULL;
	}
	// WangQianfeng add end on 2012-11-06
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	if (PathFileExists(pDoc->m_FilePath)==false)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::getAScanDataByPos"),"Leave");
		//Aɨ�ļ�������
		return PicData;
	}
	
	int m;
	ULONG i_grid_sn;
	ULONG i_postion;
	CPicData_L dPicData;	// Song Chenguang modify on 2013-12-19
	
	double dXStart = 0.0f;
	double dYStart = 0.0f;
	double dXEnd = 0.0f;
	double dYEnd = 0.0f;
	double dPipeDiameter = 0.0f;
	int dAxisSelect;
	float dGridX;
	float dGridY;
	int dGridXSize;
	ULONG dGridLength;


	int hLength=sizeof(double)*5+sizeof(float)*2+sizeof(int)*2+sizeof(ULONG) +
		GATE_STRUCT_SIZE * GATE_NUM * CHANNEL_NUM;	// Song Chenguang modify on 2013-12-19
	int pLength=sizeof(CPicData_L)+sizeof(int);	// Song Chenguang modify on 2013-12-19

	CFile file;
	// wanbingfu modify start on 2014-09-10
	//	file.Open(pDoc->m_FilePath, CFile::modeRead);
	if(m_bGuanLianFlag)	// ��������ͨ��
	{
		if(m_nObjectNum == 1)
		{
			file.Open(pDoc->m_FilePath, CFile::modeRead);
		}
		else if(m_nObjectNum == 2)
		{
			file.Open(pDoc->m_FilePath1, CFile::modeRead);
		}
		else
		{
			;//
		}
	}
	else
	{
		file.Open(pDoc->m_FilePath, CFile::modeRead);
	}
	// wanbingfu modify end on 2014-09-10

	file.Read(&dXStart,sizeof(double));
	file.Read(&dYStart,sizeof(double));
	file.Read(&dXEnd,sizeof(double));
	file.Read(&dYEnd,sizeof(double));
	file.Read(&dPipeDiameter,sizeof(double));
	file.Read(&dAxisSelect,sizeof(int));
	file.Read(&dGridX,sizeof(float));
	file.Read(&dGridY,sizeof(float));
	file.Read(&dGridXSize,sizeof(int));
	file.Read(&dGridLength,sizeof(ULONG));

	ULONGLONG dwPosition = file.GetPosition();
	ULONGLONG dwSize = file.GetLength();
	if(dwPosition > dwSize)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::getAScanDataByPos"),"Leave");
		//������
		return PicData;
	}
	
	if (dAxisSelect==3)
	{
//		i_grid_sn=static_cast<ULONG>(floor(p_point.y/dGridY));
		// wangbingfu modify start on 2014-11-04
//		i_grid_sn=static_cast<ULONG>(p_point.y / dGridY + 0.5);	// ��Ϊ��������
		i_grid_sn = static_cast<ULONG>(ULONG(fabs(p_point.x - dXStart) / dGridX + 0.5) + 
			ULONG(p_point.y / dGridY + 0.5) * dGridXSize);
		// wangbingfu modify end on 2014-11-04
	} 
	else
	{
		if (p_point.x<dXStart||p_point.x>dXEnd||p_point.y<dYStart||p_point.y>dYEnd)
		{
			MessageBox(_T("��ѡ�㲻�ڹ�����Χ�ڣ�"), _T("��ʾ"), MB_OK);
			return PicData;
		}
//		i_grid_sn=static_cast<ULONG>(floor(fabs(p_point.x-dXStart) / dGridX + 0.5)+floor(fabs(p_point.y-dYStart)/dGridY)*dGridXSize);
		i_grid_sn = static_cast<ULONG>(ULONG(fabs(p_point.x - dXStart) / dGridX + 0.5) + 
			ULONG(fabs(p_point.y - dYStart) / dGridY + 0.5) * dGridXSize);
	}
// Song Chenguang modify start on 2013-12-21	
	if (i_grid_sn<dGridLength)
	{
		ULONG i_pos=0;
		LONGLONG llpos = hLength+i_grid_sn*sizeof(ULONG);
		file.Seek(llpos, CFile::begin);
		file.Read(&i_postion,sizeof(ULONG));
		if (i_postion>0)
		{
			file.Seek(i_postion, CFile::begin);
			file.Read(&m,sizeof(int));
			file.Read(&dPicData,sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
			memcpy(PicData, &dPicData, sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
		}
		else
		{
//			MessageBox(_T("δ�ҵ��˵�Aɨ���ݣ�"), _T("��ʾ"), MB_OK);
			
			float i_length=50000;
			for (float y=p_point.y-10;y<=p_point.y+10;y++)
			{
				
				if (y>0)
				{
					int i_y=static_cast<int>(floor(fabs(y-dYStart)/dGridY)*dGridXSize);
					for (float x=p_point.x-10;x<=p_point.x+10;x++)
					{
						if (x>0)
						{
							int i_x=static_cast<int>(floor(fabs(x-dXStart)/dGridX));
							file.Seek(hLength+(i_y+i_x)*sizeof(ULONG), CFile::begin);
							file.Read(&i_postion,sizeof(ULONG));
							if (i_postion>0)
							{
								float i_tmp=pow(p_point.x-x,2)+pow(p_point.y-y,2);
								if (i_tmp<i_length)
								{
									i_pos=i_postion;
									i_length=i_tmp;
								}
							}
						}
					}
				}
			}

		}
		if (i_pos>0)
		{
			file.Seek(i_pos, CFile::begin);
			file.Read(&m,sizeof(int));
			file.Read(&dPicData,sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
			memcpy(PicData, &dPicData, sizeof(CPicData_L));	// Song Chenguang modify on 2013-12-19
		}

// Song Chenguang modify end on 2013-12-21
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::getAScanDataByPos"),"Leave");
	return PicData;
}

void CCScanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnKeyDown"),"Enter");
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pMovePath = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	if (77 == nChar)//����m������̽ͷ�ƶ������˫����
	{
		double Dbpoint[3];
		m_pMovePath->m_GraphMove.GetPos(Dbpoint);
		SetTimer(MOVEPROBE,50,NULL);
	}
	else
	{
		;
	}*/

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnKeyDown"),"Leave");
}


/** 
* ��Cɨ��ͼ����Y�̶ȱ��/����X�̶ȱ��
* @param     pDC       [in]: ��ͼ�豸����ָ��
* @param     rect      [in]: ����̶ȳ�����
* @param     nStart    [in]: ���̶�ֵ
* @param     nEnd      [in]:  ��С�̶�ֵ   
* @param     nStep     [in]: �̶Ȳ���
* @param     direction [in]: �̶ȳߵ�λ��
* @param     bReverse  [in]: ���̶ȳ߿���
* @return    void
* @since     1.0
*/
void CCScanView::DrawScale(CDC* pDC, 
							const CRect& rect, 
							float fStart, 
							float fEnd, 
							float fStep, 
							int direction, 
							BOOL bReverse)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawScale"),"Enter");
	ASSERT(fabs(fStart - fEnd) > 1E-6);

	if (fStep<1E-6)
	{
		fStep = 100.0;
	}
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush(RGB(0,192,0));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	// ������
	pDC->Rectangle(rect);
	// ���ָ���
	int n = 10;//(fEnd - fStart)/fStep;
	float fPerCoordinate = 0.0f;
	CString str;

	CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	UINT nOldAlign = 0;

	if (direction == 1)
	{
		nOldAlign = pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
	}
	else if (direction == 2)
	{
		nOldAlign = pDC->SetTextAlign(TA_LEFT);
	}
	else if (direction == 3)
	{
		nOldAlign = pDC->SetTextAlign(TA_CENTER|TA_TOP);
	}
	else if (direction == 4)
	{
		nOldAlign = pDC->SetTextAlign(TA_RIGHT);
	}
	else
	{
	}

	// ���ұ��
	if (direction == 1 || direction == 3)
	{
		fPerCoordinate = float(rect.Width())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			str.Format(_T("%d"), (int)(fStart+(fStep*i)));
			// ������㣬�յ�
			CPoint ptStart, ptEnd;
			ptStart.x = static_cast<LONG>(rect.left + fPerCoordinate*fStep*i);
			ptEnd.x = ptStart.x;
			if (direction == 1)
			{
				ptStart.y = rect.bottom;
				ptEnd.y = rect.bottom - 9;
				
				pDC->TextOut(ptEnd.x, ptEnd.y, str);
			}
			else
			{
				ptStart.y = rect.top;
				ptEnd.y = rect.top + 9;
				
				pDC->TextOut(ptEnd.x+20, ptEnd.y, str);//ptEnd.x����20����Ϊ��ʹ�̶ȿ���ƫ�ƣ�ʹ��һ���̶�ֵ�ܹ���ʾ������
			}

			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}

		for(int i = 0; i < n; i++)
		{
			CPoint ptStart, ptEnd;
			for(int j = 0; j < 5; j++)
			{
				ptStart.x = static_cast<LONG>(rect.left + fPerCoordinate*fStep*i + fPerCoordinate*fStep * j / 5);
				ptEnd.x = ptStart.x;

				if (direction == 1)
				{
					ptStart.y = rect.bottom;
					ptEnd.y = rect.bottom - 4;
				}
				else
				{
					ptStart.y = rect.top;
					ptEnd.y = rect.top + 4;
				}
				pDC->MoveTo(ptStart);
				pDC->LineTo(ptEnd);
			}
		}
	}
	// ���±��
	else if (direction == 2 || direction == 4)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		LONG lHalfHeight = tm.tmHeight/2;
		fPerCoordinate = float(rect.Height())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			int nYData = static_cast<int>(fStart+(fStep*i));
			str.Format(_T("%d"), nYData);
			// ������㣬�յ�
			CPoint ptStart, ptEnd;
			if (!bReverse)
			{
				ptStart.y = static_cast<LONG>(rect.top + fPerCoordinate*fStep*i);
			}
			else
				ptStart.y = static_cast<LONG>(rect.bottom - fPerCoordinate*fStep*i);

			ptEnd.y = ptStart.y;
			if (direction == 2)
			{
				ptStart.x = rect.left;
				ptEnd.x = rect.left + 8;
				
				pDC->TextOut(ptEnd.x, ptEnd.y - lHalfHeight, str);
			}
			else
			{
				ptStart.x = rect.right;
				ptEnd.x = rect.right - 8;
				
				pDC->TextOut(ptEnd.x, ptEnd.y - lHalfHeight, str);
			}

			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
		
		for(int i = 0; i < n; i++)
		{
			CPoint ptStart, ptEnd;
			for(int j = 0; j < 5; j++)
			{
				if (!bReverse)
				{
					ptStart.y = static_cast<LONG>(rect.top + fPerCoordinate*fStep*i + fPerCoordinate*fStep * j / 5);
				}
				else
					ptStart.y = static_cast<LONG>(rect.bottom - fPerCoordinate*fStep*i - fPerCoordinate*fStep * j / 5);
				ptEnd.y = ptStart.y;

				if (direction == 2)
				{
					ptStart.x = rect.left;
					ptEnd.x = rect.left + 5;
				}
				else
				{
					ptStart.x = rect.right;
					ptEnd.x = rect.right - 5;
				}

				pDC->MoveTo(ptStart);
				pDC->LineTo(ptEnd);
			}
		}
	}
	else
	{
	}

	pDC->SetTextAlign(nOldAlign);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawScale"),"Leave");
}
void CCScanView::FillSpace(CDC* pDC,CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::FillSpace"),"Enter");
	CBrush brush(RGB(0,192,0));
	pDC->FillRect(&rect,&brush);
	pDC->SetBkMode(TRANSPARENT);
	CString string;
	string.Format(_T("Cɨ%d"),m_nObjectNum);
	pDC->TextOut(rect.left +5, rect.bottom-20, string);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::FillSpace"),"Leave");
}
/** 
* ��Cɨ��ͼ������������
* @param     pDC  [in]: ��ͼ�豸����ָ��
* @param     rect [in]: Aɨ������ʾ����                                                    
* @return    void
* @since     1.0
*/
void CCScanView::DrawGrid(CDC* pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawGrid"),"Enter");
	//�ı�ԭ��
	pDC->SetViewportOrg(rect.TopLeft().x, rect.TopLeft().y);

// 	if (pDC->IsPrinting())
// 	{
// 		/*pDC->SetViewportOrg(200 + rect.TopLeft().x*m_fRateX, 200);*/
// 		pDC->SetViewportOrg(420, 500);
// 	}
	CPen pen;

	pen.CreatePen(PS_DOT ,1,RGB(192,192,192));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->SetBkColor(TRANSPARENT);

	//������������
	for(int i = 1;i<5;++i)
	{
		pDC->MoveTo(0,static_cast<int>(2.0/10.0*rect.Height()*i));
		pDC->LineTo(rect.right,static_cast<int>(2.0/10.0*rect.Height()*i));
	}
	//������������
	for(int i = 1;i< 10;++i)
	{
		pDC->MoveTo(static_cast<int>(1.0/10.0*rect.Width()*i),rect.top);
		pDC->LineTo(static_cast<int>(1.0/10.0*rect.Width()*i),rect.bottom);
	}


	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawGrid"),"Leave");
}
//��Cɨ˫��������ʮ��
void CCScanView::DrawDlbFlg(CDC* pDC,CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDlbFlg"),"Enter");
	pDC->SetViewportOrg(0,0);
	/*if (point.x<m_rectCScan.left)
	{
		point.x = m_rectCScan.left;
	}
	if (point.x > m_rectCScan.right)
	{
		point.x = m_rectCScan.right;
	}
	if (point.y<m_rectCScan.top)
	{
		point.y = m_rectCScan.top; 
	}
	if (point.y>m_rectCScan.bottom)
	{
		point.y = m_rectCScan.bottom;
	}
	*/

	CPen pen(PS_SOLID,2,RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(point.x - 10,point.y);
	pDC->LineTo(point.x + 10,point.y);

	pDC->MoveTo(point.x,point.y-10);
	pDC->LineTo(point.x,point.y+10);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDlbFlg"),"Leave");
}

//ͬ����ʮ��
void CCScanView::DrawSyncDlbFlg(CDC* pDC,CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSyncDlbFlg"),"Enter");
	pDC->SetViewportOrg(0,0);

/*

	if (point.x<m_rectCScan.left)
	{
		point.x = m_rectCScan.left;
	}
	if (point.x > m_rectCScan.right)
	{
		point.x = m_rectCScan.right;
	}
	if (point.y<m_rectCScan.top)
	{
		point.y = m_rectCScan.top; 
	}
	if (point.y>m_rectCScan.bottom)
	{
		point.y = m_rectCScan.bottom;
	}
*/

	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_rectGrid.left,point.y);
	pDC->LineTo(m_rectGrid.right,point.y);

	pDC->MoveTo(point.x,m_rectGrid.top);
	pDC->LineTo(point.x,m_rectGrid.bottom);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSyncDlbFlg"),"Leave");
}

void CCScanView::draw2DC(CDC* pDC, float fValue, int x, int y, float fXPixelLength, float fYPixelLength)
{
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::draw2DC"),"Enter");
	y = -y;
//	RECT stRect;	//��Ӧ�㻭�ľ���
	int nXPixelLength = (int)fXPixelLength;
	int nYPixelLength = (int)fYPixelLength;
	if (m_nColorMode == 0)//ʹ�ûҶ�ͼ��
	{
		int nColorIndex = int((fValue - m_fBoundMin)/((m_fBoundMax-m_fBoundMin)/193));
		// wangbingfu add on 2013-01-09
		if(g_bDrawRealTime)
		{
			if(bRealTimeDrawMode == 6)
			{
				nColorIndex = int(fValue/(m_pCScanParamView->m_fThick/193));
			}
		}
		// wangbingfu add on 2013-01-09
		
		if (nColorIndex > 0)
		{
			nColorIndex -= 1;					
		} 
		if(nColorIndex > 193)
		{
			nColorIndex = 193;
		}
		if(nColorIndex < 0)
		{
			nColorIndex = 0;
		}
		pDC->SetPixel(x,y, m_grayDefine[nColorIndex]);
	}
	else	// ��ɫ
	{
		if(fValue >= 0)
		{
			if(m_nColorDefine == 1)			//Ĭ����ɫ
			{
				int nColorIndex = int((fValue - m_fBoundMin)/((m_fBoundMax-m_fBoundMin)/20));
				
				// wangbingfu add on 2013-01-09
				if(g_bDrawRealTime)
				{
					if(bRealTimeDrawMode == 6)
					{
						nColorIndex = int(fValue/(m_pCScanParamView->m_fThick/20));
					}
				}
				// wangbingfu add on 2013-01-09

				if (nColorIndex > 0)
				{
					nColorIndex -= 1;					
				} 
				if(nColorIndex > 20)
				{
					nColorIndex = 20;
				}
				if(nColorIndex < 0)
				{
					nColorIndex = 0;
				}
				pDC->SetPixel(x,y, m_clrPlatte[nColorIndex]);		
			}
			else if(m_nColorDefine == 0)	//�Զ�����ɫ
			{
				//Machao modify start on 2013-07-3
//				int nColorIndex = int((fValue - m_fBoundMin)/((m_fBoundMax-m_fBoundMin)/193));
				int nColorIndex = GetColorNumber(fValue, m_fBoundMin, m_fBoundMax);
				
				// wangbingfu add on 2013-01-09
				if(g_bDrawRealTime)
				{
					if(bRealTimeDrawMode == 6)
					{
//						nColorIndex = int(fValue/(m_pCScanParamView->m_fThick/20));
						nColorIndex = GetColorNumber(fValue, 0, m_pCScanParamView->m_fThick);
					}
				}
				// wangbingfu add on 2013-01-09

// 				if (nColorIndex > 0)
// 				{
// 					nColorIndex -= 1;					
// 				} 
// 				if(nColorIndex > 193)
// 				{
// 					nColorIndex = 193;
// 				}
// 				if(nColorIndex < 0)
// 				{
// 					nColorIndex = 0;
// 				}
// 				pDC->SetPixel(x,y, m_clrDefine[nColorIndex]);
				pDC->SetPixel(x,y, m_pCScanParamView->m_plaPalette[nColorIndex].color);
				//Machao modify end on 2013-07-3
			}
			else
			{
				;//
			}
		}
		else
		{
			//pDC->SetPixel(x,y, RGB(255,255,255));//�������׵�
			pDC->SetPixel(x,y,m_pCScanParamView->m_clrBackColor);//��֮ǰ����׵׸�Ϊ������ yachang.huang modify on2014-3-28
		}
	}
//	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::draw2DC"),"Leave");
}

void CCScanView::DrawSurfaceFlaw(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSurfaceFlaw"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;
	if(m_nObjectNum == 1)
		pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	else
		pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;

	int x;
	int y;
	float fXConvertValue = 1 / m_fGridX;
	float fYConvertValue = 1 / m_fGridY;
	float fGrayScale;	//�Ҷȱ���
	int nColorIndex; //ɫ��ֵ����
	//�ı�ԭ��
	pDC->SetViewportOrg(0, m_nMemYLENGTH);

	if (pCScanParam->m_nSurfaceType == 0)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo,eRECT, TRUE);
	} 
	else if(pCScanParam->m_nSurfaceType == 1)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, eCIRCLE, TRUE);
	}
	else if (pCScanParam->m_nSurfaceType == 2)
	{
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, eELLIPSE, TRUE);
	}
	else if (pCScanParam->m_nSurfaceType == 3)
	{		
		DrawMemDCRegion(pDC, m_ptFrom, m_ptTo, ePolygon, TRUE);
	}
	else
	{
		;//
	}

	//pDC->MoveTo(ptMemDCFrom);
//	pDC->LineTo(ptMemDCTo);

	float fThick = pCScanParam->m_fThick;	//�������
	VARIANT Result = pCScanParam->m_PlateDrawCtrl.GetPlaneFlawPoints2();

	PlaneFlawHeader PFH = {0};
	long lLBound = 0;
	long lUBound = 0;

	::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//һά�����±�
	::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//һά�����ϱ�

	char* pBuf;
	SafeArrayAccessData(Result.parray,   (void   **)&pBuf);   

	UINT Size = (lUBound-lLBound+1);

	// WangQianfeng add start on 2012-11-20
	m_nDefectNum = -1;
	//ȡ�ñ�ע��Ϣ
	int nSymbol1=0,nSymbol2=0,nSymbol3=0;
	int nAreaA,nAreaB;
	CString csFont;
	int nFontSize;
	int nDefectEnable;
	GetDataFromSetFile(nSymbol1,nSymbol2,nSymbol3,nAreaA,nAreaB,csFont,nFontSize,nDefectEnable);
	// WangQianfeng add end on 2012-11-20
/*	// Song Chenguang add start on 2013-09-06
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
	int nAxisIndex = pMovePathView->m_GraphMove.GetAxisSelect();
	// Song Chenguang add end on 2013-09-06
*/	int Pos = 0;
	int i = 0;
	while((Pos + sizeof(PlaneFlawHeader)) < Size)
	{	
		memcpy(&PFH,pBuf + Pos,sizeof(PlaneFlawHeader));
		Pos +=  sizeof(PlaneFlawHeader);

		int nPosIndex = 0;
		int ptStartX = 0;
		int ptStartY = 0;
		HPOINT hPoint = {0};
		//		mp = (2 * PFH.z * 1000000) / (velocity * 1000);
		CBrush* pOldBrush = NULL;
		CBrush brush;

		if (m_nColorMode == 0)//ʹ�ûҶ�ͼ��
		{
			fGrayScale = static_cast<float>(PFH.z / fThick * 16);
			if(fGrayScale > 16)
			{
				fGrayScale = 16;
			}
			else if(fGrayScale < 0)
			{
				fGrayScale = 0;
			}

			fGrayScale = 16 - fGrayScale;

			int nGrayValue = int(fGrayScale * 16 + fGrayScale);
			brush.CreateSolidBrush(RGB(nGrayValue,nGrayValue,nGrayValue));
			pOldBrush = pDC->SelectObject(&brush);
		}
		else	// ��ɫ
		{
			nColorIndex = int(PFH.z / fThick * 20);
			if (nColorIndex > 0)
			{
				nColorIndex -= 1;					
			} 
			if(nColorIndex > 20)
			{
				nColorIndex = 20;
			}
			if(nColorIndex < 0)
			{
				nColorIndex = 0;
			}
			brush.CreateSolidBrush(m_clrPlatte[nColorIndex]);
			pOldBrush = pDC->SelectObject(&brush);
		}

		vector<CPoint> vectorEdge;
		while(nPosIndex < PFH.PointsCount) //
		{
			memcpy(&hPoint, pBuf + Pos,sizeof(HPOINT));
			Pos += sizeof(HPOINT);

			// Song Chenguang rem start on 2013-06-29
//			hPoint.x += (pCScanParam->m_fLogicalFromX * pCScanParam->m_nZoomScale);
//			hPoint.y += (pCScanParam->m_fLogicalFromY * pCScanParam->m_nZoomScale);
			// Song Chenguang rem end on 2013-06-29

			x = (int)((hPoint.x /  pCScanParam->m_nZoomScale - m_dXStart) * fXConvertValue);
/*			// Song Chenguang add start on 2013-09-06
			if (m_pCScanParamView->m_nPipeXMode == 0 && (nAxisIndex == 3 || nAxisIndex == 4))
			{
				hPoint.y = hPoint.y / 360.0f * PI * m_dPipeDiameter;
			}	

			// Song Chenguang add end on 2013-09-06
*/			y = -(int)((hPoint.y /  pCScanParam->m_nZoomScale - m_dYStart) * fYConvertValue);

			if ( nPosIndex == 0 )
			{
				ptStartX = x;
				ptStartY = y;
				pDC->BeginPath();
				pDC->MoveTo( (int)x, (int)y );
				// songchenguang add on 2012-10-30
//				pDC->SetPixel(CPoint(x,y), m_clrPlatte[nColorIndex]);
				//
				vectorEdge.push_back(CPoint((int)x, (int)y));
			}
			else
			{
				pDC->LineTo( (int)x, (int)y );
//				pDC->SetPixel(CPoint(x,y), m_clrPlatte[nColorIndex]);
				vectorEdge.push_back(CPoint((int)x, (int)y));
			}

			nPosIndex++;

			if ( nPosIndex == PFH.PointsCount )
			{
				pDC->LineTo( ptStartX, ptStartY );
				pDC->EndPath();
				// machao modify start on 2013-11-14
				CPen pen(PS_SOLID,1,m_clrPlatte[nColorIndex]);
				CPen* pOldPen=pDC->SelectObject(&pen);
				BOOL bRet = pDC->StrokeAndFillPath();
				// machao modify end on 2013-11-14
				if (!bRet)
				{
					vector<CPoint>::iterator iter;
					for (iter = vectorEdge.begin();iter != vectorEdge.end();++iter)
					{
						CPoint pt = *iter;
						pDC->SetPixel(pt, m_clrPlatte[nColorIndex]);
					}
					
				}
			}
		}

		// songchenguang add start on 2012-11-01
		if (PFH.PointsCount == 1)
		{
			pDC->SetPixel(CPoint(ptStartX,ptStartY), m_clrPlatte[nColorIndex]);
		}
		// songchenguang add end on 2012-11-01
		// WangQianfeng add start on 2012-11-20
		if (nDefectEnable)//��עʹ��
		{
			float fMin = m_pCScanParamView->m_fMinSurfaceArea;

			//�����ע����.���ĵ�
			float fMinX,fMinY,fMaxX,fMaxY;
			int nZoomScale = m_pCScanParamView->m_nZoomScale;

			double dPipeDiameter = 0;
			LOGICAL_POINT_t i_point;
			
			dPipeDiameter = m_dPipeDiameter;
			int nAxisSelect = 0;
			int nPipeXMode = 0;
			nPipeXMode = m_nPipeXMode;

			float fLogicalFromY = m_pCScanParamView->m_fLogicalFromY;
			float fLogicalFromX = m_pCScanParamView->m_fLogicalFromX;	// Song Chenguang add on 2013-05-04
			if(nAxisSelect == 3)	//�ܲ�
			{
				if(nPipeXMode == 0)	// �Ƕ�
				{
					PFH.min_y = PFH.min_y * (360.0f / (PI * dPipeDiameter));
					PFH.max_y	= PFH.max_y * (360.0f / (PI * dPipeDiameter));
					fLogicalFromY = static_cast<float>(fLogicalFromY * (360.0f / (PI * dPipeDiameter)));
				}
			}
			// Song Chenguang add start on 2013-05-04
			else if(nAxisSelect == 5)	// Wƽ��
			{
				if(nPipeXMode == 0)	// �Ƕ�
				{
					PFH.min_x = PFH.min_x * (360.0f / (PI * dPipeDiameter));
					PFH.max_x	= PFH.max_x * (360.0f / (PI * dPipeDiameter));
					fLogicalFromX = static_cast<float>(fLogicalFromX * (360.0f / (PI * dPipeDiameter)));
				}
			}
			// Song Chenguang add end on 2013-05-04

			// machao modify start on 2013-11-29
			fMinX = static_cast<float>(PFH.min_x / nZoomScale /*+ fLogicalFromX*/);
			fMinY = static_cast<float>(PFH.min_y / nZoomScale /*+ fLogicalFromY*/);
			fMaxX = static_cast<float>(PFH.max_x / nZoomScale /*+ fLogicalFromX*/);
			fMaxY = static_cast<float>(PFH.max_y / nZoomScale /*+ fLogicalFromY*/);
			// machao modify end on 2013-11-29


			float fXpos = static_cast<float>((fMinX + fMaxX)/2);
			float fYpos =  static_cast<float>((fMinY + fMaxY)/2);

			float s = static_cast<float>(PFH.area / (nZoomScale * nZoomScale));

			//if (s >= fMin && s < nAreaA) //Min<S<A //m_fMinSurfaceArea�������Ӧ�øĳ��������ļ���ȡ��
			if (s > fMin && s < nAreaA) //����ʾ�����б�ʱ��������С��������˲��ڷ�Χ��
			{
				m_nDefectNum++;
				if (m_nSymbol1Enable)
				{
					DrawGraphical(nSymbol1,fXpos,fYpos,csFont,nFontSize);
				}
			}
			else if (s >= nAreaA && s < nAreaB)
			{
				m_nDefectNum++;
				if (m_nSymbol2Enable)
				{
					DrawGraphical(nSymbol2,fXpos,fYpos,csFont,nFontSize);
				}
			}
			else if(s >= nAreaB)
			{
				m_nDefectNum++;
				if (m_nSymbol3Enable)
				{
					DrawGraphical(nSymbol3,fXpos,fYpos,csFont,nFontSize);
				}
			}
		}	
		// WangQianfeng add end on 2012-11-20
		pDC->SelectObject(pOldBrush);
	}
	SafeArrayUnaccessData(Result.parray);
	SafeArrayDestroy(Result.parray);

	pDC->SetViewportOrg(0, m_nMemYLENGTH * -1);	// Song Chenguang add on 2013-06-29
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSurfaceFlaw"),"Leave");
}
// WangQianfeng add start on 2012-11-20
void CCScanView::DrawGraphical(int nSymbol,float fXpos,float fYpos,CString csFont,int nFontSize)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::drawGraphical"),"Enter");
	//�����ת��
	float fXStep = 1; // X���
	float fYStep = 1; // Y���
	// ������(��ʱ��Ĭ��ֵ)
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nFillType = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nFillType;	//��䷽ʽ:��������

	if (nFillType == 0) // ����
	{
		fXStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
		fYStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;	
	} 
	else	// ����
	{
		fXStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;
		fYStep = (float)pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
	}

	float fXConvertValue = 1 / fXStep;
	float fYConvertValue = 1 / fYStep;

	int x = static_cast<int>((fXpos - m_dXStart) * fXConvertValue);
	int y = -static_cast<int>((fYpos - m_dYStart) * fYConvertValue);

	switch(nSymbol)
	{
		case 0://����
			{
				DrawDiamond(x,y,csFont,nFontSize);
				break;
			}
		case 1://������
			{
				DrawTriangle(x,y,csFont,nFontSize);
				break;
			}
		case 2://������
			{
				DrawSquare(x,y,csFont,nFontSize);
				break;
			}
		default:
			break;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::drawGraphical"),"Leave");
}
void CCScanView::DrawDiamond(int nXpos,int nYpos,CString csFont,int nFontSize)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDiamond"),"Enter");
	CDC *pDC = &m_MemDC;
	CPen pen(PS_SOLID,2,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//pDC->Ellipse(fXpos-10,fYpos+10,fXpos+10,fYpos-10);

	pDC->MoveTo(nXpos-10,nYpos);
	pDC->LineTo(nXpos, nYpos+10);

	pDC->MoveTo(nXpos-10,nYpos);
	pDC->LineTo(nXpos, nYpos-10);

	pDC->MoveTo(nXpos+10,nYpos);
	pDC->LineTo(nXpos, nYpos+10);

	pDC->MoveTo(nXpos+10,nYpos);
	pDC->LineTo(nXpos, nYpos-10);

	//m_nDefectNum++;

	//ȱ�ݱ��
	pDC->SetTextColor(m_defectFontColor);//����ȱ�ݱ��������ɫ
	CFont cf;
	LOGFONT lf;
	CClientDC dc(this);
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);// �����ֺ�//MSDC�Ͻ�����������
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;//���Σ��Ӵֵ�
	lf.lfItalic = FALSE;
	lf.lfUnderline = m_nlfUnderline;//�»���
	lf.lfStrikeOut = m_nlfDeleteline;//ɾ����
	//lf.lfCharSet = ANSI_CHARSET;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	//����һ�����ֻ�е���������UNICODE����ʱ�ſ��á�m_csfontΪCSTRING���ͣ���Ϊ��Ҫ�������Ϣ�洢�������ļ��У�������CSTRING����������
	wcscpy_s(lf.lfFaceName,CT2CW(csFont));

	cf.CreateFontIndirect(&lf);

	pDC->SelectObject(&cf);
	CString csNo;
	csNo.Format(_T("%d"),m_nDefectNum);
	pDC->TextOut(nXpos+10,nYpos-10,csNo);

	pDC->SelectObject(pOldPen);
	cf.DeleteObject();
	pen.DeleteObject();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDiamond"),"Leave");
}
void CCScanView::DrawTriangle(int nXpos,int nYpos,CString csFont,int nFontSize)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawTriangle"),"Enter");
	CDC *pDC = &m_MemDC;
	CPen pen(PS_SOLID,2,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

 	pDC->MoveTo(nXpos,nYpos-10);
	pDC->LineTo(static_cast<int>(nXpos+10*0.87), nYpos+5); 

	pDC->MoveTo(nXpos,nYpos-10);
	pDC->LineTo (static_cast<int>(nXpos-10*0.87), nYpos+5); 

	pDC->MoveTo(static_cast<int>(nXpos+10*0.87), nYpos+5);
	pDC->LineTo (static_cast<int>(nXpos-10*0.87), nYpos+5); 
	//m_nDefectNum++;

	//ȱ�ݱ��
	pDC->SetTextColor(m_defectFontColor);
	CFont cf;
	LOGFONT lf;
	CClientDC dc(this);
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);// �����ֺ�//MSDC�Ͻ�����������
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;//���Σ��Ӵֵ�
	lf.lfItalic = FALSE;
	lf.lfUnderline = m_nlfUnderline;//�»���
	lf.lfStrikeOut = m_nlfDeleteline;//ɾ����
	//lf.lfCharSet = ANSI_CHARSET;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	//����һ�����ֻ�е���������UNICODE����ʱ�ſ��á�m_csfontΪCSTRING���ͣ���Ϊ��Ҫ�������Ϣ�洢�������ļ��У�������CSTRING����������
	wcscpy_s(lf.lfFaceName,CT2CW(csFont));

	cf.CreateFontIndirect(&lf);
	CFont *def_font=pDC->SelectObject(&cf);

	CString csNo;
	csNo.Format(_T("%d"),m_nDefectNum);
	pDC->TextOut(nXpos+10,nYpos-10,csNo);

	pDC->SelectObject(def_font);
	cf.DeleteObject();

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawTriangle"),"Leave");
}
void CCScanView::DrawSquare(int nXpos,int nYpos,CString csFont,int nFontSize)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSquare"),"Enter");
	CDC *pDC = &m_MemDC;

	CPen pen(PS_SOLID,2,RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	//�����
	//pDC->Rectangle(fXpos-10,fYpos+10,fXpos+10,fYpos-10);
	pDC->MoveTo(nXpos-10,nYpos+10);
	pDC->LineTo (nXpos+10, nYpos+10); 

	pDC->MoveTo(nXpos-10,nYpos+10);
	pDC->LineTo (nXpos-10, nYpos-10); 

	pDC->MoveTo(nXpos-10, nYpos-10);
	pDC->LineTo (nXpos+10,nYpos-10); 

	pDC->MoveTo(nXpos+10, nYpos+10);
	pDC->LineTo (nXpos+10,nYpos-10);

	//m_nDefectNum++;

	//ȱ�ݱ��
	pDC->SetTextColor(m_defectFontColor);
	CFont cf;
	LOGFONT lf;
	CClientDC dc(this);
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);// �����ֺ�//MSDC�Ͻ�����������
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;//���Σ��Ӵֵ�
	lf.lfItalic = FALSE;
	lf.lfUnderline = m_nlfUnderline;//�»���
	lf.lfStrikeOut = m_nlfDeleteline;//ɾ����
	//lf.lfCharSet = ANSI_CHARSET;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	//����һ�����ֻ�е���������UNICODE����ʱ�ſ��á�m_csfontΪCSTRING���ͣ���Ϊ��Ҫ�������Ϣ�洢�������ļ��У�������CSTRING����������
	wcscpy_s(lf.lfFaceName,CT2CW(csFont));

	cf.CreateFontIndirect(&lf);
	CFont *def_font=pDC->SelectObject(&cf);

	CString csNo;
	csNo.Format(_T("%d"),m_nDefectNum);
	pDC->TextOut(nXpos+10,nYpos-10,csNo);

	pDC->SelectObject(def_font);
	cf.DeleteObject();

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawSquare"),"Leave");
}
void CCScanView::GetDataFromSetFile(int &nSymbol1,int &nSymbol2,int &nSymbol3,int &nAreaA,int &nAreaB,CString &csFont,int &nFontSize,int &nDefectEnable)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetDataFromSetFile"),"Enter");
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("SetSymbolDlg.ini");//ȡ�������ļ�·��;

	CString strSection;
	CString strKey;
	
	strSection = _T("DefectEnable");
	strKey = _T("Enable");
	nDefectEnable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);
	
	if(nDefectEnable)
	{
		strSection = _T("DefineSymbol");
		strKey = _T("Symbol1");
		nSymbol1 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("Symbol2");
		nSymbol2 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("Symbol3");
		nSymbol3 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("Symbol1Enable");
		m_nSymbol1Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("Symbol2Enable");
		m_nSymbol2Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("Symbol3Enable");
		m_nSymbol3Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strSection = _T("AreaSet");
		strKey = _T("AreaA");
		nAreaA = 	GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("AreaB");
		nAreaB = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strSection = _T("FontInFo");
		strKey = _T("Font");
		GetPrivateProfileString(strSection,strKey,NULL,csFont.GetBuffer(255),255,csFilePath);
		csFont.ReleaseBuffer();

		strKey = _T("FontSize");
		nFontSize = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		CString csFontColor;
		strSection = _T("FontInFo");
		strKey = _T("FontColor");
		GetPrivateProfileString(strSection,strKey,NULL,csFontColor.GetBuffer(255),255,csFilePath);
		csFontColor.ReleaseBuffer();

		if (csFontColor.IsEmpty())
		{
			m_defectFontColor = RGB(255,0,0);		
		}
		else
		{
			CString str,strTok;
			int curPos= 0;
			int colorR = 0;
			int colorG = 0;
			int colorB = 0;
			str = csFontColor.Tokenize(_T(","),curPos);
			colorR = _ttoi(str);

			str = csFontColor.Tokenize(_T(","),curPos);
			colorG = _ttoi(str);

			str = csFontColor.Tokenize(_T(","),curPos);
			colorB = _ttoi(str);
			m_defectFontColor = RGB(colorR,colorG,colorB);
		}

		strSection = _T("FontInFo");
		strKey = _T("UnderLine");
		m_nlfUnderline = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

		strKey = _T("DeleteLine");
		m_nlfDeleteline = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetDataFromSetFile"),"Leave");
}
// WangQianfeng add end on 2012-11-20
LOGICAL_POINT_t CCScanView::Client2Logical(CPoint clientPoint)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Client2Logical"),"Enter");
	float fZuobiaoYmin = m_fZuobiaoYmin;
	float fZuobiaoYmax = m_fZuobiaoYmax;
/*	if(m_nAxisSelect == 3)
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			fZuobiaoYmin = m_fZuobiaoYmin * (PI * m_dPipeDiameter / 360);
			fZuobiaoYmax = m_fZuobiaoYmax * (PI * m_dPipeDiameter / 360);
		}
	}*/

	LOGICAL_POINT_t logicalPoint;
	float fXCha = m_fZuobiaoXmax - m_fZuobiaoXmin;
	float fYCha = fZuobiaoYmax - fZuobiaoYmin;


	int nRectWidth  = m_rectGrid.Width();
	int nRectHeight = m_rectGrid.Height();


	 logicalPoint.x = (static_cast<float>(clientPoint.x - m_rectScaleY.Width())*(fXCha)/nRectWidth) + m_fZuobiaoXmin;
	float y1 = static_cast<float>(m_rectGrid.Height() - clientPoint.y);
	logicalPoint.y = (y1*(fYCha)/nRectHeight) + fZuobiaoYmin;

	if(clientPoint.x < m_rectScaleY.right)
	{
		logicalPoint.x = m_fZuobiaoXmin;
	}
	if(clientPoint.y > m_rectScaleX.top)
	{
		logicalPoint.y = fZuobiaoYmin;

	}

	// Song Chenguang add start on 2013-05-04
	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	//�ܲ� wangbingfu add on 2013-01-09
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			logicalPoint.y = static_cast<float>(logicalPoint.y * PI * m_dPipeDiameter / 360.0f);
		}
	}
/*
	if (m_nAxisSelect == 5)	// Wƽ��
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			logicalPoint.x = static_cast<float>(logicalPoint.x * PI / 360.0f);
		}
	}
*/
	// Song Chenguang add start on 2013-05-04
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Client2Logical"),"Leave");
	return logicalPoint;
}
/*
LOGICAL_POINT_t CCScanView::Client2Logical1(CPoint clientPoint)
{
	float fXmin;
	float fXmax;
	float fYmax;
	float fYmin;

	// ��X����
	if (m_Offset_Coordinate.y>=0)
	{
		fXmin = m_fZuobiaoXmin - m_Offset_Coordinate.x ;
		fXmax = m_fZuobiaoXmax -m_Offset_Coordinate.x;
		fYmax = m_fZuobiaoYmax - m_Offset_Coordinate.y;
		fYmin = m_fZuobiaoYmin - m_Offset_Coordinate.y;
	} 
	else
	{
		fXmin = m_fZuobiaoXmin;
		fXmax = m_fZuobiaoXmax;
		fYmax = m_fZuobiaoYmax;
		fYmin = m_fZuobiaoYmin;
	}

	LOGICAL_POINT_t logicalPoint;
	float fXCha = fXmax - fXmin;
	float fYCha = fYmax - fYmin;


	int nRectWidth  = m_rectGrid.Width();
	int nRectHeight = m_rectGrid.Height();


	logicalPoint.x = ((float)(clientPoint.x - m_rectScaleY.Width())*(fXCha)/nRectWidth) + fXmin;
	float y1 = m_rectGrid.Height() - clientPoint.y;
	logicalPoint.y = (y1*(fYCha)/nRectHeight) + fYmin;

	if(clientPoint.x < m_rectScaleY.right)
	{
		logicalPoint.x = fXmin;
	}
	if(clientPoint.y > m_rectScaleX.top)
	{
		logicalPoint.y = fYmin;
	}
	return logicalPoint;
}*/
CPoint CCScanView::Logical2Client(LOGICAL_POINT_t logicalPoint)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Logical2Client"),"Enter");
	float fZuobiaoYmin = m_fZuobiaoYmin;
	float fZuobiaoYmax = m_fZuobiaoYmax;
/*	if(m_nAxisSelect == 3)
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			fZuobiaoYmin = m_fZuobiaoYmin * (PI * m_dPipeDiameter / 360);
			fZuobiaoYmax = m_fZuobiaoYmax * (PI * m_dPipeDiameter / 360);
		}
	}*/
	// Song Chenguang add start on 2013-09-06
	if (m_nAxisSelect == 3 || m_nAxisSelect == 4)
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
//			logicalPoint.y = logicalPoint.y * (PI * m_dPipeDiameter / 360);
			logicalPoint.y = logicalPoint.y / (PI * m_dPipeDiameter) * 360;
		}
	}
	// Song Chenguang add end on 2013-09-06

	CPoint clientPoint;
	float fXCha = m_fZuobiaoXmax - m_fZuobiaoXmin;
	float fYCha = fZuobiaoYmax - fZuobiaoYmin;


	int nRectWidth  = m_rectGrid.Width();
	int nRectHeight = m_rectGrid.Height();


	clientPoint.x = (LONG)((logicalPoint.x - m_fZuobiaoXmin) / fXCha * nRectWidth + m_rectScaleY.Width());
	clientPoint.y = (LONG)(m_rectGrid.Height() - ((logicalPoint.y - fZuobiaoYmin) / fYCha * nRectHeight));

	if(clientPoint.x < m_rectScaleY.right)
	{
		clientPoint.x = m_rectScaleY.right;	// Machao modify on 2013-08-09
	}
	if(clientPoint.y > m_rectScaleX.top)
	{
		clientPoint.y = m_rectScaleX.top;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Logical2Client"),"Leave");
	return clientPoint;
}
void CCScanView::InvertPolygon()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertPolygon"),"Enter");
	CDC* pDC = GetDC();
	// wangbingfu add start on 2012-11-03
	if(pDC == NULL)
	{
		MessageBox(_T("CScanView  InvertPolygon(), pDC is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::InvertPolygon"),"Leave");
		return;
	}
	// wangbingfu add end on 2012-11-03
	int nOldMode = pDC->SetROP2(R2_NOT);

	pDC->BeginPath();

	for (int i=0; i<m_nPolyNum+1; i++)
	{
		if (i == 0)
			pDC->MoveTo(m_Poly[i]);
		else
			pDC->LineTo(m_Poly[i]);
	}

	pDC->LineTo(m_Poly[0]);

	pDC->EndPath();//����·��

	pDC->FillPath();


	pDC->SetROP2(nOldMode);
	ReleaseDC(pDC);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::InvertPolygon"),"Leave");
}
// songchenguang add end 2012-06-13

void CCScanView::OnRButtonDown(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRButtonDown"),"Enter");
	// TODO: Add your message handler code here and/or call default

	m_bIsDrawFlag = FALSE; //�����Ҽ�ʮ����ʧ

	if(m_pCScanParamView->m_bNoteFlag)
	{
		m_ptNoteCurrent = point;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_CSCAN_POPUP);
		CMenu *pMenu = menu.GetSubMenu(0);
		ClientToScreen(&point);	
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	}
	// Song Chenguang add start on 2013-06-20
	else if (m_pCScanParamView->m_bSurfaceFlag && m_pCScanParamView->m_nSurfaceSelType == 0)
	{
		// ����ѡ��ķ�Χ�б��ڣ������Ҽ������˵�		
		m_stCurPoint = Client2Logical(point);

		BOOL bInRegion = FALSE;
		POSITION pos = m_lstCustumSurfaceRegion.GetHeadPosition();
		LOGICAL_REGION stRegion;	// machao add on 2013-09-18
		while(pos)
		{
			stRegion = m_lstCustumSurfaceRegion.GetNext(pos);	// machao modify on 2013-09-18
			float fMinY = min(stRegion.m_stFrom.y, stRegion.m_stTo.y);
			float fMaxY = max(stRegion.m_stFrom.y, stRegion.m_stTo.y);

			if ((m_stCurPoint.x > stRegion.m_stFrom.x) && (m_stCurPoint.x < stRegion.m_stTo.x) &&
				(m_stCurPoint.y > fMinY) && (m_stCurPoint.y < fMaxY))
			{
				bInRegion = TRUE;
				break;
			}
		}

		if (bInRegion)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENU_CUSTUM_SURFACE);
			// machao modify start on 2013-09-18
			CMenu *pMenu = menu.GetSubMenu(0);
			if (stRegion.m_nInstallEdgeDB == 0)
			{
				pMenu->EnableMenuItem(ID_SEL_SURFACE_EDGE, MF_GRAYED);
			}
			else if (stRegion.m_nInstallEdgeDB == 1)
			{
				pMenu->EnableMenuItem(ID_SEL_SURFACE_CENTER, MF_GRAYED);
			}
			else if (stRegion.m_nInstallEdgeDB == 2)
			{
				pMenu->EnableMenuItem(ID_SEL_SURFACE_EDGE, MF_GRAYED);
				pMenu->EnableMenuItem(ID_SEL_SURFACE_CENTER, MF_GRAYED);
			}
//			pMenu->DeleteMenu(2, MF_BYPOSITION);
			// machao modify end on 2013-09-18
			ClientToScreen(&point);	
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
	// Song Chenguang add end on 2013-06-20
	else
	{
		if ((m_bMeasureOn == TRUE && m_eMeasureType == ePolygon) ||
			(m_pCScanParamView->m_bSurfaceFlag && m_pCScanParamView->m_nSurfaceType == 3))
		{
			// �����ѻ��Ķ����
			InvertPolygon();

			m_nPolyNum = -1;    //�����Ҽ�������ζ������»�ȡ  yachang.huang add start on 2012-6-4
			m_fPolygonArea = 0.0;  //�����Ҽ���������������  yachang.huang add start on 2012-6-4
			//			ZeroMemory(m_Poly,sizeof(m_Poly));
			for (int i=0; i<POLY_VERTEX_NUM; i++)
			{
				m_Poly[i] = CPoint(0, 0);
			}	
		}

		Invalidate(FALSE);
	}

	//yachang.huang add start on 2012-8-16
	if (TRUE == m_bRota)
	{
		Invalidate(TRUE);
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRButtonDown"),"Leave");
		return;
	}
	//yachang.huang add start on 2012-8-16
	CScrollView::OnRButtonDown(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnRButtonDown"),"Leave");
}

/*
void CCScanView::CalcSurfaceColorRate()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	// ����Cɨ����
	int nColorMode = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nSurfceColorMode;

	CDC *pDC = this->GetDC();
	int nXLength = abs(m_ptFrom.x - m_ptTo.x);
	int nYLength = abs(m_ptFrom.y - m_ptTo.y);
	if(nXLength == 0 || nYLength == 0)
	{
	return;
	}
	COLORREF colorValue = 0;

	int nSurfaceTotalPixelNum = nXLength * nYLength;	// ����ѡ�е��������ص�����
	int nSurfacePixeNum[20];	//���˸���ɫ���ص�����
	for(int i = 0; i < 20; i++)
	{
	nSurfacePixeNum[i] = 0;
	}
	if(nColorMode == 0)
	{
		for(int i = 0; i <= nXLength; i++)
		{
			for(int j = 0; j <= nYLength; j++)
			{
				if(m_ptFrom.x < m_ptTo.x)
				{
					if(m_ptFrom.y < m_ptTo.y)
					{
						colorValue = pDC->GetPixel(m_ptFrom.x + i, m_ptFrom.y + j);				
						for(int k = 0; k < 16; k++)
						{
							int nGrayValue = (15 - k) * 17;
							if(colorValue == nGrayValue)
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
					else
					{
						colorValue = pDC->GetPixel(m_ptFrom.x + i, m_ptTo.y + j);
						for(int k = 0; k < 16; k++)
						{
							int nGrayValue = (15 - k) * 17;
							if(colorValue == nGrayValue)
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
				}
				else
				{
					if(m_ptFrom.y < m_ptTo.y)
					{
						colorValue = pDC->GetPixel(m_ptTo.x + i, m_ptFrom.y + j);
						for(int k = 0; k < 16; k++)
						{
							int nGrayValue = (15 - k) * 17;
							if(colorValue == nGrayValue)
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
					else
					{
						colorValue = pDC->GetPixel(m_ptTo.x + i, m_ptTo.y + j);
						for(int k = 0; k < 16; k++)
						{
							int nGrayValue = (15 - k) * 17;
							if(colorValue == nGrayValue)
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(int i = 0; i <= nXLength; i++)
		{
			for(int j = 0; j <= nYLength; j++)
			{
				if(m_ptFrom.x < m_ptTo.x)
				{
					if(m_ptFrom.y < m_ptTo.y)
					{
						COLORREF value = RGB(245, 245, 245);
						colorValue = pDC->GetPixel(m_ptFrom.x + i, m_ptFrom.y + j);				
						for(int k = 0; k < 20; k++)
						{
							if(colorValue == m_clrPlatte[k])
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
					else
					{
						colorValue = pDC->GetPixel(m_ptFrom.x + i, m_ptTo.y + j);
						for(int k = 0; k < 20; k++)
						{
							if(colorValue == m_clrPlatte[k])
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
				}
				else
				{
					if(m_ptFrom.y < m_ptTo.y)
					{
						colorValue = pDC->GetPixel(m_ptTo.x + i, m_ptFrom.y + j);
						for(int k = 0; k < 20; k++)
						{
							if(colorValue == m_clrPlatte[k])
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
					else
					{
						colorValue = pDC->GetPixel(m_ptTo.x + i, m_ptTo.y + j);
						for(int k = 0; k < 20; k++)
						{
							if(colorValue == m_clrPlatte[k])
							{
								nSurfacePixeNum[k]++;
							}
						}
					}
				}
			}
		}
	}

	for(int i = 0; i < 20; i++)
	{
		m_fSurfacePixeRate[i] = nSurfacePixeNum[i] / nSurfaceTotalPixelNum * 100.0f;
	}
}
*/
void CCScanView::CalcSurfaceColorRate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcSurfaceColorRate"),"Enter");
	// ��ȡ������Ϣ
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;
	if(m_nObjectNum == 1)
	{
		pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	}
	else
	{
		pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	}
	float fThick = pCScanParam->m_fThick;	//�������
	VARIANT Result = pCScanParam->m_PlateDrawCtrl.GetPlaneFlawPoints2();
	int nColorMode = pCScanParam->m_nSurfceColorMode;
	int nZoomScale = pCScanParam->m_nZoomScale;
	PlaneFlawHeader stPFH = {0};
	long lLBound = 0;
	long lUBound = 0;
	::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//һά�����±�
	::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//һά�����ϱ�

	char* buf = NULL;
	SafeArrayAccessData(Result.parray,   (void   **)&buf);   

	UINT Size = (lUBound-lLBound+1);	

	int nPos = 0;
	int i = 0;
	int nColorIndex = 0;
	float fGrayScale;	//�Ҷȱ���
	// wangbingfu modify start on 2015-07-08
/*	LOGICAL_POINT_t ptLogicalFrom;
	LOGICAL_POINT_t ptLogicalTo;
	ptLogicalFrom = Client2Logical(m_ptFrom);
	ptLogicalTo = Client2Logical(m_ptTo);
	float fAllArea = 0;	// �����
	if(pCScanParam->m_nSurfaceType == 0)	// ����
	{
		fAllArea = fabs(ptLogicalFrom.x - ptLogicalTo.x) * fabs(ptLogicalFrom.y - ptLogicalTo.y);	
	}
	else if(pCScanParam->m_nSurfaceType == 1)	 // Բ 
	{
		float  fRadius = fabs(ptLogicalFrom.x - ptLogicalTo.x) / 2;
		fAllArea =static_cast<float>(PI * fRadius * fRadius);
	}
	else if(pCScanParam->m_nSurfaceType == 2)	// ��Բ
	{
		float fA = fabs(ptLogicalFrom.x - ptLogicalTo.x) / 2;
		float fB = fabs(ptLogicalFrom.y - ptLogicalTo.y) / 2;
		fAllArea = static_cast<float>(PI * fA * fB);
	}
	else if(pCScanParam->m_nSurfaceType == 3)	// �����
	{
		CalculatePolygonArea();
		fAllArea = m_fPolygonArea;
	}
	else
	{
		;//
	}
	m_SelArea=fAllArea;
	*/
	CalcHanHeColorRate();
	float fAllArea = m_SelArea;	// �����
	// wangbingfu modify end on 2015-07-08

	float fArea[20];
	for(int i = 0; i < 20; i++)
	{
		fArea[i] = 0;
	}
	while(nPos+sizeof(PlaneFlawHeader) < Size)
	{	
		memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));

		if(nColorMode == 0)
		{
			fGrayScale = static_cast<float>(stPFH.z / fThick * 16);
			if(fGrayScale > 16)
			{
				fGrayScale = 16;
			}
			else if(fGrayScale < 0)
			{
				fGrayScale = 0;
			}

			fGrayScale = 16 - fGrayScale;
			int nColorIndex = (int)fGrayScale;
			fArea[nColorIndex] += static_cast<float>(stPFH.area / (nZoomScale * nZoomScale));
		}
		else
		{
			nColorIndex = int(stPFH.z / fThick * 20);
			if(nColorIndex > 19)
			{
				nColorIndex = 19;
			}
			fArea[nColorIndex] += static_cast<float>(stPFH.area / (nZoomScale * nZoomScale));
		}

		int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT)*stPFH.PointsCount;
		nPos += sizetmp;
	}

	float fAllFlawArea = 0.0f;	// ���������
	for(int i = 0; i < 20; i++)
	{
		m_fSurfacePixeRate[i] = fArea[i] / fAllArea * 100.0f;
		fAllFlawArea += fArea[i];
	}
	m_fSurfacePixeRate[20] = (fAllArea - fAllFlawArea ) / fAllArea * 100;// �����������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcSurfaceColorRate"),"Leave");
}

LRESULT CCScanView::OnMouseLeave(WPARAM wParam, LPARAM lParam) 
{ 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseLeave"),"Enter");
	m_bMouseLeave = FALSE; 
	// �����ﴦ��Mouse�뿪�¼� 

	CDC* pDC = GetDC();
	// wangbingfu add start on 2012-11-03
	if(pDC == NULL)
	{
		MessageBox(_T("CScanView  OnMouseLeave(), pDC is null"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnMouseLeave"),"Leave");
		return 0;
	}
	// wangbingfu add end on 2012-11-03
	InvertRegion(pDC, m_ptCurPos, m_ptTo, eLINE);

	ReleaseDC(pDC);

	m_nEnterCount = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnMouseLeave"),"Leave");
	return 0; 
}
void CCScanView::PictureModeText(int nIndex,CString strText)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PictureModeText"),"Enter");		
	if(nIndex < 7)
	{
		if(m_bFileFeatureFlag[nIndex])
		{
			CString strText1;
			if(nIndex == 1)
			{
				strText1 = _T("һ�β������ֵʱ��, ");
			}
			else if(nIndex == 2)
			{
				strText1 = _T("һ�β���ʱ��, ");
			}
			else if(nIndex == 3)
			{
				strText1 = _T("��󲨷����ֵʱ��, ");
			}
			else if(nIndex == 4)
			{
				strText1 = _T("��󲨷�ʱ��, ");
			}
			else if(nIndex == 5)
			{
				strText1 = _T("һ�β����ֵ, ");
			}
			else if(nIndex == 6)
			{
				strText1 = _T("��󲨷��ֵ, ");
			}
			strText = strText + strText1;
			nIndex++;
			PictureModeText(nIndex, strText);
		}
		else
		{
			nIndex++;
			PictureModeText(nIndex, strText);
		}
	}
	else
	{
		m_strPictureMode = _T("");
		m_strPictureMode = strText;
		return;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PictureModeText"),"Leave");	
}

void CCScanView::PictureGateText(int nIndex,CString strText)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PictureGateText"),"Enter");		
	if(nIndex < 4)
	{
		if(m_bFileGateFlag[g_dwActiveChannel][nIndex])
		{
			CString strText1;
			if(nIndex == 0)
			{
				strText1 = _T("բ��I, ");
			}
			else if(nIndex == 1)
			{
				strText1 = _T("բ��A, ");
			}
			else if(nIndex == 2)
			{
				strText1 = _T("բ��B, ");
			}
			else if(nIndex == 3)
			{
				strText1 = _T("բ��C ");
			}
			strText = strText + strText1;
			nIndex++;
			PictureGateText(nIndex, strText);
		}
		else
		{
			nIndex++;
			PictureGateText(nIndex, strText);
		}
	}
	else
	{
		m_strPictureGate = _T("");
		m_strPictureGate = strText;
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PictureGateText"),"Leave");	
		return;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PictureGateText"),"Leave");	
}
// ���뻭ͼ����ڴ�
BOOL CCScanView::MallocPictureMemory()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Enter");	
	CDC* pDC = GetDC();
	// wangbingfu add start on 2012-11-03
	if(pDC == NULL)
	{
		MessageBox(_T("CScanView  OnMouseLeave(), pDC is null"), _T("����"), MB_OK);
//		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	// wangbingfu add end on 2012-11-03

	BOOL  bRet = m_MemDC.CreateCompatibleDC(pDC);
	if(!bRet)
	{
		MessageBox(_T("DC���洴��ʧ�ܣ�"), MB_OK);
//		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	// machao add start on 2013-11-27
	if (m_nAxisSelect == 5)
	{
		if (m_nPipeXMode == 0)
		{
			m_dXEnd = 360;
		}
		else
		{
			m_dXEnd = PI * m_dPipeDiameter;
		}
	}
	// machao add end on 2013-11-27
	m_nMemXLENGTH = (static_cast<int>((m_dXEnd - m_dXStart) / m_fGridX) + 200) ;/// 8 * 8; //(m_dXEnd - m_dXStart) / m_fGridX;
	m_nMemYLENGTH = (static_cast<int>((m_dYEnd - m_dYStart) / m_fGridY) + 200) ;/// 8 * 8;//(m_dYEnd - m_dYStart) / m_fGridY; �����1����������ڴ�ʱ����
	m_CScanSize = CSize(m_nMemXLENGTH, m_nMemYLENGTH);
	//machao add start on 2013-07-15
	if (!m_bNoiseReduction)
	{
		m_pfSrcData = new float*[m_nMemXLENGTH];
		if(m_pfSrcData == NULL)
		{
			MessageBox(_T("CScanView MallocPictureMemory(), m_pfSrcData malloc fail"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
			return FALSE;
		}
		for (int i = 0; i < m_nMemXLENGTH; i++)
		{
			m_pfSrcData[i] = new float[m_nMemYLENGTH];
			if (NULL == m_pfSrcData[i])
			{
				MessageBox(_T("CCScanView::CCScanView(),m_pfSrcData[i] = null"), _T("����"), MB_OK|MB_ICONERROR);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CCScanView"),"Leave");
				return FALSE;
			}
			for (int j = 0; j < m_nMemYLENGTH; j++)
			{
				m_pfSrcData[i][j] = -1;
			}
		}
	}
	//machao add end on 2013-07-15
	CBitmap Membitmap;
	bRet = Membitmap.CreateCompatibleBitmap(pDC, m_CScanSize.cx, m_CScanSize.cy);
	if(!bRet)
	{
		MessageBox(_T("Membmp����ʧ�ܣ�"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	m_MemDC.SelectObject(&Membitmap);
	Membitmap.DeleteObject();
	m_MemDC.FillSolidRect(0, 0, m_CScanSize.cx, m_CScanSize.cy, m_cscan_back_color);
/*	bRet = m_Membitmap.CreateCompatibleBitmap(pDC, m_CScanSize.cx, m_CScanSize.cy);
	if(!bRet)
	{
		MessageBox(_T("Membmp����ʧ�ܣ�"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	m_pOldBitmap = m_MemDC.SelectObject(&m_Membitmap);	// Song Chenguang modified on 2012-11-05
	m_MemDC.FillSolidRect(0, 0, m_CScanSize.cx, m_CScanSize.cy, m_cscan_back_color);
*/
	// wangbingfu modify start on 2012-11-05
	m_pPixel = new float*[m_nMemXLENGTH];
	if(m_pPixel == NULL)
	{
		MessageBox(_T("CScanView MallocPictureMemory(), m_pPixel malloc fail"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		m_pPixel[i] = new float[m_nMemYLENGTH];
		if(m_pPixel[i] == NULL)
		{
			CString csResult;
			csResult.Format(_T("CScanView MallocPictureMemory(), m_pPixel[%d] malloc fail"), i);
			MessageBox(csResult, _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
			return FALSE;
		}
		memset(m_pPixel[i], 0, m_nMemYLENGTH*sizeof(float));
	}	
	//���ص�����������
	m_pPixelCount = new int*[m_nMemXLENGTH];
	if(m_pPixelCount == NULL)
	{
		MessageBox(_T("CScanView MallocPictureMemory(), m_pPixelCount malloc fail"), _T("����"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
		return FALSE;
	}
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		m_pPixelCount[i] = new int[m_nMemYLENGTH];
		if(m_pPixel[i] == NULL)
		{
			CString csResult;
			csResult.Format(_T("CScanView MallocPictureMemory(), m_pPixelCount[%d] malloc fail"), i);
			MessageBox(csResult, _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");	
			return FALSE;
		}
		memset(m_pPixelCount[i], 0, m_nMemYLENGTH*sizeof(int));
	}
	// wangbingfu modify end on 2012-11-05

	ReleaseDC(pDC);	// wangbingfu add on 2012-11-03

	return TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::MallocPictureMemory"),"Leave");		
}
// �����ͼ����ڴ�
void CCScanView::FreePictureMemory()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::FreePictureMemory"),"Enter");
	// wangbingfu modify start on 2012-11-03
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		if(m_pPixel[i] != NULL)
		{
			delete[] (m_pPixel[i]);			
			m_pPixel[i] = NULL;
		}	
	}
	if(m_pPixel != NULL)
	{
		delete[] m_pPixel;
		m_pPixel = NULL;
	}	

	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		if(m_pPixelCount[i] != NULL)
		{
			delete[] (m_pPixelCount[i]);
			m_pPixelCount[i] = NULL;
		}
	}
	if(m_pPixelCount != NULL)
	{
		delete[] m_pPixelCount;
		m_pPixelCount = NULL;
	}	
	// wangbingfu modify start on 2012-11-03

	// machao add start on 2013-07-25
	if(!m_bNoiseReduction)
	{
		if(m_pfSrcData != NULL)
		{
			for(int i = 0; i < m_nMemXLENGTH; i++)
			{
				if(m_pfSrcData[i] != NULL)
				{
					delete[] m_pfSrcData[i];
					m_pfSrcData[i] = NULL;
				}
			}
			if(m_pfSrcData != NULL)
			{
				delete[] m_pfSrcData;
				m_pfSrcData = NULL;
			}
		}
	}
	// machao add end on 2013-07-25

	m_MemDC.DeleteDC();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::FreePictureMemory"),"Leave");
}

void CCScanView::CaculateVelocity()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateVelocity"),"Enter");
	// TODO: Add your control notification handler code here
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// ����Cɨ����
	if(m_nObjectNum == 1)
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;	//�������
	}
	else
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fThick;	//�������	
	}
	int nCount = 0;
	double	wavevelocitysum=0;
	double	wavevelocity=0;
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);
			float fRange = ndata.m_pfFeatures[2][2] - ndata.m_pfFeatures[1][2];

			if (fRange < 1E-2)
			{
				continue;
			}
			wavevelocity = 2 * m_fThick * 1000 / Range2Dot(pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed, fRange) * 100;
			//		wavevelocity = 2 * m_fThick * 1000 / Range2Dot(pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed, 8) * 100;
			wavevelocitysum=wavevelocitysum+wavevelocity;
			nCount++;
		}
	}
	if(m_nObjectNum == 1)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fAveVelocity = wavevelocitysum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(FALSE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fAveVelocity = wavevelocitysum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->UpdateData(FALSE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateVelocity"),"Leave");
}

void CCScanView::CalcuateShuaijian()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcuateShuaijian"),"Enter");
	// TODO: Add your control notification handler code here
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// ����Cɨ����
	if(m_nObjectNum == 1)
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;	//�������
	}
	else
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fThick;	//�������	
	}
	int nCount = 0;
	double	sjsum=0;//˥����
	double	sj=0;//��ǰ˥��
	//ʹ��һ�εײ������εײ�������˥��
	double	m=1;
	double	n=2;
	double	szk=0;//��ǰ�迹
	//ʹ��һ�εײ������εײ��������迹
	double	z1=1.5;//ˮ�����迹
	double	a=0;//��ֵ��
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);
			if(ndata.m_pfFeatures[1][3] > 0 && ndata.m_pfFeatures[2][3] > 0)
			{
				a=((double)ndata.m_pfFeatures[2][3])/((double)ndata.m_pfFeatures[1][3]);
			}

			//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
			if (a>=1)
			{
				continue;
			}
			szk=z1*(-2*(a+1)-4*sqrt(a))/(2*(a-1));
			sj=(20*log10(1/a)-20*log10(n/m)-(2*(n-m)-1)*20*log10(fabs((szk-z1)/(z1+szk))))/(2*(n-m)*m_fThick);
			//�޳�������ĵ㣬������sj����Ϊ��
			if (sj<=0)
			{
				continue;
			}
			sjsum=sj+sjsum;
			nCount++;
		}
	}
	if(m_nObjectNum == 1)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fAveShuaiJian = sjsum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(FALSE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fAveShuaiJian = sjsum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->UpdateData(FALSE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcuateShuaijian"),"Leave");
}

void CCScanView::CaculateSzk()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateSzk"),"Enter");
	// TODO: Add your control notification handler code here
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	int nCount = 0;
	double	szksum=0;//�迹��
	double	szk=0;//��ǰ�迹
	//ʹ��һ�εײ������εײ��������迹
	double	z1=1.5;//ˮ�����迹
	double	a=0;//��ֵ��
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);
			if(ndata.m_pfFeatures[1][3] > 0)
			{
				a=((double)ndata.m_pfFeatures[2][3])/((double)ndata.m_pfFeatures[1][3]);
			}

			//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
			if (a>=1)
			{
				continue;
			}
			szk=z1*(-2*(a+1)-4*sqrt(a))/(2*(a-1));
			szksum=szk+szksum;
			nCount++;
		}
	}
	if(m_nObjectNum == 1)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fAveSzk = szksum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(FALSE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fAveSzk = szksum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->UpdateData(FALSE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateSzk"),"Leave");	
}

void CCScanView::CaculateMidu()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateMidu"),"Enter");
	// TODO: Add your control notification handler code here
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(m_nObjectNum == 1)
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;	//�������
	}
	else
	{
		m_fThick = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fThick;	//�������	
	}

	int nCount = 0;
	double	midusum=0;//�ܶȺ�
	double	midu=0;//��ǰ�ܶ�
	double	szk;//��ǰ�迹
	double	wavevelocity;//��ǰ����
	//ʹ��һ�εײ������εײ��������迹
	double	z1=1.5;//ˮ�����迹
	double	a=0;//��ֵ��
	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);
			if(ndata.m_pfFeatures[1][3] > 0)
			{
				a=((double)ndata.m_pfFeatures[2][3])/((double)ndata.m_pfFeatures[1][3]);
			}

			//�޳�������ĵ㣬�����϶��εײ�С��һ�εײ������aӦ��С��1������Ϊ��ֵ��Ҫ����0
			if (a>=1)
			{
				continue;
			}
			float fRange = ndata.m_pfFeatures[2][2] - ndata.m_pfFeatures[1][2];

			if (fRange < 1E-2)
			{
				continue;
			}
			szk=z1*(-2*(a+1)-4*sqrt(a))/(2*(a-1));
			wavevelocity = 2 * m_fThick * 1000 / Range2Dot(pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed, fRange) * 100;
			midu=szk/(wavevelocity / 1000) * 1000;
			midusum=midusum+midu;
			nCount++;
		}
	}
	if(m_nObjectNum == 1)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fAveMidu = midusum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(FALSE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_fAveMidu = midusum/nCount;
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->UpdateData(FALSE);
	}	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CaculateMidu"),"Leave");	
}


void CCScanView::GetPrintMessage(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetPrintMessage"),"Enter");
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString strFileName = CString(strFolder) + _T("printmsg.ini");
	::GetPrivateProfileSection(_T("�вĻ�����Ϣ"), m_tcBuf_S1,400, strFileName);
	{	//ȡ�������������
		_TCHAR *pbuf=m_tcBuf_S1;
		size_t size=wcslen(pbuf);
		int j=0;
		while (size)   
		{
//			m_S1_ReportItems[j].strKey = pbuf;
			pbuf+=size+1;
			size=wcslen(pbuf);
			j++;
		}
		m_nINI_S1_NUM=j;
	}


	::GetPrivateProfileSection(_T("̽�˻�����Ϣ"),m_tcBuf_S2,400, strFileName);
	{
		_TCHAR *pbuf=m_tcBuf_S2;
		size_t size=wcslen(pbuf);
		int k=0;
		while (size)
		{
//			m_S2_ReportItems[k].strKey = pbuf;

			pbuf+=size+1;
			size=wcslen(pbuf);
			k++;
		}
		m_nINI_S2_NUM=k;
	}


	::GetPrivateProfileSection(_T("̽�˽��"),m_tcBuf_S3,400, strFileName); 
	{
		_TCHAR *pbuf=m_tcBuf_S3;
		size_t size=wcslen(pbuf);
		int n=0;
		while (size)
		{
//			m_S3_ReportItems[n].strKey = pbuf;

			pbuf+=size+1;
			size=wcslen(pbuf);
			n++;
		}
		m_nINI_S3_NUM=n;
	}

	if(( m_nINI_S3_NUM%2 ) == 1)
	{
		m_nS3_NUM = m_nINI_S3_NUM + 1;
	}
	else
	{
		m_nS3_NUM = m_nINI_S3_NUM;
	}//end if

	if(( m_nINI_S2_NUM%2 ) == 1)
	{
		m_nS2_NUM = m_nINI_S2_NUM + 1;
	}
	else
	{
		m_nS2_NUM = m_nINI_S2_NUM;
	}//end if

	if(( m_nINI_S1_NUM%2 ) == 1)
	{
		m_nS1_NUM = m_nINI_S1_NUM + 1;
	}
	else
	{
		m_nS1_NUM = m_nINI_S1_NUM;
	}//end if
	memset(m_tcBuf_S1, 0, sizeof(m_tcBuf_S1));
	memset(m_tcBuf_S2, 0, sizeof(m_tcBuf_S2));
	memset(m_tcBuf_S3, 0, sizeof(m_tcBuf_S3));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetPrintMessage"),"Leave");
	return;
}
//������ͼ�����+����+���ɫ+���Σ�
void CCScanView::PrintDefaultWave(CDC* pDC,const CRect&rect, const LOGICAL_POINT_t& point, CPicData*pascan)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintDefaultWave"),"Enter");
	CRect rectDraw; // �������� 
	rectDraw.top = rect.top;
	rectDraw.bottom = rect.bottom - MARGIN_X;
	rectDraw.left = rect.left + MARGIN_X;
	rectDraw.right = rect.right;
	// 
	//CMainFrame * pFrame = (CMainFrame*)AfxGetMainWnd();
	//CUltrasonicImmersedTestView* pView =  (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);

	// �������(�˴����޸�)
	//float fStep = (m_fXMaxRuler - m_fXMinRuler)/10.0;//�̶ȳߵĲ���

	CRect rectScaleX;  
	rectScaleX.top = rect.top;
	rectScaleX.bottom = rect.bottom - MARGIN_X;
	rectScaleX.left = rect.left;
	rectScaleX.right = rect.left + MARGIN_X;

	CRect rectScaleY;

	rectScaleY.top = rect.bottom - MARGIN_X;
	rectScaleY.bottom = rect.bottom;
	rectScaleY.left = rect.left + MARGIN_X;
	rectScaleY.right = rect.right;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
	DrawScale(pDC, rectScaleY, pView->m_fXMinRuler, pView->m_fXMaxRuler, (pView->m_fXMaxRuler - pView->m_fXMinRuler) / 10, 3, TRUE);
	DrawScale(pDC, rectScaleX, 0, 100, 10, 4, TRUE);
	// ��������   
	//CMemDC dcMem(pDC, &rectDraw);
	pDC->FillSolidRect(rectDraw,BLACK_BRUSH);
	// ����������
	DrawGrids(pDC, rectDraw);

	// ������
	DrawWaves(pDC, rectDraw, point ,pascan);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintDefaultWave"),"Leave");
}
void CCScanView::DrawDefaultResultTable(CDC* pDC,const CRect&rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDefaultResultTable"),"Enter");
	TEXTMETRIC tm;
	::ZeroMemory(&tm, sizeof(TEXTMETRIC));
	pDC->GetTextMetrics(&tm);


	//����̽�˱��
	int nYPos = m_nYPos;
	int nXPos = rect.left;
	nYPos += (tm.tmHeight + tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	pDC->MoveTo(rect.right, nYPos);
	pDC->LineTo(rect.right, nYPos + 3 * (tm.tmHeight + tm.tmExternalLeading + 10));

	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.left, nYPos + 3 * (tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 8 * tm.tmAveCharWidth, nYPos);
	pDC->LineTo(rect.left + 8 * tm.tmAveCharWidth, nYPos + 3 * (tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 15 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos);
	pDC->LineTo(rect.left + 15 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 24 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos);
	pDC->LineTo(rect.left + 24 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 30 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 30 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 40 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 40 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 45 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET+10, nYPos);
	pDC->LineTo(rect.left + 45 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET+10, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 57 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 57 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 69 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET,nYPos);
	pDC->LineTo(rect.left + 69 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 84 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 84 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + 3 * (tm.tmHeight +tm.tmExternalLeading + 10));
	nYPos += (tm.tmHeight + tm.tmExternalLeading) + 10;

	m_nYPos = nYPos;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDefaultResultTable"),"Leave");
}
void CCScanView::DrawDefaultResultTable1(CDC* pDC,const CRect&rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDefaultResultTable1"),"Enter");
	TEXTMETRIC tm;
	::ZeroMemory(&tm, sizeof(TEXTMETRIC));
	pDC->GetTextMetrics(&tm);
	int nYPos = m_nYPos;
	int nXPos = rect.left;

	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->MoveTo(rect.right, nYPos);
	pDC->LineTo(rect.right, nYPos +(tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.left, nYPos +(tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 8 * tm.tmAveCharWidth, nYPos);
	pDC->LineTo(rect.left + 8 * tm.tmAveCharWidth, nYPos + (tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 15 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos);
	pDC->LineTo(rect.left + 15 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 24 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos);
	pDC->LineTo(rect.left + 24 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 30 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 30 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 40 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 40 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 45 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos);
	pDC->LineTo(rect.left + 45 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET + 10, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 57 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 57 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 69 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET,nYPos);
	pDC->LineTo(rect.left + 69 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left + 84 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos);
	pDC->LineTo(rect.left + 84 * tm.tmAveCharWidth + REPORT_ITEM_OFFSET, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->MoveTo(rect.left, nYPos + (tm.tmHeight + tm.tmExternalLeading + 10));
	pDC->LineTo(rect.right, nYPos + (tm.tmHeight + tm.tmExternalLeading + 10));
	m_nYPos = nYPos;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawDefaultResultTable1"),"Leave");
}
void CCScanView::PrintMessageTableOne(CDC* pDC,const CRect&rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintMessageTableOne"),"Enter");
	int nYPos = m_nYPos;
	int nXPos = rect.left;

	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 1200;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, _T("Times New Roman"));

	CFont fontTitle;
	fontTitle.CreatePointFontIndirect(&lf);

	CFont* pOldFont = pDC->SelectObject(&fontTitle);

	TEXTMETRIC tm;
	::ZeroMemory(&tm, sizeof(TEXTMETRIC));
	pDC->GetTextMetrics(&tm);

	nYPos += (tm.tmHeight +tm.tmExternalLeading);
	CString strTitle = _T("��άˮ������ϵͳ");
	UINT nOldAlign = pDC->SetTextAlign(TA_CENTER);
	pDC->TextOut(rect.CenterPoint().x, nYPos + 10, strTitle);

	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	pDC->SelectObject(pOldFont);
	pDC->GetTextMetrics(&tm);

	pDC->SetTextAlign(TA_TOP | TA_LEFT);

	//̽�˻�����Ϣ����printmsg.ini�л�ȡ��
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->TextOut(rect.left + REPORT_ITEM_OFFSET, nYPos + 10, _T("�вĻ�����Ϣ"));
	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	nXPos = rect.left;
	for(int i = 0; i < m_nS1_NUM; i++)
	{
		if ((i%2) == 0 && i != 0)
		{
			nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
			nXPos = rect.left;

			pDC->MoveTo(rect.left, nYPos);
			pDC->LineTo(rect.right, nYPos);
		}

		if ((i%2) == 1)
		{
			nXPos += (rect.Width() / 2);

			pDC->MoveTo(nXPos, nYPos);
			pDC->LineTo(nXPos, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
		}

		pDC->MoveTo(nXPos + rect.Width() / 4, nYPos);
		pDC->LineTo(nXPos + rect.Width() / 4, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));

		pDC->TextOut(nXPos + REPORT_ITEM_OFFSET, nYPos + 10, m_S1_ReportItems[i].strKey);
		pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10, m_S1_ReportItems[i].strItem);
	}


	nXPos =rect.left;
	pDC->MoveTo(nXPos, nYPos);
	pDC->LineTo(nXPos, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));

	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->TextOut(rect.left + REPORT_ITEM_OFFSET, nYPos + 10, _T("̽�˻�����Ϣ"));
	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	nXPos = rect.left;
	for(int i = 0; i < m_nS2_NUM; i++)
	{
		if ((i%2) == 0 && i != 0)
		{
			nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
			nXPos = rect.left;

			pDC->MoveTo(rect.left, nYPos);
			pDC->LineTo(rect.right, nYPos);
		}

		if ((i%2) == 1)
		{
			nXPos += (rect.Width() / 2);

			pDC->MoveTo(nXPos, nYPos);
			pDC->LineTo(nXPos, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));
		}

		pDC->MoveTo(nXPos + rect.Width() / 4, nYPos);
		pDC->LineTo(nXPos + rect.Width() / 4, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));

		pDC->TextOut(nXPos + REPORT_ITEM_OFFSET, nYPos + 10, m_S2_ReportItems[i].strKey);
		pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10, m_S2_ReportItems[i].strItem);
	}
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	pDC->TextOut(rect.left + REPORT_ITEM_OFFSET, nYPos + 10, _T("̽�˽��"));
	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);
	nXPos = rect.left;

	for(int i = 0; i < m_nS3_NUM; i++)
	{
		if ((i%2) == 0 && i != 0)
		{
			nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
			nXPos = rect.left;
			pDC->MoveTo(rect.left, nYPos);
			pDC->LineTo(rect.right, nYPos);
		}

		if ((i%2) == 1)
		{
			nXPos += (rect.Width() / 2);
			pDC->MoveTo(nXPos, nYPos);
			pDC->LineTo(nXPos, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));

		}
		pDC->MoveTo(nXPos + rect.Width() / 4, nYPos);
		pDC->LineTo(nXPos + rect.Width() / 4, nYPos + (tm.tmHeight +tm.tmExternalLeading + 10));

		pDC->TextOut(nXPos + REPORT_ITEM_OFFSET, nYPos + 10, m_S3_ReportItems[i].strKey);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCScanParamPanel* pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;

		double strC1 = pCScanParam->m_PlateDrawCtrl.GetBodyAvgQueXianLv();
		double strD1 = pCScanParam->m_PlateDrawCtrl.GetBodyMaxShangZhiShiLength();
		double strE1 = pCScanParam->m_PlateDrawCtrl.GetBodyMaxQueXianArea();
		//	double strF1 = pCScanParam->m_PlateDrawCtrl.GetBodyMaxQueXianLv();//��ѭ��
		m_S3_ReportItems[1].strItem.Format(_T("%d"), m_nFlawPrintRow_NUM);
		m_S3_ReportItems[3].strItem.Format(_T("%.8f"), strC1*100);
		m_S3_ReportItems[4].strItem.Format(_T("%.2f"), strD1/10);
		m_S3_ReportItems[5].strItem.Format(_T("%.2f"), strE1/100);
		//strF.Format(_T("%.8f"), strF1);
		switch (i+1)
		{
		case 1: 
			pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,m_S3_ReportItems[1].strItem);
			break;
		case 2 :
			pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,m_csTotalStrArea);
			break;
		case 3: 
			pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,m_S3_ReportItems[3].strItem);
			break;
		case 4: 
			pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,m_S3_ReportItems[4].strItem);
			break;
		case 5: 
			pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,m_S3_ReportItems[5].strItem);
			break;
			//	case 6: pDC->TextOut(nXPos + rect.Width() / 4 + REPORT_ITEM_OFFSET, nYPos + 10,strF);break;
		default:
			break;
		}
	}
	nYPos += (tm.tmHeight +tm.tmExternalLeading + 10);
	pDC->MoveTo(rect.left, nYPos);
	pDC->LineTo(rect.right, nYPos);

	m_nYPos = nYPos;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PrintMessageTableOne"),"Leave");
}
void  CCScanView::DrawWaves(CDC* pDC, const CRect& rect, const LOGICAL_POINT_t& point, CPicData*pascan)
{ 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawWaves"),"Enter");
	LOGICAL_POINT_t i_pos;
// 	i_pos.x = m_DefaultMax_X;
// 	i_pos.y = m_DefaultMax_Y;
	i_pos.x = point.x;
	i_pos.y = point.y;

	

	CPen pen(PS_SOLID,1,RGB(255,255,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int nSize = MONITOR_DATA_SIZE/AD_SIZE;
	int height = rect.Height() - 1;
	float yb = rect.Height() / MAX_HEIGHT * WAVE_ZOOM_SCALE;
	float xb = rect.Width()/float(nSize);

	//edit by lijunwei 2012-05-08
	for (int i=0; i < nSize; i++)
	{
		if (pascan->m_wData[i] > MAX_HEIGHT) 
			pascan->m_wData[i] = static_cast<WORD>(MAX_HEIGHT);
		if (i == 0)
			pDC->MoveTo(rect.left, static_cast<int>(rect.bottom - pascan->m_wData[i]*yb));
		else
			pDC->LineTo(static_cast<int>(rect.left + i*xb), static_cast<int>(rect.bottom - pascan->m_wData[i]*yb));
	}
	pDC->SelectObject(pOldPen);

/*
	
	CPicData* i_picdata = getAScanDataByPos(i_pos);
	for (int i=0; i < nSize; i++)
	{
		if (i_picdata->m_wData[i] > MAX_HEIGHT) 
			i_picdata->m_wData[i] = MAX_HEIGHT;
		if (i == 0)
			pDC->MoveTo(rect.left, rect.bottom - i_picdata->m_wData[i]*yb);
		else
			pDC->LineTo(rect.left + i*xb, rect.bottom - i_picdata->m_wData[i]*yb);
	}

	pDC->SelectObject(pOldPen);

	delete i_picdata;
	i_picdata = NULL;
*/
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawWaves"),"Leave");
}

void CCScanView::CalcSelectArea(LOGICAL_POINT_t ptLogicalFrom, LOGICAL_POINT_t ptLogicalTo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcSelectArea"),"Enter");
	float fAllArea = 0;	// �����
	if(m_pCScanParamView->m_nSurfaceType == 0)	// ����
	{
		fAllArea = fabs(ptLogicalFrom.x - ptLogicalTo.x) * fabs(ptLogicalFrom.y - ptLogicalTo.y);	
	}
	else if(m_pCScanParamView->m_nSurfaceType == 1)	 // Բ 
	{
		float  fRadius = static_cast<float>(fabs(ptLogicalFrom.x - ptLogicalTo.x) / 2);
		fAllArea = static_cast<float>(PI * fRadius * fRadius);
	}
	else if(m_pCScanParamView->m_nSurfaceType == 2)	// ��Բ
	{
		float fA = fabs(ptLogicalFrom.x - ptLogicalTo.x) / 2;
		float fB = fabs(ptLogicalFrom.y - ptLogicalTo.y) / 2;
		fAllArea = static_cast<float>(PI * fA * fB);
	}
	else if(m_pCScanParamView->m_nSurfaceType == 3)	// �����
	{
		CalculatePolygonArea();
		fAllArea = m_fPolygonArea;
	}
	else
	{
		;//
	}
	m_SelArea = fAllArea;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcSelectArea"),"Leave");
}
BOOL CCScanView::DataInSelArea(CFeatureInfo ndata, LOGICAL_POINT_t* ptArray)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DataInSelArea"),"Enter");

	BOOL bRet = FALSE;
	if (m_pCScanParamView->m_nSurfaceType == 0)	// ����
	{
		// machao modify start on 2013-09-3
// 		if((ndata.m_dX < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dX > m_pCScanParamView->m_fLogicalToX)
// 			|| (ndata.m_dY < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dY > m_pCScanParamView->m_fLogicalToY))
// 		{
// 			bRet = FALSE;
// 		}
// 		else
// 		{
// 			bRet = TRUE;
// 		}
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
		switch(nAxisIndex)
		{
		case 0:
	 		if((ndata.m_dX < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dX > m_pCScanParamView->m_fLogicalToX)
	 			|| (ndata.m_dY < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dY > m_pCScanParamView->m_fLogicalToY))
	 		{
	 			bRet = FALSE;
	 		}
	 		else
	 		{
	 			bRet = TRUE;
	 		}
			break;
		case 1:
			if((ndata.m_dX < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dX > m_pCScanParamView->m_fLogicalToX)
				|| (ndata.m_dZ < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dZ > m_pCScanParamView->m_fLogicalToY))
			{
				bRet = FALSE;
			}
			else
			{
				bRet = TRUE;
			}
			break;
		case 2:
			if((ndata.m_dY < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dY > m_pCScanParamView->m_fLogicalToX)
				|| (ndata.m_dZ < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dZ > m_pCScanParamView->m_fLogicalToY))
			{
				bRet = FALSE;
			}
			else
			{
				bRet = TRUE;
			}
			break;
		case 3:
		case 4:
			if((ndata.m_dX < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dX > m_pCScanParamView->m_fLogicalToX)
				|| (ndata.m_dR < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dR > m_pCScanParamView->m_fLogicalToY))
			{
				bRet = FALSE;
			}
			else
			{
				bRet = TRUE;
			}
			break;
		case 5:
			if((ndata.m_dW < m_pCScanParamView->m_fLogicalFromX) || (ndata.m_dW > m_pCScanParamView->m_fLogicalToX)
				|| (ndata.m_dZ < m_pCScanParamView->m_fLogicalFromY) || (ndata.m_dZ > m_pCScanParamView->m_fLogicalToY))
			{
				bRet = FALSE;
			}
			else
			{
				bRet = TRUE;
			}
			break;
		}
		// machao modift end on 2013-09-3
	}
	else if (m_pCScanParamView->m_nSurfaceType == 1)	// Բ
	{
		bRet = m_pCScanParamView->IsInCircle(ndata, m_pCScanParamView->m_fLogicalFromX, m_pCScanParamView->m_fLogicalToX, 
			m_pCScanParamView->m_fLogicalFromY, m_pCScanParamView->m_fLogicalToY);
	}
	else if (m_pCScanParamView->m_nSurfaceType == 2)	// ��Բ
	{
		bRet = m_pCScanParamView->IsInEllipse(ndata, m_pCScanParamView->m_fLogicalFromX, m_pCScanParamView->m_fLogicalToX, 
			m_pCScanParamView->m_fLogicalFromY, m_pCScanParamView->m_fLogicalToY);
	}
	else if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
	{
		// wangbingfu add start on 2012-11-03
		if(ptArray == NULL)
		{
			MessageBox(_T("CScanView  DataInSelArea(), LOGICAL_POINT_t* ptArray is null"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DataInSelArea"),"Leave");
			return FALSE;
		}

		if ( ::IsBadReadPtr( ptArray, sizeof(LOGICAL_POINT_t) * (m_nPolyNum+1)))
		{
			MessageBox(_T("CScanView  DataInSelArea(), ptArray can not read"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DataInSelArea"),"Leave");
			return FALSE;
		}
		// wangbingfu add end on 2012-11-03
		bRet = m_pCScanParamView->IsInPolygon(ndata, ptArray, m_nPolyNum + 1);				
	}
	else
	{
		;//
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DataInSelArea"),"Leave");
	return bRet;
}
void CCScanView::CalcHanHeColorRate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Enter");
	// ��ȡ������Ϣ
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;

	int nPerCount[21] = {0};	//����ɫ�ĵ���
	int nPerCount2[21] = {0};	//����ɫ�ĵ���	// Song chenguang add on 2012-11-24
	m_nAllCount = 0; // ��Χ�����еĵ���	// machao modify on 2013-09-3
	// machao add start on 2013-09-6
	int *pPerCount;	// �Զ�����ɫ����ɫ����
	int *pPerCount2;
	int nClrStartNum = (int)(27.5 * m_pCScanParamView->m_nSize / 127.5);
	// machao modify start on 2013-11-19
	if (100 > m_pCScanParamView->m_plaPalette[nClrStartNum].fMaxPercent)
	{
		nClrStartNum--;
	}
	int nClrNum = m_pCScanParamView->m_nSize - nClrStartNum;
	// machao modify end on 2013-11-19
	if (m_nColorDefine == 0)
	{
		pPerCount = new int[nClrNum];
		pPerCount2 = new int[nClrNum];
		for (int i = 0; i < nClrNum; i++)
		{
			pPerCount[i] = 0;
			pPerCount2[i] = 0;
		}
	}
	// machao add end on 2013-09-6

	LOGICAL_POINT_t* ptArray = NULL;
	if (m_pCScanParamView->m_nSurfaceType == 3)	// �����
	{
		ptArray = new LOGICAL_POINT_t[m_nPolyNum+1];
		
		// wangbingfu add start on 2012-11-03
		if(ptArray == NULL)
		{
			MessageBox(_T("CScanView  CalcHanHeColorRate(), ptArray new fail"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Leave");
			return;			
		}
		if ( ::IsBadWritePtr( ptArray, sizeof(LOGICAL_POINT_t) * (m_nPolyNum+1)))
		{
			MessageBox(_T("CScanView  CalcHanHeColorRate(), ptArray can not write"), _T("����"), MB_OK);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Leave");
			return;
		}
		// wangbingfu add end on 2012-11-03

		for (int i = 0; i < m_nPolyNum + 1; i++)
		{
			ptArray[i] = Client2Logical(m_Poly[i]);
		}
	}

	LOGICAL_POINT_t ptLogicalFrom;
	LOGICAL_POINT_t ptLogicalTo;
	ptLogicalFrom = Client2Logical(m_ptFrom);
	ptLogicalTo = Client2Logical(m_ptTo);
	if(ptLogicalFrom.x < ptLogicalTo.x)
	{
		m_pCScanParamView->m_fLogicalFromX = ptLogicalFrom.x;
		m_pCScanParamView->m_fLogicalToX = ptLogicalTo.x;
	}
	else
	{
		m_pCScanParamView->m_fLogicalFromX = ptLogicalTo.x;
		m_pCScanParamView->m_fLogicalToX = ptLogicalFrom.x;
	}

	if(ptLogicalFrom.y < ptLogicalTo.y)
	{
		m_pCScanParamView->m_fLogicalFromY = ptLogicalFrom.y;
		m_pCScanParamView->m_fLogicalToY = ptLogicalTo.y;
	}
	else
	{
		m_pCScanParamView->m_fLogicalFromY = ptLogicalTo.y;
		m_pCScanParamView->m_fLogicalToY = ptLogicalFrom.y;
	}
	
	

	int nGateNum[CHANNEL_NUM] = {0};
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		nGateNum[i] = m_nFileGateNum[i];
	}
	int nFeatureNum = m_nFileFeatureNum;

	int nFeatureIndex = 0;	//����ѡ��
	int nMaxAmpIndex = 6;
	int nGateIndexTemp = 0;
	int nFeatureIndexTemp = 0;
	int nMaxAmpIndexTemp = 6;//��󲨷��ֵ����
	

	m_pCScanParamView->UpdateData(TRUE);
	m_fGateThreshold[0] = m_pCScanParamView->m_fGate1Threshold;
	m_fGateThreshold[1] = m_pCScanParamView->m_fGate2Threshold;
	m_fGateThreshold[2] = m_pCScanParamView->m_fGate3Threshold;
	m_fGateThreshold[3] = m_pCScanParamView->m_fGate4Threshold;
	m_nColorMode = m_pCScanParamView->m_nColorMode;
	m_nGateIndex = m_pCScanParamView->m_nGateIndex;
	m_nDrawType = m_pCScanParamView->m_nDrawType;
	m_fBoundMin = m_pCScanParamView->m_fBoundMin;
	m_fBoundMax = m_pCScanParamView->m_fBoundMax;

	float fThick = m_pCScanParamView->m_fThick;	//�������
	int nColorMode = m_pCScanParamView->m_nSurfceColorMode;
	
	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = FALSE;
	for(int i= 1; i < 7; i++)
	{
		bFeatureFlag[i] = m_bFileFeatureFlag[i];
	}

	if(bFeatureFlag[6] == FALSE)
	{
		MessageBox(_T("��󲨷��ֵMP_MAltitudeû�п��������ܽ���ͳ��"), _T("��ʾ"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Leave");
		return;
	}

//	float fGrayScale; //��ɫɫ�ȱ���
//	int nGrayValue;	//��ɫֵ
//	int nColorIndex; //��ɫ����
	
	nGateIndexTemp = m_nGateIndex;
	nFeatureIndexTemp = nFeatureIndex;

	for(int j = 1; j < nFeatureIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nFeatureIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxAmpIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxAmpIndexTemp--;
		}
	}

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		if(m_bGuanLianFlag)	// ����ģʽ
		{
			if(m_nObjectNum == 1)	// ֻ���ײ� ��ͨ���ײ� ˫ͨ���˲�
			{
				if(i == 1 || i == 3 || i == 5 || i == 7)
				{
					continue;
				}
			}
			else if(m_nObjectNum == 2)	// ֻ���˲�
			{
				if(i == 0 || i == 2 || i == 4 || i == 6)
				{
					continue;
				}
			}
			else
			{
				;//
			}
		}

		nGateIndexTemp = m_nGateIndex;

		for(int j = 0; j < m_nGateIndex; j++)
		{
			if(!m_bFileGateFlag[i][j])
			{
				nGateIndexTemp--;
			}
		}

		CFeatureInfo ndata(nGateNum[i], nFeatureNum);//��������
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition(); pos; )
		{
			//��ȡ���ݵ㣬���Զ����е���һ����
			ndata = pDoc->m_lstFlawData[i].GetNext(pos);

			BOOL bRet = DataInSelArea(ndata, ptArray);
			if(!bRet)
				continue;
			
			m_nAllCount++;	// machao modify on 2013-09-3

			// rem by Song chenguang 2012-11-24
//			if(m_pCScanParamView->m_nGateType == 0)
//			{
//				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[nGateIndexTemp])
//				{
					if (m_nColorMode == 0)//ʹ�ûҶ�ͼ��
					{
						float fGrayScale = ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] / 100 * 16;
						if(fGrayScale > 16)
						{
							fGrayScale = 16;
						}
						else if(fGrayScale < 0)
						{
							fGrayScale = 0;
						}

						fGrayScale = 16 - fGrayScale;
						int nColorIndex = (int)fGrayScale;
						nPerCount[nColorIndex]++;

						/* ��Ϊ�����õķ�Χ����Ϊ�Ǻ��õ�

						// ֻ��Ϊ������ֵ���Ǻ��õ�
						if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[nGateIndexTemp])
						nPerCount2[nColorIndex]++;
						*/


						//�������õķ�Χ����Ϊ�Ǻ��õ�  ע��:��CCScanParamPanel���С�100 - ������ 
						if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < m_pCScanParamView->m_nMinAMP 
							|| ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] > m_pCScanParamView->m_nMaxAMP)
						{
							nPerCount2[nColorIndex]++;
						}
					}
					else	// ��ɫ
					{
						if(m_nColorDefine == 1)		// machao add start on 2013-09-6
						{
							//Ĭ����ɫ						
							int nColorIndex = int(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] /(100/20));
							if (nColorIndex > 0)
							{
								nColorIndex -= 1;					
							} 
							if(nColorIndex > 20)
							{
								nColorIndex = 20;
							}
							if(nColorIndex < 0)
							{
								nColorIndex = 0;
							}
							nPerCount[nColorIndex]++;	

							/* ��Ϊ�����õķ�Χ����Ϊ�Ǻ��õ�
							// ֻ��Ϊ������ֵ���Ǻ��õ�
							if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[nGateIndexTemp])
							nPerCount2[nColorIndex]++;
							*/


							//�������õķ�Χ����Ϊ�Ǻ��õ�  ע��:��CCScanParamPanel���С�100 - ������ 
							if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < m_pCScanParamView->m_nMinAMP 
								|| ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] > m_pCScanParamView->m_nMaxAMP)
							{
								nPerCount2[nColorIndex]++;
							}
						}
						// machao add start on 2013-09-6
						else if (m_nColorDefine == 0)
						{
							int nColorIndex = int(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] * nClrNum / 100);
							if (nColorIndex > 0)
							{
								nColorIndex -= 1;					
							} 
							if(nColorIndex > nClrNum - 1)
							{
								nColorIndex = nClrNum - 1;
							}
							if(nColorIndex < 0)
							{
								nColorIndex = 0;
							}
							pPerCount[nColorIndex]++;
							/*
							// machao add start on 2013-10-31
							// ֻ��Ϊ������ֵ���Ǻ��õ�
							if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= m_fGateThreshold[nGateIndexTemp])
							pPerCount2[nColorIndex]++;
							// machao add end on 2013-10-31
							*/


							//�������õķ�Χ����Ϊ�Ǻ��õ�  ע��:��CCScanParamPanel���С�100 - ������ 
							if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < m_pCScanParamView->m_nMinAMP 
								|| ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] > m_pCScanParamView->m_nMaxAMP)
							{
								pPerCount2[nColorIndex]++;
							}
						}
						// machao add end on 2013-09-6
					}
//				}
//			}
			// rem by Song chenguang 2012-11-24
/*			else
			{
				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < m_fGateThreshold[nGateIndexTemp])
				{
					if (m_nColorMode == 0)//ʹ�ûҶ�ͼ��
					{
						float fGrayScale = ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] / 100 * 16;
						if(fGrayScale > 16)
						{
							fGrayScale = 16;
						}
						else if(fGrayScale < 0)
						{
							fGrayScale = 0;
						}

						fGrayScale = 16 - fGrayScale;
						int nColorIndex = (int)fGrayScale;
						nPerCount[nColorIndex]++;
					}
					else	// ��ɫ
					{
						//Ĭ����ɫ						
						int nColorIndex = int(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] /(100/20));
						if (nColorIndex > 0)
						{
							nColorIndex -= 1;					
						} 
						if(nColorIndex > 20)
						{
							nColorIndex = 20;
						}
						if(nColorIndex < 0)
						{
							nColorIndex = 0;
						}
						nPerCount[nColorIndex]++;		
					}
				}
			}
*/
		}
	}

	if (m_pCScanParamView->m_nSurfaceType == 3)
	{
		// wangbingfu modify start on 2012-11-03
		if(ptArray != NULL)
		{
			delete []ptArray;
			ptArray = NULL;
		}
		// wangbingfu modify start on 2012-11-03
	}

	if(m_nAllCount == 0)	// machao modify on 2013-09-3
	{
		MessageBox(_T("���������ݣ����ܽ���ͳ��"), _T("��ʾ"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Leave");
		return;
	}

	// machao modify start on 2013-09-6
	if (m_nColorDefine == 1)
	{
		for(int i = 0; i < 20; i++)
		{
			m_fHanHeRate[19 - i] = (float)nPerCount[i] / m_nAllCount * 123.0f;	// machao modify on 2013-09-3
		}
	}
	else
	{
		m_pfCustomHanheRate = new float[nClrNum];
		for(int i = 0; i < nClrNum; i++)
		{
			m_pfCustomHanheRate[nClrNum - 1 - i] = (float)pPerCount[i] / m_nAllCount * 123.0f;
		}
	}
	// machao modify end on 2013-09-6

	int nFlawCount = 0;
	// machao modify start on 2013-10-31
	if (1 == m_nColorDefine)
	{
		for(int i = 0; i < 20; i++)
		{
			nFlawCount += nPerCount2[i];
		}
	}
	else if (0 == m_nColorDefine)
	{
		for(int i = 0; i < nClrNum; i++)
		{
			nFlawCount += pPerCount2[i];
		}
	}
	// machao modify end on 2013-10-31
	double dbPerAear = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace * 
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2;
	m_dbFlawArea = nFlawCount * dbPerAear;

	// Song Chenguang modified start on 2012-11-24
	CalcSelectArea(ptLogicalFrom, ptLogicalTo);
	double fAllArea = m_nAllCount * dbPerAear;	// machao modify on 2013-09-3
	if (m_SelArea > fAllArea)
		m_SelArea = fAllArea;
	// Song Chenguang modified end on 2012-11-24


	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalcHanHeColorRate"),"Leave");
}
BOOL CCScanView::PreTranslateMessage(MSG* pMsg)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PreTranslateMessage"),"Enter");
	// TODO: Add your specialized code here and/or call the base class
	if (m_ToolTip.m_hWnd != NULL)
	{
		m_ToolTip.RelayEvent(pMsg);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::PreTranslateMessage"),"Leave");
	return CScrollView::PreTranslateMessage(pMsg);
}

//�����������   yachang.huang add start on 2012-6-1
void CCScanView::CalculatePolygonArea()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalculatePolygonArea"),"Enter");
	LOGICAL_POINT_t pBegin;
    LOGICAL_POINT_t ptemp1;
    LOGICAL_POINT_t ptemp2;

	pBegin = Client2Logical(m_Poly[0]);
	
	float x1 = pBegin.x;
	float y1 = pBegin.y;
	float x2 = 0.0;
	float y2 = 0.0;
	float x3 = 0.0;
	float y3 = 0.0;
	float area = 0.0;
	m_fPolygonArea = 0.0f;

	for(int i = 1;i < m_nPolyNum;i++)
	{
		ptemp1 = Client2Logical(m_Poly[i]);
		ptemp2 = Client2Logical(m_Poly[i+1]);
		x2 = ptemp1.x;
		y2 = ptemp1.y;
		x3 = ptemp2.x;
		y3 = ptemp2.y;
		area = (x2*y3 - x1*y3 + x1*y2-x3*y2+x3*y1-x2*y1);
		m_fPolygonArea += area;
	}
    m_fPolygonArea = fabs(m_fPolygonArea);
	m_fPolygonArea = m_fPolygonArea/2;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::CalculatePolygonArea"),"Leave");
}
void CCScanView::DrawProbePosition(double dX, double dY, double dZ, double dW)//?������
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawProbePosition"),"Enter");
	if(!g_bDrawRealTime)
	{
		if(m_nAxisSelect == 3
			|| m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
		{
			if(m_nPipeXMode == 1) // �ܳ� //?
			{
				dW = dW * (PI * m_dPipeDiameter / 360);
			}
		}

		if(m_pCScanParamView->m_bDisplayProbe)
		{
			LOGICAL_POINT_t stProbePoint;
			if(m_nAxisSelect == 0)	// XY
			{
				stProbePoint.x = static_cast<float>(dX);
				stProbePoint.y = static_cast<float>(dY);
			}
			else if(m_nAxisSelect == 1)	// XZ
			{
				stProbePoint.x = static_cast<float>(dX);
				stProbePoint.y = static_cast<float>(dZ);
			}
			else if(m_nAxisSelect == 2)	// YZ
			{
				stProbePoint.x = static_cast<float>(dY);
				stProbePoint.y = static_cast<float>(dZ);
			}
			else if(m_nAxisSelect == 3
				|| m_nAxisSelect == 4)	// W wangbingfu add on 2013-01-09
			{
				stProbePoint.x = static_cast<float>(dX);
				stProbePoint.y = static_cast<float>(dW);
			}
			else
			{
				;//
			}

			CPoint point = Logical2Client(stProbePoint);
			if((point.x >= m_rectGrid.left
				&& point.x <= m_rectGrid.right)
				|| (point.y >= m_rectGrid.top
				&& point.y <= m_rectGrid.bottom))
			{
				m_bEnterFlag = TRUE;
			}
			else
			{
				m_bEnterFlag = FALSE;
			}

			CDC* pDC = GetDC();
			// wangbingfu add start on 2012-11-03
			if(pDC == NULL)
			{
				MessageBox(_T("CScanView  OnMouseLeave(), pDC is null"), _T("����"), MB_OK);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::DrawProbePosition"),"Leave");
				return;
			}
			// wangbingfu add end on 2012-11-03

			if(m_ptProbeCurPos != point)
			{
				if(m_bEnterFlag)
				{
					InvertRegion(pDC, m_ptProbeCurPos, 0, eLINE);
					//	InvertDashLine(pDC, m_ptProbeCurPos);
				}

				InvertRegion(pDC, point, 0, eLINE);
				//InvertDashLine(pDC, point);
				m_ptProbeCurPos = point;
			}

			ReleaseDC(pDC);
		}	
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::DrawProbePosition"),"Leave");
}
void CCScanView::OnTimer(UINT_PTR nIDEvent)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnTimer"),"Enter");
	// TODO: Add your message handler code here and/or call default
	DrawProbePosition(m_dX, m_dY, m_dZ, m_dR);

	CScrollView::OnTimer(nIDEvent);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnTimer"),"Leave");
}

CPoint CCScanView::Logical2MemDC(LOGICAL_POINT_t ptLogical)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Logical2MemDC"),"Enter");
	CPoint ptMemDC;
	float fXConvertValue = 1 / m_fGridX;
	float fYConvertValue = 1 / m_fGridY;

//	// rem by Song Chenguang on 2013-07-15
	if(m_nAxisSelect == 3
		|| m_nAxisSelect == 4)	// �ܲ� wangbingfu add on 2013-01-09
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			ptLogical.y = static_cast<float>(ptLogical.y * PI * m_dPipeDiameter / 360.0f);//?
		}
	}
	// Song Chenguang add start on 2013-05-04
	// machao modify start on 2013-11-24
/*	else if (m_nAxisSelect == 5)
	{
		if(m_nPipeXMode == 0) // �Ƕ�
		{
			ptLogical.x = static_cast<float>(ptLogical.x * (PI * m_dPipeDiameter) / 360.0f);//?
		}
	}
*/	// machao modify start on 2013-11-24
	// Song Chenguang add start on 2013-05-04

	ptMemDC.x = (int)((ptLogical.x - m_dXStart) * fXConvertValue);
	ptMemDC.y = -(int)((ptLogical.y - m_dYStart) * fYConvertValue);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::Logical2MemDC"),"Leave");
	return ptMemDC;
}

void CCScanView::OnNoteEdit()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnNoteEdit"),"Enter");
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	LOGICAL_POINT_t ptLogicalFrom = Client2Logical(m_ptFrom);
	LOGICAL_POINT_t ptLogicalTo = Client2Logical(m_ptTo);
	LOGICAL_POINT_t stLgPtCurrent = Client2Logical(m_ptNoteCurrent);
	CPoint ptMemDCFrom = Logical2MemDC(ptLogicalFrom);
	CPoint ptMemDCTo = Logical2MemDC(ptLogicalTo);
	CPoint ptMemDCCur = Logical2MemDC(stLgPtCurrent);
	CPoint ptMemDC[30];
	ptMemDC[0] = ptMemDCFrom;
	ptMemDC[1] = ptMemDCTo;
	
	int nResult = Note_ShowEditDlg(this, m_pCScanParamView->m_nNoteType, ptMemDC, 2, ptMemDCCur, m_strNotePath, m_nObjectNum);
	if(nResult == IDOK)
	{				
//		m_MemDC.RestoreDC(m_nSavedDC); // note
//		m_nSavedDC = m_MemDC.SaveDC();
		for(int i = 0; i < m_nMemXLENGTH; i++)
		{
			for(int j = 0; j < m_nMemYLENGTH; j++)
			{
				if(m_pPixelCount[i][j] != 0)
				{
					if(m_pCScanParamView->m_nGateType == 0)
					{
						if(m_pPixel[i][j] >= m_fGateThreshold[m_nGateIndex])
						{
							draw2DC(&m_MemDC, m_pPixel[i][j], i, j, 0, 0);
						}
						else
						{
							draw2DC(&m_MemDC, -1, i, j, 0, 0);
						}
					}
					else
					{
						if(m_pPixel[i][j] < m_fGateThreshold[m_nGateIndex])
						{
							draw2DC(&m_MemDC, m_pPixel[i][j], i, j, 0, 0);
						}
						else
						{
							draw2DC(&m_MemDC, -1, i, j, 0, 0);
						}
					}
				}		
			}
		}

		Note_DrawAllNote(&m_MemDC, FALSE, m_nObjectNum);
		
		m_bNotEditFlag = TRUE;
		Invalidate(TRUE);

		m_ptFrom = CPoint(0, 0);
		m_ptTo = CPoint(0, 0);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnNoteEdit"),"Leave");
}

void CCScanView::OnNoteDelete()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnNoteDelete"),"Enter");
	// TODO: Add your command handler code here
	m_MemDC.SetViewportOrg(0, 0);
	m_MemDC.FillSolidRect(0, 0, m_CScanSize.cx, m_CScanSize.cy, m_cscan_back_color);
//	m_MemDC.RestoreDC(m_nSavedDC); // note
//	m_nSavedDC = m_MemDC.SaveDC();
	for(int i = 0; i < m_nMemXLENGTH; i++)
	{
		for(int j = 0; j < m_nMemYLENGTH; j++)
		{
			if(m_pPixelCount[i][j] != 0)
			{
				if(m_pCScanParamView->m_nGateType == 0)
				{
					if(m_pPixel[i][j] >= m_fGateThreshold[m_nGateIndex])
					{
						draw2DC(&m_MemDC, m_pPixel[i][j], i, j, 0, 0);
					}
					else
					{
						draw2DC(&m_MemDC, -1, i, j, 0, 0);
					}
				}
				else
				{
					if(m_pPixel[i][j] < m_fGateThreshold[m_nGateIndex])
					{
						draw2DC(&m_MemDC, m_pPixel[i][j], i, j, 0, 0);
					}
					else
					{
						draw2DC(&m_MemDC, -1, i, j, 0, 0);
					}
				}
			}		
		}
	}

	LOGICAL_POINT_t stLgPtCurrent = Client2Logical(m_ptNoteCurrent);
	CPoint ptMemDCCur = Logical2MemDC(stLgPtCurrent);

	Note_DeleteSelectNote(ptMemDCCur, m_nObjectNum);
	Note_DrawAllNote(&m_MemDC, FALSE, m_nObjectNum);
	Invalidate(TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnNoteDelete"),"Leave");
}
//yachang.huang add on 2012-8-14   ���Oncreate��Ϣ������
int CCScanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCreate"),"Enter");
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanView::OnCreate"),"Leave");	
		return -1;
	}
	// TODO:  �ڴ������ר�õĴ�������
	m_pHRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pHRuler->SetStyle( RWSTYLE_CHORZ | RWSTYLE_TOPALIGN | RWSTYLE_DRAWEDGE );
	m_pHRuler->Create(WS_VISIBLE ,m_rectScaleX,this,WM_USER+104);
	m_pHRuler->ShowWindow(WM_USER+104);

	m_pVRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pVRuler->SetStyle( RWSTYLE_CVERT | RWSTYLE_RIGHTALIGN | RWSTYLE_DRAWMODAL);
	m_pVRuler->Create(WS_VISIBLE,m_rectScaleY,this,WM_USER+103);
	m_pVRuler->Invalidate();
	m_pVRuler->ShowWindow(WM_USER+103);

	if(m_nObjectNum == 1)
	{
		m_pHRuler->m_nObjectCount = 1;
		m_pVRuler->m_nObjectCount = 1;

	}
	else
	{
		m_pHRuler->m_nObjectCount = 2;
		m_pVRuler->m_nObjectCount = 2;

	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCreate"),"Leave");
	return 0;
}


//yachang.huang add on 2012-8-16   ���OnUpdateIDMROTATE��Ϣ������
void CCScanView::OnUpdateIDMROTATE(CCmdUI* pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateIDMROTATE"),"Enter");
	pCmdUI->SetCheck(m_bRota);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnUpdateIDMROTATE"),"Leave");
}

// wangbingfu add start on 2013-01-09
/*
void CCScanView::ReadPipeIni(const CString& csFilePath)
{
	// 	TCHAR strFolder[MAX_PATH];
	// 	GetWorkingFolder(strFolder, MAX_PATH);
	// 	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//ȡ�������ļ�·��

	CString strSection;
	CString strKey;

	int nValue = 0;
	strSection = _T("OtherParams");
	//	strKey = _T("AxisSelect");
	//	int nAxisSelect = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);

	//	if(nAxisSelect == 4)
	{
		//		m_nAxisSelect = nAxisSelect;
		strKey = _T("Angle");
		int nAngle = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		m_fPipeProbeAngle = (float)nAngle;

		int nPipeChannelSelect[CHANNEL_NUM];
		memset(nPipeChannelSelect, 0, sizeof(nPipeChannelSelect));

		// 0=��,1=A���,2=A�ֲ�,3=A���+�ֲ�,4=����B1,5=����B2,6=����C1,7=����C2
		strSection = _T("GroupSelect");
		strKey = _T("G11");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[0] = nValue;
		strKey = _T("G12");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[1] = nValue;
		strKey = _T("G21");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[2] = nValue;
		strKey = _T("G22");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[3] = nValue;
		strKey = _T("G31");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[4] = nValue;
		strKey = _T("G32");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[5] = nValue;
		strKey = _T("G41");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[6] = nValue;
		strKey = _T("G42");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[7] = nValue;

		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
			m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
		}

		// 0=ͨ��1,1=ͨ��2,2=ͨ��3,3=ͨ��4,4=ͨ��5,5=ͨ��6,6=ͨ��7,7=ͨ��8
		strSection = _T("CScanSelect");
		if(m_nObjectNum == 1)
		{
			strKey = _T("C1");
		}
		else if(m_nObjectNum == 2)
		{
			strKey = _T("C2");
		}
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		m_nCScanSelect = nValue;
	}
}

void CCScanView::WritePipeIni(const CString& csFilePath)
{
	CString strKey;
	CString strValue;
	CString cstr = _T("");
	CString strSection = _T("");

	// 	TCHAR strFolder[MAX_PATH];
	// 	GetWorkingFolder(strFolder, MAX_PATH);
	// 	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//ȡ�������ļ�·��
	//����ļ�Ϊֻ���ģ���ֻ������ȥ��
	SetFileAttributes(csFilePath,GetFileAttributes(csFilePath) & ~FILE_ATTRIBUTE_READONLY);

	strSection = _T("OtherParams");
	// 	strKey = _T("AxisSelect");
	// 	strValue = _T("");
	// 	strValue.Format(_T("%d"),m_nAxisSelect);
	// 	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("Angle");
	strValue = _T("");
	strValue.Format(_T("%d"),m_fPipeProbeAngle);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);

	strSection = _T("GroupSelect");
	strKey = _T("G11");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[0]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G12");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[1]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G21");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[2]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G22");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[3]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G31");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[4]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G32");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[5]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G41");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[6]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G42");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[7]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);

	strSection = _T("CScanSelect");
	strKey = _T("C1");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nCScanSelect[0]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strSection = _T("CScanSelect");
	strKey = _T("C2");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nCScanSelect[1]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);	
}*/
int CCScanView::GetPipeGuanLianCard()
{
	int i = 0;
	if(m_nAxisSelect == 4) // W1��
	{
		for(; i < CHANNEL_NUM; i++)
		{
			if((i % 2) == 0)	// ����1ͨ��
			{
				if(m_nPipeChannelSelect[i+1] == 3) // ����2ͨ������ѡ��A���+�ֲ�ʱ��C1�����C2���˲�(�ֲ�)
				{
					return i;
				}
			}
		}
		return -1;
	}
	else
	{
		return -1;
	}
}
// wangbingfu add end on 2013-01-09

void CCScanView::OnSelSurfaceCenter()
{
	// TODO: Add your command handler code here
	m_bSelSurfaceCenter = TRUE;
}

void CCScanView::OnSelSurfaceEdge()
{
	// TODO: Add your command handler code here
	m_bSelSurfaceEdge = TRUE;
}

void CCScanView::OnCustumSurfaceDel()
{
	// TODO: Add your command handler code here
	POSITION pos = m_lstCustumSurfaceRegion.GetHeadPosition();
	POSITION pos2 = m_lstCustumFlawBufferSize.GetHeadPosition();	// machao add on 2013-09-17

	POSITION posDel = NULL;
	POSITION posTemp = NULL;
	int nIndex=0;
	long lSize = 0;	//machao add on 2013-09-17
	while (pos)
	{
		posTemp = pos;
		LOGICAL_REGION region = m_lstCustumSurfaceRegion.GetNext(pos);
		if (m_stCurPoint.x > region.m_stFrom.x && m_stCurPoint.x < region.m_stTo.x &&
			m_stCurPoint.y < region.m_stFrom.y && m_stCurPoint.y > region.m_stTo.y) 
		{
			posDel = posTemp;
			break;
		}
		// machao add start on 2013-09-17
		if (m_lstCustumFlawBufferSize.GetCount() != 0)
		{
			lSize += m_lstCustumFlawBufferSize.GetAt(pos2);
			m_lstCustumFlawBufferSize.GetNext(pos2);
		}
		// machao add end on 2013-09-17
		nIndex++;
	}

	// machao add start on 2013-09-17
	if (m_lstCustumFlawBufferSize.GetCount() != 0)
	{
		long lCurrent = m_lstCustumFlawBufferSize.GetAt(pos2);
		char* pTemp = new char[m_lCustumFlawBufferSize - lCurrent];
		memcpy(pTemp, m_pcCustumFlawBuffer, lSize);
		memcpy(pTemp + lSize, m_pcCustumFlawBuffer + lSize + lCurrent, m_lCustumFlawBufferSize - lSize - lCurrent);
		delete []m_pcCustumFlawBuffer;
		m_pcCustumFlawBuffer = pTemp;
		m_lCustumFlawBufferSize = m_lCustumFlawBufferSize - lCurrent;
		m_lstCustumFlawBufferSize.RemoveAt(pos2);
	}
	// machao add end on 2013-09-17

	m_lstCustumSurfaceRegion.RemoveAt(posDel);
	m_pCScanParamView->m_lstctrlCustumFlaw.DeleteItem(nIndex);

	int nCount = m_pCScanParamView->m_lstctrlCustumFlaw.GetItemCount();
	for (int i = nIndex; i<nCount; i++)
	{
		CString str;
		str.Format(_T("%d"), i+1);
		m_pCScanParamView->m_lstctrlCustumFlaw.SetItemText(i, 0, str);
	}

	// ˢ����ͼ
	DrawPicture(&m_MemDC);
	Invalidate(TRUE);
}

void CCScanView::OnUpdateSelSurfaceEdge(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bSurfaceCenterSelected)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
// Song Chenguang add start on 2013-07-11
#if _HAFEI
// �����϶��
float CCScanView::CalcKongXiLv(const float& fdB, const float& fThickness)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	float N = 0.0f;
	float fKongXiLv = 0.0f;
	if (fThickness >= pFrame->m_f11LayerMin && fThickness <= pFrame->m_f11LayerMax)	// 11��
	{
		N = 1.5f * 2 + 9;
	}
	else if (fThickness >= pFrame->m_f14LayerMin && fThickness <= pFrame->m_f14LayerMax)	// 14�� 
	{
		N = 1.5f * 2 + 12;
	}
	else if (fThickness >= pFrame->m_f17LayerMin && fThickness <= pFrame->m_f17LayerMax)	// 17��
	{
		N = 1.5f * 2 + 15;
	}
	else if (fThickness >= pFrame->m_f18LayerMin && fThickness <= pFrame->m_f18LayerMax)	// 18��
	{
		N = 1.5f * 3 + 15;
	}
	else
	{
	}

	float fXiShu = fdB / (40 * (N - 1));
	fKongXiLv = (1 - powf(10, fXiShu)) * 100.0f;
	return fKongXiLv;
}
#endif
// Song Chenguang add end on 2013-07-11
//Machao add start on 2013-07-03
int CCScanView::GetColorNumber(float fValue, float fBoundMin, float fBoundMax)
{
//	fValue = (fValue - m_fBoundMin) * 127.5 / (m_fBoundMax-m_fBoundMin);
	fValue = (fValue - fBoundMin) * 100 / (fBoundMax - fBoundMin);
	if (100 >= fValue && 0 < fValue)
	{
		int nLeft = 0;
		int nRight = m_pCScanParamView->m_nSize - 1;
		int nMiddle;
		while(nLeft <= nRight)
		{
			nMiddle = (nLeft +nRight) / 2;
			if (m_pCScanParamView->m_plaPalette[nMiddle].fMaxPercent < fValue)
			{
				nRight = nMiddle - 1;
			}
			else if (m_pCScanParamView->m_plaPalette[nMiddle].fMinPercent >= fValue)
			{
				nLeft = nMiddle + 1;
			}
			else if (m_pCScanParamView->m_plaPalette[nMiddle].fMaxPercent >= fValue && m_pCScanParamView->m_plaPalette[nMiddle].fMinPercent < fValue)
			{
				return nMiddle;
			}
		}
	}
	else if (fValue > 100)
	{
		// machao modify start on 2013-11-19
		int nClrStartNum = (int)(27.5 * m_pCScanParamView->m_nSize / 127.5);
		if (100 > m_pCScanParamView->m_plaPalette[nClrStartNum].fMinPercent)
		{
			nClrStartNum--;
		}
		return nClrStartNum;		// machao modify on 2013-9-2
		// machao modify end on 2013-11-19
	}
	else if (fValue <= 0.0)
	{
		return m_pCScanParamView->m_nSize;
	}
}
//Machao add end on 2013-07-3
//Machao add start on 2013-07-9
void CCScanView::OnNoiseReduction()
{
	// TODO: Add your command handler code here
	CDropFaultDlg drop;
	array<float>^ SrcData = gcnew array<float>(m_nMemXLENGTH * m_nMemYLENGTH);
	array<float>^ SrcData1 = gcnew array<float>(m_nMemXLENGTH * m_nMemYLENGTH);
	array<int>^ ColorNums = gcnew array<int>(m_pCScanParamView->m_vnMainColorNum.size());
	for (int i = 0; i < m_nMemXLENGTH; i++)
	{
		for (int j = 0; j < m_nMemYLENGTH; j++)
		{
			// machao modify start on 2013-08-21
//			SrcData[i * m_nMemYLENGTH + j] = m_pfSrcData[i][j];
			SrcData[i * m_nMemYLENGTH + j] = m_pPixel[i][j];
			// machao modify end on 2013-08-21
		}
	}
	for (int i = 0; i < m_pCScanParamView->m_vnMainColorNum.size(); i++)
	{
		ColorNums[i] = m_pCScanParamView->m_vnMainColorNum[i];
	}
	if (drop.DoModal() == IDOK)
	{
		m_bNoiseReduction = TRUE;
		DFClass ^df=gcnew DFClass();
		SrcData1 = df->DFFirst(SrcData,m_nMemXLENGTH * m_fGridX,m_nMemYLENGTH * m_fGridY,
			m_fGridX,m_fGridY,ColorNums,drop.m_nJZNum,drop.m_nTargetRange,drop.m_nCZJJ);
		if (SrcData1->Length == 1)
		{
			return;
		}
		for (int i = 0; i < m_nMemXLENGTH; i++)
		{
			for (int j = 0; j < m_nMemYLENGTH; j++)
			{
				m_pfSrcData[i][j] = SrcData1[i * m_nMemYLENGTH + j];
			}
		}
		m_pCScanParamView->OnBnClickedButtonDrawPicture();
	}
}
//Machao add end on 2013-07-9
//machao add start on 2013-07-22
void CCScanView::OnUpdateNoiseReduction(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bImage)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}
//machao add end on 2013-07-22
#if _HAFEI
// machao add start on 2013-08-8
void CCScanView::DrawTailDecreasePipeUnfoldMap(CDC *pDC)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	int nOldMode = pDc->SetROP2(R2_NOT);
	CBrush brushDashed;
	CBrush brushSolidLine;
	CPen penDashed;
	CPen penSolodLine;
	float nLongNums = pFrame->m_nASketch + pFrame->m_nABSketch + pFrame->m_nBSketch + pFrame->m_nBCSketch + pFrame->m_nCSketch + pFrame->m_nCDSketch + pFrame->m_nDSketch;
	penDashed.CreatePen(PS_DASHDOT,1,RGB(0,0,0));
	brushDashed.CreateStockObject(NULL_BRUSH);
	CPen* pPenDashed = pDC->SelectObject(&penDashed);
	pDC->SelectObject(&penDashed);
	pDC->SelectObject(&brushDashed);
 	pDC->Rectangle(CRect(CoordinateTransform(0, g_fUnfoldHeight), CoordinateTransform(nLongNums, 0)));

	pDC->MoveTo(CoordinateTransform(0, g_fUnfoldHeight / 2));
	pDC->LineTo(CoordinateTransform(nLongNums,g_fUnfoldHeight / 2));
	pDC->MoveTo(CoordinateTransform(pFrame->m_nLeft1Stop, 0));
	pDC->LineTo(CoordinateTransform(pFrame->m_nLeft1Start, g_fUnfoldHeight));
	pDC->MoveTo(CoordinateTransform(pFrame->m_nLeft2Stop, g_fUnfoldHeight / 2));
	pDC->LineTo(CoordinateTransform(pFrame->m_nLeft2Start, g_fUnfoldHeight));
	pDC->MoveTo(CoordinateTransform(pFrame->m_nRight2Stop, g_fUnfoldHeight / 2));
	pDC->LineTo(CoordinateTransform(pFrame->m_nRight2Start, g_fUnfoldHeight));
	pDC->MoveTo(CoordinateTransform(pFrame->m_nRight1Stop, 0));
	pDC->LineTo(CoordinateTransform(pFrame->m_nRight1Start, g_fUnfoldHeight));
	pDC->MoveTo(CoordinateTransform((pFrame->m_nLeft1Start + 3 * pFrame->m_nLeft1Stop) / 4, g_fUnfoldHeight / 4));
	pDC->LineTo(CoordinateTransform((pFrame->m_nRight1Start + 3 * pFrame->m_nRight1Stop) / 4, g_fUnfoldHeight / 4));

	penSolodLine.CreatePen(PS_SOLID,1,RGB(0,0,0));
	brushSolidLine.CreateStockObject(NULL_BRUSH);
	CPen* pPen = pDC->SelectObject(&penSolodLine);
	pDC->SelectObject(&penSolodLine);
	pDC->SelectObject(&brushSolidLine);
	pDC->Rectangle(CRect(CoordinateTransform(pFrame->m_nASketch, g_fUnfoldHeight), CoordinateTransform(pFrame->m_nASketch + pFrame->m_nABSketch,0)));
	pDC->Rectangle(CRect(CoordinateTransform(pFrame->m_nASketch + pFrame->m_nABSketch + pFrame->m_nBSketch,g_fUnfoldHeight),CoordinateTransform(pFrame->m_nASketch + pFrame->m_nABSketch + pFrame->m_nBSketch + pFrame->m_nBCSketch,0)));
	pDC->Rectangle(CRect(CoordinateTransform(nLongNums - pFrame->m_nDSketch - pFrame->m_nCDSketch,g_fUnfoldHeight),CoordinateTransform(nLongNums - pFrame->m_nDSketch,0)));
	// machao add start on 2013-08-22
	brushDashed.DeleteObject();
	brushSolidLine.DeleteObject();
	penDashed.DeleteObject();
	penSolodLine.DeleteObject();
	// machao add start on 2013-08-22
//	pDc->SetROP2(nOldMode);
}
// machao add end on 2013-08-8

// machao add start on 2013-08-12
void CCScanView::OnTailDecreasePipeUnfold()
{
	// TODO: Add your command handler code here
	m_bTailDecreasePipe = !m_bTailDecreasePipe;
	Invalidate(TRUE);
}

void CCScanView::OnUpdateTailDecreasePipeUnfold(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bTailDecreasePipe)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
}
// machao add end on 2013-08-12
#endif
// machao add start on 2013-08-9
CPoint CCScanView::CoordinateTransform(float fPointX, float fPointY)
{
	LOGICAL_POINT_t logical;
	logical.x = fPointX + (float)m_dXStart;
	logical.y = fPointY;
	return Logical2Client(logical);
}
// machao add end on 2013-08-9
// machao add start on 2013-08-15
void CCScanView::OnPartGainCompensateSite()
{
	// TODO: Add your command handler code here
	if (m_pPartGainCompensateDlg == NULL)
	{
		m_pPartGainCompensateDlg = new CPartGainCompensateDlg;
		m_pPartGainCompensateDlg->Create(IDD_DIALOG_PART_GAIN_COMPENSATE, this);
		m_pPartGainCompensateDlg->ShowWindow(SW_SHOW);
		m_bCompensateRange = TRUE;
	}
	else
	{
		if(m_pPartGainCompensateDlg->GetSafeHwnd() != NULL)
			m_pPartGainCompensateDlg->ShowWindow(SW_SHOW);
		else
		{
			m_pPartGainCompensateDlg->Create(IDD_DIALOG_PART_GAIN_COMPENSATE, this);
			m_pPartGainCompensateDlg->ShowWindow(SW_SHOW);
		}
	}
}

void CCScanView::DrawPartGainCompensateRange(CDC *pDC, float fWStart, float fZStart, float fWStop, float fZStop)
{
	if (!m_bMouseGainRangeSelect)
	{
		m_fWStart = m_pPartGainCompensateDlg->m_fWStart;
		m_fWStop = m_pPartGainCompensateDlg->m_fWStop;
		m_fZStart = m_pPartGainCompensateDlg->m_fZStart;
		m_fZStop = m_pPartGainCompensateDlg->m_fZStop;
	}
	else
	{
		m_fWStart = fWStart;
		m_fZStart = fZStart;
		m_fWStop = fWStop;
		m_fZStop = fZStop;
	}
	int nOldMode = pDC->SetROP2(R2_NOT);
 	CBrush brush;
 	brush.CreateStockObject(NULL_BRUSH);
 	pDC->SelectObject(&brush);
	CPoint ptLeftTop = CoordinateTransform(m_fWStart - (float)m_dXStart,m_fZStart);
	CPoint ptRightBottom = CoordinateTransform(m_fWStop - (float)m_dXStart,m_fZStop);
	int nLeft = ptLeftTop.x < ptRightBottom.x ? ptLeftTop.x : ptRightBottom.x;
	int nRight = ptLeftTop.x < ptRightBottom.x ? ptRightBottom.x : ptLeftTop.x;
	int nTop = ptLeftTop.y < ptRightBottom.y ? ptLeftTop.y : ptRightBottom.y;
	int nBottom = ptLeftTop.y < ptRightBottom.y ? ptRightBottom.y : ptLeftTop.y;

	pDC->Rectangle(CRect(nLeft, nTop, nRight, nBottom));

	pDC->SetROP2(nOldMode);
	brush.DeleteObject();
}
// machao add end on 2013-08-15
// machao add start on 2013-08-19
void CCScanView::DrawDBCompensatesite()
{
	for (int i = 0; i < m_nMemXLENGTH; i++)
	{
		for (int j = 0; j < m_nMemYLENGTH; j++)
		{
			m_pfSrcData[i][j] = m_pPixel[i][j];
			CPoint ptLeftTop = Logical2MemDC(m_fWStart,m_fZStart);
			CPoint ptRightBottom = Logical2MemDC(m_fWStop,m_fZStop);
			int nLeft = ptLeftTop.x < ptRightBottom.x ? ptLeftTop.x : ptRightBottom.x;
			int nRight = ptLeftTop.x < ptRightBottom.x ? ptRightBottom.x : ptLeftTop.x;
			int nTop = ptLeftTop.y < ptRightBottom.y ? ptLeftTop.y : ptRightBottom.y;
			int nBottom = ptLeftTop.y < ptRightBottom.y ? ptRightBottom.y : ptLeftTop.y;
			if (i >= nLeft && i < nRight && j > 0 - nBottom && j <= 0 - nTop)
			{
				m_pfSrcData[i][j] = m_pfSrcData[i][j] / (powf(10, m_pPartGainCompensateDlg->m_fPreviousdBValue / 20));
				m_pfSrcData[i][j] = (powf(10, m_pPartGainCompensateDlg->m_fdBValue / 20)) * m_pfSrcData[i][j];
			}
		}
	}
	m_pPartGainCompensateDlg->m_fPreviousdBValue = m_pPartGainCompensateDlg->m_fdBValue;
	m_bNoiseReduction = TRUE;
	m_pCScanParamView->OnBnClickedButtonDrawPicture();
}
// machao add end on 2013-08-19
// machao add start on 2013-08-16
LRESULT CCScanView::OnDialogDestroyed(WPARAM wParam, LPARAM lParam)
{
	m_fWStart = 0;
	m_fWStop = 0;
	m_fZStart = 0;
	m_fZStop = 0;
	m_bMouseGainRangeSelect = FALSE;
	m_bCompensateRange = FALSE;
	m_pPartGainCompensateDlg = NULL;
	return 0;
}
// machao add end on 2013-08-16
// machao add start on 2013-08-20
CPoint CCScanView::Logical2MemDC(float fPointX, float fPointY)
{
	LOGICAL_POINT_t logical;
	logical.x = fPointX;
	logical.y = fPointY;
	return Logical2MemDC(logical);
}
// machao add start on 2013-08-20
