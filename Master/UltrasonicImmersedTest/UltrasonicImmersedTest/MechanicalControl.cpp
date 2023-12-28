// MechanicalControl.cpp : ʵ���ļ�
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ��е���ƹ�����Դ�ļ�
*Filename : MechanicalControl.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-10 
*/

#include "stdafx.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "UltrasonicImmersedTest.h"
#include "MechanicalControl.h"


// CMechanicalControl

IMPLEMENT_DYNAMIC(CMechanicalControl, CBCGPDockingControlBar)

CMechanicalControl::CMechanicalControl()
{
	CRuntimeClass* pFactory = RUNTIME_CLASS(CMechanicalControlPanel);
	m_pMechanicalControlView = (CMechanicalControlPanel *)(pFactory->CreateObject());
}

CMechanicalControl::~CMechanicalControl()
{
}


BEGIN_MESSAGE_MAP(CMechanicalControl, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CMechanicalControl ��Ϣ�������



int CMechanicalControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rect(0,0,1000,1000);
	// TODO:  Add your specialized creation code here
	if (m_pMechanicalControlView != NULL)
	{
		m_pMechanicalControlView->CreateView(this, m_pContext);
	}

	return 0;
}

void CMechanicalControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBCGPDockingControlBar::OnPaint()
	CRect rect;
	GetClientRect (rect);

	dc.FillSolidRect (rect, ::GetSysColor (COLOR_3DFACE));
}

void CMechanicalControl::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	if (m_pMechanicalControlView->GetSafeHwnd())
	{
		m_pMechanicalControlView->SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
