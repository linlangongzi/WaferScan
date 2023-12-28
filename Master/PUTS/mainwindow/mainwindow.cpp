/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: mainwindow.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-04-27
*/
#include <QAction>
#include <QStatusBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QDockWidget>
#include <QToolBar>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDebug>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QSqlError>
#include <QDateTime>
#include <QMutex>
#include <QAxWidget>
#include <QImage>
#include <QModelIndex>
#include <QPalette>
#include <math.h>
#include <time.h>
#include <QTextCodec>

#include "mainwindow.h"
#include "CscanPanel.h"
#include "ToolsDialog.h"
#include "PlateInfoPanel.h"
#include "LogOnDialog.h"
#include "TechnologySetDialog.h"
#include "ChannelPanel.h"
#include "PlateParaSetERPDialog.h"
#include "PlateParaSetDialog.h"
#include "SimpleFree.h"
#include "GlobalFunction.h"
#include "elepopular.h"
#include "EventDefine.h"
#include "opcda.h"
#include "opcda_i.c"
#include "const.h"
#include "IniParams.h"
#include "DataDefine.h"
#include "InspectUser.h"

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)
Q_DECLARE_METATYPE(CH_FLAWDATA)

//chenyuzhu add start on 2014-7-7

//chenyuzhu add end on 2014-7-7
BOOL g_bStart = FALSE;  //探伤是否结束
BOOL g_bStop = FALSE;   //探伤是否结束
HANDLE hMonitorFlawEvent[MAX_CARD_NO]; //用于监视通道信息，报警

QMutex g_csCard;
QMutex g_ItemCS;
QMutex g_CS;
QMutex g_ItemMutex;

HANDLE hOPCInitEvent;
HANDLE hOPCReadEvent;	// OPC读值
HANDLE hOPCWriteEvent;	// OPC写值
HANDLE g_hWaveReadyEvent;	// 波形数据准备好与否事件

CH_CONFIG allCH_CONFIG[MAX_CHANNEL_NO];

CH_CONFIG_GLOBAL ch_config_global;
QMap<int,QVector<ST_CHANEL> > m_mapChanel;  //KEY值为探头盒号

PLATE_INFO g_Info[1]={3800.0,2800.00,30.000,0,"779999966_0"};

GlobalItemValue g_ItemValue;		// opc变量
CIniParams g_IniParams;             // 所有的配置信息（包括加密狗）
bool g_DepthFilterSwitch = FALSE;	// 深度过滤全局开关

QAxWidget *g_pPlateDraw;			//用于调用OCX
char *g_pBuf4;						//用于存储点伤数据的内存区域
unsigned int g_nBuf4Size;			//点伤大小
bool g_bReadyDraw = false;			//数据从文件中读出，并且已加入到控件计算完毕，等待画图

NeiTanBoxStartNum g_NeiTanBoxStartNum;
QString g_strUserPer;
DWORD g_dwFlawCount = 0;
unsigned int g_plateEndFlag = 0;   //此变量用于判断所有卡打开的标志
int nCardEndStauts = 0;
int nPlate = 0;
P9054_HANDLE hDev;				//检测 UST PLC SERVER（多功能卡的句柄） 
AScan *g_pAscan = NULL;                //A扫界面指针
AECWorkThread** g_pAECWorkThread = NULL; //指向线程对象数组的指针
int g_nSetOneChannel = 0;         //当前下发参数的物理通道号
int g_nComboxColor = 0;		//比色表选择下拉列表值chenyuzhu add on 2014-6-13
bool g_bQieHuan = false; //chenyuzhu add on 2014-10-10
bool g_pbBiaoding = false; //chenyuzhu addon 2014-11-7
QString g_Renyuan;
QString g_Banji;
OtherParameters g_OtherParameters;
GuoLvQueXian g_GuoLvQueXian;

extern DWORD g_dwActiveFlawOrBottom;    //伤波 底波
extern int g_nSpeed;
extern int g_nADCFreq;
extern int g_nADCaiYangShu;
extern QString g_strGongYi;
extern DWORD g_dwActiveChannel;  
extern DWORD g_dwActiveChannelLuoJi;//chenyuzhu add on 2014-6-20  
extern CLSID clsOPCServer;
extern CH_FLAWDATA g_MonitorFlaw[MAX_CARD_NO];
extern QList<CH_FLAWDATA>* g_LostWaveDatas[MAX_CHANNEL_NO];
extern QSqlDatabase *g_sqlDB;
extern QString g_strJiBie;
extern QString g_strBiaoZhun; //chenyuzhu add on 2014-9-29
extern float g_GainSale[500];

MainWindow::MainWindow()
{
	Delay(10000);
	InitOtherParameters(); // wangwei add on 20160114
	//chenyuzhu modify start on 2014-7-23
	QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";  // 设备文件
	QFile file1(strFileName);
	bool bRet1 = file1.open(QIODevice::ReadOnly);
	int filesize = file1.size();
	int oldsize = sizeof(SHEBEICANSHU_OLD);
	int newsize = sizeof(SHEBEICANSHU);
	if (!bRet1)
	{
		InitLog();                                 //chenyuzhu add on 2014-6-25 初始化日志文件 改为为设备文件赋初始值
	}
	else if (filesize == oldsize)
	{
		ReplaceOldToNew();
	}
	else
	{
		file1.close();
	}

	//chenyuzhu modify end on 2014-7-23
	Init();                                    //初始化成员变量
	InitOthers();
	InitWindow();                              //创建所有的窗口对象
	for (UINT i=0; i < m_nAECCardNum; i++)                     //创建线程对象，并把参数传给线程对象
	{
		m_pAECWorkThread[i] = new AECWorkThread();
		m_pAECWorkThread[i]->m_pMainWindow = this;             //获取MainWindow对象指针
		//设置相应线程参数
		m_stThreadParam[i + 1].pPLXCommunication = &m_PLXCommunication;
  		m_stThreadParam[i + 1].pAllConfig = allCH_CONFIG;
		m_stThreadParam[i + 1].index = i;
		m_stThreadParam[i + 1].qObject = this;
		m_stThreadParam[i + 1].wSelChannel = 0;
		m_pAECWorkThread[i]->m_pAecThreadParam = m_stThreadParam[i + 1];//第0个是多功能卡
		m_pAECWorkThread[i]->start(QThread::NormalPriority);         //启动合卡线程
	} 

	g_pAECWorkThread = m_pAECWorkThread;
	// Song Chenguang add start on 2013-12-03
	// 创建事件
	hOPCReadEvent = CreateEvent(NULL, TRUE, FALSE, L"OPCReadValue");
	hOPCWriteEvent = CreateEvent(NULL, TRUE, FALSE, L"OPCWriteValue");
	g_hWaveReadyEvent = CreateEvent(NULL, TRUE, FALSE, L"WaveDataFlag");  //画图数据事件
	// Song Chenguang add end on 2013-12-03
	//启动A扫
// 	m_pAscan->showMaximized();                                         //最大化显示A扫界面by chenyuzhu 2013-12-03//chenyuzhu modify on 2014-10-30
	g_pAscan = m_pAscan;
	
	m_nTimerID_PROCESS = startTimer(30);                             //设置线程定时器
	m_nTimerID_PLC = startTimer(50);                                 //TIMER_PLC_MONITOR
	m_nTimerID_LOG = startTimer(30);                                 //相应的函数给注释掉了，不知道干什么用的
	
	hDev = m_stThreadParam[0].hDev;							         //给别的界面用的，实时显示状态
	m_pCscanView->m_nTimerSTID = m_pCscanView->startTimer(1000);     //开始PLC UST SERVER 状态检测定时器
	m_pBiaoZhun = NULL;
}
MainWindow::~MainWindow()
{	
	for (UINT i=0; i < m_nAECCardNum; i++)                     //创建线程对象，并把参数传给线程对象
	{
		if (NULL != m_pAECWorkThread[i])
		{
			delete m_pAECWorkThread[i];
		}
	}

 	if (m_bPlateStart != NULL)
 	{
 		delete[] m_bPlateStart; 
 		m_bPlateStart = NULL;
 	}
 
 	if (m_bPlateEnd != NULL)
 	{
 		delete[] m_bPlateEnd; 
 		m_bPlateEnd = NULL;
 	}
	if (m_pAscan != NULL)
	{
		delete  m_pAscan;
		m_pAscan = NULL;
	}	
	if (NULL != m_pScanData)
	{
		delete m_pScanData;
		m_pScanData = NULL;
	}
	if (NULL != m_pDetectListDlg)
	{
		delete m_pDetectListDlg;
		m_pDetectListDlg = NULL;
	}
	//chenyuzhu add start on 2014-11-6
	if (NULL != m_pBiaodingDlg)
	{
		delete m_pBiaodingDlg;
		m_pBiaodingDlg = NULL;
	}
	//chenyuzhu add end on 2014-11-6
	for (int i=0; i<MAX_CHANNEL_NO; i++)
	{
		if (NULL != g_LostWaveDatas[i])
		{
			delete g_LostWaveDatas[i];
			g_LostWaveDatas[i] = NULL;
		}
	}
}

//chenyuzhu add start on 2014-6-25 初始化日志文件//改为为设备文件赋初始值用
void MainWindow::InitLog()                
{
	int NeiTan1Num = g_OtherParameters.insideCar1BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探1通道数量
	int NeiTan2Num = g_OtherParameters.insideCar2BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探2通道数量
	int nBianAllNum = g_OtherParameters.insideCar1PhysicalChannelStart;										// 边探所有通道数量
	int nBian1Begin = g_OtherParameters.edgeCar1PhysicalChannelStart;										// 边探1物理通道号起始
	int nBian2Begin = g_OtherParameters.edgeCar2PhysicalChannelStart;										// 边探2物理通道号起始
	int nNei1Begin = g_OtherParameters.insideCar1PhysicalChannelStart;										// 内探1物理通道号起始
	int nNei2Begin = g_OtherParameters.insideCar2PhysicalChannelStart;										// 内探2物理通道号起始
	int nBian1LogicalBegin = g_OtherParameters.edgeCar1LogicalChannelStart;									// 边探1逻辑通道号起始
	int nBian2LogicalBegin = g_OtherParameters.edgeCar2LogicalChannelStart;									// 边探2逻辑通道号起始
	int nNei1LogicalBegin = g_OtherParameters.insideCar1LogicalChannelStart;								// 内探1逻辑通道号起始
	int nNei2LogicalBegin = g_OtherParameters.insideCar2LogicalChannelStart;								// 内探2逻辑通道号起始
	
	//从数据库中读取文件/SheBeifile.dbfl
	QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";
	QFile file1(strFileName);
/*	bool bRet1 = file1.open(QIODevice::ReadOnly);

	if (!bRet1)
	{
		QMessageBox::warning(NULL,"InitConfig",QObject::tr("SheBeifile.dbfl打开失败"));
		return ;
	}
	file1.seek(0);
	SHEBEICANSHU SheBeiCanShu;
	quint64 nBytesRead = file1.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	file1.close();*/
	//chenyuzhu add end on 2014-7-8
	SHEBEICANSHU SheBeiCanShu;
	//chenzyuzhu add start 2014-7-10 给数据文件初始值用的
	//////////////////////////////////ChannelParam////////////////////////////////////////
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{	
		SheBeiCanShu.ChannelParam[i].nNum = i;
		if(i >= nBian1Begin && i < nBian2Begin)
			SheBeiCanShu.ChannelParam[i].nRefNum = i + nBian1LogicalBegin - nBian1Begin;
		if(i >= nBian2Begin && i < nNei1Begin)
			SheBeiCanShu.ChannelParam[i].nRefNum = i + nBian2LogicalBegin - nBian2Begin;
		if(i >=nNei1Begin && i < nNei1Begin + NeiTan1Num)
			SheBeiCanShu.ChannelParam[i].nRefNum = i + nNei1LogicalBegin - nNei1Begin;
		if(i >= nNei2Begin )
			SheBeiCanShu.ChannelParam[i].nRefNum = i + nNei2LogicalBegin - nNei2Begin;

		SheBeiCanShu.ChannelParam[i].nZhongZhiDian = 511;		//中值点
		strcpy_s(SheBeiCanShu.ChannelParam[i].strIsOpen, "开");				//通道开启
		SheBeiCanShu.ChannelParam[i].fICIV = 26;				//底波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fTCIV = 12;				//伤波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fDelay = 13.1;		//延迟(mm)
		SheBeiCanShu.ChannelParam[i].fShangmangqu = 1.0;		//上盲区(mm)
		SheBeiCanShu.ChannelParam[i].fXiamangqu = 2.0;		//下盲区(mm)
		SheBeiCanShu.ChannelParam[i].fDibozhamenkuandu = 8.0;		//底波闸门宽度(mm)
		SheBeiCanShu.ChannelParam[i].fDiBoYuZhi = 20;		//底波阈值(%)
		SheBeiCanShu.ChannelParam[i].fYiZhi = 0.1;	//抑制(%)
		SheBeiCanShu.ChannelParam[i].fShangDiCha = 14;//伤波增益差值(db)
		SheBeiCanShu.ChannelParam[i].nDemarcateResult = 1;
		SheBeiCanShu.ChannelParam[i].fZengyixishu = 1.0;

		if ((i > 2 && i < 8 ) || (i > 10 && i < 16))
		{
			strcpy_s(SheBeiCanShu.ChannelParam[i].strIsOpen, "关");				//通道开启
		}
	}
	//////////////////////////////////TanShangGongYi////////////////////////////////////////

	strcpy_s(SheBeiCanShu.TanShangGongYi.strJianBoMoShi, "全波");//检波模式
	SheBeiCanShu.TanShangGongYi.fShangMangQu = 1;//上盲区
	SheBeiCanShu.TanShangGongYi.fXiaMangQu = 2;	//下盲区
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDACOpen, "关");		//DAC开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAuToZengYiOpen, "关");		//自动增益开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAutoHuaMenOpen, "关");	//自动闸门跟踪
	strcpy_s(SheBeiCanShu.TanShangGongYi.strBaoJing, "开");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strYiQiXingHao, "PUTS-ARS144");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strName, "黑材");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strPlateType, "试板");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDetector, "ARS-P001");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangMethod, "水膜法");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTangShangBiaoZhun, "GBT2970");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieBody, "I");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieEdge, "");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangLinMingDu, "Pha5");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieEdge, "");
	SheBeiCanShu.TanShangGongYi.fTouWeiMangQu = 0;
	SheBeiCanShu.TanShangGongYi.fCeMangQu = 0;
	SheBeiCanShu.TanShangGongYi.nNetTanTongDaoShu = 132;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strOuHeJi, "水");
	SheBeiCanShu.TanShangGongYi.fHouDuChaoCha = 5;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strHouDuAutoRead, "开");
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDuiBiShiYang, "ARS-T001");
	SheBeiCanShu.TanShangGongYi.fChuShiZengYi = 12;
	SheBeiCanShu.TanShangGongYi.fDiBoYuZhi = 20;
	SheBeiCanShu.TanShangGongYi.fChuShiYiZhi = 0.1;
	SheBeiCanShu.TanShangGongYi.fShaoMiaoJianGe = 3;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strModel, "A");
	SheBeiCanShu.TanShangGongYi.nflag = 0;

	////////////////////////////////////Plate_Type//////////////////////////////////////
	SheBeiCanShu.Plate_Type.fLWaveSpeed = 5970;
	SheBeiCanShu.Plate_Type.fTWaveSpeed = 3000;

	///////////////////////////////////////YiQi///////////////////////////////////
	strcpy_s(SheBeiCanShu.YiQiParam.strName, "PUTS-ARS144");
	SheBeiCanShu.YiQiParam.nTouWeiTanTongDaoShu = 0;
	SheBeiCanShu.YiQiParam.nTouWeiTanLogicBegin = 0;

	SheBeiCanShu.YiQiParam.fTanTouZhongXinJuLi = 20;
	SheBeiCanShu.YiQiParam.fADCFreq = 50;
	SheBeiCanShu.YiQiParam.nADCaiYangShu = 10;
	SheBeiCanShu.YiQiParam.nBianTan1TongDaoShu = 3;        //边探1通道数
	SheBeiCanShu.YiQiParam.nBianTan2TongDaoShu = 3;		//边探2通道数
	SheBeiCanShu.YiQiParam.nNeiTan1TongDaoShu = NeiTan1Num;		//内探1通道数
	SheBeiCanShu.YiQiParam.nNeiTan2TongDaoShu = NeiTan2Num;			//内探2通道数

	SheBeiCanShu.YiQiParam.nBianTan1WuLiBegin = nBian1Begin;			//边探1物理开始通道号
	SheBeiCanShu.YiQiParam.nBianTan2WuLiBegin = nBian2Begin;			//边探2物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan1WuLiBegin = nNei1Begin;				//内探1物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan2WuLiBegin = nNei2Begin;				//内探2物理开始通道号

	SheBeiCanShu.YiQiParam.nBianTan1LogicBegin = nBian1LogicalBegin;
	SheBeiCanShu.YiQiParam.nBianTan2LogicBegin = nBian2LogicalBegin;
	SheBeiCanShu.YiQiParam.nNeiTan1LogicBegin = nNei1LogicalBegin;
	SheBeiCanShu.YiQiParam.nNeiTan2LogicBegin = nNei2LogicalBegin;

	SheBeiCanShu.YiQiParam.fDistanceTanCha2NeiTanYi1 = 0;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2NeiTanYi2 = 0;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2BianTan = 0;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2DaoLun = 0;
	SheBeiCanShu.YiQiParam.nWuLiTongDaoShu = 141;
	SheBeiCanShu.YiQiParam.fCengXiangSaoChaMaxDistance = 2;
	SheBeiCanShu.YiQiParam.nTouWeiTanWuLiBegin = 0;
	SheBeiCanShu.YiQiParam.fCeChang1 = 286;
	SheBeiCanShu.YiQiParam.fCeChang2 = 286;
	SheBeiCanShu.YiQiParam.fCeChang3 = 286;
	SheBeiCanShu.YiQiParam.fBianTanXiaoChe1 = 150;
	SheBeiCanShu.YiQiParam.fBianTanXiaoChe2 = 150;
	SheBeiCanShu.YiQiParam.fTouTanMangQu = 0;
	SheBeiCanShu.YiQiParam.fWeiTanMangQu = 0;
	SheBeiCanShu.YiQiParam.fLeftTanMangQu = 0;
	SheBeiCanShu.YiQiParam.fRightTanMangQu = 0;
	SheBeiCanShu.YiQiParam.fLeftTanNeiMangQu = 0;
	SheBeiCanShu.YiQiParam.fRightTanNeiMangQu = 0;
	strcpy_s(SheBeiCanShu.YiQiParam.strTanShangMethod, "水膜法");
	strcpy_s(SheBeiCanShu.YiQiParam.strOuHeJi, "水");

	//////////////////////////////////////////////////////////////////////////
	if (!file1.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	 quint64 nBytesWrite = file1.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	//////////////////////////////////////////////////////////////////////////
     //chenzyuzhu add end on 2014-7-10 给数据文件初始值用的
	 file1.close();
}

void MainWindow::OnLiluoBiaoding()
{
    InitLog();
 //   INIT_CONFIG(allCH_CONFIG, &ch_config_global);
}
//chenyuzhu add end on 2014-6-25



void MainWindow::Init()
{
	m_pSettings = NULL;			// 
 	m_pCscanView = NULL;	// C扫视图
	m_pERPDialog = NULL;             //ERP设置对话框
	m_pHandleDialog = NULL;          //手动设置参数对话框
	m_pTranslator = NULL;
	m_pPlateInfo = NULL;
	m_pMToolsDialogInfo = NULL;
	m_pAscan = NULL;
	m_bCardsStart_En = TRUE;        //为了进入电气开始判断
	m_bBiaoDingOpen_En = FALSE;
	m_bSetOne_En = FALSE;
	m_bBiaoDingReport = FALSE;
	m_nAECCardsStarted = 0;
	m_bCacluFlag = FALSE;
	m_bCardsStart_Flag = FALSE;
	m_bPlateStartEn = FALSE;

	m_bNei1DownStartCalc = FALSE;
	m_bNei2DownStartCalc = FALSE;
	m_bNei3DownStartCalc = FALSE;
	m_bNei4DownStartCalc = FALSE;
	//dong qi add start on 2013-11-25
	memset(m_bCardInit, 0, MAX_CARD_NO * sizeof(MAX_CARD_NO));// 将卡初始化标志置 否
	memset(g_LostWaveDatas, 0, MAX_CHANNEL_NO * sizeof(QList<CH_FLAWDATA>*));// 将失波数据容器清空
//	QList<CH_FLAWDATA>* g_LostWaveDatas[MAX_CHANNEL_NO];

    //dong qi add end on 2013-11-25
	g_pPlateDraw = new QAxWidget();   //初始化 OCX对象
	g_pPlateDraw->setControl(QString::fromUtf8("{60705132-106E-462D-9B63-81653234B9C4}"));	// 注册

	m_strIniFlieName = QDir::currentPath() += "/PUTS.ini";
	m_pSettings = new QSettings(m_strIniFlieName, QSettings::IniFormat);   //配置文件对象
	m_pTranslator = new QTranslator;									   //国际化对象
	m_pPlateInfo  = new CPlateInfo;                                        //参数信息	
	m_pMToolsDialogInfo = new CToolsDialogInfo;                            //工具栏对话框信息
	m_pScanData = new ScanData(this);                                      //数据查询对话框
	m_pDetectListDlg = new DetectListDlg(this);							   //伤点信息列表对话框	
	m_pBiaodingDlg = new QBiaoDing(this);								//chenyuzhu add on 2014-11-6
	//标示初始化
	m_FlagMap.insert("MTechnologySetFlag",0);                             
	m_FlagMap.insert("MPlateParaFlag",0);
	//设置MainWindow
	setWindowTitle( tr("武钢中厚钢板超声波自动探伤系统")); //chenyuzhu modify on 2014-9-29 
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73)"));
	setMinimumSize(907,720);
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	QIcon logo;
	logo.addFile(QString::fromUtf8(":/Resources/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(logo);
	qRegisterMetaType<CH_FLAWDATA>();
}
void MainWindow::InitWindow()
{
	//创建各界面
	createToolBars();    //创建工具栏和相应的控件
	createActions();     //创建工具栏上的按钮
	createToolsPanel();  //创建工具面板对象
	//登陆对话框
	m_plogonDialog = new CLogOnDialog(m_pDatabaseConnection);
	m_plogonDialog->exec();
	//根据权限设置用户管理功能
	//根据权限设置按钮功能
	if ( QString("admin") != g_strUserPer)
	{
		m_pToolsDialog->m_pUserManager->m_pbAddUser->setEnabled(false);
		m_pToolsDialog->m_pUserManager->m_pbAddUser->setStyleSheet(" background-color: gray");
		m_pToolsDialog->m_pUserManager->m_pbEditUserPer->setEnabled(false);
		m_pToolsDialog->m_pUserManager->m_pbEditUserPer->setStyleSheet(" background-color: gray");
		m_pToolsDialog->m_pUserManager->m_pbDeleteUser->setEnabled(false);
		m_pToolsDialog->m_pUserManager->m_pbDeleteUser->setStyleSheet(" background-color: gray");
	}
	//钢板信息显示面板
	m_pDockWidgetParameterDisp = new CPlateInfoPanel(this);
	m_pDockWidgetParameterDisp ->setWindowTitle(tr("钢板参数"));
	m_pDockWidgetParameterDisp->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	m_pDockWidgetParameterDisp->setMaximumSize(QSize(600, 1080));
	addDockWidget(Qt::RightDockWidgetArea, m_pDockWidgetParameterDisp);
	//配置对话框
	m_pSettingDialog = new CTechnologySetDialog(m_FlagMap,*m_pPlateInfo,m_pDatabaseConnection,this);
	m_pSettingDialog->exec();

	createDockWidget();     //通道面板 钢板参数面板  C扫面板
	InitMapChanel();		//根据已取出的通道信息，初始化m_mapChanel
    m_pAscan = new AScan(NULL,Qt::Dialog | Qt::WindowMinMaxButtonsHint);  //创建A扫
	RestoreState();  //读取配置文件，恢复上一次关闭状态
	//读取配置文件信息判断用哪种方式配置参数
	int value = m_pSettings->value("PlateInfo/InputMode","").toInt();
	m_pERPDialog = new CPlateParaSetERPDialog(m_FlagMap,*m_pPlateInfo,m_pDatabaseConnection,this);
	m_pHandleDialog = new CPlateParaSetDialog(m_FlagMap,*m_pPlateInfo,m_pDatabaseConnection,this);
/*	//chenyuzhu add start on 2014-7-22
	int ret = QMessageBox::information(this,tr("MainWindow::InitWindow()"),tr("是否用上一次配置?"),QMessageBox::Yes | QMessageBox::No);
	if(QMessageBox::No == ret)
	{
		m_pScanData->exec();
	}
	//chenyuzhu add end on 2014-7-22
	if(1 == value)
	{
		//钢板参数对话框
	part1:
		m_pERPDialog->exec();
		if (m_pERPDialog->m_bIsSwitch)
		{
			m_pERPDialog->m_bIsSwitch = false;
			m_pHandleDialog->exec();
			if (m_pHandleDialog->m_bIsSwitch)  //点切换
			{
				m_pERPDialog->m_bIsSwitch = false;
				goto part1;
			}
			if (!m_pHandleDialog->m_bIsOK)     //点取消
			{
				exit(0);
			}
			else                              //点确定
			{
				UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
				m_pDockWidgetParameterDisp->m_prbMInput->click();
			}			
		}
		if (!m_pERPDialog->m_bIsOK)
		{
			exit(0);
		}
		UpdateDockWidgetParameterDisp();	//更新到形似钢板参数显示界面
		m_pDockWidgetParameterDisp->m_prbERP->click();
	}
	else
	{	
	part2:
		m_pHandleDialog->exec();
		if (m_pHandleDialog->m_bIsSwitch)
		{
			m_pERPDialog->m_bIsSwitch = false;
			m_pERPDialog->exec();
			if (m_pERPDialog->m_bIsSwitch)  //点切换
			{
				m_pHandleDialog->m_bIsSwitch = false;
				goto part2;
			}
			if (!m_pERPDialog->m_bIsOK)     //点取消
			{
				exit(0);
			}
			else                              //点确定
			{
				UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
				m_pDockWidgetParameterDisp->m_prbERP->click();
			}			
		}
		if (!m_pHandleDialog->m_bIsOK)
		{
			exit(0);
		}
		else
		{
			UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
			m_pDockWidgetParameterDisp->m_prbMInput->click(); 
		}
	}

	//dongqi add start on 2013.12.2
	//在参数配置中点OK按钮会改变g_Info[nPlate]，g_strBiaoZhun，g_strJiBie
	//首先查询数据库中有没有相应的钢板号，如果有则更新，否则就插入一条数据，将t_Plate表中最大的id保存在m_nPlateId中
	//钢板号对应表t_Plate中name字段
/*	
	//chengyuzhu modify start on 2014-7-9
	
	QSqlQuery query(*g_sqlDB);
	QString strSQL = QString("%1%2%3").arg("select * from t_Plate where name = '").arg(g_Info[nPlate].plateno).arg("'");
	query.exec(strSQL);
	query.next();*/
/*	float fTempWeight = (g_Info[nPlate].length/1000)*(g_Info[nPlate].width/1000)*(g_Info[nPlate].height/1000)*7.85;
	QString fWeight;
	fWeight.sprintf("%.2f",fTempWeight);
	fTempWeight = fWeight.toFloat();
	QString strPlateType;  //保存与与探伤工艺相对应的钢板类型（PlateType）
	
	//chenyuzhu add start on 2014-7-9
	
	//读取数据库读出platefile.dbfl文件（未添加）
	
	QString strFileName = QDir::currentPath()+ "/platefile.dbfl";
	QFile file(strFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("platefile.dbfl打开失败"));
		//return ;
	}
	PLATEFILE_PARAM PlateParam;

	file.seek(0);
	
	quint64 nBytesRead = file.read((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
	file.close();

	strPlateType = g_Info[nPlate].plateno;
	QByteArray ba = strPlateType.toLocal8Bit();
	strcpy_s(PlateParam.strPlateType, ba.data());
	strcpy_s(PlateParam.strName, ba.data());
	PlateParam.fLength = g_Info[nPlate].length;
	PlateParam.fWidth = g_Info[nPlate].width;
	PlateParam.fThick = g_Info[nPlate].fReportHeight;
	PlateParam.fLWaveSpeed = 5970;
	PlateParam.fTWaveSpeed = 3000;
	PlateParam.fWeight = fTempWeight;
	ba = m_pPlateInfo->m_strFurnNumber.toLocal8Bit();
	strcpy_s(PlateParam.strLu, ba.data());
	ba = g_Info[nPlate].plateno.toLocal8Bit();
	strcpy_s(PlateParam.strShenChan, ba.data());
	ba = m_pPlateInfo->m_strContractNum.toLocal8Bit();
	strcpy_s(PlateParam.strHeTong, ba.data());
	ba = g_strJiBie.toLocal8Bit();
	strcpy_s(PlateParam.strGangJi, ba.data());
	ba = m_pPlateInfo->m_strSteelGrade.toLocal8Bit();
	strcpy_s(PlateParam.strGangZhong, ba.data());
	PlateParam.nflag = m_pPlateInfo->flag;

	
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("platefile.dbfl打开失败"));
	//	return ;
	}
	file.seek(0);
	quint64 nBytesWrite = file.write((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
	file.close();
	//chenyuzhu add end on 2014-7-9
*/

/*	if (query.value(0).isNull())  //数据库中没有当前钢板号
	{

		//查询数据库中有没有当前的工艺
		QString tempGongYi = g_strGongYi.trimmed();
		QString strSQl2 = QString("%1%2%3")
			.arg("select * from t_TanShangGongYi where name = '")
			.arg(tempGongYi).arg("'");
		query.exec(strSQl2);
		query.next();
		if (query.value(0).isNull())//如果不存在相应的探伤工艺
		{ 
			//应该重新配置钢板参数，现在未实现
			QMessageBox::information(NULL,tr(""),tr("工艺对应钢种无记录！"));
			return;
		}
		else //从表中取出PlateType并保存到变量中
		{
			strPlateType = query.value(2).toString();
		}
		//插入一条数据//加了插入
	    strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29").arg(tr("insert into t_Plate(PlateType, name, Length, Width, Thick, LWaveSpeed, TWaveSpeed, Weight, Lu, ShenChan, HeTong, GangJi, GangZhong, flag) \
									   values('")).arg(strPlateType).arg("','").arg(g_Info[nPlate].plateno).arg("','").arg(g_Info[nPlate].length).arg("','").arg(g_Info[nPlate].width)
									   .arg("','").arg(g_Info[nPlate].fReportHeight).arg("','").arg(5970).arg("','").arg(3000).arg("','").arg(fWeight)
									   .arg("','").arg(m_pPlateInfo->m_strFurnNumber).arg("','").arg(g_Info[nPlate].plateno).arg("','").arg(m_pPlateInfo->m_strContractNum).arg("','")
									   .arg(g_strJiBie).arg("','").arg(m_pPlateInfo->m_strSteelGrade).arg("','").arg(m_pPlateInfo->flag).arg("')");


	}
	else                       //数据库中有当前钢板号
	{ 
		QString temp1;
		QString temp2;
		QString temp3;
		QString temp4;
		QString temp5;
		QString temp6;
		strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32%33%34%35%36").arg("update t_Plate set Length = '").arg(temp1.sprintf("%.2f",g_Info[nPlate].length)).arg("', ")
					.arg("Width = '").arg(temp2.sprintf("%.2f",g_Info[nPlate].width)).arg("', ")
					.arg("Thick = '").arg(temp3.sprintf("%.2f",g_Info[nPlate].fReportHeight)).arg("', ")
					.arg("LWaveSpeed = '").arg(5970).arg("', ")
					.arg("TWaveSpeed = '").arg(3000).arg("', ")
					.arg("Weight = '").arg(fWeight).arg("', ")
					.arg("Lu='").arg(m_pPlateInfo->m_strFurnNumber).arg("', ")
					.arg("HeTong='").arg(m_pPlateInfo->m_strContractNum).arg("', ")
					.arg("ShenChan='").arg(g_Info[nPlate].plateno).arg("', ")
					.arg("GangJi='").arg(m_pPlateInfo->m_strSteelGrade).arg("', ")
					.arg("flag='").arg(m_pPlateInfo->flag).arg("' ")
					.arg("where name = '").arg(g_Info[nPlate].plateno).arg("'");

	}
	//
	
	//
	query.exec(strSQL);
	//将当前更新或者保存的钢板信息的id保存下来，生成探伤流水表的时候要用到
	strSQL = QString("%1%2%3").arg("select id from t_Plate where name='").arg(g_Info[nPlate].plateno).arg("'");
	query.exec(strSQL);
	query.next();
	m_pCscanView->m_nPlateId = query.value(0).toInt();
	*/
	//chengyuzhu modify end on 2014-7-9
	
/*	//提交到OPC中去
	// 提交探伤工艺（包括钢板参数\探伤模式，通过opc server）
	
	OPCHANDLE *phServer;   
	VARIANT values[5];
	HRESULT *pErrors;
	HRESULT r1;
	LPWSTR ErrorStr;

// 	if (m_OPCComponent.m_pErrors[5] != S_OK) // PLATEID
	//通过server 句柄选择item（由AddItem 得到的）

	phServer = new OPCHANDLE[5];
	pErrors = new HRESULT[5];

	phServer[0] = m_OPCComponent.m_pItemResult[5].hServer;	// PlateID
	// 设置Variant 变量的数据类型和数值
	values[0].vt = VT_BSTR;
	values[0].bstrVal = SysAllocString(g_Info[nPlate].plateno.utf16());
	pErrors[0] = m_OPCComponent.m_pErrors[5];


	phServer[1] = m_OPCComponent.m_pItemResult[12].hServer;	// PlateTestResult
	// 设置Variant 变量的数据类型和数值
	values[1].vt = VT_UI1;
	values[1].bVal = 0;
	pErrors[1] = m_OPCComponent.m_pErrors[12];

	phServer[2] = m_OPCComponent.m_pItemResult[6].hServer;	// PlateLength
	values[2].vt = VT_UI4;
	values[2].uintVal = UINT(g_Info[nPlate].length*10);
	pErrors[2] = m_OPCComponent.m_pErrors[6];

	phServer[3] = m_OPCComponent.m_pItemResult[7].hServer;	// Width
	values[3].vt = VT_UI4;
	values[3].uintVal = UINT(g_Info[nPlate].width*10);
	pErrors[3] = m_OPCComponent.m_pErrors[7];

	phServer[4] = m_OPCComponent.m_pItemResult[8].hServer;	// height
	values[4].vt = VT_UI4;
	values[4].uintVal = UINT(g_Info[nPlate].height*10);
	pErrors[4] = m_OPCComponent.m_pErrors[8];
	bool bRet = OPCWrite(pErrors,phServer,values,5);
	delete[] phServer;
	delete[] pErrors;
	::SysFreeString(values[0].bstrVal);
	if (!bRet)   //OPCWrite函数写失败
	{
		QMessageBox::information(this,tr("MainWindow::InitWindow"),tr("向OPC提交钢板参数失败"));
		exit(0);
	}
	//dongqi add end on 2013.12.2 

	//dongqi add start on 2013.12.3
	// OPC Server发系统Ready信号	
	OPCHANDLE *phServer2;
	VARIANT values2[1];
	HRESULT *pErrors2;

	phServer2 = new OPCHANDLE[1];
	pErrors2 = new HRESULT[1];
	phServer2[0] = m_OPCComponent.m_pItemResult[2].hServer;	// SysState	
	pErrors2[0] = m_OPCComponent.m_pErrors[2];
	values[0].vt = VT_UI1;
	values[0].bVal = 1;
	bRet = OPCWrite(pErrors2,phServer2,values,1);
	delete[] phServer2;
	delete[] pErrors2;
	if (!bRet)
	{
		QMessageBox::information(this,"MainWindow::InitWindow",tr("向OPC写Ready信号失败"));
		exit(0);
	}
	//dongqi add end on 2013.12.3
*/

	createStatusBar();//创建状态栏
	//各种信号和槽的连接
	connect(m_pDataScanAction, SIGNAL(triggered()), this, SLOT(OnDataScanAction()));						//数据浏览工具按钮相应
	connect(m_pColorType,SIGNAL(activated(int )),this,SLOT(onComBoChange(int)));  							//比色表选择控件和比色表显示标签的连接
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));											//更新当前时间
	connect(m_pSChangeLanguage,SIGNAL(clicked()),this,SLOT(OnStatusChangeLanguage()));						//语言切换
	connect(m_pSCChinese, SIGNAL(triggered()), this, SLOT(OnChangeToChinese()));							//中文切换
	connect(m_pSCEnglish, SIGNAL(triggered()), this, SLOT(OnChangeToEnglish()));							//英语切换
	connect(m_pParameterAction,SIGNAL(triggered()),this,SLOT(OnParameterAction()));							//参数配置对话框
	//工具面板相关连接

	//chenyuzhu add start on 2014.5.7
	//测试模拟开始停止探测槽和信号连接
#if PLC_DEBUG
	connect(m_pOPC_Start,SIGNAL(triggered()), this, SLOT(OPCStartOrStop())); //调试 chenyuzhu on 2014-10-8	
#endif

	//chenyuzhu add end on 2014.5.7

	//重新判伤
    connect(m_pGongYiQieHuan,SIGNAL(triggered()), m_pCscanView, SLOT(OnRePanShang()));

    connect(m_pExpandAction,SIGNAL(triggered()), this, SLOT(ExpandAction()));
    connect(m_pToolsDialog->m_pLilunBiaoding, SIGNAL(clicked()), this, SLOT(OnALLChannelBiaoDing()));
	connect(m_pToolsDialog->m_pInConPointPage->m_pLeftCheck,SIGNAL(stateChanged(int)),this,SLOT(LeftViewChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pRightCheck,SIGNAL(stateChanged(int)),this,SLOT(RightViewChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pTopCheck,SIGNAL(stateChanged(int)),this,SLOT(TopViewChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pBottonCheck,SIGNAL(stateChanged(int)),this,SLOT(BottomViewChange(int)));
	connect(m_pToolsDialog,SIGNAL(UpdateMainWindowOnCancel()),this,SLOT(OnUpdateToolDialogOnCancel()));
	connect(m_pToolsDialog->m_pInConPointPage->m_pCThicknessView,SIGNAL(stateChanged(int)),this,SLOT(CScanThicknessWidgetChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pCFrontView,SIGNAL(stateChanged(int)),this,SLOT(CScanFrontWidgetWidgetChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pCSideView,SIGNAL(stateChanged(int)),this,SLOT(CScanSideWidgetChange(int)));
	connect(m_pToolsDialog->m_pInConPointPage->m_pCOverView,SIGNAL(stateChanged(int)),this,SLOT(CScanWholeWidgetChange(int)));
	connect(m_pAscanAction,SIGNAL(triggered()),this,SLOT(OnAscanAction()));					//A扫界面
	connect(m_pCscanView->m_pWgtCscanDetectView,SIGNAL(signalToChannelPannel(QString)),m_pDockWidgetParameterDisp,SLOT(UpdatePointInformation(QString)));//显示悬浮伤点信息
	connect(m_pToolsDialog->m_pInConPointPage->m_pReticleCheck,SIGNAL(stateChanged(int)),m_pCscanView,SLOT(OnRecticCheck(int)));			//中心十字线
	connect(m_pToolsDialog->m_pInConPointPage->m_pPointRangeCheck,SIGNAL(stateChanged(int)),m_pCscanView,SLOT(OnPointRangeCheck(int)));		//伤点范围线

	connect(m_pBiaodingDlg->m_pBiaodingCheck,SIGNAL(stateChanged(int)),this,SLOT(OnBiaodingCheck(int)));//chenyuzhu add start on 2014-11-6

	connect(m_pToolsDialog->m_pInConPointPage->m_pDefectDistantCheck,SIGNAL(stateChanged(int)),m_pCscanView,SLOT(OnDefectDistantCheck(int)));//伤点合并chenyuzhu add on 2014-6-19

	connect(m_pToolsDialog->m_pPrintControlPage->m_pPrintReport,SIGNAL(clicked()),m_pCscanView,SLOT(OnMenuPrint()));						//打印报表
	connect(m_pPrintAction,SIGNAL(triggered()),m_pCscanView,SLOT(OnMenuPrint()));															//打印报表
	connect(m_pToolsDialog->m_pPrintControlPage->m_pbPreview,SIGNAL(clicked()),m_pCscanView,SLOT(OnPrintPreview()));						//打印报表预览
	connect(m_pScanData,SIGNAL(SignalDrawSelectedFile(QList<QString>)),m_pCscanView,SLOT(OnDrawSelectedFile(QList<QString>)));              //数据浏览对话框双击事件
	connect(m_pScanData,SIGNAL(SignalUpdateChannelDock(QList<QString>)),this,SLOT(OnUpdateChannelDock(QList<QString>)));              //数据浏览对话框双击事件

	connect(m_pCscanView, SIGNAL(SignalToDrawing()), this, SLOT(EndReadDetectFile()));														//通知主界面重新画图
	connect(m_pPreviousAction, SIGNAL(triggered()), m_pScanData, SLOT(OnPreDrawingImage()));												//向前读取伤点文件重绘
	connect(m_pNextAction, SIGNAL(triggered()), m_pScanData, SLOT(OnNextDrawingImage()));						
	connect(m_pDetectListAction, SIGNAL(triggered()), this, SLOT(OnDetectListDlgAction()));													//伤点列表双击响应
	connect(m_pCscanView,SIGNAL(SignalToDetectListDlg()),m_pDetectListDlg,SLOT(UpdateTableView()));											//更新伤点列表对话框
	connect(m_pDetectListDlg,SIGNAL(SignalToCscanDrawDashRect(int)),m_pCscanView,SLOT(OnCscanDrawDashRect(int)));							//画选中伤点的矩形
	connect(this, SIGNAL(signalPlateScanEnd()), m_pCscanView, SLOT(OnPlateScanEnd()));	//探伤结束
	connect(this, SIGNAL(signalPlateScanStart()), m_pCscanView, SLOT(OnPlateScanStart()));	//探伤开始
	connect(this, SIGNAL(signalAddFlawData(CH_FLAWDATA*)), m_pCscanView, SLOT(OnAddFlawData(CH_FLAWDATA*)));

	connect(m_pToolsDialog->m_pPrintControlPage->m_pEnglishReport, SIGNAL(stateChanged(int)), m_pCscanView, SLOT(OnEnglishUpdata(int)));
	


	//更新m_flowStore数据
	//test 用
	connect(m_pDemarcateAction, SIGNAL(triggered()), this, SLOT(OnBiaoDingAction()));	//chenyuzhu modify on 2014-11-6	//标定按钮，现在用于读伤点文件画图
	m_pToolsDialog->m_pInConPointPage->m_pReticleCheck->setChecked(m_pToolsDialog->m_pTToolsDialogInfo->m_bCentreCrosingLine);				//伤点十字线
	m_pToolsDialog->m_pInConPointPage->m_pPointRangeCheck->setChecked(m_pToolsDialog->m_pTToolsDialogInfo->m_bInjPointRangeLine);			//伤点范围线
	connect(m_pAscan->pushButtonAScanSave,SIGNAL(clicked()),this,SLOT(AScanSave()));														//A扫界面保存按钮
	m_pDetectListDlg->m_pvHeaderRect = &(m_pCscanView->m_vHeaderRect);
	m_pCscanView->m_pWgtCscanDetectView->m_pvHeaderRect = &(m_pCscanView->m_vHeaderRect);
	m_pCscanView->m_pWgtCscanDetectView->m_pvHRect = &(m_pCscanView->m_vHRect);//chenyuzhu add on 2014-6-19
	m_pCscanView->m_pWgtCscanDetectView->m_pbPointJoin = &(m_pCscanView->m_bPointJoin);//chenyuzhu add on 2014-6-19
	m_pCscanView->m_pbDependOnMInput = &(m_pToolsDialog->m_pTToolsDialogInfo->m_bDependOnMInput);
	m_pCscanView->m_pbPrintInjPointDetailPic = &(m_pToolsDialog->m_pTToolsDialogInfo->m_bPrintInjPointDetailPic);

	//	connect(m_pCscanView,SIGNAL(SignalToSetPlateId()),this,SLOT(OnSetPlateID()));
	connect(m_pCscanView,SIGNAL(SignalToSetPlateId()),this,SLOT(PrepareStartDetect()));
	
	//chenyuzhu add start on 2014.6.10
	//连接删除伤点后重新计算和绘图的信号和槽
	connect(m_pCscanView,SIGNAL(SignalToDeleteReDraw()), this, SLOT(deleteReDraw()));	
	//chenyuzhu add end on 2014.6.10

	connect(m_pDockWidgetChannelCtr->m_pmyWidget,SIGNAL(SignalToDrawTanTouLine(int, bool)), m_pCscanView, SLOT(DrawTanTouLine(int, bool)));	//chenyuzhu add on 2014-6-23
	connect(m_pToolsDialog, SIGNAL(SignalOnCidentLog()), m_pCscanView, SLOT(OnCidentLog()));	//chenyuzhu add on 2014-6-23
	connect(m_pToolsDialog, SIGNAL(SignalToUpdateCscan()), m_pCscanView, SLOT(OnToolsToUpdate()));	//chenyuzhu add on 2014-7-24
	m_pCscanView->m_pDefectDistaneSmaller = &m_pToolsDialog->m_pTToolsDialogInfo->m_nDefectDistaneSmaller;
	m_pIsReset->installEventFilter(this);    //注册事件过滤器
    
	hMonitorFlawEvent[0] = CreateEvent(NULL, TRUE, FALSE, L"AEC1");
	hMonitorFlawEvent[1] = CreateEvent(NULL, TRUE, FALSE, L"AEC2");
	hMonitorFlawEvent[2] = CreateEvent(NULL, TRUE, FALSE, L"AEC3");
	hMonitorFlawEvent[3] = CreateEvent(NULL, TRUE, FALSE, L"AEC4");
	hMonitorFlawEvent[4] = CreateEvent(NULL, TRUE, FALSE, L"AEC5");
}

void MainWindow::timerEvent(QTimerEvent *event)
{	
	static int nCount = 0;
	g_ItemCS.lock();
	BYTE tmpCommand = g_ItemValue.dwPLC2WSCommand;

#if PLC_DEBUG
	tmpCommand = 1; //测试 chenyuzhu add on 2014-10-9;
#endif

	g_ItemCS.unlock();
	if (event->timerId() == m_nTimerID_PROCESS)   //线程定时器
	{
		// 启动的准备工作
		if(m_bPlateStartEn)                       //Timer中开始状态时置TRUE
		{
			m_bPlateStartEn = FALSE;	
			// 初始化控件参数..
//			g_pPlateDraw->dynamicCall("clear()");
//			g_pPlateDraw->dynamicCall("SetPlateInfo(double, double, double)", g_Info[nPlate].length, g_Info[nPlate].width,g_Info[nPlate].height);
//			g_pPlateDraw->dynamicCall("SetDistanceX(double, double)", g_IniParams.m_minYDistance, g_IniParams.m_maxYDistance);
//			g_pPlateDraw->dynamicCall("SetDistanceY(double, double)", g_IniParams.m_minXDistance, g_IniParams.m_maxXDistance);
//			g_pPlateDraw->dynamicCall("SetShangBoFuZhiBiaoZhun(bool, float)", g_IniParams.m_bBaoLiu, g_IniParams.m_nSoftDAC);
			// 下发参数
			INIT_CONFIG(allCH_CONFIG, &ch_config_global);  //初始化通道信息
			for (int i = 0;i < m_nAECCardNum ;i++)
			{				
				m_PLXCommunication.ConfigDSP(m_pAECWorkThread[i]->m_pAecThreadParam.hDev, allCH_CONFIG, &(ch_config_global), m_pAECWorkThread[i]->m_pAecThreadParam.index);
				m_PLXCommunication.ConfigDSP_DAC(m_pAECWorkThread[i]->m_pAecThreadParam.hDev,m_pDatabaseConnection);
				m_PLXCommunication.InterruptDSP(m_pAECWorkThread[i]->m_pAecThreadParam.hDev, i);
			}
			nCount = 1;	
			m_bCacluFlag = FALSE;	
			g_bStart = TRUE;            //准备工作完成后置TRUE
			g_bStop = FALSE;
			// dong qi add start on 2013-11-26
			//更新通道显示界面
			m_pDockWidgetChannelCtr->m_pmyWidget->DrawingBackground();
			m_pDockWidgetChannelCtr->update();
			// dong qi add end on 2013-11-26
			nCardEndStauts = 0;			// Song Chenguang   全局变量
		
			//创建探测日志文件
		/*	QString curPath = QDir::currentPath() + QString("\\RunningDetect.txt");//chenyuzhu modify on 2014-10-22
			QFile RDFile(curPath);
			RDFile.open(QIODevice::Truncate);
			QTextStream txtOutput(&RDFile);  //chenyuzhu add on 2014-11-5
			txtOutput << tr("探测日志") << endl;  //chenyuzhu add on 2014-11-5
			RDFile.close();*/
			
			//chenyuzhu add start on 2014-11-5
			//创建探测日志文件
			QString curPath = QDir::currentPath() + QString("\\RunningDetect.txt");
			//chenyuzhu modify end on 2014-10-22
			QFile RDFile(curPath);
			RDFile.open(QIODevice::ReadWrite);
			QTextStream txtOutput(&RDFile);  
			txtOutput << tr("探测日志") << endl;  
			RDFile.close();
			//chenyuzhu add end 2014-11-5
			
			//设置闸门前沿范围
			m_pAscan->UiInit();
			m_pAscan->m_bSelChannel = false;
			m_pAscan->m_bSetOne = false;
			//chenyuzhu add start on 2014-10-22
			// 清空失波数据
			for(int i=0; i<MAX_CHANNEL_NO; i++)
			{
				if (g_LostWaveDatas[i] != NULL)
				{
					g_LostWaveDatas[i]->clear();
				}
				
			}
			//chenyuzhu add end on 2014-10-22
			emit signalPlateScanStart();  //通知C扫面板探伤开始
		}

		for (int i = 0; i < m_nAECCardNum; i++)   
		{
			if(g_csCard.tryLock())
			{
				int a = i;
				if (m_bCardInit[i])					//dong qi add on 2013.11.21  // 探伤开始标志TRUE后
				{
					m_bCardInit[i] = FALSE;			//dong qi add on 2013.11.21
					m_pAECWorkThread[i]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_INIT;
					m_bIsRunning = TRUE;			//在StopOK时置False
				} 
				else if(m_bPlateEnd[i])	// 探伤结束
				{
					m_pAECWorkThread[i]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_PLATE_STOP;
					m_bPlateEnd[i] = FALSE;
				}
				else if (m_pAscan->m_bSelChannel)	// 设置伤低波显示切换
				{
					m_pAscan->m_bSelChannel = FALSE;
					int nIndex = g_dwActiveChannel / MAX_CARD_CHANNEL_NO; //记录线程号
					m_pAECWorkThread[nIndex]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SET_CHANNEL_SETONE; // wangwei 2016-1-1
					if (g_dwActiveFlawOrBottom == 1)  //底波
					{
						m_pAECWorkThread[nIndex]->m_pAecThreadParam.wSelChannel = g_dwActiveChannel+256;
					}else
					{
						m_pAECWorkThread[nIndex]->m_pAecThreadParam.wSelChannel = g_dwActiveChannel;
					}
					g_csCard.unlock();
					break;
				}
				else if (m_pAscan->m_bSetOne)	// 当前通道设置参数
				{
					m_pAscan->m_bSetOne = FALSE;
					int nIndex = g_nSetOneChannel / MAX_CARD_CHANNEL_NO; //记录线程号
					m_pAECWorkThread[nIndex]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SETONE;
					m_pAECWorkThread[nIndex]->m_pAecThreadParam.nSetOneChannel = g_nSetOneChannel;
					g_csCard.unlock();
					break;
				}
				else
				{
				}
				// dongqi add start on 2013.11.21 
				//等所有卡启动完毕后再SelChannel
				if (m_pAECWorkThread[i]->m_pAecThreadParam.nAEC2MainThreadFlag == AECTHREADREURN_START_OK)
				{
					m_nAECCardsStarted |= static_cast<int>(pow(2.0,i));//用于判断所有卡是否都已打开
					m_pAECWorkThread[i]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					m_pAECWorkThread[i]->m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_IDLE;   //未做任何处理
				}
				// dongqi add end on 2013.11.21

				// 结束后的收尾工作
				if (m_pAECWorkThread[i]->m_pAecThreadParam.nAEC2MainThreadFlag == AECTHREADREURN_STOP_OK)
				{
					nCardEndStauts |= static_cast<int>(pow(2.0,i));   //全局变量
					if( nCount == 1)
					{
						m_bIsRunning = FALSE;
						m_pAECWorkThread[i]->m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_INIT_FALSE;
					}
					//dong qi add start on 2013.11.21
					//通知另一块卡结束
					if (i < m_nAECCardNum -1)
					{
						m_bPlateEnd[i+1] = TRUE;
					}
					//dong qi add end on 2013.11.21
				}
				g_csCard.unlock();
			}	
		}
		//全部卡Start后下发通道号
		if (m_nAECCardsStarted == g_plateEndFlag)
		{
			m_nAECCardsStarted = 0;
			m_bCardsStop_En = TRUE;
			m_bCardsStop_Flag = FALSE;
		
			// 设置A扫界面的当前通道是0通道，并置SELCHANNEL为TRUE
			//m_pAscan->comboBoxProbeBox->setCurrentIndex(0);
			//m_pAscan->ProbeBoxChange();  //探头盒切换，判断哪些探头选中然后设置选中状态
		
			m_bCardsStart_Flag = TRUE;
			m_bBiaoDingOpen_En = TRUE;
			m_bSetOne_En = TRUE;
		}
		if ((nCardEndStauts == g_plateEndFlag) && (!m_bCacluFlag) ) // 所有线程都处理结束
		{	
			nCardEndStauts = 0;
			nCount = 0;
			m_bCacluFlag = TRUE;
			g_bStop = TRUE;
			g_bStart = FALSE;

			// dong qi add start on 2013-11-26
			//更新通道显示界面
			m_pDockWidgetChannelCtr->m_pmyWidget->DrawingBackground();
			m_pDockWidgetChannelCtr->update();
			// dong qi add end on 2013-11-26

			// 搜索失波数据，将大于指定长度连续的失波数据加入控件
			//先注释掉泪因为报错，还没修改
 			/*for (int i = 0; i < MAX_CHANNEL_NO; i++)//取消掉注释 chenyuzhu 2014.6.30
 			{
  				if (g_LostWaveDatas[i]->size() > 0)
  				{
  					// 搜索单个通道
 					SearchUsefulLostWaveData(*g_LostWaveDatas[i]);
  				}
 			}*/
			//chenyuzhu add start on 2014-11-7
			if (false == g_pbBiaoding)
			{
				emit signalPlateScanEnd();     //探伤结束绘制C扫图，此时可以手动打报表
			}
			//chenyuzhu add end on 2014-11-7
			m_bCardsStart_En = TRUE;
			m_bCardsStart_Flag = FALSE;
			QApplication::restoreOverrideCursor();//恢复鼠标

		}
	}	
	else if (event->timerId() == m_nTimerID_PLC)      //TIMER_PLC_MONITOR 处理分支 PLC定时器
	{
		static int count =  0;  // 标志
		QString str;
		WORD wStart = 2; //PLC 状态值
#if HAVE_PLX
        wStart = P9054_ReadWord(m_stThreadParam[0].hDev, P9054_ADDR_SPACE0, MPB_SYSTEM_START_ADDR);
#endif
		if (count == 0 && wStart == 1 && m_bCardsStart_En)	//开始状态
		{			
		// PLC2WSCommand值  0---无效	1---探伤 2---自动标定
			if (tmpCommand == 0)      //无效
			{
				QMessageBox::information(NULL, "Error", tr("PLC2WSCommand命令无效!"), QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
			else if (tmpCommand == 2)   //自动标定，未实现
			{
					for (int i=0; i<MAX_CHANNEL_NO; i++)
					{
						allCH_CONFIG[i].CH_GAIN_DIFF = 0;
						allCH_CONFIG[i].CH_EN = 0;
					}
			}
			else if(tmpCommand != 1 && tmpCommand != 3) //错误命令
			{
				QString strCommand = QString("%1%2").arg(tr("错误的PLC命令，命令代码 = ")).arg(tmpCommand);
				QMessageBox::information(NULL, "Error", strCommand, QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
			for(int i=0; i< m_nAECCardNum; i++)
			{
				m_bCardInit[i] = TRUE;        //进入线程的INIT分支，成功后直接进入Start分支
			}
			m_bPlateStartEn = TRUE;			  // 进入准备工作判断，将g_bStart置True
			count++;
			m_bCardsStart_En = FALSE;
			m_bReportFlaw_En = TRUE;
			g_CS.lock();
			g_dwFlawCount = 0;
			g_CS.unlock();
		}		
		else if (count == 1 && wStart == 0) // 中止状态
		{
		//if (tmpCommand == 1) 
			{
				m_bSetOne_En = FALSE;
					if (m_bCardsStop_En)		 
				{
					if (g_IniParams.m_bReadPlateLenthEn)
					{
						// 读取PLC传递的长宽厚，更新
						HRESULT *pErrors = &m_OPCComponent.m_pErrors[4];
						OPCHANDLE *phServer = new OPCHANDLE[1];
						//通过server 句柄选择item（由AddItem 得到的）
						phServer[0] = m_OPCComponent.m_pItemResult[6].hServer;	// PlateLength
						OPCRead(pErrors,phServer,1);
						delete[] phServer;
						DWORD ret = ::WaitForSingleObject(hOPCReadEvent,INFINITE);
						ResetEvent(hOPCReadEvent);		
                        m_pPlateInfo[nPlate].m_nPlateLength = g_ItemValue.ulPlateLength/10.0;
                        m_pPlateInfo[nPlate].m_nPlateLength = qMax(m_pPlateInfo[nPlate].m_nPlateLength, 10.0f); //防止为零
                        g_Info[nPlate].length = m_pPlateInfo[nPlate].m_nPlateLength;
                        UpdateDockWidgetParameterDisp();
                    }
					//dong qi add start on 2013-11-26
					//在这里终止轮询选中通道的MAP，改变dwActiveChannel
					if ( m_pAscan->m_nTIMER_Sel_Channel != 0 )
					{
						killTimer(m_pAscan->m_nTIMER_Sel_Channel);
						m_pAscan->m_nTIMER_Sel_Channel = 0;
					}
					//dong qi add end on 2013-11-26
					m_bCardsStop_En = FALSE;
					//dongqi add start on 2013.11.21
 					m_bPlateEnd[0] = TRUE; // 第一块卡结束
					//dongqi add end on 2013.11.21
					count--;
					m_bReportFlaw_En = FALSE;
					QApplication::setOverrideCursor(Qt::WaitCursor);//设置鼠标等待
				}
			}
		}
		else
		{
			
		}
	}
	if (event->timerId() == m_nTimerID_LOG)
	{
		if ((tmpCommand == 1 || tmpCommand == 3) && m_bReportFlaw_En)
		{
			// 监视同步
			for (int i=0; i < m_nAECCardNum; i++)
			{
				DWORD dwWaitObjectIndex = WaitForSingleObject(hMonitorFlawEvent[i], 0);
				if (dwWaitObjectIndex == WAIT_OBJECT_0)
				{
					UpdateMonitorWndTemp(i);   //临时用来让报伤的探头闪几下，通道报警 // 通道面板报警
					UpdateMonitorWnd(i);//chenyuzhu modify on 2014-8-7 原来注视掉了 //  C扫实时图绘制
					ResetEvent(hMonitorFlawEvent[i]);
				}
			}
		}
	} 
}

void MainWindow::createToolBars()
{
	m_pMainToolBar = new QToolBar(QObject::tr("MainToolBar"),this);
	m_pMainToolBar->setMovable(true);
	m_pMainToolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
	m_pMainToolBar->setIconSize(QSize(this->width()/25, 48));
	m_pMainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

	//chenyuzhu add start on 2014.5.6
	//m_pMainToolBar->setStyleSheet(QString::fromUtf8("font: 20pt \"Sans Serif\";"));
	m_pMainToolBar->setStyleSheet(QString::fromUtf8("font-size:12px;color:rgb(255,255,0)"));
	//chenyuzhu add end on 2014.5.6

	this->addToolBar(Qt::TopToolBarArea, m_pMainToolBar);

	m_pMainToolBar->setEnabled(true);
	m_pMainToolBar->setWindowTitle(tr("常用工具栏"));

 	//比色表选择工具栏
 	m_pColorSelectToolBar = new QToolBar("ColorSelectToolBar",this);
 	m_pColorSelectToolBar->setMinimumSize(QSize(0, 60));
	m_pColorSelectToolBar->setMovable(true);
 	m_pColorSelectToolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
 	m_pColorSelectToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
 	this->addToolBar(Qt::TopToolBarArea, m_pColorSelectToolBar);
 	m_pColorSelectToolBar->setEnabled(true);
 	m_pColorSelectToolBar->setWindowTitle(tr("比色表选择工具栏"));


	//比色表选择控件
	m_pColorType = new QComboBox();
//	m_pColorType->addItem(tr("当量比色表"));//chenyuzhu modify on 2014-7-21
	m_pColorType->addItem(tr("分级比色表"));
	m_pColorType->addItem(tr("深度比色表"));
	m_pColorType->setMinimumSize(QSize(this->width()/10, 25));
	m_pColorType->setSizeAdjustPolicy(QComboBox::AdjustToContents);//控件大小随内容变化

	//比色表显示标签
	m_pColorLabel = new QLabel(this);
	m_pColorLabel ->setFrameStyle(QFrame::Panel);
	m_pColorLabel ->setText("first line\nsecond line");
 	m_pColorLabel ->setMinimumSize(QSize(this->width()/10 *7,25));
	m_pColorLabel ->setMaximumSize(QSize(this->width()/10 *7,50));
    DrawColorLabel(); // 完成3种比色表的绘制
	m_pColorSelectToolBar->addWidget(m_pColorType);
	m_pColorSelectToolBar->addWidget(m_pColorLabel);
}

void MainWindow::createDockWidget()
{   
	
	//通道控制
	m_pDockWidgetChannelCtr = new CChannelPanel(this,m_pPlateInfo,m_pDatabaseConnection);
	m_pDockWidgetChannelCtr->setAllowedAreas(Qt::TopDockWidgetArea|Qt::BottomDockWidgetArea);
	m_pDockWidgetChannelCtr->setMaximumSize(QSize(1920, 73));
	addDockWidget(Qt::TopDockWidgetArea, m_pDockWidgetChannelCtr);


	//C扫面板
	m_pCscanView = new CCscanPanel(m_pDatabaseConnection,this);
	m_pCscanView->setGeometry(0,0,1550,880);
	m_pCscanView->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, m_pCscanView);


}

void MainWindow::createActions()
{
	//标定
	QIcon iconDemarcate;
	m_pDemarcateAction = new QAction(tr("标定"),this);
	iconDemarcate.addFile(QString::fromUtf8(":/Resources/biaoding_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconDemarcate.addFile(QString::fromUtf8(":/Resources/biaoding_select.png"), QSize(), QIcon::Active, QIcon::Off);

	m_pDemarcateAction->setIcon(iconDemarcate);
	m_pDemarcateAction->setCheckable(true); //chenyuzhu add on 2014-12-27
	m_pDemarcateAction->setChecked(false);	//chenyuzhu add on 2014-12-27
    m_pDemarcateAction->setShortcut(QApplication::translate("MainWindow", "Shift+B"));
	m_pMainToolBar->addAction(m_pDemarcateAction);
	//act_selected_mode->setCheckable(true);
	//act_drag_mode->setCheckable(true);
	//自检
	QIcon iconSelfTest;
	m_pSelfTestAction = new QAction(tr("自检"),this);
	iconSelfTest.addFile(QString::fromUtf8(":/Resources/zijian_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconSelfTest.addFile(QString::fromUtf8(":/Resources/zijian_select.png"), QSize(), QIcon::Active, QIcon::Off);

	m_pSelfTestAction->setIcon(iconSelfTest);
    m_pSelfTestAction->setShortcut(QApplication::translate("MainWindow", "Shift+T"));
	m_pMainToolBar->addAction(m_pSelfTestAction);

	//A扫BUTTON
	QIcon iconAscan;
	m_pAscanAction = new QAction(tr("A扫"),this);
	iconAscan.addFile(QString::fromUtf8(":/Resources/A_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconAscan.addFile(QString::fromUtf8(":/Resources/A_select.png"), QSize(), QIcon::Active, QIcon::Off);

	m_pAscanAction->setIcon(iconAscan);
    m_pAscanAction->setShortcut(QApplication::translate("MainWindow", "Shift+A"));
	m_pMainToolBar->addAction(m_pAscanAction);

	//C扫BUTTON
	QIcon iconCscan;
	m_pPrintAction = new QAction(tr("打印"),this);
	iconCscan.addFile(QString::fromUtf8(":/Resources/dayin_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconCscan.addFile(QString::fromUtf8(":/Resources/dayin_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconCscan.addFile(QString::fromUtf8(":/Resources/C_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pPrintAction->setIcon(iconCscan);
    m_pPrintAction->setShortcut(QApplication::translate("MainWindow", "Shift+C"));
	m_pMainToolBar->addAction(m_pPrintAction);

	//钢板参数设置BUTTON
	QIcon iconParameter;
	m_pParameterAction = new QAction(tr("钢板参数设置"),this);
	iconParameter.addFile(QString::fromUtf8(":/Resources/gangbancanshushezhi_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconParameter.addFile(QString::fromUtf8(":/Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconParameter.addFile(QString::fromUtf8("Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pParameterAction->setIcon(iconParameter);
    m_pParameterAction->setShortcut(QApplication::translate("MainWindow", "Shift+I"));
	m_pMainToolBar->addAction(m_pParameterAction);

	//伤点列表BUTTON
	QIcon iconDetectList;
	m_pDetectListAction = new QAction(tr("伤点列表"),this);
	iconDetectList.addFile(QString::fromUtf8(":/Resources/tanshangliebiao_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconDetectList.addFile(QString::fromUtf8(":/Resources/tanshangliebiao_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconDetectList.addFile(QString::fromUtf8("Resources/tanshangliebiao_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pDetectListAction->setIcon(iconDetectList);
    m_pDetectListAction->setShortcut(QApplication::translate("MainWindow", "Shift+L"));
    m_pParameterAction->setShortcut(QApplication::translate("MainWindow", "Shift+L"));
	m_pMainToolBar->addAction(m_pDetectListAction);

	//前一数据BUTTON
	QIcon iconPrevious;
	m_pPreviousAction = new QAction(tr("上一数据"),this);
	iconPrevious.addFile(QString::fromUtf8(":/Resources/shangyiji_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconPrevious.addFile(QString::fromUtf8(":/Resources/shangyiji_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconPrevious.addFile(QString::fromUtf8(":/Resources/shangyiji_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pPreviousAction->setIcon(iconPrevious);
    m_pPreviousAction->setShortcut(QApplication::translate("MainWindow", "Shift+Left"));
	m_pMainToolBar->addAction(m_pPreviousAction);

	//数据浏览BUTTON
	QIcon iconSacnData;
	m_pDataScanAction = new QAction(tr("数据浏览"),this);
	iconSacnData.addFile(QString::fromUtf8(":/Resources/findData_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconSacnData.addFile(QString::fromUtf8(":/Resources/findData_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//icon.addFile(QString::fromUtf8(":/Resources/next_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pDataScanAction->setIcon(iconSacnData);
    m_pDataScanAction->setShortcut(QApplication::translate("MainWindow", "Shift+Right"));
	m_pMainToolBar->addAction(m_pDataScanAction);
	//下一数据BUTTON
	QIcon iconNext;
	m_pNextAction = new QAction(tr("下一数据"),this);
	iconNext.addFile(QString::fromUtf8(":/Resources/next_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconNext.addFile(QString::fromUtf8(":/Resources/next_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//icon.addFile(QString::fromUtf8(":/Resources/next_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pNextAction->setIcon(iconNext);
    m_pNextAction->setShortcut(QApplication::translate("MainWindow", "Shift+Right"));
	m_pMainToolBar->addAction(m_pNextAction);

	//chenyuzhu add start on 2014-7-23
    //重新判伤
	QIcon iconGongYiQieHuan;
	m_pGongYiQieHuan = new QAction(tr("重新判伤"),this);
	iconGongYiQieHuan.addFile(QString::fromUtf8(":/Resources/qiehuan_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconGongYiQieHuan.addFile(QString::fromUtf8(":/Resources/qiehuan_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconExpand.addFile(QString::fromUtf8(":/Resources/mnue_select.png"), QSize(), QIcon::Selected, QIcon::Off);
 	m_pGongYiQieHuan->setIcon(iconGongYiQieHuan);
    //m_pExpandAction->setShortcut(QApplication::translate("MainWindow", "Shift+E"));
	m_pMainToolBar->addAction(m_pGongYiQieHuan);
	//chenyuzhu add start on 2014-7-23

	//详细工具面板BUTTON
	QIcon iconExpand;
	m_pExpandAction = new QAction(tr("其他工具"),this);
	iconExpand.addFile(QString::fromUtf8(":/Resources/mnue_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconExpand.addFile(QString::fromUtf8(":/Resources/mnue_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconExpand.addFile(QString::fromUtf8(":/Resources/mnue_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pExpandAction->setIcon(iconExpand);
    m_pExpandAction->setShortcut(QApplication::translate("MainWindow", "Shift+E"));
	m_pMainToolBar->addSeparator();
	m_pMainToolBar->addAction(m_pExpandAction);


	//chenyuzhu add start on 2014.5.7
	//OPC测试BUTTON  测试模拟开始停止探测按键

#if PLC_DEBUG
	QIcon iconOPCStart;
	m_pOPC_Start = new QAction(tr("OPC开始停止"),this);
	iconOPCStart.addFile(QString::fromUtf8(":/Resources/mnue_noselect.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconOPCStart.addFile(QString::fromUtf8(":/Resources/mnue_select.png"), QSize(), QIcon::Active, QIcon::Off);
	//iconExpand.addFile(QString::fromUtf8(":/Resources/mnue_select.png"), QSize(), QIcon::Selected, QIcon::Off);
	m_pOPC_Start->setIcon(iconOPCStart);
    //m_pExpandAction->setShortcut(QApplication::translate("MainWindow", "Shift+E"));
	m_pMainToolBar->addSeparator();
	m_pMainToolBar->addAction(m_pOPC_Start);
#endif

	//chenyuzhu add start on 2014.5.7//调试 chenyuzhu on 2014-10-8	
}

void MainWindow::createStatusBar()
{
	//状态栏样式
	 statusBar()->setStyleSheet("border: 3px groove rgb(73, 73, 73)");
	 statusBar()->setSizeGripEnabled(false);
	//参数配置状态
	m_pIsReset= new QLabel(this);
	m_pIsReset->setMinimumSize(m_pIsReset->sizeHint());
	if(0 ==m_FlagMap["MTechnologySetFlag"])
	{
		m_pIsReset->setStyleSheet("color:rgb(255, 0, 0)");
		m_pIsReset->setText(tr("未选择工艺名称"));
	}
	else if (0 == m_FlagMap["MPlateParaFlag"])
	{
		m_pIsReset->setStyleSheet("color:rgb(255, 0, 0)");
		m_pIsReset->setText(tr("未设置钢板参数"));
	}
	else
	{
		m_pIsReset->setStyleSheet("color:rgb(255, 255, 255)");
		m_pIsReset->setText(tr("准备就绪"));
	}
	 statusBar()->addWidget(m_pIsReset);
	//国际化
	m_pSChangeLanguage = new QPushButton(this);
	m_pSChangeLanguage->setText(tr("语言切换"));
	m_pSChangeLanguage->setMinimumHeight(statusBar()->height());
	statusBar()->addPermanentWidget(m_pSChangeLanguage);
	m_pSChangeLanguage->setStyleSheet("color:rgb(255, 255, 255);border: 0px");
	m_pSMenu =new QMenu(tr("语言切换"),this);
	m_pSMenu->setStyleSheet("color:rgb(255, 255, 255);border: 0px");
	m_pSCChinese= new QAction(tr("中文"),this);
	m_pSMenu->addAction(m_pSCChinese);
	m_pSCEnglish = new QAction(tr("英语"),this);
	m_pSMenu->addAction(m_pSCEnglish);
	m_pSMenu->setFixedWidth(75);
    //操作者姓名显示
	m_pSOperator = new QLabel(this);
	m_pSOperator->setStyleSheet("color:rgb(255, 255, 255)");
	m_pSOperator->setMinimumSize(m_pSOperator->sizeHint());
	//m_pSOperator->setAlignment(Qt::AlignHCenter);
	QString strOperatorName = QString("%1%2").arg(tr("操作者姓名: ")).arg(m_plogonDialog->m_pcbUser->currentText());
	m_pSOperator->setText(strOperatorName);
	statusBar()->addPermanentWidget(m_pSOperator);
	//时间显示
	m_pSTime = new QLabel(this);
	m_pSTime->setStyleSheet("color:rgb(255, 255, 255)");
	m_pSTime->setMinimumSize(m_pSTime->sizeHint());
	QDateTime time = QDateTime::currentDateTime();
	QString strTime = time.toString("yyyy-MM-dd hh:mm:ss");
	m_pSTime->setText(strTime);
	m_pTimer = new QTimer(this);
	m_pTimer->start(1000); 
	statusBar()->addPermanentWidget(m_pSTime);
}

void MainWindow::RestoreState()
{
	//4个边缘视图
	bool bTopCheckBoxState = m_pSettings->value("ToolsDialogInfo/TopEdgePicture","").toBool();
	bool bBottonCheckBoxState = m_pSettings->value("ToolsDialogInfo/BottonEdgePicture","").toBool();
	bool bLeftCheckBoxState = m_pSettings->value("ToolsDialogInfo/LeftEdgePicture","").toBool();
	bool nRightCheckBoxState = m_pSettings->value("ToolsDialogInfo/RightEdgePicture","").toBool();

	m_pCscanView->m_pWgtLeftView->setVisible(bLeftCheckBoxState);
	m_pCscanView->m_pWgtRightView->setVisible(nRightCheckBoxState);
	m_pCscanView->m_pWgtTopView->setVisible(bTopCheckBoxState);
	m_pCscanView->m_pWgtBottomView->setVisible(bBottonCheckBoxState);

	//4个子视图
	bool bThickNessPicture = m_pSettings->value("ToolsDialogInfo/ThickNessPicture","").toBool();
	bool bFrontPicture = m_pSettings->value("ToolsDialogInfo/FrontPicture","").toBool();
	bool bSidePicture = m_pSettings->value("ToolsDialogInfo/SidePicture","").toBool();
	bool bWholePicture = m_pSettings->value("ToolsDialogInfo/WholePicture","").toBool();


 	m_pCscanView->m_pWgtThicknessView->setVisible(bThickNessPicture);
	m_pCscanView->m_pWgtFrontView->setVisible(bFrontPicture);
	m_pCscanView->m_pWgtSideView->setVisible(bSidePicture);
	m_pCscanView->m_pWgtWholeView->setVisible(bWholePicture);

	if(m_pToolsDialog->m_pInConPointPage->m_pCFrontView->isChecked())
	{

	}
	else
	{
		m_pCscanView->m_pWgtDetectResult->setVisible(false);
	}

	if(m_pToolsDialog->m_pInConPointPage->m_pCThicknessView->isChecked())
	{

	}
	else
	{
		m_pCscanView->m_pWgtOtherSystem->setVisible(false);
	}
    if (!m_pToolsDialog->m_pInConPointPage->m_pCSideView->isChecked())
    {

		m_pCscanView->m_pWgtOtherSystem->setVisible(false);
		m_pCscanView->m_pWgtDetectResult->setVisible(false);
    }
}

//初始化数据库
bool MainWindow::InitDatabase()
{
	try
	{
		// 数据库文件路径
		QString strPath = QDir::currentPath();
		strPath += "/database.cfg"; 
		// 读取数据库文件中的内容
		QFile File(strPath);
		File.open(QIODevice::ReadOnly);
		QByteArray byData;
		byData.clear();
		byData =  File.readAll();
		File.close();		
		QString strData;
		strData = byData.data();
		QStringList strlData = strData.split(";");
		// 解析各种变量
		QString strUserName = strlData.at(4);
		int n = strUserName.indexOf("=");
		strUserName = strUserName.right(strUserName.length()-n-1);
		QString strUserPWD = strlData.at(5);
		n = strUserPWD.indexOf("=");
		strUserPWD = strUserPWD.right(strUserPWD.length()-n-1);
		// 设置DSN
		QString strTemp = strlData.at(0) +";"+ strlData.at(1)+";" + strlData.at(3)+";";
		QString  strDsn = strTemp.toLocal8Bit(); 
		// 连接数据库
		m_pDatabaseConnection = new QSqlDatabase;
		*m_pDatabaseConnection = QSqlDatabase::addDatabase("QODBC","initConnect");
		m_pDatabaseConnection -> setDatabaseName(strDsn);
		m_pDatabaseConnection -> setUserName(strUserName);;
		m_pDatabaseConnection -> setPassword(strUserPWD);	 
		if(!m_pDatabaseConnection ->open())
		{
			QMessageBox::critical(0,QObject::tr("Database Error"),m_pDatabaseConnection->lastError().text());
			return false;
		}
		g_sqlDB = m_pDatabaseConnection;//全局指针初始化
		return true;
	}
	catch (...)
	{
		QMessageBox::critical(0,QObject::tr("Database Error"),m_pDatabaseConnection->lastError().text());
		return false;
	}

	return true;
}

void MainWindow::DrawColorLabel(int flag)
{
	int nClw = m_pColorLabel->width();
	int nClh = m_pColorLabel->height();
	m_pColorLabel ->setLineWidth(0);
	switch(flag)
	{
		//chenyuzhu modify start on 2014-7-21
	/*case 0: //当量比色表 5个
		{
			m_pColorLabel ->resize(nClw,25);
			nClw = m_pColorLabel->width();
			nClh = m_pColorLabel->height();
			int nDrawx = 0;       // 起始X坐标
			int nDrawy = 0;       // 起始Y坐标
			int nDrawPx = 50;     //文字的起始X坐标
			int nDrawWidth = nClw / 5; // 小矩形的宽
			int nDrawHeight = nClh;    // 小矩形的高
			const QColor Colors[5]= {QColor(142,30,32,255),QColor(255,255,0,255),QColor(254,141,177,255),
																QColor(0,250,0,255),QColor(255,0,0,255)};
			QPixmap pPixmap(nClw,nClh);
			QPainter pPainter;
			bool rnt = pPainter.begin(&pPixmap);
			if(!rnt)
			{
				QString sStr = QString("%1").arg("painter.begin error");
				QMessageBox::information(NULL, "Drawing color label", sStr, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes); 
				return ;
			}
			for (int i = 0;i<5;i++)
			{
				pPainter.setBrush(Colors[i]);
				pPainter.setPen(QColor(73,73,73,255));
				if (4 == i)
				{
					pPainter.drawRect(nDrawx,nDrawy,nClw - nDrawx,nDrawHeight);
				}else
				{
					pPainter.drawRect(nDrawx,nDrawy,nDrawWidth,nDrawHeight);
				}
				nDrawx += nDrawWidth;
				nDrawPx +=nDrawWidth;
			}
			pPainter.end();
			m_pColorLabel->setPixmap(pPixmap);
			break;
		}*/
	case 0:  //分级比色表 3个
		{
			int nDrawx = 0;
			int nDrawy = 0;
			int nDrawPx = 100;
			int nDrawWidth = nClw / 3;
			int nDrawHeight = nClh;
			const QColor Colors[3]= {QColor(0,255,0,255),QColor(255,255,0,255),QColor(255,0,0,255)};
			QPixmap pPixmap(nClw,nClh);
			QPainter pPainter;
			bool rnt = pPainter.begin(&pPixmap);
			if(!rnt)
			{
				QString sStr = QString("%1").arg("painter.begin error");
				QMessageBox::information(NULL, "Drawing color label", sStr, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes); 
				return ;
			}
			pPainter.setBrush(QColor(73,73,73,255));
			pPainter.setPen(QColor(73,73,73,255));
			pPainter.drawRect(nDrawx,nDrawy,nClw,nClh);
			QString sStr1 = QString("%1").arg(tr("合格"));
			QString sStr2 = QString("%1").arg(tr("超标"));
			pPainter.setPen(QColor(0,0,0,255));
			pPainter.setFont(QFont(NULL,15));
			for (int i = 0;i<3;i++)
			{
				pPainter.setBrush(Colors[i]);
				pPainter.drawRect(nDrawx,nDrawy+25,nDrawWidth,nDrawHeight);
				if(0 == i)
				{
					int nMaxZhiShiLength;
					double dDivLengthValue;
					pPainter.drawText(nDrawPx,20,sStr1);
					QString strBiaoZhun = g_strBiaoZhun;
					strBiaoZhun.trimmed();
					if (!QString::compare(strBiaoZhun,QString("GBT2970"),Qt::CaseInsensitive) ||
						!QString::compare(strBiaoZhun,QString("JBT4730"),Qt::CaseInsensitive))
					{
						if (g_strJiBie == ("I"))
						{
							nMaxZhiShiLength = 80;
							dDivLengthValue = 80 / 2;
						}
						else if (g_strJiBie == ("II"))
						{
							nMaxZhiShiLength = 100;
							dDivLengthValue = 100 / 2;
						}
						else if (g_strJiBie == ("III"))
						{
							nMaxZhiShiLength = 120;
							dDivLengthValue = 120 / 2;
						}
						else if (g_strJiBie == ("IV"))
						{
							nMaxZhiShiLength = 150;
							dDivLengthValue = 150 / 2;
						}
					}
					else if (!QString::compare(strBiaoZhun,QString("ASTM 435"),Qt::CaseInsensitive) ||
						!QString::compare(strBiaoZhun,QString("ASTM 435M"),Qt::CaseInsensitive))
					{
						
					}
            //		QString strZhiShiLength = QString("%1%2").arg(tr("指示长度")).arg(nMaxZhiShiLength);
            //		QString strDivZhiShiLength = QString("%1%2").arg(tr("指示长度")).arg(dDivLengthValue);
            //		pPainter.drawText(400,20,strZhiShiLength);
            //		pPainter.drawText(200,20,strDivZhiShiLength);
				}else if (2 == i )
				{
					pPainter.drawText(nDrawPx,20,sStr2);
				}
				nDrawx += nDrawWidth;
				nDrawPx +=nDrawWidth;
			}
			pPainter.end();
			m_pColorLabel->setPixmap(pPixmap);
			break;
		}
	case 1:  //深度比色表30个
		{
			//chenyuzhu modify start on 2014-6-3
			m_pColorLabel ->resize(800,50);
			int nDrawx = 10;
			int nDrawy = 0;
			double nDrawPx = 0.0;//chenyuzhu modify on 2014-6-3
			double nDrawWidth  = ((double)nClw-40.0) / 20.0;//chenyuzhu modify on 2014-6-3
			int nDrawHeight = nClh;
			
			double nDrawColor = ((double)nClw-40.0) / 510.0;//chenyuzhu add on 2014-6-3

		/*	const QColor Colors[20]= {QColor(0,255,0,255),QColor(0,240,14,255),QColor(0,209,41,255),QColor(1,191,68,255),
				QColor(0,160,91,255),QColor(0,132,118,255),QColor(0,107,135,255),QColor(0,74,179,255),
				QColor(0,48,218,255),QColor(0,53,213,255),QColor(0,0,255,255),QColor(20,0,235,255),
				QColor(32,0,210,255),QColor(70,0,186,255),QColor(97,0,167,255),QColor(130,0,124,255),
				QColor(141,0,106,255),QColor(170,0,88,255),QColor(197,0,46,255),QColor(226,0,38,255)};*/
			QPixmap pPixmap(nClw,nClh);
			QPainter pPainter;
			bool rnt = pPainter.begin(&pPixmap);
			if(!rnt)
			{
				QString sStr = QString("%1").arg("painter.begin error");
				QMessageBox::information(NULL, "Drawing color label", sStr, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes); 
				return ;
			}
			pPainter.setBrush(QColor(73,73,73,255));
			pPainter.setPen(QColor(73,73,73,255));
			pPainter.drawRect(nDrawx-10,nDrawy,nClw,nClh);
			float nNum = 0; //chenyuzhu modify on 2014-11-5
			//chenyuzhu add start on 2014-6-3
			double nWidth = (double)nClw-40;
			for (int i = 510; i > 0; i--)
			{
				if (i > 255)
				{
					int Color = i-255;
					pPainter.setBrush(QColor(Color,0,255-Color,255));
					pPainter.setPen(QColor(Color,0,255-Color,255));
					pPainter.drawRect(nDrawx,nDrawy+25,nWidth,nDrawHeight);	
				}
				else
				{
					int Color = i;
					pPainter.setBrush(QColor(0,255-Color,Color,255));
					pPainter.setPen(QColor(0,255-Color,Color,255));
					pPainter.drawRect(nDrawx,nDrawy+25,nWidth,nDrawHeight);		
				}	
				nWidth = nWidth - nDrawColor; 
			}
			//chenyuzhu add end on 2014-6-3
			float nJianju = g_Info->height / 5.0;
			for (int i = 0;i<20;i++)
			{
			//	pPainter.setBrush(Colors[i]);
			//	pPainter.drawRect(nDrawx,nDrawy+25,nDrawWidth,nDrawHeight);					
			//	nDrawx += nDrawWidth;
				pPainter.setPen(QColor(0,0,0,255));
				pPainter.setFont(QFont(NULL,15));
				if(0 == (i % 4) )
				{
					if(nNum<10)
					{
						QString sStr = QString("%1").arg(nNum);
						pPainter.drawText(nDrawPx+5,25,sStr);
						nDrawPx +=4*nDrawWidth;
						//nNum = nNum+6;
						nNum = nNum + nJianju; //chenyuzhu add on 2014-11-5
					}else
					{
						QString sStr = QString("%1").arg(nNum);
						pPainter.drawText(nDrawPx,25,sStr);
						nDrawPx +=4*nDrawWidth;
						
						//nNum = nNum+6;
						nNum = nNum + nJianju; //chenyuzhu add on 2014-11-5
					}
				}else if (19 == i)
				{
					QString sStr = QString("%1%2").arg(nNum).arg("mm");
					pPainter.drawText(nDrawPx-2,25,sStr);
				}
			}
			pPainter.end();
			m_pColorLabel->setPixmap(pPixmap);
			break;
		//chenyuzhu modify end on 2014-6-3
		}
	//chenyuzhu modify end on 2014-7-21
	}
}
//slot for combobox
void MainWindow::onComBoChange(int index)
{
	g_nComboxColor = index;
	m_pCscanView->DrawWholeDetial(); //全局视图//chenyuzhu add on 2014-11-4
	m_pCscanView->m_pWgtCscanDetectView->m_bSectionImage = false;//chenyuzhu add on 2014-11-4
	m_pCscanView->DrawDetectDetial();//chenyuzhu add on 2014-11-4
	m_pCscanView->m_pWgtCscanDetectView->m_bBiSeBiao = true;
	m_pCscanView->update();//chenyuzhu add on 2014-7-21
	DrawColorLabel(index);
}

void MainWindow::createToolsPanel()
{
	m_pToolsDialog = new CToolsDialog(m_pMToolsDialogInfo,this);
}

// 响应工具面板的槽函数
void MainWindow::ExpandAction(void)
{
	m_pToolsDialog->exec();
}

void MainWindow::OnDetectListDlgAction()
{
	m_pDetectListDlg->exec();
}
void MainWindow::CsanDetectDoubleClick(QMouseEvent *e)
{
	//当前鼠标位置
	int x = e->x();
	int y = e->y();
	//当前窗体长度高度
	int nlength = m_pCscanView->m_pWgtCscanDetectView->width()-40;
	int nheight = m_pCscanView->m_pWgtCscanDetectView->height()-40;
	if (x < nlength/3 )
	{
		if (y < nheight/3)
		{
			//左侧，上侧
			m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->isChecked());
			m_pToolsDialog->m_pInConPointPage->m_pTopCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pTopCheck->isChecked());
		}
		else if (y > nheight/3 * 2)
		{
			//左侧，下侧
			m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->isChecked());
			m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->isChecked());
		}
		else
		{
			//左侧
			m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->isChecked());
		}
	}
	else if (x > nlength/3 * 2)
	{
		if (y < nheight/3)
		{
			//右侧，上侧
			m_pToolsDialog->m_pInConPointPage->m_pRightCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pRightCheck->isChecked());
			m_pToolsDialog->m_pInConPointPage->m_pTopCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pTopCheck->isChecked());
		}
		else if (y > nheight/3 * 2)
		{
			//右侧，下侧
			m_pToolsDialog->m_pInConPointPage->m_pRightCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pRightCheck->isChecked());
			m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->isChecked());
		}
		else
		{
			//右侧
			m_pToolsDialog->m_pInConPointPage->m_pRightCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pRightCheck->isChecked());
		}
	}
	else
	{
		if (y < nheight/3)
		{
			//上侧
			m_pToolsDialog->m_pInConPointPage->m_pTopCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pTopCheck->isChecked());
		}
		else if (y > nheight/3 * 2)
		{
			//下侧
			m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->setChecked(!m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->isChecked());
		}
	}
}

void  MainWindow::LeftViewChange(int state)
{
	m_pCscanView->m_pWgtLeftView->setVisible(state);
}
void  MainWindow::RightViewChange(int state)
{
	m_pCscanView->m_pWgtRightView->setVisible(state);
}
void  MainWindow::TopViewChange(int state)
{
	m_pCscanView->m_pWgtTopView->setVisible(state);
}
void  MainWindow::BottomViewChange(int state)
{
	m_pCscanView->m_pWgtBottomView->setVisible(state);
}


//语言切换的重写函数
void MainWindow::retranslate() 
{
	if (1 == m_pColorType->currentIndex())
	{
		onComBoChange(1);
	}
	setWindowTitle(tr("武钢中厚钢板超声波自动探伤系统")); //chenyuzhu modify on 2014-9-29 
//	m_pColorType->setItemText(0,tr("当量比色表"));//chenyuzhu modify on 2014-7-21
	m_pColorType->setItemText(1,tr("分级比色表"));
	m_pColorType->setItemText(2,tr("深度比色表"));
	m_pDockWidgetParameterDisp ->setWindowTitle(tr("钢板参数"));
}

//用来接受语言切换事件
void MainWindow::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);  
	switch (e->type())  
	{  
	case QEvent::LanguageChange:  
		retranslate();  
		break;  
	default:  
		break;  
	}
}
void MainWindow::UpdateDockWidgetParameterDisp(void)
{
    m_pDockWidgetParameterDisp->m_plineEdit->setText(m_pPlateInfo->m_strArtName);
	m_pDockWidgetParameterDisp->m_plineEdit->setAlignment(Qt::AlignCenter);

	m_pDockWidgetParameterDisp->m_pleDStandard->setText(m_pPlateInfo->m_strDetectStandard);
	m_pDockWidgetParameterDisp->m_pleDStandard->setAlignment(Qt::AlignCenter);

    m_pDockWidgetParameterDisp->m_pleDGrade->setText(m_pPlateInfo->m_strBodyLevel);
	m_pDockWidgetParameterDisp->m_pleDGrade->setAlignment(Qt::AlignCenter);

	m_pDockWidgetParameterDisp->m_pleSNumber->setText(m_pPlateInfo->m_strSteelNumber);
	m_pDockWidgetParameterDisp->m_pleSNumber->setAlignment(Qt::AlignCenter);

    QString strTemp = QString("%1").arg(m_pPlateInfo->m_nPlateLength);
	m_pDockWidgetParameterDisp->m_pleLength->setText(strTemp);
    m_pDockWidgetParameterDisp->m_pleLength->setAlignment(Qt::AlignCenter);

	strTemp = QString("%1").arg(m_pPlateInfo->m_nPlateWidth);
	m_pDockWidgetParameterDisp->m_pleWidth->setText(strTemp);
    m_pDockWidgetParameterDisp->m_pleWidth->setAlignment(Qt::AlignCenter);

	strTemp = QString("%1").arg(m_pPlateInfo->m_nPlateThickness);
	m_pDockWidgetParameterDisp->m_pleThickness->setText(strTemp);
    m_pDockWidgetParameterDisp->m_pleThickness->setAlignment(Qt::AlignCenter);
}

void MainWindow::TimerUpdate(void)
{
	QDateTime time = QDateTime::currentDateTime();
	QString strTiem = time.toString("yyyy-MM-dd hh:mm:ss");
	m_pSTime->setText(strTiem);
}
void MainWindow::OnStatusChangeLanguage(void)
{
	QPoint pos;
	pos.setX(0);
	pos.setY(-m_pSMenu->sizeHint().height());
	m_pSMenu->exec(m_pSChangeLanguage->mapToGlobal(pos));
}
void MainWindow::OnChangeToChinese(void)
{

	QString QmName;  
	QTranslator Translator;  
	QmName = "mainwindow_zh.qm";  
	QString QmPath = QString("%1%2").arg("./").arg(QmName);
	if(m_pTranslator->load(QmPath))  
	{  
		qApp ->installTranslator(m_pTranslator);  
	} 
	if (1 == m_pColorType->currentIndex())
	{
		onComBoChange(1);
	}
	m_pSChangeLanguage->setText(tr("中文"));
}


void MainWindow::OnChangeToEnglish(void)
{
	QString QmName;  
	QTranslator Translator;  
	QmName = "mainwindow_en.qm";  
	QString QmPath = QString("%1%2").arg("./").arg(QmName);
	if(m_pTranslator->load(QmPath))  
	{  
		qApp ->installTranslator(m_pTranslator);  
	} 
	if (1 == m_pColorType->currentIndex())
	{
		onComBoChange(1);
	}
	m_pSChangeLanguage->setText(tr("英文"));
}

void MainWindow::OnParameterAction(void)
{
	//chenyuzhu modify start on 2014-7-23
	PrepareStartDetect();
	INIT_CONFIG(allCH_CONFIG, &ch_config_global);  //初始化通道信息//chenyuzhu add on 2014-7-20 查询钢板厚获取通道信息后 重新初始化通道
	//OnSetPlateID();
	//chenyuzhu modify end on 2014-7-23
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == m_pIsReset)
	{
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			return FilerChangeStatus();
		}
	}
	return QMainWindow::eventFilter(obj, event);
}

bool MainWindow::FilerChangeStatus(void)
{
	OnSetPlateID();
	return true;
}

//窗口关闭

void MainWindow::closeEvent(QCloseEvent *event)
{

	SaveToolDialogInfo();
	if (-1 != m_pCscanView->m_nTimerSTID)
	{
		killTimer(m_pCscanView->m_nTimerSTID);
		m_pCscanView->m_nTimerSTID = -1;
	}
	//关闭主窗口时如果A扫窗口没关闭，则一同关闭A扫
	if (m_pAscan->isVisible())
	{
		m_pAscan->close();
	}

	for (UINT i=0; i < m_nAECCardNum; i++)                     //创建线程对象，并把参数传给线程对象
	{
		m_pAECWorkThread[i]->m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_THREAD_STOP;
	}

	Sleep(100);
	killTimer(m_nTimerID_PROCESS);
	m_nTimerID_PROCESS = -1;
	killTimer(m_nTimerID_PLC);
	m_nTimerID_PLC = -1;
	killTimer(m_nTimerID_LOG);
	m_nTimerID_LOG = -1;

	m_PLXCommunication.UnInit(m_stThreadParam);
	m_OPCComponent.StopThread();

	// 关闭加密狗
#if HAVE_DOG
	ElepClose(m_ElepHandle);
#endif
}

//保存ToolDoialog的参数配置信息
void MainWindow::SaveToolDialogInfo(void)
{
	if (m_pSettings != NULL)
	{
		m_pSettings->setValue("ToolsDialogInfo/TopEdgePicture",m_pMToolsDialogInfo->m_bTopEdgePicture);
		m_pSettings->setValue("ToolsDialogInfo/BottonEdgePicture",m_pMToolsDialogInfo->m_bBottonEdgePicture);
		m_pSettings->setValue("ToolsDialogInfo/LeftEdgePicture",m_pMToolsDialogInfo->m_bLeftEdgePicture);
		m_pSettings->setValue("ToolsDialogInfo/RightEdgePicture",m_pMToolsDialogInfo->m_bRightEdgePicture);
		m_pSettings->setValue("ToolsDialogInfo/ThickNessPicture",m_pMToolsDialogInfo->m_bThickNessPicture);
		m_pSettings->setValue("ToolsDialogInfo/FrontPicture",m_pMToolsDialogInfo->m_bFrontPicture);
		m_pSettings->setValue("ToolsDialogInfo/SidePicture",m_pMToolsDialogInfo->m_bSidePicture);
		m_pSettings->setValue("ToolsDialogInfo/WholePicture",m_pMToolsDialogInfo->m_bWholePicture);
		m_pSettings->setValue("ToolsDialogInfo/CentreCrosingLine",m_pMToolsDialogInfo->m_bCentreCrosingLine);
		m_pSettings->setValue("ToolsDialogInfo/InjPointRangeLine",m_pMToolsDialogInfo->m_bInjPointRangeLine);
		m_pSettings->setValue("ToolsDialogInfo/InjPointPath",m_pMToolsDialogInfo->m_bInjPointPath);

		m_pSettings->setValue("ToolsDialogInfo/InjPointFilter",m_pMToolsDialogInfo->m_bInjPointFilter);
		m_pSettings->setValue("ToolsDialogInfo/FilterBiger",m_pMToolsDialogInfo->m_bFilterBiger);
		m_pSettings->setValue("ToolsDialogInfo/FilterSmaller",m_pMToolsDialogInfo->m_bFilterSmaller);
		QString temp = QString("%1").arg(m_pMToolsDialogInfo->m_nFilterBigerNum / 100); //chenyuzhu modify on 2014-10-30
		m_pSettings->setValue("ToolsDialogInfo/FilterBigerNum",temp);
		temp = QString("%1").arg(m_pMToolsDialogInfo->m_nFilterSmallerNum / 100); //chenyuzhu modify on 2014-10-30
		m_pSettings->setValue("ToolsDialogInfo/FilterSmallerNum",temp);

		m_pSettings->setValue("ToolsDialogInfo/NextDefectDistant",m_pMToolsDialogInfo->m_bNextDefectDistant);
		temp = QString("%1").arg(m_pMToolsDialogInfo->m_nDefectDistaneSmaller);
		m_pSettings->setValue("ToolsDialogInfo/DefectDistaneSmaller",temp);
		m_pSettings->setValue("ToolsDialogInfo/DependOnMInput",m_pMToolsDialogInfo->m_bDependOnMInput);
		m_pSettings->setValue("ToolsDialogInfo/PrintInjPointDetailPic",m_pMToolsDialogInfo->m_bPrintInjPointDetailPic);
		m_pSettings->setValue("ToolsDialogInfo/Address",m_pMToolsDialogInfo->m_strAddress);
		m_pSettings->setValue("ToolsDialogInfo/UserName",m_pMToolsDialogInfo->m_strUserName);
		m_pSettings->setValue("ToolsDialogInfo/Password",m_pMToolsDialogInfo->m_strPwd);
	}	
}

void MainWindow::OnUpdateToolDialogOnCancel()
{
	//4个边缘窗口的显示
	bool bTopCheckBoxState =m_pMToolsDialogInfo->m_bTopEdgePicture;
	bool bBottonCheckBoxState = m_pMToolsDialogInfo->m_bBottonEdgePicture;
	bool bLeftCheckBoxState = m_pMToolsDialogInfo->m_bLeftEdgePicture;
	bool nRightCheckBoxState = m_pMToolsDialogInfo->m_bRightEdgePicture;

	m_pToolsDialog->m_pInConPointPage->m_pLeftCheck->setChecked(bLeftCheckBoxState);
	m_pToolsDialog->m_pInConPointPage->m_pRightCheck->setChecked(nRightCheckBoxState);
	m_pToolsDialog->m_pInConPointPage->m_pTopCheck->setChecked(bTopCheckBoxState);
	m_pToolsDialog->m_pInConPointPage->m_pBottonCheck->setChecked(bBottonCheckBoxState);

	//4个子窗口的显示
	bool bThickNessPicture =m_pMToolsDialogInfo->m_bThickNessPicture;
	bool bSidePicture = m_pMToolsDialogInfo->m_bSidePicture;
	bool bWholePicture = m_pMToolsDialogInfo->m_bWholePicture;
	bool bFrontPicture = m_pMToolsDialogInfo->m_bFrontPicture;

	m_pToolsDialog->m_pInConPointPage->m_pCThicknessView->setChecked(bThickNessPicture);
	m_pToolsDialog->m_pInConPointPage->m_pCSideView->setChecked(bSidePicture);
	m_pToolsDialog->m_pInConPointPage->m_pCOverView->setChecked(bWholePicture);
	m_pToolsDialog->m_pInConPointPage->m_pCFrontView->setChecked(bFrontPicture);
	m_pToolsDialog->m_pInConPointPage->m_pReticleCheck->setChecked(m_pToolsDialog->m_pTToolsDialogInfo->m_bCentreCrosingLine);
	m_pToolsDialog->m_pInConPointPage->m_pPointRangeCheck->setChecked(m_pToolsDialog->m_pTToolsDialogInfo->m_bInjPointRangeLine);
}


//槽函数
void MainWindow::CScanThicknessWidgetChange(int state) //厚度图
{
	m_pCscanView->m_pWgtThicknessView->setVisible(state);
	m_pCscanView->m_pWgtOtherSystem->setVisible(state);

	if(!m_pToolsDialog->m_pInConPointPage->m_pCSideView->isChecked())
	{
		m_pCscanView->m_pWgtOtherSystem->setVisible(false);
	}
}
void MainWindow::CScanFrontWidgetWidgetChange(int state) //正视图
{
	m_pCscanView->m_pWgtFrontView->setVisible(state);
	m_pCscanView->m_pWgtDetectResult->setVisible(state);

	if(!m_pToolsDialog->m_pInConPointPage->m_pCSideView->isChecked())
	{
		m_pCscanView->m_pWgtDetectResult->setVisible(false);
	}
}
void MainWindow::CScanSideWidgetChange(int state) //侧视图
{
	m_pCscanView->m_pWgtSideView->setVisible(state);
	m_pCscanView->m_pWgtDetectResult->setVisible(state);
	m_pCscanView->m_pWgtOtherSystem->setVisible(state);

	if(m_pToolsDialog->m_pInConPointPage->m_pCFrontView->isChecked())
	{
	}else
	{
		m_pCscanView->m_pWgtDetectResult->setVisible(false);
	}

	if(m_pToolsDialog->m_pInConPointPage->m_pCThicknessView->isChecked())
	{
	}else
	{
		m_pCscanView->m_pWgtOtherSystem->setVisible(false);
	}

}
void MainWindow::CScanWholeWidgetChange(int state) //整体视图
{
	m_pCscanView->m_pWgtWholeView->setVisible(state);
}

void MainWindow::OnAscanAction(void)
{
	m_pAscan->setVisible(!m_pAscan->isVisible());
}

void MainWindow::customEvent( QEvent *event )
{
	int type = event->type();
	if ( type == A_SCAN_EVENT ) 
	{  
		m_pAscan->SetAScanData(((ASCANEvent*)event)->wChannelNo,((ASCANEvent*)event)->nFlawOrBottom,((ASCANEvent*)event)->pData);
	}
}
void MainWindow::OPCRead(HRESULT *pErrors,OPCHANDLE *phServer,int nItemCount)
{
	HRESULT temp = *pErrors;
	// 读PLC2WSCommand值  0---无效	1---探伤 2---自动标定
	IUnknown *pIunknown;
	HRESULT hr = m_OPCComponent.GetIO2Interface(IID_IOPCAsyncIO2, (IUnknown**)&pIunknown);
	if (FAILED(hr))
	{
		return;
	}
	IOPCAsyncIO2* m_IOPCAsyncIO2 = NULL;
	hr = pIunknown->QueryInterface(IID_IOPCAsyncIO2, (void**)&m_IOPCAsyncIO2);
	if (FAILED(hr))
	{
		pIunknown->Release();
		return;
	}
	DWORD dwCancelID;
	HRESULT r1;


	if((*pErrors)!=S_OK)       //Item 不可用
	{
		QMessageBox::information(NULL, "Error", tr("OPC Item 不可用，不能用异步读功能!"), QMessageBox::Yes, QMessageBox::Yes);
		m_IOPCAsyncIO2->Release();
		pIunknown->Release();
		return;
	}

	r1 = m_IOPCAsyncIO2->Read(nItemCount, // [in]读 1 个 Item
		phServer,   // [in] 定义的Item
		1,          //  [out] 客户Transaction ID
		&dwCancelID, // [out] 服务器Cancel ID
		&pErrors // [out] 服务器返回的错误码
		);
	//delete[] phServer;

	if (r1 == S_FALSE)
	{
		QMessageBox::information(NULL, "Error", tr("Read()错误"), QMessageBox::Yes, QMessageBox::Yes);
		m_IOPCAsyncIO2->Release();
		pIunknown->Release();
		return;
	}
	if (FAILED(r1))
	{
		QMessageBox::information(NULL, "Error", tr("异步读失败!"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		//释放内存，如果操作成功
		CoTaskMemFree(pErrors);
	}
	m_IOPCAsyncIO2->Release();
	pIunknown->Release();
}
bool MainWindow::OPCWrite(HRESULT *pSErrors,OPCHANDLE *phServer,VARIANT *values,const unsigned long& nItemCount)
{
	IUnknown *pIunknown;
	HRESULT hr = m_OPCComponent.GetIO2Interface(IID_IOPCAsyncIO2, (IUnknown**)&pIunknown);
	if (FAILED(hr))
	{
		QMessageBox::information(NULL,tr("MainWindow::OPCWrite"),tr("GetIO2Interface失败"));
		return false;
	}

	IOPCAsyncIO2* m_IOPCAsyncIO2 = NULL;
	hr = pIunknown->QueryInterface(IID_IOPCAsyncIO2, (void**)&m_IOPCAsyncIO2);
	if (FAILED(hr))
	{
		QMessageBox::information(NULL,tr("MainWindow::OPCWrite"),tr("QueryInterface失败"));
		pIunknown->Release();
		return false;
	}

	for (int i = 0; i<nItemCount; i++ )
	{
		if (pSErrors[i] != S_OK)    // Item not available
		{
			QMessageBox::information(NULL, "MainWindow::OPCWrite", tr("OPC Item 不可用，不能用异步读功能!"));
			m_IOPCAsyncIO2->Release();
			pIunknown->Release();
			return false;
		}
	}

	DWORD dwCancelID;
	HRESULT *pErrors;
	HRESULT r1;
	LPWSTR ErrorStr;

	r1 = m_IOPCAsyncIO2->Write( nItemCount, // [in] 写 1 Item
		phServer, // [in] 定义的Item
		values, // [in] 定义的值
		2, // [in] 客户 transaction ID
		&dwCancelID,// [out] 服务器 Cancel ID
		&pErrors // [out] 服务器返回的错误码
		);

	if (r1 == S_FALSE)
	{
		m_OPCComponent.m_IOPCServer->GetErrorString(pErrors[0], LOCALE_ID,&ErrorStr);
	}
	if (FAILED(r1))
	{
		QMessageBox::information(NULL, "Error", tr("异步写Item 错误"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{ 
		CoTaskMemFree(pErrors);
	}
	m_IOPCAsyncIO2->Release();
	pIunknown->Release();

	DWORD ret = ::WaitForSingleObject(hOPCWriteEvent,INFINITE);	
	ResetEvent(hOPCWriteEvent);
	return true;
}

bool MainWindow::EndReadDetectFile()
 {
//	if (!StartReadDetectFile())
//	{
//		return false;
//	}
    m_pCscanView->CalcResultData(); //读取文件创建utimage及判伤识别器
	//chenyuzhu add start on 2014-9-29
/*	if (m_pBiaoZhun != NULL)
	{
		delete m_pBiaoZhun;
		m_pBiaoZhun = NULL;
	}
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	if (!QString::compare(strBiaoZhun,QString("GBT2970"),Qt::CaseInsensitive) ||
		!QString::compare(strBiaoZhun,QString("JBT4730"),Qt::CaseInsensitive))
	{
		m_pBiaoZhun = new CBiaoZhun_GBT2970(g_strJiBie);
	}
	m_pBiaoZhun->GetDrawSquarePosList();//在取得矩形列表的同时改变了(buf4)伤点的颜色值,因此必须在画伤操作前执行此操作//chenyuzhu add on 2014-9-29
*/
    //chenyuzhu add end on 2014-9-29
	g_bReadyDraw = true;//用于标识已读取完伤点文件，可以继续下面操作

	//读取零点配置信息
	int nTempValue = m_pSettings->value("CscanPanel/ZeroPos").toInt();
	if (0 == nTempValue) //判断配置
	{
		m_pSettings->setValue("CscanPanel/ZeroPos",1);
		nTempValue = 1;
	}
	else if(!(1 == nTempValue || 2 == nTempValue || 3 == nTempValue || 4 == nTempValue))
	{
		QMessageBox::information(this,"EndReadDetectFile",tr("零点配置错误，按默认处理"));
		nTempValue = 1;		
	}
	nTempValue = 4; //chenyuzhu add on 2014-10-27右上角为0点
	if (1 == nTempValue)
	{
		m_pCscanView->m_ZeroPosFlag = leftTop;
	}
	else if (2 == nTempValue)
	{
		m_pCscanView->m_ZeroPosFlag = leftBottom;
	}
	else if (3 == nTempValue)
	{
		m_pCscanView->m_ZeroPosFlag = rightTop;
	}
	else if (4 == nTempValue)
	{
		m_pCscanView->m_ZeroPosFlag = rightBottom;
	}
	else
	{}
	m_pCscanView->m_pWgtCscanDetectView->m_nSectionFlag = 0;  //放大计数器置零
	m_pCscanView->m_pWgtCscanDetectView->m_CurrentWholeSection = QRect(0,0,0,0);
	m_pCscanView->m_pWgtFrontView->m_bBoostFlag = false;
	m_pCscanView->m_pWgtSideView->m_bBoostFlag = false;
	m_pCscanView->m_pWgtWholeView->m_bBoostFlag = false;
	if( NULL != m_pCscanView->m_pWgtCscanDetectView->m_pSectionImage)
	{
		delete m_pCscanView->m_pWgtCscanDetectView->m_pSectionImage;
		m_pCscanView->m_pWgtCscanDetectView->m_pSectionImage = NULL;
	}
   //初始化一些变量
	m_pCscanView->m_nPlateYEnd = m_pCscanView->m_nPlateYStart + g_Info[nPlate].width
 		*(float(m_pCscanView->m_pWgtCscanDetectView->rect().height())/(float)m_pCscanView->m_pWgtCscanDetectView->rect().width());
	//ASSERT(m_PlateYStart!=m_PlateYEnd);
	m_pCscanView->m_RegionRect = m_pCscanView->m_pWgtWholeView->rect();

	int temp = float(g_Info[nPlate].width)*(float(m_pCscanView->m_pWgtCscanDetectView->height())/float(m_pCscanView->m_pWgtCscanDetectView->width()))
				/float(g_Info[nPlate].length)*m_pCscanView->m_pWgtWholeView->width();

    if (m_pCscanView->m_ZeroPosFlag < 3)
    {
		m_pCscanView->m_RegionRect.setRight(m_pCscanView->m_RegionRect.left()+temp);
    }else
	{
		m_pCscanView->m_RegionRect.setLeft(m_pCscanView->m_RegionRect.right()-temp);
	}
	m_pCscanView->m_pWgtWholeView->m_nXSize = m_pCscanView->m_pWgtWholeView->width();
	m_pCscanView->m_pWgtWholeView->m_nYSize = m_pCscanView->m_pWgtWholeView->height();
	//更新正视图中的长度指针
	m_pCscanView->m_pWgtFrontView->m_nPlateYStart =  &m_pCscanView->m_nPlateYStart;
	m_pCscanView->m_pWgtFrontView->m_nPlateYEnd = &m_pCscanView->m_nPlateYEnd;
	//更新侧视图中的长度指针
	m_pCscanView->m_pWgtSideView->m_nPlateYStart =  &m_pCscanView->m_nPlateYStart;
	m_pCscanView->m_pWgtSideView->m_nPlateYEnd = &m_pCscanView->m_nPlateYEnd;
	//更新放大正视图的区域
	m_pCscanView->m_pWgtFrontView->m_BoostImageRect = m_pCscanView->m_pWgtFrontView->rect();
	m_pCscanView->m_pWgtSideView->m_BoostImageRect = m_pCscanView->m_pWgtSideView->rect();
	m_pCscanView->m_nDoubleClickedDoshRectID = -1;
	m_pCscanView->update();
 	m_pCscanView->JiSuanBiaoZhun();//chenyuzhu add on start 2014-7-25
	return true;
}

void MainWindow::InitMapChanel()
{
	CH_CONFIG stAllCH_CONFIG[MAX_CHANNEL_NO];
	memcpy(stAllCH_CONFIG, allCH_CONFIG, sizeof(CH_CONFIG) * MAX_CHANNEL_NO);
	int nBianTan1ChanelNum = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan1ChanelNumber;
	int nBianTan2ChanelNum = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan2ChanelNumber;
	int nNeiTan1ChanelNumber = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan1ChanelNumber;
	int nNeiTan2ChanelNumber = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan2ChanelNumber;

	int nBianTan1WuliBegin  = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan1WuliBegin;
	int nBianTan2WuliBegin  = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan2WuliBegin;
	int nNeiTan1WuliBegin  = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan1WuliBegin;
	int nNeiTan2WuliBegin  = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan2WuliBegin;
	//边探1
	int countFlag = 0;
	int BoxIndex = 0;
	QVector<ST_CHANEL> vChannel;
	InitMapForChannel(nBianTan1WuliBegin,nBianTan1ChanelNum,stAllCH_CONFIG,countFlag,BoxIndex,vChannel);
   //边探2
	InitMapForChannel(nBianTan2WuliBegin,nBianTan2ChanelNum,stAllCH_CONFIG,countFlag,BoxIndex,vChannel);
	//内探1
	InitMapForChannel(nNeiTan1WuliBegin,nNeiTan1ChanelNumber,stAllCH_CONFIG,countFlag,BoxIndex,vChannel);
	//内探2
	InitMapForChannel(nNeiTan2WuliBegin,nNeiTan2ChanelNumber,stAllCH_CONFIG,countFlag,BoxIndex,vChannel);
}

void MainWindow::InitMapForChannel(int& nWuliBegin,int& nChannelNum,CH_CONFIG* stAllCH_CONFIG,int& countFlag,int& nBoxIndex,QVector<ST_CHANEL>& vChannel)
{
	for (int i = nWuliBegin;i <= nWuliBegin + nChannelNum;i++ )
	{
		if (countFlag<3)
		{
			ST_CHANEL temp;
			if (countFlag == 0)
			{
				temp.isSelect = true;
			}
			else
			{
				temp.isSelect = false;
			}
			temp.PhysicalNo = i;
			temp.LogicNo = stAllCH_CONFIG[i].CH_REF_NO;
			vChannel.append(temp);
			countFlag++;
		}
		else
		{
			m_mapChanel[nBoxIndex] = vChannel;
			nBoxIndex++;
			countFlag = 0;
			vChannel.clear();
			if (i < nWuliBegin + nChannelNum)
			{
				i--;
			}
		}
	}
}


bool MainWindow::UpdateMonitorWndTemp(int nCardNum)  //让报伤的通道闪几下
{
	int NeiTan1Num = g_OtherParameters.insideCar1BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探1通道数量
	int NeiTan2Num = g_OtherParameters.insideCar2BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探2通道数量
	int nBianAllNum = g_OtherParameters.insideCar1PhysicalChannelStart;										// 边探所有通道数量
	int nBian1Begin = g_OtherParameters.edgeCar1PhysicalChannelStart;										// 边探1物理通道号起始
	int nBian2Begin = g_OtherParameters.edgeCar2PhysicalChannelStart;										// 边探2物理通道号起始
	int nNei1Begin = g_OtherParameters.insideCar1PhysicalChannelStart;										// 内探1物理通道号起始
	int nNei2Begin = g_OtherParameters.insideCar2PhysicalChannelStart;										// 内探2物理通道号起始
	int nBian1LogicalBegin = g_OtherParameters.edgeCar1LogicalChannelStart;									// 边探1逻辑通道号起始
	int nBian2LogicalBegin = g_OtherParameters.edgeCar2LogicalChannelStart;									// 边探2逻辑通道号起始
	int nNei1LogicalBegin = g_OtherParameters.insideCar1LogicalChannelStart;								// 内探1逻辑通道号起始
	int nNei2LogicalBegin = g_OtherParameters.insideCar2LogicalChannelStart;								// 内探2逻辑通道号起始

	g_ItemCS.lock();
	BYTE tmpCommand2 = g_ItemValue.dwPLC2WSCommand;
	g_ItemCS.unlock();

	// command = 3, 标定报伤开关关闭时不报伤
	if (tmpCommand2 == 3 && !m_bBiaoDingReport)
		return FALSE;

	if (g_MonitorFlaw[nCardNum].CH_NO == 0)
	{
		return FALSE;
	}

	if(m_pDockWidgetChannelCtr->m_pmyWidget->m_nTimeId == -1)//如果上个提示没完，则不报下一个伤
	{
		int nLogicalNum = g_MonitorFlaw[nCardNum].CH_NO;//获得伤点的逻辑通道号

		if(nLogicalNum < nBian2LogicalBegin && nLogicalNum >= nBian1LogicalBegin)//边探1
		{
			int nWuLiNum = nLogicalNum - nBian1LogicalBegin + nBian1Begin;
			int nSize  = m_pDockWidgetChannelCtr->m_pmyWidget->m_EdgeVectorToWLNum.size();
			int i = 0;
			for(i;i<nSize;i++) //获得边探矩形的下标索引
			{
				if(m_pDockWidgetChannelCtr->m_pmyWidget->m_EdgeVectorToWLNum[i] == nWuLiNum)
						break;	
			}
			m_pDockWidgetChannelCtr->m_pmyWidget->m_bIndexFlag = false; //边探标示
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nCurrentRect = i;//修改当前选中矩形索引
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nTimeId = m_pDockWidgetChannelCtr->m_pmyWidget->startTimer(100);  //开启定时器用于让选中的通道号闪几下
		}
		else if( nLogicalNum >= nBian2LogicalBegin && nLogicalNum < nNei1LogicalBegin) //边探2
		{
			int nWuLiNum = nLogicalNum - nBian2LogicalBegin + nBian2Begin;
			int nSize  = m_pDockWidgetChannelCtr->m_pmyWidget->m_EdgeVectorToWLNum.size();
			int i = 0;
			for(i;i<nSize;i++) //获得边探矩形的下标索引
			{
				if(m_pDockWidgetChannelCtr->m_pmyWidget->m_EdgeVectorToWLNum[i] == nWuLiNum)
						break;	
			}
			m_pDockWidgetChannelCtr->m_pmyWidget->m_bIndexFlag = false; //边探标示
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nCurrentRect = i;//修改当前选中矩形索引
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nTimeId = m_pDockWidgetChannelCtr->m_pmyWidget->startTimer(100);  //开启定时器用于让选中的通道号闪几下	
		}
		else if( nLogicalNum >= nNei1LogicalBegin && nLogicalNum < nNei2LogicalBegin) //内探1
		{
			int nWuLiNum = nLogicalNum - nNei1LogicalBegin + nNei1Begin;
			int nSize  = m_pDockWidgetChannelCtr->m_pmyWidget->m_InsideVectorToWLNum.size();
			int i = 0;
			for(i;i<nSize;i++) //获得边探矩形的下标索引
			{
				if(m_pDockWidgetChannelCtr->m_pmyWidget->m_InsideVectorToWLNum[i] == nWuLiNum)
						break;	
			}
			m_pDockWidgetChannelCtr->m_pmyWidget->m_bIndexFlag = true;	//内探标示
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nCurrentRect = i;	//修改当前选中矩形索引
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nTimeId = m_pDockWidgetChannelCtr->m_pmyWidget->startTimer(100);  //开启定时器用于让选中的通道号闪几下			
		}
		else if( nLogicalNum >=nNei2LogicalBegin) //内探2
		{
			int nWuLiNum = nLogicalNum - nNei2LogicalBegin + nNei2Begin;
			int nSize  = m_pDockWidgetChannelCtr->m_pmyWidget->m_InsideVectorToWLNum.size();
			int i = 0;
			for(i;i<nSize;i++) //获得边探矩形的下标索引
			{
				if(m_pDockWidgetChannelCtr->m_pmyWidget->m_InsideVectorToWLNum[i] == nWuLiNum)
						break;	
			}
			m_pDockWidgetChannelCtr->m_pmyWidget->m_bIndexFlag = true; //内探标示
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nCurrentRect = i;//修改当前选中矩形索引
			m_pDockWidgetChannelCtr->m_pmyWidget->m_nTimeId = m_pDockWidgetChannelCtr->m_pmyWidget->startTimer(100);  //开启定时器用于让选中的通道号闪几下		
		}
		m_pDockWidgetChannelCtr->m_pmyWidget->DrawingBackground(); //chenyuzhu add start on 2014-8-11解决通道闪烁问题
	}
	return true;
}
BOOL MainWindow::UpdateMonitorWnd(int nCardNum)
{
	g_ItemCS.lock();
	BYTE tmpCommand2 = g_ItemValue.dwPLC2WSCommand;
	g_ItemCS.unlock();

	// command = 3, 标定报伤开关关闭时不报伤
	if (tmpCommand2 == 3 && !m_bBiaoDingReport)
		return FALSE;

	if (g_MonitorFlaw[nCardNum].CH_NO == 0)
	{
		return FALSE;
	}

	float x_distance = 0.0f;
	float y_distance = 0.0f;

	DeltaX dx;
	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
	//	dx.m_fNeiTan3DeltaX = dx.m_fNeiTan4DeltaX = g_IniParams.m_fNeitan34_FreeEdge_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;
	BOOL bRet = CalculateFlawXY(g_MonitorFlaw[nCardNum], x_distance, y_distance, dx);
	if (!bRet)
	{
		return FALSE;
	}

	if ((x_distance > g_Info[nPlate].length || y_distance > g_Info[nPlate].width ) && tmpCommand2 == 1)
	{
		return FALSE;
	}

	// 判断DETECT_STATE，设channel的enabel值
	float fZ = Dot2Range(VSOUND,g_MonitorFlaw[nCardNum].CH_FLAW_TIME);

	// 根据长度选择过滤需要的Z值
	float fGuoLvZ = GetGuoLvZ(x_distance, y_distance);

	// 去掉失波显示控制
	if (//g_MonitorFlaw[nCardNum].CH_NO == 0 
		(g_MonitorFlaw[nCardNum].DETECT_STATE == TYPE_LOST_WAVE && g_IniParams.m_nDisplayShiBo != 1) 
		||	!IsValidData(x_distance, y_distance, fZ) //fZ < g_IniParams.m_fminZ
		)	// 不显示失波
	{
		// 若plccommand 为标定命令或探伤命令时，点标定命令，弹出标定对话框时，不滤掉失波
		g_ItemCS.lock();
		BYTE tmpCommand = g_ItemValue.dwPLC2WSCommand;
		g_ItemCS.unlock();

		/*HWND hBiaoDingDlg = ::FindWindow(NULL, _T("标定"));

		if (tmpCommand == 2 || tmpCommand == 3)
		{
		}
		else if (tmpCommand == 1 && hBiaoDingDlg != NULL)
		{
		}
		else
		{
			return FALSE;
		}*/

	}
	bRet = IsYunXuFlaw(g_MonitorFlaw[nCardNum]);
//	bRet = true;//chenyuzhu
	g_ItemCS.lock();
	BYTE tmpCommand = g_ItemValue.dwPLC2WSCommand;
	g_ItemCS.unlock();
	if (!bRet && g_IniParams.m_bNewGuoLvEn && tmpCommand == 1)
	{
		return FALSE;
	}

	// 若内探第一次报伤，计算钢板边缘落下的探头盒编号
	if (!m_bNei1DownStartCalc && 
		(g_MonitorFlaw[nCardNum].CH_NO >= 3000 && g_MonitorFlaw[nCardNum].CH_NO < 4000))
	{
		GetNeitanBoxStartNum2(g_NeiTanBoxStartNum);
		m_bNei1DownStartCalc = TRUE;
	}

	if (!m_bNei2DownStartCalc && 
		(g_MonitorFlaw[nCardNum].CH_NO >= 4000 && g_MonitorFlaw[nCardNum].CH_NO < 5000))
	{
		GetNeitanBoxStartNum2(g_NeiTanBoxStartNum);
		m_bNei2DownStartCalc = TRUE;
	}

	if (!m_bNei3DownStartCalc && 
		(g_MonitorFlaw[nCardNum].CH_NO >= 5000 && g_MonitorFlaw[nCardNum].CH_NO < 6000))
	{
		GetNeitanBoxStartNum2(g_NeiTanBoxStartNum);
		m_bNei3DownStartCalc = TRUE;
	}

	if (!m_bNei4DownStartCalc && 
		(g_MonitorFlaw[nCardNum].CH_NO >= 6000 && g_MonitorFlaw[nCardNum].CH_NO < 7000))
	{
		GetNeitanBoxStartNum2(g_NeiTanBoxStartNum);
		m_bNei4DownStartCalc = TRUE;
	}

	WORD nLogicNum = g_MonitorFlaw[nCardNum].CH_NO;//g_pFlawData[Count-1].CH_NO;


	// 计算伤显示的对应方格数
	// (在参考边逻辑通道号<非参考边<内探1<内探2<内探3<内探4 情况下有效)
	WORD nDisplayIndex = 0;
	// 参考边
	if (nLogicNum >= m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan1LogicBegin && nLogicNum < m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan2LogicBegin)
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan1WuliBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan1LogicBegin);
	}
	// 非参考边
	else if (nLogicNum < m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan1LogicBegin)
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan2WuliBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nBianTan2LogicBegin);
	}
	// 内探1
	else if (nLogicNum < m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan2LogicBegin)
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan1WuliBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan1LogicBegin);
	}
	// 内探2
	else if (nLogicNum < m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan3LogicBegin)
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan2WuliBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan2LogicBegin);
	}
	// 内探3
	else if (nLogicNum < m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan4LogicBegin)
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan3WuLiBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan3LogicBegin);
	}
	// 内探4
	else if (nLogicNum < (m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan4LogicBegin + m_pDockWidgetChannelCtr->m_pmyWidget->m_nVirtualNeitan4Number))
	{
		nDisplayIndex = m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan4WuLiBegin + (nLogicNum - m_pDockWidgetChannelCtr->m_pmyWidget->m_nNeiTan4LogicBegin);
	}
	else
	{
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//chenyuzhu modify start on 2014-10-22
	QString strNewLine;
	if (g_MonitorFlaw[nCardNum].DETECT_STATE == 0)
	{
	//	channel[nDisplayIndex].enable = 1;
 		strNewLine = QString("%1%2%3").arg(QObject::tr("通道号:")).arg(nLogicNum).arg(QObject::tr(", 失波报警\r\n"));
	} 
	else if (g_MonitorFlaw[nCardNum].DETECT_STATE == 1)
	{
	//	channel[nDisplayIndex].enable = 2;
	//	strNewLine.sprintf("%s%d%s%.2f%s%.2f%s%.2f%s",tr("通道号:"),nLogicNum,tr(", 有伤报警, X:"),x_distance,tr(" mm, Y:"),y_distance,tr(" mm, Z:"),Dot2Range(VSOUND, (float)g_MonitorFlaw[nCardNum].CH_FLAW_TIME),tr("f mm\r\n"));
		strNewLine = QString("%1%2%3%4%5%6%7").arg(QObject::tr("通道号:")).arg(nLogicNum).arg(QObject::tr(", 有伤报警, X:")).arg(x_distance).arg(tr(" mm, Z:")).arg(Dot2Range(VSOUND, (float)g_MonitorFlaw[nCardNum].CH_FLAW_TIME)).arg(tr("f mm\r\n"));//chenyuzhu modify on 2014-11-4
	//	strNewLine.Format(_T("通道号:%d, 有伤报警, X:%.2f mm, Y:%.2f mm, Z:%.2f mm\r\n"), nLogicNum, x_distance,y_distance,Dot2Range(VSOUND, (float)g_MonitorFlaw[nCardNum].CH_FLAW_TIME));
	}
	else if (g_MonitorFlaw[nCardNum].DETECT_STATE == 2)
	{
//		strNewLine.sprintf("%s%d%s%.2f%s%.2f%s%.2f%s",tr("通道号:"),nLogicNum,tr(", 厚度偏差:"),Dot2Range(VSOUND, (float)g_MonitorFlaw[nCardNum].CH_HEIGHT),tr(" mm\r\n"));
//		strNewLine.Format(_T("通道号:%d, 厚度偏差: %.2f mm\r\n"), nLogicNum, Dot2Range(VSOUND, (float)g_MonitorFlaw[nCardNum].CH_HEIGHT));
	}
	else if (g_MonitorFlaw[nCardNum].DETECT_STATE == 3)
	{
        strNewLine = QString("%1%2%3").arg(QObject::tr("通道号:")).arg(nLogicNum).arg(QObject::tr(", 耦合太厚\r\n"));
//		strNewLine.Format(_T("通道号:%d, 耦合太厚\r\n"), nLogicNum);
	}
	else if (g_MonitorFlaw[nCardNum].DETECT_STATE == 4)
	{
		strNewLine = QString("%1%2%3").arg(QObject::tr("通道号:")).arg(nLogicNum).arg(QObject::tr(", 耦合太薄\r\n"));
//		strNewLine.Format(_T("通道号:%d, 耦合太薄\r\n"), nLogicNum);
	}
	else
	{
	}
	//创建探测日志文件
	QString curPath = QDir::currentPath() + QString("\\RunningDetect.txt");
	//chenyuzhu modify end on 2014-10-22
	QFile RDFile(curPath);
	RDFile.open(QIODevice::Append);
	QTextStream txtOutput(&RDFile);  
	txtOutput << strNewLine << endl;  
	RDFile.close();
	// 向PlateGeneralDlg 发送伤点信息
	CH_FLAWDATA *tempData = new CH_FLAWDATA;
	*tempData = g_MonitorFlaw[nCardNum];
	emit signalAddFlawData(tempData);
	return TRUE;
}

void MainWindow::AScanSave()
{
	//根据权限设置按钮功能
	if ( QString("管理员") != g_strUserPer)
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("当前用户无法执行此操作"));
		return;
	}
	//保存参数验证
	CInspectUser *pInspectUser = new CInspectUser(m_pDatabaseConnection);
	pInspectUser->exec();
	if (pInspectUser->m_isOK)
	{
		//chenyuzhu add start on 2014-10-9 
		delete pInspectUser;
		pInspectUser = NULL;
		//chenyuzhu add end on 2014-10-9 
		/*************************************需要********************************************/
		g_ItemCS.lock();
		BYTE tmpCommand = g_ItemValue.dwPLC2WSCommand;
		g_ItemCS.unlock();

		float fICIV  = Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D + 
			allCH_CONFIG[g_dwActiveChannel].CH_GAIN_DIFF);	
		fICIV = (fICIV - 4.2) / g_GainSale[g_dwActiveChannel];
		float fTCIV  = Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D);
		fTCIV = (fTCIV - 4.2) / g_GainSale[g_dwActiveChannel];
		//chenyuzhu modify start on 2014-8-9
		//	float fDelay = ((float)allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT)*g_nSpeed/(2.0*g_nADCFreq);
		//float fDelay = Dot2Range(VSOUND,allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT);
		float fDelay = (float)allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT / ((float)g_nADCFreq * 0.000001); //chenyuzhu add on 2014-10-10

		//	float fHuaMenQianYan = ((float)allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME)*g_nSpeed/(2.0*g_nADCFreq);
		float fHuaMenQianYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	
		//	float fHuaMenHouYan = ((float)allCH_CONFIG[g_dwActiveChannel].CH_REARTIME)*g_nSpeed/(2.0*g_nADCFreq);
		float fHuaMenHouYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME);
	
		//	float fDiBoHuaMenHouYan = ((float)allCH_CONFIG[g_dwActiveChannel].CH_MAXBACKTIME)*g_nSpeed/(2.0*g_nADCFreq);
		float fDiBoHuaMenHouYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_MAXBACKTIME);
		//chenyuzhu modify end on 2014-8-9
		float fDiBoYuZhi = ((float)allCH_CONFIG[g_dwActiveChannel].CH_BEMINDATA)*100.0/(pow(2.0f,g_nADCaiYangShu-1)-1);
		float fYiZhi     = ((float)allCH_CONFIG[g_dwActiveChannel].CH_REJECT_VALUE)*100.0/(pow(2.0f,g_nADCaiYangShu-1)-1);

		QString strGongYi = g_strGongYi;
		strGongYi.trimmed();
		QString strIsOpen = allCH_CONFIG[g_dwActiveChannel].CH_EN?tr("开"):tr("关");
	
		//chenyuzhu add start on 2014-7-7
		
		//1、读取数据库获得SheBeiFile.dbfl文件

		QString strFileName = QDir::currentPath()+ "/SheBeiFile.dbfl";
		QFile file(strFileName);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
			return ;
		}
		SHEBEICANSHU SheBeiCanShu;
		
		file.seek(0);
	
		quint64 nBytesRead = file.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));//读文件到结构体中
		file.close();

		SheBeiCanShu.ChannelParam[g_dwActiveChannel].nZhongZhiDian = allCH_CONFIG[g_dwActiveChannel].MIDDLE_VALUE;
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fZengyixishu = g_GainSale[g_dwActiveChannel];
	//	strIsOpen = "asd";

		//QByteArray ba = strIsOpen.toLatin1();
		QByteArray ba = strIsOpen.toLocal8Bit();
		strcpy_s(SheBeiCanShu.ChannelParam[g_dwActiveChannel].strIsOpen, ba.data());
	//	strcpy_s(SheBeiCanShu.ChannelParam[g_dwActiveChannel].strIsOpen, strIsOpen.toStdString().c_str());
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fICIV = fICIV;
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fTCIV = fTCIV;
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fDelay = fDelay;
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fShangmangqu = fHuaMenQianYan; //闸门前沿
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fXiamangqu = g_Info[nPlate].height - fHuaMenHouYan; //厚度-闸门后沿
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fDibozhamenkuandu = fDiBoHuaMenHouYan -fHuaMenHouYan;//- g_Info[nPlate].height+;//厚度-下盲区+闸门2的宽度
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fDiBoYuZhi = fDiBoYuZhi;
		SheBeiCanShu.ChannelParam[g_dwActiveChannel].fYiZhi = fYiZhi;		
	//	file.clear();
		//chenyuzhu add end on 2014-7-7
		
		//chenyuzhu modify start on 2014-6-20 注释掉原来的 将里面的条件变量g_dwActiveChannel改为逻辑通道g_dwActiveChannelLuoJi
		//QString strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25").arg("update t_Channel set ZhongZhiDian='").arg(allCH_CONFIG[g_dwActiveChannel].MIDDLE_VALUE).arg("',IsOpen='").arg(strIsOpen).arg("',ICIV='").arg(fICIV).arg("',TCIV='").arg(fTCIV).arg("',Delay='").arg(fDelay).arg("',HuaMenQianYan='").arg(fHuaMenQianYan).arg("',HuaMenHouYan='").arg(g_Info[nPlate].height - fHuaMenHouYan).arg("',DiBoHuaMenHouYan='").arg(fDiBoHuaMenHouYan - g_Info[nPlate].height).arg("',DiBoYuZhi='").arg(fDiBoYuZhi).arg("',YiZhi='").arg(fYiZhi).arg("' where RefNum = '").arg(g_dwActiveChannel).arg("' and TanShangGongYi = '").arg(strGongYi).arg("'");
		//	QString strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25").arg("update t_Channel set ZhongZhiDian='").arg(allCH_CONFIG[g_dwActiveChannel].MIDDLE_VALUE).arg("',IsOpen='").arg(strIsOpen).arg("',ICIV='").arg(fICIV).arg("',TCIV='").arg(fTCIV).arg("',Delay='").arg(fDelay).arg("',HuaMenQianYan='").arg(fHuaMenQianYan).arg("',HuaMenHouYan='").arg(g_Info[nPlate].height - fHuaMenHouYan).arg("',DiBoHuaMenHouYan='").arg(fDiBoHuaMenHouYan - g_Info[nPlate].height).arg("',DiBoYuZhi='").arg(fDiBoYuZhi).arg("',YiZhi='").arg(fYiZhi).arg("' where RefNum = '").arg(g_dwActiveChannelLuoJi).arg("' and TanShangGongYi = '").arg(strGongYi).arg("'");
		//chenyuzhu modify end on 2014-6-20

		float fZengyiCha = fICIV - fTCIV;
		if (fZengyiCha > 17)
		{
			QString str = QString("%1%2").arg(tr(" 通道伤波底波增益差大于17，保存失败！")).arg(g_dwActiveChannel);
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return;
		}

		//chenyuzhu add start on 2014-7-16
		if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
			return ;
		}
		quint64 nBytesWrite = file.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));

		file.close();
		//chenyuzhu add end on 2014-7-16
		//g_sqlDB.ExecuteSQL(strSQL);
		/*	QSqlQuery query(*g_sqlDB);
		query.exec(strSQL);
		if (!query.isActive())
		{
			QString strError = query.lastError().text();
			QMessageBox::information(this,"Error",tr("SQL语句执行失败,%1").arg(strError));
		}*/

		// 修改缓存备份
		//::memcpy(&pDoc->allCH_CONFIG_BackUp[m_nWuLiNum], &allCH_CONFIG[m_nWuLiNum], sizeof(CH_CONFIG));
//		QMessageBox::information(NULL, "Info", tr("保存到文件成功"), QMessageBox::Yes, QMessageBox::Yes);//chenyuzhu modify on 2014-7-16
		/*************************************需要********************************************/
    }
}

void MainWindow::OnALLChannelBiaoDing()
{
    //chenyuzhu add start on 2014-10-9
    //chenyuzhu add end on 2014-10-9
    /*************************************需要********************************************/
    g_ItemCS.lock();
    BYTE tmpCommand = g_ItemValue.dwPLC2WSCommand;
    g_ItemCS.unlock();

    float fICIV  = Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D +
        allCH_CONFIG[g_dwActiveChannel].CH_GAIN_DIFF);
	fICIV = (fICIV - 4.2) / g_GainSale[g_dwActiveChannel];
    float fTCIV  = Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D);
	fTCIV = (fTCIV - 4.2) / g_GainSale[g_dwActiveChannel];
    //chenyuzhu modify start on 2014-8-9
    float fDelay = (float)allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT / ((float)g_nADCFreq * 0.000001); //chenyuzhu add on 2014-10-10
    float fHuaMenQianYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
    float fHuaMenHouYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME);
    float fDiBoHuaMenHouYan = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_MAXBACKTIME);
    //chenyuzhu modify end on 2014-8-9
    float fDiBoYuZhi = ((float)allCH_CONFIG[g_dwActiveChannel].CH_BEMINDATA)*100.0/(pow(2.0f,g_nADCaiYangShu-1)-1);
    float fYiZhi     = ((float)allCH_CONFIG[g_dwActiveChannel].CH_REJECT_VALUE)*100.0/(pow(2.0f,g_nADCaiYangShu-1)-1);

    QString strGongYi = g_strGongYi;
    strGongYi.trimmed();
    QString strIsOpen = allCH_CONFIG[g_dwActiveChannel].CH_EN?tr("开"):tr("关");

    //chenyuzhu add start on 2014-7-7

    //1、读取数据库获得SheBeiFile.dbfl文件

    QString strFileName = QDir::currentPath()+ "/SheBeiFile.dbfl";
    QFile file(strFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
        return ;
    }
    SHEBEICANSHU SheBeiCanShu;

    file.seek(0);

    quint64 nBytesRead = file.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));//读文件到结构体中
    file.close();
    for (UINT index = 0; index < MAX_CHANNEL_NO; index++)
    {
        float fZengyiCha = fICIV - fTCIV;
        if (fZengyiCha > 17)
        {
            continue;
        }
        SheBeiCanShu.ChannelParam[index].nZhongZhiDian = allCH_CONFIG[g_dwActiveChannel].MIDDLE_VALUE;
        QByteArray ba = strIsOpen.toLocal8Bit();
        strcpy_s(SheBeiCanShu.ChannelParam[index].strIsOpen, ba.data());
        SheBeiCanShu.ChannelParam[index].fICIV = fICIV;
        SheBeiCanShu.ChannelParam[index].fTCIV = fTCIV;
        SheBeiCanShu.ChannelParam[index].fDelay = fDelay;
        SheBeiCanShu.ChannelParam[index].fShangmangqu = fHuaMenQianYan; //闸门前沿
        SheBeiCanShu.ChannelParam[index].fXiamangqu = g_Info[nPlate].height - fHuaMenHouYan; //厚度-闸门后沿
        SheBeiCanShu.ChannelParam[index].fDibozhamenkuandu = fDiBoHuaMenHouYan -fHuaMenHouYan;//- g_Info[nPlate].height+;//厚度-下盲区+闸门2的宽度
        SheBeiCanShu.ChannelParam[index].fDiBoYuZhi = fDiBoYuZhi;
        SheBeiCanShu.ChannelParam[index].fYiZhi = fYiZhi;

		if ((index > 2 && index < 8) || (index > 10 && index < 16))
		{
			strcpy_s(SheBeiCanShu.ChannelParam[index].strIsOpen, "关");
		}
    }

    //chenyuzhu add start on 2014-7-16
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
        return ;
    }
    quint64 nBytesWrite = file.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));

    file.close();
	INIT_CONFIG(allCH_CONFIG, &ch_config_global); 
}

//根据窗口大小确定工具栏
void MainWindow::resizeEvent(QResizeEvent * event)
{

	m_pMainToolBar->setIconSize(QSize(this->width()/35, 48));
	m_pColorType->setMinimumSize(QSize(this->width()/12, 25));
	m_pColorLabel ->setMinimumSize(QSize(this->width()/15 *7,25));
	m_pColorLabel ->setMaximumSize(QSize(this->width()/15 *7,50));
	DrawColorLabel(); // 完成3种比色表的绘制
}

//响应双击TableView的发出的信号 更新界面信息
void MainWindow::OnUpdateChannelDock(QList<QString> argList)
{
	m_pPlateInfo->m_strArtName = argList[7];
	m_pPlateInfo->m_strDetectStandard = argList[1];
 	m_pPlateInfo->m_strBodyLevel = argList[2];
	m_pPlateInfo->m_strSteelNumber = argList[0];

	m_pPlateInfo->m_nPlateWidth = argList[5].toFloat();
	m_pPlateInfo->m_nPlateLength = argList[4].toFloat();
	m_pPlateInfo->m_nPlateThickness = argList[6].toFloat();
	UpdateDockWidgetParameterDisp();
}
void MainWindow::InitOthers()      //初始化组件对象和其他参数
{
	g_IniParams.m_pMainWindow = this; 

#if HAVE_DOG
	//1 加密狗验证
	m_ElepHandle = 0;
    unsigned long RetVal = ElepOpenFirst(0,0,0,0,0,0,&m_ElepHandle);   	// 打开加密狗
	
    if(RetVal == ELEP_SUCCESS)
	{
	}
	else
	{
		QMessageBox::about(this,"Error",QObject::tr("未授权,程序自动关闭!"));
		return;
    }
#endif

	//2 线程参数初始化
	::ZeroMemory(m_stThreadParam, sizeof(AECTHREAD_PARAM)*MAX_CARD_NO);
	for (int i=0; i<MAX_CARD_NO; i++)
	{
		m_stThreadParam[i].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
	}
	//3 PLX初始化
#if _PLX
#if HAVE_PLX
    BOOL bRet = m_PLXCommunication.Init(m_stThreadParam);
	if (!bRet)
	{
		exit(0);
    }
#endif // HAVE_PLX
#endif // _PLX

	// 4读加密狗内的配置文件
    BOOL bRetPara = g_IniParams.ReadIniData();
	if (!bRetPara)
	{	
		QMessageBox::information(this,"AddUser::OnOK",tr("加密狗读取失败")); //chenyuzhu add on 2014.5.4
		exit(0);
    }

	// 5初始化数据库
#if HAVE_DATA_BASE
	if (!InitDatabase())
	{
		return;
	}
#endif

	//6创建OPC对象
#if HAVE_OPC
	hOPCInitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_OPCComponent.StartThread();
	IUnknown* pIServer = NULL;
	m_OPCComponent.CreateComponent(clsOPCServer, IID_IOPCServer, &pIServer);
    DWORD ret = ::WaitForSingleObject(hOPCInitEvent,INFINITE);    //CreateComponentOnthread中置信号状态
	ResetEvent(hOPCInitEvent);
	pIServer->Release();
	::Sleep(10);
#endif

#if HAVE_PLX
	//7 使能打开编码器
    ControlCodec(m_stThreadParam[0].hDev,CODEC_1_END);
#endif

	//8 初始化失波数据链表
	for (int i=0; i<MAX_CHANNEL_NO; i++)
	{
		g_LostWaveDatas[i] = new QList<CH_FLAWDATA>;
	}	

	//9创建卡线程
	m_nAECCardNum = m_PLXCommunication.GetAECCardNum();
	g_plateEndFlag = (1<<m_nAECCardNum) - 1;                   // dwCardCount-1 --->g_nAECCardNum

	m_pAECWorkThread = new AECWorkThread*[m_nAECCardNum];      //线程对象数组
	for (int i=0; i<m_nAECCardNum; i++)
	{
		m_pAECWorkThread[i] = NULL;
	}  
	m_bPlateStart = new BOOL[m_nAECCardNum];                    //开始标志数组
	m_bPlateEnd = new BOOL[m_nAECCardNum];						//结束标志数组
	::ZeroMemory(m_bPlateStart, sizeof(BOOL) * m_nAECCardNum);
	::ZeroMemory(m_bPlateEnd, sizeof(BOOL) * m_nAECCardNum);
}

void MainWindow::InitOtherParameters()
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QSettings setting_fileName("./OtherParameters.ini", QSettings::IniFormat);
	setting_fileName.setIniCodec(codec);
	setting_fileName.beginGroup("");
	QString fileName = setting_fileName.value("ProjectFileName").toString();
	setting_fileName.endGroup();

	if (fileName == "") {
		QMessageBox::information(NULL, tr("MainWindow::InitOtherParameters"), tr("OtherParameters.ini 文件读取失败"));
		return;
	}

	QSettings setting(QString("./%1").arg(fileName), QSettings::IniFormat);
	setting.setIniCodec(codec);
	setting.beginGroup("");
	g_OtherParameters.edgeCar1BoxCount = setting.value("edgeCar1BoxCount").toInt();
	g_OtherParameters.edgeCar2BoxCount = setting.value("edgeCar2BoxCount").toInt();
	g_OtherParameters.insideCar1BoxCount = setting.value("insideCar1BoxCount").toInt();
	g_OtherParameters.insideCar2BoxCount = setting.value("insideCar2BoxCount").toInt();
	g_OtherParameters.edgeCarEachBoxChannelCount = setting.value("edgeCarEachBoxChannelCount").toInt();
	g_OtherParameters.insideCarEachBoxChannelCount = setting.value("insideCarEachBoxChannelCount").toInt();
	g_OtherParameters.edgeCar1PhysicalChannelStart = setting.value("edgeCar1PhysicalChannelStart").toInt();
	g_OtherParameters.edgeCar2PhysicalChannelStart = setting.value("edgeCar2PhysicalChannelStart").toInt();
	g_OtherParameters.insideCar1PhysicalChannelStart = setting.value("insideCar1PhysicalChannelStart").toInt();
	g_OtherParameters.insideCar2PhysicalChannelStart = setting.value("insideCar2PhysicalChannelStart").toInt();
	g_OtherParameters.edgeCar1LogicalChannelStart = setting.value("edgeCar1LogicalChannelStart").toInt();
	g_OtherParameters.edgeCar2LogicalChannelStart = setting.value("edgeCar2LogicalChannelStart").toInt();
	g_OtherParameters.insideCar1LogicalChannelStart = setting.value("insideCar1LogicalChannelStart").toInt();
	g_OtherParameters.insideCar2LogicalChannelStart = setting.value("insideCar2LogicalChannelStart").toInt();
	g_OtherParameters.chineseReportTitle = setting.value("chineseReportTitle").toString();
	g_OtherParameters.englishReportTitle = setting.value("englishReportTitle").toString();
	setting.endGroup();

	QSettings setting_GuoLvQueXian("./GuoLvQueXian.ini", QSettings::IniFormat);
	setting_GuoLvQueXian.setIniCodec(codec);
	setting_GuoLvQueXian.beginGroup("");
	g_GuoLvQueXian.IsCompareX1 = setting_GuoLvQueXian.value("IsCompareX1").toBool();
	g_GuoLvQueXian.IsCompareX2 = setting_GuoLvQueXian.value("IsCompareX2").toBool();
	g_GuoLvQueXian.IsCompareY1 = setting_GuoLvQueXian.value("IsCompareY1").toBool();
	g_GuoLvQueXian.IsCompareY2 = setting_GuoLvQueXian.value("IsCompareY2").toBool();
	g_GuoLvQueXian.DifferenceX1 = setting_GuoLvQueXian.value("DifferenceX1").toDouble();
	g_GuoLvQueXian.DifferenceX2 = setting_GuoLvQueXian.value("DifferenceX2").toDouble();
	g_GuoLvQueXian.DifferenceY1 = setting_GuoLvQueXian.value("DifferenceY1").toDouble();
	g_GuoLvQueXian.DifferenceY2 = setting_GuoLvQueXian.value("DifferenceY2").toDouble();
	g_GuoLvQueXian.PointStartX1 = setting_GuoLvQueXian.value("PointStartX1").toDouble();
	g_GuoLvQueXian.PointStartY1 = setting_GuoLvQueXian.value("PointStartY1").toDouble();
	g_GuoLvQueXian.PointEndX1 = setting_GuoLvQueXian.value("PointEndX1").toDouble();
	g_GuoLvQueXian.PointEndY1 = setting_GuoLvQueXian.value("PointEndY1").toDouble();
	g_GuoLvQueXian.PointStartX2 = setting_GuoLvQueXian.value("PointStartX2").toDouble();
	g_GuoLvQueXian.PointStartY2 = setting_GuoLvQueXian.value("PointStartY2").toDouble();
	g_GuoLvQueXian.PointEndX2 = setting_GuoLvQueXian.value("PointEndX2").toDouble();
	g_GuoLvQueXian.PointEndY2 = setting_GuoLvQueXian.value("PointEndY2").toDouble();
	g_GuoLvQueXian.StandardLength = setting_GuoLvQueXian.value("StandardLength").toDouble();
	g_GuoLvQueXian.StandardWidth = setting_GuoLvQueXian.value("StandardWidth").toDouble();
	g_GuoLvQueXian.StandardHeight = setting_GuoLvQueXian.value("StandardHeight").toDouble();
	g_GuoLvQueXian.StandardOffsetLength = setting_GuoLvQueXian.value("StandardOffsetLength").toDouble();
	g_GuoLvQueXian.StandardOffsetHeight = setting_GuoLvQueXian.value("StandardOffsetHeight").toDouble();
	setting_GuoLvQueXian.endGroup();
}

void MainWindow::OnSetPlateID()   //重设钢板号
{
	//读取配置文件信息判断用哪种方式配置参数
	int value = m_pSettings->value("PlateInfo/InputMode","").toInt();
	if(1 == value)
	{
		//钢板参数对话框
part1:
		m_pERPDialog->exec();
		if (m_pERPDialog->m_bIsSwitch)
		{
			m_pERPDialog->m_bIsSwitch = false;
			m_pHandleDialog->exec();
			if (m_pHandleDialog->m_bIsSwitch)  //点切换
			{
				m_pERPDialog->m_bIsSwitch = false;
				goto part1;
			}
			if (!m_pHandleDialog->m_bIsOK)     //点取消
			{
			}
			else                              //点确定
			{
				UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
				m_pDockWidgetParameterDisp->m_prbMInput->click();
			}			
		}
		if (!m_pERPDialog->m_bIsOK)
		{
		}
		UpdateDockWidgetParameterDisp();	//更新到形似钢板参数显示界面
		m_pDockWidgetParameterDisp->m_prbERP->click();
	}
	else
	{	
part2:
		m_pHandleDialog->exec();
		if (m_pHandleDialog->m_bIsSwitch)
		{
			m_pERPDialog->m_bIsSwitch = false;
			m_pERPDialog->exec();
			if (m_pERPDialog->m_bIsSwitch)  //点切换
			{
				m_pHandleDialog->m_bIsSwitch = false;
				goto part2;
			}
			if (!m_pERPDialog->m_bIsOK)     //点取消
			{
				//exit(0);
			}
			else                              //点确定
			{
				UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
				m_pDockWidgetParameterDisp->m_prbERP->click();
			}			
		}
		if (!m_pHandleDialog->m_bIsOK)
		{
		}
		else
		{
			UpdateDockWidgetParameterDisp();  //更新到形似钢板参数显示界面
			m_pDockWidgetParameterDisp->m_prbMInput->click();
		}
	}
    if (0 == m_FlagMap["MPlateParaFlag"])
	{
		m_pIsReset->setStyleSheet("color:rgb(255, 0, 0)");
		m_pIsReset->setText(tr("未设置钢板参数"));
	}
	else
	{
		m_pIsReset->setStyleSheet("color:rgb(255, 255, 255)");
		m_pIsReset->setText(tr("准备就绪"));
	}
	DrawColorLabel();//chenyuzhu add on 014-11-5
}


void MainWindow::PrepareStartDetect() //一张钢板探完后，重新配置钢板参数，提交OPC
{
	OnSetPlateID();//重新配置钢板参数
	//提交钢板参数到OPC,并且提交Ready信号
	if (0 == m_FlagMap["MPlateParaFlag"]) //未设置钢板参数
	{

	}
	else                                  //设置钢板参数,提交OPC，写入Read信号
	{
		//给数据库添加数据
		//在参数配置中点OK按钮会改变g_Info[nPlate]，g_strBiaoZhun，g_strJiBie
		//chengyuzhu modify start on 2014-7-9
		/*
		QSqlQuery query(*g_sqlDB);
		QString strSQL = QString("%1%2%3").arg("select * from t_Plate where name = '").arg(g_Info[nPlate].plateno).arg("'");
		query.exec(strSQL);
		query.next();*/
		float fTempWeight = (g_Info[nPlate].length/1000)*(g_Info[nPlate].width/1000)*(g_Info[nPlate].height/1000)*7.85;
		QString fWeight;
		fWeight.sprintf("%.2f",fTempWeight);
		fTempWeight = fWeight.toFloat();
		QString strPlateType;  //保存与与探伤工艺相对应的钢板类型（PlateType）

		//chenyuzhu add start on 2014-7-9
		//读取数据库读出platefile.dbfl文件（未添加）

		QString strFileName = QDir::currentPath()+ "/platefile.dbfl";
		QFile file(strFileName);
		//if (!file.open(QIODevice::ReadOnly))
		if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			QMessageBox::warning(NULL,"AScanSave",QObject::tr("platefile.dbfl打开失败"));
			return ;
		}
		PLATEFILE_PARAM PlateParam;

		file.seek(0);

		quint64 nBytesRead = file.read((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
		file.close();

		strPlateType = g_Info[nPlate].plateno;
		QByteArray ba = strPlateType.toLocal8Bit();
		strcpy_s(PlateParam.strPlateType, ba.data());
		strcpy_s(PlateParam.strName, ba.data());
		PlateParam.fLength = g_Info[nPlate].length;
		PlateParam.fWidth = g_Info[nPlate].width;
		PlateParam.fThick = g_Info[nPlate].fReportHeight;
		PlateParam.fLWaveSpeed = 5970;
		PlateParam.fTWaveSpeed = 3000;
		PlateParam.fWeight = fTempWeight;
		ba = m_pPlateInfo->m_strFurnNumber.toLocal8Bit();
		strcpy_s(PlateParam.strLu, ba.data());
		ba = g_Info[nPlate].plateno.toLocal8Bit();
		strcpy_s(PlateParam.strShenChan, ba.data());
		ba = m_pPlateInfo->m_strContractNum.toLocal8Bit();
		strcpy_s(PlateParam.strHeTong, ba.data());
		ba = g_strJiBie.toLocal8Bit();
		strcpy_s(PlateParam.strGangJi, ba.data());
		ba = m_pPlateInfo->m_strSteelGrade.toLocal8Bit();
		strcpy_s(PlateParam.strGangZhong, ba.data());
		PlateParam.nflag = m_pPlateInfo->flag;

		ba = QString().toLocal8Bit();
		strcpy_s(PlateParam.strMuBan, ba.data());



		if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			QMessageBox::warning(NULL,"AScanSave",QObject::tr("platefile.dbfl打开失败"));
			return ;
		}
		file.seek(0);
		quint64 nBytesWrite = file.write((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
		file.close();
		//chenyuzhu add end on 2014-7-9

		
/*
		if (query.value(0).isNull())  //数据库中没有当前钢板号
		{

			//查询数据库中有没有当前的工艺
			QString tempGongYi = g_strGongYi.trimmed();
			QString strSQl2 = QString("%1%2%3")
				.arg("select * from t_TanShangGongYi where name = '")
				.arg(tempGongYi).arg("'");
			query.exec(strSQl2);
			query.next();
			if (query.value(0).isNull())//如果不存在相应的探伤工艺
			{ 
				//应该重新配置钢板参数，现在未实现
				QMessageBox::information(NULL,tr(""),tr("工艺对应钢种无记录！"));
				return;
			}
			else //从表中取出PlateType并保存到变量中
			{
				strPlateType = query.value(2).toString();
			}
			//插入一条数据
			strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29")
				.arg(tr("insert into t_Plate(PlateType, name, Length, Width, Thick, LWaveSpeed, TWaveSpeed, Weight, Lu, ShenChan, HeTong, GangJi, GangZhong, flag) \values('"))
				.arg(strPlateType).arg("','").arg(g_Info[nPlate].plateno).arg("','").arg(g_Info[nPlate].length).arg("','").arg(g_Info[nPlate].width)
			    .arg("','").arg(g_Info[nPlate].fReportHeight).arg("','").arg(5970).arg("','").arg(3000).arg("','").arg(fWeight)
				.arg("','").arg(m_pPlateInfo->m_strFurnNumber).arg("','").arg(g_Info[nPlate].plateno).arg("','").arg(m_pPlateInfo->m_strContractNum).arg("','")
				.arg(g_strJiBie).arg("','").arg(m_pPlateInfo->m_strSteelGrade).arg("','").arg(m_pPlateInfo->flag).arg("')");
		}
		else                       //数据库中有当前钢板号
		{ 
			QString temp1;
			QString temp2;
			QString temp3;
			QString temp4;
			QString temp5;
			QString temp6;
			strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32%33%34%35%36").arg("update t_Plate set Length = '").arg(temp1.sprintf("%.2f",g_Info[nPlate].length)).arg("', ")
				.arg("Width = '").arg(temp2.sprintf("%.2f",g_Info[nPlate].width)).arg("', ")
				.arg("Thick = '").arg(temp3.sprintf("%.2f",g_Info[nPlate].fReportHeight)).arg("', ")
				.arg("LWaveSpeed = '").arg(5970).arg("', ")
				.arg("TWaveSpeed = '").arg(3000).arg("', ")
				.arg("Weight = '").arg(fWeight).arg("', ")
				.arg("Lu='").arg(m_pPlateInfo->m_strFurnNumber).arg("', ")
				.arg("HeTong='").arg(m_pPlateInfo->m_strContractNum).arg("', ")
				.arg("ShenChan='").arg(g_Info[nPlate].plateno).arg("', ")
				.arg("GangJi='").arg(m_pPlateInfo->m_strSteelGrade).arg("', ")
				.arg("flag='").arg(m_pPlateInfo->flag).arg("' ")
				.arg("where name = '").arg(g_Info[nPlate].plateno).arg("'");
				
		}
		query.exec(strSQL);
		//将当前更新或者保存的钢板信息的id保存下来，生成探伤流水表的时候要用到
		strSQL = QString("%1%2%3").arg("select id from t_Plate where name='").arg(g_Info[nPlate].plateno).arg("'");
		query.exec(strSQL);
		query.next();
		m_pCscanView->m_nPlateId = query.value(0).toInt();*/

		//chengyuzhu modify end on 2014-7-9
		//提交到OPC中去
		// 提交探伤工艺（包括钢板参数\探伤模式，通过opc server）	

#if HAVE_OPC
		OPCHANDLE *phServer;   
		VARIANT values[5];
		HRESULT *pErrors;
		HRESULT r1;
		LPWSTR ErrorStr;
		//通过server 句柄选择item（由AddItem 得到的）
		phServer = new OPCHANDLE[5];
		pErrors = new HRESULT[5];

		phServer[0] = m_OPCComponent.m_pItemResult[5].hServer;	// PlateID
		// 设置Variant 变量的数据类型和数值
		values[0].vt = VT_BSTR;
		values[0].bstrVal = SysAllocString(g_Info[nPlate].plateno.utf16());
		pErrors[0] = m_OPCComponent.m_pErrors[5];


		phServer[1] = m_OPCComponent.m_pItemResult[12].hServer;	// PlateTestResult
		// 设置Variant 变量的数据类型和数值
		values[1].vt = VT_UI1;
		values[1].bVal = 0;
		pErrors[1] = m_OPCComponent.m_pErrors[12];

		phServer[2] = m_OPCComponent.m_pItemResult[6].hServer;	// PlateLength
		values[2].vt = VT_UI4;
		values[2].uintVal = UINT(g_Info[nPlate].length*10);
		pErrors[2] = m_OPCComponent.m_pErrors[6];

		phServer[3] = m_OPCComponent.m_pItemResult[7].hServer;	// Width
		values[3].vt = VT_UI4;
		values[3].uintVal = UINT(g_Info[nPlate].width*10);
		pErrors[3] = m_OPCComponent.m_pErrors[7];

		phServer[4] = m_OPCComponent.m_pItemResult[8].hServer;	// height
		values[4].vt = VT_UI4;
		values[4].uintVal = UINT(g_Info[nPlate].height*10);
		pErrors[4] = m_OPCComponent.m_pErrors[8];
		bool bRet = OPCWrite(pErrors,phServer,values,5);
		delete[] phServer;
		delete[] pErrors;
		::SysFreeString(values[0].bstrVal);
		if (!bRet)   //OPCWrite函数写失败
		{
			QMessageBox::information(this,tr("MainWindow::InitWindow"),tr("向OPC提交钢板参数失败"));
			exit(0);
		}
		// OPC Server发系统Ready信号	
		OPCHANDLE *phServer2;
		VARIANT values2[1];
		HRESULT *pErrors2;

		phServer2 = new OPCHANDLE[1];
		pErrors2 = new HRESULT[1];
		phServer2[0] = m_OPCComponent.m_pItemResult[2].hServer;	// SysState	
		pErrors2[0] = m_OPCComponent.m_pErrors[2];
		values[0].vt = VT_UI1;
		values[0].bVal = 1;
		bRet = OPCWrite(pErrors2,phServer2,values,1);
		delete[] phServer2;
		delete[] pErrors2;
		if (!bRet)
		{
			QMessageBox::information(this,"MainWindow::InitWindow",tr("向OPC写Ready信号失败"));
			exit(0);
		} 
#endif
	}
}


//chenyuzhu add start on 2014.5.7
//测试模拟开始停止探测槽 用于调试
void MainWindow::OPCStartOrStop()
{
#if HAVE_PLX
    static int i = 0;
	if(0 == i) {
		i = 1;
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb2, 0xffff);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb4, 0xffff);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb6, 0xffff);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb8, 0xffff);
	} else {
		i = 0;
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb2, 0x0000);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb4, 0x0000);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb6, 0x0000);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb8, 0x0000);
    }

// 	emit signalPlateScanEnd();     //探伤结束,画C扫图打报表
//     testDrawImage();
#else
	QMessageBox::information(this, tr("MainWindow::OPCStartOrStop"), tr("未发现可操作板卡"));
#endif
}//调试 chenyuzhu on 2014-10-8	
//chenyuzhu add end on 2014.5.7 

//chenyuzhu add start on 2014.6.10
//删除伤点后重新计算和绘图的槽
void MainWindow::deleteReDraw()
{
	EndReadDetectFile();
}
//chenyuzhu add start on 2014.6.10

//chenyuzhu add start on 2014-7-23
//探伤开始工艺配置切换按键
//void MainWindow::GongYiQieHuan()
//{
//	g_bQieHuan = true;
//	m_pScanData->exec();
//	INIT_CONFIG(allCH_CONFIG, &ch_config_global);  //初始化通道信息//chenyuzhu add on 2014-7-20 查询钢板厚获取通道信息后 重新初始化通道
//	PrepareStartDetect();
//}
//chenyuzhu add end on 2014-7-23

void MainWindow::OnDataScanAction()
{
	if (m_pScanData->m_rbGongyiCheck->isChecked())
	{
		g_bQieHuan = true;
	}
	else
	{
		g_bQieHuan = false;
	}
	m_pScanData->exec();
	if (g_bQieHuan)
	{
		INIT_CONFIG(allCH_CONFIG, &ch_config_global);  //初始化通道信息//chenyuzhu add on 2014-7-20 查询钢板厚获取通道信息后 重新初始化通道
		PrepareStartDetect();
	}
}

//chenyuzhu add start on 2014-11-6
void MainWindow::OnBiaoDingAction()
{
	m_pBiaodingDlg->exec();
}
//chenyuzhu add end on 2014-11-6

//chenyuzh add start on 2014-11-6
void MainWindow::OnBiaodingCheck(int nCheck)//标定选择 
{
	g_pbBiaoding = nCheck;
}
//chenyuzh add end on 2014-11-6

void MainWindow::testDrawImage()
{
    EndReadDetectFile();
//    m_pCscanView->CalcResultData();
//    double length = g_Info[nPlate].length;
//    double width = g_Info[nPlate].width;
//    QRectF sourceRect = QRectF(0, 0, length, width);
//    QSizeF gridSize = QSizeF(5, 5);
//    ImageTransform transform(sourceRect, gridSize);
//    UTImage image1 = UTImage(transform.TargetRect());
//    UTImage image2 = UTImage(transform.TargetRect());
//    UTImage image3 = UTImage(transform.TargetRect());
//    UTImage image4 = UTImage(transform.TargetRect());

//    QString strPath = QDir::currentPath() += "/DianShang.txt";
//    QFile file(strPath);
//    if (!file.exists(strPath))
//    {
//        QMessageBox::information(NULL, "Error", tr("伤点文件不存在!!!"),  QMessageBox::Yes, QMessageBox::Yes);
//        return;
//    }

//    bool bRet = file.open(QIODevice::ReadOnly);
//    stDianShang tmp;
//    quint16 size = sizeof(stDianShang);
//    if (bRet)//文件打开正常
//    {
//        quint64 nFileSize = file.size();
//        while(nFileSize)
//        {
//            quint64 nBytesRead = file.read((char*)&tmp,size);
//            QPointF pixel(tmp.fX, tmp.fY);
//            QPoint point = transform.MapToGrid(pixel);
//            if (tmp.fX + 20 < g_IniParams.m_fNeiFrontLength ||
//            tmp.fX >(g_Info[nPlate].length - g_IniParams.m_fNeiTailLength))
//            {
//                if (transform.GridContains(point))
//                {
//                    image1(point.x() + 1, point.y()) = tmp.fShangBoFuZhi;
//                    image1(point.x() + 2, point.y()) = tmp.fShangBoFuZhi;
//                    image1(point.x() + 3, point.y()) = tmp.fShangBoFuZhi;
//                    image1(point) = tmp.fShangBoFuZhi;
//                    image2(point) = tmp.fZ;
//                    image3(point) = tmp.fShangBoFuZhi;
//                    image4(point) = tmp.fShangBoFuZhi;
//                }
//            }
//            else
//            {
//                if (transform.GridContains(point))
//                {
//                    image1(point) = tmp.fShangBoFuZhi;
//                    image1(point.x(), point.y() + 1) = tmp.fShangBoFuZhi;
//                    image1(point.x(), point.y() + 2) = tmp.fShangBoFuZhi;
//                    image1(point.x(), point.y() + 3) = tmp.fShangBoFuZhi;
//                    image2(point) = tmp.fZ;
//                    image3(point) = tmp.fShangBoFuZhi;
//                    image4(point) = tmp.fShangBoFuZhi;
//                }
//            }
//            nFileSize -= nBytesRead;
//            qDebug() << "nFileSize = " << nFileSize;
//        }
//        file.close();
//    }
//    else
//    {
//        QMessageBox::information(NULL, "Error", tr("伤点文件打开失败!!!"),  QMessageBox::Yes, QMessageBox::Yes);
//    }
//  //  UTImage::Boundaries boundaries = image1.FindBoundaries(30, UTImage::ThresholdMode::THRESHOLD_ABOVE);
//  //  int oo = boundaries.size();

//    UTImagePalette palette;
//  //  image1.ToImage(palette);
//    float minValue = 0;
//    float maxValue = 0;
//    QPoint minLocation;
//    QPoint maxLocation;
//    image1.GetMinMax(minValue, maxValue, minLocation, maxLocation);
//    float scale = 1.0 / (maxValue - minValue);

//    UTImage imagescale = UTImage(transform.TargetRect());
//    UTImage imageoffset = UTImage(transform.TargetRect());
//    for (int x = 0; x < transform.TargetRect().width(); x++)
//    {
//        for (int y = 0; y < transform.TargetRect().height(); y++)
//        {
//            imagescale(x, y) = scale;
//            imageoffset(x, y) = - minValue;
//        }
//    }
//    image1.Add(imageoffset);
//    image1.Multiply(imagescale);
//    image1.DebugPrint(tr("图形测试"));

//    image2.GetMinMax(minValue, maxValue, minLocation, maxLocation);
//    scale = 1.0 / (maxValue - minValue);

//    for (int x = 0; x < transform.TargetRect().width(); x++)
//    {
//        for (int y = 0; y < transform.TargetRect().height(); y++)
//        {
//            imagescale(x, y) = scale;
//            imageoffset(x, y) = - minValue;
//        }
//    }
//    image2.Add(imageoffset);
//    image2.Multiply(imagescale);
//    image2.DebugPrint(tr("图形测试1"));

    return;
}
