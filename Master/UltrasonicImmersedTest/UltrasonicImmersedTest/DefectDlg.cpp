/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 缺陷标注实现窗口
* Filename: DefectDlg.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2012-11-21
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "DefectDlg.h"
#include "GlobalFunction.h"

// CDefectDlg dialog

IMPLEMENT_DYNAMIC(CDefectDlg, CDialog)

CDefectDlg::CDefectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDefectDlg::IDD, pParent)
	, m_dMinArea(0)
	, m_dAreaA(0)
	, m_dAreaB(0)
{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::CDefectDlg"),"Enter");
	m_nFontSize = 0;
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::CDefectDlg"),"Leave");
}

CDefectDlg::~CDefectDlg()
{
}

void CDefectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BADFLAG, m_bBadFlag);
	DDX_Control(pDX, IDC_COMBO_SYMBOL1, m_cmbSymbolSel1);
	DDX_Control(pDX, IDC_COMBO_SYMBOL2, m_cmbSymbolSel2);
	DDX_Control(pDX, IDC_COMBO_SYMBOL3, m_cmbSymbolSel3);
	DDX_Check(pDX, IDC_CHECK_DEFINE1, m_bCheckFlag1);
	DDX_Check(pDX, IDC_CHECK_DEFINE2, m_bCheckFlag2);
	DDX_Check(pDX, IDC_CHECK_DEFINE3, m_bCheckFlag3);
	DDX_Text(pDX, IDC_EDIT_MIN_AREA, m_dMinArea);
	DDX_Text(pDX, IDC_EDIT_BADMIN, m_dAreaA);
	DDX_Text(pDX, IDC_EDIT_BADMAX, m_dAreaB);
}


BEGIN_MESSAGE_MAP(CDefectDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_BADFLAG, &CDefectDlg::OnBnClickedCheckBadflag)
	ON_BN_CLICKED(IDOK, &CDefectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BADFONT, &CDefectDlg::OnBnClickedButtonBadfont)
	ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL1, &CDefectDlg::OnCbnSelchangeComboSymbol1)
	ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL2, &CDefectDlg::OnCbnSelchangeComboSymbol2)
	ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL3, &CDefectDlg::OnCbnSelchangeComboSymbol3)
	ON_BN_CLICKED(IDCANCEL, &CDefectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_DEFINE1, &CDefectDlg::OnBnClickedCheckDefine1)
	ON_BN_CLICKED(IDC_CHECK_DEFINE2, &CDefectDlg::OnBnClickedCheckDefine2)
	ON_BN_CLICKED(IDC_CHECK_DEFINE3, &CDefectDlg::OnBnClickedCheckDefine3)
END_MESSAGE_MAP()


// CDefectDlg message handlers

void CDefectDlg::OnBnClickedCheckBadflag()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedCheckBadflag"),"Enter");

	UpdateData(TRUE);
	if(m_bBadFlag)
	{
		m_nDefectEnable = 1;
	}
	else
	{
		m_nDefectEnable = 0;
	}
	UpdateCtrlStatus();

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedCheckBadflag"),"Leave");
}
BOOL CDefectDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnInitDialog"),"Enter");
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon

	m_csSymbolSetFilePath = GetSetFilePath() + _T("SetSymbolDlg.ini");//取得配置文件路径
	GetDataFromSetFile(m_csSymbolSetFilePath);

	UpdateMapDrawSymbol();

	if (m_csfont.IsEmpty())
	{
		m_csfont = "@宋体";
	}
	if (0 == m_nFontSize)
	{
		m_nFontSize = 10;
	}
	CString str;
	str.Format(_T("%s%s%d"),m_csfont,_T("  "),m_nFontSize);
	GetDlgItem(IDC_BUTTON_BADFONT)->SetWindowText(str);
	if (m_nDefectEnable)
	{
		m_bBadFlag = TRUE;
	}
	else
	{
		m_bBadFlag = FALSE;
	}
	UpdateCtrlStatus();

	UpdateCheckBoxSelect();

// 	if (m_nDefectEnable)//初期化时，如果标注使能打开，并且图形没有选择项，则默认选择第一个。这种情况只有用户手动改配置文件才会出现，在界面端已经防止了此种情况发生
// 	{
// 		if(!m_nSymbol1Enable && !m_nSymbol2Enable && !m_nSymbol3Enable)
// 		{
// 			m_bCheckFlag1 = TRUE;
// 			m_nSymbol1Enable = 1;
// 		}
// 	}
	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnInitDialog"),"Leave");	
	return TRUE;
}
void CDefectDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedOk"),"Enter");
	UpdateData(TRUE);
	if (m_bBadFlag)
	{
		CString text;
		GetDlgItemText(IDC_EDIT_BADMIN,text);
		if (text.IsEmpty() || m_dAreaA == 0)
		{
			MessageBox(_T("分段面积值A为空或为零，请再次确认！"), _T("警告"), MB_OK|MB_ICONWARNING);
			return;
		}
		GetDlgItemText(IDC_EDIT_BADMAX,text);
		if (text.IsEmpty() || m_dAreaB == 0)
		{
			MessageBox(_T("分段面积值B为空或为零，请再次确认！"), _T("警告"), MB_OK|MB_ICONWARNING);
			return;
		}
		if (!m_bCheckFlag1 && !m_bCheckFlag2 && !m_bCheckFlag3)
		{
			MessageBox(_T("没有选择标注范围，请再次确认！"), _T("警告"), MB_OK|MB_ICONWARNING);
			return;
		}
	}
	WriteDataToSetFile();
	OnOK();	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedOk"),"Leave");	
}

void CDefectDlg::OnBnClickedButtonBadfont()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedButtonBadfont"),"Enter");
	LOGFONT lf;
	CFont   cf;
	CClientDC dc(this);
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(m_nFontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);// 设置字号//MSDC上建议这样设置
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	lf.lfUnderline = m_nlfUnderline;
	lf.lfStrikeOut = m_nlfStrikeOut;
	lf.lfCharSet = GB2312_CHARSET;//ANSI_CHARSET
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	//下面一句代码只有当工程是用UNICODE编译时才可用。m_csfont为CSTRING类型，因为须要将这个信息存储到配置文件中，所以用CSTRING操作更容易
	wcscpy_s(lf.lfFaceName,CT2CW(m_csfont));

	CFontDialog dlg(&lf);
	//dlg.m_cf.Flags = CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS|CF_EFFECTS;   //用于设置窗体样式，去掉CF_EFFECTS属性则不能设置下划线和颜色
	dlg.m_cf.rgbColors = m_FontColor;//默认字体颜色
	if(dlg.DoModal()==IDOK)
	{
		CString str;
		dlg.GetCurrentFont(&lf);
		m_nFontSize = dlg.GetSize()/10;
		m_csfont = lf.lfFaceName;
		if (m_csfont == _T(""))
		{
			m_csfont = _T("@宋体");
		}
		if (0 == m_nFontSize)
		{
			m_nFontSize = 10;
		}
		
		str.Format(_T("%s%s%d"),m_csfont,_T("  "),m_nFontSize);
		GetDlgItem(IDC_BUTTON_BADFONT)->SetWindowText(str);

		m_csFontColor.Format(_T("%d%s%d%s%d"),GetRValue(dlg.GetColor()),_T(","),GetGValue(dlg.GetColor()),_T(","),GetBValue(dlg.GetColor()));

		if (m_csFontColor.IsEmpty())
		{
			m_csFontColor = _T("RGB(255,0,0)");
		}

		//取得下划线信息
		if (lf.lfUnderline)
		{
			m_nlfUnderline = 1;
		}
		else
		{
			m_nlfUnderline = 0;
		}
		//删除线
		if (lf.lfStrikeOut)
		{
			m_nlfStrikeOut = 1;
		}
		else
		{
			m_nlfStrikeOut = 0;
		}

//		CDC *pDC = AfxGetApp()->m_pMainWnd->GetDC();
// 		pDC->SetTextColor();
// 		cf.CreateFontIndirect(&lf);
// 		CFont *def_font=pDC->SelectObject(&cf);
// 		pDC->TextOut(point.x,point.y,"钓鱼岛是中国的");
// 		pDC->SelectObject(def_font);
// 		cf.DeleteObject();
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnBnClickedButtonBadfont"),"Leave");
	}
}
void CDefectDlg::UpdateMapDrawSymbol()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::UpdateMapDrawSymbol"),"Enter");
	// TODO: Add your control notification handler code here
	m_cmbSymbolSel1.SetCurSel(m_nSymbol1);
	m_cmbSymbolSel2.SetCurSel(m_nSymbol2);
	m_cmbSymbolSel3.SetCurSel(m_nSymbol3);
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::UpdateMapDrawSymbol"),"Leave");
}
void CDefectDlg::OnCbnSelchangeComboSymbol1()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnCbnSelchangeComboSymbol1"),"Enter");
	UpdateData(TRUE);
	switch(m_cmbSymbolSel1.GetCurSel())
	{
	case 0:
		{
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 0;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 0;
// 			}
			m_nSymbol1 = 0;
			break;
		}
	case 1:
		{
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 1;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 1;
// 			}
			m_nSymbol1 = 1;
			break;
		}
	case 2:
		{
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 2;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 2;
// 			}
			m_nSymbol1 = 2;
			break;
		}	
	default:
		break;
	}
	m_cmbSymbolSel1.SetCurSel(m_nSymbol1);
	UpdateData(FALSE);
	//UpdateMapDrawSymbol();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnCbnSelchangeComboSymbol1"),"Leave");
}

void CDefectDlg::OnCbnSelchangeComboSymbol2()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnCbnSelchangeComboSymbol2"),"Enter");
	UpdateData(TRUE);
	switch(m_cmbSymbolSel2.GetCurSel())
	{
	case 0:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 0;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 0;
// 			}
			m_nSymbol2 = 0;
			break;
		}
	case 1:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 1;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 1;
// 			}
// 			m_nSymbol2 = 1;
			break;
		}
	case 2:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 2;
// 			}
// 			if (m_bCheckFlag3)
// 			{
// 				m_nSymbol3 = 2;
// 			}
			m_nSymbol2 = 2;
			break;
		}	
	default:
		break;
	}
	m_cmbSymbolSel2.SetCurSel(m_nSymbol2);
	UpdateData(FALSE);
	//UpdateMapDrawSymbol();
	WriteDataToSetFile();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDefectDlg::OnCbnSelchangeComboSymbol2"),"Leave");	
}

void CDefectDlg::OnCbnSelchangeComboSymbol3()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCbnSelchangeComboSymbol3"),"Enter");
	UpdateData(TRUE);
	switch(m_cmbSymbolSel3.GetCurSel())
	{
	case 0:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 0;
// 			}
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 0;
// 			}
			m_nSymbol3 = 0;
			break;
		}
	case 1:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 1;
// 			}
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 1;
// 			}
			m_nSymbol3 = 1;
			break;
		}
	case 2:
		{
// 			if (m_bCheckFlag1)
// 			{
// 				m_nSymbol1 = 2;
// 			}
// 			if (m_bCheckFlag2)
// 			{
// 				m_nSymbol2 = 2;
// 			}
			m_nSymbol3 = 2;
			break;
		}	
	default:
		break;
	}
	m_cmbSymbolSel2.SetCurSel(m_nSymbol2);
	UpdateData(FALSE);
	//UpdateMapDrawSymbol();
	WriteDataToSetFile();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnCbnSelchangeComboSymbol3"),"Leave");
}
CString CDefectDlg::GetSetFilePath()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetSetFilePath"),"Enter");
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString strFileName = CString(strFolder);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetSetFilePath"),"Leave");
	return strFileName;
}
void CDefectDlg::GetDataFromSetFile(CString csFilePath)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetDataFromSetFile"),"Enter");
	CString strSection = _T("DefineSymbol");
	CString strKey;
	strKey = _T("Symbol1");
	m_nSymbol1 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("Symbol2");
	m_nSymbol2 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("Symbol3");
	m_nSymbol3 = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("Symbol1Enable");
	m_nSymbol1Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("Symbol2Enable");
	m_nSymbol2Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("Symbol3Enable");
	m_nSymbol3Enable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strSection = _T("AreaSet");
	strKey = _T("AreaA");
	m_dAreaA = 	GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("AreaB");
	m_dAreaB = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strSection = _T("DefectEnable");
	strKey = _T("Enable");
	m_nDefectEnable = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strSection = _T("FontInFo");
	strKey = _T("Font");
	GetPrivateProfileString(strSection,strKey,NULL,m_csfont.GetBuffer(255),255,csFilePath);
	m_csfont.ReleaseBuffer();

	strKey = _T("FontSize");
	m_nFontSize = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	CString csFontColor;
	strKey = _T("FontColor");
	GetPrivateProfileString(strSection,strKey,NULL,csFontColor.GetBuffer(255),255,csFilePath);
	csFontColor.ReleaseBuffer();

	if (csFontColor.IsEmpty())
	{
		m_FontColor = RGB(255,0,0);		
	}
	else
	{
		CString str,strTok;
		int curPos= 0;
		int colorR = 0;
		int colorG = 0;
		int colorB = 0;
		str = csFontColor.Tokenize(_T(","),curPos);
		colorR = _ttoi(str);

		str = csFontColor.Tokenize(_T(","),curPos);
		colorG = _ttoi(str);

		str = csFontColor.Tokenize(_T(","),curPos);
		colorB = _ttoi(str);
		m_FontColor = RGB(colorR,colorG,colorB);
	}

	strSection = _T("FontInFo");
	strKey = _T("UnderLine");
	m_nlfUnderline = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	strKey = _T("DeleteLine");
	m_nlfStrikeOut = GetPrivateProfileInt(strSection,strKey, NULL, csFilePath);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::GetDataFromSetFile"),"Leave");
}
void CDefectDlg::WriteDataToSetFile()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::WriteDataToSetFile"),"Enter");
	CString cstrKey;
	CString strValue;
	CString cstr = _T("");
	CString strSection = _T("");

	//如果文件为只读的，将只读属性去掉
	SetFileAttributes(m_csSymbolSetFilePath,GetFileAttributes(m_csSymbolSetFilePath) & ~FILE_ATTRIBUTE_READONLY);

	//使能打开或不打开，只要是按了确定键，就将使能存入配置文件中
	strSection = _T("DefectEnable");
	strValue = _T("");
	strValue.Format(_T("%d"),m_nDefectEnable);
	WritePrivateProfileString(strSection,_T("Enable"),strValue,m_csSymbolSetFilePath);

	if (m_bBadFlag)
	{
		strSection = _T("DefineSymbol");
		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol1);
		WritePrivateProfileString(strSection,_T("Symbol1"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol2);
		WritePrivateProfileString(strSection,_T("Symbol2"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol3);
		WritePrivateProfileString(strSection,_T("Symbol3"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol1Enable);
		WritePrivateProfileString(strSection,_T("Symbol1Enable"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol2Enable);
		WritePrivateProfileString(strSection,_T("Symbol2Enable"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nSymbol3Enable);
		WritePrivateProfileString(strSection,_T("Symbol3Enable"),strValue,m_csSymbolSetFilePath);

		strSection =_T("AreaSet");
		strValue = _T("");
		strValue.Format(_T("%f"),m_dMinArea);
		WritePrivateProfileString(strSection,_T("Min"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%f"),m_dAreaA);
		WritePrivateProfileString(strSection,_T("AreaA"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%f"),m_dAreaB);
		WritePrivateProfileString(strSection,_T("AreaB"),strValue,m_csSymbolSetFilePath);

		strSection =_T("FontInFo");
		WritePrivateProfileString(strSection,_T("Font"),m_csfont,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nFontSize);
		WritePrivateProfileString(strSection,_T("FontSize"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		//strValue.Format(_T("%d"),m_csFontColor);
		WritePrivateProfileString(strSection,_T("FontColor"),m_csFontColor,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nlfUnderline);
		WritePrivateProfileString(strSection,_T("UnderLine"),strValue,m_csSymbolSetFilePath);

		strValue = _T("");
		strValue.Format(_T("%d"),m_nlfStrikeOut);
		WritePrivateProfileString(strSection,_T("DeleteLine"),strValue,m_csSymbolSetFilePath);
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::WriteDataToSetFile"),"Leave");
}
void CDefectDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnBnClickedCancel"),"Enter");
	OnCancel();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanView::OnBnClickedCancel"),"Leave");
}
void CDefectDlg::UpdateCheckBoxSelect()
{
	if (m_nSymbol1Enable)
	{
		m_bCheckFlag1 = TRUE;
	}
	else
	{
		m_bCheckFlag1 = FALSE;
	}
	if (m_nSymbol2Enable)
	{
		m_bCheckFlag2 = TRUE;
	}
	else
	{
		m_bCheckFlag2 = FALSE;
	}
	if (m_nSymbol3Enable)
	{
		m_bCheckFlag3 = TRUE;
	}
	else
	{
		m_bCheckFlag3 = FALSE;
	}
}
void CDefectDlg::UpdateCtrlStatus()
{
	if (m_nDefectEnable)
	{
		GetDlgItem(IDC_BUTTON_BADFONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_AREA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BADMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BADMAX)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEFINE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYMBOL1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEFINE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYMBOL2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEFINE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SYMBOL3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BADFONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_AREA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BADMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BADMAX)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEFINE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYMBOL1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEFINE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYMBOL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEFINE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SYMBOL3)->EnableWindow(FALSE);
	}
}
void CDefectDlg::OnBnClickedCheckDefine1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckFlag1)
	{
		m_nSymbol1Enable = 1;
	}
	else
	{
		m_nSymbol1Enable = 0;
// 		if (!m_bCheckFlag2 && !m_bCheckFlag3)//同时要判断另外两个是否也为FALSE，如果一个也未选择，则将缺陷标注使能关闭
// 		{
// 			m_bBadFlag = FALSE;//使能关闭
// 			m_nDefectEnable = 0;
// 			UpdateCtrlStatus();
// 			UpdateData(FALSE);
// 		}
	}
}

void CDefectDlg::OnBnClickedCheckDefine2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckFlag2)
	{
		m_nSymbol2Enable = 1;
	}
	else
	{
		m_nSymbol2Enable = 0;
// 		if (!m_bCheckFlag1 && !m_bCheckFlag3)
// 		{
// 			m_bBadFlag = FALSE;
// 			m_nDefectEnable = 0;
// 			UpdateCtrlStatus();
// 			UpdateData(FALSE);
// 		}
	}
}

void CDefectDlg::OnBnClickedCheckDefine3()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckFlag3)
	{
		m_nSymbol3Enable = 1;
	}
	else
	{
		m_nSymbol3Enable = 0;
// 		if (!m_bCheckFlag1 && !m_bCheckFlag2)
// 		{
// 			m_bBadFlag = FALSE;
// 			m_nDefectEnable = 0;
// 			UpdateCtrlStatus();
// 			UpdateData(FALSE);
// 		}
	}
}
