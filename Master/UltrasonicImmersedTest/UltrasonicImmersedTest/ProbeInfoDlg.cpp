// ProbeInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ProbeInfoDlg.h"


// CProbeInfoDlg dialog

IMPLEMENT_DYNAMIC(CProbeInfoDlg, CDialog)

CProbeInfoDlg::CProbeInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProbeInfoDlg::IDD, pParent)
	, m_nProbeFrequency(3)
	, m_nPressureType(3)
	, m_nOscillationMode(2)
	, m_nImpedanceType(0)
	, m_nLvBoType(0)
	, m_fLvBoHigh(0)
	, m_fLvBoLow(0)
	, m_nMaiKuan(50)
	, m_nSpiData(0)
	, m_nMidValue(504)
{
	m_wChannelNo = 0;
	m_wWorkMode = 0;
	m_nEnergy = 0;//yachang.huang add on 2013-11-25
	m_nRevAndSendMode = 0;//yachang.huang add on 2013-11-25
	m_nSpiAddr = 0;//yachang.huang add on 2013-11-25
	m_nSpiData = 0;//yachang.huang add on 2013-11-25
}

CProbeInfoDlg::~CProbeInfoDlg()
{
}

void CProbeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_1M, m_nProbeFrequency);
	DDX_Radio(pDX, IDC_RADIO_PRESSURE1, m_nPressureType);
	DDX_Radio(pDX, IDC_RADIO_PLUS_HWAVE, m_nOscillationMode);
	DDX_Radio(pDX, IDC_RADIO_LOW_IMPEDANCE, m_nImpedanceType);
	//DDX_Radio(pDX, IDC_RADIO_HIGH_LVBO, m_nLvBoType);
	//DDX_Text(pDX, IDC_EDIT_HIGH_LVBO, m_fLvBoHigh);
	//DDX_Text(pDX, IDC_EDIT_LOW_LVBO, m_fLvBoLow);
	//DDX_Text(pDX, IDC_EDIT_MAIKUAN, m_nMaiKuan);
	DDX_Radio(pDX, IDC_RADIO_LOW_ERNERGY, m_nEnergy);//yachang.huang add on 2013-11-25
	DDX_Radio(pDX, IDC_RADIO_LOW_OneSendOneRev, m_nRevAndSendMode);//yachang.huang add on 2013-11-25
	DDX_Radio(pDX, IDC_RADIO_RDA11, m_nSpiAddr);//yachang.huang add on 2013-11-25

	DDX_Control(pDX, IDC_SLIDER_SPI_DATA, m_sliderSpiData);//yachang.huang add on 2013-11-25
	DDX_Text(pDX, IDC_EDIT_SPI_DATA, m_nSpiData);//yachang.huang add on 2013-11-25
	DDX_Text(pDX, IDC_EDIT_MID_VALUE, m_nMidValue);
}


BEGIN_MESSAGE_MAP(CProbeInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProbeInfoDlg::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPI_DATA, &CProbeInfoDlg::OnDeltaposSpinSpiData)
END_MESSAGE_MAP()


// CProbeInfoDlg message handlers
BOOL CProbeInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
// 	CComboBox* pcmdWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
// 
// 	pcmdWorkMode->SetCurSel(m_wWorkMode);
	m_sliderSpiData.SetRange(0, 16383);
	m_sliderSpiData.SetPos(0);
	m_nSpiData = 0;
	return TRUE;
}


void CProbeInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
// 	CComboBox* pCmbWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
// 	m_wWorkMode = pCmbWorkMode->GetCurSel();
	UpdateData(TRUE);
	OnOK();
}
void CProbeInfoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnHScroll"),"Enter");

	// TODO: Add your message handler code here and/or call default
	if (nSBCode == TB_THUMBTRACK && pScrollBar != NULL)
	{
		UpdateData(TRUE);
		int nCtrlID = pScrollBar->GetDlgCtrlID();

		if (nCtrlID == IDC_SLIDER_SPI_DATA)	
		{			
			m_nSpiData = m_sliderSpiData.GetPos();
		}
	}	
    UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnHScroll"),"Leave");
}
void CProbeInfoDlg::OnDeltaposSpinSpiData(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinFront"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_nSpiData -= pNMUpDown->iDelta;
	m_sliderSpiData.SetPos(m_nSpiData);

	*pResult = 0;
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinFront"),"Leave");
}