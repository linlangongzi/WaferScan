/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 通道面板窗体视图头文件
*Filename : ChannelPanel.h
*
*@author       songchenguang zhaoli 
*@version      1.0         Date: 2011-7-10
*/

#pragma once
#include "afxcmn.h"

const int MATERIAL_NUM = 11;
class CUltrasonicImmersedTestView;

class CMaterialSoundSpeed
{
public:
	CMaterialSoundSpeed():m_strMaterial(_T("")),m_wSoundSpeed(5980)
	{
	}

	~CMaterialSoundSpeed()
	{
	}
public:
	CString m_strMaterial;
	WORD m_wSoundSpeed;
};
// CChannelPanel form view

class CChannelPanel : public CFormView
{
	DECLARE_DYNCREATE(CChannelPanel)

protected:
	CChannelPanel();           // protected constructor used by dynamic creation
	virtual ~CChannelPanel();

public:
	enum { IDD = IDD_CHANNEL_PANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbGateNo;//闸门combobox控件变量
public:
	virtual void OnInitialUpdate();
	int m_nChannelNo;
public:	
	CSliderCtrl m_sliderGateFront;// 闸门前沿滑块
	CSliderCtrl m_sliderGateBack;// 闸门宽度滑块变量
	CSliderCtrl m_sliderGatePeak;// 闸门阈值滑块变量
	float m_fGateFront;// 闸门前沿edit控件变量
	float m_fGateBack;//宽度
	float m_fGatePeak;//阈值

	CUltrasonicImmersedTestView* m_pAScanView;//A扫视图指针
public:
	BOOL m_bGateIsOpen;//闸门开关
	
public:
	// 静态框显示闸门颜色
	CStatic m_staticDisplayColor;
	CBrush m_brush;//静态框背景画刷
public:
	void SetGateInfo(void);
	CChannelInfo GetChannelInfo(const WORD& wChannelNo);	// 获取通道数据
	void SetStaticColor(COLORREF color);//设置闸门颜色
	void SetProbeInfo(void);//设置探头参数
	void InitChannelInfo();
	void ChannelInfoToData();
	void ALLSCanItemEnableJudge();
	void SaveParam(const CString& strFilePath);
	void ReadParam(const CString& strFilePath);
//private:
	//void SaveParam(const CString& strFilePath);
private:
	CString m_strParamFilePath;
public:
	afx_msg void OnCbnSelchangeComboGateNo();	//闸门切换
	afx_msg void OnBnClickedCheckGateIsopen();	// 闸门开关
	afx_msg void OnBnClickedButtonGateColor();	// 选择颜色
	afx_msg void OnBnChickedOtherParam();		// 其它参数
	afx_msg void OnBnClickedRadioCh1();			//通道切换，1
	afx_msg void OnBnClickedRadioCh2();			//通道切换，2
	afx_msg void OnBnClickedRadioInitpulse();	//初始脉冲
	afx_msg void OnBnClickedRadioFirstecho();	//首次回波
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeltaposSpinFront(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinGatePeak(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMtpPeak(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZeropoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPlus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSaveAsParam();
	afx_msg void OnBnClickedButtonReadParam();
	afx_msg void OnCbnSelchangeComboChSi();
	afx_msg void OnCbnSelchangeComboChSavg();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnKillfocusEditWaveSpeed();
	afx_msg void OnCbnSelchangeComboProbeFreq();
	afx_msg void OnBnClickedCheckMpMAltitude();
	afx_msg void OnBnClickedCheckFpMAltitude();
	afx_msg void OnBnClickedCheckMpFTime();
	afx_msg void OnBnClickedCheckMpMTime();
	afx_msg void OnBnClickedCheckFpFTime();
	afx_msg void OnBnClickedCheckFpMTime();
	afx_msg void OnBnClickedButtonSaveParam();
	afx_msg void OnBnClickedCheckGroup1Switch();
	afx_msg void OnBnClickedCheckGroup2Switch();
	afx_msg void OnDeltaposSpinRestrain(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelchangeComboWorkMode();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRadioCh3();
	afx_msg void OnBnClickedRadioCh4();
	afx_msg void OnBnClickedRadioCh5();
	afx_msg void OnBnClickedRadioCh6();
	afx_msg void OnBnClickedRadioCh7();
	afx_msg void OnBnClickedRadioCh8();
	afx_msg void OnBnClickedCheckGroup3Switch();
	afx_msg void OnBnClickedCheckGroup4Switch();
	afx_msg void OnCbnSelchangeComboMaterial();
	afx_msg void OnEnChangeEditWaveSpeed();
	afx_msg void OnDeltaposSpinReFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckLockParam();
	afx_msg void OnDeltaposSpinRefPlus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//yachang.huang add on 2013-06-29
	afx_msg void OnReleasedcaptureSliderReFreq(NMHDR *,LRESULT *);
public:
	CSliderCtrl m_sliderDelay;	//延迟滑块变量
	float m_fDelay;				//延迟edit控件变量
	CSliderCtrl m_sliderRange;	//范围滑块控件变量
	float m_fRange;				//滑块edit控件变量
	float m_fRestrain;			//抑制
	CChannelInfo m_ChannelInfo[CHANNEL_NUM];
	CSliderCtrl m_sliderPlus;
	float m_fPlus;				// 增益
	float m_fRefPlus;			// 参考增益
	BOOL m_bMTPmode;	
	CSliderCtrl m_sliderMTPPeak;// 同步阈值
	float m_fMTPPeak;	
	CComboBox m_comboChSI;		// 通道采样间隔
	CComboBox m_comboChSavg;	// 通道采样平均数
	CSliderCtrl m_sliderZeroPoint;	// 零点slider
	float m_fZeroPoint;			// 零点edit
	short m_nWaveSpeed;			// 声速
	BOOL m_bMPFTimeDisplay;
	BOOL m_bMPMTimeDisplay;
	BOOL m_bMPMAltitudeDisplay;
	BOOL m_bFPFTimeDisplay;
	BOOL m_bFPMTimeDisplay;
	BOOL m_bFPMAltitudeDisplay;
	CComboBox m_cmbProbeFreq;	
	BOOL m_bGroup1Flag;		// 组1开关
	BOOL m_bGroup2Flag;		// 组2开关
	BOOL m_bGroup3Flag;		// 组3开关
	BOOL m_bGroup4Flag;		// 组4开关
	CSliderCtrl m_sliderRestrain;
private:
	BOOL IsInSafeRange(float& fZeroPoint, float& fDelay, float& fRange, const int& nInputIndex);
	void SyncIGateThreshValue(const float fThreshValue);
	void DoModifyGateFront();
	void DoModifyGateWidth();
	void DoModifyGatePeak();
	void DoModifyMTPPeak();	
	void DoModifyPlus();
	void DoModifyRefPlus();
	void DoModifyRestain();	
	void InitMaterialSoundSpeed();
public:
	void DoModifyDelay();
	void DoChangeChannel();
	void DoModifyZeroPoint();  //将该函数改为公有，方便在CUltrasonicImmersedTestView中调用 yachang.huang modify on 2012-7-24
	void DoModifyRange();  //将该函数改为公有，方便在CUltrasonicImmersedTestView中调用 yachang.huang modify on 2012-7-24
	void DoModifyGateWidth(int nGateNum, float fBack); // wangbingfu add at 2012-12-5
public:	
	CComboBox m_cmbMaterial;
	
	CMaterialSoundSpeed m_MaterialSoundSpeeds[MATERIAL_NUM];
public:
	CComboBox m_cmbSyncFreq;
	CProbeInfoDlg m_ProbeInfoDlg;
private:
	int CalcRepeateCounter(const int& nSynFreq);
	int CalcRepeateCounterReverse(const int& nValue);
	void DoModifyReFreq();
public:
	CSliderCtrl m_sliderReFreq;
	int m_nReFreq;
	BOOL m_bLockParam;
	CSliderCtrl m_sldRefPlus;
//	WORD m_wWorkMode;	
	int CalGroupOtherIndex(const int& nChannelIndex);
private:
	VOLTAGE_TYPE_e FindVoltageType(const WORD& wValue);	
	
public:
	// 发送工作模式
	BOOL SendWorkMode(const int& nCardIndex, const WORD& wWorkMode);
	// 根据工作模式设定机械运动规则
//	void SetMoveRule(const WORD& wWorkMode);
// wangbingfu add start at 2012-11-19
public:
	int m_nOldOscillationMode[CHANNEL_NUM];
	float m_fOldDelay[CHANNEL_NUM];			// 展宽前延迟
	float m_fOldRange[CHANNEL_NUM];			// 展宽前声程
	float m_fOldPlus[CHANNEL_NUM];			// 展宽前声程
	BOOL  m_bRfReduced[CHANNEL_NUM];
	BOOL m_bExtendFlag;						// 展宽标志
	BOOL m_bExtendArrayFlag[CHANNEL_NUM];	// 展宽标志
	afx_msg void OnBnClickedCheckExtension();
	void RfExtendOn();
	void RfExtendOff();
public:
	BOOL m_bGateAdjust;
public:
	afx_msg void OnBnClickedCheckGateAdjust();
public:
	// wangbingfu add start on 2013-01-09
	void SetGroupOtherChannel();
	// wangbingfu add end on 2013-01-09
public:
	int MicroSecond2DotChannel( const float& fMicroSecond );//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang add on 2014-05-19
	float Dot2RangeChannel( float VSound, int DotNo );//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang add on 2014-05-19
	DWORD Range2DotChannel( float VSound,  float Range );//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang add on 2014-05-19
	float Dot2MicroSecondChannel( const int& nDot );//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang add on 2014-05-19
public:
	//修改转换函数，里面的ADFREQUENCY写成100 yachang.huang modify start on 2014-05-27
	int MicroSecond2DotChannel100( const float& fMicroSecond );
	float Dot2RangeChannel100( float VSound, int DotNo );
	DWORD Range2DotChannel100( float VSound,  float Range );
	float Dot2MicroSecondChannel100( const int& nDot );
	// yachang.huang modify end on 2014-05-27
};
// wangbingfu add start at 2012-11-21

