//SysParam.h实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 设置系统参数对话框头文件
*Filename : SysParam.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-19 
*/


#pragma once
#include "afxwin.h"
#include "PipeParam.h"
// CSysParam 对话框

class CSysParam : public CDialog
{
	DECLARE_DYNAMIC(CSysParam)

public:
	CSysParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysParam();

// 对话框数据
	enum { IDD = IDD_SYS_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP() 
public:
	double m_dxl;		// x轴最大行程
	double m_dyl;		// y轴最大行程
	double m_dzl;		// z轴最大行程
	double m_dxacc;		// x轴加速度
	double m_dyacc;		// y轴加速度
	double m_dzacc;		// z轴加速度
	double m_dXResetVel;	// x轴复位速度
	double m_dYResetVel;	// y轴复位速度
	double m_dZResetVel;	// z轴复位速度
	double m_dXMoveVel;		// x轴移动速度
	double m_dYMoveVel;		// y轴移动速度
	double m_dZMoveVel;		// z轴移动速度
	double m_dXKongVel;		// x轴空走速度
	double m_dYKongVel;		// y轴空走速度
	double m_dZKongVel;		// z轴空走速度
	double m_dZHigh;		// 空走z高度
	double m_Time;			// 停留时间
	BOOL m_bUse;		// 使用
	BOOL m_bKeDu;		// 刻度复选框
	BOOL m_bPath;		// 路径复选框
	BOOL m_bCatch;		// 捕获复选框
	int m_nFillType;	// 填充方式
	double m_dFillSpace;// 填充间距
public:
	double m_dFillSpace2;
	BOOL m_bUseSafeArea;
	float m_fPerLength;
	float m_fProbeDistance;
	int m_nProbeNum;
	float m_fSafeMinX;
	float m_fSafeMaxX;
	float m_fSafeMinY;
	float m_fSafeMaxY;
	float m_fSafeMaxZ;
	int m_nScanType;
public:
	CComboBox m_cmbProbeNum;
	CComboBox m_cmbOneProbeType;
	CComboBox m_cmbTwoProbeType;
	CComboBox m_cmbThreeProbeType;
	CComboBox m_cmbFourProbeType;
public:
	virtual BOOL OnInitDialog();
public:
	int m_nIndexProbeNum;
	int m_nIndexOneProbeType;
	int m_nIndexTwoProbeType;
	int m_nIndexThreeProbeType;
public:
	afx_msg void OnCbnSelchangeComboProbeNum();
	afx_msg void OnCbnSelchangeComboOneProbeType();
	afx_msg void OnCbnSelchangeComboTwoProbeType();
	afx_msg void OnCbnSelchangeComboThreeProbeType();
	afx_msg void OnCbnSelchangeComboFourProbeType();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedScanTypeStep();
	afx_msg void OnBnClickedScanTypeDepth();
	afx_msg void OnCbnSelchangeComboAxis();	
	afx_msg void OnBnClickedRadioStepX();
	afx_msg void OnBnClickedRadioStepY();
	afx_msg void OnBnClickedRadioStepZ();	
	afx_msg void OnBnClickedRadioScanX();
	afx_msg void OnBnClickedRadioScanY();
	afx_msg void OnBnClickedRadioSacnZ();
	afx_msg void OnBnClickedProbeLocation();
public:
	// 设置通道面板通道状态
	void SetChannelState(const int& nProbeNum, 
		                 const int& nOneProbeIndex,
						 const int& nTwoProbeIndex,
						 const int& nThreeProbeIndex);

	// 设置通道面板通道状态(New)
	void SetChannelStateNew(const int& nGroupNum, 
		const int& nOneGroupIndex,
		const int& nTwoGroupIndex,
		const int& nThreeGroupIndex, const WORD& wWorkMode);
	// 设置单组通道状态
	void SetGroupState(const int& nGroupIndex, const WORD& wWorkMode, const BOOL& bOPen);
public:
	CComboBox m_cmbAxis;
	int m_nAxisIndex;	// 平面切换
	float m_fChongDie;	
	int m_nStepDirect;	// 步进方向
	int m_nScanDirect;	// 扫描方向
	unsigned int m_nRSpeed;
	float m_fShangLiaoX;
	float m_fShangLiaoY;
	BOOL m_pbChannelOpen[CHANNEL_NUM];
	float m_fProbeLocation[CHANNEL_NUM][3];	// 探头1..n距离探头1的偏移
	WORD m_wWorkMode;
public:
	afx_msg void OnCbnSelchangeComboWorkMode();
	// 根据工作模式设定机械运动规则
	void SetMoveRule(const int& nGroupNum, 
		const int& nOneGroupIndex,
		const int& nTwoGroupIndex,
		const int& nThreeGroupIndex,
		const WORD& wWorkMode);
	void Group2ProbeValue(int &nProbeNum, int &nOneProbeIndex, int &nTwoProbeIndex, int &nThreeProbeIndex);
	// wangbingfu add start on 2012-12-26
	afx_msg void OnCbnSelchangeComboStepMode();
	CComboBox m_cmbStepMode;
	int m_nStepMode;
	// wangbingfu add end on 2012-12-26
public:
	afx_msg void OnBnClickedButtonPipeSet();
	// wangbingfu add start on 2013-01-09
public:
	CPipeParam m_pipeParamDlg;
	// wangbingfu add end on 2013-01-09
	int m_nAxisIndexBack;	// 轴平面状态备份  Song Chenguang add on 2013-04-12
public:
	unsigned int m_nABSpeed;	// AB轴速度
	unsigned int m_nWSpeed;		// W轴速度
public:
	BOOL m_bTailDecreasePipe;
	// machao add start on 2013-08-27
	afx_msg void OnBnClickedRadioStepW();
	afx_msg void OnBnClickedRadioScanW();
	// machao add end on 2013-08-27
public:
	BOOL m_bChangeZWAxis;	// 北方重工切换ZW轴标志
};
