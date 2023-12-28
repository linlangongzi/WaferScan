/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 应用程序MainFrame框架程序的头文件
*Filename : MainFrm.h
*
*@author       Song Chenguang zhaoli
*@version      1.0         Date: 2011-7-1 
*/

#pragma once
class CCScanDlgBar;
class CUltrasonicImmersedTestView;
class CChannelInfoDockView;
class CMechanicalControl;
class CCScanParamDockBar;
class CFeatureInfoWnd;
class CDacCurveDlg;
class CARSSocket;
class CWSocket;
class CDataSocket;
class CTcgCurveDlg;
class CTaskDockView;	// Song Chenguang add on 2013-04-16
class CAScanDataSocket;	// Song Chenguang add on 2013-12-17
#include "CProbeSet.h"
class CMainFrame : public CBCGPFrameWnd
{	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	CSplitterWnd m_wndSplitter;	
	BOOL m_bSetOne;			// SetOne操作标志
	BOOL m_bAScanIsOpen;	//A扫开关
	BOOL m_bSelChannel;
	CFeatureInfoWnd* m_pFeatureWnd;	// 特征窗口
	CDacCurveDlg* m_pDacCurveDlg;		// 自定义DAC曲线
	CTcgCurveDlg* m_pTcgCurveDlg;		// 自定义TCG曲线
	BOOL m_bSaveAScan;
	CCScanDlgBar m_wndCScanDlgBar;//直尺测量工具栏(不能设置为私有)
	CString m_strDataFolder;
	BOOL m_bDacWndDisplayed;
	BOOL m_bTcgWndDisplayed;
	CProbeSet m_ProbeSet;		//探伤设置
	BOOL *m_bPlateStart;	//A扫开始标志位
private:
	BOOL m_bIsRunning;		// 是否运行标志
	BOOL m_bFeatureWndDisplayed;
	BOOL m_bCardInit;		// 卡初始化标志

	BOOL *m_bPlateEnd;		// A扫结束标志

	CImageList m_ilStandard;
	CImageList m_ilDrawTu;
	CBCGPToolBarImages m_imageStandard;
//	CBCGPToolBar  m_ToolBar;	// 自定义标准工具栏
	// WangQianfeng add start on 2012-08-22
	CBCGPToolBar			m_wndChannelToolBar;
	CBCGPToolBar			m_wndMechanicalToolBar;
	CBCGPToolBar			m_wndCScanParamToolBar;
	// WangQianfeng add end on 2012-08-22
//	CBCGPToolBar  m_tuToolBar;	// 绘图工具栏
//	CBCGPToolBar  m_viewToolBar;// 其他工具栏
	DWORD m_dwTimeCount;	// wangbingfu add on 2014-12-31

// 操作
public:
	CChannelInfo GetChannelInfo(const WORD& wChannelNo);
// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 控件条嵌入成员
	CBCGPStatusBar  m_wndStatusBar;
	CBCGPToolBar m_wndToolBar;
	//2013-06-03 wangqianfeng add start
	BOOL m_bPressDirectKeys;//是否按过键盘方向键
	//2013-06-03 wangqianfeng add end
	// WangQianfeng delete start on 2013-06-28
	BOOL m_bBScanDisplay;		// B扫窗口显示标志
	// WangQianfeng delete end on 2013-06-28
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// WangQianfeng add start on 2013-06-03
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// WangQianfeng add end on 2013-06-03
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ResetView();
	void HandStart(int type);//手动运动函数，单独控制轴移动方向
	void HandStop();//运行停止函数

private:
	BOOL m_bPathDisplay;		// 运动控件窗口显示标志
	BOOL m_bAScanDisplay;		// A扫窗口显示标志
	BOOL m_bCScanDisplay;		// C扫窗口1显示标志
	BOOL m_bCScanDisplay2;		// C扫窗口2显示标志
	// WangQianfeng delete start on 2013-06-28
//	BOOL m_bBScanDisplay;		// B扫窗口显示标志
	// WangQianfeng delete end on 2013-06-28
	BOOL m_bSplitterCreated;	// 拆分窗口创建标志
public:
	CChannelInfoDockView m_wndChannelInfo;		// 通道信息控制面板
	CMechanicalControl m_wndMechanicalControl;	// 机械控制面板
	CCScanParamDockBar m_wndCScanParamDockBar;	// C扫控制面板
	CTaskDockView m_wndTaskDockBar;				// 任务面板  Song Chenguang add on 2013-04-16
// songchenguang add start 2012-01-12
public:
	CARSSocket* m_AscanSocket;
//	CWSocket* m_CommandSocket;
	CCommandSocket* m_CommandSocket;
	CAScanDataSocket* m_AScanDataSocket;	// A扫数据socket Song Chenguang add on 2013-12-17
	CDataSocket* m_DataSocket;
	WORD m_ReSendNum;
	WORD m_ReSendMaxNum;
	CString m_strIP;
	int m_nPort;
	int m_nNetMonitorCount;
	int m_nNetFeatureCount;
public:
	void TransAScanData(void);
	void StopTransAscanData(void);
	BOOL SendData( const char* data, const int& len, const int& nChannelIndex);
	void TransFeatureData();
	void StopTransFeatureData();
// songchenguang add start 2012-01-12
private:
	int GetDisplayedViewNum();
	BOOL Login2Database();
	afx_msg void OnPathView();
	afx_msg void OnUpdatePathView(CCmdUI *pCmdUI);
	afx_msg void OnAScanView();
	afx_msg void OnUpdateAScanView(CCmdUI *pCmdUI);
	afx_msg void OnCScanView();
	afx_msg void OnUpdateCScanView(CCmdUI *pCmdUI);
	// WangQianfeng delete start on 2013-06-28
 	afx_msg void OnBScanView();
 	afx_msg void OnUpdateBScanView(CCmdUI *pCmdUI);
	// WangQianfeng delete end on 2013-06-28
	afx_msg void OnStartAScan();
	afx_msg void OnStopAScan();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();	
	afx_msg void OnChannelPanel();
	afx_msg void OnUpdateChannelPanel(CCmdUI *pCmdUI);
	afx_msg void OnMechanicalControl();
	afx_msg void OnUpdateMechanicalControl(CCmdUI *pCmdUI);
	// WangQianfeng add start on 2013-06-28
	afx_msg void OnPictrueParam();
	afx_msg void OnUpdatePictrueParam(CCmdUI *pCmdUI);
	// WangQianfeng add end on 2013-06-28
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI *pCmdUI);
	afx_msg void OnViewStandardBar();
	afx_msg void OnUpdateViewStandardBar(CCmdUI *pCmdUI);
	afx_msg void OnViewDrawBar();
	afx_msg void OnUpdateViewDrawBar(CCmdUI *pCmdUI);
	afx_msg void OnViewBar();
	afx_msg void OnUpdateViewBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStartAScan(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopAScan(CCmdUI *pCmdUI);
	afx_msg void OnCscanParam();
	afx_msg void OnUpdateCscanParam(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFeatureInfo(CCmdUI *pCmdUI);
	//Machao delete start on 2013-06-27
//	afx_msg void OnFeatureInfo();
	//Machao delete end on 2013-06-27
	afx_msg void OnDacUserDefine();
	afx_msg void OnDacStandard();
	afx_msg void OnUpdateDacStandard(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDacUserDefine(CCmdUI *pCmdUI);	
	LRESULT OnCloseFeatureInfoWnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSaveDataDefine();
	afx_msg void OnUpdateSaveAScan(CCmdUI *pCmdUI);
	afx_msg void OnCscanRulerMeasure();
	afx_msg void OnUpdateCscanRulerMeasure(CCmdUI *pCmdUI);
	afx_msg void OnDacCurve();
	afx_msg void OnUpdateDacCurve(CCmdUI *pCmdUI);
	afx_msg void OnDrawArc();
	afx_msg void OnDrawDao();
	afx_msg void OnDrawSelect();
	afx_msg void OnDrawHole();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawArcCenetr();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawCircleSan();
	afx_msg void OnDrawArcline();
	afx_msg void OnDrawRect();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawPolygon();
	afx_msg void OnDrawRunway();
	afx_msg void OnDecompose();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditFind();
	afx_msg void OnEditShai();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditFan();
	afx_msg void OnEditMirror();
	afx_msg void OnEditRota();
	afx_msg void OnToolCan();
	afx_msg void OnToolColor();
	afx_msg void OnToolJxian();
	afx_msg void OnToolJpian();
	afx_msg void OnToolEdit();
//	afx_msg void OnToolReset();
	afx_msg void OnToolRun();
	afx_msg void OnIpConfig();
	afx_msg void OnViewShi();
	afx_msg void OnViewTu();
	afx_msg void OnViewWindow();
	afx_msg void OnRotateImage();
	afx_msg void OnCscanSmooth();
	afx_msg void OnCscanMf();
	afx_msg void OnSetUnit();
	afx_msg void OnRulerMeasure();
	afx_msg void OnTriangleMeasure();
	afx_msg void OnEllipseMeasure();
    afx_msg void OnPolygonMeasure();  //yachang.huang add start on 2012-5-31
	afx_msg void OnMeasureOnOff();
	afx_msg void OnButtonReposition();
	afx_msg void OnButtonRun();
	afx_msg void OnButtonSysparam();
	afx_msg void OnProbeSet();		//探伤设置
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDataDirectorySet();
	afx_msg void OnUpdateMeasureOnOff(CCmdUI *pCmdUI);
	afx_msg void OnCscanParam2();
	afx_msg void OnUpdateCscanParam2(CCmdUI *pCmdUI);
	afx_msg void OnCScanView2();
	afx_msg void OnUpdateCScanView2(CCmdUI *pCmdUI);
	afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD_PTR dwData);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnTcgCurve();
	afx_msg void OnUpdateTcgCurve(CCmdUI *pCmdUI);
	// WangQianfeng add start on 2012-08-22
	afx_msg void OnViewCustomize();
	afx_msg void OnChannelToolBarShow();
	afx_msg void OnUpdateChannelToolBarShow(CCmdUI *pCmdUI);		
	afx_msg void OnMechanicalToolBarShow();
	afx_msg void OnUpdateMechanicalToolBarShow(CCmdUI *pCmdUI);
	afx_msg void OnCScanparamToolBarShow();
	afx_msg void OnUpdateCScanparamToolBarShow(CCmdUI *pCmdUI);

	afx_msg void OnReadParam();
	afx_msg void OnSaveParam();
	
	afx_msg void OnOpenPathFile();
	afx_msg void OnSavePathFile();

	afx_msg void OnSaveData();
	afx_msg void OnReadData();
	afx_msg void OnReadCParam();
	afx_msg void OnSaveCParam();
	// WangQianfeng add end on 2012-08-22
public:
	CSCAN_VERSION_DATA_t* m_stYiQiVer;	// 下位机版本信息
private:
	HDEVNOTIFY m_hDevNotify1;	// 加密狗...
	HDEVNOTIFY m_hDevNotify2;	//
	void InitDogNotify();		// 初始化狗消息句柄
	// wangbingfu add start on 2013-01-09
public:
	int m_nPipeChannelSelect[CHANNEL_NUM];
	int m_nCScanSelect[CSCAN_VIEW_NUM];
	int m_nAxisSelect; //平面选择
	float m_fPipeProbeAngle;	// 管材探头角度
public:
	void ReadPipeIni();// const CString& csFilePath);
	void WritePipeIni();//(const CString& csFilePath);
	// wangbingfu add end on 2013-01-09
public:
	BOOL m_bMoinitorDataRun;	// 监视数据Socket是否关闭标志  Song Chenguang add on 2013-04-03
public:
	afx_msg void OnTaskToolbar();
public:
	afx_msg void OnUpdateTaskToolbar(CCmdUI *pCmdUI);
	//Machao add start on 2013-06-27
	afx_msg void OnFeatureInfo();
	//Machao add end on 2013-06-27
	//Song Chenguang add start on 2013-07-11
#if _HAFEI
	// 尾减管参数
public:
	float m_f11LayerMin;	// 11层厚度最小值
	float m_f11LayerMax;	// 11层厚度最大值
	float m_f14LayerMin;	// 14层厚度最小值
	float m_f14LayerMax;	// 14层厚度最大值
	float m_f17LayerMin;	// 17层厚度最小值
	float m_f17LayerMax;	// 17层厚度最大值
	float m_f18LayerMin;	// 18层厚度最小值
	float m_f18LayerMax;	// 18层厚度最大值
	float m_fWeiJianW1;		//W1直径 wangbingfu add on 2014-11-03
	//Song Chenguang add end on 2013-07-11
	// machao add start on 2013-08-09
	// 展开图参数
	float m_nLeft1Start;
	float m_nLeft1Stop;
	float m_nLeft2Start;
	float m_nLeft2Stop;
	float m_nRight1Start;
	float m_nRight1Stop;
	float m_nRight2Start;
	float m_nRight2Stop;
	//示意图参数
	float m_nASketch;
	float m_nABSketch;
	float m_nBSketch;
	float m_nBCSketch;
	float m_nCSketch;
	float m_nCDSketch;
	float m_nDSketch;
 	// machao add end on 2013-08-09
public:
	afx_msg void OnTailDecreasePipeParam();
 	CMenu m_menuHafei;	// 哈飞菜单
#endif
	LRESULT OnDialogDestroyed (WPARAM wParam, LPARAM lParam);	// machao add on 2013-08-16
	BOOL m_bDuplicateDrawOnce;	// 重复轨迹只画一次标志
	BOOL m_bDuplicateDrawOnceEn;	// 重复轨迹只画一次标志使能
	DWORD m_dwDuplicateDrawCount;	// 重复轨迹只画一次延迟计数	Song Chenguang add on 2013-09-26
public:
	afx_msg void OnDuplicateDrawOnce();
	afx_msg void OnUpdateDuplicateDrawOnce(CCmdUI *pCmdUI);
	afx_msg void CommandConnect();
	afx_msg void CommandClose();
	afx_msg void OnCommandSConnect();
	afx_msg void OnCommandSClose();
	BOOL m_bChangeZWAixs;	// 新Z,W轴替换原Z,W轴标志(北方重工)	Song Chenguang add on 2013-09-13  
public:
	afx_msg void OnAScanDataStart();
public:
	afx_msg void OnAScanDataStop();
public:
	void SetGateFromFile(const int& nCardIndex, const int& nChannelIndex);
public:
//	BOOL m_bReceiveAScan;	// 是否接收A扫数据 Song Chenguang add on 2013-12-21
};


