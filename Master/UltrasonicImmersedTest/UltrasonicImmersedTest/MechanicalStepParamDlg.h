#pragma once


// CMechanicalStepParamDlg �Ի���

class CMechanicalStepParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CMechanicalStepParamDlg)

public:
	CMechanicalStepParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMechanicalStepParamDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETSTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// x�ᵥ���˶�����ֵ
	float m_fXStep;
public:
	// y�ᵥ���˶�����
	float m_fYStep;
public:
	// z�ᵥ���˶�����
	float m_fZStep;
public:
	afx_msg void OnBnClickedOk();
public:
	float m_fABStep;		// A�ᵥ���˶�����
	float m_fWStep;		// W�ᵥ���˶�����
	float m_fRStep;		// R�ᵥ���˶�����
};
