#pragma once


// CProbeLocationDlg dialog

class CProbeLocationDlg : public CDialog
{
	DECLARE_DYNAMIC(CProbeLocationDlg)

public:
	CProbeLocationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProbeLocationDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROBE_LOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fX1;
	float m_fX2;
	float m_fX3;
	float m_fX4;
	float m_fX5;
	float m_fX6;
	float m_fX7;
	float m_fX8;

	float m_fY1;
	float m_fY2;
	float m_fY3;
	float m_fY4;
	float m_fY5;
	float m_fY6;
	float m_fY7;
	float m_fY8;

	float m_fZ1;
	float m_fZ2;
	float m_fZ3;
	float m_fZ4;
	float m_fZ5;
	float m_fZ6;
	float m_fZ7;
	float m_fZ8;
};
