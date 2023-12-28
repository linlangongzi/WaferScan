// ExButton.cpp : ʵ���ļ�
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



// CExButton ��Ϣ�������



void CExButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	short status = m_pParent->m_pMovePathView->m_GraphMove.GetWorkStatus();
	if (status == 200)
	{
		MessageBox(_T("ϵͳδ��λ���븴λ��ִ�д˲�����"), _T("��ʾ"), MB_OK);
		return;
	}
	if (status==7)
	{
		return;
	}
	m_pParent->HandStart(type);
	//�ڴ˵���messagebox�� ����ִ��OnLButtonUp����������꽹�㲻���뿪

	CButton::OnLButtonDown(nFlags, point);
}

void CExButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pParent->HandStop();

	CButton::OnLButtonUp(nFlags, point);
}
