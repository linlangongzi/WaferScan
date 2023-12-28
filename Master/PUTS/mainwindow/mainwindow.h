/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: mainwindow.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-04-27
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTranslator>
#include <Qtimer>
#include <QMenuBar>
#include <QMenu>
#include <QSettings>
#include <QLabel>
#include <QThread>
#include <QPushButton>
#include <QCloseEvent>
#include "ToolsDialog.h"
#include "PlateParaSetERPDialog.h"
#include "PlateParaSetDialog.h"
#include "LogOnDialog.h"
#include "TechnologySetDialog.h"
#include "CscanPanel.h"
#include "ChannelPanel.h"
#include "PlateInfoPanel.h" 
#include "headerFile.h"
#include "AECWorkThread.h"
#include "ascan.h"
#include "SimpleFree.h"
#include "IniParams.h"
#include "AddUser.h"
#include "ScanData.h"
#include "DetectListDlg.h"
#include "DataBaseFile.h"
#include "QBiaoDing.h"
#include "DataDefine.h"


class CLogOnDialog;
class CPlateInfo;
class CCscanPanel;
class CChannelPanel;
class CPlateInfoPanel;
class CInspectUser;
class CToolsDialog;
class CPlateParaSetERPDialog;
class CPlateParaSetDialog;
class CTechnologySetDialog;
class CIniParams;
class QAxWidget;
class QMutex;


QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
	MainWindow();
	~MainWindow();
public:
	QSqlDatabase *m_pDatabaseConnection;		// 保存打开的数据库连接
	QTranslator  *m_pTranslator;				// 国际化
	CPlateInfo   *m_pPlateInfo;					//钢板参数信息
	QMap<QString,int> m_FlagMap;				//配置标示集合
	QString m_strIniFlieName;					//配置文件名
	QSettings *m_pSettings;
	CToolsDialogInfo *m_pMToolsDialogInfo;		//工具栏对话框信息
	AScan *m_pAscan;							//A扫界面
	CChannelPanel  *m_pDockWidgetChannelCtr; 	//通道控制面板
private:
	//工具栏
	QToolBar *m_pMainToolBar;
	QToolBar *m_pColorSelectToolBar;
	//C扫面板
	CCscanPanel *m_pCscanView;
	//钢板参数显示面板
	CPlateInfoPanel *m_pDockWidgetParameterDisp;
	//工具条上的按钮
	QComboBox *m_pColorType;
	QLabel  *m_pColorLabel;
	QAction *m_pAscanAction;//A扫
	QAction *m_pPrintAction;//打印
	QAction *m_pParameterAction;//参数设置
	QAction *m_pDetectListAction;//伤点列表
	QAction *m_pPreviousAction;//前一数据
	QAction *m_pNextAction;//下一数据
	QAction *m_pExpandAction;//工具面板
	QAction *m_pDemarcateAction;//标定
	QAction *m_pSelfTestAction;//自检
	QAction *m_pDataScanAction;//数据浏览

	//chenyuzhu add start on 2014.5.7
	QAction *m_pOPC_Start;	//测试模拟开始停止探测按键//调试 chenyuzhu on 2014-10-8	
	//chenyuzhu add end on 2014.5.7
	
	//chenyuzhu add start on 2014-7-23
	QAction *m_pGongYiQieHuan;	//工艺配置切换
	//chenyuzhu add end on 201-7-23

	//状态栏上的控件
	QLabel *m_pSTime;
	QLabel *m_pSStatus;
    QLabel *m_pSOperator;
	QTimer *m_pTimer; 
	QMenu    *m_pSMenu;
	QAction  *m_pSCChinese; //中文
	QAction  *m_pSCEnglish; //中文
    QPushButton  *m_pSChangeLanguage; //语言切换
    QLabel  *m_pIsReset;

	CInspectUser *m_pInspectUser;//用户验证
	CLogOnDialog *m_plogonDialog;     //登录对话框
	CToolsDialog *m_pToolsDialog;//工具面板
	CPlateParaSetERPDialog *m_pERPDialog;//ERP钢板参数设置
	CPlateParaSetDialog   *m_pHandleDialog;//钢板参数设置
	CTechnologySetDialog   *m_pSettingDialog;//工艺设置
	ScanData  *m_pScanData;  //数据浏览对话框
	DetectListDlg *m_pDetectListDlg; // 伤点信息列表
	QBiaoDing *m_pBiaodingDlg; //标定模式选择面板//chenyuzhu add on 2014-11-6
private:
	BOOL m_bBiaoDingReport;
	WORD m_nAECCardsStarted;
public:
	CPLXCommunication m_PLXCommunication;
	AECTHREAD_PARAM m_stThreadParam[MAX_CARD_NO];
	AECWorkThread** m_pAECWorkThread;
	BOOL m_bPlateStartEn;		  // Song Chenguang add on 2013-10-18
	BOOL *m_bPlateStart;
	BOOL m_bIsRunning;
	BOOL *m_bPlateEnd;
	unsigned long m_ElepHandle;
	int m_nTimerID_PROCESS;
	int m_nTimerID_PLC;
	int m_nTimerID_LOG;
	DWORD m_nAECCardNum;
	BOOL m_bCardInit[MAX_CARD_NO];	// 卡初始化标志
	BOOL m_bCacluFlag;
	//不知道下面的变量是做什么
	BOOL m_bNei1DownStartCalc;
	BOOL m_bNei2DownStartCalc;
	BOOL m_bNei3DownStartCalc;
	BOOL m_bNei4DownStartCalc;
public:
	CSimpleFree m_OPCComponent;
	BOOL m_bCardsStart_En;				// 开始使能
	BOOL m_bCardsStop_En;				// 结束使能

	BOOL m_bReportFlaw_En;
	BOOL m_bSetOne_En;					// 允许SetOne

	BOOL m_bCardsStart_Flag;			// 开始标志
	BOOL m_bCardsStop_Flag;				// 结束标志
	BOOL m_bBiaoDingOpen_En;			// 标定打开允许
	BOOL m_bPlateStartFlag[MAX_CARD_NO];    //没用到
	BOOL m_bPlateEndFlag[MAX_CARD_NO];
	// 标注对象指针
	CBiaoZhun* m_pBiaoZhun;			//数据库调出的数据用到的标准
private:
	void Init();
	void InitWindow();
	void InitOthers();					//初始化组件对象和其他参数
	void InitOtherParameters();
	void createToolBars();
	void createDockWidget();
	void createToolsPanel();
	void createActions();
	void createStatusBar();
	void RestoreState();
	void InitPlateInfoWindow();
	bool InitDatabase();				// 初始化连接数据库
	void DrawColorLabel(int flag = 0);	//画ColorLabel背景
	void retranslate();					//语言切换的重写函数
	void UpdateDockWidgetParameterDisp(void);
	bool FilerChangeStatus(void);
	void customEvent(QEvent *event);
	void SaveToolDialogInfo(void);		 //保存工具栏信息
	void InitMapChanel();
	void InitMapForChannel(int& nWuliBegin,int& nChannelNum,CH_CONFIG* stAllCH_CONFIG,int& countFlag,int& nBoxIndex,QVector<ST_CHANEL>& vChannel);


    void testDrawImage();
public:
	void changeEvent(QEvent *e);		 //用来接受语言切换事件
	bool eventFilter(QObject *obj, QEvent *event);   //事件过滤器
	void timerEvent( QTimerEvent *event );
	void OPCRead(HRESULT *pErrors,OPCHANDLE *hServer,int nItemCount);
//	void OPCWrite(HRESULT *Errors,OPCHANDLE *phServer,VARIANT *values,int nItemCount);
	bool OPCWrite(HRESULT *pSErrors,OPCHANDLE* phServer, VARIANT* itemArray,  const unsigned long& ulSize);
	BOOL UpdateMonitorWnd(int nCardNum);
	bool UpdateMonitorWndTemp(int nCardNum);
public slots:
    bool EndReadDetectFile();				//读取伤点文件结束，用于置读取结束标志
    void onComBoChange(int index);		//绘制不同的ColorLabel背景
    void ExpandAction(void);				// 点击这个Button弹出对话框
	void TimerUpdate(void);             //当前时间定时器响应函数
    void OnStatusChangeLanguage(void);	//状态栏的button槽函数
	void OnChangeToChinese(void);		//语言切换到中文
	void OnChangeToEnglish(void);		//语言切换到英文
	void OnParameterAction(void);		//参数设置
	void LeftViewChange(int state);
	void RightViewChange(int state);
	void TopViewChange(int state);
	void BottomViewChange(int state);
	void CScanThicknessWidgetChange(int state); //厚度图
	void CScanFrontWidgetWidgetChange(int state); //正视图
	void CScanSideWidgetChange(int state); //侧视图
	void CScanWholeWidgetChange(int state); //整体视图
	void CsanDetectDoubleClick(QMouseEvent *e);//双击伤点界面用于显示四周伤点图	
	void OnUpdateToolDialogOnCancel(void);
	void OnAscanAction(void);                   //弹出A扫界面
	void AScanSave();//A扫保存
    void OnALLChannelBiaoDing();
	void OnDataScanAction();
	void OnDetectListDlgAction();
	void OnUpdateChannelDock(QList<QString> argList); //响应双击TableView的发出的信号 更新界面信息
	void OnSetPlateID();                              //重设钢板ID
	void PrepareStartDetect(); //一张钢板探完后，重新配置钢板参数，提交OPC
	void OnBiaoDingAction();//chenyuzhu add on 2014-11-6
	//chenyuzhu add start on 2014.5.7
	//测试模拟开始停止探测槽
	void OPCStartOrStop(); 
	//chenyuzhu add end on 2014.5.7
	
	//chenyuzhu add start on 2014-7-23
    //void GongYiQieHuan(); //功能改为重新判伤函数
	//chenyuzhu add end on 2014-7-23
	
	void deleteReDraw();//chenyuzhu add on 2014-6-10 用于接收伤点删除后重新计算并重绘各视图
	void OnBiaodingCheck(int nCheck); //标定选择 //chenyuzh add on 2014-11-6

    void InitLog();                                  
    void OnLiluoBiaoding();
protected: 
	void closeEvent(QCloseEvent *event); 
	void resizeEvent(QResizeEvent * event);

signals:
	void signalPlateScanEnd();			//探伤结束
	void signalAddFlawData(CH_FLAWDATA* pData);  //更新m_flowStore数据，并画图
	void signalPlateScanStart();		//探伤开始

	

};


#endif
