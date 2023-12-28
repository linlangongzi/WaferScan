// ExButton.cpp : 实现文件
//

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
#include "MovePathView.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "ExButton.h"
#include "FeatureInfoWnd.h"


// CExButton

IMPLEMENT_DYNAMIC(CExButton, CButton)

CExButton::CExButton()
{
	m_pParent = NULL;
}

CExButton::~CExButton()
{
}


BEGIN_MESSAGE_MAP(CExButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CExButton 消息处理程序



void CExButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	short status = m_pParent->m_pMovePathView->m_GraphMove.GetWorkStatus();
	if (status == 200)
	{
		MessageBox(_T("系统未复位，请复位再执行此操作！"), _T("提示"), MB_OK);
		return;
	}
	if (status==7)
	{
		return;
	}
	m_pParent->HandStart(type);
	//在此弹出messagebox后 不能执行OnLButtonUp函数，则鼠标焦点不能离开

	CButton::OnLButtonDown(nFlags, point);
}

void CExButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pParent->HandStop();

	CButton::OnLButtonUp(nFlags, point);
}
