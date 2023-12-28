// DacMakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "DacMakeDlg.h"


// CDacMakeDlg dialog

IMPLEMENT_DYNAMIC(CDacMakeDlg, CDialog)

CDacMakeDlg::CDacMakeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDacMakeDlg::IDD, pParent)
{
	m_ampLevel = 0;
	m_curPos = 0;
	m_curAmp = 0;
}

CDacMakeDlg::~CDacMakeDlg()
{
}

void CDacMakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DAC, m_DACList);
	DDX_Control(pDX, IDC_SCROLLBAR_AMP_LEVEL, m_scrollAmpLevel);
	DDX_Control(pDX, IDC_SCROLLBAR_CUR_POS, m_scrollCurPos);
	DDX_Text(pDX, IDC_EDIT_AMP_LEVEL, m_ampLevel);
	DDX_Text(pDX, IDC_EDIT_CUR_POS, m_curPos);
	DDX_Text(pDX, IDC_EDIT_CUR_AMP, m_curAmp);
}


BEGIN_MESSAGE_MAP(CDacMakeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_DAC_STAT_ON, &CDacMakeDlg::OnBnClickedRadioDacStatOn)
	ON_BN_CLICKED(IDC_RADIO_DAC_STAT_OFF, &CDacMakeDlg::OnBnClickedRadioDacStatOff)
	ON_BN_CLICKED(IDC_RADIO_DAC_SHOW_ON, &CDacMakeDlg::OnBnClickedRadioDacShowOn)
	ON_BN_CLICKED(IDC_RADIO_DAC_SHOW_OFF, &CDacMakeDlg::OnBnClickedRadioDacShowOff)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDacMakeDlg message handlers

BOOL CDacMakeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//��ʼ��������
	SCROLLINFO	info;
	//ѡ���ֵ������
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = 1009;//���262143����
	info.nPage = 10;
	info.nPos = 0;
	info.nTrackPos = 1;
	m_scrollAmpLevel.SetScrollInfo(&info);

	//��ǰλ�ù�����
	info.nMin = 0;
	info.nMax = 30099;//���262143����
	info.nPage = 100;
	info.nPos = 0;
	info.nTrackPos = 1;
	m_scrollCurPos.SetScrollInfo(&info);

	//	m_ListResult.DeleteAllItems();

	DWORD dwStyle = m_DACList.GetExtendedStyle();//����list�ؼ�����ʾ��ѡ��һ�������ж���
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_DACList.SetExtendedStyle(dwStyle); 

	CString strTitle[3] = {_T("���"), _T("Position(us)"), _T("Gain(dB)")};
	for(int j = 0; j <3; j++)
	{		
		m_DACList.InsertColumn(j, strTitle[j],LVCFMT_CENTER, 80);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDacMakeDlg::OnBnClickedRadioDacStatOn()
{
	// TODO: Add your control notification handler code here
	m_bDacEnable = TRUE;
}

void CDacMakeDlg::OnBnClickedRadioDacStatOff()
{
	// TODO: Add your control notification handler code here
	m_bDacEnable = FALSE;
}

void CDacMakeDlg::OnBnClickedRadioDacShowOn()
{
	// TODO: Add your control notification handler code here
	m_bDacShowFlag = TRUE;
}

void CDacMakeDlg::OnBnClickedRadioDacShowOff()
{
	// TODO: Add your control notification handler code here
	m_bDacShowFlag = FALSE;
}

void CDacMakeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//�õ����������ƶ���Χ
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos,&maxpos);
	maxpos=pScrollBar->GetScrollLimit();

	//�õ���ǰ��������λ��
	int curpos=pScrollBar->GetScrollPos();
	//�õ���ǰ����������Ϣ
	SCROLLINFO	info;
	pScrollBar->GetScrollInfo(&info,SIF_ALL);

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
		//������ק�ƶ�
	case SB_THUMBPOSITION:
		curpos=nPos;
		break;
	case SB_THUMBTRACK:
		curpos=nPos;
		break;
	}

	//���ù���������λ��
	pScrollBar->SetScrollPos(curpos);
	//��������
	UpdateData(TRUE);
	m_ampLevel = (float)m_scrollAmpLevel.GetScrollPos()/100;//ѡ��ķ�ֵ
	m_curPos = (float)m_scrollCurPos.GetScrollPos()/100;//��ǰλ��
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
