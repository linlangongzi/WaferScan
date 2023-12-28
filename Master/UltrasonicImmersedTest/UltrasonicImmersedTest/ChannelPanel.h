/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ͨ����崰����ͼͷ�ļ�
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
	CComboBox m_cmbGateNo;//բ��combobox�ؼ�����
public:
	virtual void OnInitialUpdate();
	int m_nChannelNo;
public:	
	CSliderCtrl m_sliderGateFront;// բ��ǰ�ػ���
	CSliderCtrl m_sliderGateBack;// բ�ſ�Ȼ������
	CSliderCtrl m_sliderGatePeak;// բ����ֵ�������
	float m_fGateFront;// բ��ǰ��edit�ؼ�����
	float m_fGateBack;//���
	float m_fGatePeak;//��ֵ

	CUltrasonicImmersedTestView* m_pAScanView;//Aɨ��ͼָ��
public:
	BOOL m_bGateIsOpen;//բ�ſ���
	
public:
	// ��̬����ʾբ����ɫ
	CStatic m_staticDisplayColor;
	CBrush m_brush;//��̬�򱳾���ˢ
public:
	void SetGateInfo(void);
	CChannelInfo GetChannelInfo(const WORD& wChannelNo);	// ��ȡͨ������
	void SetStaticColor(COLORREF color);//����բ����ɫ
	void SetProbeInfo(void);//����̽ͷ����
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
	afx_msg void OnCbnSelchangeComboGateNo();	//բ���л�
	afx_msg void OnBnClickedCheckGateIsopen();	// բ�ſ���
	afx_msg void OnBnClickedButtonGateColor();	// ѡ����ɫ
	afx_msg void OnBnChickedOtherParam();		// ��������
	afx_msg void OnBnClickedRadioCh1();			//ͨ���л���1
	afx_msg void OnBnClickedRadioCh2();			//ͨ���л���2
	afx_msg void OnBnClickedRadioInitpulse();	//��ʼ����
	afx_msg void OnBnClickedRadioFirstecho();	//�״λز�
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
	CSliderCtrl m_sliderDelay;	//�ӳٻ������
	float m_fDelay;				//�ӳ�edit�ؼ�����
	CSliderCtrl m_sliderRange;	//��Χ����ؼ�����
	float m_fRange;				//����edit�ؼ�����
	float m_fRestrain;			//����
	CChannelInfo m_ChannelInfo[CHANNEL_NUM];
	CSliderCtrl m_sliderPlus;
	float m_fPlus;				// ����
	float m_fRefPlus;			// �ο�����
	BOOL m_bMTPmode;	
	CSliderCtrl m_sliderMTPPeak;// ͬ����ֵ
	float m_fMTPPeak;	
	CComboBox m_comboChSI;		// ͨ���������
	CComboBox m_comboChSavg;	// ͨ������ƽ����
	CSliderCtrl m_sliderZeroPoint;	// ���slider
	float m_fZeroPoint;			// ���edit
	short m_nWaveSpeed;			// ����
	BOOL m_bMPFTimeDisplay;
	BOOL m_bMPMTimeDisplay;
	BOOL m_bMPMAltitudeDisplay;
	BOOL m_bFPFTimeDisplay;
	BOOL m_bFPMTimeDisplay;
	BOOL m_bFPMAltitudeDisplay;
	CComboBox m_cmbProbeFreq;	
	BOOL m_bGroup1Flag;		// ��1����
	BOOL m_bGroup2Flag;		// ��2����
	BOOL m_bGroup3Flag;		// ��3����
	BOOL m_bGroup4Flag;		// ��4����
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
	void DoModifyZeroPoint();  //���ú�����Ϊ���У�������CUltrasonicImmersedTestView�е��� yachang.huang modify on 2012-7-24
	void DoModifyRange();  //���ú�����Ϊ���У�������CUltrasonicImmersedTestView�е��� yachang.huang modify on 2012-7-24
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
	// ���͹���ģʽ
	BOOL SendWorkMode(const int& nCardIndex, const WORD& wWorkMode);
	// ���ݹ���ģʽ�趨��е�˶�����
//	void SetMoveRule(const WORD& wWorkMode);
// wangbingfu add start at 2012-11-19
public:
	int m_nOldOscillationMode[CHANNEL_NUM];
	float m_fOldDelay[CHANNEL_NUM];			// չ��ǰ�ӳ�
	float m_fOldRange[CHANNEL_NUM];			// չ��ǰ����
	float m_fOldPlus[CHANNEL_NUM];			// չ��ǰ����
	BOOL  m_bRfReduced[CHANNEL_NUM];
	BOOL m_bExtendFlag;						// չ���־
	BOOL m_bExtendArrayFlag[CHANNEL_NUM];	// չ���־
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
	int MicroSecond2DotChannel( const float& fMicroSecond );//Ӧ��ԽҪ���޸ģ����Ķ�Aɨ��������ʾ���Ӱ�����ֵ���㷽��  yachang.huang add on 2014-05-19
	float Dot2RangeChannel( float VSound, int DotNo );//Ӧ��ԽҪ���޸ģ����Ķ�Aɨ��������ʾ���Ӱ�����ֵ���㷽��  yachang.huang add on 2014-05-19
	DWORD Range2DotChannel( float VSound,  float Range );//Ӧ��ԽҪ���޸ģ����Ķ�Aɨ��������ʾ���Ӱ�����ֵ���㷽��  yachang.huang add on 2014-05-19
	float Dot2MicroSecondChannel( const int& nDot );//Ӧ��ԽҪ���޸ģ����Ķ�Aɨ��������ʾ���Ӱ�����ֵ���㷽��  yachang.huang add on 2014-05-19
public:
	//�޸�ת�������������ADFREQUENCYд��100 yachang.huang modify start on 2014-05-27
	int MicroSecond2DotChannel100( const float& fMicroSecond );
	float Dot2RangeChannel100( float VSound, int DotNo );
	DWORD Range2DotChannel100( float VSound,  float Range );
	float Dot2MicroSecondChannel100( const int& nDot );
	// yachang.huang modify end on 2014-05-27
};
// wangbingfu add start at 2012-11-21

