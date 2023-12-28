#pragma once

class CpictureNote;
// CEditNoteDlg dialog

class CEditNoteDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditNoteDlg)

public:
	CEditNoteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditNoteDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	CString strPictureNote;
	CWnd* m_pParent;
	NOTE_DATA_t m_stNoteData;
	CpictureNote* m_pictureNote;
};
