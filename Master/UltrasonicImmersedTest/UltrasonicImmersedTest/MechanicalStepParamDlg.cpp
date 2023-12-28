// MechanicalStepParamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MechanicalStepParamDlg.h"

// CMechanicalStepParamDlg �Ի���

IMPLEMENT_DYNAMIC(CMechanicalStepParamDlg, CDialog)

CMechanicalStepParamDlg::CMechanicalStepParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMechanicalStepParamDlg::IDD, pParent)
	, m_fXStep(0)
	, m_fYStep(0)
	, m_fZStep(0)
	, m_fABStep(0)
	, m_fWStep(0)
	, m_fRStep(0)
{

}

CMechanicalStepParamDlg::~CMechanicalStepParamDlg()
{
}

void CMechanicalStepParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STEP_X, m_fXStep);
	DDX_Text(pDX, IDC_EDIT_STEP_Y, m_fYStep);
	DDX_Text(pDX, IDC_EDIT_STEP_Z, m_fZStep);
	DDX_Text(pDX, IDC_EDIT_STEP_AB, m_fABStep);
	DDX_Text(pDX, IDC_EDIT_STEP_W, m_fWStep);
	DDX_Text(pDX, IDC_EDIT_STEP_R, m_fRStep);
}


BEGIN_MESSAGE_MAP(CMechanicalStepParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMechanicalStepParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMechanicalStepParamDlg ��Ϣ�������

void CMechanicalStepParamDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	OnOK();
}
