#pragma once
#include "afxwin.h"
#include "NumEdit.h"


// COriginDlg �Ի���
class CCScanParamPanel;
class COriginDlg : public CDialog
{
	DECLARE_DYNAMIC(COriginDlg)

public:
	COriginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COriginDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ORIGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	double m_offset_x;
	double m_offset_y;
public:
	CCScanParamPanel* m_pParentWnd;
private:
	CNumEdit m_NumEditX;   //yachang.huang  add on 2012-11-19
	CNumEdit m_NumEditY;   //yachang.huang  add on 2012-11-19
};
