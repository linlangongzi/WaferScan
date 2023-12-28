#pragma once
#include "listctrleditable.h"

class CChannelPanel;
class CFeatureInfoWnd;
// CTcgCurveDlg dialog

class CTcgCurveDlg : public CDialog
{
	DECLARE_DYNAMIC(CTcgCurveDlg)

public:
	CTcgCurveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTcgCurveDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TCG_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	TCG_CURVE_t m_stTcgCurve[CHANNEL_NUM];	// TCG曲线
	TCG_CURVE_t m_stTcgCurveBuDian[CHANNEL_NUM];// TCG曲线,前后各补一个点
	COLORREF m_colorTcg;					// TCG曲线颜色
	BOOL m_bMakeTcgFlag;					// 是否正在制作或修改TCG曲线
	int m_nSelectPoint;						// 选中的DAC点
	int m_nCurPointNum;						// 当位点序号
//	CChannelPanel* m_pChannelPanel;
//	CFeatureInfoWnd* m_pFeatureWnd;			// 特征窗口
public:
	afx_msg void OnBnClickedButtonTcgCreate();
public:
	afx_msg void OnBnClickedButtonTcgAddPoint();
public:
	float m_fCurrentPointRange;
public:
	float m_fCurrentPointAmp;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonPrevTcgPoint();
public:
	afx_msg void OnBnClickedButtonNextTcgPoint();
public:
	afx_msg void OnBnClickedButtonTcgDeletePoint();
public:
	afx_msg void OnBnClickedButtonTcgModifyConfirm();
public:
	BOOL m_bSwitchFlag;
public:
	afx_msg void OnBnClickedRadioTcgOn();
public:
	afx_msg void OnBnClickedRadioTcgOff();
public:
	afx_msg void OnBnClickedButtonTcgFileOpen();
public:
	afx_msg void OnBnClickedButtonTcgFileSave();
public:
	void DisplayValue(float fPlus, float fRefPlus, int nChnNo);
	void SendTcgCurve();
	void InitResultList(CListCtrlEditable* pListCtrl);
	CURVEPOINT_DATA_t GetHeightestPoint(CURVEPOINT_DATA_t stCurveForSearch[], int nPointNum);
	void CalcTcgSram();
	void UpdateList(CListCtrlEditable* pListCtrl);
	BOOL BuDian(CURVEPOINT_DATA_t stCurveToBuDian[],CURVEPOINT_DATA_t stCurveAfterBuDian[], int nPointNum);
public:
	afx_msg void OnClose();
public:
	CListCtrlEditable m_lstTcg;
	float m_fTcgSramGain[TCG_SRAM_NUM];
	int m_nAllDotNum;
	WORD m_wSamplingInterval;//采样间隔  yachang.huang add on 2014-06-18
	int m_nFrontAddDotNum;	//第一点之前增加的点
public:
	afx_msg void OnLvnEndlabeleditListTcg(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnBeginlabeleditListTcg(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnHdnItemchangedListTcg(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedButtonTcgSramModify();
// wangbingfu add start at 2012-11-19
public:
	int m_nTcgOnMode;
	float CalcNewAmp(float fAmp, float fRange);
	afx_msg void OnBnClickedRadioTcgSoft();
	afx_msg void OnBnClickedRadioHardware();
// wangbingfu add end at 2012-11-20
};
