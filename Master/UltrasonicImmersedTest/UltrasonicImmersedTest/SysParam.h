//SysParam.hʵ���ļ�
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ����ϵͳ�����Ի���ͷ�ļ�
*Filename : SysParam.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-19 
*/


#pragma once
#include "afxwin.h"
#include "PipeParam.h"
// CSysParam �Ի���

class CSysParam : public CDialog
{
	DECLARE_DYNAMIC(CSysParam)

public:
	CSysParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysParam();

// �Ի�������
	enum { IDD = IDD_SYS_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP() 
public:
	double m_dxl;		// x������г�
	double m_dyl;		// y������г�
	double m_dzl;		// z������г�
	double m_dxacc;		// x����ٶ�
	double m_dyacc;		// y����ٶ�
	double m_dzacc;		// z����ٶ�
	double m_dXResetVel;	// x�Ḵλ�ٶ�
	double m_dYResetVel;	// y�Ḵλ�ٶ�
	double m_dZResetVel;	// z�Ḵλ�ٶ�
	double m_dXMoveVel;		// x���ƶ��ٶ�
	double m_dYMoveVel;		// y���ƶ��ٶ�
	double m_dZMoveVel;		// z���ƶ��ٶ�
	double m_dXKongVel;		// x������ٶ�
	double m_dYKongVel;		// y������ٶ�
	double m_dZKongVel;		// z������ٶ�
	double m_dZHigh;		// ����z�߶�
	double m_Time;			// ͣ��ʱ��
	BOOL m_bUse;		// ʹ��
	BOOL m_bKeDu;		// �̶ȸ�ѡ��
	BOOL m_bPath;		// ·����ѡ��
	BOOL m_bCatch;		// ����ѡ��
	int m_nFillType;	// ��䷽ʽ
	double m_dFillSpace;// �����
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
	// ����ͨ�����ͨ��״̬
	void SetChannelState(const int& nProbeNum, 
		                 const int& nOneProbeIndex,
						 const int& nTwoProbeIndex,
						 const int& nThreeProbeIndex);

	// ����ͨ�����ͨ��״̬(New)
	void SetChannelStateNew(const int& nGroupNum, 
		const int& nOneGroupIndex,
		const int& nTwoGroupIndex,
		const int& nThreeGroupIndex, const WORD& wWorkMode);
	// ���õ���ͨ��״̬
	void SetGroupState(const int& nGroupIndex, const WORD& wWorkMode, const BOOL& bOPen);
public:
	CComboBox m_cmbAxis;
	int m_nAxisIndex;	// ƽ���л�
	float m_fChongDie;	
	int m_nStepDirect;	// ��������
	int m_nScanDirect;	// ɨ�跽��
	unsigned int m_nRSpeed;
	float m_fShangLiaoX;
	float m_fShangLiaoY;
	BOOL m_pbChannelOpen[CHANNEL_NUM];
	float m_fProbeLocation[CHANNEL_NUM][3];	// ̽ͷ1..n����̽ͷ1��ƫ��
	WORD m_wWorkMode;
public:
	afx_msg void OnCbnSelchangeComboWorkMode();
	// ���ݹ���ģʽ�趨��е�˶�����
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
	int m_nAxisIndexBack;	// ��ƽ��״̬����  Song Chenguang add on 2013-04-12
public:
	unsigned int m_nABSpeed;	// AB���ٶ�
	unsigned int m_nWSpeed;		// W���ٶ�
public:
	BOOL m_bTailDecreasePipe;
	// machao add start on 2013-08-27
	afx_msg void OnBnClickedRadioStepW();
	afx_msg void OnBnClickedRadioScanW();
	// machao add end on 2013-08-27
public:
	BOOL m_bChangeZWAxis;	// �����ع��л�ZW���־
};
