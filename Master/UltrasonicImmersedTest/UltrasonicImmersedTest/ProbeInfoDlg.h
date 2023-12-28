#pragma once


// CProbeInfoDlg dialog

class CProbeInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProbeInfoDlg)

public:
	CProbeInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProbeInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROBE_INFO };
public:
	WORD m_wChannelNo;
	WORD m_wWorkMode;	// ����ģʽ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nProbeFrequency;
public:
	int m_nPressureType;
public:
	int m_nOscillationMode;
public:
	int m_nImpedanceType;
public:
	int m_nLvBoType;
public:
	float m_fLvBoHigh;
public:
	float m_fLvBoLow;
public:
	unsigned int m_nMaiKuan;
public:
	int m_nEnergy;//����  yachang.huang add on 2013-11-25
public:
	int m_nRevAndSendMode;//�շ�ģʽ  yachang.huang add on 2013-11-25
public:
	int m_nSpiAddr;//SPI��ַ  yachang.huang add on 2013-11-25
	CSliderCtrl m_sliderSpiData;// SPI���ݻ������  yachang.huang add on 2013-11-25
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	int m_nSpiData;//SPI���ݱ༭�����yachang.huang add on 2013-11-25
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//������������  yachang.huang add on 2013-11-25
	afx_msg void OnDeltaposSpinSpiData(NMHDR *pNMHDR, LRESULT *pResult);//  yachang.huang add on 2013-11-25
public:
	int m_nMidValue;
};
