// PaletteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "PaletteDlg.h"
#include "AddDlg.h"
#include <math.h>
#include <afxpriv.h>


// CPaletteDlg dialog

IMPLEMENT_DYNAMIC(CPaletteDlg, CDialog)

CPaletteDlg::CPaletteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDlg::IDD, pParent)
 , m_nMainColorNum(0)
 , m_bMenuInsert(FALSE)
 , m_bMenuClear(FALSE)
 , m_nNumRect(2)
 , m_nMaxAdd(253)
 , m_bPlatte(FALSE)
//  ,m_bToolbarAdd(FALSE)
//  ,m_bToolbarDelete(FALSE)
{
	m_color = RGB(255,76,9);
	m_clrBackground = RGB(180,180,180);
	m_clrBottom = RGB(75, 140, 255);

	m_pStaticDlg = new CStaticDlg;
	m_vnyColorbar.push_back(200);
	m_vnyColorbar.push_back(350);
	m_vnyColorbar.push_back(500);
}

CPaletteDlg::~CPaletteDlg()
{
	if (m_pStaticDlg != NULL)
	{
		delete m_pStaticDlg;
		m_pStaticDlg = NULL;
	}
}

void CPaletteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPaletteDlg, CDialog)
	ON_WM_INITMENUPOPUP()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_ADD, &CPaletteDlg::OnBnClickedButton1)
	ON_COMMAND(ID_BUTTON_DELETE, &CPaletteDlg::OnBnClickedButton2)
	ON_COMMAND(ID_FILE_NEW, &CPaletteDlg::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CPaletteDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CPaletteDlg::OnFileSave)
	ON_COMMAND(ID_FILE_QUIT, &CPaletteDlg::OnFileQuit)
	ON_COMMAND(ID_LEVEL_INSERT, &CPaletteDlg::OnBnClickedButton1)
	ON_COMMAND(ID_LEVEL_CLEAR, &CPaletteDlg::OnBnClickedButton2)
	ON_COMMAND(ID_FILE_DEFINE, &CPaletteDlg::OnFileDefine)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_INSERT, &CPaletteDlg::OnUpdateLevelInsert)
	ON_UPDATE_COMMAND_UI(ID_LEVEL_CLEAR, &CPaletteDlg::OnUpdateLevelClear)
// 	ON_UPDATE_COMMAND_UI(ID_BUTTON_ADD, &CPaletteDlg::OnUpdateBttonAdd)
// 	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE, &CPaletteDlg::OnUpdateBttonDelete)
END_MESSAGE_MAP()

BOOL CPaletteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( !m_WndToolBar.CreateEx( this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_WndToolBar.LoadToolBar( IDR_TOOLBAR_PALETTE ))
	{
		MessageBox( _T("Can not create ToolBar!"),_T("Error"),MB_ICONWARNING | MB_OK );
		return FALSE;
	}
//	m_WndToolBar.ShowWindow( SW_SHOW );
	RepositionBars( AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0 );

//	m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_ADD,FALSE);
//	m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_DELETE,FALSE);
	// 得到窗口位置的大小

	int n = m_pStaticDlg->Create(IDD_STATIC_DIALOG, this);
	GetDlgItem(IDC_STATIC_SIZE)->GetWindowRect(rc);
	ScreenToClient(&rc);

	//设置窗口位置
	m_pStaticDlg->MoveWindow(rc);

	CClientDC dc(this);
	fontAmp.CreatePointFont(120, _T("Calibri"), &dc);
	GetDlgItem(IDC_STATIC_AMP)->SetFont(&fontAmp);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CPaletteDlg message handlers

void CPaletteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	m_pDC = GetDC();   //获取DC

	InitDiscolorRect();
	DrawVRuler(m_pDC,CRect(60, 200, 60, 500),0,127,10,1,1,0);
	DrawVRuler(m_pDC,CRect(117,200,117,500),0,127,10,0,1,1);
}

HCURSOR CPaletteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPaletteDlg::InitDiscolorRect()
{
	for(int i = 0; i < m_nNumRect; i++) 
	{
		ChangeColorRect(i);
	}
	SpecificColor();
}

//*************画背景色底色*******************
void CPaletteDlg::SpecificColor()
{
	CBrush brush3(m_clrBackground);
	m_pDC->FillRect(CRect(110, 200, 118, 510),&brush3);
	m_pDC->FillRect(CRect(35,120,95,140),&brush3);

	CBrush brush4(m_clrBottom);
	m_pDC->FillRect(CRect(60, 500, 110, 510),&brush4);
	m_pDC->FillRect(CRect(175, 120, 235, 140),&brush4);
}

void CPaletteDlg::ChangeColorRect(int nIndex)
{
	CBrush brush(m_pStaticDlg->m_vColor[nIndex]);
	m_pDC->FillRect(CRect(60, m_vnyColorbar[nIndex], 110, m_vnyColorbar[nIndex + 1]),&brush);
//	UpdateWindow();
}

//*****************画刻度**************
BOOL CPaletteDlg::DrawVRuler(CDC *pDC,CRect rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection,int ndB)
{
	////LOG4CXX_INFO(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CRulerWnd::DrawHRuler pDC"), _T("警告"), MB_OK|MB_ICONERROR);
		//LOG4CXX_ERROR(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02

	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , RGB( 0 , 0 , 0 ) );
	CPen *pOldPen = pDC->SelectObject( &pen );

	if (nTextLocation)
	{
		pDC->MoveTo(rect.left - 2,rect.top);
		pDC->LineTo(rect.right - 2,rect.bottom);
	}
	else
	{
		pDC->MoveTo(rect.left + 2,rect.top);
		pDC->LineTo(rect.right + 2,rect.bottom);
	}

	LOGFONT lf;

	memset( &lf , 0 , sizeof( lf ) );

	CFont font;
	font.CreatePointFont(90 , _T( "黑体" ), pDC );
	font.GetLogFont( &lf );
//	font.DeleteObject();
	if (nEnd == 20)
	{
		int test = 0;
	}

	CRect rectText;
	int iSayac = 0;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( nTextLocation == 1 ) 
	{
		lf.lfOrientation = 900;
		lf.lfEscapement  = 900;

		rectText.right  = rect.right - 10;

		iLLineStart = rect.right - 9;
		iLLineEnd	 = rect.right - 2;

		iMLineStart = rect.right - 6;
		iMLineEnd	 = rect.right - 2;

		iSLineStart = rect.right - 4;
		iSLineEnd	 = rect.right - 2;
	}
	else if( nTextLocation == 0 ) 
	{
		lf.lfOrientation = -900;
		lf.lfEscapement  = -900;

		rectText.left  = rect.left + 10;

		iLLineStart = rect.left  + 2;
		iLLineEnd	 = rect.left  + 9;

		iMLineStart = rect.left  + 2;
		iMLineEnd	 = rect.left + 6;

		iSLineStart = rect.left + 2;
		iSLineEnd	 = rect.left + 4;
	}
	pDC->FillSolidRect( &rect , RGB(7,165,0) );
	CFont *pOldFont = pDC->SelectObject( &font );

	CString str;

	int iPos;
	int iSel = 0;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( RGB( 0 , 0 , 0 ) );


	bool bXiaoyu = FALSE;
	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}
	nStep = nStep/10;
	if (nStep%5 != 0)
	{
		if (nStep > 3)
		{
			nStep = (nStep/5 + 1)*5;
		}
		else
		{

		}
	}


	int nLinestart = (nStart/nStep + 1)*nStep;
	if (nStart == 0)
	{
		nLinestart = 0;
	}
	for (int i = nLinestart;i < nEnd; i+=nStep)
	{
		if (i % nStep == 0)
		{
			if( nTextDirection == 0 )  
			{
				iPos = rect.top + (rect.Height()*(i - nStart))/(nEnd - nStart);
				rectText.top  = iPos;
			}
			else if(nTextDirection == 1 )
			{
				iPos = rect.bottom - (rect.Height()*(i - nStart))/(nEnd - nStart);
				rectText.top  = iPos - 13;
			}
			rectText.bottom = rectText.top + 20;

			if (i % (nStep*5) == 0)
			{
				pDC->MoveTo(iMLineStart,iPos);
				pDC->LineTo(iMLineEnd,iPos);	
				if (i % (nStep*10) == 0)
				{
					pDC->MoveTo(iLLineStart,iPos);
					pDC->LineTo( iLLineEnd,iPos);
					if( nTextLocation == 0 )
					{
						rectText.right = rectText.left + 43;
					}
					else
					{
						rectText.left = rectText.right - 43;
					}

					if (rectText.top < rect.top)
					{
						rectText.top = rect.top;
					}
					if (rectText.bottom > rect.bottom)
					{
						rectText.bottom = rect.bottom;
					}
					if (bXiaoyu == 1 && ndB == 0)
					{
						str.Format(_T("%d"), i/100);
					}
					else if(bXiaoyu == 0 && ndB == 0)
					{
						str.Format(_T("%d"), i);
					}
					else if (bXiaoyu == 1 && ndB == 1 && i != 0)
					{
						str.Format(_T("%.2f"), 0 - 20 * log10f(100 / ((float)i/100)));
					}
					else if (bXiaoyu == 0 && ndB == 1 && i != 0)
					{
						str.Format(_T("%.2f"), 0 - 20 * log10f(100 / (float)i));
					}
					else if (ndB == 1 && i == 0)
					{
						str = _T("-∞");
					}
					if( nTextLocation == 0 )
					{
						pDC->DrawText( str , &rectText , DT_LEFT );
					}
					else
					{
						pDC->DrawText( str , &rectText , DT_RIGHT );
					}

					continue;
				}
			}
// 			pDC->MoveTo(iSLineStart,iPos);
// 			pDC->LineTo( iSLineEnd,iPos );
			if ((rect.Height()*nStep)/(nEnd - nStart) > rect.Height()/10 )
			{
				if( nTextLocation == 0 )
				{
					rectText.right = rectText.left + 43;
				}
				else
				{
					rectText.left = rectText.right - 43;
				}
				str.Format(_T("%d"), i);
				if( nTextLocation == 0 )
				{
					pDC->DrawText( str , &rectText , DT_LEFT );
				}
				else
				{
					pDC->DrawText( str , &rectText , DT_RIGHT );
				}
			}
		}
	}

	pDC->SelectObject( pOldFont );
	font.DeleteObject();
	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
	////LOG4CXX_INFO(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
	return TRUE;
}
//***************计算颜色条大小分配******************
void CPaletteDlg::InitDisplayRect(int nNum)
{
	int nIndex = m_pStaticDlg->m_nSite;
	m_nNumRect += nNum;
	POINT leftTop = {10,m_vnyColorbar[nIndex - 1]};

	int nyTop = m_vnyColorbar[nIndex - 1];
	int nyBottom = m_vnyColorbar[nIndex];
	int nlevel = 0;
	if ((nyBottom - nyTop) % (nNum + 1) != 0)
	{
		nlevel = (nNum+1) % ((nyBottom - nyTop) % (nNum+1));
	}
	for(int i = 0; i < nNum; i++)
	{
		if ((nyBottom - nyTop) >= (nNum + 1))
		{
			if(nlevel != 0 && (nyBottom - nyTop) % (nNum + 1) != 0)
			{
				if (i % ((nNum+1) / ((nyBottom - nyTop) % (nNum + 1)) + 1) == 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1) + 1;
					nlevel--;
				}
				else if (i % ((nNum+1) / ((nyBottom - nyTop) % (nNum + 1)) + 1) != 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1);
				}
			}
			else if (nlevel == 0 && (nyBottom - nyTop) % (nNum + 1) != 0)
			{
				if (i % ((nNum+1) / ((nyBottom - nyTop) % (nNum + 1))) == 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1) + 1;
				}
				else if (i % ((nNum+1) / ((nyBottom - nyTop) % (nNum + 1))) != 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1);
				}
			}
			else if ((nyBottom - nyTop) % (nNum + 1) == 0)
			{
				leftTop.y += (nyBottom - nyTop) / (nNum + 1);
			}
		}
		else if ((nyBottom - nyTop) < (nNum + 1))
		{
			if (nlevel != 0)
			{
				if (i % ((nNum+1) / (nyBottom - nyTop) + 1) == 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1) + 1;
					nlevel--;
				}
				else if(i % ((nNum+1) / (nyBottom - nyTop) + 1) != 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1);
				}
			}
			else if (nlevel == 0)
			{
				if (i % ((nNum+1) / (nyBottom - nyTop)) == 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1) + 1;
				}
				else if(i % ((nNum+1) / (nyBottom - nyTop)) != 0)
				{
					leftTop.y += (nyBottom - nyTop) / (nNum + 1);
				}
			}
		}
		m_vnyColorbar.insert(m_vnyColorbar.begin() + nIndex + i,leftTop.y);

// 		CBrush brush(m_pStaticDlg->m_vColor[nIndex + i]);
// 		m_pDC->FillRect(CRect(60, m_vnyColorbar[nIndex + i], 110, m_vnyColorbar[nIndex + i + 1]),&brush);
		ChangeColorRect(nIndex + i);
	}
}

void CPaletteDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	CAddDlg m_addDlg;

	if(m_addDlg.DoModal() == IDOK)
	{
		if(m_addDlg.nAddRect == 0)
		{
			MessageBox(_T("插入最小值为1！"));
			return;
		}
		else if (m_addDlg.nAddRect > m_nMaxAdd)
		{
			CString str;
			str.Format(_T("插入最大值为%d！"), m_nMaxAdd);
			MessageBox(str);
			return;
		}

//		m_pStaticDlg->m_nAddNum = m_addDlg.nAddRect; 
		m_pStaticDlg->m_nRectNum += m_addDlg.nAddRect;
		m_pStaticDlg->AddItem(m_addDlg.nAddRect);
		InitDisplayRect(m_addDlg.nAddRect);
		m_pStaticDlg->LayOutControl();
		m_pStaticDlg->LayOutControlGraduate();
		m_pStaticDlg->LayOutControlStatic();
		m_nMaxAdd -= m_addDlg.nAddRect;
	}
	else
	{
		return;
	}
}

void CPaletteDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNumRect--;
	m_nMaxAdd++;
	m_pStaticDlg->DeleteItem();
}

//************ 判断用户是否点击了某个矩形区域 ************
void CPaletteDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x >= 35 && point.y >= 120 && point.x <= 95 && point.y <= 140)
	{
		ChangeBackground();
	}
	else if (point.x >= 175 && point.y >= 120 && point.x <= 235 && point.y <= 140)
	{
		ChangeAcquisition();
	}
}

BOOL CPaletteDlg::ChangeBackground()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_color;
	if(dlg.DoModal() == IDOK)
	{
		m_clrBackground = dlg.m_cc.rgbResult;
		SpecificColor();
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CPaletteDlg::ChangeAcquisition()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_color;
	if(dlg.DoModal() == IDOK)
	{
		m_clrBottom = dlg.m_cc.rgbResult;
		SpecificColor();
		return true;
	}
	else
	{
		return false;
	}
}

void CPaletteDlg::OnFileNew()
{
	// TODO: Add your command handler code here
	if (m_pStaticDlg)
	{
		m_pStaticDlg->DeleteLayoutControl();
		m_pStaticDlg->m_vColor.clear();
		m_pStaticDlg->m_vEdit.clear();
		m_pStaticDlg->m_vGraduateEdit.clear();
		m_pStaticDlg->m_vStatic.clear();
		m_pStaticDlg->m_vfPercent.clear();
		m_pStaticDlg->m_vmClrMain.clear();
	}

	m_pStaticDlg->m_nRectNum = 3;
	m_nMainColorNum = 0;
	m_pStaticDlg->m_vfPercent.push_back(127.50);
	m_pStaticDlg->m_vfPercent.push_back(63.75);
	m_pStaticDlg->m_vfPercent.push_back(0.00);
	m_pStaticDlg->InitializeMColor();
	m_pStaticDlg->OnInitDialog();
	m_pStaticDlg->SetScrollRange(SB_VERT, 0, 0, TRUE);
	m_nNumRect = 2;
	m_nMaxAdd = 253;
	m_clrBackground = RGB(180,180,180);
	m_clrBottom = RGB(75, 140, 255);
	m_vnyColorbar.clear();
	m_vnyColorbar.push_back(200);
	m_vnyColorbar.push_back(350);
	m_vnyColorbar.push_back(500);
	InitDiscolorRect();
}

void CPaletteDlg::OnFileOpen()
{
	// TODO: Add your command handler code here
	CString strExt = _T(".prt");
	CString strFilePath;
	CString strFilter;

	m_nMaxAdd = 253;
	m_pStaticDlg->SetScrollPos(SB_VERT,0);
	strFilter.Format(_T("Prt Files (*prt)|*prt|"));
	CFileDialog dlg(TRUE, NULL, NULL, NULL, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		m_pStaticDlg->DeleteLayoutControl();
		if (m_pStaticDlg)
		{
			m_pStaticDlg->m_vColor.clear();
			m_pStaticDlg->m_vEdit.clear();
			m_pStaticDlg->m_vGraduateEdit.clear();
			m_pStaticDlg->m_vStatic.clear();
			m_pStaticDlg->m_vfPercent.clear();
			m_pStaticDlg->m_vmClrMain.clear();
		}
		m_vnyColorbar.clear();

		strFilePath = dlg.GetPathName();
		if (strFilePath.Find(strExt) == -1)
		{
			MessageBox(_T("找不到文件，请检查文件名是否正确"));
		}
		CFile file(strFilePath,CFile::modeRead);
		file.Read(&m_nMainColorNum,sizeof(m_nMainColorNum));
		for (int i = 0; i < m_nMainColorNum; i++)
		{
			MCOLORR mclr;
			file.Read(&mclr,sizeof(mclr));
			m_pStaticDlg->m_vmClrMain.push_back(mclr);
		}
		file.Read(&m_clrBackground,sizeof(m_clrBackground));
		file.Read(&m_clrBottom,sizeof(m_clrBottom));
		for (int i = 0;i < 256; i++)
		{
			PALETTE palette;
			int n = file.Read(&palette,sizeof(palette));
			if(n == 0)
				break;
			m_pStaticDlg->m_vColor.push_back(palette.color);
			m_pStaticDlg->m_vfPercent.push_back(palette.fPercent);
			m_vnyColorbar.push_back(palette.ncolorbar);
		}
		file.Close();
		vector<int>::iterator iter;
		int nIndex = 0;
		for (iter = m_vnyColorbar.begin(); iter != m_vnyColorbar.end() - 1; iter++)
		{
// 			CBrush brush(m_pStaticDlg->m_vColor[nIndex]);
// 			m_pDC->FillRect(CRect(60, m_vnyColorbar[nIndex], 110, m_vnyColorbar[nIndex + 1]),&brush);
			ChangeColorRect(nIndex);
			nIndex++;
		}
		m_nNumRect = nIndex;
		m_nMaxAdd -= nIndex - 2;
		m_pStaticDlg->OpenLayoutControl();
	}
}

void CPaletteDlg::OnFileSave()
{
	// TODO: Add your command handler code here
	CString strExt = _T(".prt");
	CString strFilePath;
	CString strFilter;

	strFilter.Format(_T("Prt Files (*prt)|*prt|All Files (*.*)|*.*||"));
	CFileDialog dlg(FALSE, _T("prt"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		OnSequence();
		OnCountGradualColor();
		m_nMainColorNum = m_pStaticDlg->m_vmClrMain.size();
		strFilePath = dlg.GetPathName();
		if (strFilePath.Find(strExt) == -1)
		{
			strFilePath += strExt;
		}
		CFile file(strFilePath,CFile::modeCreate|CFile::modeWrite);
		int nIndex = 0;
		file.Write(&m_nMainColorNum,sizeof(m_nMainColorNum));
		vector<MCOLORR>::iterator clriter;
		for (clriter = m_pStaticDlg->m_vmClrMain.begin(); clriter != m_pStaticDlg->m_vmClrMain.end(); clriter++)
		{
			file.Write(&m_pStaticDlg->m_vmClrMain[nIndex],sizeof(m_pStaticDlg->m_vmClrMain[nIndex]));
			nIndex++;
		}
		file.Write(&m_clrBackground,sizeof(m_clrBackground));
		file.Write(&m_clrBottom,sizeof(m_clrBottom));

		nIndex = 0;
		vector<int>::iterator iter;
		for (iter = m_vnyColorbar.begin(); iter != m_vnyColorbar.end(); iter++)
		{
			PALETTE palette;
			palette.color = m_pStaticDlg->m_vColor[nIndex];
			palette.fPercent = m_pStaticDlg->m_vfPercent[nIndex];
			palette.ncolorbar = m_vnyColorbar[nIndex];
			file.Write(&palette,sizeof(palette));
			nIndex++;
		}
		file.Close();
	}
}

void CPaletteDlg::OnFileQuit()
{
	// TODO: Add your command handler code here
	m_bPlatte = FALSE;
	OnOK();
}

void CPaletteDlg::OnFileDefine()
{
	// TODO: Add your command handler code here
	m_bPlatte = TRUE;
	OnOK();
}

BOOL CPaletteDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// if there is a top level routing frame then let it handle the message
	if (GetRoutingFrame() != NULL) return FALSE;

	// to be thorough we will need to handle UNICODE versions of the message also !!
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool 
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		AfxLoadString(nID, szFullText);//#include <afxpriv.h> for AfxLoadString
		strTipText=szFullText;

#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif

		*pResult = 0;

		// bring the tooltip window above other popup windows
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); return TRUE;
	}
}

void CPaletteDlg::OnSequence()
{
	vector<MCOLORR>::iterator iteri;
	vector<MCOLORR>::iterator iterj;
	float fAid;
	for (int i = 0; i < m_pStaticDlg->m_vmClrMain.size() - 1; i++)
	{
		for (int j = 0; j < m_pStaticDlg->m_vmClrMain.size() - i - 1; j++)
		{
			if (m_pStaticDlg->m_vmClrMain[j].fPercent < m_pStaticDlg->m_vmClrMain[j + 1].fPercent)
			{
				fAid = m_pStaticDlg->m_vmClrMain[j].fPercent;
				m_pStaticDlg->m_vmClrMain[j].fPercent = m_pStaticDlg->m_vmClrMain[j + 1].fPercent;  
				m_pStaticDlg->m_vmClrMain[j + 1].fPercent = fAid;
			}
		}
	}
}

void CPaletteDlg::OnCountGradualColor()
{
	vector<MCOLORR>::iterator iter;
	int nIndex = 1;
	int nNum = 0;
	for (int i = 0; i < m_pStaticDlg->m_vmClrMain.size(); i++)
	{
		m_pStaticDlg->m_vmClrMain[i].nGradualColor = 0;
	}
	for (int i = 0; i < m_pStaticDlg->m_vfPercent.size(); i++)
	{
		if (m_pStaticDlg->m_vfPercent[i] == m_pStaticDlg->m_vmClrMain[nIndex].fPercent)
		{
			m_pStaticDlg->m_vmClrMain[nIndex - 1].nGradualColor += (nNum + 1) / 2;
			m_pStaticDlg->m_vmClrMain[nIndex].nGradualColor += (nNum + 1) - (nNum + 1) / 2;
			nIndex++;
			nNum = 0;
			continue;
		}
		nNum++;
	}
}

void CPaletteDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CPaletteDlg::OnUpdateLevelInsert(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bMenuInsert)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CPaletteDlg::OnUpdateLevelClear(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bMenuClear)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
// 
// void CPaletteDlg::OnUpdateBttonAdd(CCmdUI *pCmdUI)
// {
// 	if (m_bToolbarAdd)
// 	{
// 		pCmdUI->Enable(TRUE);
// 	}
// 	else
// 	{
// 		pCmdUI->Enable(FALSE);
// 	}
// }
// 
// void CPaletteDlg::OnUpdateBttonDelete(CCmdUI *pCmdUI)
// {
// 	if (m_bToolbarDelete)
// 	{
// 		pCmdUI->Enable(TRUE);
// 	}
// 	else
// 	{
// 		pCmdUI->Enable(FALSE);
// 	}
// }