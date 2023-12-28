// CScanParamPanel .h : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* C扫参数面板窗体视图头文件
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
// CCScanParamPanel 窗体视图

class CCScanParamPanel : public CFormView
{
	DECLARE_DYNCREATE(CCScanParamPanel)

protected:
	CCScanParamPanel();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	void DrawRule(CDC* pDC,float fStart, float fEnd);//画刻度尺
//machao delete start on 2013-07-12
//public:
//	afx_msg void OnBnClickedButtonEndColor();
//machao delete end on 2013-07-12
public:
	afx_msg void OnBnClickedRadioDeepdraw();
public:
	afx_msg void OnBnClickedRadioRangedraw();
public:
	// 成像方式
	BOOL m_nDrawType;	// 0:幅度成像 1:深度成像 2:声速成像 3:声阻抗成像 4:声衰减成像 5:密度成像
	float m_fMinRule;//刻度尺最小刻度
	float m_fMaxRule;//刻度尺最大刻度
public:
	BOOL	m_bLianXuShuJu;	//连续数据标志
	
	double	m_dGatedelay[4];//延迟
	double	m_dGatedtThresh[4];//检测阈值
	BOOL	m_bGateon[4];//四个门的开关，貌似到后面的就没有用了
	float	m_uvm;//材料的预设声速
//	FeaturesData *pSortData;//指向排列数据的指针的指针
//	CList	<FeaturesData,FeaturesData> FeaturesFilterList;//数据过滤后链表
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
	float m_fThick;			// 工件厚度
	int m_nZoomScale;		// 参数放大比例
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
	int m_nValueMode;			// 取值模式	
	int m_nGateIndex;			// 选择的闸门	
	float m_fBoundMin;			// 显示范围上限	
	float m_fBoundMax;			// 显示范围下限	
	float m_fGate1Threshold;	// C扫闸门1阈值	
	float m_fGate2Threshold;	// C扫闸门2值	
	float m_fGate3Threshold;	// C扫闸门3阈值	
	float m_fGate4Threshold;	// C扫闸门1阈值
	//Machao add start on 2013-06-27
	BOOL m_bChangeOrigin;			// 变更坐标原点
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
	// 幅值特征选择
	int m_nAmpFeatureIndex;
public:
	afx_msg void OnBnClickedRadioMaxAmp();
public:
	afx_msg void OnBnClickedRadioFirstAmp();
public:
	// 时间特征选择
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
	//控件选中设置
	void OneItemSetCheck(int nID, BOOL bFlag);
//	void AllColorItemSetCheck(BOOL bFlag);	//machao delete on 2013-07-12
	void AllDrawModeItemSetCheck(BOOL bFlag);
	void AllGateSelectItemSetCheck(BOOL bFlag);
	void AllValueModeItemSetCheck(BOOL bFlag);
	void AllAmpSelectItemSetCheck(BOOL bFlag);
	void AllTimeSelectItemSetCheck(BOOL bFlag);
	void AllItemSetCheck(BOOL bFlag);
	//控件使能设置
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
	CSliderCtrl m_sliderBScanOrigin;//B扫起点
	float m_fBScanOrigin;
	CSliderCtrl m_sliderBScanWidth;//B扫显示宽度
	float m_fBScanWidth;
public:
	afx_msg void OnEnChangeEditThick();
// WangQianfeng delete end on 2013-06-28
public:
	// 面伤统计
	BOOL m_bSurfaceFlag;
	int m_nSurfceColorMode;	//面板色板模式
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
	// 特征值索引
	int m_nFeatureIndex;	// 成像模式
public:
	static int m_nObjectCount;
	int m_nObjectNum;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strPicturePath;
public:
	// 图片类型
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
	float m_fMinSurfaceArea;	//最小面积
	int m_nColorDefine;		//颜色定义 0=自定义；1=默认颜色
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
	int m_nSurfaceType;	// 框面伤的形状类型
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
	BOOL bHanHeFlag;	//点击焊合成像
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
	int m_nSurfaceSelType;	// 统计伤类型  Song Chenguang add on 2013-06-20
	CListCtrl m_lstctrlCustumFlaw;
	BOOL m_bRealScale;		// 实际比例显示标志  Song Chenguang add on 2013-07-10
	//Machao add start on 2013-07-1
public:
	vector<int> m_vnMainColorNum;	//主色信息	// machao add start on 2013-07-25
	PLATTE m_plaPalette[PLATTE_NUM];	//自定义调色板信息
	COLORREF m_clrBack1;		//背景色
	COLORREF m_clrBack2;
	int m_nSize;			//调色板总颜色数
	int m_nMainColorNum;	//主颜色数	//machao add on 2013-07-22
	afx_msg void OnBnClickedButtonPalette();
	void DrawColorBar();
	//Machao add end on 2013-07-1
	//Machao add start on 2013-07-11
public:
	afx_msg void OnBnClickedButtonImportPalette();
	void CountRect(int nNum, int nIndex, float fTopPercent, float fBottomPercent, COLORREF clrTop, COLORREF clrBottom);
	//Machao add end on 2013-07-11
	void ReadColorFile(CString strPath);	// machao add on 2013-11-21
//	BOOL m_bInitializationColor;		// 是否为初始化颜色	//machao delete on 2013-09-22
	void ExtractAScanFile();			// 提取原始A扫文件为某种特定情况下的A扫文件 Song Chenguang add on 2014-01-16
public:
	afx_msg void OnBnClickedLockParam();
private:
	BOOL m_bLockParam;//yachang.huang add on 2014-3-7  
	afx_msg void OnBnClickedButtonBackgroundColor();
private:
	CBrush m_brushBackColor;//选底色静态框背景画刷  yachang.huang add on 2014.4.1
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//控件颜色函数 yachang.huang add on 2014.4.1
	COLORREF m_clrBackColor;// 底色 yachang.huang add on 2014.4.1
public:
	int m_nMinAMP;//焊合率计算范围最低阈值
public:
	int m_nMaxAMP;//焊合率计算范围最高阈值
	float m_fTestAmp;	// 试块波高
	float m_fSurComp;	// 表面补偿
};