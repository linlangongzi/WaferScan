// MechanicalControlPanel.h : ʵ���ļ�
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* ��е����CMechanicalControlPanel��崰����ͼͷ�ļ�
*Filename : MechanicalControlPanel.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-10 
*/

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "exbutton.h"



// CMechanicalControlPanel ������ͼ
class CMovePathView;
class CExButton;
class CDGraphMove;
class CMechanicalStepParamDlg;
class CUltrasonicImmersedTestDoc;
class CCScanView;
class CSysParam;

class CMechanicalControlPanel : public CFormView
{
	DECLARE_DYNCREATE(CMechanicalControlPanel)

protected:
	CMechanicalControlPanel();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMechanicalControlPanel();
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	enum { IDD = IDD_MECHANICAL_CONTROL };

	CExButton m_btnXLeft;//��е���ư�ť����
	CExButton m_btnXRight;//����
	CExButton m_btnYFront;//ǰ��
	CExButton m_btnYBack;//����
	CExButton m_btnZUp;//��
	CExButton m_btnZDown;//��
	CExButton m_btnABPositive;
	CExButton m_btnABReverse;
	CExButton m_btnWPositive;
	CExButton m_btnWReverse;
	CExButton m_btnRPositive;
	CExButton m_btnRReverse;	

	long m_lGrapNum;//ͼ�μ�¼����

	CMovePathView* m_pMovePathView;//�ƶ��켣��ͼָ��
	CCScanView* m_pCScanView;
	CCScanView* m_pCScanView2;
	//void	Quit();//�˳�����
public:
	BOOL m_bPathExecute;// �Ƿ�����ġ����С�����

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
	bool CreateView(CWnd* pParent, CCreateContext* pContext);
public:
	afx_msg void OnBnClickedButtonReposition();//��λ��ť
public:
	afx_msg void OnBnClickedButtonRun();//���а�ť
	void PathExeute();//·��ִ��
	CButton m_btnRun;//���� ��ť���Ʊ���
public:
	virtual void OnInitialUpdate();
public:
	void HandStart(int type);//�ֶ���ʼ
	void HandStop(void);
public:
	double m_dXcoord;//�Զ��帴λ����ԭ��---X����
	double m_dYcoord;// �Զ��帴λ����ԭ��---Y����
	double m_dZcoord;// �Զ��帴λ����ԭ��--Z����
	double m_dABcoord;// �Զ��帴λ����ԭ��--W1����
	// wangbingfu add start on 2013-01-31
	double m_dWcoord;
	double m_dRcoord;
	// wangbingfu add end on 2013-01-31
public:
	afx_msg void OnBnClickedButtonDefineReposition();//�Զ���ԭ�㸴λ��ť
public:
	CStatic m_staticTx;
	CStatic m_staticTy;
	CStatic m_staticTz;
	double m_dAcoord[6];//��������ģʽ���������飬X���꣬Y����,Z����
	//double m_dRcoord[3];//�������ģʽ���������飬�����ƫ����
	double m_dCurrentCoord[6];//��ǰ����

	double m_dSysPara[SYSPARAM1_NUM];//����ϵͳ����
	long	m_logPara[10];//��䷽ʽ
	
public:
//	afx_msg void OnBnClickedRadioRelative();//������굥ѡ��ť��Ӧ
//	afx_msg void OnBnClickedRadioAbsolute();//�������굥ѡ��ť��Ӧ
//	int m_PTPmode;//�Զ��帴λ��ģʽ0��������꣬1����������
	//int m_bPTPmode;
	double m_Xcoord;
	double m_Ycoord;
	double m_Zcoord;
	double m_W1coord;
	double m_dRelativeCoord[7];
	int m_nMPMTimeIndex;//������󲨷�ʱ��������ֵ������
	int m_nMPMPeakIndex;
public:
	afx_msg void OnBnClickedButtonSysparam();//ϵͳ������ť��Ӧ����

	// ����/�����˶����ƿ���
	BOOL m_bOneStep;
	float m_fXstep;		// x����
	float m_fYstep;		// y����
	float m_fZstep;		// z����	
	// Song Chenguang add start on 2013-03-15
	float m_fABStep;		// A����
	float m_fWStep;		// W����
	// Song Chenguang add end on 2013-03-15
	float m_fRStep;		// R����

	afx_msg void OnBnClickedButtonSetstep();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CStatic m_staticYCoord;//�ƶ�̽ͷ,ͬ����̬������ʾ��ǰ����
	CStatic m_staticZCoord;
	CStatic m_staticXCoord;
//	CStatic m_staticTW1;
	CSysParam m_SysParamDlg;
public:
	afx_msg void OnDestroy();
public:
	BOOL SaveTestResult();	// ����̽������
private:
	BOOL SaveDataFile( const CString& strPath, const CString& strFileName);	// wangbingfu modify on 2013-01-09
private:
	float m_fWdrX;
	float m_fWdrY;
public:
	float m_fWdrZ;
	afx_msg void OnBnClickedSavePathFile();
	afx_msg void OnBnClickedOpenPathFile();
	afx_msg void OnBnClickedXiangduiCrood();
	afx_msg void OnBnClickedRadioAbsolute();
	
	int m_nAbsolute;
	afx_msg void OnBnClickedFileSaveAs();
	afx_msg void OnBnChickedGetRect();
public:
	int **m_pGridCount;	//���������������
public:
	void AllocateGridNumMemory();	// ���������ڴ�
	void FreeGridNumMemory();		// �ͷ������ڴ�
	void GridConvert(CFeatureInfo& Fdata, float fXStep, float fYStep);
	void FeatureReCalculate(CFeatureInfo& featureTail, const CFeatureInfo& feature, 
						BOOL bFeatureFlag[], int i, int j, int k, int x, int y, int nValueMode);
private:
	void SaveResultToDB();
	void SetChannelOpenFlag(BOOL bOpen[], const int& nCardNo);
public:
	void InitSysParamDlg();	// ��ʼ��ϵͳ�����Ի���ֵ
public:
	float m_fFirstWdrX;		// ���Xֵ
	float m_fFirstWdrY;		// ���Yֵ
	float m_fFirstWdrZ;		// ���Zֵ
public:
	CListCtrl m_lstRectPoints;
	afx_msg void OnBnClickedButtonDisplayRect();
	LRESULT OnReceiveFeatureInfo(WPARAM wParam, LPARAM lParam);
public:
	BOOL CScanViewSet(CCScanView* pCScanView);	//Cɨ��ͼ������� Song Chenguang modify on 2013-07-18
public:
	
public:
	void AddScanData(CH_FLAWDATA_CSCAN_S dataArray[], const int& nDataSize, const int& nCardNo, const int& nChannelNo);
	void PathStop( BOOL p_flag );
	void OnArriveStartPoint();
	void OnResetLowerMachine();
// public:
// 	double m_dW1Coord;
private:
	void SetStepLabelText();
	afx_msg void OnBnClickedButtonShangliaoPos();
private:
	int m_ChannelInfoID[8];
	int m_CScanInfoID[50];
	int m_MechanicalInfoID[18];
	CString m_strFilePath;		// ·���ļ���
public:    //�޸�m_strDataFolder����Ϊpublic   yachang.huang  modify  on  2012-11-21
	CString m_strDataFolder;
	float m_fOldGateBBack;	// wangbingfu add at 2012-12-5
	// Song Chenguang add on 2013-04-06
	CFile m_AsyncCScanFile;	// Cɨ����ʱ�洢�ļ�
	CFile m_AsyncAScanFile;	// Aɨ����ʱ�洢�ļ�
	CFile m_AsyncAScanFile1;// Aɨ����ʱ�洢�ļ� ˫������ʱ����ͨ���ֿ������ļ��� wangbingfu add on 2014-09-10
	CFile m_AsyncAScanPrimitiveFile;	// Aɨ����ʱ�洢ԭʼ�ļ�  Song Chenguang add on 2014-01-15
	//CStdioFile m_AscanTxtFile;	// Aɨ����txt��ʾ
	CString m_strTempPath;	// ɨ����ʱ�洢�ļ�·������������׺��
	CString m_strTempPath1;	// ɨ����ʱ�洢�ļ�·������������׺��˫������ʱ����ͨ���ֿ������ļ��� wangbingfu add on 2014-09-10
	// Song Chenguang add on 2013-04-06
	BOOL m_bTaskRun;	// �������б�־ Song Chenguang add on 2013-04-26
	WORD m_wTaskNo;		// �����������
	WORD m_wTaskItemCount;	// ����������
	CString m_strTaskListPath;	// �б��ļ�·��
	CString m_strCurrentMovePath;	// ��ǰ����ɨ���ļ�·��
private:
	int m_nTracingCount;	// wangbingfu add on 2014-10-14
public:
	void RunTask(CString strListPath);	// �������� Song Chenguang add on 2013-04-26
public:
	afx_msg void OnBnClickedButtonGetWaterDistance();
public:
	double m_dWaterDistance;
public:
	double m_dABRadius;
public:
	double m_dWRadius;
	BOOL m_bXYReverse;
public:
	afx_msg void OnBnClickedCheckXYReverse();
	void SaveFilePath(const CString& strPath);	// machao add on 2013-11-18
private:
	void SetMoveButtonIcon(const BOOL& bIsInit);
	afx_msg void OnBnClickedCheckOnestep();
//////////////////////////////////////////////////////////////////////////
// test
	double m_fWCurPos;
//////////////////////////////////////////////////////////////////////////
public:
	// �Ƚ�����Aɨ���ݣ�data3Ϊ���Ҫ�����ֵ
	void CompareTwoAscanData(const CPicData_L& data1, const CPicData_L& data2, CPicData_L& data3, 
		const int& nGateIndex, const int& nFeatureIndex, const int& nValueMode);	// Song Chenguang add on 2014-01-15
	void SaveAScanHeadData(const CCScanFileHeader &fileHeader, CFile* file); // wangbingfu add on 2014-09-10
};

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp �еĵ��԰汾
inline CUltrasonicImmersedTestDoc* CMechanicalControlPanel::GetDocument() const
{ return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif
