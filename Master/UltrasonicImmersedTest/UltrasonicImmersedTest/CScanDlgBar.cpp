// CScanDlgBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"


// CCScanDlgBar �Ի���

IMPLEMENT_DYNAMIC(CCScanDlgBar, CBCGPDialogBar)

CCScanDlgBar::CCScanDlgBar(CWnd* pParent /*=NULL*/)
	//: CDialog(CCScanDlgBar::IDD, pParent)
{

}

CCScanDlgBar::~CCScanDlgBar()
{
}

void CCScanDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCScanDlgBar, CBCGPDialogBar)
END_MESSAGE_MAP()


// CCScanDlgBar ��Ϣ�������

BOOL CCScanDlgBar::OnInitDialog()
{
	//CDialogBar::OnInitDialog();
	UpdateData(FALSE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
