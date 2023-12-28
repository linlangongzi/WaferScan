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
	TCG_CURVE_t m_stTcgCurve[CHANNEL_NUM];	// TCG����
	TCG_CURVE_t m_stTcgCurveBuDian[CHANNEL_NUM];// TCG����,ǰ�����һ����
	COLORREF m_colorTcg;					// TCG������ɫ
	BOOL m_bMakeTcgFlag;					// �Ƿ������������޸�TCG����
	int m_nSelectPoint;						// ѡ�е�DAC��
	int m_nCurPointNum;						// ��λ�����
//	CChannelPanel* m_pChannelPanel;
//	CFeatureInfoWnd* m_pFeatureWnd;			// ��������
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
	WORD m_wSamplingInterval;//�������  yachang.huang add on 2014-06-18
	int m_nFrontAddDotNum;	//��һ��֮ǰ���ӵĵ�
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
