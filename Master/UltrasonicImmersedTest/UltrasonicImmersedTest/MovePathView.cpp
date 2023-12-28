//MovePathView.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 扫查架运动轨迹视图源文件
*Filename : MovePathView.cpp
*
*@author       songchenguang zhaoli
*@version      1.0         Date: 2011-7-10
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MovePathView.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "BScanView.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"

extern BOOL g_bDrawRealTime; //实时成像
// CMovePathView

IMPLEMENT_DYNCREATE(CMovePathView, CFormView)

CMovePathView::CMovePathView()
	: CFormView(CMovePathView::IDD)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::CMovePathView"),"Enter");

	m_nPanelIndex = 0;
	m_ParentWnd = NULL;
	m_bULDPtp = FALSE;
	m_bESC = FALSE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::CMovePathView"),"Leave");
}

CMovePathView::~CMovePathView()
{
}

void CMovePathView::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::DoDataExchange"),"Enter");

	CFormView::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_GRAPHMOVECTRL, m_GraphMove);
	DDX_Control(pDX, IDC_MDSOCX, m_GraphMove);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::DoDataExchange"),"Leave");
}

BEGIN_MESSAGE_MAP(CMovePathView, CFormView)
	ON_COMMAND(ID_EDIT_UNDO, &CMovePathView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMovePathView::OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CMovePathView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, &CMovePathView::OnEditFind)
	ON_COMMAND(ID_EDIT_SHAI, &CMovePathView::OnEditShai)
	ON_COMMAND(ID_EDIT_CUT, &CMovePathView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CMovePathView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMovePathView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, &CMovePathView::OnEditClear)
	ON_COMMAND(ID_EDIT_FAN, &CMovePathView::OnEditFan)
	ON_COMMAND(ID_EDIT_MIRROR, &CMovePathView::OnEditMirror)
	ON_COMMAND(ID_EDIT_ROTA, &CMovePathView::OnEditRota)
	ON_COMMAND(ID_DRAW_DAO, &CMovePathView::OnDrawDao)
	ON_COMMAND(ID_DRAW_SELECT, &CMovePathView::OnDrawSelect)
	ON_COMMAND(ID_DRAW_HOLE, &CMovePathView::OnDrawHole)
	ON_COMMAND(ID_DRAW_LINE, &CMovePathView::OnDrawLine)
	ON_COMMAND(ID_DRAW_ARC, &CMovePathView::OnDrawArc)
	ON_COMMAND(ID_DRAW_ARC_CENETR, &CMovePathView::OnDrawArcCenetr)
	ON_COMMAND(ID_DRAW_CIRCLE, &CMovePathView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CIRCLE_SAN, &CMovePathView::OnDrawCircleSan)
	ON_COMMAND(ID_DRAW_ARCLINE, &CMovePathView::OnDrawArcline)
	ON_COMMAND(ID_DRAW_RECT, &CMovePathView::OnDrawRect)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CMovePathView::OnDrawEllipse)
	ON_COMMAND(ID_DRAW_POLYGON, &CMovePathView::OnDrawPolygon)
	ON_COMMAND(ID_DRAW_RUNWAY, &CMovePathView::OnDrawRunway)
	ON_COMMAND(ID_TOOL_CAN, &CMovePathView::OnToolCan)
	ON_COMMAND(ID_TOOL_COLOR, &CMovePathView::OnToolColor)
	ON_COMMAND(ID_TOOL_JXIAN, &CMovePathView::OnToolJxian)
	ON_COMMAND(ID_TOOL_JPIAN, &CMovePathView::OnToolJpian)
	ON_COMMAND(ID_TOOL_EDIT, &CMovePathView::OnToolEdit)
	ON_COMMAND(ID_TOOL_RESET, &CMovePathView::OnToolReset)
	ON_COMMAND(ID_TOOL_RUN, &CMovePathView::OnToolRun)
	ON_COMMAND(ID_VIEW_SHI, &CMovePathView::OnViewShi)
	ON_COMMAND(ID_VIEW_TU, &CMovePathView::OnViewTu)
	ON_COMMAND(ID_VIEW_WINDOW, &CMovePathView::OnViewWindow)
	ON_WM_SIZE()
	ON_COMMAND(ID_DECOMPOSE, &CMovePathView::OnDecompose)
	ON_COMMAND(ID_EMD_FILE_OPEN, &CMovePathView::OnFileOpen)
	ON_COMMAND(ID_EMD_FILE_SAVE, &CMovePathView::OnFileSave)
	ON_COMMAND(ID_EMD_NEW_FILE, &CMovePathView::OnFileNew)
	ON_COMMAND(ID_IP_CONFIG, &CMovePathView::OnIpConfig)
END_MESSAGE_MAP()


// CMovePathView diagnostics

#ifdef _DEBUG
void CMovePathView::AssertValid() const
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::AssertValid"),"Enter");

	CFormView::AssertValid();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::AssertValid"),"Leave");
}

#ifndef _WIN32_WCE
void CMovePathView::Dump(CDumpContext& dc) const
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::Dump"),"Enter");

	CFormView::Dump(dc);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::Dump"),"Leave");
}
#endif
#endif //_DEBUG


// CMovePathView message handlers
//撤销
void CMovePathView::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditUndo"),"Enter");

	m_GraphMove.Undo();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditUndo"),"Leave");
}
//重做
void CMovePathView::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditRedo"),"Enter");

	m_GraphMove.Redo();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditRedo"),"Leave");
}
//全选
void CMovePathView::OnEditSelectAll()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditSelectAll"),"Enter");

	m_GraphMove.SelectAll();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditSelectAll"),"Leave");
}
//查找
void CMovePathView::OnEditFind()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditFind"),"Enter");

	m_GraphMove.Find();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditFind"),"Leave");
}
//筛选
void CMovePathView::OnEditShai()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditShai"),"Enter");

	m_GraphMove.Filter();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditShai"),"Leave");
}
//剪切
void CMovePathView::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditCut"),"Enter");

	m_GraphMove.Cut();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditCut"),"Leave");
}
//复制
void CMovePathView::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditCopy"),"Enter");

	m_GraphMove.Copy();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditCopy"),"Leave");
}
//粘贴
void CMovePathView::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditPaste"),"Enter");

	m_GraphMove.Paste();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditPaste"),"Leave");
}
//删除
void CMovePathView::OnEditClear()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditClear"),"Enter");

	m_GraphMove.Delete();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditClear"),"Leave");
}
//反向
void CMovePathView::OnEditFan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditFan"),"Enter");

	m_GraphMove.Reverse();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditFan"),"Leave");
}
//镜像

void CMovePathView::OnEditMirror()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditMirror"),"Enter");

	m_GraphMove.Mirror();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditMirror"),"Leave");
}

//旋转
void CMovePathView::OnEditRota()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditRota"),"Enter");

	m_GraphMove.Rotate();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnEditRota"),"Leave");
}
//导航
void CMovePathView::OnDrawDao()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawDao"),"Enter");

	m_GraphMove.Nevigation();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawDao"),"Leave");
}
//选择对象
void CMovePathView::OnDrawSelect()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawSelect"),"Enter");

	m_GraphMove.nSelect();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawSelect"),"Leave");
}


void CMovePathView::OnDrawHole()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawHole"),"Enter");

	m_GraphMove.DrawPoint();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawHole"),"Leave");
}


void CMovePathView::OnDrawLine()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawLine"),"Enter");

	m_GraphMove.DrawLine();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawLine"),"Leave");
}

void CMovePathView::OnDrawArc()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArc"),"Enter");

	m_GraphMove.DrawArc();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArc"),"Leave");
}

void CMovePathView::OnDrawArcCenetr()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArcCenetr"),"Enter");

	m_GraphMove.DrawThreeArc();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArcCenetr"),"Leave");
}


void CMovePathView::OnDrawCircle()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawCircle"),"Enter");

	m_GraphMove.DrawCircle();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawCircle"),"Leave");
}


void CMovePathView::OnDrawCircleSan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawCircleSan"),"Enter");

	m_GraphMove.DrawThreeCircle();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawCircleSan"),"Leave");
}

void CMovePathView::OnDrawArcline()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArcline"),"Enter");

	m_GraphMove.DrawArcLine();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawArcline"),"Leave");
}

void CMovePathView::OnDrawRect()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawRect"),"Enter");

	m_GraphMove.DrawRect();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawRect"),"Leave");
}

void CMovePathView::OnDrawEllipse()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawEllipse"),"Enter");

	m_GraphMove.DrawEllipse();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawEllipse"),"Leave");
}

void CMovePathView::OnDrawPolygon()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawPolygon"),"Enter");

	m_GraphMove.DrawPolygon();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawPolygon"),"Leave");
}

void CMovePathView::OnDrawRunway()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawRunway"),"Enter");

	m_GraphMove.DrawRunway();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDrawRunway"),"Leave");
}

void CMovePathView::OnToolCan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolCan"),"Enter");

	m_GraphMove.SysPara();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolCan"),"Leave");
}

void CMovePathView::OnToolColor()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolColor"),"Enter");

	m_GraphMove.LayerColor();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolColor"),"Leave");
}

void CMovePathView::OnToolJxian()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolJxian"),"Enter");

	m_GraphMove.LayerShow();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolJxian"),"Leave");
}

void CMovePathView::OnToolJpian()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolJpian"),"Enter");

	m_GraphMove.LayerMove();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolJpian"),"Leave");
}

void CMovePathView::OnToolEdit()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolEdit"),"Enter");

	m_GraphMove.EditDlg();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolEdit"),"Leave");
}

void CMovePathView::OnToolReset()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolReset"),"Enter");

	m_GraphMove.ReSet();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnToolReset"),"Leave");
}

void CMovePathView::OnToolRun()
{
	// TODO: 在此添加命令处理程序代码
}

void CMovePathView::OnViewShi()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewShi"),"Enter");

	m_GraphMove.ViewWindow();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewShi"),"Leave");
}

void CMovePathView::OnViewTu()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewTu"),"Enter");

	m_GraphMove.ViewGraph();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewTu"),"Leave");
}

void CMovePathView::OnViewWindow()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewWindow"),"Enter");

	m_GraphMove.ViewRegion();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnViewWindow"),"Leave");
}

void CMovePathView::OnSize(UINT nType, int cx, int cy)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnSize"),"Enter");

	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_GraphMove.GetSafeHwnd() != NULL)
	{
//		m_GraphMove.SetWindowPos(&wndTopMost,0,0,cx,cy,SWP_SHOWWINDOW);
//		m_GraphMove.SetFocus();
		m_GraphMove.MoveWindow(0, 0, cx, cy, TRUE);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnSize"),"Leave");
}
//分解
void CMovePathView::OnDecompose()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDecompose"),"Enter");

	m_GraphMove.Decompose();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnDecompose"),"Leave");
}

void CMovePathView::OnFileOpen()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileOpen"),"Enter");

	TCHAR szFilters[] =
		_T("Drawing files (*.dsj)|*.dsj|All files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("dsj"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		m_GraphMove.OpenFile(strPath);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileOpen"),"Leave");
}

void CMovePathView::OnFileSave()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileSave"),"Enter");

	m_GraphMove.SaveFile();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileSave"),"Leave");
}
void CMovePathView::OnFileNew()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileNew"),"Enter");

	m_GraphMove.NewFile();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnFileNew"),"Leave");
}
/*
BOOL CMovePathView::PreTranslateMessage(MSG* pMsg)
{
	// WangQianfeng add start on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::PreTranslateMessage"),"Enter");

	if (NULL == pMsg)
	{
		MessageBox(_T("CMovePathView::PreTranslateMessage,pMsg = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMovePathView::PreTranslateMessage"),"Leave");
		return FALSE;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::PreTranslateMessage"),"Leave");
	// WangQianfeng add end on 2012-11-05
	return CFormView::PreTranslateMessage(pMsg);
}
*/
void CMovePathView::OnIpConfig()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnIpConfig"),"Enter");

	m_GraphMove.SetIPDlg();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnIpConfig"),"Leave");
}
BEGIN_EVENTSINK_MAP(CMovePathView, CFormView)
	ON_EVENT(CMovePathView, IDC_GRAPHMOVECTRL, 4, CMovePathView::AxisSelectGraphmovectrl, VTS_I4)
	ON_EVENT(CMovePathView, IDC_MDSOCX, 5, CMovePathView::OnGraphEnd, VTS_NONE)
	// WangQianfeng delete start on 2013-06-28
	ON_EVENT(CMovePathView, IDC_MDSOCX, 6, CMovePathView::OnJump, VTS_I2)
	// WangQianfeng delete end on 2013-06-28
	ON_EVENT(CMovePathView, IDC_MDSOCX, 7, CMovePathView::OnPathStop, VTS_I2)
	ON_EVENT(CMovePathView, IDC_MDSOCX, 8, CMovePathView::OnReadFileMdsocx, VTS_NONE)
END_EVENTSINK_MAP()

void CMovePathView::AxisSelectGraphmovectrl(long AxisIndex)
{
	// TODO: Add your message handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::AxisSelectGraphmovectrl"),"Enter");

	m_nPanelIndex = AxisIndex;
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	WORD wXLow = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x30);
	DWORD wXHigh = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x32);
	WORD wYLow = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x34);
	DWORD wYHigh = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x36);
	WORD wZLow = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x38);
	DWORD wZHigh = P9054_ReadWord(pApp->m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x3a);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrX = ((wXHigh << 16) + wXLow) / CODER_PER_RING_XY;
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrY = ((wYHigh << 16) + wYLow) / CODER_PER_RING_XY;
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrZ = ((wZHigh << 16) + wZLow) / CODER_PER_RING_Z;
	
	// 旋转
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	if(nAxisIndex == 3)	
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrX = 0;
	}

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_fFirstWdrZ;
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::AxisSelectGraphmovectrl"),"Leave");
}



void CMovePathView::OnGraphEnd()
{
	// TODO: Add your message handler code here
}

void CMovePathView::OnPathStop(short p_flag)
{
	// TODO: Add your message handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnPathStop"),"Enter");

	if (p_flag == 1 && !m_bULDPtp)	// 运行结束
	{
		m_ParentWnd->PathStop( TRUE );
	}
	else if (p_flag == 3 && !m_bULDPtp)	// 运动到采集点
	{
		if (m_bESC)
		{
			m_ParentWnd->m_bPathExecute = TRUE;
			m_ParentWnd->PathStop( FALSE);
		} 
		else
		{
			m_ParentWnd->OnArriveStartPoint();
		}
	}
	else if (p_flag == 6)	// PTP
	{
	}
	else if (p_flag == 10)	// 复位
	{
		m_ParentWnd->OnResetLowerMachine();
	}
	else
	{}
	m_bESC = FALSE;

	if (m_bULDPtp)
	{
		m_bULDPtp = FALSE;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnPathStop"),"Leave");
}

void CMovePathView::OnJump(short p_flag)
{
	// TODO: Add your message handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnJump"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBScanView* pBView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4, 0);

	if (p_flag == 1)
	{
//		if(pFrame->m_bBScanDisplay)
		{
			pBView->m_vfeature.clear();
			pBView->InvalidateRect(pBView->m_BScanArea, TRUE);
		}
	}

	if (pFrame->m_bDuplicateDrawOnceEn)
	{
		if (p_flag == 1)
			pFrame->m_bDuplicateDrawOnce = FALSE;
		else
		{
			pFrame->m_bDuplicateDrawOnce = TRUE;
			pFrame->m_dwDuplicateDrawCount = 0;
		}
	}
	else
		pFrame->m_bDuplicateDrawOnce = FALSE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnJump"),"Leave");
}
// WangQianfeng delete end on 2013-06-28
void CMovePathView::OnReadFileMdsocx()
{
	// TODO: Add your message handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnReadFileMdsocx"),"Enter");

	m_ParentWnd->InitSysParamDlg();
//	m_ParentWnd->m_SysParamDlg.SetChannelStateNew(m_ParentWnd->m_SysParamDlg.m_nProbeNum, m_ParentWnd->m_SysParamDlg.m_nIndexOneProbeType,
//		m_ParentWnd->m_SysParamDlg.m_nIndexTwoProbeType, m_ParentWnd->m_SysParamDlg.m_nIndexThreeProbeType, m_ParentWnd->m_SysParamDlg.m_wWorkMode);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMovePathView::OnReadFileMdsocx"),"Leave");
}
