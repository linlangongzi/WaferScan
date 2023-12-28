// CScanParamPanel .h : ʵ���ļ�
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* Cɨ������崰����ͼͷ�ļ�
*Filename : CScanParamPanel.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-27 
*/

#pragma once
#include "platedrawctrl.h"
#include "afxcmn.h"
#include "afxwin.h"
//machao add start on 2013-07-25
#include <vector>
using namespace std;
//machao add start on 2013-07-25

class CUltrasonicImmersedTestDoc;
class CCScanView;
class CUltrasonicImmersedTestView;
class CCScanFileHeader;
class CSurfaceDetailDlg;
class CHanHeRateDlg;
class CMessageDlg;
// WangQianfeng add start on 2012-11-21
class CDefectDlg;
// WangQianfeng add end on 2012-11-21
// CCScanParamPanel ������ͼ

class CCScanParamPanel : public CFormView
{
	DECLARE_DYNCREATE(CCScanParamPanel)

protected:
	CCScanParamPanel();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCScanParamPanel();

public:
	enum { IDD = IDD_CSCANPARAM_PANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);
	void DrawGradientV( CDC* pDC,COLORREF co1,COLORREF co2,CRect& DrawRect );
	void DrawGradientV( CDC* pDC,float nGrayBegin,float nGrayEnd,CRect& DrawRect );

	BOOL SavePicByPath( CString p_path, BOOL p_flag );
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
protected:

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
public:
	COLORREF m_colorBegin;
	COLORREF m_colorEnd;
	
	float m_fBeginGrayValue;
	float m_fEndGrayValue;
//	afx_msg void OnBnClickedButtonBeginColor();	//machao delete on 2013-07-12
	void DrawRule(CDC* pDC,float fStart, float fEnd);//���̶ȳ�
//machao delete start on 2013-07-12
//public:
//	afx_msg void OnBnClickedButtonEndColor();
//machao delete end on 2013-07-12
public:
	afx_msg void OnBnClickedRadioDeepdraw();
public:
	afx_msg void OnBnClickedRadioRangedraw();
public:
	// ����ʽ
	BOOL m_nDrawType;	// 0:���ȳ��� 1:��ȳ��� 2:���ٳ��� 3:���迹���� 4:��˥������ 5:�ܶȳ���
	float m_fMinRule;//�̶ȳ���С�̶�
	float m_fMaxRule;//�̶ȳ����̶�
public:
	BOOL	m_bLianXuShuJu;	//�������ݱ�־
	
	double	m_dGatedelay[4];//�ӳ�
	double	m_dGatedtThresh[4];//�����ֵ
	BOOL	m_bGateon[4];//�ĸ��ŵĿ��أ�ò�Ƶ�����ľ�û������
	float	m_uvm;//���ϵ�Ԥ������
//	FeaturesData *pSortData;//ָ���������ݵ�ָ���ָ��
//	CList	<FeaturesData,FeaturesData> FeaturesFilterList;//���ݹ��˺�����
public:
	afx_msg void OnBnClickedButtonReadFile();
	afx_msg void OnBnClickedButtonSurfaceCalc();
	afx_msg void OnBnClickedButtonSurfaceDetail();
public:
	CPlatedrawctrl m_PlateDrawCtrl;	
public:
	// machao delete start on 2013-08-14
//	CCScanView* m_pCScanView;
//	CCScanView* m_pCScanView2;
	// machao delete end on 2013-08-14
	CCScanView* m_pCurCScanView;
	CUltrasonicImmersedTestView* m_pAScanView;
public:
	float m_fThick;			// �������
	int m_nZoomScale;		// �����Ŵ����
public:
	//machao delete start on 2013-07-12
//	afx_msg void OnBnClickedRadioColor();
//	afx_msg void OnBnClickedRadioGray();	
	//machao delete end on 2013-07-12
	afx_msg void OnBnClickedRadioMaxValue();
	afx_msg void OnBnClickedRadioMinValue();
	afx_msg void OnBnClickedRadioAvgValue();	
	afx_msg void OnBnClickedRadioGate1();
	afx_msg void OnBnClickedRadioGate2();
	afx_msg void OnBnClickedRadioGate3();
	afx_msg void OnBnClickedRadioGate4();
	afx_msg void OnBnClickedButtonDrawPicture();
public:
	int m_nColorMode;	
	int m_nValueMode;			// ȡֵģʽ	
	int m_nGateIndex;			// ѡ���բ��	
	float m_fBoundMin;			// ��ʾ��Χ����	
	float m_fBoundMax;			// ��ʾ��Χ����	
	float m_fGate1Threshold;	// Cɨբ��1��ֵ	
	float m_fGate2Threshold;	// Cɨբ��2ֵ	
	float m_fGate3Threshold;	// Cɨբ��3��ֵ	
	float m_fGate4Threshold;	// Cɨբ��1��ֵ
	//Machao add start on 2013-06-27
	BOOL m_bChangeOrigin;			// �������ԭ��
	afx_msg void OnBnClickedCheckOrigin();
	afx_msg void OnBnRadioXmodeAngle();
	afx_msg void OnBnRadioXmodePerimeter();
	afx_msg void OnBnRadioGateAbove();
	afx_msg void OnBnRadioGateBelow();
	//Machao add end on 2013-06-27
public:
	afx_msg void OnBnClickedButtonCscanRealtime();
public:
	afx_msg void OnBnClickedButtonStopCscan();
public:
	void Rgb2Hsv(float R, float G, float B, float& H, float& S, float&V);
	void Hsv2Rgb(float H, float S, float V, float &R, float &G, float &B);
	int IsShowProbe();
	void CleanSurfaceDisplay();
public:
	// ��ֵ����ѡ��
	int m_nAmpFeatureIndex;
public:
	afx_msg void OnBnClickedRadioMaxAmp();
public:
	afx_msg void OnBnClickedRadioFirstAmp();
public:
	// ʱ������ѡ��
	int m_nTimeFeatureIndex;
public:
	afx_msg void OnBnClickedRadioMaxTime();
public:
	afx_msg void OnBnClickedRadioMaxThresholTime();
public:
	afx_msg void OnBnClickedRadioFirstTime();
public:
	afx_msg void OnBnClickedRadioFirstThresholdTime();
public:
	afx_msg void OnBnClickedButtonSaveCscanPicture();
public:
	afx_msg void OnBnClickedButtonCscanParamSave();
public:
	afx_msg void OnBnClickedButtonCscanParamRead();
public:
	//�ؼ�ѡ������
	void OneItemSetCheck(int nID, BOOL bFlag);
//	void AllColorItemSetCheck(BOOL bFlag);	//machao delete on 2013-07-12
	void AllDrawModeItemSetCheck(BOOL bFlag);
	void AllGateSelectItemSetCheck(BOOL bFlag);
	void AllValueModeItemSetCheck(BOOL bFlag);
	void AllAmpSelectItemSetCheck(BOOL bFlag);
	void AllTimeSelectItemSetCheck(BOOL bFlag);
	void AllItemSetCheck(BOOL bFlag);
	//�ؼ�ʹ������
	void OneItemSwitch(int nID, BOOL bFlag);
//	void AllColorItemSwitch(BOOL bFlag);	//machao delete on 2013-07-12
	void AllDrawModeItemSwitch(BOOL bFlag);
	void AllGateSelectItemSwitch(BOOL bFlag);
	void AllValueModeItemSwitch(BOOL bFlag);
	void AllAmpSelectItemSwitch(BOOL bFlag);
	void AllTimeSelectItemSwitch(BOOL bFlag);
	void AllItemSwitch(BOOL bFlag);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
// WangQianfeng delete start on 2013-06-28
public:
	CSliderCtrl m_sliderBScanOrigin;//Bɨ���
	float m_fBScanOrigin;
	CSliderCtrl m_sliderBScanWidth;//Bɨ��ʾ���
	float m_fBScanWidth;
public:
	afx_msg void OnEnChangeEditThick();
// WangQianfeng delete end on 2013-06-28
public:
	// ����ͳ��
	BOOL m_bSurfaceFlag;
	int m_nSurfceColorMode;	//���ɫ��ģʽ
public:
	afx_msg void OnBnClickedCheckSurfaceStatistics();
public:
	afx_msg void OnEnKillfocusEditBoundMin();
public:
	afx_msg void OnEnKillfocusEditBoundMax();
public:
	afx_msg void OnEnKillfocusEditGate1Threshold();
public:
	afx_msg void OnEnKillfocusEditGate2Threshold();
public:
	afx_msg void OnEnKillfocusEditGate3Threshold();
public:
	afx_msg void OnEnKillfocusEditGate4Threshold();
	afx_msg void OnBnClickedButtonSaveData();
public:
	// ����ֵ����
	int m_nFeatureIndex;	// ����ģʽ
public:
	static int m_nObjectCount;
	int m_nObjectNum;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strPicturePath;
public:
	// ͼƬ����
	CComboBox m_cmbPictureMode;
public:
	int m_nPictureModeIndex;
public:
	int m_nPipeXMode;
public:
	void SetCScanDrawPicturePara(CCScanView* pCScanView, CCScanFileHeader fileHeader, int nGateNum[], int nFeatureNum);
public:
	void DefaultColor1();	// machao add on 2013-09-12
	afx_msg void OnBnClickedButtonDefaultColor1();
public:
	afx_msg void OnBnClickedButtonDefaultColor2();
	afx_msg void OnBnClickedButtonDefaultColor3();	//machao add on 2013-08-5
public:
	afx_msg void OnBnClickedRadioVelocityPicture();
public:
	afx_msg void OnBnClickedRadioVelocityReducePicture();
public:
	afx_msg void OnBnClickedRadioSzkPicture();
public:
	afx_msg void OnBnClickedRadioMiduPicture();
public:
	afx_msg void OnBnClickedButtonCaculateVelocity();
public:
	afx_msg void OnBnClickedButtonCalcuateShuaijian();
public:
	afx_msg void OnBnClickedButtonCaculateSzk();
public:
	afx_msg void OnBnClickedButtonCaculateMidu();
public:
	double m_fAveVelocity;
public:
	double m_fAveShuaiJian;
public:
	double m_fAveSzk;
public:
	double m_fAveMidu;

public:
	float m_fMinSurfaceArea;	//��С���
	int m_nColorDefine;		//��ɫ���� 0=�Զ��壻1=Ĭ����ɫ
	float m_fLogicalFromX;
	float m_fLogicalToX;
	float m_fLogicalFromY;
	float m_fLogicalToY;
public:
	CSurfaceDetailDlg *m_pSurfaceDeatailDlg;
	CHanHeRateDlg *m_pSHanHeRateDlg;
public:
	afx_msg void OnBnClickedButtonHanheRate();
public:
	afx_msg void OnBnClickedButtonHanheCaculate();
public:
	bool m_bCscanSync;
public:
	afx_msg void OnBnClickedCheckCscanSync();
public:
	CList<FlawNode, FlawNode&> m_lstSurfaceDetail;
	int m_nSurfaceType;	// �����˵���״����
	BOOL IsInEllipse(const CFeatureInfo& feature, const float& fXmin, const float& fXMax,
		const float& fYMin, const float& fYMax);
	BOOL IsInCircle(const CFeatureInfo& feature, const float& fXmin, const float& fXMax,
		const float& fYMin, const float& fYMax);

	BOOL IsInPolygon(const CFeatureInfo& feature, LOGICAL_POINT_t* ptPolygon, const int& nCount);
public:
	afx_msg void OnBnClickedRadioRect();
public:
	afx_msg void OnBnClickedRadioCircle();
public:
	afx_msg void OnBnClickedRadioEllipse();
public:
	BOOL m_bDisplayProbe;
public:
	afx_msg void OnBnClickedRadioPolygon();
	afx_msg void OnBnClickedCheckDisplayProbe();
public:
	afx_msg void OnEnChangeEditBoundMin();
public:
	afx_msg void OnEnChangeEditBoundMax();
public:
	CMessageDlg* m_pMessageDlg;
	CString m_filename;
	void ReadFile();
	void SurfaceCalc();
	BOOL bHanHeFlag;	//������ϳ���
public:
	afx_msg void OnBnClickedButtonOrigin();
//Machao delete start on 2013-07-3
// public:
// 	afx_msg void OnBnClickedButtonDefColor0();
// public:
// 	afx_msg void OnBnClickedButtonDefColor1();
// public:
// 	afx_msg void OnBnClickedButtonDefColor2();
// public:
// 	afx_msg void OnBnClickedButtonDefColor3();
// public:
// 	afx_msg void OnBnClickedButtonDefColor4();
// public:
// 	afx_msg void OnBnClickedButtonDefColor5();
//Machao delete end on 2013-07-3
public:
	COLORREF m_colrDefine0;
	COLORREF m_colrDefine1;
	COLORREF m_colrDefine2;
	COLORREF m_colrDefine3;
	COLORREF m_colrDefine4;
	COLORREF m_colrDefine5;
public:
	BOOL m_bNoteFlag;
public:
	afx_msg void OnBnClickedCheckZhushi();
public:
	int m_nNoteType;
public:
	afx_msg void OnBnClickedRadioZhushiRect();
public:
	afx_msg void OnBnClickedRadioZhushiCircle();
public:
	afx_msg void OnBnClickedRadioZhushiEllipse();
	int m_nGateType;
// WangQianfeng add start on 2012-11-21
public:
	afx_msg void OnBnClickedDefect();
	afx_msg void OnBnClickedRealScale();	// Song Chenguang add on 2013-07-15
// WangQianfeng add end on 2012-11-21
	// wangbingfu add start on 2013-01-09
	CString m_csPipeSetPath; 
	void ReadPipeIni(const CString& csFilePath);
	void WritePipeIni(const CString& csFilePath);
	// wangbingfu add end on 2013-01-09
	void SaveCScanPara(CString strPath);
	void ReadCScanPara(CString strPath, const BOOL& bTaskRun);
public:
	int m_nSurfaceSelType;	// ͳ��������  Song Chenguang add on 2013-06-20
	CListCtrl m_lstctrlCustumFlaw;
	BOOL m_bRealScale;		// ʵ�ʱ�����ʾ��־  Song Chenguang add on 2013-07-10
	//Machao add start on 2013-07-1
public:
	vector<int> m_vnMainColorNum;	//��ɫ��Ϣ	// machao add start on 2013-07-25
	PLATTE m_plaPalette[PLATTE_NUM];	//�Զ����ɫ����Ϣ
	COLORREF m_clrBack1;		//����ɫ
	COLORREF m_clrBack2;
	int m_nSize;			//��ɫ������ɫ��
	int m_nMainColorNum;	//����ɫ��	//machao add on 2013-07-22
	afx_msg void OnBnClickedButtonPalette();
	void DrawColorBar();
	//Machao add end on 2013-07-1
	//Machao add start on 2013-07-11
public:
	afx_msg void OnBnClickedButtonImportPalette();
	void CountRect(int nNum, int nIndex, float fTopPercent, float fBottomPercent, COLORREF clrTop, COLORREF clrBottom);
	//Machao add end on 2013-07-11
	void ReadColorFile(CString strPath);	// machao add on 2013-11-21
//	BOOL m_bInitializationColor;		// �Ƿ�Ϊ��ʼ����ɫ	//machao delete on 2013-09-22
	void ExtractAScanFile();			// ��ȡԭʼAɨ�ļ�Ϊĳ���ض�����µ�Aɨ�ļ� Song Chenguang add on 2014-01-16
public:
	afx_msg void OnBnClickedLockParam();
private:
	BOOL m_bLockParam;//yachang.huang add on 2014-3-7  
	afx_msg void OnBnClickedButtonBackgroundColor();
private:
	CBrush m_brushBackColor;//ѡ��ɫ��̬�򱳾���ˢ  yachang.huang add on 2014.4.1
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//�ؼ���ɫ���� yachang.huang add on 2014.4.1
	COLORREF m_clrBackColor;// ��ɫ yachang.huang add on 2014.4.1
public:
	int m_nMinAMP;//�����ʼ��㷶Χ�����ֵ
public:
	int m_nMaxAMP;//�����ʼ��㷶Χ�����ֵ
	float m_fTestAmp;	// �Կ鲨��
	float m_fSurComp;	// ���油��
};