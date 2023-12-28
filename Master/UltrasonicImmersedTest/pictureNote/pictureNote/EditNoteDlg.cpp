// EditNoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pictureNote.h"
#include "EditNoteDlg.h"


// CEditNoteDlg dialog

IMPLEMENT_DYNAMIC(CEditNoteDlg, CDialog)

CEditNoteDlg::CEditNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditNoteDlg::IDD, pParent)
	, strPictureNote(_T(""))
{

}

CEditNoteDlg::~CEditNoteDlg()
{
}

void CEditNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NOTE, strPictureNote);
}


BEGIN_MESSAGE_MAP(CEditNoteDlg, CDialog)
	
	ON_BN_CLICKED(IDOK, &CEditNoteDlg::OnBnClickedOk)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


void CEditNoteDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_pictureNote.m_stNoteData.strNote = strPictureNote;
	g_pictureNote.m_stNoteData.nStrLength = strPictureNote.GetLength();
	OnOK();
}

void CEditNoteDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox(_T("向服务器发送数据失败！"), MB_OK);
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}