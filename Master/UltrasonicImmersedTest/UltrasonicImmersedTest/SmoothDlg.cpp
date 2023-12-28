// SmoothDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "SmoothDlg.h"


// CSmoothDlg �Ի���

IMPLEMENT_DYNAMIC(CSmoothDlg, CDialog)

CSmoothDlg::CSmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmoothDlg::IDD, pParent)
	, m_nTempH(3)
	, m_nTempW(3)
	, m_nTempMX(1)
	, m_nTempMY(1)
	, m_fTempC(0)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::CSmoothDlg"),"Enter");

	m_fTempC  = (float) (1.0 / (m_nTempH*m_nTempW));
	CString str;
	str.Format(_T("%.2f"),m_fTempC);
	m_fTempC = _tstof(str);

	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 1.0;
	m_fpArray[11] = 1.0;
	m_fpArray[12] = 1.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;


	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::CSmoothDlg"),"Leave");
}

CSmoothDlg::~CSmoothDlg()
{
}

void CSmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEMPH, m_nTempH);
	DDV_MinMaxInt(pDX, m_nTempH, 2, 5);
	DDX_Text(pDX, IDC_EDIT_TEMPW, m_nTempW);
	DDX_Text(pDX, IDC_EDIT_MX, m_nTempMX);
	DDX_Text(pDX, IDC_EDIT_MY, m_nTempMY);
	DDX_Text(pDX, IDC_EDIT_TEMPC, m_fTempC);
	DDX_Text(pDX, IDC_EDIT_V0, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT_V1, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT_V2, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT_V3, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT_V4, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT_V5, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT_V6, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT_V7, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT_V8, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT_V9, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT_V10, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT_V11, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT_V12, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT_V13, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT_V14, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT_V15, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT_V16, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT_V17, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT_V18, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT_V19, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT_V20, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT_V21, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT_V22, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT_V23, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT_V24, m_fpArray[24]);
}


BEGIN_MESSAGE_MAP(CSmoothDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPH, &CSmoothDlg::OnEnKillfocusEditTemph)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMPW, &CSmoothDlg::OnEnKillfocusEditTempw)
	ON_BN_CLICKED(IDOK, &CSmoothDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSmoothDlg ��Ϣ�������

//�����ı�ģ��״̬
void CSmoothDlg::UpdateEdit(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::UpdateEdit"),"Enter");

	int i,j;
	for (i = IDC_EDIT_V0; i <= IDC_EDIT_V24; i++)
	{
		(CEdit *) GetDlgItem(i)->EnableWindow(TRUE);
	}	
	for (i = 0; i < m_nTempH; i++)
	{
		for (j = 0; j < m_nTempW; j++)
		{
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
	for (i = 0; i < m_nTempH; i++)
	{
		for (j = m_nTempW; j < 5; j++)
		{
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}
	for (i = m_nTempH; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			(CEdit *) GetDlgItem(IDC_EDIT_V0 + i*5 + j)->ShowWindow(SW_HIDE);
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::UpdateEdit"),"Leave");
}

void CSmoothDlg::OnEnKillfocusEditTemph()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnEnKillfocusEditTemph"),"Enter");

	UpdateData(TRUE);
	UpdateEdit();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnEnKillfocusEditTemph"),"Leave");
}


void CSmoothDlg::OnEnKillfocusEditTempw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnEnKillfocusEditTempw"),"Enter");

	UpdateData(TRUE);
	UpdateEdit();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnEnKillfocusEditTempw"),"Leave");
}

BOOL CSmoothDlg::OnInitDialog()
{

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnInitDialog"),"Enter");

	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateEdit();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnInitDialog"),"Leave");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSmoothDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnBnClickedOk"),"Enter");

	UpdateData(TRUE);
	//�ж������Ƿ���Ч
	if ((m_nTempMX < 0)||(m_nTempMY < 0)||(m_nTempMX > m_nTempW - 1)||(m_nTempMY > m_nTempH - 1))
	{
		MessageBox(_T("����Ԫ�ز������ô���"));

		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnBnClickedOk"),"Leave");

		return;
	}
	// ����ģ��Ԫ�����飨����ЧԪ�ط����������ǰ�棩
	for (int i = 0; i < m_nTempH; i++)
	{
		for (int j = 0; j < m_nTempW; j++)
		{
			m_fpArray[i * m_nTempW + j] = m_fpArray[i * 5 + j];
		}
	}	
	UpdateData(FALSE);

	CDialog::OnOK();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSmoothDlg::OnBnClickedOk"),"Leave");
}
