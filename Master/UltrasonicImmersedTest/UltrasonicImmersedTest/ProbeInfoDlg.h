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
	WORD m_wWorkMode;	// 工作模式
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
	int m_nEnergy;//能量  yachang.huang add on 2013-11-25
public:
	int m_nRevAndSendMode;//收发模式  yachang.huang add on 2013-11-25
public:
	int m_nSpiAddr;//SPI地址  yachang.huang add on 2013-11-25
	CSliderCtrl m_sliderSpiData;// SPI数据滑块变量  yachang.huang add on 2013-11-25
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	int m_nSpiData;//SPI数据编辑框变量yachang.huang add on 2013-11-25
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);//滚动条处理函数  yachang.huang add on 2013-11-25
	afx_msg void OnDeltaposSpinSpiData(NMHDR *pNMHDR, LRESULT *pResult);//  yachang.huang add on 2013-11-25
public:
	int m_nMidValue;
};
