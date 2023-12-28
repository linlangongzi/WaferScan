// MechanicalControlPanel.h : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 机械控制CMechanicalControlPanel面板窗体视图头文件
*Filename : MechanicalControlPanel.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-10 
*/

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "exbutton.h"



// CMechanicalControlPanel 窗体视图
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
	CMechanicalControlPanel();           // 动态创建所使用的受保护的构造函数
	virtual ~CMechanicalControlPanel();
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	enum { IDD = IDD_MECHANICAL_CONTROL };

	CExButton m_btnXLeft;//机械控制按钮左移
	CExButton m_btnXRight;//右移
	CExButton m_btnYFront;//前移
	CExButton m_btnYBack;//后移
	CExButton m_btnZUp;//上
	CExButton m_btnZDown;//下
	CExButton m_btnABPositive;
	CExButton m_btnABReverse;
	CExButton m_btnWPositive;
	CExButton m_btnWReverse;
	CExButton m_btnRPositive;
	CExButton m_btnRReverse;	

	long m_lGrapNum;//图形记录总数

	CMovePathView* m_pMovePathView;//移动轨迹视图指针
	CCScanView* m_pCScanView;
	CCScanView* m_pCScanView2;
	//void	Quit();//退出操作
public:
	BOOL m_bPathExecute;// 是否操作的“运行”功能

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
	bool CreateView(CWnd* pParent, CCreateContext* pContext);
public:
	afx_msg void OnBnClickedButtonReposition();//复位按钮
public:
	afx_msg void OnBnClickedButtonRun();//运行按钮
	void PathExeute();//路径执行
	CButton m_btnRun;//运行 按钮控制变量
public:
	virtual void OnInitialUpdate();
public:
	void HandStart(int type);//手动开始
	void HandStop(void);
public:
	double m_dXcoord;//自定义复位坐标原点---X坐标
	double m_dYcoord;// 自定义复位坐标原点---Y坐标
	double m_dZcoord;// 自定义复位坐标原点--Z坐标
	double m_dABcoord;// 自定义复位坐标原点--W1坐标
	// wangbingfu add start on 2013-01-31
	double m_dWcoord;
	double m_dRcoord;
	// wangbingfu add end on 2013-01-31
public:
	afx_msg void OnBnClickedButtonDefineReposition();//自定义原点复位按钮
public:
	CStatic m_staticTx;
	CStatic m_staticTy;
	CStatic m_staticTz;
	double m_dAcoord[6];//绝对坐标模式下坐标数组，X坐标，Y坐标,Z坐标
	//double m_dRcoord[3];//相对坐标模式下坐标数组，即相对偏移量
	double m_dCurrentCoord[6];//当前坐标

	double m_dSysPara[SYSPARAM1_NUM];//返回系统参数
	long	m_logPara[10];//填充方式
	
public:
//	afx_msg void OnBnClickedRadioRelative();//相对坐标单选按钮相应
//	afx_msg void OnBnClickedRadioAbsolute();//绝对坐标单选按钮相应
//	int m_PTPmode;//自定义复位点模式0：相对坐标，1：绝对坐标
	//int m_bPTPmode;
	double m_Xcoord;
	double m_Ycoord;
	double m_Zcoord;
	double m_W1coord;
	double m_dRelativeCoord[7];
	int m_nMPMTimeIndex;//门内最大波幅时间在特征值里的序号
	int m_nMPMPeakIndex;
public:
	afx_msg void OnBnClickedButtonSysparam();//系统参数按钮响应函数

	// 连续/单步运动控制开关
	BOOL m_bOneStep;
	float m_fXstep;		// x步进
	float m_fYstep;		// y步进
	float m_fZstep;		// z步进	
	// Song Chenguang add start on 2013-03-15
	float m_fABStep;		// A步进
	float m_fWStep;		// W步进
	// Song Chenguang add end on 2013-03-15
	float m_fRStep;		// R步进

	afx_msg void OnBnClickedButtonSetstep();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CStatic m_staticYCoord;//移动探头,同步静态框中显示当前坐标
	CStatic m_staticZCoord;
	CStatic m_staticXCoord;
//	CStatic m_staticTW1;
	CSysParam m_SysParamDlg;
public:
	afx_msg void OnDestroy();
public:
	BOOL SaveTestResult();	// 保存探伤数据
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
	int **m_pGridCount;	//矩阵点描绘次数矩阵
public:
	void AllocateGridNumMemory();	// 分配网格内存
	void FreeGridNumMemory();		// 释放网格内存
	void GridConvert(CFeatureInfo& Fdata, float fXStep, float fYStep);
	void FeatureReCalculate(CFeatureInfo& featureTail, const CFeatureInfo& feature, 
						BOOL bFeatureFlag[], int i, int j, int k, int x, int y, int nValueMode);
private:
	void SaveResultToDB();
	void SetChannelOpenFlag(BOOL bOpen[], const int& nCardNo);
public:
	void InitSysParamDlg();	// 初始化系统参数对话框值
public:
	float m_fFirstWdrX;		// 起点X值
	float m_fFirstWdrY;		// 起点Y值
	float m_fFirstWdrZ;		// 起点Z值
public:
	CListCtrl m_lstRectPoints;
	afx_msg void OnBnClickedButtonDisplayRect();
	LRESULT OnReceiveFeatureInfo(WPARAM wParam, LPARAM lParam);
public:
	BOOL CScanViewSet(CCScanView* pCScanView);	//C扫视图相关设置 Song Chenguang modify on 2013-07-18
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
	CString m_strFilePath;		// 路径文件名
public:    //修改m_strDataFolder属性为public   yachang.huang  modify  on  2012-11-21
	CString m_strDataFolder;
	float m_fOldGateBBack;	// wangbingfu add at 2012-12-5
	// Song Chenguang add on 2013-04-06
	CFile m_AsyncCScanFile;	// C扫描临时存储文件
	CFile m_AsyncAScanFile;	// A扫描临时存储文件
	CFile m_AsyncAScanFile1;// A扫描临时存储文件 双晶关联时两个通道分开两个文件存 wangbingfu add on 2014-09-10
	CFile m_AsyncAScanPrimitiveFile;	// A扫描临时存储原始文件  Song Chenguang add on 2014-01-15
	//CStdioFile m_AscanTxtFile;	// A扫数据txt显示
	CString m_strTempPath;	// 扫描临时存储文件路径（不包含后缀）
	CString m_strTempPath1;	// 扫描临时存储文件路径（不包含后缀）双晶关联时两个通道分开两个文件存 wangbingfu add on 2014-09-10
	// Song Chenguang add on 2013-04-06
	BOOL m_bTaskRun;	// 任务运行标志 Song Chenguang add on 2013-04-26
	WORD m_wTaskNo;		// 任务子项序号
	WORD m_wTaskItemCount;	// 任务子项数
	CString m_strTaskListPath;	// 列表文件路径
	CString m_strCurrentMovePath;	// 当前正在扫的文件路径
private:
	int m_nTracingCount;	// wangbingfu add on 2014-10-14
public:
	void RunTask(CString strListPath);	// 运行任务 Song Chenguang add on 2013-04-26
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
	// 比较两个A扫数据，data3为输出要保存的值
	void CompareTwoAscanData(const CPicData_L& data1, const CPicData_L& data2, CPicData_L& data3, 
		const int& nGateIndex, const int& nFeatureIndex, const int& nValueMode);	// Song Chenguang add on 2014-01-15
	void SaveAScanHeadData(const CCScanFileHeader &fileHeader, CFile* file); // wangbingfu add on 2014-09-10
};

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp 中的调试版本
inline CUltrasonicImmersedTestDoc* CMechanicalControlPanel::GetDocument() const
{ return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif
