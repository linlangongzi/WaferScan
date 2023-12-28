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

	//初始化滚动条
	SCROLLINFO	info;
	//选择幅值滚动条
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = 1009;//最大262143个点
	info.nPage = 10;
	info.nPos = 0;
	info.nTrackPos = 1;
	m_scrollAmpLevel.SetScrollInfo(&info);

	//当前位置滚动条
	info.nMin = 0;
	info.nMax = 30099;//最大262143个点
	info.nPage = 100;
	info.nPos = 0;
	info.nTrackPos = 1;
	m_scrollCurPos.SetScrollInfo(&info);

	//	m_ListResult.DeleteAllItems();

	DWORD dwStyle = m_DACList.GetExtendedStyle();//设置list控件的显示，选中一行则整行都亮
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_DACList.SetExtendedStyle(dwStyle); 

	CString strTitle[3] = {_T("序号"), _T("Position(us)"), _T("Gain(dB)")};
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
	//得到滚动条的移动范围
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos,&maxpos);
	maxpos=pScrollBar->GetScrollLimit();

	//得到当前滚动条的位置
	int curpos=pScrollBar->GetScrollPos();
	//得到当前滚动条的信息
	SCROLLINFO	info;
	pScrollBar->GetScrollInfo(&info,SIF_ALL);

	//根据响应情况选择新的滚动条位置
	switch(nSBCode)
	{
	case SB_LINELEFT://点击左边的箭头
		if(curpos>minpos)
			curpos--;
		break;
	case SB_LINERIGHT://点击右边的箭头
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
		//设置拖拽移动
	case SB_THUMBPOSITION:
		curpos=nPos;
		break;
	case SB_THUMBTRACK:
		curpos=nPos;
		break;
	}

	//设置滚动条的新位置
	pScrollBar->SetScrollPos(curpos);
	//更新数据
	UpdateData(TRUE);
	m_ampLevel = (float)m_scrollAmpLevel.GetScrollPos()/100;//选择的幅值
	m_curPos = (float)m_scrollCurPos.GetScrollPos()/100;//当前位置
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
