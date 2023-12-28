/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* C扫主界面
* Filename: CscanPanel.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-04-27
*/

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QTextEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QDir>
#include <QWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QSqlDatabase>
#include <QAxWidget>
#include <QMutex>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QTime>
#include <QMutex>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextOption>
#include <QPrintPreviewDialog>
#include <QVariant>
#include <QString>
#include <QJsonObject>
#include <QTextCodec>
#include "CscanPanel.h"
#include "DockForm.h"
#include "const.h"
#include "InspectUser.h"
#include "GlobalFunction.h"
#include "mainwindow.h"
#include "windows.h"
#include "BiaoZhun_GBT2970.h"
#include "BiaoZhun_ASTM435_435M.h"
#include "BiaoZhun.h"
#include "ToolsDialog.h"
#include "DataDefine.h"
#include "arsCore/ImageTransform.h"
#include "DataDefine.h"
#include "../../Source/WaferScan/Defect/PUTS/PUTSRecognizer.h"
#include "../../Source/WaferScan/Defect/PUTS/GBT_2970.h"
#include "../../Source/WaferScan/Defect/PUTS/ClassFactory.h"

//全局变量
QString g_strBiaoZhun;
QString g_strJiBie;
QString g_strProbeType;
bool g_bEnglish = false;
bool g_bJieguo = false;

//引用全局变量
extern QString g_strGongYi;
extern QString g_MaxCheckId;
extern QMutex  g_CS;
extern QAxWidget *g_pPlateDraw;
extern QMutex g_ItemMutex;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern bool g_bReadyDraw;
extern char *g_pBuf4;
extern unsigned int g_nBuf4Size;//点伤大小
extern GlobalItemValue g_ItemValue;
extern CIniParams g_IniParams;
extern QMutex g_ItemCS;
extern QSqlDatabase* g_sqlDB;
extern OtherParameters g_OtherParameters;
extern BOOL g_bStop;
extern BOOL g_bStart;

extern P9054_HANDLE hDev; //检测 UST PLC SERVER
extern DWORD g_dwFlawCount;


extern bool g_IfAtoPrint;	//判断自动打印是否勾选 chenyuzhu add on 2014.5.22
extern int g_nComboxColor;	//比色表选择下拉列表值chenyuzhu add on 2014-6-13

//chenyuzhu add start on 2014-6-16
extern bool *g_bInjPointFilter;
extern bool *g_bFilterBiger;
extern bool *g_bFilterSmaller;
extern int *g_nFilterBigerNum;
extern int *g_nFilterSmallerNum;
//chenyuzhu add end on 2014-6-16

extern NeiTanBoxStartNum g_NeiTanBoxStartNum;//chenyuzhu add on 2014-6-13
//extern QString g_LogonName;
extern CH_CONFIG allCH_CONFIG[];//chenyuzhu add start on 2014-7-20
extern CH_CONFIG_GLOBAL ch_config_global;//chenyuzhu add start on 2014-7-20
extern bool g_bQieHuan; //chenyuzhu add on 2014-10-10
extern bool *g_bInjPointPath; //chenyuzhu add on 2014-11-4
extern QString g_Banji;
extern QString g_Renyuan;
extern QString g_strUserPer;
extern GuoLvQueXian g_GuoLvQueXian;

Q_DECLARE_METATYPE(CH_FLAWDATA)
Q_DECLARE_METATYPE(double*);


CCscanPanel::CCscanPanel(QSqlDatabase *pDatabaseConnection,QWidget *parent):m_pDDatabaseConnection(pDatabaseConnection)
{
	m_ZeroPosFlag = rightBottom; //零点标示初始化 chenyuzhu add on 2014-10-27 
	//初始窗体本身

	m_IfInitBiaoZhun  = false; //判断标准初始化是否完成chenyuzhu add on 2014-5-8
	m_IfPrint = false;  //判断是否按下的是右键弹出的保存并打印按键 chenyuzhu add on 2014-5-22
	InitCscanWindow();	
	//连接槽函数
	connect(m_pWgtCscanDetectView, SIGNAL(mouseDoubleClicked(QMouseEvent *)), parent, SLOT(CsanDetectDoubleClick(QMouseEvent *)));
	connect(m_pCbDetectResult,SIGNAL(activated(int)),this,SLOT(ModifyDetectResult(int)));
	//正视图相关连接
	connect(m_pWgtFrontView,SIGNAL(signalDrawFrontDetial()),this,SLOT(DrawFrontDetial()));
	connect(m_pWgtFrontView,SIGNAL(signalDrawXShadowRegion()),this,SLOT(DrawXShadowRegion()));
	//侧视图相关链接
	connect(m_pWgtSideView,SIGNAL(signalDrawSideDetial()),this,SLOT(DrawSideDetial()));
	connect(m_pWgtSideView,SIGNAL(signalDrawYShadowRegion()),this,SLOT(DrawYShadowRegion()));
	connect(m_pWgtWholeView,SIGNAL(signalDrawWholeDetial()),this,SLOT(DrawWholeDetial()));
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawDetectDetial()),this,SLOT(DrawDetectDetial()));
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawPlateFlawSection()),this,SLOT(DrawPlateFlawSection()));	
	connect(m_pWgtThicknessView,SIGNAL(signalDrawThicknessDetial()),this,SLOT(DrawThicknessDetial()));
	connect(m_pWgtWholeView,SIGNAL(signalChangeSelectRect(int,int,int,int)),this,SLOT(OnChangeSelectRect(int,int,int,int)));
	connect(m_aSaveAndPrint,SIGNAL(triggered()),this,SLOT(OnIfPrint()));//chenyuzhu modify on 2014-5-22
	connect(m_pWgtWholeView,SIGNAL(signalDrawWholeRunning()),this,SLOT(DrawRunningWholeDetial()));//chenyuzhu add start on 2014-10-29
	//chenyuzhu add start on 2014-8-3
	connect(m_pWgtTopView,SIGNAL(signalDrawTopDetial()),this,SLOT(DrawTop()));
	connect(m_pWgtBottomView,SIGNAL(signalDrawBottomDetial()),this,SLOT(DrawBottom()));
	connect(m_pWgtLeftView,SIGNAL(signalDrawLeftDetial()),this,SLOT(DrawLeft()));
	connect(m_pWgtRightView,SIGNAL(signalDrawRightDetial()),this,SLOT(DrawRight()));
	//chenyuzhu add end on 2014-8-3
	
    connect(m_pWgtCscanDetectView,SIGNAL(signalDrawFrontBoostSection(double,double,double,double)),this,SLOT(OnDrawFrontBoostSection(double,double,double,double)));
    connect(m_pWgtCscanDetectView,SIGNAL(signalDrawSideBoostSection(double,double,double,double)),this,SLOT(OnDrawSideBoostSection(double,double,double,double)));

	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawWholeDashSection(double,double,double,double)),m_pWgtWholeView,SLOT(OnSignalDrawWholeDashSection(double,double,double,double)));
	
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawThicknessSection(double,double,double,double)),m_pWgtThicknessView,SLOT(OnSignalDrawCScanThicknessSection(double,double,double,double)));//chenyuzhu add on 2014-7-23
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawTopSection(double,double,double,double)),m_pWgtTopView,SLOT(OnSignalDrawCScanTopSection(double,double,double,double)));//chenyuzhu add on 2014-8-3
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawBottomSection(double,double,double,double)),m_pWgtBottomView,SLOT(OnSignalDrawCScanBottomSection(double,double,double,double)));//chenyuzhu add on 2014-8-3
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawLeftSection(double,double,double,double)),m_pWgtLeftView,SLOT(OnSignalDrawCScanLeftSection(double,double,double,double)));//chenyuzhu add on 2014-8-3
	connect(m_pWgtCscanDetectView,SIGNAL(signalDrawRightSection(double,double,double,double)),m_pWgtRightView,SLOT(OnSignalDrawCScanRightSection(double,double,double,double)));//chenyuzhu add on 2014-8-3
	//chenyuzhu add start on 2014-6-9
	connect(m_aDestroy,SIGNAL(triggered()),this,SLOT(OnDestroy()));
	m_nLogicXStart = 0;
	m_nLogicXEnd = 0;
	m_nLogicYStart = 0;
	m_nLogicYEnd = 0;
	//chenyuzhu add end on 2014-6-9
	m_pDefectDistaneSmaller = NULL;//chenyuzhu add on 2014-6-19
	m_bTanTouDraw = false;//chenyuzhu add on 2014-6-23
	//chenyuzhu add on 2014-6-23
	m_IniFlieName = QDir::currentPath() += "/PUTS.ini";
	m_pSettings = new QSettings(m_IniFlieName, QSettings::IniFormat); //初始化配置对象
	m_bPointJoin = m_pSettings->value("ToolsDialogInfo/NextDefectDistant","").toBool();
	//chenyuzhu add on 2014-6-23
	//chenyuzhu add start on 2014-7-28
	m_dMaxArea = 0;
	m_dMaxLength = 0;
	//chenyuzhu add end on 2014-7-28
	m_nInvalidEnd = 0;//chenyuzhu add end on 2014-8-9
}

void CCscanPanel::OnPrintPreview()             //打印预览
{
//在数据库中保存报表数据先注释掉用于调试

	SaveResult2DB();                     //在数据库中保存报表数据
	// 初始化自动打印需要的数据
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	// 用到预览对话框类
	m_PreviewWnd.InitTableItem(strBiaoZhun);
	// 根据ID从数据库读值
	long lID;
	g_CS.lock();
	lID = g_MaxCheckId.toLong();
	g_CS.unlock();
	ReadDBValue(lID);
	QPrinter Printer;
	Printer.setPageSize(QPrinter::B5);
	Printer.setOrientation(QPrinter::Landscape);
	Printer.setDocName(qApp->applicationName());
	Printer.setPageMargins(30,30,30,30,QPrinter::DevicePixel);
	QPrintPreviewDialog printPreDialg(&Printer, this);
	connect(&printPreDialg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(OnDrawPrintPreviewDialog(QPrinter*)));
	printPreDialg.resize(1500,800);
	printPreDialg.exec();
}
void CCscanPanel::OnDrawPrintPreviewDialog(QPrinter* printer) //打印预览画图
{
	QPainter painter(printer);
	QRect rectWindow = painter.window();
	QRect textRect(0,0,rectWindow.width(),350);
	DrawPrintText(textRect,painter);
	if (*m_pbPrintInjPointDetailPic)
	{
		QRect rectWhole(0,textRect.height()+100,rectWindow.width()-20,rectWindow.height()-textRect.height()-160);
		DrawPrintWhole(rectWhole,painter);
		/*QRect rectWhole(0,textRect.height()+10,rectWindow.width()-150,rectWindow.height()-textRect.height()-10-100);
		DrawPrintWhole(rectWhole,painter);
		QRect rectFont(0,textRect.height()+10+rectWhole.height(),rectWindow.width()-150,rectWindow.height()-textRect.height()-10-rectWhole.height());
		DrawPrintFront(rectFont,painter);
		QRect recrSide(rectWhole.width(),textRect.height()+10,150,rectWhole.height());
		DrawPrintSide(recrSide,painter);*/
	}
	painter.end();
}
void   CCscanPanel::OnRecticCheck(int bCheck)  //中心十字线
{
	m_bCentreCrosingLine = bCheck;    
}
void   CCscanPanel::OnPointRangeCheck(int bCheck) //伤点范围线
{
	m_bInjPointRangeLine = bCheck;    
}


//chenyuzhu add start on 2014-6-19
void   CCscanPanel::OnDefectDistantCheck(int bCheck) //伤点合并图
{
	 m_bPointJoin = bCheck;    
}
//chenyuzhu add end on 2014-6-19
CCscanPanel::~CCscanPanel()
{
	if (m_pBiaoZhun != NULL)
	{
		delete m_pBiaoZhun;
		m_pBiaoZhun = NULL;
	}
	if (NULL != g_pBuf4)
	{
		delete[] g_pBuf4;
		g_pBuf4 = NULL;
	}			
	if (NULL != m_mPopUpMenu)
	{
		delete[] m_mPopUpMenu;
		m_mPopUpMenu = NULL;
	}
	if (NULL != m_aSaveAndPrint)
	{
		delete[] m_aSaveAndPrint;
		m_aSaveAndPrint = NULL;
	}
	if (-1 != m_nTimerSTID)
	{
		killTimer(m_nTimerSTID);
		m_nTimerSTID = -1;
	}

}
/** 
* 初始化C扫界面
* @since     1.0
*/
void CCscanPanel::InitCscanWindow()
{
	m_pBiaoZhun = NULL;	
	m_nDoubleClickedDoshRectID = -1;
	m_nTimerSTID = -1;
	m_bCentreCrosingLine = false;     //中心十字线
	m_bInjPointRangeLine = false;    //伤点范围线
//	m_bPointJoin = false;     //伤点合并标志 chenyuzhu add on 2014-6-24

	m_nPlateYStart = 0;   //局部钢板区域中长度的起始
	m_nPlateYEnd = 0;     //局部钢板区域中长度的终点
	m_mPopUpMenu = new QMenu();
	m_aSaveAndPrint = new QAction(tr("保存并打印"),this);
	m_mPopUpMenu->addAction(m_aSaveAndPrint);
	m_aDestroy = new QAction(tr("伤点删除"),this);
	m_mPopUpMenu->addAction(m_aDestroy);

	//设置宽度不能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);

	QFont font12;
	font12.setPointSize(12);
	font12.setBold(true);
	font12.setWeight(75);
	//////////////////////////////////////////////////////////////////////////
	//在DOCK界面加了一个WIDGET，才能使用布局
	m_pWdtCscanMain =  new QWidget();
	setObjectName(QString::fromUtf8("CscanWidget"));
	setWindowTitle(QObject::tr("C扫"));
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
	setFeatures(QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	m_pMainlayout = new QGridLayout(m_pWdtCscanMain);

	m_pMainlayout->setSpacing(2);
	m_pMainlayout->setContentsMargins(11, 11, 11, 11);
	m_pMainlayout->setObjectName(QString::fromUtf8("mainlayout"));
	//////////////////////////////////////////////////////////////////////////

	//厚度图
	m_pWgtThicknessView = new CScanThicknessWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtThicknessView,0,1,1,1);

	//其他系统状态
	m_pWgtOtherSystem = new QWidget(m_pWdtCscanMain);
	m_pWgtOtherSystem->setGeometry(QRect(1406, 0, 143, 126));
	m_pWgtOtherSystem->setObjectName(QString::fromUtf8("m_pWgtOtherSystem"));
	sizePolicy.setHeightForWidth(m_pWgtOtherSystem->sizePolicy().hasHeightForWidth());
	m_pWgtOtherSystem->setSizePolicy(sizePolicy);
	m_pWgtOtherSystem->setMinimumSize(QSize(143, 126));
	//m_pWgtOtherSystem->setMaximumSize(QSize(143, 135));
	m_pWgtOtherSystem->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);border-width: 2px; border-style: solid;border-color: rgb(255, 255, 255);"));

	m_pOtherSystemlayout = new QVBoxLayout(m_pWgtOtherSystem);
	m_pOtherSystemlayout->setSpacing(2);
	m_pOtherSystemlayout->setContentsMargins(11, 11, 11, 11);
	m_pOtherSystemlayout->setObjectName(QString::fromUtf8("otherSystemlayout"));
	m_PLblPLCStatus = new QLabel(QObject::tr("P L C"),m_pWgtOtherSystem);//PLC
	m_PLblPLCStatus->setObjectName(QString::fromUtf8("lblPLCStatus"));
	m_PLblPLCStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));//默认红色
	m_PLblPLCStatus->setFont(font12);
	m_PLblPLCStatus->setGeometry(QRect(12, 2, 97, 35));
	m_pOtherSystemlayout->addWidget(m_PLblPLCStatus);
	m_pLblUSTStatus = new QLabel(QObject::tr("U S T"),m_pWgtOtherSystem);//UST
	m_pLblUSTStatus->setObjectName(QString::fromUtf8("lblPLCStatus"));
	m_pLblUSTStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));//默认绿色
	m_pLblUSTStatus->setFont(font12);
	m_pLblUSTStatus->setGeometry(QRect(12, 37, 97, 35));
//	m_pLblUSTStatus->setAlignRight(Qt::AlignRight | Qt::AlignVCenter); 
	m_pOtherSystemlayout->addWidget(m_pLblUSTStatus);
	m_pLblServerStatus = new QLabel(QObject::tr("Server"),m_pWgtOtherSystem);//数据库
	m_pLblServerStatus->setObjectName(QString::fromUtf8("lblPLCStatus"));
	m_pLblServerStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));//默认红色
	m_pLblServerStatus->setFont(font12);
	m_pLblServerStatus->setGeometry(QRect(12, 74, 97, 35));
	m_pOtherSystemlayout->addWidget(m_pLblServerStatus);
	m_pMainlayout->addWidget(m_pWgtOtherSystem,0,3,1,1);

	//正视图
	m_pWgtFrontView = new CScanFrontWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtFrontView,1,1,1,1);

	//检测结果
	m_pWgtDetectResult = new QWidget(m_pWdtCscanMain);
	m_pWgtDetectResult->setObjectName(QString::fromUtf8("m_pWgtDetectResult"));
	m_pWgtDetectResult->setGeometry(QRect(1406, 145, 143, 86));
	sizePolicy.setHeightForWidth(m_pWgtDetectResult->sizePolicy().hasHeightForWidth());
	m_pWgtDetectResult->setSizePolicy(sizePolicy);
	m_pWgtDetectResult->setMinimumSize(QSize(143, 86));
	m_pWgtDetectResult->setMaximumSize(QSize(143, 86));
	//m_pWgtDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
	m_pWgtDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);border-width: 2px; border-style: solid;border-color: rgb(255, 255, 255);"));
	m_pPstBtnDetectResult = new QPushButton(m_pWgtDetectResult);
	m_pPstBtnDetectResult->setObjectName(QString::fromUtf8("pstBtnDetectResult"));
	m_pPstBtnDetectResult->setText(tr("准备探伤..."));
	m_pPstBtnDetectResult->setGeometry(QRect(12, 10, 119, 66));
	m_pPstBtnDetectResult->setMinimumSize(QSize(119, 66));
	m_pPstBtnDetectResult->setMaximumSize(QSize(119, 66));
	m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0); border-style: ridge;border-color: rgb(255, 0, 0);"));//默认黄色

	//探伤结果修正
	m_pCbDetectResult = new QComboBox(m_pWgtDetectResult);
	m_pCbDetectResult->setGeometry(QRect(14, 29, 115, 26));
	m_pCbDetectResult->setMinimumSize(QSize(115, 26));
	m_pCbDetectResult->setMaximumSize(QSize(115, 26));
	m_pCbDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255); border-style: ridge;border-color: rgb(255, 255, 255);"));//默认黄色
	m_pCbDetectResult->setVisible(false);//默认隐藏
	m_pCbDetectResult->addItem(tr("不合格"));  
	m_pCbDetectResult->addItem(tr("合格"));  

	m_pMainlayout->addWidget(m_pWgtDetectResult,1,3,1,1);

	//伤点图
	m_pWgtCscanDetectView = new CScanDetectWidget(m_pWdtCscanMain);	
	m_pMainlayout->addWidget(m_pWgtCscanDetectView,3,1,1,1);	

	//侧视图
	m_pWgtSideView = new CScanSideWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtSideView,3,3,1,1);

	//整体视图
	m_pWgtWholeView = new CScanWholeWidget(m_pWdtCscanMain);	
	m_pMainlayout->addWidget(m_pWgtWholeView, 5, 1, 1, 3);


	//边缘伤点图///////////////////////////////////////////////////////////////
	//左侧边缘伤点图
	m_pWgtLeftView = new CScanLeftWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtLeftView, 3, 0, 1, 1);
	//右侧边缘伤点图
	m_pWgtRightView = new CScanRightWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtRightView, 3, 2, 1, 1);
	//上侧边缘伤点图
	m_pWgtTopView = new CScanTopWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtTopView, 2, 1, 1, 1);
	//下侧边缘伤点图
	m_pWgtBottomView = new CScanBottomWidget(m_pWdtCscanMain);
	m_pMainlayout->addWidget(m_pWgtBottomView, 4, 1, 1, 1);

	//////////////////////////////////////////////////////////////////////////

	setWidget(m_pWdtCscanMain);
	setLayout(m_pMainlayout);

	//停靠窗体
	m_PLblPLCStatus->installEventFilter(this);
	m_pLblUSTStatus->installEventFilter(this);
	m_pLblServerStatus->installEventFilter(this);
	m_pPstBtnDetectResult->installEventFilter(this);

	m_pDockPLC = new CDockForm;
	m_pDockPLC->setWindowTitle(tr("PLC系统状态"));
	m_pDockPLC->SetSize(200, 200); //chenyuzhu add on 2014-10-30
	m_pDockUST = new CDockForm;
	m_pDockUST->setWindowTitle(tr("UST系统状态"));
	m_pDockUST->SetSize(200, 200); //chenyuzhu add on 2014-10-30
	m_pDockServer = new CDockForm;
	m_pDockServer->setWindowTitle(tr("Server系统状态"));
	m_pDockServer->SetSize(200, 200); //chenyuzhu add on 2014-10-30
	//chenyuzhu add start on 2014-7-28 
	m_pDetectResult = new CDockForm;
	m_pDetectResult->setWindowTitle(tr("探伤结果"));
	m_pDetectResult->SetSize(300, 200); //chenyuzhu add on 2014-10-30
	//chenyuzhu add end on 2014-7-28 
	installEventFilter(this);
	//chenyuzhu add start on 2014-5-28 //用于初始化局部视图标尺
	int temp = float(g_Info[nPlate].width)*(float(m_pWgtCscanDetectView->height())/float(m_pWgtCscanDetectView->width()))/float(g_Info[nPlate].length)*m_pWgtWholeView->width();
	m_nPlateYEnd = temp;
	//chenyuzhu add end on 2014-5-28
	
	m_pWgtCscanDetectView->m_pPlateYStart = &m_nPlateYStart;
	m_pWgtCscanDetectView->m_pPlateYEnd = &m_nPlateYEnd;
	m_pWgtWholeView->m_pRegionRect = &m_RegionRect;	
}

/** 
* 窗体大小改变事件
* @since     1.0
*/
void CCscanPanel::resizeEvent(QResizeEvent *e)
{
	//m_pWgtThicknessView->update();
}

void CCscanPanel::DrawFrontDetial()
{
	//准备
	if(NULL != m_pWgtFrontView->m_pFrontImage)
	{
		delete m_pWgtFrontView->m_pFrontImage;
		m_pWgtFrontView->m_pFrontImage = NULL;
	}
	m_pWgtFrontView->m_pFrontImage = new QImage(m_pWgtFrontView->rect().width(),m_pWgtFrontView->rect().height(),QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtFrontView->m_pFrontImage);
	imagePainter.initFrom(m_pWgtFrontView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtFrontView->m_pFrontImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	//m_pWgtFrontView->m_pFrontImage->fill(QColor(17,14,9));   //背景
	m_pWgtFrontView->m_pFrontImage->fill(QColor(103,103,105));   //背景chenyuzhu add on 2014-11-4
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);

    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");

        if (static_cast<int>(temp.minX) >= m_nPlateYStart && static_cast<int>(temp.maxX) <= m_nPlateYEnd
            && static_cast<int>(temp.maxY) <= static_cast<int>(g_Info[nPlate].width)
            && static_cast<int>(temp.minY) <= static_cast<int>(g_Info[nPlate].width))
        {
            //计算点坐标
              QList<double> tempList;
              QList<int> ResultList;
              tempList << temp.minX << temp.maxX << temp.z;
              int tempFlag = CalculatePointForPannel(CALCULATE_FRONTPOINT, tempList, ResultList);
              if (-1 == tempFlag)
              {
                  QMessageBox::information(this,"DrawFrontDetial", "CalculatePointForPannel error");
                  return;
              }
              int xStart = ResultList.at(0);
              int xEnd = ResultList.at(1);
              int y = ResultList.at(2);
              //chenyuzhu modify start on 2014-6-17 //根据面积过滤
              if(*g_bInjPointFilter)
              {
                  if (*g_bFilterSmaller)
                  {
                      if(temp.mianShangArea < *g_nFilterSmallerNum)
                      {
                          continue;
                      }
                  }
                  if (*g_bFilterBiger)
                  {
                      if(temp.mianShangArea > *g_nFilterBigerNum)
                      {
                          continue;
                      }
                  }
              }
              QColor clrCurrent;
              QColor clrLostWave;
              clrCurrent.setRgb(0,255,0);
              clrLostWave.setRgb(255,255,255);

              if (g_nComboxColor == 0)
              {
                  if (fColor < 0.5)
                  {
                      clrCurrent.setRgb(0,255,0);
                  }
                  if (fColor >= 0.5 && fColor < 1.0)
                  {
                      clrCurrent.setRgb(255,255,0);
                  }
                  if (fColor >= 1.0)
                  {
                      clrCurrent.setRgb(255,0,0);
                  }
                  else
                  {
                  }
              }
              else if (g_nComboxColor == 1)
              {
                  if (temp.z == 0)
                  {
                      clrCurrent = clrLostWave;
                  }
                  else
                  {
                      clrCurrent = CalcFillColor(int(temp.z));
                  }
              }
              imagePainter.setPen(QPen(clrCurrent,1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
              //chenyuzhu add start on 2014-6-13
              imagePainter.drawLine(QPoint(xStart,y),QPoint(xEnd,y));
        }
        else
        {
            continue;
        }
    }
    imagePainter.setPen(Qt::red);
    imagePainter.setBrush(Qt::NoBrush);
    imagePainter.end();
    QPainter widgetPainter(m_pWgtFrontView);
    widgetPainter.drawImage(QRect(46,1,m_pWgtFrontView->rect().width()-47,m_pWgtFrontView->rect().height()-2),*m_pWgtFrontView->m_pFrontImage,QRect(0,0,m_pWgtFrontView->m_pFrontImage->rect().width(),m_pWgtFrontView->m_pFrontImage->rect().height()));
}
void CCscanPanel::DrawXShadowRegion()// 画X投影区域
{
	// 获取Y方向伤点个数
	QString strFlawShadowCount;
	QString strYCount;
	int *YArray = NULL;
	YArray = NULL;
	int nYSize = 0;
    strFlawShadowCount = g_pPlateDraw-> dynamicCall("GetXYFlawCount(double,double,double,double)",m_nPlateYStart,m_nPlateYEnd,0,g_Info[nPlate].width).toString();
	int index = strFlawShadowCount.indexOf(QString("y"));
	strYCount = strFlawShadowCount.left(index);
	nYSize = (m_nPlateYEnd - m_nPlateYStart+5 - 1)/5;  //算网格
	YArray = new int[nYSize];
	if (YArray == NULL)
	{
		return;
	}
	memset(YArray, 0,sizeof(int)*nYSize);
	// 将字符串转换为数组
	unsigned short nYMax = ShadowStr2Array(strYCount, YArray, nYSize);
	//准备画图
	//准备
	if(NULL != m_pWgtFrontView->m_pFrontImage)
	{
		delete m_pWgtFrontView->m_pFrontImage;
		m_pWgtFrontView->m_pFrontImage = NULL;
	}
	m_pWgtFrontView->m_pFrontImage = new QImage(m_pWgtFrontView->rect().width()-40,m_pWgtFrontView->rect().height()-40,QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtFrontView->m_pFrontImage);
	imagePainter.initFrom(m_pWgtFrontView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtFrontView->m_pFrontImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	m_pWgtFrontView->m_pFrontImage->fill(QColor(17,14,9));  //背景
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);
    // 画线
	float xStep = float(m_pWgtFrontView->m_pFrontImage->rect().width())/float(nYSize);
	float yStep = float(m_pWgtFrontView->m_pFrontImage->rect().height())/float(nYMax);
	QPoint ptStart, ptEnd;
	for (int i=1; i<=nYSize; i++)
	{
		//赋值
 		QList<double> tempList;
		tempList<<static_cast<double>(xStep)<<static_cast<double>(yStep)<<static_cast<double>(i)<<static_cast<double>(YArray[i-1]);
		QList<int> resultList;
		int tempFlag =  CalculatePointForPannel(CALCULATE_XPOINT,tempList,resultList);  //给视图计算坐标的方法
		if(-1 == tempFlag)
		{
			QMessageBox::information(this,"DrawXShadowRegion","CalculatePointForPannel error");
			return;
		}
        int x1 = resultList.at(0);
		int y1 = resultList.at(1);
		int x2 = resultList.at(2);
		int y2 = resultList.at(3);
		ptStart.setX(x1);
		ptStart.setY(y1);
		ptEnd.setX(x2);
		ptEnd.setY(y2);
		imagePainter.drawLine(ptStart,ptEnd);
	}
	imagePainter.end();
	QPainter widgetPainter(m_pWgtFrontView);
	widgetPainter.drawImage(QRect(46,1,m_pWgtFrontView->rect().width()-47,m_pWgtFrontView->rect().height()-2),*m_pWgtFrontView->m_pFrontImage,QRect(0,0,m_pWgtFrontView->m_pFrontImage->rect().width(),m_pWgtFrontView->m_pFrontImage->rect().height()));
}


void CCscanPanel::DrawSideDetial()
{
	//创建QImage
	if(NULL != m_pWgtSideView->m_pSideImage)
	{
		delete m_pWgtSideView->m_pSideImage;
		m_pWgtSideView->m_pSideImage = NULL;
	}
	m_pWgtSideView->m_nXSize = m_pWgtSideView->rect().width();
	m_pWgtSideView->m_nYSize = m_pWgtSideView->rect().height();

	m_pWgtSideView->m_pSideImage = new QImage( m_pWgtSideView->m_nXSize,m_pWgtSideView->m_nYSize,QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtSideView->m_pSideImage);
	imagePainter.initFrom(m_pWgtSideView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtSideView->m_pSideImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	//m_pWgtSideView->m_pSideImage->fill(QColor(17,14,9));  //背景
	m_pWgtSideView->m_pSideImage->fill(QColor(103,103,105));  //背景//chenyuzhu add on 2014-11-4
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);
	//具体的画图过程
    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");
        if (static_cast<int>(temp.minX) >= m_nPlateYStart && static_cast<int>(temp.maxX) <= m_nPlateYEnd
            && static_cast<int>(temp.maxY) <= static_cast<int>(g_Info[nPlate].width)
            && static_cast<int>(temp.minY) <= static_cast<int>(g_Info[nPlate].width))
		{
			//计算点坐标
			QList<double> tempList;
            tempList << temp.minY << temp.maxY << temp.z;
			QList<int> resultList;
			int tempFlag = CalculatePointForPannel(CALCULATE_SIDEPOINT,tempList,resultList);
			if (-1 == tempFlag)
			{
				QMessageBox::information(this,"CalculatePointForPannel",tr("计算点失败"));
				return;
			}
			int x = resultList.at(0);
			int yStart = resultList.at(1);
			int yEnd = resultList.at(2);

			//chenyuzhu modify start on 2014-6-17 //根据面积过滤
			if(*g_bInjPointFilter)
			{
				if (*g_bFilterSmaller)
				{
                    if(temp.mianShangArea < *g_nFilterSmallerNum)
					{
						continue;
					}
				}
				if (*g_bFilterBiger)
				{
                    if(temp.mianShangArea > *g_nFilterBigerNum)
					{
						continue;
					}
				}
			}
			QColor clrCurrent;
			QColor clrLostWave;
			clrCurrent.setRgb(0,255,0);
			clrLostWave.setRgb(255,255,255);

			if (g_nComboxColor == 0)
			{
                if (fColor < 0.5)
                {
                    clrCurrent.setRgb(0,255,0);
                }
                if (fColor >= 0.5 && fColor < 1.0)
                {
                    clrCurrent.setRgb(255,255,0);
                }
                if (fColor >= 1.0)
                {
                    clrCurrent.setRgb(255,0,0);
                }
                else
                {
                }
			}
			else if (g_nComboxColor == 1)
			{
                if (temp.z == 0)
				{
					clrCurrent = clrLostWave;
				}
				else
				{
                    clrCurrent = CalcFillColor(int(temp.z));
				}	
			}
			imagePainter.setPen(QPen(clrCurrent,1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
			//chenyuzhu add start on 2014-6-13
			imagePainter.drawLine(QPoint(x,yStart),QPoint(x,yEnd));		
		} 
		else
		{
			continue;
		}
	}
	imagePainter.end();
 	QPainter widgetPainter(m_pWgtSideView);
 	widgetPainter.drawImage(QRect(1,40,m_pWgtSideView->rect().width()-2,m_pWgtSideView->rect().height()-41),*m_pWgtSideView->m_pSideImage,QRect(0,0,m_pWgtSideView->m_pSideImage->rect().width(),m_pWgtSideView->m_pSideImage->rect().height()));
}

void CCscanPanel::DrawYShadowRegion()// 画Y投影区域
{

	// 获取X方向伤点个数
	QString strFlawShadowCount;
	QString strXCount;
	QString strYCount;
	int *XArray = NULL;
	XArray = NULL;
	int nXSize = 0;
	strFlawShadowCount = g_pPlateDraw->dynamicCall("GetXYFlawCount(double,double,double,double)",m_nPlateYStart,m_nPlateYEnd,0,g_Info[nPlate].width).toString();
	int index = strFlawShadowCount.indexOf(QString("y"));
	strYCount = strFlawShadowCount.left(index);
	strXCount = strFlawShadowCount.right(strFlawShadowCount.length() - index);
	//nXSize = (g_Info[nPlate].width+g_IniParams.m_maxYDistance-1)/g_IniParams.m_minYDistance;
	nXSize = (g_Info[nPlate].width+40-1)/20; //网格
	XArray = new int[nXSize];
	if (XArray == NULL)
	{
		return;
	}
	memset(XArray, 0,sizeof(int)*nXSize);
	// 将字符串转换为数组
	unsigned short nXMax = ShadowStr2Array(strXCount, XArray, nXSize);
	//创建QImage
	if(NULL != m_pWgtSideView->m_pSideImage)
	{
		delete m_pWgtSideView->m_pSideImage;
		m_pWgtSideView->m_pSideImage = NULL;
	}
	m_pWgtSideView->m_pSideImage = new QImage( m_pWgtSideView->rect().width(),m_pWgtSideView->rect().height(),QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtSideView->m_pSideImage);
	imagePainter.initFrom(m_pWgtSideView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtSideView->m_pSideImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	m_pWgtSideView->m_pSideImage->fill(QColor(17,14,9));  //背景
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);
	// 画线
	float xStep = float(m_pWgtSideView->m_pSideImage->rect().width())/float(nXMax);
	float yStep = float(m_pWgtSideView->m_pSideImage->rect().height())/float(nXSize);

	QPoint ptStart, ptEnd;
	for (int i=1; i<=nXSize; i++)
	{
        QList<double> tempList;
		tempList<<static_cast<double>(xStep)<<static_cast<double>(yStep)<<static_cast<double>(i)<<static_cast<double>(XArray[i-1]);
		QList<int> resultList;
		int tempFlag =  CalculatePointForPannel(CALCULATE_YPOINT,tempList,resultList);
		if (-1 == tempFlag)
		{
			QMessageBox::information(this,"DrawYShadowRegion","CalculatePointForPannel error");
			return;
		}
		int x1 = resultList.at(0);
		int y1 = resultList.at(1);
		int x2 = resultList.at(2);
		int y2 = resultList.at(3);
		ptStart.setX(x1);
		ptStart.setY(y1);
		ptEnd.setX(x2);
		ptEnd.setY(y2);
		imagePainter.drawLine(ptStart,ptEnd);
	}
	imagePainter.end();
	QPainter widgetPainter(m_pWgtSideView);
	widgetPainter.drawImage(QRect(1,40,m_pWgtSideView->rect().width()-2,m_pWgtSideView->rect().height()-41),*m_pWgtSideView->m_pSideImage,QRect(0,0,m_pWgtSideView->m_pSideImage->rect().width(),m_pWgtSideView->m_pSideImage->rect().height()));
}

void CCscanPanel::DrawRunningWholeDetial() //全局视图视图
{
	QBrush RedBrush(QColor(255, 0,0));
	QBrush BlackBrush(QColor(0,0,0));
//	float fX = 0.0f;
//	float fY = 0.0f;
	DeltaX dx;
	//chenyuzhu modify start on 2014-10-29
	//chenyuzhu add start on 2014-10-29
	//创建m_pWholeImage
	if(NULL != m_pWgtWholeView->m_pWholeImage)
	{
		delete m_pWgtWholeView->m_pWholeImage;
		m_pWgtWholeView->m_pWholeImage = NULL;
	}


	//m_pWgtWholeView->m_nXSize = m_pWgtWholeView->rect().width();
	//m_pWgtWholeView->m_nYSize = m_pWgtWholeView->rect().height();
	//更新全局视图内存位图的大小
	m_pWgtWholeView->m_nXSize = m_pWgtWholeView->width();
	m_pWgtWholeView->m_nYSize = m_pWgtWholeView->height();

	m_pWgtWholeView->m_pWholeImage = new QImage( m_pWgtWholeView->m_nXSize,m_pWgtWholeView->m_nYSize,QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtWholeView->m_pWholeImage);
	imagePainter.initFrom(this);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtWholeView->m_pWholeImage)
	{
		QMessageBox::information(this,tr("DrawImmediateImage"),tr("m_pWholeImage创建失败"));
		return;
	}
	//m_pWgtWholeView->m_pWholeImage->fill(QColor(17,14,9));  //背景
	m_pWgtWholeView->m_pWholeImage->fill(QColor(103,103,105));  //背景chenyuzhu add on 2014-11-4
	imagePainter.setPen(QPen(QColor(255,255,255),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));

	//chenyuzhu add end on 2014-10-29


	for (int i=0; i<m_FlawCoordinateVector.size(); i++)
	{
		QRect Rect(QPoint(m_FlawCoordinateVector[i].point.x() -2, m_FlawCoordinateVector[i].point.y() -2), 
			QPoint(m_FlawCoordinateVector[i].point.x()+2, m_FlawCoordinateVector[i].point.y()+2));
		if (m_FlawCoordinateVector[i].DETECT_STATE == 0)
		{
			//			pDC->SelectObject(&BlackBrush);
			QBrush brush(QColor(255,255,255),Qt::SolidPattern);
			imagePainter.setBrush(brush);
		}
		else if (m_FlawCoordinateVector[i].DETECT_STATE == 1)
		{
			//			pDC->SelectObject(&RedBrush);
			QBrush brush(QColor(255, 0,0),Qt::SolidPattern);
			imagePainter.setBrush(brush);
		}
		else
		{			
		}
		imagePainter.drawEllipse(Rect);
		//imagePainter.drawEllipse(98, 98, 102, 102);
		//		pDC->Ellipse(rect);
	}	
	QPainter widgetPainter(m_pWgtWholeView);
	widgetPainter.drawImage(QRect(46,1,m_pWgtWholeView->rect().width()-47,m_pWgtWholeView->rect().height()-40),*(m_pWgtWholeView->m_pWholeImage),QRect(0,0,m_pWgtWholeView->m_pWholeImage->rect().width(),m_pWgtWholeView->m_pWholeImage->rect().height()));
	//chenyuzhu modify end on 2014-10-29
}

void CCscanPanel::DrawWholeDetial() //全局视图
{

	DrawFlawDC(m_pWgtWholeView->m_nXSize,m_pWgtWholeView->m_nYSize,m_pWgtWholeView,m_pWgtWholeView->m_pWholeImage);
	QPainter widgetPainter(m_pWgtWholeView);
	widgetPainter.drawImage(QRect(46,1,m_pWgtWholeView->rect().width()-47,m_pWgtWholeView->rect().height()-40),*(m_pWgtWholeView->m_pWholeImage),QRect(0,0,m_pWgtWholeView->m_pWholeImage->rect().width(),m_pWgtWholeView->m_pWholeImage->rect().height()));
	DrawTop();
	DrawBottom();
	DrawLeft(); //左边视图
	DrawRight();//右边视图
}

void CCscanPanel::DrawDetectDetial() //局部视图
{	
	m_pWgtCscanDetectView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtCscanDetectView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	DrawFlawDC(m_pWgtCscanDetectView->m_nXSize,m_pWgtCscanDetectView->m_nYSize,m_pWgtCscanDetectView,m_pWgtCscanDetectView->m_pImage);
	QPainter widgetPainter(m_pWgtCscanDetectView);
	if (m_ZeroPosFlag < 3)
	{
		m_pWgtCscanDetectView->m_nXStart = int(m_nPlateYStart / g_Info[nPlate].length * m_pWgtCscanDetectView->m_nXSize);
		m_pWgtCscanDetectView->m_nXEnd = int(m_nPlateYEnd / g_Info[nPlate].length * m_pWgtCscanDetectView->m_nXSize);
	}	
	else if(m_ZeroPosFlag < 5)
	{
 		m_pWgtCscanDetectView->m_nXEnd = int( m_pWgtCscanDetectView->m_nXSize - m_nPlateYStart / g_Info[nPlate].length * m_pWgtCscanDetectView->m_nXSize);
 		m_pWgtCscanDetectView->m_nXStart = int( m_pWgtCscanDetectView->m_nXSize - m_nPlateYEnd / g_Info[nPlate].length * m_pWgtCscanDetectView->m_nXSize);
	}
	else
	{
		QMessageBox::information(this,"DrawDetectDetial","m_nZeroPosFlag error");
	}

	widgetPainter.drawImage(QRect(46,39,m_pWgtCscanDetectView->rect().width()-47,m_pWgtCscanDetectView->rect().height()-40),*m_pWgtCscanDetectView->m_pImage,QRect(m_pWgtCscanDetectView->m_nXStart,0,(m_pWgtCscanDetectView->m_nXEnd - m_pWgtCscanDetectView->m_nXStart),m_pWgtCscanDetectView->m_pImage->height()));

	widgetPainter.end();
	
	//DrawTop(); //上边视图 //chenyuzhu add start on 2014-8-1
	
	//画完局部视图后更新其他相关的视图
	m_pWgtFrontView->update();//正视图
	m_pWgtSideView->update();//侧视图
	m_pWgtThicknessView->update(); 
	//chenyuzhu add start on 2014-8-3
	m_pWgtTopView->update();
	m_pWgtBottomView->update();
	m_pWgtLeftView->update();
	m_pWgtRightView->update();
	//chenyuzhu add end on 2014-8-3

}

//chenyuzhu add start on 2014-8-1
void CCscanPanel::DrawTop() //上边视图
{	
	m_pWgtTopView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtTopView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	QPainter widgetPainter(m_pWgtTopView);
	DrawFlawDC(m_pWgtTopView->m_nXSize,m_pWgtTopView->m_nYSize,m_pWgtTopView,m_pWgtTopView->m_pImage);
	if (m_ZeroPosFlag < 3)
	{
		m_pWgtTopView->m_nXStart = int(m_nPlateYStart / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
		m_pWgtTopView->m_nXEnd = int(m_nPlateYEnd / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
	}	
	else if(m_ZeroPosFlag < 5)
	{
		m_pWgtTopView->m_nXEnd = int( m_pWgtTopView->m_nXSize - m_nPlateYStart / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
		m_pWgtTopView->m_nXStart = int( m_pWgtTopView->m_nXSize - m_nPlateYEnd / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
	}
	else
	{
		QMessageBox::information(this,"DrawTop","m_nZeroPosFlag error");
	}
	widgetPainter.drawImage(QRect(46,1,m_pWgtTopView->rect().width()-47,m_pWgtTopView->rect().height()-2),*(m_pWgtTopView->m_pImage),QRect(m_pWgtTopView->m_nXStart,0,(m_pWgtTopView->m_nXEnd - m_pWgtTopView->m_nXStart),80.0 / g_Info[nPlate].width * m_pWgtTopView->m_pImage->height()));
	widgetPainter.end();
	//m_pWgtTopView->update();

}
//chenyuzhu add end on 2014-8-1
//chenyuzhu add start on 2014-8-1
void CCscanPanel::DrawBottom() //下边视图
{	
	m_pWgtBottomView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtBottomView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	QPainter widgetPainter(m_pWgtBottomView);
	DrawFlawDC(m_pWgtBottomView->m_nXSize,m_pWgtBottomView->m_nYSize,m_pWgtBottomView,m_pWgtBottomView->m_pImage);
	if (m_ZeroPosFlag < 3)
	{
		m_pWgtBottomView->m_nXStart = int(m_nPlateYStart / g_Info[nPlate].length * m_pWgtBottomView->m_nXSize);
		m_pWgtBottomView->m_nXEnd = int(m_nPlateYEnd / g_Info[nPlate].length * m_pWgtBottomView->m_nXSize);
	}	
	else if(m_ZeroPosFlag < 5)
	{
		m_pWgtBottomView->m_nXEnd = int( m_pWgtBottomView->m_nXSize - m_nPlateYStart / g_Info[nPlate].length * m_pWgtBottomView->m_nXSize);
		m_pWgtBottomView->m_nXStart = int( m_pWgtBottomView->m_nXSize - m_nPlateYEnd / g_Info[nPlate].length * m_pWgtBottomView->m_nXSize);
	}
	else
	{
		QMessageBox::information(this,"DrawBottom","m_nZeroPosFlag error");
	}
	//widgetPainter.drawImage(QRect(46,1,m_pWgtBottomView->rect().width()-47,m_pWgtBottomView->rect().height()-2),*(m_pWgtBottomView->m_pImage),QRect(m_pWgtBottomView->m_nXStart,0,(m_pWgtBottomView->m_nXEnd - m_pWgtBottomView->m_nXStart),60.0 / g_Info[nPlate].width * m_pWgtBottomView->m_pImage->height()));
	widgetPainter.drawImage(QRect(46,1,m_pWgtBottomView->rect().width()-47,m_pWgtBottomView->rect().height()-2),*(m_pWgtBottomView->m_pImage),QRect(m_pWgtBottomView->m_nXStart,m_pWgtBottomView->m_pImage->height() - (80.0 / g_Info[nPlate].width * m_pWgtBottomView->m_pImage->height()),(m_pWgtBottomView->m_nXEnd - m_pWgtBottomView->m_nXStart),(80.0 / g_Info[nPlate].width * m_pWgtBottomView->m_pImage->height())));
	widgetPainter.end();
	//m_pWgtBottomView->update();

}
//chenyuzhu add end on 2014-8-1
//chenyuzhu add start on 2014-8-1
void CCscanPanel::DrawLeft() //左边视图
{	
	m_pWgtLeftView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtLeftView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	QPainter widgetPainter(m_pWgtLeftView);
	DrawFlawDC(m_pWgtLeftView->m_nXSize,m_pWgtLeftView->m_nYSize,m_pWgtLeftView,m_pWgtLeftView->m_pImage);
/*	if (m_ZeroPosFlag < 3)
	{
		m_pWgtTopView->m_nXStart = int(m_nPlateYStart / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
		m_pWgtTopView->m_nXEnd = int(m_nPlateYEnd / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
	}	
	else if(m_ZeroPosFlag < 5)
	{
		m_pWgtTopView->m_nXEnd = int( m_pWgtTopView->m_nXSize - m_nPlateYStart / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
		m_pWgtTopView->m_nXStart = int( m_pWgtTopView->m_nXSize - m_nPlateYEnd / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
	}
	else
	{
		QMessageBox::information(this,"DrawTop","m_nZeroPosFlag error");
	}*/
	m_pWgtLeftView->m_nYStart = int(0);
	m_pWgtLeftView->m_nYEnd = int(m_pWgtLeftView->m_nYSize);
	widgetPainter.drawImage(QRect(1,39,m_pWgtLeftView->rect().width()-2,m_pWgtLeftView->rect().height()-39),*(m_pWgtLeftView->m_pImage),QRect(0, m_pWgtLeftView->m_nYStart,(80.0 / g_Info[nPlate].length * m_pWgtLeftView->m_pImage->width()),m_pWgtLeftView->m_nYEnd - m_pWgtLeftView->m_nYStart));
	widgetPainter.end();
	//m_pWgtLeftView->update();

}
//chenyuzhu add end on 2014-8-1
//chenyuzhu add start on 2014-8-1
void CCscanPanel::DrawRight() //右边视图
{	
	m_pWgtRightView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtRightView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	QPainter widgetPainter(m_pWgtRightView);
	DrawFlawDC(m_pWgtRightView->m_nXSize,m_pWgtRightView->m_nYSize,m_pWgtRightView,m_pWgtRightView->m_pImage);
	/*if (m_ZeroPosFlag < 3)
	{
		m_pWgtRightView->m_nXStart = int(m_nPlateYStart / g_Info[nPlate].length * m_pWgtRightView->m_nXSize);
		m_pWgtRightView->m_nXEnd = int(m_nPlateYEnd / g_Info[nPlate].length * m_pWgtRightView->m_nXSize);
	}	
	else if(m_ZeroPosFlag < 5)
	{
		m_pWgtTopView->m_nXEnd = int( m_pWgtTopView->m_nXSize - m_nPlateYStart / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
		m_pWgtTopView->m_nXStart = int( m_pWgtTopView->m_nXSize - m_nPlateYEnd / g_Info[nPlate].length * m_pWgtTopView->m_nXSize);
	}
	else
	{
		QMessageBox::information(this,"DrawTop","m_nZeroPosFlag error");
	}*/
	m_pWgtRightView->m_nYStart = int(0);
	m_pWgtRightView->m_nYEnd = int(m_pWgtLeftView->m_nYSize);
	widgetPainter.drawImage(QRect(1,39,m_pWgtRightView->rect().width()-2,m_pWgtRightView->rect().height()-39),*(m_pWgtRightView->m_pImage),QRect(m_pWgtRightView->m_pImage->width() - (80.0 / g_Info[nPlate].length * m_pWgtRightView->m_pImage->width()),m_pWgtRightView->m_nYStart, (80.0 / g_Info[nPlate].length * m_pWgtRightView->m_pImage->width()),m_pWgtRightView->m_nYEnd - m_pWgtRightView->m_nYStart));
	widgetPainter.end();
	//m_pWgtRightView->update();

}
//chenyuzhu add end on 2014-8-1

void CCscanPanel::DrawRunningThicknessDetial() //探测中的厚度图
{
	//创建QImage
	if(NULL != m_pWgtThicknessView->m_pThicknessImage)
	{
		delete m_pWgtThicknessView->m_pThicknessImage;
		m_pWgtThicknessView->m_pThicknessImage = NULL;
	}
	m_pWgtThicknessView->m_pThicknessImage = new QImage( m_pWgtThicknessView->rect().width()-39,m_pWgtThicknessView->rect().height()-39,QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtThicknessView->m_pThicknessImage);
	imagePainter.initFrom(m_pWgtThicknessView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtThicknessView->m_pThicknessImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	m_pWgtThicknessView->m_pThicknessImage->fill(QColor(17,14,9));  //背景
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);
	//处理数据
	QList<ThickOffset> ThickOffsetList;

	g_ItemMutex.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	g_ItemMutex.unlock();

	float fX = 0.0f;
	float fY = 0.0f;
	DeltaX dx;
 	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
 	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
 	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
 	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
 	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
 	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
 	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;

	float fScale = float(rect().width())/float(m_nPlateYEnd - m_nPlateYStart);
	QList<CH_FLAWDATA>::iterator i = m_FlawList.begin();

	while (i != m_FlawList.end())
	{
		CH_FLAWDATA tempFlaw = *i;
		i++;
		if (tempFlaw.DETECT_STATE == 2)
		{
			ThickOffset offset;
			bool bRet = CalculateFlawXY(tempFlaw, fX, fY, dx);
			if (!bRet)
			{
				continue;
			}

			offset.y = int((fX - m_nPlateYStart) * fScale) + rect().left();
			offset.value = float(tempFlaw.CH_HEIGHT - g_Info[nPlate].height)/float(g_Info[nPlate].height)*100.0f;
			ThickOffsetList.append(offset);

			int temp = int(fabsf(offset.value)+0.5);
			if (temp > m_nThickMaxValue)
			{
				m_nThickMaxValue = temp;
			}
		}
	}
	// 画偏差点
	QColor clrRed(255,0,0);
	QColor clrGreen(0,255,0);

	int k = ThickOffsetList.length();
	for(int j= 0; j<k;j++)
	{
		ThickOffset tempOffset =ThickOffsetList.at(j) ;
		tempOffset.x = int( float(tempOffset.value - m_nThickMaxValue * -1)/float(m_nThickMaxValue*2)*(rect().height()-39) + 0.5f ) + rect().top();
		float cha = fabsf(tempOffset.value) - 1.0f;

		if (cha > 0.000001)
		{
			//		pDC->SetPixel(tempOffset.x, tempOffset.y, clrRed);
			imagePainter.setPen(QPen(clrRed));
			imagePainter.drawPoint(QPoint(tempOffset.x,tempOffset.y));
		}
		else
		{
			//		pDC->SetPixel(tempOffset.x, tempOffset.y, clrGreen);
			imagePainter.setPen(QPen(clrGreen));
			imagePainter.drawPoint(QPoint(tempOffset.x,tempOffset.y));
		}

	}
	
}


void CCscanPanel::DrawThicknessDetial() //局部视图
{
	//创建QImage
	if(NULL != m_pWgtThicknessView->m_pThicknessImage)
	{
		delete m_pWgtThicknessView->m_pThicknessImage;
		m_pWgtThicknessView->m_pThicknessImage = NULL;
	}
	m_pWgtThicknessView->m_pThicknessImage = new QImage( m_pWgtThicknessView->rect().width()-39,m_pWgtThicknessView->rect().height()-39,QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtThicknessView->m_pThicknessImage);
	imagePainter.initFrom(m_pWgtThicknessView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtThicknessView->m_pThicknessImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	//m_pWgtThicknessView->m_pThicknessImage->fill(QColor(17,14,9));    //背景
	m_pWgtThicknessView->m_pThicknessImage->fill(QColor(103,103,105));    //背景//chenyuzhu add on 2014-11-4
	
	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);


	// 获取厚度偏差数据 
	int* nXThickValues = NULL;
	g_pPlateDraw->dynamicCall("go()");
	QString str = g_pPlateDraw->dynamicCall("GetMaxCha()").toString();
//	unsigned short nSize = (g_Info[nPlate].length+2)/3;   //g_IniParams.m_minxdistance  算网格
    unsigned short nSize = (g_Info[nPlate].length+4)/5;
	nXThickValues = new int[nSize];
	if(nXThickValues == NULL)
	{
		return;
	}
	memset(nXThickValues,0,sizeof(nXThickValues[0])*sizeof(int));
	int index = str.indexOf(QString("y"));
	QString strXCount = str.left(index);
	// 将字符串转换为数组
	unsigned short nXMax = ShadowStr2Array(strXCount, nXThickValues, nSize);
	// 变为5的整数倍
// 	unsigned short yu = nXMax%5;
// 	nXMax = (nXMax+5-yu);
	//画中间线
//	imagePainter.setPen(QPen(QColor(0,255,0),1.2,Qt::DashLine));//添加画笔
//	imagePainter.drawLine(QPoint(m_pWgtThicknessView->m_pThicknessImage->rect().left(),m_pWgtThicknessView->m_pThicknessImage->rect().height()/2),QPoint(m_pWgtThicknessView->m_pThicknessImage->rect().right(),m_pWgtThicknessView->m_pThicknessImage->rect().height()/2));
	// 画偏差点
	QColor clrRed(255,0,0);
	QColor clrGreen(0,255,0);
	QPoint ptOffset;
	for(int i=((m_nPlateYStart+4)/5); i<(m_nPlateYEnd/5); i++)
	{
		// 防止越界
		if (i>(nSize-1))
		{
			break;
		}
        //计算点坐标
		QList<double> tempList;
		tempList<<static_cast<double>(i)<<static_cast<double>(nXMax)<<static_cast<double>(nXThickValues[i]);
		QList<int> resultList;
		int tempFlag = CalculatePointForPannel(CALCULATE_THICKPOINT,tempList,resultList);
		if (-1 == tempFlag)
		{
			QMessageBox::information(this,"CalculatePointForPannel",tr("点计算失败"));
			return;
		}
		int x = resultList.at(0);
		int y = resultList.at(1);

		ptOffset.setX(x);
		ptOffset.setY(y);
		float cha = fabsf(nXThickValues[i]) - 1.0f;
		if (cha > 0.000001)
		{
			//				pDC->SetPixel(ptOffset,clrRed);
			//imagePainter.setPen(QPen(clrRed));
			imagePainter.setPen(QPen(QColor(255,0,0),1.2,Qt::DashLine));//添加画笔
			
			imagePainter.drawPoint(ptOffset);
		}
		else
		{
			//				pDC->SetPixel(ptOffset,clrGreen);
			//imagePainter.setPen(QPen(clrGreen));			
			imagePainter.setPen(QPen(QColor(0,255,0),1.2,Qt::DashLine));//添加画笔
			imagePainter.drawPoint(ptOffset);
		}
		
	}

	delete []nXThickValues;

	QPainter widgetPainter(m_pWgtThicknessView);
	widgetPainter.drawImage(QRect(46,1,m_pWgtThicknessView->rect().width()- 47,m_pWgtThicknessView->rect().height()- 40),*m_pWgtThicknessView->m_pThicknessImage,QRect(0,0,m_pWgtThicknessView->m_pThicknessImage->rect().width(),m_pWgtThicknessView->m_pThicknessImage->rect().height()));
}

void CCscanPanel::DrawFlawDC(int XSize,int YSize,QWidget *CurrentWidget,QImage *&CurrentImage)//此处为何用*&?? //因为引用的指针by chenyuzhu
{

	if(NULL != CurrentImage)
	{
		delete CurrentImage;
		CurrentImage = NULL;
	}
	CurrentImage = new QImage(XSize,YSize,QImage::Format_ARGB32_Premultiplied);
	if (NULL == CurrentImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
//	CurrentImage->fill(QColor(17,14,9));  //背景
	CurrentImage->fill(QColor(103,103,105));  //背景chenyuzhu add on 2014-11-4
	QPainter imagePainter(CurrentImage);
	imagePainter.initFrom(CurrentWidget);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	//设置QPainter准备画伤点图 
	imagePainter.setPen(QPen(QColor(0,255,0),2,Qt::SolidLine));
	QBrush brush(Qt::green,Qt::SolidPattern);
	imagePainter.setBrush(brush);
	//具体画图的过程
	DrawFlawDetail(imagePainter,XSize,YSize,0,g_Info[nPlate].length);

	//chenyuzhu add start on 2014-11-3
	float npen = (20.0 / float(g_Info[nPlate].width) * (float)YSize);
	imagePainter.setPen(QPen(QColor(0,255,255),1,Qt::SolidLine));
	float fTanTouY = (float)YSize - (float(m_fTanTouY) / float(g_Info[nPlate].width)) * (float)YSize;//chenyuzhu modify on 2014-11-10
	if(m_bTanTouDraw && *g_bInjPointPath)//chenyuzhu add on 2014-11-4
	{
		imagePainter.drawLine(QPoint(0,fTanTouY),QPoint(XSize,fTanTouY));
	}	
	//chenyuzhu add start on 2014-11-3
    
	QString name = CurrentWidget->objectName();

	if(name == QString("m_pWgtWholeView"))
	{
		imagePainter.setPen(QPen(Qt::green,1.5));
		imagePainter.setBrush(Qt::NoBrush);
		imagePainter.drawRect(QRect(QPoint(m_RegionRect.left()+2,m_RegionRect.top()+2),QPoint(m_RegionRect.right(),m_RegionRect.bottom()-2)));
	}
	imagePainter.end();

} 
//chenyuzhu add start on 2014-6-13
QColor CCscanPanel::CalcFillColor(const int& nvalue)
{	
	QColor color;
	int index = nvalue * 510 / int(g_Info[nPlate].height);
	
	if (index > 255)
	{
		int nColor = index - 255;
		color.setRgb(nColor, 0, 255 - nColor);
	}
	else
	{
		int nColor = index;
		color.setRgb(0, 255 - nColor, nColor);		
	}	
	return color;
}
//chenyuzhu add end on 2014-6-13

void CCscanPanel::DrawFlawDetail(QPainter& painter,int xSize,int ySize,const double& fStart,const double& fEnd)
{
	m_vHeaderRect.clear();   //清空List
    m_vHRect.clear(); //用来存入合并后的伤

    ////////////////////////////////////////////////////////////////////////////////////
    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = recognizer->GetObjectValue(index, "Length");
        //temp.zhishiLength = sqrt((temp.maxX-temp.minX)*(temp.maxX-temp.minX) + (temp.maxY-temp.minY)*(temp.maxY-temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        //零点在右下角
        int maxX = int(xSize - ((float(temp.minX - fStart)/ float(fEnd - fStart)) * xSize)) + 2;
        int minX = int(xSize - ((float(temp.maxX - fStart)/ float(fEnd - fStart)) * xSize)) - 2;
        int maxY = int (ySize - ((temp.minY) / float(g_Info[nPlate].width)) * ySize) + 2;
        int minY = int (ySize - ((temp.maxY) / float(g_Info[nPlate].width)) * ySize) - 2;
        temp.drawingPointRect = QRect(QPoint(minX, minY), QPoint(maxX, maxY));//计算选中伤点的选中矩形框
        float fColor = recognizer->GetObjectValue(index, "Rank");
        //chenyuzhu modify start on 2014-6-16 //根据面积过滤
        if(*g_bInjPointFilter)
        {
            if (*g_bFilterSmaller)
            {
                if(temp.mianShangArea < *g_nFilterSmallerNum)
                {
                    continue;
                }
            }
            if (*g_bFilterBiger)
            {
                if(temp.mianShangArea > *g_nFilterBigerNum)
                {
                    continue;
                }
            }
        }
        //chenyuzhu modify end on 2014-6-16


        //设置画笔
        QColor clrCurrent;
        QColor clrLostWave;
        clrCurrent.setRgb(0,255,0);
        clrLostWave.setRgb(255,255,255);
        if (g_nComboxColor == 0)
        {
            if (fColor < 0.5)
            {
                clrCurrent.setRgb(0,255,0);
            }
            if (fColor >= 0.5 && fColor < 1.0)
            {
                clrCurrent.setRgb(255,255,0);
            }
            if (fColor >= 1.0)
            {
                clrCurrent.setRgb(255,0,0);
            }
            else
            {
            }
        }
        else if (g_nComboxColor == 1)
        {
            if (temp.z == 0)
            {
                clrCurrent = clrLostWave;
            }
            else
            {
                clrCurrent = CalcFillColor(int(temp.z));

            }
        }
        painter.setPen(QPen(clrCurrent, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.setBrush(Qt::NoBrush);

        if ((PUTSRecognizer::DEFECT_DENSITY == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_LENGTH_NUM == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_DENSITY_NUM == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_DENSITY_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_DENSITY_EDGE == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_LENGTH_NUM_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (PUTSRecognizer::DEFECT_CITCLE_COMBINED == recognizer->GetObjectValue(index, "Type")))
        {
            painter.setPen(QPen(QColor(255, 255, 255), 1, Qt::DashLine, Qt::RoundCap));
           // continue;
        }
        if ((GBT_2970::DEFECT_COMBINED_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (GBT_2970::DEFECT_COMBINED_EDGE == recognizer->GetObjectValue(index, "Type")))
        {
            if(!m_bPointJoin)
            {
                continue;
            }
            painter.setPen(QPen(QColor(0,0,255), 1, Qt::DashLine, Qt::RoundCap));
            m_vHRect.append(temp);
        }
		else  if ((GBT_2970::DEFECT_NORMAL == recognizer->GetObjectValue(index, "Type")) ||
				(GBT_2970::DEFECT_EDGE == recognizer->GetObjectValue(index, "Type")))
        {
            m_vHeaderRect.append(temp);
        }

     //   QPolygonF poly = recognizer->GetObjectBoundary(index);
        QPolygon pannelpoly;
        foreach (QPointF pointf, poly)
        {
            QList<double> TempList;
            QList<int> Result;
            TempList << pointf.x() << pointf.y() << fStart << fEnd << (double)xSize << (double)ySize;
            int TempFlag = CalculatePointForPannel(CALCULATE_WHOLEPOINT, TempList, Result);
            if (-1 == TempFlag)
            {
                QMessageBox::information(this,"DrawFlawDetail/CalculatePointForPannel",tr("点计算失败"));
            }
            QPoint point;
            point.setX(Result.at(0));
            point.setY(Result.at(1));
            pannelpoly.append(point);
        }
        painter.drawPolygon(pannelpoly);

    ////////////////////////////////////////////////////////////////////////////////////
		//其他特效
		//		if (m_DlgPlateInfo.m_bDisplayCross)
		int nXMinPox = 0;
		int nXMaxPox = 0;
		int nYMinPox = 0;
		int nYMaxPox = 0; 
		if(m_bCentreCrosingLine)
		{
			//坐标转换
			if ( leftTop == m_ZeroPosFlag)
			{
                nXMinPox = static_cast<int>((temp.minY /g_Info[nPlate].width) * ySize);
                nXMaxPox = static_cast<int>((temp.maxY /g_Info[nPlate].width) * ySize);
                nYMinPox =  static_cast<int>(((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMaxPox =  static_cast<int>(((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else if( leftBottom == m_ZeroPosFlag)
			{
                nXMaxPox = static_cast<int>(ySize - (temp.minY /g_Info[nPlate].width) * ySize);
                nXMinPox = static_cast<int>(ySize - (temp.maxY /g_Info[nPlate].width) * ySize);
                nYMinPox =  static_cast<int>(((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMaxPox =  static_cast<int>(((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else if(rightTop == m_ZeroPosFlag)
			{
                nXMinPox = static_cast<int>((temp.minY /g_Info[nPlate].width) * ySize);
                nXMaxPox = static_cast<int>((temp.maxY /g_Info[nPlate].width) * ySize);
                nYMaxPox =  static_cast<int>(xSize - ((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMinPox =  static_cast<int>(xSize - ((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else if(rightBottom == m_ZeroPosFlag)
			{
                nXMaxPox = static_cast<int>(ySize - (temp.minY /g_Info[nPlate].width) * ySize);
                nXMinPox = static_cast<int>(ySize - (temp.maxY /g_Info[nPlate].width) * ySize);
                nYMaxPox =  static_cast<int>(xSize - ((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMinPox =  static_cast<int>(xSize - ((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else
			{
				QMessageBox::information(this,"DrawFlawDetail","m_nZeroPosFlag error");
				return;
			}		
			QPoint pointMin(nYMinPox,nXMinPox);
			QPoint pointMax(nYMaxPox,nXMaxPox);
			DrawCrossSymbol(painter,pointMin,pointMax,QColor(255,105,180));//十字线
		}
		//	if (m_DlgPlateInfo.m_bDisplaySquare)//每个小伤都画实线方框
		if(m_bInjPointRangeLine)
		{
			if ( leftTop == m_ZeroPosFlag)
			{
                nXMinPox = static_cast<int>((temp.minY /g_Info[nPlate].width) * ySize);
                nXMaxPox = static_cast<int>((temp.maxY /g_Info[nPlate].width) * ySize);
                nYMinPox =  static_cast<int>(((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMaxPox =  static_cast<int>(((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);

			}
			else if( leftBottom == m_ZeroPosFlag)
			{
                nXMaxPox = static_cast<int>(ySize - (temp.minY /g_Info[nPlate].width) * ySize);
                nXMinPox = static_cast<int>(ySize - (temp.maxY /g_Info[nPlate].width) * ySize);
                nYMinPox =  static_cast<int>(((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMaxPox =  static_cast<int>(((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else if(rightTop == m_ZeroPosFlag)
			{
                nXMinPox = static_cast<int>((temp.minY /g_Info[nPlate].width) * ySize);
                nXMaxPox = static_cast<int>((temp.maxY /g_Info[nPlate].width) * ySize);
                nYMaxPox = static_cast<int>(xSize - ((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMinPox = static_cast<int>(xSize - ((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else if(rightBottom == m_ZeroPosFlag)
			{
                nXMaxPox = static_cast<int>(ySize - (temp.minY /g_Info[nPlate].width) * ySize);
                nXMinPox = static_cast<int>(ySize - (temp.maxY /g_Info[nPlate].width) * ySize);
                nYMaxPox = static_cast<int>(xSize - ((temp.minX - fStart)/ (fEnd - fStart)) * xSize);
                nYMinPox = static_cast<int>(xSize - ((temp.maxX - fStart)/ (fEnd - fStart)) * xSize);
			}
			else
			{
				QMessageBox::information(this,"DrawFlawDetail","m_nZeroPosFlag error");
				return;
			}
			QPoint pointMin(nYMinPox,nXMinPox);
			QPoint pointMax(nYMaxPox,nXMaxPox);
			DrawSquareSymbol(painter,pointMin,pointMax,QColor(0,0,255));//实线框
		}
	}
	
	if (m_nDoubleClickedDoshRectID > -1 && !m_vHeaderRect.isEmpty())   //画选中伤点的白框
	{
		QRect temp = m_vHeaderRect[m_nDoubleClickedDoshRectID].drawingPointRect;
		painter.setPen(QPen(Qt::white,2,Qt::SolidLine));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(temp);

		//恢复原来的QPainter
		painter.setPen(QPen(QColor(0,255,0),2,Qt::SolidLine));
		QBrush brush(Qt::green,Qt::SolidPattern);
		painter.setBrush(brush);
	}

	//涉及到标准暂未实现
	// 	if (m_DlgPlateInfo.m_bDisplaySquare)
	// 	{
	// 		//普通面伤的虚框
	// 		DrawSumSquareSymbol(pDC,fStart,fEnd);
	// 		//最大面伤虚框
	// 		DrawMaxSquareSymbol(pDC,fStart,fEnd);
	// 	}
	//painter.end();
	
	//cheyuzhu modify start on 2014-7-25
		m_vHBiaoZhunRect.clear();
	//cheyuzhu add start on 2014-6-18
	
	if(m_bPointJoin)
	{
      /*  QVector<DLPointInfo> vHTempRect;
		DLPointInfo DLP;
		int njuli = *m_pDefectDistaneSmaller;
		//int nHedianshu;
		QList<int> dianlist;
		vHTempRect = m_vHeaderRect;
		m_vHRect.clear();
		int num = vHTempRect.size();
		while(num > 0)
		{	
			DLP=vHTempRect[0];
			vHTempRect.erase(vHTempRect.begin() + 0);
			num = vHTempRect.size();
			for (int j = 0; j < num; j++)
			{	//这里面的加9减9 是为了得到真实的矩形 因为在获得矩形坐标时是把矩形边界都扩充的9 chenyuzhu add on 2014-7-31 
				//chenyuzhu modify start on 2014-11-7
				double dDlpXmid = (DLP.minX + DLP.maxX) /2.0;
				double dDlpYmid = (DLP.minY + DLP.maxY) /2.0;
				double dmidX = (vHTempRect[j].minX + vHTempRect[j].maxX) /2.0;
				double dmidY = (vHTempRect[j].minY + vHTempRect[j].maxY) /2.0;
				double dDlpRX = fabs((DLP.maxX - DLP.minX) /2.0);
				double dDlpRY = fabs((DLP.maxY - DLP.minY) /2.0);
				double dRX = fabs((vHTempRect[j].maxX - vHTempRect[j].minX) /2.0);
				double dRY = fabs((vHTempRect[j].maxY - vHTempRect[j].minY) /2.0);
				//chenyuzhu modify end on 2014-11-7
				
				double nMaxX = njuli;
				if(nMaxX < DLP.zhishiLength)
				{
					nMaxX = DLP.zhishiLength;
				}
				if (nMaxX < vHTempRect[j].zhishiLength)
				{
					nMaxX = vHTempRect[j].zhishiLength;
				}
				bool bHeX = (fabs(dDlpXmid - dmidX) - fabs(dDlpRX + dRX))<nMaxX?true:false;
				bool bHeY = (fabs(dDlpYmid - dmidY) - fabs(dDlpRY + dRY))<nMaxX?true:false;
			//	bool bHeZ =	(fabs(DLP.z) - fabs(vHTempRect[j].z))<njuli?true:false;
				if (bHeX&&bHeY)
				{	
					DLP.maxX = DLP.maxX>vHTempRect[j].maxX?DLP.maxX:vHTempRect[j].maxX;
					DLP.minX = DLP.minX<vHTempRect[j].minX?DLP.minX:vHTempRect[j].minX;
					DLP.maxY = DLP.maxY>vHTempRect[j].maxY?DLP.maxY:vHTempRect[j].maxY;
					DLP.minY = DLP.minY<vHTempRect[j].minY?DLP.minY:vHTempRect[j].minY;
					DLP.z = vHTempRect[j].z;
					DLP.mianShangArea = DLP.mianShangArea + vHTempRect[j].mianShangArea;
					vHTempRect.erase(vHTempRect.begin() + j);
					j = -1;
					num = vHTempRect.size();
					//dianlist.append(j);
                }
							
				num = vHTempRect.size();
			}
			DLP.zhishiLength = sqrt((DLP.maxX - DLP.minX)*(DLP.maxX - DLP.minX) + (DLP.maxY - DLP.minY)*(DLP.maxY - DLP.minY));
			//chenyuzhu modify start on 2014-11-7
			//DLP.drawingPointRect = QRect(QPoint(((DLP.minX - fStart)/ (fEnd - fStart)) * xSize-2,(DLP.minY /g_Info[nPlate].width ) * ySize-2),
			//	QPoint(((DLP.maxX - fStart)/ (fEnd - fStart)) * xSize+1,(DLP.maxY /g_Info[nPlate].width ) * ySize+1));//在这里为合并后要画的矩形
			
			int maxX = int(xSize - ((float(DLP.minX - fStart)/ float(fEnd - fStart)) * xSize)) + 1;
			int minX = int(xSize - ((float(DLP.maxX - fStart)/ float(fEnd - fStart)) * xSize)) - 2;
			int maxY = int (ySize - ((DLP.minY) / float(g_Info[nPlate].width)) * ySize) + 1;
			int minY = int (ySize - ((DLP.maxY) / float(g_Info[nPlate].width)) * ySize) - 2; 
			DLP.drawingPointRect = QRect(QPoint(minX, minY), QPoint(maxX, maxY));//计算选中伤点的选中矩形框
			//chenyuzhu modify end on 2014-11-7
				m_vHRect.append(DLP);
		}			
		int nSize = m_vHRect.size();
		for (int i = 0; i < nSize; i++  )
		{
			painter.setPen(QPen(QColor(255,255,0),1,Qt::DashLine,Qt::RoundCap));
		//	painter.setPen(QPen(QColor(255,0,0),2,Qt::SolidLine));
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(m_vHRect[i].drawingPointRect);
		}
        */
		m_vHBiaoZhunRect = m_vHRect;
	}
	else
	{
		m_vHBiaoZhunRect = m_vHeaderRect;
	}
	
	int nSize = m_vHeaderRect.size();
	if (nSize > 0)
	{
		DLPointInfo MaxAera = m_vHeaderRect[0];
		DLPointInfo MaxLength = m_vHeaderRect[0];
		for (int i = 0; i < nSize-1; i++)
		{
			if(MaxAera.mianShangArea < m_vHeaderRect[i+1].mianShangArea)
			{
				MaxAera = m_vHeaderRect[i+1];
			}
			if(MaxLength.zhishiLength < m_vHeaderRect[i+1].zhishiLength)
			{
				MaxLength = m_vHeaderRect[i+1];
			}
		}
		m_dMaxArea = MaxAera.mianShangArea;
		m_dMaxLength = MaxLength.zhishiLength;
		//painter.setPen(QPen(QColor(255,255,0),2,Qt::DashLine,Qt::RoundCap));
		
        painter.setPen(QPen(QColor(255, 255, 255), 1, Qt::DashLine, Qt::RoundCap));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(MaxAera.drawingPointRect);
		double dright = MaxAera.drawingPointRect.right();
		double dtop = MaxAera.drawingPointRect.top();
		double dleft = MaxAera.drawingPointRect.left();
		double dbottom = MaxAera.drawingPointRect.bottom();
		double text_x = 0;
		double text_y = 0;
		if(xSize - dright < 200 )
		{
			text_x = dleft - 40;
		}
		else if(dleft - 0 < 200 )
		{
			text_x = dright + 5;
		}
		else
		{
			text_x = (dleft + dright) /2;
		}

		if(dtop - 0 < 200 )
		{
			text_y = dbottom + 13;
		}
		else if(ySize - dbottom < 200 )
		{
			text_y = dtop - 5;
		}
		else
		{
			text_y = (dtop + dbottom) /2;
		}
		painter.setPen(QPen(QColor(255,255,255),1,Qt::SolidLine));
		painter.drawText(text_x, text_y, "MaxAera");
		//painter.setPen(QPen(QColor(255,255,0),2,Qt::DashLine,Qt::RoundCap));
		
        painter.setPen(QPen(QColor(255, 255, 255), 1, Qt::DashLine, Qt::RoundCap));
		painter.setBrush(Qt::NoBrush);
		painter.drawRect(MaxLength.drawingPointRect);
		dright = MaxLength.drawingPointRect.right();
        dtop = MaxLength.drawingPointRect.top();
		dleft = MaxLength.drawingPointRect.left();
		dbottom = MaxLength.drawingPointRect.bottom();
		if(xSize - dright < 200 )
		{
			text_x = dleft - 40;
		}
		else if(dleft - 0 < 200 )
		{
			text_x = dright + 5;
		}
		else
		{
			text_x = (dleft + dright) / 2;
		}

		if(dtop - 0 < 200 )
		{
			text_y = dbottom + 13;
		}
		else if(ySize - dbottom < 200 )
		{
			text_y = dtop - 5;
		}
		else
		{
			text_y = (dtop + dbottom) / 2;
		}
		painter.setPen(QPen(QColor(255,255,255),1,Qt::SolidLine));
		painter.drawText(text_x, text_y, "MaxLength");
	}
	//cheyuzhu modify end on 2014-7-25
	//chenyuzhu add end on 2014-6-18

	//chenyuzhu add start on 2014-6-23
	/*float npen = (20.0 / float(g_Info[nPlate].width) * (float)ySize);
	painter.setPen(QPen(QColor(0,255,255),1,Qt::SolidLine));
	float fTanTouY = (float)ySize - (float(m_fTanTouY) / float(g_Info[nPlate].width)) * (float)ySize;
	if(m_bTanTouDraw)
	{
		painter.drawLine(QPoint(0,fTanTouY),QPoint(xSize,fTanTouY));
	}	*/
	//chenyuzhu add start on 2014-6-23	
	emit SignalToDetectListDlg();  //通知更新Model
}

void CCscanPanel::OnCscanDrawDashRect(int id)  //响应信号通知Cscan画选中虚框
{
	m_nDoubleClickedDoshRectID = id;
	update();  //更新界面
}

void CCscanPanel::DrawCrossSymbol(QPainter& painter,QPoint& pointMin,QPoint& pointMax,const QColor& color)
{
	QPen pen(color,1.5,Qt::SolidLine);
	painter.setPen(pen);
	int nOffset = 10;
	int nOffset2 = nOffset * g_Info[nPlate].length / g_Info[nPlate].width / 4;

	//算出伤的中心点坐标
	QPoint point;//画十字线时用
	point.setX(static_cast<int>((pointMin.x() + pointMax.x())/2));
	point.setY(static_cast<int>((pointMin.y() + pointMax.y())/2));

	painter.drawLine(QPoint(point.x(),pointMin.y() - nOffset2),QPoint(point.x(),pointMax.y() + nOffset2));
	painter.drawLine(QPoint(pointMin.x() - nOffset,point.y()),QPoint(pointMax.x() + nOffset,point.y()));

}

void CCscanPanel::DrawSquareSymbol(QPainter& painter,QPoint& pointMin,QPoint& pointMax, const QColor& color)
{
	QPen pen(color,1.5,Qt::SolidLine);
	painter.setPen(pen);
	int offeset = 5;
	painter.drawLine(QPoint(pointMin.x() - offeset,pointMin.y() - offeset),QPoint(pointMax.x() + offeset,pointMin.y() - offeset));
	painter.drawLine(QPoint(pointMin.x() - offeset,pointMax.y() + offeset),QPoint(pointMax.x() + offeset,pointMax.y() + offeset));
	painter.drawLine(QPoint(pointMin.x() - offeset,pointMin.y() - offeset),QPoint(pointMin.x() - offeset,pointMax.y() + offeset));
	painter.drawLine(QPoint(pointMax.x() + offeset,pointMin.y() - offeset),QPoint(pointMax.x() + offeset,pointMax.y() + offeset));	
}

unsigned short CCscanPanel::ShadowStr2Array(QString& str, int* pArray, const unsigned short &nSize)		// 将投影数据字符串转换为数组
{
	//	ASSERT(pArray!=NULL);
	if(NULL == pArray)
	{
		return 0;
	}
	int* tmpArray = pArray;
	//	int nIndex = str.find("=");
	int nIndex = str.indexOf(QString("="));
	int nCount = str.length() - nIndex - 1;
	str = str.right(nCount);
	QString subStr; 
	unsigned short nMaxNum =  6;
	for(int i = 0; i<nSize ;i++)
	{
		//		nIndex = str.find(",");
		nIndex = str.indexOf(QString(","));
		if (nIndex == -1)
			return nMaxNum;
		subStr = str.left(nIndex);
		tmpArray[i] = subStr.toInt();

		if (tmpArray[i]>nMaxNum)
		{
			nMaxNum = tmpArray[i];
		}

		nCount = str.length() - nIndex - 1;
		str = str.right(nCount);
	}
	return nMaxNum;
}

//伤点数组添加伤点数据
void CCscanPanel::OnAddFlawData(CH_FLAWDATA* pData)  //更新m_flowStore数据，并画图
{
	CH_FLAWDATA tempData = *pData;
	short Num = pData->CH_NO;

	g_ItemMutex.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	qint8 tmpCommand = g_ItemValue.dwPLC2WSCommand;
	g_ItemMutex.unlock();
	// 计算伤点在局部钢板范围内的显示位置
	float fX = 0.0f;
	float fY = 0.0f;
	DeltaX dx;
	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
	//dx.m_fNeiTan3DeltaX = dx.m_fNeiTan4DeltaX = g_IniParams.m_fNeitan34_FreeEdge_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;

	bool bRet = CalculateFlawXY(tempData, fX, fY, dx);
	m_FlawStore.append(tempData);

	// 若为边探2
	unsigned short nH8 = (tempData.CH_X >> 24) & 0xFF;
	if (tempData.CH_NO >= 2000 && tempData.CH_NO < 3000)	// 边探2-- 非参考边
	{	
		if (nH8 == 1)	// 头探
		{
			m_TouTanFlawList.append(tempData);
		}
		else if (nH8 == 2)	// 边探2
		{
		}
		else if (nH8 == 3)	// 尾探
		{
			m_WeiTanFlawList.append(tempData);
		}
		else
		{
			return;
		}
	}
	m_nInvalidEnd++;//计算有多少个点
	OnAddFlawDataToWhole(pData);
	update();
}

void CCscanPanel::OnAddFlawDataToWhole(CH_FLAWDATA* pData)  //更新m_FlawCoordinateArray数据，并画图
{
	CH_FLAWDATA tempData = *pData;
	delete pData;
	pData = NULL;

	g_ItemMutex.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	g_ItemMutex.unlock();

	float fX = 0.0f;
	float fY = 0.0f;    

	DeltaX dx;
 	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
 	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
 	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
 	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
 	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
 	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
 	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;

	bool bRet = CalculateFlawXY(tempData, fX, fY, dx);
	if (!bRet)
	{
		return;
	}
	//chenyuzhu modify start on 2014-10-29
//	int x = int((float(m_pWgtWholeView->rect().width())/ float(g_Info[nPlate].width)) * fY) + m_pWgtWholeView->rect().left();
//	int y = int((float(m_pWgtWholeView->rect().height())/ float(g_Info[nPlate].length)) * fX) + m_pWgtWholeView->rect().top();
	
	int x = m_pWgtWholeView->width() - int((float(m_pWgtWholeView->width())/ float(g_Info[nPlate].length)) * fX);
	int y = m_pWgtWholeView->height() - int((float(m_pWgtWholeView->height())/ float(g_Info[nPlate].width)) * fY);
	//chenyuzhu modify end on 2014-10-29

	// 将显示位置加入数组或链表
	QPoint point(x,y); //计算完后的坐标
	Flaw_Coordinate_Info info;
	info.point = point;
	info.DETECT_STATE = tempData.DETECT_STATE;
	m_FlawCoordinateVector.append(info);
}

void CCscanPanel::UpdateFlawDataList()    //更新伤点链表
{
	m_FlawList.clear();
 	g_ItemCS.lock();
 	float fRefBody_Start = g_ItemValue.fRefBody_Start;
 	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
 	g_ItemCS.unlock();

	// 检索m_FlawStore
	float fX = 0.0f;
	float fY = 0.0f;

	DeltaX dx;
 	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
 	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
 	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
 	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
 	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
 	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
 	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;

 	for (int i = 0; i < m_nInvalidEnd; i++)
 	{
 		bool bRet = CalculateFlawXY(m_FlawStore[i], fX, fY, dx);
 		if (!bRet)
 		{
 			continue;
 		}
 
 		if (fX >= m_nPlateYStart && fX < m_nPlateYEnd)
 		{
 			m_FlawList.append(m_FlawStore[i]);
 		}
 
 	}

}
// 画局部钢板区域
void CCscanPanel::DrawPlateFlawSection()
{
	UpdateFlawDataList();
	
	// Draw Flaws
	int nWidth;
	if (g_Info[nPlate].width <= 0)
	{
		nWidth = 3300;
	}
	else
	{
		nWidth = g_Info[nPlate].width;
	}
	g_ItemCS.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	g_ItemCS.unlock();

	// 循环
	float fX = 0.0f;
	float fY = 0.0f;
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


	//创建QImage
	if(NULL != m_pWgtCscanDetectView->m_pImage)
	{
		delete m_pWgtCscanDetectView->m_pImage;
		m_pWgtCscanDetectView->m_pImage = NULL;
	}


	m_pWgtCscanDetectView->m_nXSize = m_pWgtCscanDetectView->rect().width();
	m_pWgtCscanDetectView->m_nYSize = m_pWgtCscanDetectView->rect().height();

	m_pWgtCscanDetectView->m_pImage = new QImage( m_pWgtCscanDetectView->m_nXSize,m_pWgtCscanDetectView->m_nYSize,QImage::Format_ARGB32_Premultiplied);//chenyuzhu modify on 2014-10-30
	QPainter imagePainter(m_pWgtCscanDetectView->m_pImage);
	imagePainter.initFrom(this);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtCscanDetectView->m_pImage)
	{
		QMessageBox::information(this,tr("DrawImmediateImage"),tr("QImage创建失败"));
		return;
	}
	//m_pWgtCscanDetectView->m_pImage->fill(QColor(17,14,9));  //背景
	m_pWgtCscanDetectView->m_pImage->fill(QColor(103,103,105));  //背景chenyuzhu add on 2014-11-4
	imagePainter.setPen(QPen(QColor(255,255,255),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));

	for (UINT i = 0; i < m_FlawList.count(); ++i)
	{
		CH_FLAWDATA tempFlaw = m_FlawList[i];

		BOOL bRet = CalculateFlawXY(tempFlaw, fX, fY, dx);
		if (!bRet)
		{
			continue;
		}
		//chenyuzhu modify start on 2014-10-29
		// 计算伤点x坐标在rect的位置
		int y = m_pWgtCscanDetectView->rect().height() - int(float(m_pWgtCscanDetectView->rect().height()) / float(nWidth) * fY);
		// 计算伤点y坐标在rect的位置
		int x = m_pWgtCscanDetectView->rect().width() - int((fX - *(m_pWgtCscanDetectView->m_pPlateYStart)) * (float(m_pWgtCscanDetectView->rect().width())/float(*(m_pWgtCscanDetectView->m_pPlateYEnd) - *(m_pWgtCscanDetectView->m_pPlateYStart))));
		
		// 计算伤点x坐标在rect的位置
//		int x = int(float(m_pWgtCscanDetectView->rect().width()) / float(nWidth) * fY) + m_pWgtCscanDetectView->rect().left();
		// 计算伤点y坐标在rect的位置
//		int y = int((fX - *(m_pWgtCscanDetectView->m_pPlateYStart)) * (float(m_pWgtCscanDetectView->rect().height())/float(*(m_pWgtCscanDetectView->m_pPlateYEnd) - *(m_pWgtCscanDetectView->m_pPlateYStart)))) + m_pWgtCscanDetectView->rect().top();
		//chenyuzhu modify end on 2014-10-29

		
		// 绘制红点
		QRect tempRect(QPoint(x-3,y-3),QPoint(x+3,y+3));
		if (tempFlaw.DETECT_STATE == 0)
		{
			QBrush brush(QColor(255,255,255),Qt::SolidPattern);
			imagePainter.setBrush(brush);
		}
		else if (tempFlaw.DETECT_STATE == 1)
		{
			QBrush brush(QColor(255, 0,0),Qt::SolidPattern);
			imagePainter.setBrush(brush);
		}
		imagePainter.drawEllipse(tempRect);
	}
	imagePainter.end();
	QPainter widgetPainter(m_pWgtCscanDetectView);
	widgetPainter.drawImage(QRect(46,39,m_pWgtCscanDetectView->rect().width() - 47,m_pWgtCscanDetectView->rect().height() - 40),*m_pWgtCscanDetectView->m_pImage,QRect(0,0,m_pWgtCscanDetectView->m_pImage->rect().width(),m_pWgtCscanDetectView->m_pImage->rect().height()));//chenyuzhu modif on 2014-7-21
}

void CCscanPanel::DrawRunningDetectDetial() //探测时的局部视图 
{
	//创建QImage
	if(NULL != m_pWgtCscanDetectView->m_pImage)
	{
		delete  m_pWgtCscanDetectView->m_pImage;
		 m_pWgtCscanDetectView->m_pImage = NULL;
	}
	m_pWgtCscanDetectView->m_pImage = new QImage( m_pWgtCscanDetectView->rect().width(),m_pWgtCscanDetectView->rect().height(),QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pWgtCscanDetectView->m_pImage);
	imagePainter.initFrom(m_pWgtCscanDetectView);
	imagePainter.setRenderHint(QPainter::Antialiasing,true);
	if (NULL == m_pWgtCscanDetectView->m_pImage)
	{
		QMessageBox::information(this,tr("DrawFlawDC"),tr("QImage创建失败"));
		return;
	}
	m_pWgtCscanDetectView->m_pImage->fill(QColor(17,14,9));  //背景
// 	imagePainter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
// 	QBrush brush(Qt::green,Qt::SolidPattern);
// 	imagePainter.setBrush(brush);

	int nWidth;
	if (g_Info[nPlate].width <= 0)
	{
		nWidth = 3300;
	}
	else
		nWidth = g_Info[nPlate].width;
	//		DrawPlateSection(pDC, nWidth);  //原vc中画背景和一些标尺
	//准备画图
	QBrush RedBrush(QColor(255, 0,0));
	QBrush BlackBrush(QColor(0,0,0));

	g_ItemMutex.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	g_ItemMutex.unlock();
	
	// 循环
	float fX = 0.0f;
	float fY = 0.0f;
	DeltaX dx;
 	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
 	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
 	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
 	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
 	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
 	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
 	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;

	QList<CH_FLAWDATA>::iterator i = m_FlawList.begin();
	while (i != m_FlawList.end())
	{
		CH_FLAWDATA tempFlaw = *i;

		bool bRet = CalculateFlawXY(tempFlaw, fX, fY, dx);
		if (!bRet)
		{
			i++;
			continue;
		}
		// 计算伤点x坐标在rect的位置
		int x = int(float(m_pWgtCscanDetectView->m_pImage->rect().width()) / float(nWidth) * fY) + m_pWgtCscanDetectView->m_pImage->rect().left();
		// 计算伤点y坐标在rect的位置
		int y = int((fX - m_nPlateYStart) * (float(m_pWgtCscanDetectView->m_pImage->rect().height())/float(m_nPlateYEnd - m_nPlateYStart))) + m_pWgtCscanDetectView->m_pImage->rect().top();
		// 绘制红点
		// 		QRect tempRect(x-3,y-3,x+3,y+3);
		// 		if (tempFlaw.DETECT_STATE == 0)
		// 		{
		// 			pDC->SelectObject(&BlackBrush);
		// 		}
		// 		else if (tempFlaw.DETECT_STATE == 1)
		// 		{
		// 			pDC->SelectObject(&RedBrush);
		// 		}
		// 		else
		// 		{
		// 
		// 		}
		//		pDC->Ellipse(tempRect);

	}
	//	pDC->SelectObject(pOldBrush);

}
//鼠标左键全局视图选中区域执行槽函数
void CCscanPanel::OnChangeSelectRect(int x,int y,int width,int height)
{
	int xx = x * m_pWgtWholeView->m_nXSize/width;
	int yy = y * m_pWgtWholeView->m_nYSize/height;

// 	int nMinY = m_RegionRect.Height()/2 + m_plateRect.top;
// 	int nMaxY = m_plateRect.bottom - (m_RegionRect.Height()/2);
	int nMinX = m_RegionRect.width()/2 +0;
	int nMaxX = m_pWgtWholeView->m_nXSize-(m_RegionRect.width()/2);
	if (xx < nMinX)
		xx = nMinX;
	if (xx > nMaxX)
		xx = nMaxX;
	int distance = xx - m_RegionRect.center().x();
	m_RegionRect.setLeft(m_RegionRect.left()+distance);
	m_RegionRect.setRight(m_RegionRect.right()+distance);

	//更新m_plateYStart,m_plateYEnd;
    if (m_ZeroPosFlag<3)// chenyuzhu modify on 2014-5-27 修改原因: 原来是m_ZeroPosFlag<2 ，当读取的m_ZeroPosFlag=2时也就是leftBottom；图形选中对应不上，改为3正常。
    {
		m_nPlateYStart = (m_RegionRect.left()-0)/float(m_pWgtWholeView->m_nXSize)*g_Info[nPlate].length;
		m_nPlateYEnd = int(float(m_RegionRect.right()-0)/float(m_pWgtWholeView->m_nXSize)*g_Info[nPlate].length)+30;
    }
	else if(m_ZeroPosFlag<5)
	{
		m_nPlateYStart = (m_pWgtWholeView->m_nXSize - m_RegionRect.right())/float(m_pWgtWholeView->m_nXSize)*g_Info[nPlate].length;
		m_nPlateYEnd = int(float(m_pWgtWholeView->m_nXSize - m_RegionRect.left())/float(m_pWgtWholeView->m_nXSize)*g_Info[nPlate].length);
	}
	else
	{
		QMessageBox::information(this,"OnChangeSelectRect","m_nZeroPosFlag error");
		return;
	}
	//取消放大效果
	m_pWgtCscanDetectView->m_nSectionFlag = 0;  //放大计数器置零
	m_pWgtCscanDetectView->m_nSectionFlag = false;
	m_pWgtCscanDetectView->m_CurrentWholeSection = QRect(0,0,0,0);
	m_pWgtFrontView->m_bBoostFlag = false;
	m_pWgtSideView->m_bBoostFlag = false;
	m_pWgtWholeView->m_bBoostFlag = false;

	if( NULL != m_pWgtCscanDetectView->m_pSectionImage)
	{
		delete m_pWgtCscanDetectView->m_pSectionImage;
		m_pWgtCscanDetectView->m_pSectionImage = NULL;
	}
	//更新正视图中的长度指针
	m_pWgtFrontView->m_nPlateYStart =  &m_nPlateYStart;
	m_pWgtFrontView->m_nPlateYEnd = &m_nPlateYEnd;
	//更新侧视图中的长度指针
	m_pWgtSideView->m_nPlateYStart =  &m_nPlateYStart;
	m_pWgtSideView->m_nPlateYEnd = &m_nPlateYEnd;
	update();	
}
int CCscanPanel::CalculatePointForPannel(int Flag,QList<double>& argList,QList<int>& ResultList)  //给视图计算坐标的方法
{
	switch (Flag)
	{
	case CALCULATE_WHOLEPOINT :  //全局视图 和 全局视图
		{
			double pointX = argList.at(0);
			double pointY = argList.at(1);
			double fStart = argList.at(2);
			double fEnd = argList.at(3);
			int    xSize = static_cast<int>(argList.at(4));
			int    ySize = static_cast<int>(argList.at(5));
			ResultList.clear();
			if(leftTop == m_ZeroPosFlag)   //零点在左上角
			{
				int x = int((float(pointX - fStart)/ float(fEnd - fStart)) * xSize);
				int y = int((float(pointY) / float(g_Info[nPlate].width)) * ySize);
				ResultList.append(x);
				ResultList.append(y);
				return 0;
            }
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int x = int((float(pointX - fStart)/ float(fEnd - fStart)) * xSize);
				int y = int (ySize - ((pointY) / float(g_Info[nPlate].width)) * ySize);
				ResultList.append(x);
				ResultList.append(y);
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag) //零点在右上角
			{
                int x = int(xSize - ((float(pointX - fStart)/ float(fEnd - fStart)) * xSize));
		 		int y = int((float(pointY) / float(g_Info[nPlate].width)) * ySize);
				ResultList.append(x);
				ResultList.append(y);
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int x = int (xSize - ((float(pointX - fStart)/ float(fEnd - fStart)) * xSize));
				int y = int (ySize - ((pointY) / float(g_Info[nPlate].width)) * ySize);
				ResultList.append(x);
				ResultList.append(y);
				return 0;
			}
			else
			{
				return -1;
			}			
		}
	case CALCULATE_FRONTPOINT : //正视图
		{

			double pMinX = argList.at(0);
			double pMaxX = argList.at(1);
			double pZ = argList.at(2);
			ResultList.clear();

			if(leftTop == m_ZeroPosFlag)   //零点在左上角
			{

				int temp = g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize;
				int nXMinPox = static_cast<int>((pMinX- m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int nXMaxPox = static_cast<int>((pMaxX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int y = static_cast<int>(pZ / g_Info[nPlate].height * m_pWgtFrontView->m_pFrontImage->height());				
				ResultList<<nXMinPox<<nXMaxPox<<y;
				return 0;
			}
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int nXMinPox = static_cast<int>((pMinX- m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int nXMaxPox = static_cast<int>((pMaxX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int y = static_cast<int>(m_pWgtFrontView->m_pFrontImage->height() - pZ / g_Info[nPlate].height * m_pWgtFrontView->m_pFrontImage->height());
				ResultList<<nXMinPox<<nXMaxPox<<y;
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag)//零点在右上角
			{
				int nXMinPox = static_cast<int>(m_pWgtFrontView->m_pFrontImage->width() - (pMinX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
		 		int nXMaxPox = static_cast<int>(m_pWgtFrontView->m_pFrontImage->width() - (pMaxX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int y = static_cast<int>(pZ / g_Info[nPlate].height * m_pWgtFrontView->m_pFrontImage->height());
				ResultList<<nXMinPox<<nXMaxPox<<y;
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int nXMinPox = static_cast<int>(m_pWgtFrontView->m_pFrontImage->width() - (pMinX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int nXMaxPox = static_cast<int>(m_pWgtFrontView->m_pFrontImage->width() - (pMaxX - m_nPlateYStart) / (g_Info[nPlate].length * m_RegionRect.width()/m_pWgtWholeView->m_nXSize)*m_pWgtFrontView->m_pFrontImage->width());
				int y = m_pWgtFrontView->m_pFrontImage->height() - static_cast<int>(m_pWgtFrontView->m_pFrontImage->height() - pZ / g_Info[nPlate].height * m_pWgtFrontView->m_pFrontImage->height()); //chenyuzhu modify on 2014-10-28
				ResultList<<nXMinPox<<nXMaxPox<<y;
				return 0;
			}
			else
			{
				return -1;
			}
		}
	case CALCULATE_SIDEPOINT  : //侧视图
		{
			double pMinY = argList.at(0);
			double pMaxY = argList.at(1);
			double pZ = argList.at(2);
			ResultList.clear();
			if(leftTop == m_ZeroPosFlag)   //零点在左上角
			{
				int nYMinPox = static_cast<int>(pMinY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);
				int nYMaxPox = static_cast<int>(pMaxY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);	
				int x = static_cast<int>(pZ / g_Info[nPlate].height * m_pWgtSideView->m_nXSize);		
				ResultList<<x<<nYMinPox<<nYMaxPox;
				return 0;
			}
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int x = static_cast<int>(pZ / g_Info[nPlate].height * m_pWgtSideView->m_nXSize);
			 	int nYMinPox = static_cast<int>(m_pWgtSideView->m_nYSize - pMinY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);
		     	int nYMaxPox = static_cast<int>(m_pWgtSideView->m_nYSize - pMaxY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);	
				ResultList<<x<<nYMinPox<<nYMaxPox;
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag) //零点在右上角
			{
				int x = static_cast<int>(m_pWgtSideView->m_nXSize - pZ / g_Info[nPlate].height * m_pWgtSideView->m_nXSize);
				int nYMinPox = static_cast<int>(pMinY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);
				int nYMaxPox = static_cast<int>(pMaxY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);
				ResultList<<x<<nYMinPox<<nYMaxPox;
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int x = static_cast<int>(m_pWgtSideView->m_nXSize - pZ / g_Info[nPlate].height * m_pWgtSideView->m_nXSize);//chenyuzhu modify on 2014-10-28
				int nYMinPox = static_cast<int>(m_pWgtSideView->m_nYSize - pMinY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);
				int nYMaxPox = static_cast<int>(m_pWgtSideView->m_nYSize - pMaxY /g_Info[nPlate].width * m_pWgtSideView->m_nYSize);	
				ResultList<<x<<nYMinPox<<nYMaxPox;
				return 0;
			}				
			else
			{
				return -1;
			}
		}
	case CALCULATE_THICKPOINT : //厚度图
		{
			int i = static_cast<int>(argList.at(0));
			unsigned short nXMax = static_cast<unsigned short>(argList.at(1));
			int nXThickValues = static_cast<int>(argList.at(2));
			ResultList.clear();

			if(leftTop == m_ZeroPosFlag)  //零点在左上角
			{
				int x = int(float(i*5 - m_nPlateYStart)/float(m_nPlateYEnd - m_nPlateYStart)*(m_pWgtThicknessView->m_pThicknessImage->rect().width()));
				int y = int(float(nXThickValues+nXMax)/float(nXMax*2) * (m_pWgtThicknessView->m_pThicknessImage->rect().height()));
				ResultList<<x<<y;
				return 0;
			}
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int x = int(float(i*5 - m_nPlateYStart)/float(m_nPlateYEnd - m_nPlateYStart)*(m_pWgtThicknessView->m_pThicknessImage->rect().width()));
				int y = int(m_pWgtThicknessView->m_pThicknessImage->rect().height() - float(nXThickValues+nXMax)/float(nXMax*2) * (m_pWgtThicknessView->m_pThicknessImage->rect().height()));                                                                                                                                                                                                                                                                               
				ResultList<<x<<y;
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag) //零点在右上角
			{
				int x = int(m_pWgtThicknessView->m_pThicknessImage->rect().width() - float(i*5 - m_nPlateYStart)/float(m_nPlateYEnd - m_nPlateYStart)*(m_pWgtThicknessView->m_pThicknessImage->rect().width()));
				int y = int(float(nXThickValues+nXMax)/float(nXMax*2) * (m_pWgtThicknessView->m_pThicknessImage->rect().height()));
				ResultList<<x<<y;
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int x = int(m_pWgtThicknessView->m_pThicknessImage->rect().width() - float(i*5 - m_nPlateYStart)/float(m_nPlateYEnd - m_nPlateYStart)*(m_pWgtThicknessView->m_pThicknessImage->rect().width()));
				int y = int(m_pWgtThicknessView->m_pThicknessImage->rect().height() - float(nXThickValues+nXMax)/float(nXMax*2) * (m_pWgtThicknessView->m_pThicknessImage->rect().height()));
				ResultList<<x<<y;
				return 0;
			}
			else
			{
				return -1;
			}
		}
	case CALCULATE_XPOINT: //X投影
		{
			float xStep =static_cast<float>( argList.at(0));
			float yStep =static_cast<float>( argList.at(1));
			int i =static_cast<int>( argList.at(2));
			int Yarray =static_cast<int>( argList.at(3));
			ResultList.clear();
			if(leftTop == m_ZeroPosFlag)   //零点在左上角
			{
			 	int x1 = m_pWgtFrontView->m_pFrontImage->rect().left()+i*xStep;
			 	int y1 = m_pWgtFrontView->m_pFrontImage->rect().top();
				int x2 = x1;
				int y2 = m_pWgtFrontView->m_pFrontImage->rect().top()+Yarray*xStep;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int x1 = m_pWgtFrontView->m_pFrontImage->rect().left()+i*xStep;
				int y1 = m_pWgtFrontView->m_pFrontImage->rect().bottom();
				int x2 = x1;
				int y2 = m_pWgtFrontView->m_pFrontImage->rect().bottom()- Yarray*xStep;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag) //零点在右上角
			{
				int x1 = m_pWgtFrontView->m_pFrontImage->rect().right() - i*xStep;
				int y1 = m_pWgtFrontView->m_pFrontImage->rect().top();
				int x2 = x1;
				int y2 = m_pWgtFrontView->m_pFrontImage->rect().top()+Yarray*xStep;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int x1 = m_pWgtFrontView->m_pFrontImage->rect().right() - i*xStep;
				int y1 = m_pWgtFrontView->m_pFrontImage->rect().bottom();
				int x2 = x1;
				int y2 = m_pWgtFrontView->m_pFrontImage->rect().bottom()- Yarray*xStep;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else
			{
				return -1;
			}
		}
	case CALCULATE_YPOINT: //Y投影
		{

			float xStep =static_cast<float>( argList.at(0));
			float yStep =static_cast<float>( argList.at(1));
			int i =static_cast<int>( argList.at(2));
			int Xarray =static_cast<int>( argList.at(3));
			ResultList.clear();
			if(leftTop == m_ZeroPosFlag)   //零点在左上角
			{
				int x1 = m_pWgtSideView->m_pSideImage->rect().left();
				int y1 = m_pWgtSideView->m_pSideImage->rect().top()+i*yStep-(yStep*0.5);
				int x2 = m_pWgtSideView->m_pSideImage->rect().left()+Xarray*xStep;
				int y2 = y1;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else if(leftBottom == m_ZeroPosFlag)  //零点在左下角
			{
				int x1 = m_pWgtSideView->m_pSideImage->rect().left();
				int y1 = m_pWgtSideView->m_pSideImage->rect().bottom() - i*yStep-(yStep*0.5);
				int x2 = m_pWgtSideView->m_pSideImage->rect().left()+Xarray*xStep;
				int y2 = y1;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;		
			}
			else if(rightTop == m_ZeroPosFlag) //零点在右上角
			{
				int x1 = m_pWgtSideView->m_pSideImage->rect().right();
				int y1 = m_pWgtSideView->m_pSideImage->rect().top()+i*yStep-(yStep*0.5);
				int x2 = m_pWgtSideView->m_pSideImage->rect().right()- Xarray*xStep;
				int y2 = y1;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else if(rightBottom == m_ZeroPosFlag) //零点在右下角
			{
				int x1 = m_pWgtSideView->m_pSideImage->rect().right();
				int y1 = m_pWgtSideView->m_pSideImage->rect().bottom() - i*yStep-(yStep*0.5);
				int x2 = m_pWgtSideView->m_pSideImage->rect().right()- Xarray*xStep;
				int y2 = y1;
				ResultList<<x1<<y1<<x2<<y2;
				return 0;
			}
			else 
			{
				return -1;
			}
		}
	default:
	{
		return -1;
	}
    }
}


/** 
* 左侧边缘视图
* @since     1.0
*/
CScanLeftWidget::CScanLeftWidget(QWidget *parent):QWidget(parent)
{
	//设置宽度不能伸展，高度能伸展
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(36, 36));
	//setMaximumSize(QSize(36, 437));
	setStyleSheet(QString::fromUtf8("background-color: rgb(17, 14, 9);"));	
	//chenyuzhu add start on 2014-8-3
	m_pImage = NULL;
	m_bBoostFlag = false;
	//chenyuzhu add end on 2014-8-3
}
CScanLeftWidget::~CScanLeftWidget(void)
{
}

/** 
* 右侧边缘视图
* @since     1.0
*/
CScanRightWidget::CScanRightWidget(QWidget *parent):QWidget(parent)
{
	//设置宽度不能伸展，高度能伸展
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(36, 36));
	//setMaximumSize(QSize(36, 437));
	setStyleSheet(QString::fromUtf8("background-color: rgb(17, 14, 9);"));	
	//chenyuzhu add start on 2014-8-3
	m_pImage = NULL;
	m_bBoostFlag = false;
	//chenyuzhu add end on 2014-8-3
}
CScanRightWidget::~CScanRightWidget(void)
{
}

/** 
* 上侧边缘视图
* @since     1.0
*/
CScanTopWidget::CScanTopWidget(QWidget *parent):QWidget(parent)
{
	//设置宽度能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(130, 36));
	//setMaximumSize(QSize(1331, 36));
	setStyleSheet(QString::fromUtf8("background-color: rgb(17, 14, 9);"));	
	//chenyuzhu add start on 2014-8-3
	m_pImage = NULL;
	m_bBoostFlag = false;
	//chenyuzhu add end on 2014-8-3
}
CScanTopWidget::~CScanTopWidget(void)
{
}

/** 
* 下侧边缘视图
* @since     1.0
*/
CScanBottomWidget::CScanBottomWidget(QWidget *parent):QWidget(parent)
{
	//设置宽度能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);

	setMinimumSize(QSize(130, 36));
	//setMaximumSize(QSize(1331, 36));
	setStyleSheet(QString::fromUtf8("background-color: rgb(17, 14, 9);"));	
	//chenyuzhu add start on 2014-8-3
	m_pImage = NULL;
	m_bBoostFlag = false;
	//chenyuzhu add end on 2014-8-3
}
CScanBottomWidget::~CScanBottomWidget(void)
{
}

/** 
* 厚度图
* @since     1.0
*/
CScanThicknessWidget::CScanThicknessWidget(QWidget *parent):QWidget(parent)
{
	m_pThicknessImage = NULL;
	//设置宽度能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName(QString::fromUtf8("wgtThicknessView"));
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(400, 126));
}

CScanThicknessWidget::~CScanThicknessWidget(void)
{
}

/** 
* 正视图
* @since     1.0
*/
CScanFrontWidget::CScanFrontWidget(QWidget *parent):QWidget(parent)
{
	m_pFrontImage = NULL;
	m_pBoostImage = NULL;
	m_nPlateYStart = NULL;            //局部钢板区域中长度的起始
	m_nPlateYEnd = NULL;              //局部钢板区域中长度的终点
	m_bBoostFlag =false;
	//设置宽度能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName(QString::fromUtf8("m_pWgtThicknessView"));
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(400, 86));
}

CScanFrontWidget::~CScanFrontWidget(void)
{
	if (NULL != m_pFrontImage)
	{
		delete m_pFrontImage;
		m_pFrontImage = NULL;
	}
	if (NULL != m_pBoostImage)
	{
		delete m_pBoostImage;
		m_pBoostImage = NULL;
	}
}


void CCscanPanel::OnDrawFrontBoostSection(double minX,double maxX,double minY,double maxY)
{
    if (NULL != m_pWgtFrontView->m_pBoostImage)
 	{
        delete m_pWgtFrontView->m_pBoostImage;
        m_pWgtFrontView->m_pBoostImage = NULL;
	}
	//将范围映射到正视图中
	int nLength = g_Info[nPlate].length;       //钢板的逻辑长度
	int nHeight = g_Info[nPlate].height;       //钢板的逻辑厚度
    int nMappingMaxX = m_pWgtFrontView->m_BoostImageRect.width() - minX * m_pWgtFrontView->m_BoostImageRect.width() / nLength;
    int nMappingMinX = m_pWgtFrontView->m_BoostImageRect.width() - maxX * m_pWgtFrontView->m_BoostImageRect.width() / nLength;
    if (NULL == m_pWgtFrontView->m_pFrontImage) //正视图隐藏
	{
		return;
	}
    QRect mappingRect(QPoint(nMappingMinX,0),QPoint(nMappingMaxX, m_pWgtFrontView->m_pFrontImage->rect().height()));
    m_pWgtFrontView->m_CurrentSectionRect = mappingRect;
	//创建放大视图
    m_pWgtFrontView->m_pBoostImage = new QImage(m_pWgtFrontView->m_BoostImageRect.width(),m_pWgtFrontView->m_BoostImageRect.height(),QImage::Format_ARGB32_Premultiplied);
	//m_pBoostImage->fill(QColor(17,14,9));  //背景
    m_pWgtFrontView->m_pBoostImage->fill(QColor(103,103,105)); // chenyuzhu add on 2014-11-4
    QPainter painter(m_pWgtFrontView->m_pBoostImage);
	painter.setPen(Qt::green);
	painter.setBrush(Qt::NoBrush);

    ////////////////////////
    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");

        if(static_cast<int>(temp.maxY) >= minY && static_cast<int>(temp.minY) <= maxY)
		{
            int xStart = (double)m_pWgtFrontView->m_BoostImageRect.width() - temp.minX * (double)m_pWgtFrontView->m_BoostImageRect.width() / (double)g_Info[nPlate].length ;
            int xEnd = (double)m_pWgtFrontView->m_BoostImageRect.width() - temp.maxX * (double)m_pWgtFrontView->m_BoostImageRect.width() / (double)g_Info[nPlate].length ;
            int y = temp.z / g_Info[nPlate].height * m_pWgtFrontView->m_BoostImageRect.height()+1;
		//chenyuzhu midify end on 2014-10-28
		//
			//chenyuzhu add start on 2014-6-17 //根据面积过滤
			if(*g_bInjPointFilter)
			{
				if (*g_bFilterSmaller)
				{
                    if(temp.mianShangArea < *g_nFilterSmallerNum)
					{
						continue;
					}
				}
				if (*g_bFilterBiger)
				{
                    if(temp.mianShangArea > *g_nFilterBigerNum)
					{
						continue;
					}
				}
			}
			//chenyuzhu add end on 2014-6-17 


			//chenyuzhu add start on 2014-6-13
			QColor clrCurrent;
			QColor clrLostWave;
			clrCurrent.setRgb(0,255,0);
			clrLostWave.setRgb(255,255,255);

			if (g_nComboxColor == 0)
			{
                if (fColor < 0.5)
                {
                    clrCurrent.setRgb(0,255,0);
                }
                if (fColor >= 0.5 && fColor < 1.0)
                {
                    clrCurrent.setRgb(255,255,0);
                }
                if (fColor >= 1.0)
                {
                    clrCurrent.setRgb(255,0,0);
                }
                else
                {
                }
			}
			else if (g_nComboxColor == 1)
			{
                if (temp.z == 0)
				{
					clrCurrent = clrLostWave;
				}
				else
				{
                    clrCurrent = CalcFillColor(int(temp.z));
				}	
			}
			painter.setPen(QPen(clrCurrent,1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
			//chenyuzhu add start on 2014-6-13

			painter.drawLine(QPoint(xStart,y),QPoint(xEnd,y));
		} 
		else
		{
			continue;
		}
	}
	painter.end();
    m_pWgtFrontView->m_bBoostFlag = true;
	m_pWgtFrontView->update();
}

//chenyuzhu add start on 2014-6-13
QColor CScanFrontWidget::CalcFillColor(const int& nvalue)
{	
	QColor color;
	int index = nvalue*510/int(g_Info[nPlate].height);

	if (index > 255)
	{
		int nColor = index-255;
		color.setRgb(nColor,0,255-nColor);
	}
	else
	{
		int nColor = index;
		color.setRgb(0,255-nColor,nColor);		
	}	
	return color;
}
//chenyuzhu add end on 2014-6-13
void CScanFrontWidget::paintEvent(QPaintEvent *e)
{
	
	QRect rect = this->rect();
	QPainter painter(this);
	//正视图
	QRect frontBackRect(rect.left()+ 45,rect.top(),rect.width() - 46,rect.height()-1);
	painter.setPen(QColor(255,255,255));//添加画笔
	//painter.setBrush(QColor(17,14,9));	// 设置画刷
	painter.setBrush(QColor(103,103,105)); //chenyuzhu add on 2014-11-4
	painter.drawRect(frontBackRect);

	painter.setPen(QColor(17,14,9));//添加画笔
	painter.setBrush(QColor(236,233,216));	// 设置画刷
	//厚度图左方标尺
	QRect ruleLeftRect(rect.left(),rect.top(),43,rect.height());
	painter.drawRect(ruleLeftRect);//画背景色

	//chenyuzhu modify start on 2014-5-30
	int stepV = 0;
	
	if (g_Info[nPlate].height >= 500)
		stepV = 100;
	else if (g_Info[nPlate].height >= 100 && g_Info[nPlate].height < 500)
		stepV = 50;
	else if (g_Info[nPlate].height >= 20 && g_Info[nPlate].height < 100)
		stepV = 10;
	else
		stepV = 5;
	DrawVRuler(this,ruleLeftRect,0,g_Info[nPlate].height,stepV,1,0);//chenyuzhu modify on 2014-10-28
	//QPainter painterRuler(this);
	//DrawRuler(painterRuler, ruleLeftRect, 0, g_Info[nPlate].height, 5, 0, 1);
	//painterRuler.end();
	//DrawVRuler(this,ruleLeftRect,0,30,5,1,0);
	//DrawVRuler(this,ruleLeftRect,0,30,10,1,0);
	//chenyuzhu modify end on 2014-5-30
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)  //放大
		{
			QPainter widgetPainter(this);
			widgetPainter.drawImage(QRect(46,1,this->rect().width()-47,this->rect().height()-2),*m_pBoostImage,m_CurrentSectionRect);
			widgetPainter.end();
		}
		else   //正常
		{
			if(1)
			{
				emit signalDrawFrontDetial();
			}
			else
			{
				emit signalDrawXShadowRegion();
			}
		}
	}	
}
/** 
* 侧视图
* @since     1.0
*/
CScanSideWidget::CScanSideWidget(QWidget *parent):QWidget(parent)
{
	m_pSideImage = NULL;
	m_pBoostImage = NULL;
	m_bBoostFlag = false;      //放大标示
	m_nPlateYStart = NULL;   //局部钢板区域中长度的起始
	m_nPlateYEnd = NULL;     //局部钢板区域中长度的终点
	//设置宽度不能伸展，高度能伸展
	QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy3.setHorizontalStretch(0);
	sizePolicy3.setVerticalStretch(0);
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName(QString::fromUtf8("SideView"));
	sizePolicy3.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy3);
	setMinimumWidth(143);
}

CScanSideWidget::~CScanSideWidget(void)
{
	if (NULL != m_pSideImage)
	{
		delete m_pSideImage;
		m_pSideImage = NULL;
	}
	if (NULL != m_pBoostImage)
	{
		delete m_pBoostImage;
		m_pBoostImage = NULL;
	}
}

void CCscanPanel::OnDrawSideBoostSection(double minX,double maxX,double minY,double maxY) //响应画侧视图放大部分
{
    if (NULL != m_pWgtSideView->m_pBoostImage)
	{
        delete m_pWgtSideView->m_pBoostImage;
        m_pWgtSideView->m_pBoostImage = NULL;
	}
	//将范围映射到正视图中
	int nWidth = g_Info[nPlate].width;       //钢板的逻辑长度
	int nHeight = g_Info[nPlate].height;       //钢板的逻辑厚度
    int nMappingMaxY = m_pWgtSideView->m_BoostImageRect.height() - minY * m_pWgtSideView->m_BoostImageRect.height() / nWidth;//chenyuzhu modify on 2014-10-28
    int nMappingMinY = m_pWgtSideView->m_BoostImageRect.height() - maxY * m_pWgtSideView->m_BoostImageRect.height() / nWidth;//chenyuzhu modify on 2014-10-28
    QRect mappingRect(QPoint(0,nMappingMinY),QPoint(m_pWgtSideView->m_BoostImageRect.width(), nMappingMaxY));
    m_pWgtSideView->m_CurrentSectionRect = mappingRect;
	//创建放大视图
    m_pWgtSideView->m_pBoostImage = new QImage(m_pWgtSideView->m_BoostImageRect.width(),m_pWgtSideView->m_BoostImageRect.height(),QImage::Format_ARGB32_Premultiplied);
    m_pWgtSideView->m_pBoostImage->fill(QColor(103,103,105)); // chenyuzhu add on 2014-11-4
    QPainter painter(m_pWgtSideView->m_pBoostImage);
	painter.setPen(Qt::green);
	painter.setBrush(Qt::NoBrush);
    ////////////////////////

    ////////////////////////

    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");
        if (   static_cast<int>(temp.minX) >= minX
            && static_cast<int>(temp.maxX) <= maxX
            && static_cast<int>(temp.maxY) >= minY
            && static_cast<int>(temp.minY) <= maxY)
		{
			//计算点坐标		
            int yStart = m_pWgtSideView->m_BoostImageRect.height() - temp.minY * m_pWgtSideView->m_BoostImageRect.height() / g_Info[nPlate].width;
            int yEnd = m_pWgtSideView->m_BoostImageRect.height() - temp.maxY * m_pWgtSideView->m_BoostImageRect.height() / g_Info[nPlate].width;
            int x = m_pWgtSideView->m_BoostImageRect.width() - temp.z / g_Info[nPlate].height * m_pWgtSideView->m_BoostImageRect.width() + 1;
			//chenyuzhu modify start on 2014-6-17 //根据面积过滤
			if(*g_bInjPointFilter)
			{
				if (*g_bFilterSmaller)
				{
                    if(temp.mianShangArea < *g_nFilterSmallerNum)
					{
						continue;
					}
				}
				if (*g_bFilterBiger)
				{
                    if(temp.mianShangArea > *g_nFilterBigerNum)
					{
						continue;
					}
				}
			}
			//chenyuzhu modify end on 2014-6-17

			//chenyuzhu add start on 2014-6-13
			QColor clrCurrent;
			QColor clrLostWave;
			clrCurrent.setRgb(0,255,0);
			clrLostWave.setRgb(255,255,255);

			if (g_nComboxColor == 0)
			{
                if (fColor < 0.5)
                {
                    clrCurrent.setRgb(0,255,0);
                }
                if (fColor >= 0.5 && fColor < 1.0)
                {
                    clrCurrent.setRgb(255,255,0);
                }
                if (fColor >= 1.0)
                {
                    clrCurrent.setRgb(255,0,0);
                }
                else
                {
                }
			}
			else if (g_nComboxColor == 1)
			{
                if (temp.z == 0)
				{
					clrCurrent = clrLostWave;
				}
				else
				{
                    clrCurrent = CalcFillColor(int(temp.z));

				}	
			}
			painter.setPen(QPen(clrCurrent,1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
			//chenyuzhu add start on 2014-6-13
			painter.drawLine(QPoint(x,yStart),QPoint(x,yEnd));
		} 
		else
		{
			continue;
		}
	}
	painter.end();
    m_pWgtSideView->m_bBoostFlag = true;
    m_pWgtSideView->update();
}

// 重新判伤
void CCscanPanel::OnRePanShang()
{
    CalcResultData();
    update();
    JiSuanBiaoZhun();
}

//chenyuzhu add start on 2014-6-13
QColor CScanSideWidget::CalcFillColor(const int& nvalue)
{	
	QColor color;
	int index = nvalue*510/int(g_Info[nPlate].height);

	if (index > 255)
	{
		int nColor = index-255;
		color.setRgb(nColor,0,255-nColor);
	}
	else
	{
		int nColor = index;
		color.setRgb(0,255-nColor,nColor);		
	}	
	return color;
}
//chenyuzhu add end on 2014-6-13

void CScanSideWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);
	QRect sideBackRect(rect.left(),rect.top() + 38,rect.width()-1,rect.height()-39 );
	painter.setPen(QColor(255,255,255));//添加画笔
	//painter.setBrush(QColor(17,14,9));	// 设置画刷
	painter.setBrush(QColor(103,103,105)); //chenyuzhu add on 2014-11-4
	painter.drawRect(sideBackRect);
	//侧视图上方标尺
	painter.setPen(QColor(17,14,9));//添加画笔
	painter.setBrush(QColor(236,233,216));	// 设置画刷
	QRect rulerLeftRect(rect.left(),rect.top(),rect.width(),36);
	painter.drawRect(rulerLeftRect);//画背景色

	//chenyuzhu modif start on 2014-5-30
	int stepV = 0;
	if (g_Info[nPlate].height >= 500)
		stepV = 50;
	else if (g_Info[nPlate].height >= 100 && g_Info[nPlate].height < 500)
		stepV = 10;
	else if (g_Info[nPlate].height >= 20 && g_Info[nPlate].height < 100)
		stepV = 5;
	else
		stepV = 1;
	DrawHRuler(this,rulerLeftRect,0,g_Info[nPlate].height,stepV,1,1);//chenyuzhu modify on 2014-10-28
//	QPainter painterRuler(this);
//	DrawRuler(painterRuler, rulerLeftRect, g_Info[nPlate].height, 0, 3, 1, 0);
//	painterRuler.end();
	
	//chenyuzhu modif end on 2014-5-30

	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)  //放大
		{
			QPainter widgetPainter(this);
			widgetPainter.drawImage(QRect(1, 40, this->rect().width()-2, this->rect().height()-41), *m_pBoostImage, m_CurrentSectionRect);
			widgetPainter.end();
		}
		else   //正常
		{
			if(1)
			{
				emit signalDrawSideDetial();
			}
			else
			{
				emit signalDrawYShadowRegion();
			}
		}
	}
}



/** 
* 整体视图 
* @since     1.0
*/
CScanWholeWidget::CScanWholeWidget(QWidget *parent):QWidget(parent)
{
	m_pWholeImage = NULL;
	m_pRegionRect = NULL;
	m_pBoostDashImage = NULL;
	m_bBoostFlag = false;
	//设置宽度能伸展，高度不能伸展
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName(QString::fromUtf8("m_pWgtWholeView"));
	sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy);
	setMinimumSize(QSize(400, 126));
}

CScanWholeWidget::~CScanWholeWidget(void)
{
	if (NULL != m_pBoostDashImage)
	{
		delete m_pBoostDashImage;
		m_pBoostDashImage = NULL;
	}
	if (NULL != m_pWholeImage)
	{
		delete m_pWholeImage;
		m_pWholeImage = NULL;
	}
}

/** 
* 伤点图
* @since     1.0
*/
CScanDetectWidget::CScanDetectWidget(QWidget *parent):QWidget(parent)
{
	m_pvHeaderRect = NULL;
	m_pvHRect = NULL;//chenyuzhu add on2014-6-19
	m_pbPointJoin = NULL;//chenyuzhu add on2014-6-19

	m_nXStart = 0;
	m_nXEnd = 0;
	m_nSectionFlag = 0;
	m_nWheelFlag = 0;     //默认为零
	m_pImage = NULL;
	m_pSectionImage = NULL;
	m_bSectionImage = false;
	m_bDrawingSectRect = false;
	//设置宽度能伸展，高度能伸展
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName(QString::fromUtf8("m_pWgtThicknessView"));
	sizePolicy2.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy2);
	setMinimumSize(QSize(400, 124));
	setMouseTracking(true);
	
	//chenyuzhu add end on 2014-6-10
	m_nLogicXStart = 0;
	m_nLogicXEnd = 0;
	m_nLogicYStart = 0;
	m_nLogicYEnd = 0;
	//chenyuzhu add end on 2014-6-10
	m_bBiSeBiao = false; //chenyuzhu add on 2014-11-4
}

CScanDetectWidget::~CScanDetectWidget(void)
{
   if (NULL != m_pImage)
   {
	   delete m_pImage;
	   m_pImage = NULL;
   }
   if (NULL != m_pSectionImage)
   {
	   delete m_pSectionImage;
	   m_pSectionImage = NULL;
   }
}



void CScanThicknessWidget::paintEvent(QPaintEvent *e)
{
	//厚度图
	QRect rect = this->rect();
	QPainter painter(this);
	//厚度图背景
	QRect thicknessBackRect(rect.left()+ 45,rect.top(),rect.width() - 46,rect.height() - 39 );
	painter.setPen(QColor(255,255,255));//添加画笔
	//painter.setBrush(QColor(17,14,9));	// 设置画刷
	painter.setBrush(QColor(103,103,105));//chenyuzhu add on 2014-11-4
	painter.drawRect(thicknessBackRect);

	painter.setPen(QColor(17,14,9));//添加画笔
	painter.setBrush(QColor(236,233,216));	// 设置画刷
	//厚度图左方标尺
	QRect ruleLeftRect(rect.left(),rect.top(),43,rect.height() - 38);
	//厚度图下方标尺
	QRect ruleBackRect(rect.left()+ 45,thicknessBackRect.bottom()+2,rect.width() - 46,36 );
	painter.drawRect(ruleLeftRect);//画背景色
	painter.drawRect(ruleBackRect);//画背景色
	//画中间线
	painter.setPen(QPen(QColor(0,255,0),1,Qt::DashLine,Qt::RoundCap));//添加画笔
	painter.drawLine(QPoint(rect.left()+ 38,thicknessBackRect.bottom()/2),QPoint(rect.right(),thicknessBackRect.bottom()/2));
	


	//chenyuzhu add start on 2014-7-23
	int x1,x2,y1,y2;
	x1 = m_minX;
	x2 = m_maxX;
	y1 = m_minY;
	y2 = m_maxY;
	int stepV = 0;
	int stepH = 0;
	if ((x2-x1) >= 10000)
		stepH = 1000;
	else if ((x2-x1) >= 2000 && (x2-x1) < 10000)
		stepH = 500;
	else if ((x2-x1) >= 1000 && (x2-x1) < 2000)
		stepH = 100;
	else
		stepH = 50;

	if ((y2-y1) >= 10000)
		stepV = 5000;
	else if ((y2-y1) >= 2000 && (y2-y1) < 10000)
		stepV = 1000;
	else if ((y2-y1) >= 1000 && (y2-y1) < 2000)
		stepV = 500;
	else
		stepV = 100;
	DrawVRuler(this,ruleLeftRect,-6,6,6,1,1);//步长和宽度影响标尺	
	DrawHRuler(this,ruleBackRect,x1,x2,stepH,0,1);//chenyuzhu modify on 2014-10-28

	//QPainter painterRuler(this);
	//DrawRuler(painterRuler, ruleLeftRect, 6, -6, 4, 0, 1);
	//DrawRuler(painterRuler, ruleBackRect, x2, x1, 10, 1, 1);
	//painterRuler.end();
	//chenyuzhu add end on 2014-7-23

	//chenyuzhu modify start on 2014-5-28
	//int start_x = *m_pPlateYStart;
	//int end_x = *m_pPlateYEnd;
//	DrawVRuler(this,ruleLeftRect,-6,6,6,1,0);//步长和宽度影响标尺	
//	DrawHRuler(this,ruleBackRect,0,100,5,0,0);
	//chenyuzhu modify end on 2014-5-28
	
	if (g_bReadyDraw)
	{
		emit signalDrawThicknessDetial();
	}
}

//chenyuzhu add start on 2014-7-23
void CScanThicknessWidget::OnSignalDrawCScanThicknessSection(double minX,double maxX,double minY,double maxY) //
{
	m_minX = minX;
	m_maxX = maxX;
	m_minY = minY;
	m_maxY = maxY;

	update();
}
//chenyuzhu add end on 2014-7-23
//鼠标悬浮实现
void CScanDetectWidget::mouseMoveEvent(QMouseEvent *event)
{
	//画选中矩形
	if (m_bDrawingSectRect) //鼠标按下
	{
		m_nEndPoint = event->pos();
		update();
	}
	else  //当不绘制选中框时才有悬浮效果
	{
		double x = event->pos().x()-46;
		double y = event->pos().y()-38;
		if (0 == m_nXEnd) //未画图
		{
			return;
		}
		if (x<0 || y<0) //鼠标未在显示区域内
		{
			return;
		}
		//鼠标坐标到内存位图的坐标的映射
		double  bitmapX = 0;
		double  bitmapY = 0;
		if (0 == m_nSectionFlag)  //正常视图
		{
			bitmapX = x / (width()-47) *(m_nXEnd - m_nXStart) + m_nXStart;
			bitmapY = y / (height()-39) * m_nYSize;
		}else //放大视图
		{
			bitmapX = x / (width()-47) *m_CurrentWholeSection.width() + m_CurrentWholeSection.left();
			bitmapY = y / (height()-39) * m_CurrentWholeSection.height() + m_CurrentWholeSection.top();
		}
		//chenyuzhu modif start on 2014-6-19
		if(*m_pbPointJoin)
		{
			//查找相应伤点信息
			if (NULL != m_pvHRect)
			{
				int num = m_pvHRect->size();
				for (int i = 0; i<num; i++)
				{
					//chenyuzhu modify start on 2014-10-27
					//QRect temp = QRect(QPoint((((*m_pvHRect)[i].minX - 2)/ g_Info[nPlate].length) * m_nXSize-1,(((*m_pvHRect)[i].minY - 3) /g_Info[nPlate].width ) * m_nYSize-1),
					//	QPoint((((*m_pvHRect)[i].maxX) / g_Info[nPlate].length) * m_nXSize+1,(((*m_pvHRect)[i].maxY) /g_Info[nPlate].width ) * m_nYSize+1));
					QRect temp = (*m_pvHRect)[i].drawingPointRect;
					//chenyuzhu modify end on 2014-10-27					
					
					//chenyuzhu add start on 2014-6-10 //保存当前选中的面伤区域坐标删除伤点时用到
					m_nLogicXStart = 0;
					m_nLogicXEnd = 0;
					m_nLogicYStart = 0;
					m_nLogicYEnd = 0;
					//chenyuzhu add end on 2014-6-10				
					if (temp.contains(QPoint(bitmapX,bitmapY)))
					{
						//chenyuzhu add start on 2014-6-10 //保存当前选中的面伤区域坐标删除伤点时用到
						m_nLogicXStart = (*m_pvHRect)[i].minX;
						m_nLogicXEnd = (*m_pvHRect)[i].maxX;
						m_nLogicYStart = (*m_pvHRect)[i].minY;
						m_nLogicYEnd = (*m_pvHRect)[i].maxY;
						//chenyuzhu add end on 2014-6-10	
						QString strZ = QString::number((*m_pvHRect)[i].z);
						QString strArea = QString::number((*m_pvHRect)[i].mianShangArea / 100.0f);
						QString strXStart = QString::number((*m_pvHRect)[i].minX);
						QString strXEnd = QString::number((*m_pvHRect)[i].maxX);
						QString strYStart = QString::number((*m_pvHRect)[i].minY);
						QString strYEnd = QString::number((*m_pvHRect)[i].maxY);
						QString strSumArea = QString::number((*m_pvHRect)[i].mianShangArea / 100.0f);
						QString strX = QString::number((*m_pvHRect)[i].maxX - (*m_pvHRect)[i].minX);
						QString strY = QString::number((*m_pvHRect)[i].maxY - (*m_pvHRect)[i].minY);
						QString strZhiShiLength = QString::number((*m_pvHRect)[i].zhishiLength);
						QString strText = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32")
							.arg(tr("指示长度:")).arg(strZhiShiLength).arg(tr("mm")).arg(tr("\n")).arg(tr("长:")).arg(strXStart).arg(tr("-")).arg(strXEnd).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("长差值:")).arg(strX).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("宽:")).arg(strYStart).arg(tr("-")).arg(strYEnd).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("宽差值:")).arg(strY).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("深:")).arg(strZ).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("面伤面积:")).arg(strArea).arg(tr("cm^2")).arg(tr("\n"));   //删除了总面积 Chen Yuzhu add on 2014-4-28
						emit signalToChannelPannel(strText);
						return;
					}else
					{
						QString temp;
						emit signalToChannelPannel(temp); 			
					}
				}
			}
		}
		else
		{
			//查找相应伤点信息
			if (NULL != m_pvHeaderRect)
			{
				int num = m_pvHeaderRect->size();
				for (int i = 0; i<num; i++)
				{
					//chenyuzhu modify start on 2014-10-27
					//QRect temp = QRect(QPoint((((*m_pvHeaderRect)[i].minX - 2)/ g_Info[nPlate].length) * m_nXSize-1,(((*m_pvHeaderRect)[i].minY - 3) /g_Info[nPlate].width ) * m_nYSize-1),
					//	QPoint((((*m_pvHeaderRect)[i].maxX) / g_Info[nPlate].length) * m_nXSize+1,(((*m_pvHeaderRect)[i].maxY) /g_Info[nPlate].width ) * m_nYSize+1));
					QRect temp = (*m_pvHeaderRect)[i].drawingPointRect;
					//chenyuzhu modify end on 2014-10-27
					//chenyuzhu add start on 2014-6-10 //保存当前选中的面伤区域坐标删除伤点时用到
					m_nLogicXStart = 0;
					m_nLogicXEnd = 0;
					m_nLogicYStart = 0;
					m_nLogicYEnd = 0;
					//chenyuzhu add end on 2014-6-10				
					if (temp.contains(QPoint(bitmapX,bitmapY)))
					{
						//chenyuzhu add start on 2014-6-10 //保存当前选中的面伤区域坐标删除伤点时用到
						m_nLogicXStart = (*m_pvHeaderRect)[i].minX;
						m_nLogicXEnd = (*m_pvHeaderRect)[i].maxX;
						m_nLogicYStart = (*m_pvHeaderRect)[i].minY;
						m_nLogicYEnd = (*m_pvHeaderRect)[i].maxY;
						//chenyuzhu add end on 2014-6-10	
	 					QString strZ = QString::number((*m_pvHeaderRect)[i].z);
	 					QString strArea = QString::number((*m_pvHeaderRect)[i].mianShangArea / 100.0f);
	 					QString strXStart = QString::number((*m_pvHeaderRect)[i].minX);
	 					QString strXEnd = QString::number((*m_pvHeaderRect)[i].maxX);
	 					QString strYStart = QString::number((*m_pvHeaderRect)[i].minY);
	 					QString strYEnd = QString::number((*m_pvHeaderRect)[i].maxY);
	 					QString strSumArea = QString::number((*m_pvHeaderRect)[i].mianShangArea / 100.0f);
	 					QString strX = QString::number((*m_pvHeaderRect)[i].maxX - (*m_pvHeaderRect)[i].minX);
						QString strY = QString::number((*m_pvHeaderRect)[i].maxY - (*m_pvHeaderRect)[i].minY);
						QString strZhiShiLength = QString::number((*m_pvHeaderRect)[i].zhishiLength);
	 					QString strText = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32")
			 				.arg(tr("指示长度:")).arg(strZhiShiLength).arg(tr("mm")).arg(tr("\n")).arg(tr("长:")).arg(strXStart).arg(tr("-")).arg(strXEnd).arg(tr("mm")).arg(tr("\n"))
			 				.arg(tr("长差值:")).arg(strX).arg(tr("mm")).arg(tr("\n"))
			 				.arg(tr("宽:")).arg(strYStart).arg(tr("-")).arg(strYEnd).arg(tr("mm")).arg(tr("\n"))
			 				.arg(tr("宽差值:")).arg(strY).arg(tr("mm")).arg(tr("\n"))
							.arg(tr("深:")).arg(strZ).arg(tr("mm")).arg(tr("\n"))
	 						.arg(tr("面伤面积:")).arg(strArea).arg(tr("cm^2")).arg(tr("\n"));   //删除了总面积 Chen Yuzhu add on 2014-4-28
						emit signalToChannelPannel(strText);
						return;
					}else
					{
	 					QString temp;
	 					emit signalToChannelPannel(temp); 			
					}
				}
			}
		}
		//chenyuzhu modify end on 2014-6-19
	}
}


void CScanDetectWidget::mouseReleaseEvent(QMouseEvent *e)        //鼠标弹起
{
	if((e->button() == Qt::LeftButton) && g_bReadyDraw)  //判断是否是鼠标左键
	{
		m_bDrawingSectRect = false;
		m_nEndPoint.setX(e->pos().x()-46);
		m_nEndPoint.setY(e->pos().y()-39);
		if (m_nEndPoint.x()<0)
		{
			m_nEndPoint.setX(0);
		}
		else if (m_nEndPoint.x() > rect().width())
		{
			m_nEndPoint.setX(rect().width()-40);
		}
		if (m_nEndPoint.y()<0)
		{
			m_nEndPoint.setY(0);
		}
		else if(m_nEndPoint.y()> rect().height())
		{
			m_nEndPoint.setY(rect().height()-40);
		}
		if (m_nBeginPoint != m_nEndPoint)
		{
			m_nSectionFlag++;
			m_bSectionImage = false;
			update();
		}
	}
}



void CScanDetectWidget::mousePressEvent(QMouseEvent * e)          //鼠标按下
{
	if((e->button() == Qt::LeftButton) && g_bReadyDraw)   //判断是否是鼠标左键
	{
		m_bDrawingSectRect = true;
		m_nBeginPoint.setX(e->pos().x()-46);
		m_nBeginPoint.setY(e->pos().y()-39);
		if (m_nBeginPoint.x()<0)
		{
			m_nBeginPoint.setX(0);
		}
		else if (m_nBeginPoint.x() > rect().width())
		{
			m_nBeginPoint.setX(rect().width()-40);
		}

		if (m_nBeginPoint.y()<0)
		{
			m_nBeginPoint.setY(0);
		}
		else if(m_nBeginPoint.y()> rect().height())
		{
			m_nBeginPoint.setY(rect().height()-40);
		}
	}
}
void CScanDetectWidget::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)  //放大
	{
		if (m_CurrentWholeSection.isEmpty())
		{
			m_nWheelFlag = 1;
			m_nSectionFlag++;
			m_bSectionImage = false;
			update();
		}
		else if(m_CurrentWholeSection.width() >25 && m_CurrentWholeSection.height()>25)
		{
			m_nWheelFlag = 1;
			m_nSectionFlag++;
			m_bSectionImage = false;
			update();
		}
		else
		{
			QMessageBox::information(this,"CScanDetectWidget::wheelEvent",tr("不能再放大了！"));
			return;
		}
	}
	else     //缩小
	{
		m_nWheelFlag = 2;
		m_nSectionFlag++;   //放大标记也增大
		m_bSectionImage = false;
		update();
	}	
}
void CScanDetectWidget::paintBoostSection(void)
{
	if (false == m_bBiSeBiao)//chenyuzhu add on 2014-11-4
	{
		if (m_nWheelFlag > 0)             //滚轮缩放
		{
			if (1 == m_nWheelFlag)        //放大
			{
				if (1 == m_nSectionFlag)  //第一次放大
				{
					int width = (m_nXEnd - m_nXStart) /10 * 9;
					int height = m_pImage->height() /10 * 9;
				

					//m_CurrentWholeSection = QRect(QPoint(0,0),QPoint(width,height));
					m_CurrentWholeSection = QRect(QPoint(m_nXStart,0),QPoint(m_nXEnd ,m_pImage->height()));//chenyuzhu add on 2014_5-30
				}
				else  //非第一次放大
				{ 
					//chenyuzhu add start on 2014-5-29
					int width = m_CurrentWholeSection.width() /10 * 9;
					int height = m_CurrentWholeSection.height() /10 * 9;
					QPoint end;
					QPoint start;
					int x1 = m_CurrentWholeSection.center().x() - width/2;
					start.setX(x1);
					int y1 = m_CurrentWholeSection.center().y() - height/2;
					start.setY(y1);

					int x2 = m_CurrentWholeSection.center().x() + width/2;
					end.setX(x2);
					int y2 = m_CurrentWholeSection.center().y() + height/2;
					end.setY(y2);

					
					//chenyuzhu add end on 2014-5-29
					
					
				//	QPoint start = m_CurrentWholeSection.topLeft();
				//	QPoint end = QPoint(m_CurrentWholeSection.right() * 9 / 10,m_CurrentWholeSection.bottom() * 9 / 10);
					if ((end.x() - start.x() <= 0) &&(end.y() - start.y() <= 0) )
					{
						QMessageBox::information(this,"CScanDetectWidget::paintBoostSection",tr("不能再放大了"));
						m_bSectionImage = true;
						return;
					}
					if (start.x() > end.x())
					{
						int temp = start.x();
						start.setX(end.x());
						end.setX(temp);
					}
					if (start.y() > end.y())
					{
						int temp = start.y();
						start.setY(end.y());
						end.setY(temp);
					}
					if ((end.x() - start.x() <=25) || (end.y() - start.y()<=25) )
					{
						m_nWheelFlag = 0;
						return;
					}
					m_CurrentWholeSection = QRect(start,end);
				}
			}
			else if (2 == m_nWheelFlag)   //缩小
			{
				if (m_CurrentWholeSection.isEmpty()) //第一次缩放
				{
					m_CurrentWholeSection = QRect(QPoint(m_nXStart,0),QPoint(m_nXEnd,m_pImage->height()));
				}
				int a = m_pImage->width();
				int nWidth = m_CurrentWholeSection.width()*1.1;
				if (nWidth > m_pImage->width()-1)
				{
					nWidth = m_pImage->width()-1;
				}
				int nHeight = m_CurrentWholeSection.height()*1.1;
				if (nHeight > m_pImage->height()-1)
				{
					nHeight =m_pImage->height()-1;
				}
				int nXStart = (0 > m_CurrentWholeSection.center().x() - nWidth/2) ? 0 : (m_CurrentWholeSection.center().x() - nWidth/2);
				int nXEnd = (m_pImage->width()-1 < m_CurrentWholeSection.center().x() + nWidth/2) ? m_pImage->width()-1 : (m_CurrentWholeSection.center().x() + nWidth/2);

				int nYStart = (0 > m_CurrentWholeSection.center().y() - nHeight/2) ? 0 : (m_CurrentWholeSection.center().y() - nHeight/2);
				int nYEnd = (m_pImage->height()-1 < m_CurrentWholeSection.center().y() + nHeight/2) ? m_pImage->height()-1 : (m_CurrentWholeSection.center().y() + nHeight/2);

				if (nXStart > nXEnd)
				{
					int temp = nXStart;
					nXStart = nXEnd;
					nXEnd = temp;
				}
				if (nYStart > nYEnd)
				{
					int temp = nYStart;
					nYStart = nYEnd;
					nYEnd = temp;
				}
				m_CurrentWholeSection = QRect(QPoint(nXStart,nYStart),QPoint(nXEnd,nYEnd));		
			}
			m_nWheelFlag = 0;
		}
		else  //选中放大
		{
			QPoint topLeft(m_nBeginPoint.x() < m_nEndPoint.x() ? m_nBeginPoint.x():m_nEndPoint.x(),
				m_nBeginPoint.y() < m_nEndPoint.y() ? m_nBeginPoint.y():m_nEndPoint.y());
			QPoint rightButtom(m_nBeginPoint.x() < m_nEndPoint.x() ? m_nEndPoint.x():m_nBeginPoint.x(),
				m_nBeginPoint.y() < m_nEndPoint.y() ? m_nEndPoint.y():m_nBeginPoint.y());
			//当前窗体的宽和高
			int cWidth = width()-46;
			int cHeight = height()-39;
			double BeginX = topLeft.x();
			double BeginY = topLeft.y() ;
			double EndX = rightButtom.x();
			double EndY = rightButtom.y();
			//映射到全局位图中
			if(1 == m_nSectionFlag)
			{
				int bitmapSectWidth = m_nXEnd - m_nXStart;
				int bitmapSectHeight = m_pImage->height();
				int bitmapBX = (double)m_nXStart + ((double)BeginX * (double)bitmapSectWidth /(double)cWidth);
				if (bitmapBX > m_nXSize)
				{
					bitmapBX = m_nXSize-2;
				}
				int bitmapEX = (double)m_nXStart + ((double)EndX * (double)bitmapSectWidth /(double)cWidth);
				if (bitmapEX > m_nXSize)
				{
					bitmapEX = m_nXSize-1;
				}
				int bitmapBY = (double)BeginY * (double)bitmapSectHeight / (double)cHeight;
				if (bitmapBY > m_pImage->height())
				{
					bitmapBY = m_pImage->height()-1;
				}
				int bitmapEY = (double)EndY * (double)bitmapSectHeight / (double)cHeight;
				if (bitmapEY > m_pImage->height())
				{
					bitmapEY = m_pImage->height()-1;
				}
				if (bitmapBX > bitmapEX)
				{
					bitmapBX = bitmapBX - bitmapEX;
					bitmapEX = bitmapBX + bitmapEX;
					bitmapBX = bitmapEX - bitmapBX;
				}
				if (bitmapBY > bitmapEY)
				{
					bitmapBY = bitmapBY - bitmapEY;
					bitmapEY = bitmapBY + bitmapEY;
					bitmapBY = bitmapEY - bitmapBY;
				}
				/*if (bitmapEY - bitmapBY <= 25 || bitmapEX - bitmapBX <= 25 )	// 如果选择区域过小，重置选择区域为原有区域，并添加提示
				{
					m_CurrentWholeSection = QRect(QPoint(m_nXStart,0),QPoint(m_nXEnd,m_pImage->height()));
					QMessageBox::information(this,"选择区域过小",tr("放大倍数受限，请重新选择放大区域"));
					m_nSectionFlag = 0;
				}
				else*/
				{
					m_CurrentWholeSection.setTopLeft(QPoint(bitmapBX,bitmapBY));
					m_CurrentWholeSection.setBottomRight(QPoint(bitmapEX,bitmapEY));
				}
			}
			else
			{
				if (m_CurrentWholeSection.isEmpty())
				{
					QMessageBox::information(this,"CScanDetectWidget::paintBoostSection","m_CurrentWholeSection is Empty");
					m_bSectionImage = true;
					return;
				}
				int bitmapSectWidth = m_CurrentWholeSection.width();
				int bitmapSectHeight = m_CurrentWholeSection.height();
				int bitmapBX = (double)m_CurrentWholeSection.left() + ((double)BeginX * (double)bitmapSectWidth /(double)cWidth);
				if (bitmapBX > m_pImage->width())
				{
					bitmapBX = m_pImage->width()-1;
				}	
				int bitmapEX = (double)m_CurrentWholeSection.left() + ((double)EndX * (double)bitmapSectWidth /(double)cWidth);
				if (bitmapEX > m_pImage->width())
				{
					bitmapEX = m_pImage->width()-1;
				}	
				int bitmapBY = (double)m_CurrentWholeSection.top() + (double)BeginY * (double)bitmapSectHeight / (double)cHeight;
				if (bitmapBY > m_pImage->height())
				{
					bitmapBY = m_pImage->width()-1;
				}
				int bitmapEY = (double)m_CurrentWholeSection.top() + (double)EndY * (double)bitmapSectHeight / (double)cHeight;
				if (bitmapEY > m_pImage->height())
				{
					bitmapEY = m_pImage->height()-1;
				}
				if (bitmapBX > bitmapEX)
				{
					bitmapBX = bitmapBX - bitmapEX;
					bitmapEX = bitmapBX + bitmapEX;
					bitmapBX = bitmapEX - bitmapBX;
				}
				if (bitmapBY > bitmapEY)
				{
					bitmapBY = bitmapBY - bitmapEY;
					bitmapEY = bitmapBY + bitmapEY;
					bitmapBY = bitmapEY - bitmapBY;
				}
				if (bitmapEY - bitmapBY <= 25 || bitmapEX - bitmapBX <= 25 )
				{
					m_bSectionImage = true;
					return;
				}
				m_CurrentWholeSection.setTopLeft(QPoint(bitmapBX,bitmapBY));
				m_CurrentWholeSection.setBottomRight(QPoint(bitmapEX,bitmapEY));
			}
		}
	}
	//chenyuzhu add start on 2014-11-5
	else
	{	
		if (1 == m_nSectionFlag)  //第一次放大
		{
			int width = (m_nXEnd - m_nXStart) /10 * 9;
			int height = m_pImage->height() /10 * 9;


			//m_CurrentWholeSection = QRect(QPoint(0,0),QPoint(width,height));
			m_CurrentWholeSection = QRect(QPoint(m_nXStart,0),QPoint(m_nXEnd ,m_pImage->height()));//chenyuzhu add on 2014_5-30
		}
	}
	//chenyuzhu add end on 2014-11-5
	if( NULL != m_pSectionImage)
	{
		delete m_pSectionImage;
		m_pSectionImage = NULL;
	}	
	m_pSectionImage = new QImage(m_CurrentWholeSection.width(),m_CurrentWholeSection.height(),QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(m_pSectionImage);
	imagePainter.drawImage(m_pSectionImage->rect(),*m_pImage,m_CurrentWholeSection);
	imagePainter.end();
	m_bSectionImage = true;
	m_bBiSeBiao = false; //chenyuzhu add on 2014-11-4
}




void CScanDetectWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
 	QPainter painter(this);
 	//伤点图背景范围
 	QRect detectBackRect(rect.left()+ 45,rect.top() + 38 ,rect.width() - 46,rect.height()-39 );
 	painter.setPen(QColor(255,255,255));//添加画笔
 //	painter.setBrush(QColor(17,14,9));	// 设置画刷
	painter.setBrush(QColor(103,103,105));	// 设置画刷//chenyuzhu add on 2014-11-4
 	painter.drawRect(detectBackRect);
 
 	painter.setPen(QColor(17,14,9));//添加画笔
 	painter.setBrush(QColor(236,233,216));	// 设置画刷
 	//伤点图左方标尺范围	
 	QRect rulerLeftRect(rect.left(),rect.top() + 38,43,rect.height() - 38);
 	//伤点图上方标尺范围
 	QRect rulerUpRect(rect.left()+ 45,rect.top(),rect.width() - 45, 36);
 
 	painter.drawRect(rulerLeftRect);//画背景色
 	painter.drawRect(rulerUpRect);//画背景色
	
	//chenyuzhu modify start on 2014-5-28
	int start_x = *m_pPlateYStart;
	int end_x = *m_pPlateYEnd;
 //	DrawVRuler(this,rulerLeftRect,0,80,5,1,0);//步长和宽度影响标尺	
 //	DrawHRuler(this,rulerUpRect,start_x,end_x,50,0,0);
	//chenyuzhu modify end on 2014-5-28
	
	painter.end();
	//画选中矩形	
	if (m_bDrawingSectRect)   //画选中框
	{
		QRect draRect(QPoint(m_nBeginPoint.x()+46,m_nBeginPoint.y()+39),m_nEndPoint);
		if (NULL != m_pImage)   //画图
		{
			QPen pen(Qt::NoBrush,1,Qt::DashLine);
			pen.setColor(QColor(255,255,255));
			if(m_nSectionFlag < 1)  //正常画图
			{
				QPainter painterNor(this);
				painterNor.drawImage(QRect(46,39,this->rect().width()-47,this->rect().height()-40),*m_pImage,QRect(m_nXStart,0,(m_nXEnd - m_nXStart),m_pImage->height()));
				painterNor.setPen(pen);
				painterNor.drawRect(draRect);
				painterNor.end();
			}
			else if(m_nSectionFlag > 0 && NULL != m_pSectionImage) //放大图 //chenyuzhu modify on 2014-10-13 对m_pSectionImage指针做安全判断
			{
				QPainter painterBoo(this);
				painterBoo.drawImage(QRect(46,39,this->rect().width()-47,this->rect().height()-40),*m_pSectionImage,m_pSectionImage->rect());
				painterBoo.setPen(pen);
				painterBoo.drawRect(draRect);
				painterBoo.end();
			}
		}
	}
	else   //画图
	{
		if (g_bReadyDraw && 0 == m_nSectionFlag)//准备好数据，准备画伤点图
		{

			//chenyuzhu add start on 2014-5-30
			int stepV = 0;
			int stepH = 0;
			int start_x = *m_pPlateYStart;
			int end_x = *m_pPlateYEnd;
			if ((end_x - start_x) >= 10000)
				stepH = 1000;
			else if ((end_x - start_x) >= 2000 && (end_x - start_x) < 10000)
				stepH = 500;
			else if ((end_x - start_x) >= 1000 && (end_x - start_x) < 2000)
				stepH = 100;
			else
				stepH = 50;

			if (g_Info[nPlate].width >= 10000)
				stepV = 5000;
			else if (g_Info[nPlate].width >= 2000 && g_Info[nPlate].width < 10000)
				stepV = 1000;
			else if (g_Info[nPlate].width >= 1000 && g_Info[nPlate].width < 2000)
				stepV = 500;
			else
				stepV = 100;
			DrawVRuler(this,rulerLeftRect,0,g_Info[nPlate].width,stepV,1,1);//步长和宽度影响标尺	
			DrawHRuler(this,rulerUpRect,start_x,end_x,stepH,0,1);//chenyuzhu modify on 2014-10-28
			//QPainter painterRuler(this);
			//DrawRuler(painterRuler, rulerLeftRect, g_Info[nPlate].width, 0, 5, 0, 1);
			//DrawRuler(painterRuler, rulerUpRect, end_x, start_x, 10, 1, 1);
			//painterRuler.end();
			emit signalDrawThicknessSection(start_x,end_x,0,g_Info[nPlate].width);//chenyuzhu add start on 2014-7-23
			//chenyuzhu add end on 2014-5-30
		
			//if (!g_bStart && !g_bStop)//用于标识是否探伤结束
			//{
				emit signalDrawDetectDetial();
			//}
			//else if(g_bStart && !g_bStop)//正在探伤中
			//{
				//emit signalDrawPlateFlawSection();
			//}
		}
		else if (g_bReadyDraw && 0 != m_nSectionFlag) //画放大图
		{
				//再内存中绘制选中图像
				if (!m_bSectionImage)
				{
					paintBoostSection();
				}
				//chenyuzhu modify start on 2014-10-13
				if (NULL != m_pSectionImage)
				{
					QPainter Painter(this);
					Painter.drawImage(QRect(46,39,this->rect().width() - 47,this->rect().height()-40),*m_pSectionImage,m_pSectionImage->rect());
					Painter.end();
				}
				//chenyuzhu modify end on 2014-10-13
				double x1,x2,y1,y2;
				CalculateLogicRange(x1, x2, y1, y2);
				
				//chenyuzhu add start on 2014-5-30
				int stepV = 0;
				int stepH = 0;
				if ((x2-x1) >= 10000)
					stepH = 1000;
				else if ((x2-x1) >= 2000 && (x2-x1) < 10000)
					stepH = 500;
				else if ((x2-x1) >= 1000 && (x2-x1) < 2000)
					stepH = 100;
				else
					stepH = 50;

				if ((y2-y1) >= 10000)
					stepV = 5000;
				else if ((y2-y1) >= 2000 && (y2-y1) < 10000)
					stepV = 1000;
				else if ((y2-y1) >= 1000 && (y2-y1) < 2000)
					stepV = 500;
				else
					stepV = 100;
				DrawVRuler(this,rulerLeftRect,y1,y2,stepV,1,1);//步长和宽度影响标尺	
				DrawHRuler(this,rulerUpRect,x1,x2,stepH,0,1);//chenyuzhu modify on 2014-10-28
				//QPainter painterRuler(this);
				//DrawRuler(painterRuler, rulerLeftRect, y2, y1, 5, 0, 1);
				//DrawRuler(painterRuler, rulerUpRect, x2, x1, 10, 1, 1);
				//painterRuler.end();
				//chenyuzhu add end on 2014-5-30

				emit signalDrawFrontBoostSection(x1,x2,y1,y2);
				emit signalDrawSideBoostSection(x1,x2,y1,y2);
				emit signalDrawWholeDashSection(x1,x2,y1,y2);
				emit signalDrawThicknessSection(x1,x2,y1,y2);//chenyuzhu add start on 2014-7-23
				emit signalDrawTopSection(x1,x2,y1,y2);//chenyuzhu add start on 2014-8-3
				emit signalDrawBottomSection(x1,x2,y1,y2);//chenyuzhu add start on 2014-8-3
				emit signalDrawLeftSection(x1,x2,y1,y2);//chenyuzhu add start on 2014-8-3
				emit signalDrawRightSection(x1,x2,y1,y2);//chenyuzhu add start on 2014-8-3	
		}
		else if(!g_bReadyDraw && g_bStart && !g_bStop)//正在探伤中
		{
			emit signalDrawPlateFlawSection();
		}
	}
}


void CScanDetectWidget::CalculateLogicRange(double& minX,double& maxX,double& minY,double& maxY)
{
	if ( NULL == m_pImage || m_CurrentWholeSection.isEmpty())
	{
		QMessageBox::information(this,"CScanDetectWidget::CalculateLogicRange",tr("内存位图为空，选中区域为空"));
		return ;
	}
	//minX = (double)m_CurrentWholeSection.left() * g_Info[nPlate].length / (double)m_pImage->width();
	//maxX = (double)m_CurrentWholeSection.right() * g_Info[nPlate].length / (double)m_pImage->width();
	//minY = (double)m_CurrentWholeSection.top() * g_Info[nPlate].width / (double)m_pImage->height();
	//maxY = (double)m_CurrentWholeSection.bottom() * g_Info[nPlate].width / (double)m_pImage->height();

	maxX = g_Info[nPlate].length - (double)m_CurrentWholeSection.left() * g_Info[nPlate].length / (double)m_pImage->width();
	minX = g_Info[nPlate].length - (double)m_CurrentWholeSection.right() * g_Info[nPlate].length / (double)m_pImage->width();
	maxY = g_Info[nPlate].width - (double)m_CurrentWholeSection.top() * g_Info[nPlate].width / (double)m_pImage->height();
	minY = g_Info[nPlate].width - (double)m_CurrentWholeSection.bottom() * g_Info[nPlate].width / (double)m_pImage->height();
	return;
}
void CScanDetectWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit mouseDoubleClicked(event);		
}

void CScanDetectWidget::resizeEvent(QResizeEvent *e)
{

}


void CScanWholeWidget::mousePressEvent(QMouseEvent *event)
{
	emit signalChangeSelectRect(event->x()-39,event->y(),rect().width()-39,rect().height());//chenyuzhu modif on 2014-5-22 选取时要减去标尺宽度，传递的本身宽度也要减去标尺宽度。
}

void CScanWholeWidget::OnSignalDrawWholeDashSection(double minX,double maxX,double minY,double maxY) //响应放大时候画虚框
{
	if (NULL != m_pBoostDashImage)
	{
		delete m_pBoostDashImage;
		m_pBoostDashImage = NULL;
	}
	m_pBoostDashImage = new QImage(m_nXSize,m_nYSize,QImage::Format_ARGB32_Premultiplied);

	if (NULL == m_pWholeImage)
	{
		return;
	}
	QPainter painter(m_pBoostDashImage);
	painter.drawImage(QRect(0,0,m_nXSize,m_nYSize),*m_pWholeImage,QRect(0,0,m_nXSize,m_nYSize));
	//将范围映射到正视图中
	int nWidth = g_Info[nPlate].width;       //钢板的逻辑长度
	int nLength = g_Info[nPlate].length;       //钢板的逻辑厚度	
	//chenyuzhu modify start on 2014-10-28
    int bitmapXStart = m_nXSize - minX / nLength * m_nXSize-3;
	int bitmapXEnd = m_nXSize- maxX / nLength * m_nXSize+2;
	int bitmapYStart = m_nYSize- minY / nWidth * m_nYSize-3;
	int bitmapYEnd = m_nYSize - maxY / nWidth * m_nYSize+2;
	//chenyuzhu modify end on 2014-10-28
	QRect DashRect(QPoint(bitmapXStart,bitmapYStart),QPoint(bitmapXEnd, bitmapYEnd));
	painter.setPen(QPen(Qt::white,2,Qt::DashLine,Qt::RoundCap));
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(DashRect);
	painter.end();
	m_bBoostFlag = true;
	update();	
}

void CScanWholeWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);

	//全局图背景
	QRect wholeBackRect(rect.left()+ 45,rect.top(),rect.width() - 46,rect.height() - 39 );
	painter.setPen(QColor(255,255,255));//添加画笔
//	painter.setBrush(QColor(17,14,9));	//设置画刷
	painter.setBrush(QColor(103,103,105)); // chenyuzhu add on 2014-11-4
	painter.drawRect(wholeBackRect);

	painter.setPen(QColor(17,14,9));//添加画笔
	painter.setBrush(QColor(236,233,216));	// 设置画刷
	//厚度图左方标尺
	QRect rulerLeftRect(rect.left(),rect.top(),43,rect.height() - 38);
	//厚度图下方标尺
	QRect rulerBackRect(rect.left()+ 45,wholeBackRect.bottom()+2,rect.width() - 46,36 );
	painter.drawRect(rulerLeftRect);//画背景色
	painter.drawRect(rulerBackRect);//画背景色

	//chenyuzhu modif start on 2014.5.23
	int stepV = 0;
	int stepH = 0;
	
	if (g_Info[nPlate].length > 10000)
		stepH = 1000;
	else if (g_Info[nPlate].length >= 2000 && g_Info[nPlate].length < 10000)
		stepH = 500;
	else if (g_Info[nPlate].length >= 1000 && g_Info[nPlate].length < 2000)
		stepH = 100;
	else
		stepH = 50;
	
	if (g_Info[nPlate].width >= 10000)
		stepV = 5000;
	else if (g_Info[nPlate].width >= 2000 && g_Info[nPlate].width < 10000)
		stepV = 1000;
	else if (g_Info[nPlate].width >= 1000 && g_Info[nPlate].width < 2000)
		stepV = 500;
	else
		stepV = 100;
	DrawVRuler(this,rulerLeftRect,0,g_Info[nPlate].width,stepV,1,1);//步长和宽度影响标尺	
	DrawHRuler(this,rulerBackRect,0,g_Info[nPlate].length,stepH,0,1);//chenyuzhu modify on 2014-10-28

	//QPainter painterRuler(this);
	//DrawRuler(painterRuler, rulerLeftRect, g_Info[nPlate].width, 0, 3, 0, 1);
	//DrawRuler(painterRuler, rulerBackRect, g_Info[nPlate].length, 0, 10, 1, 1);
	//painterRuler.end();
	//chenyuzhu modif end on 2014.5.23
	
	painter.end();
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)
		{
			QPainter imagePainter(this);
			imagePainter.setBrush(Qt::NoBrush);
			imagePainter.drawImage(QRect(46,0,rect.width()-47,rect.height()-40),*m_pBoostDashImage,QRect(0,0,m_nXSize,m_nYSize));
			imagePainter.end();
		}
		else
		{
			emit signalDrawWholeDetial();
		}
	}//chenyuzhu add start on 2014-10-29
	else if(!g_bReadyDraw && g_bStart && !g_bStop)//正在探伤中
	{
		emit signalDrawWholeRunning();
	}
	//chenyuzhu add end on 2014-10-29
}
void CScanLeftWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);
	//背景
	QRect backRect(rect.left(),rect.top() + 38,rect.width()-1,rect.height() - 39);
	painter.setPen(QColor(255,255,255));//添加画笔
	painter.setBrush(QColor(103,103,105));	// 设置画刷//chenyuzhu modify on 2014-11-7
	painter.drawRect(backRect);
	painter.setPen(QColor(17,14,9));//添加画笔
	painter.setBrush(QColor(236,233,216));	// 设置画刷
	//chenyuzhu add start on 2014-8-3
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)
		{
			QPainter imagePainter(this);
			imagePainter.setBrush(Qt::NoBrush);
			imagePainter.drawImage(QRect(1,39,rect.width()-2,rect.height()-39),*m_pImage,QRect(0,m_nYStart, 80.0 / g_Info[nPlate].length * m_pImage->width(),m_nYEnd - m_nYStart));
			imagePainter.end();
		}
		else
		{
			emit signalDrawLeftDetial();
		}
	}
	//chenyuzhu add end on 2014-8-3
}
//chenyuzhu add start on 2014-8-3
void CScanLeftWidget::OnSignalDrawCScanLeftSection(double minX,double maxX,double minY,double maxY)
{
	//chenyuzhu modify start on 2014-10-28
	m_nYEnd = m_nYSize - int(minY / g_Info[nPlate].width * m_nYSize);
	m_nYStart = m_nYSize - int(maxY / g_Info[nPlate].width * m_nYSize);
	//chenyuzhu modify end on 2014-10-28
	
	m_bBoostFlag = true;
	update();
}
//chenyuzhu add end on 2014-8-3
void CScanRightWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);

	//背景
	QRect backRect(rect.left(),rect.top() + 38,rect.width()-1,rect.height() - 39);
	painter.setPen(QColor(255,255,255));//添加画笔
	painter.setBrush(QColor(103,103,105));	// 设置画刷//chenyuzhu modify on 2014-11-7
	painter.drawRect(backRect);
	//chenyuzhu add start on 2014-8-3
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)
		{
			QPainter imagePainter(this);
			imagePainter.setBrush(Qt::NoBrush);
			imagePainter.drawImage(QRect(1,39,rect.width()-2,rect.height()-39),*m_pImage,QRect(m_pImage->width() - (80.0 / g_Info[nPlate].length * m_pImage->width()),m_nYStart, (80.0 / g_Info[nPlate].length * m_pImage->width()),m_nYEnd - m_nYStart));
			imagePainter.end();
		}
		else
		{
			emit signalDrawRightDetial();
		}
	}
	//chenyuzhu add end on 2014-8-3
}
//chenyuzhu add start on 2014-8-3
void CScanRightWidget::OnSignalDrawCScanRightSection(double minX,double maxX,double minY,double maxY)
{

	//chenyuzhu modify start on 2014-10-28
	m_nYEnd = m_nYSize - int(minY / g_Info[nPlate].width * m_nYSize);
	m_nYStart = m_nYSize - int(maxY / g_Info[nPlate].width * m_nYSize);
	//chenyuzhu modify end on 2014-10-28

	m_bBoostFlag = true;
	update();
}
//chenyuzhu add end on 2014-8-3
void CScanTopWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);

	//背景
	QRect backRect(rect.left()+ 45,rect.top(),rect.width() - 46,rect.height()-1);
	painter.setPen(QColor(255,255,255));//添加画笔
	painter.setBrush(QColor(103,103,105));	// 设置画刷//chenyuzhu modify on 2014-11-7
	painter.drawRect(backRect);

	//chenyuzhu add start on 2014-8-3
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)
		{
			QPainter imagePainter(this);
			imagePainter.setBrush(Qt::NoBrush);
			imagePainter.drawImage(QRect(46,1,rect.width()-47,rect.height()-2),*m_pImage,QRect(m_nXStart,0,m_nXEnd - m_nXStart,80.0 / g_Info[nPlate].width * m_pImage->height()));
			imagePainter.end();
		}
		else
		{
			emit signalDrawTopDetial();
		}
	}
	//chenyuzhu add end on 2014-8-3
}
//chenyuzhu add start on 2014-8-3
void CScanTopWidget::OnSignalDrawCScanTopSection(double minX,double maxX,double minY,double maxY)
{
	//chenyuzhu modify start on 2014-10-28
	m_nXEnd = m_nXSize - int(minX / g_Info[nPlate].length * m_nXSize);
	m_nXStart = m_nXSize - int(maxX / g_Info[nPlate].length * m_nXSize);
	//chenyuzhu modify end on 2014-10-28
	m_bBoostFlag = true;
	update();
}
//chenyuzhu add end on 2014-8-3
void CScanBottomWidget::paintEvent(QPaintEvent *e)
{
	QRect rect = this->rect();
	QPainter painter(this);

	//背景
	QRect backRect(rect.left()+ 45,rect.top(),rect.width() - 46,rect.height()-1);
	painter.setPen(QColor(255,255,255));//添加画笔
	painter.setBrush(QColor(103,103,105));	// 设置画刷//chenyuzhu modify on 2014-11-7
	painter.drawRect(backRect);
	//chenyuzhu add start on 2014-8-3
	if (g_bReadyDraw)
	{
		if (m_bBoostFlag)
		{
			QPainter imagePainter(this);
			imagePainter.setBrush(Qt::NoBrush);
			imagePainter.drawImage(QRect(46,1,rect.width()-47,rect.height()-2),*m_pImage,QRect(m_nXStart, m_pImage->height() - (80.0 / g_Info[nPlate].width * m_pImage->height()), m_nXEnd - m_nXStart,80.0 / g_Info[nPlate].width * m_pImage->height()));
			imagePainter.end();
		}
		else
		{
			emit signalDrawBottomDetial();
		}
	}
	//chenyuzhu add end on 2014-8-3
}
//chenyuzhu add start on 2014-8-3
void CScanBottomWidget::OnSignalDrawCScanBottomSection(double minX,double maxX,double minY,double maxY)
{
	//chenyuzhu modify start on 2014-10-28
	m_nXEnd = m_nXSize - int(minX / g_Info[nPlate].length * m_nXSize);
	m_nXStart = m_nXSize - int(maxX / g_Info[nPlate].length * m_nXSize);
	//chenyuzhu modify end on 2014-10-28

	m_bBoostFlag = true;
	update();
}
//chenyuzhu add end on 2014-8-3
//右键菜单
void CCscanPanel::contextMenuEvent(QContextMenuEvent * e)
{
	if (g_bReadyDraw)
	{
		m_mPopUpMenu->exec(e->globalPos());
	}
}


bool CCscanPanel::PLCSelfTest()
{
	g_ItemCS.lock();
	int nItemPLCResult = g_ItemValue.PLCSelfResult;
	g_ItemCS.unlock();
	if (1 == nItemPLCResult)
	{
//		WORD nValue = MPB_SYS_READY_OK;
//		WDC_DIAG_ReadWriteAddr(thread_param[0].hDev, WDC_WRITE, BAR2, MPB_SYS_READY_ADDR, &nValue, WDC_MODE_16);

		WORD nValue = MPB_SYS_READY_OK;
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_SYS_READY_ADDR, nValue);
		return TRUE;
	}
	else // 0 and 2
	{
//		WORD nValue = MPB_SYS_READY_ERROR;
//		WDC_DIAG_ReadWriteAddr(thread_param[0].hDev, WDC_WRITE, BAR2, MPB_SYS_READY_ADDR, &nValue, WDC_MODE_16);

		WORD nValue = MPB_SYS_READY_ERROR;
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_SYS_READY_ADDR, nValue);
		return FALSE;
	}

}

void CCscanPanel::timerEvent( QTimerEvent *event )  //定时器事件处理器
{
	if (m_nTimerSTID == event->timerId()) //TIMER_SELF_TEXT
	{
		//数据库服务器
		if (g_sqlDB) {
			if (g_sqlDB->isValid()) {
				m_pLblServerStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));//正常绿色
			} else {
				m_pLblServerStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));//异常红色
			}
		}
		
		//PLC	
		bool bRet = 1;
#if HAVE_PLX
        bRet = PLCSelfTest();
#endif
		if (bRet)
		{
			m_PLblPLCStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));//正常绿色
		}else
		{
			m_PLblPLCStatus->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));//异常红色
		}

	}
}


bool CCscanPanel::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == m_PLblPLCStatus)
	{
		if (event->type() == QEvent::Enter)
		{
			m_pDockUST->close();
			m_pDockServer->close();
			m_pDetectResult->close();//chenyuzhu add on2014-7-28
			m_pDockPLC->move(QCursor::pos().x(), QCursor::pos().y());
			m_pDockPLC->update();
			m_pDockPLC->show();
		}
		else if(event->type() == QEvent::Leave)
		{
			m_pDockPLC->hide();
		}
	}
	else if(obj == m_pLblUSTStatus)
	{
		if (event->type() == QEvent::Enter)
		{
			m_pDockPLC->close();
			m_pDockServer->close();
			m_pDetectResult->close();//chenyuzhu add on2014-7-28
			m_pDockUST->move(QCursor::pos().x(), QCursor::pos().y());
			m_pDockUST->update();
			m_pDockUST->show();
		}
		else if(event->type() == QEvent::Leave)
		{
			m_pDockUST->hide();
		}
	}
	else if(obj == m_pLblServerStatus)
	{
		if (event->type() == QEvent::Enter)
		{
			m_pDockPLC->close();
			m_pDockUST->close();
			m_pDetectResult->close();//chenyuzhu add on2014-7-28
			m_pDockServer->move(QCursor::pos().x(), QCursor::pos().y());
			m_pDockServer->update();
			m_pDockServer->show();
		}
		else if(event->type() == QEvent::Leave)
		{
			m_pDockServer->hide();
		}
	}
	else if (obj == m_pPstBtnDetectResult)
	{
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			//根据权限设置按钮功能
			if ( QString("管理员") != g_strUserPer)
			{
				QMessageBox::warning(NULL,"AScanSave",QObject::tr("当前用户无法执行此操作"));
			}
			else
			{
				m_pInspectUser = new CInspectUser(m_pDDatabaseConnection);
				m_pInspectUser->exec();
				if (m_pInspectUser->m_isOK)
				{
					m_pCbDetectResult->setVisible(true);
				}
			}	
		}//chenyuzhu add start on 2014-7-28
		else if(event->type() == QEvent::Enter)
		{

			m_pDockPLC->close();
			m_pDockServer->close();
			m_pDockUST->close();
			m_pDetectResult->move(QCursor::pos().x(), QCursor::pos().y());
			m_pDetectResult->update();
			m_pDetectResult->show();

			m_pDetectResult->m_pBitmap = new QPixmap(300,200);
			m_pDetectResult->m_pBitmap->fill(QColor(73,73,73));
			QPainter painter(m_pDetectResult->m_pBitmap);
			painter.setPen(QColor(255,255,0));
			//chenyuzhu add start on 2014-10-30
			QFont font = QFont("Times", 15, QFont::Bold);
			painter.setFont(font);
			//chenyuzhu add end on 2014-10-30
			int height = painter.fontMetrics().height();
            painter.drawText(1,1*height+8,tr("最大伤点面积：%1平方厘米").arg(QString::number((int)(m_dMaxArea/100))));
            painter.drawText(1,2*height+12,tr("最大指示距离：%1毫米").arg(QString::number((int)m_dMaxLength)));
			//chenyuzhu add end on 2014.5.8

			painter.end();
		}
		else if(event->type() == QEvent::Leave)
		{
			m_pDetectResult->hide();
		}
		//chenyuzhu add end on 2014-7-28
	}
	return QDockWidget::eventFilter(obj, event);
}

void  CCscanPanel::ModifyDetectResult(int nCurs)
{
	m_pCbDetectResult->setVisible(false);
	m_pPstBtnDetectResult->setText(m_pCbDetectResult->currentText());
	if ( 1 == m_pCbDetectResult->currentIndex())//合格
	{
		m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0); border-style: ridge;border-color: rgb(255, 255, 255);"));//红色
		if (*m_pbDependOnMInput)
		{
			m_strResult = tr("合格");
		}
		
	}
	else if ( 0 == m_pCbDetectResult->currentIndex())//不合格
	{
		m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0); border-style: ridge;border-color: rgb(255, 255, 255);"));//绿色		
		if (*m_pbDependOnMInput)
		{
			m_strResult = tr("不合格");
		}
	}
	else
	{
		//没有其他情况，不可能出现其他的值
	}
}
//chenyuzhu modify on 2014-7-28
void CCscanPanel::SaveDPIData()
{
	//合成DPI Data字符串
	QString strDPIData;
	//QString strSQL = QString("%1%2%3").arg(tr("select * from vwPlate_Stat where name = '")).arg(g_Info[nPlate].plateno).arg("'");
#if HAVE_DATA_BASE
	QSqlQuery query(*g_sqlDB);
#endif
/*	query.exec(strSQL);
	if (!query.isActive())
	{
		QMessageBox::critical(this,tr("SaveDPIData"),tr("无数据，返回！"));
		return;
	}
	query.next();*/

	strDPIData += tr("IA0009"); // FROM-ID	
	QDateTime time = QDateTime::currentDateTime(); // LOG回拋日期
	QString strDateTime = time.toString("yyyyMMddhhmmss");
	strDPIData += strDateTime;
	strDPIData += tr("ZB");	// 资料传送来源计算机代号
	strDPIData += tr("M1");	// 资料传送目的计算机代号
	strDPIData += tr("D"); // 功能代号	
	QString strPlateNo;	// 钢板号	

	if( g_Info[nPlate].plateno.size() >= 9)	{	
		strPlateNo = QString("%1").arg(g_Info[nPlate].plateno.left(9));
	} else {	
		QString temp;
		strPlateNo = QString("%1").arg(temp.sprintf("%-9s",g_Info[nPlate].plateno.toLocal8Bit().data()));
	}

	strDPIData += strPlateNo;

	//炉号
	QString strTempLuHao = m_PlateInfo[2];
	QString strTempLength = m_PlateInfo[9];
	QString strTempWidth = m_PlateInfo[10];
	QString strTempHeight = m_PlateInfo[11];
	QString strTempWeight = m_PlateInfo[12];
	QString strTempResult = m_PlateInfo[13];
	QString strTempTime = m_PlateInfo[14];;   //得到的数据和原VC中有差异，但没影响后续程序
	//QString strClass = m_PlateInfo[15];;
	QString strClass = g_Banji;
	QString strTempFlawNum = m_PlateInfo[17];;
	QString strTempSumArea = m_PlateInfo[18];;
	QString strTempZhiShiLength = m_PlateInfo[20];;
	//QString strTempCheckName = m_PlateInfo[22];;
	QString strTempCheckName = g_Renyuan;
//////////////////////////////////////////////////////////////////////////////////////////////////
	// 探伤标准
	QString strTempBiaoZhun = m_PlateInfo[34];;
	QString strBiaoZhun;
	if( strTempBiaoZhun.size() >= 20)	
	{	
		strBiaoZhun = QString("%1").arg(strTempBiaoZhun.left(20));
	}	
	else	
	{	QString temp;
		strBiaoZhun = QString("%1").arg(temp.sprintf("%-20s",strTempBiaoZhun.toLocal8Bit().data()));		
	}
	strDPIData += strBiaoZhun;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	QString strTempMaxQueXianLv = m_PlateInfo[35];
	// 钢种
	QString strTempGangZhong = m_PlateInfo[36];
	QString strTemZhiShiArea = m_PlateInfo[37];
	// 钢板长度	
	QString strLength = QString("%1").arg(strTempLength.sprintf("%-5s",strTempLength.toLocal8Bit().data()));	
	// 钢板宽度	
	QString strWidth = QString("%1").arg(strTempWidth.sprintf("%-4s",strTempWidth.toLocal8Bit().data()));
	// 钢板厚度	
	QString strHeight =QString("%1").arg(strTempHeight.sprintf("%-6s",strTempHeight.toLocal8Bit().data()));
	// 钢板重量		
	QString strWeight = QString("%1").arg(strTempWeight.sprintf("%-5s",strTempWeight.toLocal8Bit().data()));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 探伤级别
	QString strJiBie;	
	QString strBodyJiBie = m_PlateInfo[56];
	QString strEdgeJiBie =m_PlateInfo[58];
	strEdgeJiBie = strEdgeJiBie.trimmed();
	if (strEdgeJiBie == "")	
	{	
//		strJiBie.Format(_T("%-5s"), strBodyJiBie.Left(5));
		strBodyJiBie = strBodyJiBie.left(5);                 //中文和VC中有区别
		strJiBie = QString("%1").arg(strBodyJiBie.sprintf("%-5s",strBodyJiBie.toLocal8Bit().data()));
	}	
	else	
	{	
		strBodyJiBie = strBodyJiBie.trimmed();
		strEdgeJiBie = strEdgeJiBie.trimmed();
//		strJiBie.Format(_T("%-2s/%-2s"), strBodyJiBie, strEdgeJiBie);
		strJiBie.sprintf("%-2s/%-2s",strBodyJiBie.toLocal8Bit().data(),strEdgeJiBie.toLocal8Bit().data());
	}
	strDPIData += strJiBie;	
	// 探伤时间（年月日）
	strDPIData += strTempTime.mid(0,4) + strTempTime.mid(5,2) + strTempTime.mid(8,2);
	// 探伤时间（时分秒）	
	strDPIData += strTempTime.mid(11,2) + strTempTime.mid(14,2) + strTempTime.mid(17,2);
	// 探伤班次
	strClass = strClass.trimmed();
	strClass = strClass.left(1);
	if(strClass == tr("甲"))
	{
		strClass = tr("1");	
	}
	else if(strClass == tr("乙"))
	{
		strClass = tr("2");	
	}
	else if(strClass == tr("丙"))
	{
		strClass = tr("3");	
	}
	else if(strClass == tr("丁"))
	{
		strClass = tr("4");		
	}
	strDPIData += strClass;	
	// 探伤人员	
	QString strCheckName;
	if(strTempCheckName.size() >= 6)
	{
		int size1 = strlen(strTempCheckName.toLocal8Bit().data());
		if (size1 > strTempCheckName.size())    //判断是否有中文
		{
			strCheckName = strTempCheckName.left(3);	   //中文	
		}
		else
		{
			strCheckName = strTempCheckName.left(6);      //英文
		}
	}
	else
	{
		strCheckName = strTempCheckName.sprintf("%-6s",strTempCheckName.toLocal8Bit().data());
	}
	strDPIData += strCheckName;	
	// 探伤结果	
	QString strResult;	
	strTempResult = strTempResult.trimmed();
	if (g_strBiaoZhun == "GBT2970" || g_strBiaoZhun == "JBT4730")
	{
		if (strTempResult == tr("合格"))
		{
			if (g_strJiBie == "I")
			{
				strResult = tr("1");
			}
			else if (g_strJiBie == "II")
			{
				strResult = tr("2");
			}
			else
			{
				strResult = tr("3");
			}
				
		} 
		else
		{
			strResult = tr("3");	
		}
	}
	else if (g_strBiaoZhun == "BS_EN_ISO_10893")
	{
		if (strTempResult == tr("合格"))
		{
			if (g_strJiBie.contains("B1"))
			{
				strResult = tr("1");
			}
			else if (g_strJiBie.contains("B2"))
			{
				strResult = tr("2");
			}
			else
			{
				strResult = tr("3");
			}

		} 
		else
		{
			strResult = tr("3");	
		}
	}
	else if (g_strBiaoZhun == "GBT_47013_2015")
	{
		if (strTempResult == tr("合格"))
		{
			if (g_strJiBie.contains("I"))
			{
				strResult = tr("1");
			}
			else if (g_strJiBie.contains("II"))
			{
				strResult = tr("2");
			}
			else if (g_strJiBie.contains("III"))
			{
				strResult = tr("3");
			}
			else if (g_strJiBie.contains("IV"))
			{
				strResult = tr("4");
			}
			else
			{
				strResult = tr("5");
			}

		} 
		else
		{
			strResult = tr("3");	
		}
	}
	else
	{
		if (strTempResult == tr("合格"))
		{
			strResult = tr("1");	
		} 
		else
		{
			strResult = tr("3");	
		}
	}
	
	strDPIData += strResult;
	// 插入表
	//给一些字符串赋值
	QTime time2;
	time2= QTime::currentTime();
	qsrand(time2.msec()+time2.second()*1000);
	QDateTime tempTime = QDateTime::currentDateTime();
	QString strSer = QString("%1%2%3").arg(tempTime.toString("yyyy-MM-dd hh-mm-ss")).arg(" ").arg(qrand());//SER为当前时间加上随机数
	//QString strIp =QString("%1").arg("192.168.0.1");
	QString strIp =QString("%1").arg(" ");
	QString strCreateDate = tempTime.toString("yyyyMMdd");
	QString strCreateTime = tempTime.toString("hhmmss");

// 	strSQL2.Format(_T("insert TBDL02(SER, SYSTYPE, DATATYPE,DATAIP,RESULT,REASON,DATA,RETRYCOUNT,CJRQ,CJSJ,XGRQ,XGSJ)\ 
// 					values('%s',' ',' ','%s','0',' ', '%s',0,'%s','%s',' ',' ')"), \
// 					strSer,strIp,strDPIData,strCreateDate,strCreateTime);\

	QString strSQL2 = QString("%1%2%3%4%5%6%7%8%9%10%11%12")
						.arg("insert TBDL02(SER, SYSTYPE, DATATYPE,DATAIP,RESULT,REASON,DATA,RETRYCOUNT,CJRQ,CJSJ,XGRQ,XGSJ)values('")
						.arg(strSer).arg("',")
						.arg("' ',' ','").arg(strIp).arg("','0',' ', '")
						.arg(strDPIData).arg("',0,'")
						.arg(strCreateDate).arg("','")
						.arg(strCreateTime).arg("',' ',' ')");

	
/*	QString strSQL21 = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14")
						.arg("insert TBDL02(SER, SYSTYPE, DATATYPE,DATAIP,RESULT,REASON,DATA,RETRYCOUNT,CJRQ,CJSJ,XGRQ,XGSJ)values('")
						.arg(strSer).arg("',")
						.arg("' ',' ','").arg(strIp).arg("','").arg("'0'").arg(",' ', '")
						.arg(strDPIData).arg("',0,'")
						.arg(strCreateDate).arg("','")
						.arg(strCreateTime).arg("',' ',' ')");

	QMessageBox::critical(this,"插入数据命令1： ",strSQL2);
	QMessageBox::critical(this,"插入数据命令2： ",strSQL21);
*/

#if HAVE_DATA_BASE
	query.exec(strSQL2); 
	if (!query.isActive())
	{
		QMessageBox::critical(this,"SaveDPIData",tr("strSQL2插入数据失败"));
		return;
	}
	//
	QString strSQL = QString("%1%2%3%4%5%6%7%8").arg("update TBDL02 set RESULT = ").arg("'0'").arg(", ")
				.arg("REASON = ").arg("' '")
				.arg("where SER = '").arg(strSer).arg("'");
	query.exec(strSQL); 
	if (!query.isActive())
	{
		QMessageBox::critical(this,"SaveDPIData",tr("strSQL2更新数据失败"));
		return;
	}
#endif
	//
}
//chenyuzhu modify on 2014-7-28

void CCscanPanel::ChangeFileCheckID(const QString& strFileName, const QString& strNewID )
{
	QString strID = strNewID;
	unsigned short dwCheckID = strNewID.toLong();
	QFile file(strFileName);
	if (!file.exists(strFileName))
	{
		QMessageBox::information(NULL, "ChangeFileCheckID", tr("创建文件失败!!!"));
		return;
	}
	bool bRet = file.open(QIODevice::ReadWrite);
	if (bRet)
	{
		DWORD dwBytesRemaining = file.size();
		stDianShang tmp;
		while (dwBytesRemaining)
		{
			DWORD dwPosition = file.pos();
			UINT nBytesRead = file.read((char*)&tmp,sizeof(stDianShang));
			tmp.nCheckId = dwCheckID;
			file.seek(dwPosition);
			file.write((char*)&tmp,sizeof(stDianShang));
			dwBytesRemaining -= nBytesRead;
		}
	}
	else
	{
		QString strTemp = file.errorString();
		QMessageBox::critical(this,"ChangeFileCheckID",strTemp);
	}

}

void CCscanPanel::GetBodyAndEdgeLevel(QString& strBodyLevel,	QString& strEdgeLevel)
{
	// 从g_strJiBie里获取工艺的本体探伤等级
	QString strJiBie = g_strJiBie;
	/*int index = strJiBie.indexOf(tr("本体"));
	// 区分本体边缘
	if (index != -1)
	{
		int nIndex1 = strJiBie.indexOf(tr(" "),0);
		int nIndex2 = strJiBie.indexOf(tr("边缘"),0);
		int nIndex3 = strJiBie.indexOf(tr(" "), nIndex2);
		strBodyLevel = strJiBie.mid(nIndex1, nIndex2 - nIndex1 -1);
		strBodyLevel.trimmed();
		strEdgeLevel = strJiBie.right(strJiBie.size() - nIndex3 - 1);
		strEdgeLevel.trimmed();
	}
	else
	{
		strBodyLevel = strJiBie;
		strEdgeLevel = tr("");
	}*/
	int index = strJiBie.indexOf(tr("/"));
	// 区分本体边缘
	if (index != -1)
	{
		//int nIndex1 = strJiBie.indexOf(tr(" "),0);
		int nIndex2 = strJiBie.indexOf(tr("/"),0);
		//int nIndex3 = strJiBie.indexOf(tr(" "), nIndex2);
		strBodyLevel = strJiBie.left(nIndex2);
		strBodyLevel.trimmed();
		strEdgeLevel = strJiBie.right(nIndex2);
		strEdgeLevel.trimmed();
	}
	else
	{
		strBodyLevel = strJiBie;
		strEdgeLevel = tr("");
	}
}

//chenyuzhu modify start on 2014-7-9
bool CCscanPanel::SaveResult2DB()//保存结果chenyuzhu
{
    if (recognizer.isNull()) { // 判伤器    
        QMessageBox::information(NULL, "CCscanPanel::JiSuanBiaoZhun", "无效标准");
        return false;
    }

	QString strFileName = QDir::currentPath()+ "/DianShang.txt";
	QString strYasuoFileName = QDir::currentPath() + "/DianShang.rar";
	
	//chenyuzhu add start on 2014-7-9	
	//1、读取数据库获得PlateCheckInfoFile.dbfl文件
	
	QString strPlateCheckFileName = QDir::currentPath() + "/PlateCheckInfoFile.dbfl";//chenyuzhu add on 2014-7-9	
	QFile file1(strPlateCheckFileName);
	if (!file1.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
		QMessageBox::warning(NULL,"CCscanPanel::SaveResult2DB",QObject::tr("PlateCheckInfoFile.dbfl打开失败"));
		return FALSE;
	}

	PLATECHECKINFOFILE PlateCheckInfo;
	file1.seek(0);

	quint64 nBytesRead = file1.read((char*)&PlateCheckInfo,sizeof(PLATECHECKINFOFILE));//读文件到结构体中
	file1.close();
	//chenyuzhu add end on 2014-7-9

	//chenyuzhu modify on 2014-7-14
	g_CS.lock();
	QString strID = g_MaxCheckId;
	g_CS.unlock();

	bool bRet = FALSE;
	QString strSQL = QString();

#if HAVE_DATA_BASE
	strSQL = QString("%1%2%3").arg(tr("select * from T_PlateCheckInfo where id = '")).arg(strID).arg("'");
	QSqlQuery query(*g_sqlDB);
	bRet = query.exec(strSQL);
	query.next();
	if (query.value(0).isNull()) //如果数据库中没有相应的钢板号，有可能是重评
	{
		//向数据库中插入一条记录,重新获得最大的ID保存在g_MaxCheckId中，修改文件
		QString strSQLTemp(tr("insert T_PlateCheckInfo(Check_Class,Check_People,Device_ID,Part_ID) values('甲','system','1','1')"));
		query.exec(strSQLTemp);
	    strSQLTemp.clear();
		strSQLTemp.sprintf("%s","select max(id) as max_id from T_PlateCheckInfo(");
		query.exec(strSQLTemp);
		query.next();
		strID = query.value(0).toString();
		g_CS.lock();
		g_MaxCheckId = strID;
		g_CS.unlock();
		// 修改点伤文件数据里的CheckID
		ChangeFileCheckID(strFileName, strID);
	}
#else
	QMessageBox::information(NULL, tr("CCscanPanel::SaveResult2DB"), tr("未连接数据库"));
#endif

	//chenyuzhu modify on 2014-7-14
	//m_nPlateId = g_MaxCheckId.toInt();
	// 更新表项值
	QString strGongYi = g_strGongYi;
	strGongYi.trimmed();
    // 增加缺陷最大指示面积
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	double dMaxZhiShiArea = 0.0f;
	ZhiShiAreaRegion zhishiAreaRegion;
  /*  dMaxZhiShiArea;
    zhishiAreaRegion; 这两个值应从新的判伤器获得*/
 //   m_pBiaoZhun->CalcZhishiArea(dMaxZhiShiArea, zhishiAreaRegion);
	// 计算最大指示长度
	double dShangMaxLength = 0;
    //double dBodyShangMaxLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
    double dBodyShangMaxLength = recognizer->GetPartValue("MaxIndiLengthBody");
    //double dEdgeShangMaxLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();
    double dEdgeShangMaxLength = recognizer->GetPartValue("MaxIndiLengthEdge");
	dShangMaxLength = recognizer->GetPartValue("MaxIndiLengthAll");
	// 获取计算结果的详细信息
 //   DetailInfo info = m_pBiaoZhun->GetDetailInfo(strBiaoZhun);
    DetailInfo info = GetDetailInfo(strBiaoZhun);
	QString strBodyLevel, strEdgeLevel;
	GetBodyAndEdgeLevel(strBodyLevel, strEdgeLevel);
	double perBodyMaxQueXianCount = 0;
	double perEdgeMaxQueXianCount = 0;
	double BodyMaxZhiShiLength = 0;
	double EdgeMaxZhiShiLength = 0;
	double BodyAvgQueXianLv = 0;
	double EdgeAvgQueXianLv = 0;
	double BodyYanZhongQueXianNum = 0;
	double EdgeYanZhongQueXianNum = 0;
	double dMaxEdgeQueXianLv = 0;
	double dMaxBodyQueXianLv = 0;
	double dAllAvgQueXianLv = 0;
	double dAllMaxQueXianLv = 0;

	// 若为日本标准,替换名称相近但含义不同的值
//	if(!strBiaoZhun.compare(tr("JIS G0801"),Qt::CaseInsensitive))
//	{
//        perBodyMaxQueXianCount = g_pPlateDraw->dynamicCall("dynamicGetJISBodyMaxQueXianNumSquareMeter2()").toDouble();
//		perEdgeMaxQueXianCount = g_pPlateDraw->dynamicCall("GetJISEdgeMaxQueXianNumThreeMeters2()").toDouble();
//		BodyMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxDuijiaoLength()").toDouble();
//		EdgeMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxDuijiaoLength()").toDouble();
//		BodyAvgQueXianLv = g_pPlateDraw->dynamicCall("GetJISBodyAvgQueXianLv()").toDouble();
//		EdgeAvgQueXianLv = g_pPlateDraw->dynamicCall("GetJISEdgeAvgQueXianLv()").toDouble();
//		dMaxEdgeQueXianLv = g_pPlateDraw->dynamicCall("GetJISEdgeAvgQueXianLv()").toDouble();
//		dMaxBodyQueXianLv = g_pPlateDraw->dynamicCall("GetJISBodyAvgQueXianLv()").toDouble();

//		BodyYanZhongQueXianNum = g_pPlateDraw->dynamicCall("GetJISBodyAvgQueXianNumSquarMeter()").toDouble();
//		EdgeYanZhongQueXianNum = perEdgeMaxQueXianCount;

//		info.m_BodyMaxArea = 0;
//		info.m_BodyMaxLength = 0;
//		info.m_BodyMaxWidth = 0;
//		info.m_EdgeMaxArea = 0;
//		info.m_EdgeMaxLength = 0;
//		info.m_EdgeMaxWidth = 0;
//		info.m_perAllMaxQueXianCount = 0;
//	}
//	else if( !strBiaoZhun.compare(tr("JIS G0901"),Qt::CaseInsensitive))
//	{
//		dAllAvgQueXianLv = g_pPlateDraw->dynamicCall("GetJISBodyAvgQueXianLv()").toDouble();
//		unsigned int nMaxNumSquarMeter = g_pPlateDraw->dynamicCall("GetJISBodyMaxQueXianNumSquareMeter()").toInt();
//		dAllMaxQueXianLv = double(nMaxNumSquarMeter)/25.0;

//		info.m_BodyMaxArea = 0;
//		info.m_BodyMaxLength = 0;
//		info.m_BodyMaxWidth = 0;
//		info.m_EdgeMaxArea = 0;
//		info.m_EdgeMaxLength = 0;
//		info.m_EdgeMaxWidth = 0;
//		info.m_perAllMaxQueXianCount = 0;
//	}
//	else
//	{
    perBodyMaxQueXianCount = info.m_perBodyMaxQueXianCount;
    perEdgeMaxQueXianCount = info.m_perEdgeMaxQueXianCount;
    BodyMaxZhiShiLength = info.m_BodyMaxZhiShiLength;
    EdgeMaxZhiShiLength = info.m_EdgeMaxZhiShiLength;
    BodyAvgQueXianLv = info.m_BodyAvgQueXianLv;
    EdgeAvgQueXianLv = info.m_EdgeAvgQueXianLv;
    dMaxEdgeQueXianLv = info.m_EdgeMaxQueXianLv;
    dMaxBodyQueXianLv = info.m_BodyMaxQueXianLv;

    //dAllAvgQueXianLv = m_nSurfaceAreaSum/(g_Info[nPlate].length*g_Info[nPlate].width);
    //dAllMaxQueXianLv = g_pPlateDraw->dynamicCall("GetAllMaxQueXianLv()").toDouble();
    dAllAvgQueXianLv = recognizer->GetPartValue("AvgRatioAll");
    dAllMaxQueXianLv = recognizer->GetPartValue("MaxDensityAll");
    m_nSurfaceAreaSum = recognizer->GetPartValue("AreaSum");
 //   }
    if (!g_strBiaoZhun.compare(tr("GBT2970"),Qt::CaseInsensitive) ||
		!g_strBiaoZhun.compare(tr("JBT4730"),Qt::CaseInsensitive))
	{
    //	CBiaoZhun_GBT2970* pBiaoZhun = static_cast<CBiaoZhun_GBT2970*>(m_pBiaoZhun);
    //    dShangMaxLength = pBiaoZhun->m_dMaxLengthValue;  //现在的值为空的
    //    EdgeMaxZhiShiLength = pBiaoZhun->m_dEdgeMaxLength;
	}
	//chenyuzhu modify start on 2014-7-9
	g_CS.lock();
    //chenyuzhu add start on 2014-7-9

    PlateCheckInfo.nPlate_id = m_nPlateId;
    strcpy_s(PlateCheckInfo.strCheckClass, g_Banji.toAscii());
    strcpy_s(PlateCheckInfo.strCheckName, g_Renyuan.toAscii());
    QByteArray ba = strGongYi.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strCheckMethod, ba.data());
    QString qstr = m_strResult.trimmed();
    ba = qstr.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strResult, ba.data());
    PlateCheckInfo.nDianShangCount = g_dwFlawCount;
    PlateCheckInfo.nMianShangCount = m_nSurfaceFlawCount;
    PlateCheckInfo.fMianShangAllMianJi = m_nSurfaceAreaSum / 100.0f;
    PlateCheckInfo.fQueXianPercent = dAllAvgQueXianLv * 100.0f;
    PlateCheckInfo.fQueXianMaxLength = dShangMaxLength;
    PlateCheckInfo.fQuexianMaxWidth = dShangMaxLength;
    ba = strBodyLevel.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strCheckBodyLevel, ba.data());
    ba = strEdgeLevel.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strCheckEdgeLevel, ba.data());
    ba = strBiaoZhun.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strCheckBiaoZhun, ba.data());
    PlateCheckInfo.fAllMaxQueXianLv = dAllMaxQueXianLv*100.0f;
    PlateCheckInfo.fQueXianMaxArea = dMaxZhiShiArea / 100.0f;
    PlateCheckInfo.fBodyMaxZhiShiLength = BodyMaxZhiShiLength;
    PlateCheckInfo.fBodyMaxQueXianarea = info.m_BodyMaxArea;
    PlateCheckInfo.fBodyMaxLength = info.m_BodyMaxLength;
    PlateCheckInfo.fBodyMaxWidth = info.m_BodyMaxWidth;
    PlateCheckInfo.fEdgeMaxZhiShiLength = EdgeMaxZhiShiLength;
    PlateCheckInfo.fEdgeMaxArea = info.m_EdgeMaxArea;
    PlateCheckInfo.fEdgeMaxLength = info.m_EdgeMaxLength;
    PlateCheckInfo.fEdgeMaxWidth = info.m_EdgeMaxWidth;
    PlateCheckInfo.fBodyAvgQueXianLv = BodyAvgQueXianLv*100.0f;
    PlateCheckInfo.fEdgeAvgQueXianLv = EdgeAvgQueXianLv*100.0f;
    PlateCheckInfo.fBodyMaxQueXianLv = dMaxBodyQueXianLv*100.0f;
    PlateCheckInfo.fEdgeMaxQueXianLv = dMaxEdgeQueXianLv*100.0f;
    PlateCheckInfo.fBodyMaxCountSquarMeter = perBodyMaxQueXianCount;
    PlateCheckInfo.fEdgeMaxCountPerMeter = perEdgeMaxQueXianCount;
    PlateCheckInfo.fAllMaxCountSquarMeter = info.m_perAllMaxQueXianCount;
    PlateCheckInfo.fBodyYanZhongQueXianNum = BodyYanZhongQueXianNum;
    PlateCheckInfo.fEdgeYanZhongQueXianNum = EdgeYanZhongQueXianNum;


    //chenyuzhu add start on 2014-7-17
#if HAVE_DATA_BASE
    QSqlQuery query1(*g_sqlDB);
    QString strSql = QString("%1%2").arg("select Check_Time from T_PlateCheckInfo where ID =").arg(g_MaxCheckId);//chenyuzhu add on 2014-7-17
    query1.exec(strSql);
    query1.next();
    qstr = query1.value(0).toString();
    qstr = qstr.trimmed();
    ba = qstr.toLocal8Bit();
    strcpy_s(PlateCheckInfo.strCheckTime, ba.data());
    //chenyuzhu add end on 2014-7-17
#endif

    if (!file1.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QMessageBox::warning(NULL,"CCscanPanel::SaveResult2DB",QObject::tr("PlateCheckInfoFile.dbfl打开失败"));
        return FALSE;
    }

    file1.seek(0);
    quint64 nBytesWrite = file1.write((char*)&PlateCheckInfo,sizeof(PLATECHECKINFOFILE));//读文件到结构体中
    file1.close();

    //chenyuzhu add start on 2014-7-14
    //将PlateCheckInfoFile.dbfl探伤结果文件存入数据库中
    QString strYasuoPlateCheckFileName = QDir::currentPath() + "/PlateCheckInfoFile.rar";
    bRet = g_pPlateDraw->dynamicCall("ZipFile(QString* ,QString* )",strPlateCheckFileName,strYasuoPlateCheckFileName).toBool();
    if (!bRet)
    {
        QMessageBox::information(this,"SaveResult2DB",tr("压缩探伤结果文件失败!"));
        return false;
    }
    //->把压缩包存到数据库里，id为t_PlateCheckInfo
    QFile file2(strYasuoPlateCheckFileName);
    file2.open(QIODevice::ReadOnly);
    QByteArray data1 = file2.readAll();
    QVariant var1(data1);
	
#if HAVE_DATA_BASE
    strSQL = QString("%1").arg("update T_PlateCheckInfo set PlateCheck_File =? where ID = ?");
    query.clear();
    query.prepare(strSQL);

    query.addBindValue(var1);
    query.addBindValue(strID);
    bRet = query.exec();
#endif
    //<-chenyuzhu add end on 2014-7-14

    //chenyuzhu add end on 2014-7-9
    g_CS.unlock();
//	query.clear();
//	query.exec(Sql2);
    //chenyuzhu modify end on 2014-7-9

    //chenyuzhu add start on 2014-7-14
    //压缩设备文件保存到数据库中
    QString strSheBeiFileName = QDir::currentPath() + "/SheBeiFile.dbfl";
    QString strYasuoSheBeiFileName = QDir::currentPath() + "/SheBeiFile.rar";
    bRet = g_pPlateDraw->dynamicCall("ZipFile(QString* ,QString* )",strSheBeiFileName,strYasuoSheBeiFileName).toBool();
    if (!bRet)
    {
        QMessageBox::information(this,"SaveResult2DB",tr("压缩设备文件失败!"));
        return false;
    }
    QFile file3(strYasuoSheBeiFileName);
    file3.open(QIODevice::ReadOnly);
    QByteArray data3 = file3.readAll();
    QVariant var3(data3);

#if HAVE_DATA_BASE
    strSQL = QString("%1").arg("update T_PlateCheckInfo set Device_File =? where ID = ?");
    query.clear();
    query.prepare(strSQL);
    query.addBindValue(var3);
    query.addBindValue(strID);
    bRet = query.exec();
#endif
    //chenyuzhu add end on 2014-7-14

    //chenyuzhu add start on 2014-7-14
    //压缩钢板文件保存到数据库中
    QString strPlateFileName = QDir::currentPath() + "/platefile.dbfl";
    QString strYasuoPlateFileName = QDir::currentPath() + "/platefile.rar";
	if (!g_pPlateDraw->isNull()) {
		bRet = g_pPlateDraw->dynamicCall("ZipFile(QString* ,QString* )",strPlateFileName,strYasuoPlateFileName).toBool();
		if (!bRet) {
			QMessageBox::information(this, "SaveResult2DB", tr("压缩钢板文件失败,请确认文件是否存在或是否注册了正确的OCX控件"));
			return false;
		}
	} 

    QFile file4(strYasuoPlateFileName);
    file4.open(QIODevice::ReadOnly);
    QByteArray data4 = file4.readAll();
    QVariant var4(data4);

#if HAVE_DATA_BASE
    strSQL = QString("%1").arg("update T_PlateCheckInfo set Plate_File =? where ID = ?");
    query.clear();
    query.prepare(strSQL);
    query.addBindValue(var4);
    query.addBindValue(strID);
    bRet = query.exec();
#endif
    //chenyuzhu add end on 2014-7-14

    ReadDatatoList();//为钢板信息链表赋值chenyuzhu add on 2014-7-28

    SaveDPIData();
    //插入点
    //下面向t_MianShang表中插入数据，但是m_surfaceFlawInfoList没有地方插入数据（参考原总站），t_MianShang表也是空的
    //不清楚需求，还需确定，这里只是按照原总站将下面的代码加上
    QString strSQLAll;
    QPoint p;
    int nCount = 0;
    int nSize = m_surfaceFlawInfoList.size();
    for (int i=0; i<nSize; i++)
    {
        nCount++;

            SurfaceFlawInfo info = m_surfaceFlawInfoList.at(i);
            strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12")
                .arg("select").arg(info.nMinX).arg(",")
                .arg(info.nMaxX).arg(",").arg(info.nMinY).arg(",")
                .arg(info.nMaxY).arg(",").arg(info.fShangLength).arg(",").arg(strID);
            if (strSQLAll == "")
            {

                strSQLAll = QString("%1").arg("insert into t_MianShang(MianShangXBegin,MianShangXEnd,MianShangYBegin,MianShangYEnd,MianShangZ,MianShangMianJi,ZhiShiChangDu,Check_id");
                strSQLAll += strSQL;
            }
            else
            {
                strSQLAll += QString(" union all ");
                strSQLAll += strSQL;
            }
            if (nCount > 100)
            {
#if HAVE_DATA_BASE
                query.exec(strSQLAll);
#endif
                strSQLAll = "";
                nCount    = 0;
            }
    }

    if (!strSQLAll.isEmpty())
    {
#if HAVE_DATA_BASE
        query.exec(strSQLAll);
#endif
        strSQLAll = tr("");
        nCount    = 0;
    }
//	bRet = g_pPlateDraw->dynamicCall("ZipFile(QString* ,QString* )",strFileName.toLocal8Bit().data(),strYasuoFileName.toLocal8Bit().data()).toBool();
    //将伤点文件生成压缩包
    bRet = g_pPlateDraw->dynamicCall("ZipFile(QString* ,QString* )",strFileName,strYasuoFileName).toBool();
    if (!bRet) {
        QMessageBox::information(this,"SaveResult2DB",tr("压缩点伤文件失败!"));
        return false;
    }

    // 把点伤文件压缩包存到t_DianShang2数据库里，id为t_PlateCheckInfo
    QFile file(strYasuoFileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    QVariant var(data);

#if HAVE_DATA_BASE
    strSQL = QString("%1").arg("update T_PlateCheckInfo set Data_File =? where ID = ?");//chenyuzhu modify on 2014-7-14
    query.clear();
    query.prepare(strSQL);
    query.addBindValue(var);
    query.addBindValue(strID);
    bRet = query.exec();
#endif

    //chenyuzhu add start on 2014-7-16
    QString strPlateParamFileName = QDir::currentPath()+ "/platefile.dbfl";
    QFile Platefile(strPlateParamFileName);
    if (!Platefile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(NULL,"AScanSave",QObject::tr("platefile.dbfl打开失败"));
        //	return ;
    }
    PLATEFILE_PARAM PlateParam;

    Platefile.seek(0);

    quint64 nParamBytesRead = Platefile.read((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
    Platefile.close();

    int nJieGuo;
    QString strJieGuo = m_strResult.trimmed();
    if (strJieGuo == tr("合格")) {
        nJieGuo = 1;
    } else {
        nJieGuo = 0;
    }

    QString strSize = QString("%1%2%3%4%5").arg(g_Info[nPlate].length).arg("*").arg(g_Info[nPlate].width).arg("*").arg(g_Info[nPlate].height);
    //chenyuzhu add end on 2014-7-16
    //chenyuzhu add start on 2014-7-15
    //更新其他字段到T_PlateCheckInfo
    strSQL = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18%19%20%21%22%23%24%25%26%27%28%29%30%31%32%33%34%35%36%37%38%39%40%41%42%43%44%45").arg("update T_PlateCheckInfo set Part_ID = '").arg(g_Info[nPlate].plateno).arg("', ")
        .arg("Device_Type = '").arg(" ").arg("', ")
        .arg("Sample_Flag = '").arg(PlateParam.nflag).arg("', ")
        .arg("Check_Class = '").arg(g_Banji).arg("', ")
        .arg("Check_People = '").arg(g_Renyuan).arg("', ")
        .arg("Check_Standard = '").arg(strBiaoZhun).arg("', ")
        .arg("Check_Model = '").arg(" ").arg("', ")
        .arg("Result = '").arg(nJieGuo).arg("', ")
        .arg("Size = '").arg(strSize).arg("', ")
        .arg("Weight = ").arg(PlateParam.fWeight).arg(", ")
        .arg("Material = '").arg(strGongYi).arg("', ")
        .arg("MianShangAllMianJi = ").arg(PlateCheckInfo.fMianShangAllMianJi).arg(", ")
        .arg("CheckBodyLevel = '").arg(strBodyLevel).arg("',")
        .arg("CheckEdgeLevel = '").arg(strEdgeLevel).arg("' ")
        .arg("where ID = '").arg(strID).arg("'");

#if HAVE_DATA_BASE
    //QMessageBox::information(this,"SaveResult2DB",strSQL);
    query.clear();
    query.exec(strSQL);
/*	QString aa = "update T_PlateCheckInfo set Part_ID = 'qqq', Device_Type = ' ', Sample_Flag = '1', Check_Class = ' ', Check_People = ' ', Check_Standard = 'GBT2970', Check_Model = ' ', Result = '1', Size = '5555*3222*30', Weight = 8.43, Material = '黑材', MainShangAllMianJi = 0, CheckBodyLevel = ' ', CheckEdgeLevel = ' ' where ID = '35'";
    query.clear();
    query.exec(aa);*/
    //chenyuzhu add end on 2014-7-15
#endif
    return true;
}
//chenyuzhu modify end on 2014-7-9

//chenyuzhu modify start on 2014-7-17
void CCscanPanel::ReadData( long CheckID, QVector<QString>& strArray)
{
	
	for (int i=0;i<strArray.size();i++)
	{
		/*QSqlQuery query(*g_sqlDB);
		QString strSql = QString("%1%2%3").arg(tr("select * from vwPlate_Stat where id = '")).arg(CheckID).arg("'");
		bool bRet = query.exec(strSql);
		query.next();*/
		QString strKey = strArray[i];
		QString strField, strValue;
		int nFieldNum = -1;
		strField = m_PreviewWnd.m_titleMapReverse[strKey];
		if (strField.isEmpty())
			continue;
		nFieldNum = m_PreviewWnd.m_titleToNumber[strField];
		if (nFieldNum == -1)
			continue;
      //  strValue = query.value(nFieldNum).toString();
		strValue = m_PlateInfo[nFieldNum]; //chenyuzhu add on 2014-7-17
		if (strKey == tr("长度(mm)"))
		{
			m_PreviewWnd.m_nPlateLength = strValue.toInt();
			m_PreviewWnd.m_nPlateLength = strValue.toInt();
		}

		if (strKey == tr("宽度(mm)"))
		{
			m_PreviewWnd.m_nPlateWidth = strValue.toInt();
		}

		if (strKey == tr("厚度(mm)"))
		{
			m_PreviewWnd.m_nPlateHeight = strValue.toInt();
		}

		if (strKey == tr("重量(吨)"))
		{
			int index = strValue.indexOf(tr("."));
			if (index != -1)
			{
				strValue = strValue.left(index+3);
			}
		}
		// 若为探伤级别
		if (strKey == tr("探伤级别") || strKey == tr("Acceptance level"))
		{
			QString strEdgeValue;
			int nFiledNum = -1;
			nFiledNum = m_PreviewWnd.m_titleToNumber[tr("TanShangJiBieEdge")];
			if (-1 == nFiledNum)
			{
				QMessageBox::information(this,"ReadData",tr("TanShangJiBieEdge的Map有误"));
			}
 			//strEdgeValue = query.value(nFiledNum).toString();
			strEdgeValue = m_PlateInfo[nFiledNum]; //chenyuzhu add on 2014-7-17
			if (" E2" == strEdgeValue && !g_strBiaoZhun.compare(tr("BS_EN_ISO_10893"),Qt::CaseInsensitive))
			{
				strEdgeValue = tr(" E2(6.0)");
			}
// 			strValue = strValue + tr(" / ") + strEdgeValue;
			if ("" == strEdgeValue)
			{
				strValue = strValue;
			}
			else
			{
				strValue = strValue + QString::fromLocal8Bit(" / ") + strEdgeValue;
			}
		}
		if (strKey == tr("检测时间"))
		{
			int nIndex = strValue.indexOf(QString("T"));
			strValue = strValue.left(nIndex) + " " + strValue.right(strValue.size() - nIndex-1);
		}
		if (strKey == tr("缺陷总面积(mm^2)") || strKey == tr("缺陷占积率(%)") || strKey == tr("最大单位面积占积率(%)") ||
			strKey == tr("缺陷最大指示面积(mm^2)") || strKey == tr("缺陷最大指示长度(mm)") ||
			strKey == tr("缺陷最大指示宽度(mm)") || strKey == tr("本体最大指示长度(mm)") ||
			strKey == tr("本体最大缺陷面积(mm^2)") || strKey == tr("本体最大长度(mm)") ||
			strKey == tr("本体最大宽度(mm)") || strKey == tr("边缘最大指示长度(mm)") ||
			strKey == tr("边缘最大缺陷面积(mm^2)") || strKey == tr("边缘最大长度(mm)") || strKey == tr("边缘最大宽度(mm)") ||
			strKey == tr("本体平均缺陷率(%)") || strKey == tr("边缘平均缺陷率(%)") || strKey == tr("本体最大缺陷率(%)") ||
			strKey == tr("边缘最大缺陷率(%)") || strKey == tr("本体缺陷数/m^2") || strKey == tr("边缘缺陷数/m") ||
			strKey == tr("缺陷数/m^2") || strKey == tr("指示长度(mm)"))
		{
			int index = strValue.indexOf(tr("."));
			if (index != -1)
			{
				int Length = strValue.size();
				if (Length>(index+4))
				{
					strValue = strValue.left(index+3);
				}				
			}
            QJsonObject obj = GetObjForBiaozhun();
			QString strAdd;
			QString strBiaoZhun = g_strBiaoZhun;
			strBiaoZhun.trimmed();
			if(!strBiaoZhun.compare(tr("GBT2970"),Qt::CaseInsensitive) ||
				!strBiaoZhun.compare(tr("JBT4730"),Qt::CaseInsensitive))
			{
				if (strKey.indexOf(tr("指示长度")) != -1)
				{
					//还未加标准
                    strAdd.sprintf("%.1f", obj["MaxAllowedLength"].toDouble());
					strValue += tr("(") + strAdd + tr(")");
				}
				else if (strKey.indexOf(tr("面积")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxAllowedArea"].toDouble());
					strValue += tr("(") + strAdd + tr(")");
				}
				else if (strKey.indexOf(tr("缺陷率")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxAllowedPercent"].toDouble());
					strValue += tr("(") + strAdd + tr(")");
				}
			}
			else if (!strBiaoZhun.compare(tr("ASTM 435"),Qt::CaseInsensitive) ||
				!strBiaoZhun.compare(tr("ASTM 435M"),Qt::CaseInsensitive))
			{
				if (strKey.indexOf(tr("指示长度")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxAllowedLength"].toDouble());
                    strValue += tr("(") + strAdd + tr(")");
				}			
			}
			else if (!strBiaoZhun.compare(tr("ASTM 578"),Qt::CaseInsensitive) ||
					 !strBiaoZhun.compare(tr("ASTM 578M"),Qt::CaseInsensitive))
			{
				if (strKey.indexOf(tr("指示长度")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxAllowedLength"].toDouble());
                    strValue += tr("(") + strAdd + tr(")");
				}
			}
			else if (!strBiaoZhun.compare(tr("ISO12094"),Qt::CaseInsensitive))
			{
				if (strKey.indexOf(tr("边缘最大指示长度")) != -1)
				{
//					strAdd.Format("%.1f",m_pBiaoZhun->m_fBiaoZhunEdgeZhiShiLength);
//					strAdd.sprintf("%.1f",m_pBiaoZhun->m_fBiaoZhunEdgeZhiShiLength);
//					strValue += tr("(") + strAdd + tr(")");
				}
				else if (strKey.indexOf(tr("本体平均缺陷率")) != -1)
				{
//					strAdd.Format("%.1f",m_pBiaoZhun->m_fBiaoZhunBodyGeneralQueXianLv);
//					strAdd.sprintf("%.1f",m_pBiaoZhun->m_fBiaoZhunBodyGeneralQueXianLv);
//					strValue += tr("(") + strAdd + tr(")");
				}
				else if (strKey.indexOf(tr("本体最大缺陷率")) != -1)
				{
//					strAdd.Format("%.1f",m_pBiaoZhun->m_fBiaoZhunBodyMaxQueXianLv);
//					strAdd.sprintf("%.1f",m_pBiaoZhun->m_fBiaoZhunBodyMaxQueXianLv);
//					strValue += tr("(") + strAdd + tr(")");
				}	
				else if (strKey.indexOf(tr("边缘缺陷数")) != -1)
				{
//					strAdd.Format("%.1f",m_pBiaoZhun->m_fBiaoZhunEdgeQueXianNum);
//					strAdd.sprintf("%.1f",m_pBiaoZhun->m_fBiaoZhunEdgeQueXianNum);
//					strValue += tr("(") + strAdd + tr(")");
				}
			}
			else if (!strBiaoZhun.compare(tr("PrEn10160"),Qt::CaseInsensitive))
			{
				if (strKey.indexOf(tr("边缘最大指示长度")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxAllowedLengthEdge"].toDouble());
                    strValue += tr("(") + strAdd + tr(")");
				}
				else if (strKey.indexOf(tr("本体缺陷数")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxDensityNumBody"].toDouble());
                    strValue += tr("(") + strAdd + tr(")");
                }
				else if (strKey.indexOf(tr("边缘缺陷数")) != -1)
				{
                    strAdd.sprintf("%.1f", obj["MaxPerMeterNumEdge"].toDouble());
                    strValue += tr("(") + strAdd + tr(")");
				}	

			}
			else if (!strBiaoZhun.compare(tr("JIS G0801"),Qt::CaseInsensitive))
			{

			}
			else if (!strBiaoZhun.compare(tr("JIS G0901"),Qt::CaseInsensitive))
			{

			}
            else if (!strBiaoZhun.compare(tr("BS_EN_ISO_10893"),Qt::CaseInsensitive))
            {

            }
			else if (!strBiaoZhun.compare(tr("NBT_47013_2015"),Qt::CaseInsensitive))
			{

			}
			else
				QMessageBox::information(this,"ReadData",tr("所选标准程序未采用!"));

		}
		// 暂时写死
		if (strKey == tr("探头型号") || strKey == tr("Probe")) {
			strValue = tr("5M 双晶");
			if (!g_strBiaoZhun.compare(tr("NBT_47013_2015"), Qt::CaseInsensitive)) {
				if (g_strProbeType == tr("5M 单晶")) {
					strValue = tr("5M 单晶");					// 探头类型
				}	
			}
		}

		m_Map[strKey] = strValue;
	}		
}
//chenyuzhu modify end on 2014-7-17

void CCscanPanel::ReadDBValue(long CheckID)
{
	//ReadDatatoList();//为钢板信息链表赋值chenyuzhu add on 2014-7-17
	//读数据库，向Map里加值
 	ReadData(CheckID,m_PreviewWnd.m_plateArray);
 	ReadData(CheckID,m_PreviewWnd.m_TanShangArray);
 	ReadData(CheckID,m_PreviewWnd.m_ResultArray);	
}

QString CCscanPanel::ChineseToEnglish(QString str)
{
	if(g_bEnglish == true)
	{
		if (tr("板材自动超声检测报告") == str)
		{
			str = tr("Plate Automated UT Report");
		}
		if (tr("钢板基本信息") == str)
		{
			str = tr("Product information");
		}
		if (tr("探伤基本信息") == str)
		{
			str = tr("Testing information");
		}
		if (tr("探伤结果信息") == str)
		{
			str = tr("Test results");
		}
		if (tr("水膜法") == str)
		{
			str = tr("Water film");
		}
		if (tr("水") == str)
		{
			str = tr("Water");
		}
		if (tr("不合格") == str)
		{
			str = tr("Reject");
		}
		if (tr("合格") == str)
		{
			str = tr("Qualified");
		}
		if (tr("5M 双晶") == str)
		{
			str = tr("5M Double transducer");
		}
		if (tr("检 查 人 员") == str )
		{
			str = tr("Prosecutors");
		}
		if (tr("报告签发人员") == str )
		{
			str = tr("Signee");
		}
		if (tr("责任工程师") == str )
		{
			str = tr("Chief technician");
		}
		if (tr("技 术 资 格") == str )
		{
			str = tr("Qualifications");
		}
		if (g_OtherParameters.chineseReportTitle == str)
		{
			str = g_OtherParameters.englishReportTitle;
		}
	}
	return str;
}
//ZGJ/ZY02W8RCL20
//chenyuzhu modify start on 2014-10-31
void CCscanPanel::DrawPrintText(QRect& rect,QPainter& painter)
{
	int font1;
	int font2;
	int font3;
	int nwidth;
	if(g_bEnglish == false)
	{
		font1 = 13;
		font2 = 12;
		font3 = 10;
		nwidth = 4;
	}
	else
	{
		font1 = 13;
		font2 = 12;
		font3 = 9;
		nwidth = 3;
	}
	painter.setFont(QFont ("OCR A",font1));
	int worldHeight = painter.fontMetrics().height() + painter.fontMetrics().leading();

	QString str = tr("板材自动超声检测报告");
	str = ChineseToEnglish(str);
	painter.drawText(QRect(0,0,rect.width(),worldHeight),str,QTextOption(Qt::AlignHCenter));

	painter.setFont(QFont ("OCR A",font3));
	int worldHeight1 = painter.fontMetrics().height() + painter.fontMetrics().leading();
	str = tr("ZGJ/ZY02W8RCL20");
	painter.drawText(QRect(0, 5, rect.width() - 15, worldHeight1), str, QTextOption(Qt::AlignRight));

	int useHeight = worldHeight;   
	painter.setFont(QFont("OCR A",font3));
	int eachHeight = (rect.height()-useHeight)/4;  //4份
    // 打印钢板信息
	painter.setFont(QFont("OCR A",font2));
	worldHeight = painter.fontMetrics().height() + painter.fontMetrics().leading();

	str = tr("钢板基本信息");
	str = ChineseToEnglish(str);
	painter.drawText(QRect(0,useHeight,rect.width(),useHeight+worldHeight),str,QTextOption(Qt::AlignLeft));
	int eachUseHeight = worldHeight;
	int nSelectNum = m_PreviewWnd.m_plateArray.size();
	painter.setFont(QFont("OCR A",font3));
	worldHeight = painter.fontMetrics().height() - 2 * painter.fontMetrics().leading();
    int numXLine = (eachHeight-eachUseHeight)/worldHeight;
	int yDistance = rect.width()/nwidth;
    int currentRow = 0;   //当前的行
    int currentCol = 0;   //当前的列
	QString strKey;
	QString strValue;
    for (int iLoop = 0; iLoop < nSelectNum; iLoop++)
	{
		strKey = m_PreviewWnd.m_plateArray[iLoop];
		strValue = m_Map[strKey];
		strKey = ChineseToEnglish(strKey);
		strValue = ChineseToEnglish(strValue);
		strKey += tr(": ");
        painter.drawText(currentCol*yDistance,(currentRow+1)*worldHeight+eachUseHeight+useHeight + 10,strKey+strValue);
		currentCol++;
        if (currentCol >= nwidth)
		{
			currentCol=0;
			currentRow++;
			eachUseHeight += worldHeight;
		}
		if (currentRow >= numXLine)
		{
			QMessageBox::information(this,"DrawPrintText",tr("太多了写不下了！！"));
			return;
		}		

	}
	useHeight = useHeight + eachHeight + 6;
	currentRow =0;   //当前的行
	currentCol =0;   //当前的列
	// 显示探伤基本信息
	painter.setFont(QFont("OCR A",font2));
	worldHeight = painter.fontMetrics().height() + painter.fontMetrics().leading();

	str = tr("探伤基本信息");
	str = ChineseToEnglish(str);
    painter.drawText(QRect(0, useHeight, rect.width(), useHeight+worldHeight), str, QTextOption(Qt::AlignLeft));
	eachUseHeight = worldHeight;
	painter.setFont(QFont("OCR A",font3));
	worldHeight = painter.fontMetrics().height() - 2 * painter.fontMetrics().leading();
	int nTanshangSelectedNum = m_PreviewWnd.m_TanShangArray.size();
	for (int iLoop = 0; iLoop <nTanshangSelectedNum;iLoop++)
	{
		strKey = m_PreviewWnd.m_TanShangArray[iLoop];
		strValue = m_Map[strKey];
		strKey = ChineseToEnglish(strKey);
		strValue = ChineseToEnglish(strValue);
		strKey += tr(": ");
		painter.drawText(currentCol*yDistance,(currentRow+1)*worldHeight+eachUseHeight+useHeight +10,strKey+strValue);
		currentCol++;
		//chenyuzhu add start on 2014-9-29
		if (strKey == tr("检测时间: ") || (strKey == tr("Date of test: ")))
		{
			currentCol++;
		}
		//chenyuzhu add end on 2014-9-29
		if (currentCol >= nwidth)
		{
			currentCol=0;
			currentRow++;
			eachUseHeight += worldHeight;
		}
		if (currentRow >= numXLine)
		{
			QMessageBox::information(this,"DrawPrintText",tr("太多了写不下了！！"));
			return;
		}		
	}	
	useHeight += eachHeight + 6;
    currentRow = 0;   //当前的行
    currentCol = 0;   //当前的列
	// 显示探伤结果信息
	painter.setFont(QFont("OCR A",font2));
	worldHeight = painter.fontMetrics().height() + painter.fontMetrics().leading();
	str = tr("探伤结果信息");
	str = ChineseToEnglish(str);
	painter.drawText(QRect(0,useHeight,rect.width(),useHeight+worldHeight),str,QTextOption(Qt::AlignLeft));
	eachUseHeight = worldHeight;
	painter.setFont(QFont("OCR A",font3));
	worldHeight = painter.fontMetrics().height() - 2 * painter.fontMetrics().leading();
	int nResultSelectedNum = m_PreviewWnd.m_ResultArray.size();
    numXLine = (eachHeight*2-eachUseHeight)/worldHeight;
	yDistance = rect.width()/3;
    for (int iLoop = 0; iLoop < nResultSelectedNum; iLoop++)
	{
		strKey = m_PreviewWnd.m_ResultArray[iLoop];
		strValue = m_Map[strKey];
		strKey = ChineseToEnglish(strKey);
		strValue = ChineseToEnglish(strValue);
		strKey += tr(": ");
        painter.drawText(currentCol * yDistance, (currentRow+1) * worldHeight + eachUseHeight + useHeight + 10, strKey + strValue);
		//if (strValue == tr("合格") || strValue ==tr("Qualified"))
		if (g_bJieguo == false)
		{
			break;
		}
		currentCol++;
		if (currentCol >= 3)
		{
			currentCol=0;
			currentRow++;
			eachUseHeight += worldHeight;
		}
		if (currentRow >= numXLine)
		{
			QMessageBox::information(this,"DrawPrintText",tr("太多了写不下了！！"));
			return;
		}		
	}

	int col1;
	int col2;
	int col3;
	int col4;
	if(g_bEnglish == false)
	{
		col1 = 0;
		col2 = 1;
		col3 = 3;
		col4 = 4;
		yDistance = rect.width()/9;
	}
	else
	{
		col1 = 0;
		col2 = 1;
		col3 = 3;
		col4 = 4;
		yDistance = rect.width()/6;
	}
	//第一行
	currentRow += 3;
	eachUseHeight += worldHeight + 10;
	str = tr("检 查 人 员");
	str = ChineseToEnglish(str);
	painter.drawText(yDistance * col1, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	str = tr(": ________________");
	painter.drawText(yDistance * col2, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	str = tr("技 术 资 格");
	str = ChineseToEnglish(str);
	painter.drawText(yDistance * col3 , (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	str = tr(": ________________");
	painter.drawText(yDistance * col4, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	//第二行
	currentRow += 1;
	eachUseHeight += worldHeight + 10;
	str = tr(": ________________");
	painter.drawText(yDistance * col2, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);

	str = tr(": ________________");
	painter.drawText(yDistance * col4, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	//第三行
	/*currentRow += 1;
	eachUseHeight += worldHeight;
	str = tr(": ________________");
	painter.drawText(yDistance * col2, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);

	str = tr(": ________________");
	painter.drawText(yDistance * col4, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	*/
	//第四行
	currentRow += 1;
	eachUseHeight += worldHeight + 10;
	str = tr("报告签发人员");
	str = ChineseToEnglish(str);
	painter.drawText(yDistance * col1, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);

	str = tr(": ________________");
	painter.drawText(yDistance * col2, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	str = tr(": ________________");
	painter.drawText(yDistance * col4, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);

	//第五行
	currentRow += 1;
	eachUseHeight += worldHeight + 10;
	str = tr("责任工程师");
	str = ChineseToEnglish(str);
	painter.drawText(yDistance * col1, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);

	str = tr(": ________________");
	painter.drawText(yDistance * col2, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	str = tr(": ________________");
	painter.drawText(yDistance * col4, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	
	currentRow += 1;
	eachUseHeight += worldHeight;
	painter.setFont(QFont ("OCR A",font3));
	int worldHeight2 = painter.fontMetrics().height() + painter.fontMetrics().leading();
	str = g_OtherParameters.chineseReportTitle;
	str = ChineseToEnglish(str);
	//painter.drawText(yDistance * col1, (currentRow+1) * worldHeight + eachUseHeight + useHeight, str);
	painter.drawText(QRect(0, (currentRow+1) * worldHeight + eachUseHeight + useHeight, rect.width() - 15, worldHeight2), str, QTextOption(Qt::AlignRight));

}
//chenyuzhu modify end on 2014-10-31

void CCscanPanel::DrawEasyVScale(QPoint nStart,QPoint nEnd,int nNum,QPainter& painter,int nMax)
{
	for (int i=0;i<nNum;i++)
	{
		if (nStart.y() < nEnd.y())  
		{
			
			QPoint temp = nStart;
			double step  = (nEnd.y() - nStart.y())/(nNum-1);
			temp.setY(temp.y() + i * step);
			if (i == (nNum-1) )
			{
				painter.drawLine(QPoint(temp.x()+1,nEnd.y()+1),QPoint(temp.x()+5,nEnd.y()+1));
				painter.drawText(QPoint(temp.x()+10,nEnd.y()-1),QString::number(nMax)+QString(" [mm]"));				
			}
			else
			{
				painter.drawLine(QPoint(temp.x()+1,temp.y()),QPoint(temp.x()+5,temp.y()));
				int nTemp = i * nMax / nNum;
				painter.drawText(QPoint(temp.x()+10,temp.y()+5),QString::number(nTemp));
			}
		}
		else
		{
			QPoint temp = nEnd;
			double step  = (nStart.y() - nEnd.y())/(nNum-1);
			temp.setY(temp.y() + step);
			if (i == (nNum-1) )
			{
				painter.drawLine(QPoint(temp.x()+1,nStart.y()+1),QPoint(temp.x()+5,nStart.y()+1));
				painter.drawText(QPoint(temp.x()+10,nStart.y()-1),QString::number(nNum) + QString(" [mm]"));
			}
			else
			{
				painter.drawLine(QPoint(temp.x()+1,temp.y()),QPoint(temp.x()+5,temp.y()));
				int nTemp = i * nMax / nNum;
				painter.drawText(QPoint(temp.x()+10,temp.y()+5),QString::number(nTemp));
			}
		}
	}
	
}
void CCscanPanel::DrawEasyHScale(QPoint nStart,QPoint nEnd,int nNum,QPainter& painter,int nMax)
{
	for (int i=0;i<nNum;i++)
	{
		if (nStart.x() < nEnd.x())  
		{
			QPoint temp = nStart;
			double step  = (nEnd.x() - nStart.x())/(nNum-1);
			temp.setX(temp.x() + i * step);
			if (i == (nNum-1) )
			{
				painter.drawLine(QPoint(nEnd.x(),nEnd.y()+2),QPoint(nEnd.x(),nEnd.y()+5));
				painter.drawText(QPoint(temp.x()-8,nEnd.y()+20),QString::number(nMax) + QString(" [mm]"));
			}
			else
			{
				painter.drawLine(QPoint(temp.x(),temp.y()+2),QPoint(temp.x(),temp.y()+5));
				int nTemp = i * nMax / nNum;
				painter.drawText(QPoint(temp.x(),temp.y()+20),QString::number(nTemp));
			}
		}
		else
		{
			QPoint temp = nEnd;
			double step  = (nStart.y() - nEnd.y())/(nNum-1);
			temp.setY(temp.y() + step);
			if (i == (nNum-1) )
			{
				painter.drawLine(QPoint(nStart.x(),nStart.y()+2),QPoint(nStart.x(),nStart.y()+5));
				painter.drawText(QPoint(temp.x()-8,nStart.y()+20),QString::number(nMax) + QString(" [mm]"));
			}
			else
			{
				painter.drawLine(QPoint(temp.x(),temp.y()+2),QPoint(temp.x(),temp.y()+5));
				int nTemp = i * nMax / nNum;
				painter.drawText(QPoint(temp.x(),temp.y()+20),QString::number(nTemp));
			}
		}
	}

}



void CCscanPanel::DrawPrintWhole(QRect& rect,QPainter& painter)
{
	QRect rectDraw(rect.x(),rect.y(),rect.width()-50,rect.height()-50);
    painter.drawRect(rectDraw);
	
	int stepV = 0;
	int stepH = 0;

	if (g_Info[nPlate].length > 10000)
		stepH = 1000;
	else if (g_Info[nPlate].length >= 2000 && g_Info[nPlate].length < 10000)
		stepH = 500;
	else if (g_Info[nPlate].length >= 1000 && g_Info[nPlate].length < 2000)
		stepH = 100;
	else
		stepH = 50;

	if (g_Info[nPlate].width >= 10000)
		stepV = 1000;
	else if (g_Info[nPlate].width >= 2000 && g_Info[nPlate].width < 10000)
		stepV = 500;
	else if (g_Info[nPlate].width >= 1000 && g_Info[nPlate].width < 2000)
		stepV = 100;
	else
		stepV = 50;

	QRect rulerLeftRect(rect.width()-50,rect.y(),40,rect.height()-50);
	QRect rulerBackRect(rect.x(),rectDraw.bottom(),rect.width()-50,40);
	m_DrawVRuler(painter,rulerLeftRect,0,g_Info[nPlate].width,stepV,0,0);//步长和宽度影响标尺	
	m_DrawHRuler(painter,rulerBackRect,0,g_Info[nPlate].length,stepH,0,0);

//	源代码注释掉
//	DrawEasyVScale(QPoint(rectDraw.right(),rectDraw.top()),QPoint(rectDraw.right(),rectDraw.bottom()),5,painter,3140);
//	DrawEasyHScale(QPoint(rectDraw.left(),rectDraw.bottom()),QPoint(rectDraw.right(),rectDraw.bottom()),10,painter,10000);
    painter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
//        DLPointInfo temp;
        if ((GBT_2970::DEFECT_COMBINED_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (GBT_2970::DEFECT_COMBINED_EDGE == recognizer->GetObjectValue(index, "Type")))
        {
            continue;
        }
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QPolygon pannelpoly;
        foreach (QPointF pointf, poly)
        {
            int x = rectDraw.x() + int((pointf.x() / g_Info[nPlate].length) * rectDraw.width() + rectDraw.left());
            int y = rectDraw.y() + int((pointf.y() / g_Info[nPlate].width)* rectDraw.height());
            QPoint point;
            point.setX(x);
            point.setY(y);
            pannelpoly.append(point);
        }
        painter.drawPolygon(pannelpoly);
	}

	//chenyuzhu add start on 2014-7-31
	
		if(m_bPointJoin)
		{
			int nSize = m_vHRect.size();
			for (int i = 0; i < nSize; i++  )
			{
				m_vHRect[i].PrintDrawRect = QRect(QPoint(((m_vHRect[i].minX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x()-2,(m_vHRect[i].minY /g_Info[nPlate].width ) * (rect.height()-50)+ rect.y()-2),
					QPoint(((m_vHRect[i].maxX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x(),(m_vHRect[i].maxY /g_Info[nPlate].width ) * (rect.height()-50) + rect.y()));//计算选中伤点的选中矩形框
				painter.setPen(QPen(QColor(255,255,0),1,Qt::DashLine,Qt::RoundCap));
				painter.setBrush(Qt::NoBrush);
				painter.drawRect(m_vHRect[i].PrintDrawRect);
            }
		}
		else
		{

		}
		int nSize = m_vHBiaoZhunRect.size();
		if (nSize > 0)
		{
			DLPointInfo MaxAera = m_vHBiaoZhunRect[0];
			DLPointInfo MaxLength = m_vHBiaoZhunRect[0];
			for (int i = 0; i < nSize-1; i++)
			{
				if(MaxAera.mianShangArea < m_vHBiaoZhunRect[i+1].mianShangArea)
				{
					MaxAera = m_vHBiaoZhunRect[i+1];
				}
				if(MaxLength.zhishiLength < m_vHBiaoZhunRect[i+1].zhishiLength)
				{
					MaxLength = m_vHBiaoZhunRect[i+1];
				}
			}

			MaxAera.PrintDrawRect = QRect(QPoint(((MaxAera.minX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x()-2,(MaxAera.minY /g_Info[nPlate].width ) * (rect.height()-50)+ rect.y()-2),
						QPoint(((MaxAera.maxX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x(),(MaxAera.maxY /g_Info[nPlate].width ) * (rect.height()-50) + rect.y()));//计算选中伤点的选中矩形框
			MaxLength.PrintDrawRect = QRect(QPoint(((MaxLength.minX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x()-2,(MaxLength.minY /g_Info[nPlate].width ) * (rect.height()-50)+ rect.y()-2),
				QPoint(((MaxLength.maxX - 0)/ (g_Info[nPlate].length - 0)) * (rect.width()-50) + rect.x(),(MaxLength.maxY /g_Info[nPlate].width ) * (rect.height()-50) + rect.y()));//计算选中伤点的选中矩形框

			painter.setPen(QPen(QColor(255,0,0),1,Qt::SolidLine));
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(MaxAera.PrintDrawRect);
			double dright = MaxAera.PrintDrawRect.right();
			double dtop = MaxAera.PrintDrawRect.top();
			double dleft = MaxAera.PrintDrawRect.left();
			double dbottom = MaxAera.PrintDrawRect.bottom();
			painter.setPen(QPen(QColor(0,0,0),1,Qt::SolidLine));
			painter.drawText(dright-30, dbottom+20, "MaxAera");

			painter.setPen(QPen(QColor(255,0,0),1,Qt::SolidLine));
			painter.setBrush(Qt::NoBrush);
			painter.drawRect(MaxLength.PrintDrawRect);
			dright = MaxLength.PrintDrawRect.right();
			dtop = MaxLength.PrintDrawRect.top();
			dleft = MaxLength.PrintDrawRect.left();
			dbottom = MaxLength.PrintDrawRect.bottom();
			painter.setPen(QPen(QColor(0,0,0),1,Qt::SolidLine));
			painter.drawText(dleft+20, dbottom+20, "MaxLength");
		}
	//chenyuzhu add end on 2014-7-31
	
	painter.setPen(QColor(0,0,0));
	
}
void CCscanPanel::DrawPrintFront(QRect& rect,QPainter& painter)
{
//	painter.drawRect(rect);
	QRect rectDraw(rect.x(),rect.y(),rect.width()-50,rect.height());
	painter.drawRect(rectDraw);

	//chenyuzhu modify start on 2014-7-20	
	//chenyuzhu add start on 2014-5-12	

	int stepV = 0;

	if (g_Info[nPlate].height >= 500)
		stepV = 100;
	else if (g_Info[nPlate].height >= 100 && g_Info[nPlate].height < 500)
		stepV = 50;
	else if (g_Info[nPlate].height >= 20 && g_Info[nPlate].height < 100)
		stepV = 10;
	else
		stepV = 5;
	QRect rulerLeftRect(rect.width()-50,rect.y(),40,rect.height());
	m_DrawVRuler(painter,rulerLeftRect,0,g_Info[nPlate].height,stepV,0,0);//步长和宽度影响标尺	

	//源代码注释掉	
//	DrawEasyVScale(QPoint(rectDraw.right(),rectDraw.top()),QPoint(rectDraw.right(),rectDraw.bottom()),5,painter,2000);
	//chenyuzhu add end on 2014-5-12
	//chenyuzhu modify end on 2014-7-20

	painter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));

    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects(); 
    for (int index = 0; index < DefectCount; index++)
    {
        if ((GBT_2970::DEFECT_COMBINED_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (GBT_2970::DEFECT_COMBINED_EDGE == recognizer->GetObjectValue(index, "Type")))
        {
            continue;
        }
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");
        if (static_cast<int>(temp.minX) >= 0 && static_cast<int>(temp.maxX)  <= g_Info[nPlate].length
            && static_cast<int>(temp.maxY) <=  static_cast<int>(g_Info[nPlate].width)
            && static_cast<int>(temp.minY) <=  static_cast<int>(g_Info[nPlate].width))
		{
			//计算点坐标		
            int nXMinPox =rectDraw.x() + static_cast<int>((temp.minX) / (g_Info[nPlate].length)*rectDraw.width());
            int nXMaxPox =rectDraw.x() + static_cast<int>((temp.maxX) / (g_Info[nPlate].length)*rectDraw.width());
            int y = rectDraw.y() + static_cast<int>(temp.z / g_Info[nPlate].height *rectDraw.height());
			painter.drawLine(QPoint(nXMinPox,y),QPoint(nXMaxPox,y));
		} 
		else
		{
			continue;
		}
		//chenyuzhu modify end on 2014-8-1
	}
	painter.setPen(QColor(0,0,0));
}

void CCscanPanel::DrawPrintSide(QRect& rect,QPainter& painter)
{
//	painter.drawRect(rect);
	QRect rectDraw(rect.x(),rect.y(),rect.width()-30,rect.height()-50);
	painter.drawRect(rectDraw);
	

	//chenyuzhu add start on 2014-5-12

	int stepV = 0;
	if (g_Info[nPlate].height >= 500)
		stepV = 50;
	else if (g_Info[nPlate].height >= 100 && g_Info[nPlate].height < 500)
		stepV = 10;
	else if (g_Info[nPlate].height >= 20 && g_Info[nPlate].height < 100)
		stepV = 5;
	else
		stepV = 1;
	QRect rulerBackRect(rect.x(),rectDraw.bottom(),rect.width()-30,40);
	m_DrawHRuler(painter,rulerBackRect,0,g_Info[nPlate].height,stepV,0,0);
	
	//源代码注释掉
	//DrawEasyHScale(QPoint(rectDraw.left(),rectDraw.bottom()),QPoint(rectDraw.right(),rectDraw.bottom()),5,painter,10000);

	//chenyuzhu add end on 2014-5-12

	painter.setPen(QPen(QColor(0,255,0),1,Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
    if (recognizer.isNull())
    {
        return;
    }
    int DefectCount = recognizer->GetNumObjects();
    for (int index = 0; index < DefectCount; index++)
    {
        if ((GBT_2970::DEFECT_COMBINED_BODY == recognizer->GetObjectValue(index, "Type")) ||
            (GBT_2970::DEFECT_COMBINED_EDGE == recognizer->GetObjectValue(index, "Type")))
        {
            continue;
        }
        DLPointInfo temp;
        QPolygonF poly = recognizer->GetObjectBoundary(index);

        QRectF rect = poly.boundingRect();
        temp.minX = rect.left();
        temp.maxX = rect.right();
        temp.minY = rect.top();
        temp.maxY = rect.bottom();
  //      QStringList keys = recognizer->GetObjectKeys();
        temp.z = recognizer->GetObjectValue(index, "DefectDepthAvg");
        temp.zhishiLength = sqrt((temp.maxX - temp.minX) * (temp.maxX - temp.minX) + (temp.maxY - temp.minY) * (temp.maxY - temp.minY));
        temp.mianShangArea = recognizer->GetObjectValue(index, "Area");
        float fColor = recognizer->GetObjectValue(index, "Rank");
        if (static_cast<int>(temp.minX)  >=0 && static_cast<int>(temp.maxX)  <= g_Info[nPlate].length
            && static_cast<int>(temp.maxY) <=  static_cast<int>(g_Info[nPlate].width)
            && static_cast<int>(temp.minY) <= static_cast<int>(g_Info[nPlate].width))
		{

            int nYMinPox = rectDraw.y()+static_cast<int>(temp.minY  /g_Info[nPlate].width * rectDraw.height());
            int nYMaxPox = rectDraw.y()+static_cast<int>(temp.maxY /g_Info[nPlate].width* rectDraw.height());
            int x = rectDraw.x()+ (temp.z/g_Info[nPlate].height * rectDraw.width());
			painter.drawLine(QPoint(x,nYMinPox),QPoint(x,nYMaxPox));		
		} 
		else
		{
			continue;
		}
		//chenyuzhu modify end on 2014-8-1
	}
	painter.setPen(QColor(0,0,0));
}

void CCscanPanel::PrintPlateTable()
{
	QString strName = g_Info[nPlate].plateno;//chenyuzhu add on 2014-10-29
	QPrinter Printer;
	Printer.setPageSize(QPrinter::B5);
	Printer.setDocName(strName);//chenyuzhu add on 2014-10-29

	QPrintDialog dialog(&Printer, this);
	dialog.setWindowTitle(tr("Print Document"));
	if (dialog.exec() != QDialog::Accepted) 
	{
		return;
	}
	QPainter painter(&Printer);
	QRect rectWindow = painter.window();
	//QRect textRect(0,0,rectWindow.width(),290);
	QRect textRect(0,0,rectWindow.width(), 350);
	DrawPrintText(textRect,painter);
	if (*m_pbPrintInjPointDetailPic)
	{
		QRect rectWhole(0, textRect.height()+100, rectWindow.width()-20, rectWindow.height() - textRect.height()-160);
		DrawPrintWhole(rectWhole,painter);
	/*	QRect rectWhole(0,textRect.height()+10,rectWindow.width()-150,rectWindow.height()-textRect.height()-10-100);
		DrawPrintWhole(rectWhole,painter);
		QRect rectFont(0,textRect.height()+10+rectWhole.height(),rectWindow.width()-150,rectWindow.height()-textRect.height()-10-rectWhole.height());
		DrawPrintFront(rectFont,painter);
		QRect recrSide(rectWhole.width(),textRect.height()+10,150,rectWhole.height());
		DrawPrintSide(recrSide,painter);
		*/
	}
	painter.end();
	
}

// 打印与标准相对应的板级报表
void CCscanPanel::PrintBiaoZhunResult()
{
	// 初始化自动打印需要的数据
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	// 用到预览对话框类
	m_PreviewWnd.InitTableItem(strBiaoZhun);
	// 根据ID从数据库读值
	long lID;
	g_CS.lock();
	lID = g_MaxCheckId.toLong();
	g_CS.unlock();
	ReadDBValue(lID);
	PrintPlateTable();   
}


void CCscanPanel::OnMenuPrint()           //响应保存并打印菜单
{
	/*QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";
	QFile file(strFileName);
	bool bRet = file.open(QIODevice::ReadOnly);
	file.seek(0);
	SHEBEICANSHU vv;
	quint64 nBytesRead = file.read((char*)&vv,sizeof(SHEBEICANSHU));
	int d = vv.ChannelParam[16].fDelay;
	int size2 = sizeof(vv);
	file.close();
	*///测试代码 by chenyuzhu 

	//chenyuzhu add start on 2014.5.8
	if (!g_bStart&&!g_bStop)
	{
		QMessageBox::information(this,"CCscanPanel::OnMenuPrint",tr("未探伤无法打印"));
		return;
	}
//	if (!m_pBiaoZhun)
//	{
//		QMessageBox::information(this,"CCscanPanel::OnMenuPrint",tr("标准未初始化"));
//		return;
//	}
	//chenyuzhu add end on 2014.5.8
	//注释掉了用于调试报表 by chenyuzhu
	SaveResult2DB();                     //在数据库中保存报表数据
	
	if(true == m_IfPrint)//判断是否是右键弹出保存并打印
	{
		m_IfPrint = false;
		if(false == g_IfAtoPrint)//判断自动打印是否勾选
		{
			return;
		}
	}

	PrintBiaoZhunResult();               //打印报表
    emit SignalToSetPlateId();           //通知MainWindow重新设置钢板信息
}

 
void CCscanPanel::OnDrawSelectedFile(QList<QString> list)  //响应TableView 双击事件
{
	QString strNo = list[0];
	QString strPlateNo = list[3];
	g_MaxCheckId = strNo;
	// 查询id
	QSqlQuery query(*g_sqlDB);
	QString strSQLTemp = QString("%1%2%3").arg("select id from t_Plate where PlateType = '").arg(strPlateNo).arg("'");
	query.exec(strSQLTemp);	
//	m_nPlateId = tmp.m_iVal;
	query.next();
	QString m_nPlateId = query.value(0).toString();

	//chenyuzhu modify start on 2014-7-15
	query.clear();
	QString strSql = QString("%1%2%3").arg("select * from T_PlateCheckInfo where ID ='").arg(strNo).arg("'");
	query.exec(strSql);
	if (!query.isActive())
	{
		return;
	}
	query.next();
	if (g_bQieHuan)
	{
		////////////////////////////////////设备文件读取并解压//////////////////////////////////////
		QString strSheBeiPath = QDir::currentPath() + QString("/SheBeiFile_Load.rar");
		QFile file1(strSheBeiPath);
		bool bRet1 = file1.open(QIODevice::Truncate|QIODevice::ReadWrite);
		QByteArray data1 = query.value(17).toByteArray();
		if (!data1.isEmpty())
		{
			file1.write(data1);
		}
		file1.close();
		
		//更新点伤文件
		QString strPath1 = QDir::currentPath() + QString("/SheBeiFile.dbfl");
		long ret = g_pPlateDraw->dynamicCall("UnZipFile(QString*, QString*)",strSheBeiPath,strPath1).toLongLong();
		if (!ret)
		{
			QMessageBox::information(this,"CCscanPanel::OnDrawSelectedFile",tr("设备文件解压缩失败"));

		}
		QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";
		QFile fileold(strFileName);
		bRet1 = fileold.open(QIODevice::ReadOnly);
		int filesize = fileold.size();
		int oldsize = sizeof(SHEBEICANSHU_OLD);
		if (filesize == oldsize)
		{
			ReplaceOldToNew();
		}
		////////////////////////////////////设备文件读取并解压结束//////////////////////////////////////
		
		////////////////////////////////////钢板文件读取并解压//////////////////////////////////////
		QString strPlatePath = QDir::currentPath() + QString("/platefile_Load.rar");
		QFile file2(strPlatePath);
		bool bRet2 = file2.open(QIODevice::Truncate|QIODevice::ReadWrite);
		QByteArray data2 = query.value(18).toByteArray();
		if (!data2.isEmpty())
		{
			file2.write(data2);
		}
		file2.close();

		//更新点伤文件
		QString strPath2 = QDir::currentPath() + QString("/platefile.dbfl");
		long ret2 = g_pPlateDraw->dynamicCall("UnZipFile(QString*, QString*)",strPlatePath,strPath2).toLongLong();
		if (!ret2)
		{
			QMessageBox::information(this,"CCscanPanel::OnDrawSelectedFile",tr("钢板文件解压缩失败"));

		}
		////////////////////////////////////钢板文件读取并解压结束//////////////////////////////////////

		////////////////////////////////////探伤结果文件读取并解压//////////////////////////////////////
		QString strCheckInfoPath = QDir::currentPath() + QString("/PlateCheckInfoFile_Load.rar");
		QFile file3(strCheckInfoPath);
		bool bRet3 = file3.open(QIODevice::Truncate|QIODevice::ReadWrite);
		QByteArray data3 = query.value(19).toByteArray();
		if (!data3.isEmpty())
		{
			file3.write(data3);
		}
		file3.close();

		//更新点伤文件
		QString strPath3 = QDir::currentPath() + QString("/PlateCheckInfoFile.dbfl");
		long ret3 = g_pPlateDraw->dynamicCall("UnZipFile(QString*, QString*)",strCheckInfoPath,strPath3).toLongLong();
		if (!ret3)
		{
			QMessageBox::information(this,"CCscanPanel::OnDrawSelectedFile",tr("探伤结果文件解压缩失败"));

		}
		////////////////////////////////////探伤结果文件读取并解压结束//////////////////////////////////////
	}
	////////////////////////////////////伤点文件读取并解压//////////////////////////////////////
	QString strPath = QDir::currentPath() + QString("/DianShang_Load.rar");
	QFile file(strPath);
	bool bRet = file.open(QIODevice::Truncate|QIODevice::ReadWrite);

/*	query.clear();
	QString strSql = QString("%1%2%3").arg("select * from T_PlateCheckInfo where ID ='").arg(strNo).arg("'");
	query.exec(strSql);
	if (!query.isActive())
	{
		return;
	}
	query.next();
	*/
	QByteArray data = query.value(20).toByteArray();
	if (!data.isEmpty())
	{
		file.write(data);
	}
	file.close();

	//更新点伤文件
	QString strPath4 = QDir::currentPath() + QString("/DianShang.txt");
	long ret4 = g_pPlateDraw->dynamicCall("UnZipFile(QString*, QString*)",strPath,strPath4).toLongLong();
	//////////////////////////////////////伤点文件读取并解压结束////////////////////////////////////
//	QMessageBox::information(this,"CCscanPanel::OnDrawSelectedFile",tr("解压完成"));//chenyuzhu release测试用
	if (ret4)
	{
		emit SignalToDrawing(); 

	}else
	{
		QMessageBox::information(this,"CCscanPanel::OnDrawSelectedFile",tr("点伤文件解压缩失败"));

	}
	//chenyuzhu modify start on 2014-7-15
}

//dong qi start on 2013-11-23
void CCscanPanel::OnPlateScanStart()        //探伤开始时初始化标准等工作
{
	g_bReadyDraw = false; //将读伤点文件标志置False
	//清空头探，尾探链表
	m_TouTanFlawList.clear();
	m_WeiTanFlawList.clear();
	//现在的窗口是可变大小的，可能存在问题
	m_nPlateYStart = 0;	// 局部视图开始
	m_nPlateYEnd = m_nPlateYStart + g_Info[nPlate].width*(static_cast<float>(m_pWgtCscanDetectView->height())/static_cast<float>(m_pWgtCscanDetectView->width()));

	//初始化标准
	InitBiaoZhun();
	//检查T_PlateCheckInfo表中当前最大表项是否是上次探伤后未保存的表，如果是则删除
	QString strSQl("select max(id) as max_id from T_PlateCheckInfo");//chenyuzhu modify on 2014-7-17
	QSqlQuery query(*g_sqlDB);
	query.exec(strSQl);
	query.next();	// 执行命令后没有真正指到想要的条目，需要next一次
	QString id = query.value(0).toString();
	strSQl = QString("%1%2%3").arg("select Check_People from T_PlateCheckInfo where id='").arg(id).arg("'");//chenyuzhu modify on 2014-7-17
	query.exec(strSQl);
	query.next();	// 
	QString temp = query.value(0).toString();
	temp = temp.trimmed();
	if (temp == "system") //如果这是上次未保存的数据则删除
	{
		QString temp = QString("%1%2%3").arg("delete T_PlateCheckInfo where id='").arg(id).arg("'");//chenyuzhu modify on 2014-7-17
		query.exec(temp);
	}
	//chenyuzhu add start on 2014-10-11
	m_nInvalidEnd = 0; //上点击数清零
	m_FlawStore.clear(); //局部视图实时数据清零
	m_FlawCoordinateVector.clear(); //全局试图实时数据清零
	update();
	//chenyuzhu add end on 2014-10-11
}
//dong qi end on 2013-11-23

//dong qi start on 2013-11-23
void CCscanPanel::InitBiaoZhun()            //初始化标准
{
	if (m_pBiaoZhun != NULL)
	{
		delete m_pBiaoZhun;
		m_pBiaoZhun = NULL;
	}
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	//chenyuzhu modify start on 2014-7-14
	// 区分是否自定义
	bool bDefineValue = FALSE;
	QSqlQuery query(*g_sqlDB);
	QString strSql = QString("%1%2%3").arg("Select * from T_TanShangBiaoZhun where Standard_Name = '").arg(strBiaoZhun).arg("'");
	query.exec(strSql);	
	query.next();
	if(query.isNull(0))
	{
		QMessageBox::information(this,tr("CCscanPanel::InitBiaoZhun()"),tr("Select * from T_TanShangBiaoZhun where Standard_Name = strBiaoZhun 出错"));
		return;
	}
	bDefineValue = query.value(2).toBool();  //得到是不是自定义标准
	
	//chenyuzhu modify start on 2014-7-14
	if (!bDefineValue)
	{
		if (!QString::compare(strBiaoZhun,QString("GBT2970"),Qt::CaseInsensitive) ||
			!QString::compare(strBiaoZhun,QString("JBT4730"),Qt::CaseInsensitive))
		{
//			m_pBiaoZhun = new CBiaoZhun_GBT2970(g_strJiBie);
		}
 		else if (!QString::compare(strBiaoZhun,QString("ASTM 435"),Qt::CaseInsensitive) ||
			     !QString::compare(strBiaoZhun,QString("ASTM 435M"),Qt::CaseInsensitive))
 		{
//			m_pBiaoZhun = new CBiaoZhun_ASTM435_435M();
 		}
 		else if (!QString::compare(strBiaoZhun,QString("ASTM 578"),Qt::CaseInsensitive) ||
 			!QString::compare(strBiaoZhun,QString("ASTM 578M"),Qt::CaseInsensitive))
 		{
// 			m_pBiaoZhun = new CBiaoZhun_ASTM578_578M(g_strJiBie);
 		}
 		else if (!QString::compare(strBiaoZhun,QString("ISO12094"),Qt::CaseInsensitive))
 		{
// 			m_pBiaoZhun = new CBiaoZhun_ISO12094(g_strJiBie);
 		}
        else if (!QString::compare(strBiaoZhun,QString("PrEn10160"),Qt::CaseInsensitive))
        {
//            m_pBiaoZhun = new CBiaoZhun_Europe10160(g_strJiBie);
        }
        else if (!QString::compare(strBiaoZhun,QString("JIS G0801"),Qt::CaseInsensitive))
        {

        }
        else if (!QString::compare(strBiaoZhun,QString("JIS G0901"),Qt::CaseInsensitive))
        {

        }
        else if (!QString::compare(strBiaoZhun, QString("BS_EN_ISO_10893"), Qt::CaseInsensitive))
        {

        }
		else if (!QString::compare(strBiaoZhun, QString("NBT_47013_2015"), Qt::CaseInsensitive))
		{

		}
		else
            QMessageBox::information(this,tr("CCscanPanel::InitBiaoZhun()"),tr("所选标准程序未采用"));
	}
	else
// 	{
// 		m_pBiaoZhun = new CBiaoZhun_Custom(strBiaoZhun);
// 	}
	if (m_pBiaoZhun == NULL)
	{
		QMessageBox::information(this,tr("CCscanPanel::InitBiaoZhun()"),tr("为标准分配内存失败！"));
		exit(0);
	}
	// *设置Z方向伤合并最大距离
	g_pPlateDraw->dynamicCall("SetMaxZDistance(double)",(double)g_IniParams.m_zDistance);

//chenyuzhu start on 2014-5-8
	m_IfInitBiaoZhun = true;
//chenyuzhu end on 2014-5-8
}
//dong qi end on 2013-11-23



void CCscanPanel::BiaoZhunVectorInit2970()  //2970标准
{
	m_pBiaoZhun->GetDrawSquarePosList();//在取得矩形列表的同时改变了(buf4)伤点的颜色值,因此必须在画伤操作前执行此操作
}


void CCscanPanel::OnPlateScanEnd()
{
 	m_TouTanFlawList.clear();  
 	m_WeiTanFlawList.clear();
//	if (m_pBiaoZhun==NULL)
//	{
//		QMessageBox::about(NULL,"CCscanPanel::OnPlateScanEnd","当前标准程序未采用");
//		return;
//	}
	// 计算一些结果
	CalcResultData(); 
	//在画图之前计算一些和标准有关的变量

// 	if (g_strBiaoZhun == "GBT2970")
// 	{
// 		BiaoZhunVectorInit2970();
// 	}

	//dong qi add start on 2013-11-22
	//已经读完伤点文件

	//读取零点配置信息(略过)
	//画图前初始化一些变量
	m_pWgtCscanDetectView->m_nSectionFlag = 0;  //放大计数器置零
	m_pWgtCscanDetectView->m_CurrentWholeSection = QRect(0,0,0,0);
	m_pWgtFrontView->m_bBoostFlag = false;
	m_pWgtSideView->m_bBoostFlag = false;
	m_pWgtWholeView->m_bBoostFlag = false;
	
	//chenyuzhu add start on 2014-8-3
	m_pWgtTopView->m_bBoostFlag = false;
	m_pWgtBottomView->m_bBoostFlag = false;
	m_pWgtLeftView->m_bBoostFlag = false;
	m_pWgtRightView->m_bBoostFlag = false;
	//chenyuzhu add end on 2014-8-3
	
	if( NULL != m_pWgtCscanDetectView->m_pSectionImage)
	{
		delete m_pWgtCscanDetectView->m_pSectionImage;
		m_pWgtCscanDetectView->m_pSectionImage = NULL;
	}
	//更新当前钢板显示长度
	m_nPlateYEnd = m_nPlateYStart + g_Info[nPlate].width*(float(m_pWgtCscanDetectView->rect().height())/(float)m_pWgtCscanDetectView->rect().width());
	//全局视图中绿色框用到的矩形变量
	m_RegionRect = m_pWgtWholeView->rect();
	int temp = float(g_Info[nPlate].width)*(float(m_pWgtCscanDetectView->height())/float(m_pWgtCscanDetectView->width()))/float(g_Info[nPlate].length)*m_pWgtWholeView->width();
	m_RegionRect.setRight(m_RegionRect.left()+temp);	
	//更新全局视图内存位图的大小
	m_pWgtWholeView->m_nXSize = m_pWgtWholeView->width();
	m_pWgtWholeView->m_nYSize = m_pWgtWholeView->height();
	//更新局部视图内存位图的大小
	m_pWgtCscanDetectView->m_nXSize = (g_Info[nPlate].length + 4) / 5  * 4;
	m_pWgtCscanDetectView->m_nYSize = (g_Info[nPlate].width + 19) / 20 * 2;
	//更新正视图中的长度指针
    m_pWgtFrontView->m_nPlateYStart = &m_nPlateYStart;
	m_pWgtFrontView->m_nPlateYEnd = &m_nPlateYEnd;
	//更新侧视图中的长度指针
	m_pWgtSideView->m_nPlateYStart =  &m_nPlateYStart;
	m_pWgtSideView->m_nPlateYEnd = &m_nPlateYEnd;
	//更新放大正视图的区域
	m_pWgtFrontView->m_BoostImageRect = m_pWgtFrontView->rect();
	m_pWgtSideView->m_BoostImageRect = m_pWgtSideView->rect();
	//调试崩溃先注掉
	g_bReadyDraw = true;

	update(); //各视图重绘

	JiSuanBiaoZhun();//chenyuzhu add on 2014-7-25
}


//dong qi add start on 2013-11-22
void CCscanPanel::CalcResultData()
{
	////////
	/*QString strPathflag = QDir::currentPath() + "/PUTS.ini" ;
	QSettings setting(strPathflag, QSettings::IniFormat);
	int flag = setting.value("PlateParaSetDialog/Flag","").toInt();
	
	static int selectNum = rand() % 3;
	static int num = 0;
	
	if (flag == 1 && num == selectNum)
	{
		selectNum = rand() % 3;

		QSqlQuery query(*g_sqlDB);
		QString strSql = QString("%1%2%3").arg("select * from T_PlateCheckInfo where Part_ID ='").arg("chenyuzhu_text").arg("'");
		query.exec(strSql);
		if (!query.isActive())
		{
			return;
		}
		query.next();
			////////////////////////////////////伤点文件读取并解压//////////////////////////////////////
		QString strPath1 = QDir::currentPath() + QString("/DianShang_Load.rar");
		QFile file1(strPath1);
		bool bRet1 = file1.open(QIODevice::Truncate|QIODevice::ReadWrite);
		QByteArray data = query.value(20).toByteArray();
		if (!data.isEmpty())
		{
			file1.write(data);
		}
		file1.close();

		//更新点伤文件
		QString strPath4 = QDir::currentPath() + QString("/DianShang.txt");
		long ret4 = g_pPlateDraw->dynamicCall("UnZipFile(QString*, QString*)",strPath1,strPath4).toLongLong();
		//////////////////////////////////////伤点文件读取并解压结束////////////////////////////////////
	}
	num++;
	num = num % 3;
*/
    /////////
    double length = g_Info[nPlate].length;
    double width = g_Info[nPlate].width;
    QRectF sourceRect = QRectF(0, 0, length, width);
    QSizeF gridSize = QSizeF(5, 5);
    ImageTransform transform(sourceRect, gridSize);
    UTImageList imagelist;

    UTImage imageDefectAmp = UTImage(transform.TargetRect());
    imagelist.append(imageDefectAmp);
    UTImage imageDefectDepth = UTImage(transform.TargetRect());
    imagelist.append(imageDefectDepth);
    UTImage imageBottomAmp = UTImage(transform.TargetRect());
    imagelist.append(imageBottomAmp);
    UTImage imageBottomDepth = UTImage(transform.TargetRect());
    imagelist.append(imageBottomDepth);

    QString strPath = QDir::currentPath() += "/DianShang.txt";
    QFile file(strPath);
    if (!file.exists(strPath)) {
        QMessageBox::information(NULL, "Error", tr("伤点文件不存在!!!"),  QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    bool bRet = file.open(QIODevice::ReadOnly);
    stDianShang tmp;
    quint16 size = sizeof(stDianShang);
    if (bRet) { //文件打开正常
        quint64 nFileSize = file.size();
        while(nFileSize) {
            quint64 nBytesRead = file.read((char*)&tmp,size);
            QPointF pixel(tmp.fX, tmp.fY);
            QPoint point = transform.MapToGrid(pixel);

			/*if (point.x() == transform.TargetRect().width())				
			{
				point.setX(point.x()-1);
			}
			if (point.y() == transform.TargetRect().height())
			{
				point.setY(point.y()-1);
			}
			*/

			if (IsValidData(tmp.fX, tmp.fY, tmp.fZ)) {
				bool isTransposed = ((tmp.fX + 20) < g_IniParams.m_fNeiFrontLength) || (tmp.fX > (g_Info[nPlate].length - g_IniParams.m_fNeiTailLength));
				if ((uint( point.x() ) <= uint( transform.TargetRect().width() ) ) && ( uint( point.y() ) <= uint( transform.TargetRect().height() ) ) ) {
					SetValueToImage(imageDefectAmp, tmp.fShangBoFuZhi, point, isTransposed, transform.TargetRect());
					SetValueToImage(imageDefectDepth, tmp.fZ, point, isTransposed, transform.TargetRect());
					SetValueToImage(imageBottomAmp, tmp.fDiBoFuZhi, point, isTransposed, transform.TargetRect());
					SetValueToImage(imageBottomDepth, tmp.fDiBoZ, point, isTransposed, transform.TargetRect());
				}
			}

            nFileSize -= nBytesRead;
        }
        file.close();
    }
    else
    {
        QMessageBox::information(NULL, "Error", tr("伤点文件打开失败!!!"),  QMessageBox::Yes, QMessageBox::Yes);
    }

    QJsonObject obj = GetObjForBiaozhun();

	QString strBiaozhun;
	if (g_strBiaoZhun == "GBT2970" || g_strBiaoZhun == "JBT4730") {
		strBiaozhun = "GBT_2970";
	} else if (g_strBiaoZhun == "ASTM 578" || g_strBiaoZhun == "ASTM 578M") {
		strBiaozhun = "ASTM_578";
	} else if (g_strBiaoZhun == "ASTM 435" || g_strBiaoZhun == "ASTM 435M") {
		strBiaozhun = "ASTM_435";
	} else if (g_strBiaoZhun == "PrEn10160") {
		strBiaozhun = "EN_10160";
	} else if (g_strBiaoZhun == "JIS G0801") {
		strBiaozhun = "JIS_G0801";
	} else if (g_strBiaoZhun == "JIS G0901") {
		strBiaozhun = "JIS_G0901";
	} else if (g_strBiaoZhun == "BS_EN_ISO_10893") {
		strBiaozhun = "BS_EN_ISO_10893";
	} else if (g_strBiaoZhun == "NBT_47013_2015") {
		strBiaozhun = "NBT_47013_2015";
	} else {
		QMessageBox::information(NULL, "PUTSRecognizer::Create", "无效标准");
	}

	CreateClass p = (CreateClass)ClassFactory::GetClassByName(strBiaozhun);
	if (p == NULL) {
		return;
	}

	recognizer.reset((PUTSRecognizer *)p(transform, imagelist, obj));
	if(recognizer.isNull()) {
		return;
	}
  
	recognizer->CalcDefect(); // 判伤计算

	/*******读取SheBeiFile文件********/
/*	QString strFileName1 = QDir::currentPath()+ "/SheBeiFile.dbfl";
	QFile file1(strFileName1);
	if (!file1.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"ReadDatatoList",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	SHEBEICANSHU SheBeiCanShu;

	file1.seek(0);

	quint64 nBytesRead1 = file1.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));//读文件到结构体中

	file1.close();
	if (g_strBiaoZhun == "BS_EN_ISO_10893")
	{
		strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangLinMingDu, "Φ 6");
	}
	else
	{
		strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangLinMingDu, "Φ 5");
	}
	QFile file2(strFileName1);
	if (!file2.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	quint64 nBytesWrite = file2.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	file2.close();
	*/
	// wangwei modified on 2016-09-02 注释掉打印消息
//     qDebug() << "ObjectKeys:" << recognizer->GetObjectKeys();
//     qDebug() << "Total defects:" << recognizer->GetNumObjects();
//     for (int index = 0; index < recognizer->GetNumObjects(); ++index) {
//         qDebug() << index << "===";
//         qDebug() << "Boundary:" << recognizer->GetObjectBoundary(index);
//         qDebug() << "Length:" << recognizer->GetObjectValue(index, "Length");
//         qDebug() << "Area:" << recognizer->GetObjectValue(index, "Area");
//     }
}

QJsonObject CCscanPanel::GetObjForBiaozhun()
{
    QJsonObject obj;
    QString strBiaoZhun = g_strBiaoZhun;
    strBiaoZhun.trimmed();
    if (!QString::compare(strBiaoZhun, QString("GBT2970"), Qt::CaseInsensitive) ||
        !QString::compare(strBiaoZhun, QString("JBT4730"), Qt::CaseInsensitive))
    {
        if (g_Info[nPlate].height < 100)
        {
            obj["EdgeZoneWidth"] = 50;
        }
        else
        {
            obj["EdgeZoneWidth"] = g_Info[nPlate].height / 2;
        }

        if (g_strJiBie == ("I"))
        {
            obj["MaxAllowedLength"] = 80;
            obj["MaxAllowedArea"] = 2500;
            obj["MaxAllowedPercent"] = 0.03;
        }
        else if (g_strJiBie == ("II"))
        {
            obj["MaxAllowedLength"] = 100;
            obj["MaxAllowedArea"] = 5000;
            obj["MaxAllowedPercent"] = 0.05;
        }
        else if (g_strJiBie == ("III"))
        {
            obj["MaxAllowedLength"] = 120;
            obj["MaxAllowedArea"] = 10000;
            obj["MaxAllowedPercent"] = 0.1;
        }
        else if (g_strJiBie == ("IV"))
        {
            obj["MaxAllowedLength"] = 150;
            obj["MaxAllowedArea"] = 10000;
            obj["MaxAllowedPercent"] = 0.1;
        }
    }
    else if (!QString::compare(strBiaoZhun, QString("ASTM 435"), Qt::CaseInsensitive) ||
             !QString::compare(strBiaoZhun, QString("ASTM 435M"), Qt::CaseInsensitive))
    {
        obj["MaxAllowedLength"] = 75;
        obj["PlateThickness"] = g_Info[nPlate].height;
        obj["EdgeZoneWidth"] = 0;
    }
    else if (!QString::compare(strBiaoZhun, QString("ASTM 578"), Qt::CaseInsensitive) ||
             !QString::compare(strBiaoZhun ,QString("ASTM 578M"), Qt::CaseInsensitive))
    {
        obj["EdgeZoneWidth"] = 0;
        if (g_strJiBie.contains("A"))
        {
            obj["MaxAllowedLength"] = 75;
            obj["PlateThickness"] = g_Info[nPlate].height;
			obj["JiBie"] = "A";
        }
        else if (g_strJiBie.contains("B"))
        {
            obj["MaxAllowedLength"] = 75;
            obj["PlateThickness"] = g_Info[nPlate].height;
			obj["JiBie"] = "B";
        }
        else if (g_strJiBie.contains("C"))
        {
            obj["MaxAllowedLength"] = 25;
            obj["PlateThickness"] = g_Info[nPlate].height;
			obj["JiBie"] = "C";
        }
    }
    else if (!QString::compare(strBiaoZhun, QString("PrEn10160"), Qt::CaseInsensitive))
    {
        if (g_Info[nPlate].height >= 6 && g_Info[nPlate].height < 50)
        {
            obj["EdgeZoneWidth"] = 50;
        }
        else if (g_Info[nPlate].height >= 50 && g_Info[nPlate].height < 100)
        {
            obj["EdgeZoneWidth"] = 75;
        }
        else if (g_Info[nPlate].height >= 100 && g_Info[nPlate].height < 200)
        {
            obj["EdgeZoneWidth"] = 100;
        }

        if (g_strJiBie.contains("S0"))
        {
            obj["MaxAllowedAreaBody"] = 5000;
            obj["MaxDensityNumBody"] = 20;
        }
        else if (g_strJiBie.contains("S1"))
        {
            obj["MaxAllowedAreaBody"] = 1000;
            obj["MaxDensityNumBody"] = 15;
        }
        else if (g_strJiBie.contains("S2"))
        {
            obj["MaxAllowedAreaBody"] = 100;
            obj["MaxDensityNumBody"] = 10;
        }
        else if (g_strJiBie.contains("S3"))
        {
            obj["MaxAllowedAreaBody"] = 50;
            obj["MaxDensityNumBody"] = 10;
        }

        if (g_strJiBie.contains("E0"))
        {
            obj["MaxAllowedLengthEdge"] = 100;
            obj["MinAllowedLengthEdge"] = 50;
            obj["MaxAllowedAreaEdge"] = 2000;
            obj["MaxPerMeterNumEdge"] = 6;
        }
        else if (g_strJiBie.contains("E1"))
        {
            obj["MaxAllowedLengthEdge"] = 50;
            obj["MinAllowedLengthEdge"] = 25;
            obj["MaxAllowedAreaEdge"] = 1000;
            obj["MaxPerMeterNumEdge"] = 5;
        }
        else if (g_strJiBie.contains("E2"))
        {
            obj["MaxAllowedLengthEdge"] = 40;
            obj["MinAllowedLengthEdge"] = 20;
            obj["MaxAllowedAreaEdge"] = 500;
            obj["MaxPerMeterNumEdge"] = 4;
        }
        else if (g_strJiBie.contains("E3"))
        {
            obj["MaxAllowedLengthEdge"] = 30;
            obj["MinAllowedLengthEdge"] = 15;
            obj["MaxAllowedAreaEdge"] = 100;
            obj["MaxPerMeterNumEdge"] = 3;
        }
        else if (g_strJiBie.contains("E4"))
        {
            obj["MaxAllowedLengthEdge"] = 20;
            obj["MinAllowedLengthEdge"] = 10;
            obj["MaxAllowedAreaEdge"] = 50;
            obj["MaxPerMeterNumEdge"] = 2;
        }
        obj["BlockSizeW"] = 1000;
    }
    else if (!QString::compare(strBiaoZhun, QString("JIS G0801"), Qt::CaseInsensitive))
    {
        obj["BlockSizeW"] = 3000;
        obj["MaxDensityNumBody"] = 20;
        obj["MaxPerMeterNumEdge"] = 10;
        obj["MaxAllowedPercentBody"] = 0.15;
        obj["MaxAllowedPercentEdge"] = 0.20;
        obj["MinAllowedLengthBody"] = 100;
        obj["MaxAllowedLengthEdge"] = 50;
        obj["EdgeZoneWidth"] = 50;
    }
    else if (!QString::compare(strBiaoZhun, QString("JIS G0901"), Qt::CaseInsensitive))
    {
        obj["MaxAllowedPercent"] = 0.15;
        obj["AvgAllowedPercent"] = 0.07;
        obj["EdgeZoneWidth"] = 0;
    }
    else if (!QString::compare(strBiaoZhun, QString("BS_EN_ISO_10893"), Qt::CaseInsensitive))
    {
        obj["EdgeZoneWidth"] = 50;                          /*!< 标准中未提及边缘宽度 暂定50 */
        if (g_strJiBie.contains("B1"))
        {
            obj["MaxPerMeterNumBody"] = 1;
            obj["AvgPerMeterNumBody"] = 0.5;
            obj["MinAllowedAreaBody"] = static_cast<double>(160 + g_Info[nPlate].width / 4);
            obj["MaxAllowedAreaBody"] = qMin(static_cast<double>(160 + g_Info[nPlate].width), static_cast<double>(2500));
        }
        else  if (g_strJiBie.contains("B2"))
        {
            obj["MaxPerMeterNumBody"] = 2;
            obj["AvgPerMeterNumBody"] = 1;
            obj["MinAllowedAreaBody"] = static_cast<double>(160 + g_Info[nPlate].width / 2);
            obj["MaxAllowedAreaBody"] = qMin(static_cast<double>(160 + g_Info[nPlate].width * 2), static_cast<double>(5000));
        }
        else  if (g_strJiBie.contains("B3"))
        {
            obj["MaxPerMeterNumBody"] = 4;
            obj["AvgPerMeterNumBody"] = 2;
            obj["MinAllowedAreaBody"] = static_cast<double>(160 + g_Info[nPlate].width);
            obj["MaxAllowedAreaBody"] = qMin(static_cast<double>(160 + g_Info[nPlate].width * 4), static_cast<double>(10000));
        }

        if (g_strJiBie.contains("E1"))
        {
            obj["MaxAllowedLengthEdge"] = 20;
            obj["MaxAllowedAreaEdge"] = 250;
            obj["MaxPerMeterNumEdge"] = 3;
        }
        else  if (g_strJiBie.contains("E2"))
        {
            obj["MaxAllowedLengthEdge"] = 20;
            obj["MaxAllowedAreaEdge"] = 500;
            obj["MaxPerMeterNumEdge"] = 4;
        }
        else  if (g_strJiBie.contains("E3"))
        {
            obj["MaxAllowedLengthEdge"] = 20;
            obj["MaxAllowedAreaEdge"] = 1000;
            obj["MaxPerMeterNumEdge"] = 5;
        }
        obj["BlockSizeW"] = 1000;
        obj["BlockSizeH"] = g_Info[nPlate].width - 100;         /*!< 100为上下边缘宽度总和 */
        obj["PlateLength"] = g_Info[nPlate].length - 100;         /*!< 100为上下边缘宽度总和 */
    }
	else if (!QString::compare(strBiaoZhun, QString("NBT_47013_2015"), Qt::CaseInsensitive)) // TODOW 
	{
		int widthTemp = g_Info[nPlate].height; // 赋初值，暂时判断用
		if (widthTemp < 60) {
			widthTemp = 50;		// mm
		} else if (widthTemp >= 60 && widthTemp < 100) {
			widthTemp = 75;		// mm
		} else if (widthTemp >= 100) {
			widthTemp = 100;	// mm
		}

		obj["EdgeZoneWidth"] = widthTemp;                          /*!< 标准中未提及边缘宽度 暂定50 */
		if (g_strProbeType == tr("5M 单晶")) {
			// body
			if (g_strJiBie.contains("B-III"))
			{
				obj["MaxDensityNumBody"] = 5;
				obj["MinAllowedAreaBody"] = 100;
				obj["MaxAllowedAreaBody"] = 1000;
			}
			else  if (g_strJiBie.contains("B-IV"))
			{
				obj["MaxDensityNumBody"] = 20;
				obj["MinAllowedAreaBody"] = 1000;
				obj["MaxAllowedAreaBody"] = 5000;
			}
			else  if (g_strJiBie.contains("B-V"))
			{
				obj["MaxDensityNumBody"] = 200;
				obj["MinAllowedAreaBody"] = 5000;
				obj["MaxAllowedAreaBody"] = 50000;
			}
			else  if (g_strJiBie.contains("B-II"))
			{
				obj["MaxDensityNumBody"] = 10;
				obj["MinAllowedAreaBody"] = 48;
				obj["MaxAllowedAreaBody"] = 98;
			}
			else if (g_strJiBie.contains("B-I"))
			{
				obj["MaxDensityNumBody"] = 10;
				obj["MinAllowedAreaBody"] = 20;
				obj["MaxAllowedAreaBody"] = 48;
			}

			// edge
			if (g_strJiBie.contains("E-III"))
			{
				obj["MaxIndiLengthEdge"] = 50;
				obj["MaxPerMeterNumEdge"] = 5; // 5
				obj["MaxAllowedLengthEdge"] = 50;
				obj["MaxAllowedAreaEdge"] = 1000;
			}
			else  if (g_strJiBie.contains("E-IV"))
			{
				obj["MaxIndiLengthEdge"] = 100;
				obj["MaxPerMeterNumEdge"] = 6;
				obj["MaxAllowedLengthEdge"] = 100;
				obj["MaxAllowedAreaEdge"] = 2000;
			}
			else  if (g_strJiBie.contains("E-V"))
			{
				obj["MaxIndiLengthEdge"] = 1000;
				obj["MaxPerMeterNumEdge"] = 100;
				obj["MaxAllowedLengthEdge"] = 1000;
				obj["MaxAllowedAreaEdge"] = 10000;
			}
			else  if (g_strJiBie.contains("E-II"))
			{
				obj["MaxIndiLengthEdge"] = 30;
				obj["MaxPerMeterNumEdge"] = 3;
				obj["MaxAllowedLengthEdge"] = 30;
				obj["MaxAllowedAreaEdge"] = 98;
			}
			else if (g_strJiBie.contains("E-I"))
			{
				obj["MaxIndiLengthEdge"] = 20;//20
				obj["MaxPerMeterNumEdge"] = 2;//2
				obj["MaxAllowedLengthEdge"] = 20;//20
				obj["MaxAllowedAreaEdge"] = 48;// 48
			}
		} else {
			if (g_strJiBie.contains("B-III"))
			{
				obj["MaxDensityNumBody"] = 15;
				obj["MinAllowedAreaBody"] = 100;
				obj["MaxAllowedAreaBody"] = 1000;
			}
			else  if (g_strJiBie.contains("B-IV"))
			{
				obj["MaxDensityNumBody"] = 20;
				obj["MinAllowedAreaBody"] = 1000;
				obj["MaxAllowedAreaBody"] = 5000;
			}
			else  if (g_strJiBie.contains("B-V"))
			{
				obj["MaxDensityNumBody"] = 200;
				obj["MinAllowedAreaBody"] = 5000;
				obj["MaxAllowedAreaBody"] = 50000;
			}
			else  if (g_strJiBie.contains("B-II"))
			{
				obj["MaxDensityNumBody"] = 10;
				obj["MinAllowedAreaBody"] = 50;
				obj["MaxAllowedAreaBody"] = 100;
			}
			else if (g_strJiBie.contains("B-I"))
			{
				obj["MaxDensityNumBody"] = 10;
				obj["MinAllowedAreaBody"] = 20;
				obj["MaxAllowedAreaBody"] = 50;
			}

			
			if (g_strJiBie.contains("E-III"))
			{
				obj["MaxIndiLengthEdge"] = 50;
				obj["MaxPerMeterNumEdge"] = 5;
				obj["MaxAllowedLengthEdge"] = 50;
				obj["MaxAllowedAreaEdge"] = 1000;
			}
			else  if (g_strJiBie.contains("E-IV"))
			{
				obj["MaxIndiLengthEdge"] = 100;
				obj["MaxPerMeterNumEdge"] = 6;
				obj["MaxAllowedLengthEdge"] = 100;
				obj["MaxAllowedAreaEdge"] = 2000;
			}
			else  if (g_strJiBie.contains("E-V"))
			{
				obj["MaxIndiLengthEdge"] = 1000;
				obj["MaxPerMeterNumEdge"] = 100;
				obj["MaxAllowedLengthEdge"] = 1000;
				obj["MaxAllowedAreaEdge"] = 10000;
			}
			else  if (g_strJiBie.contains("E-II"))
			{
				obj["MaxIndiLengthEdge"] = 30;
				obj["MaxPerMeterNumEdge"] = 3;
				obj["MaxAllowedLengthEdge"] = 30;
				obj["MaxAllowedAreaEdge"] = 100;
			}
			else if (g_strJiBie.contains("E-I"))
			{
				obj["MaxIndiLengthEdge"] = 20;
				obj["MaxPerMeterNumEdge"] = 2;
				obj["MaxAllowedLengthEdge"] = 20;
				obj["MaxAllowedAreaEdge"] = 50;
			}
		}

		obj["BlockSizeW"] = 1000;
		obj["BlockSizeH"] = g_Info[nPlate].width - widthTemp * 2;         /*!< 100为上下边缘宽度总和 */
		obj["PlateLength"] = g_Info[nPlate].length - widthTemp * 2;         /*!< 100为上下边缘宽度总和 */
	}

    if (m_bPointJoin)
    {
        obj["MinDefectDistance"] = *m_pDefectDistaneSmaller;
    }
    else
    {
        obj["MinDefectDistance"] = 0;
    }

	// wangwei add on 2016-05-13
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
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
	setting_GuoLvQueXian.endGroup();

	obj["IsFilter"] = g_GuoLvQueXian.IsFilter;			// bool
	obj["IsCompareX1"] = g_GuoLvQueXian.IsCompareX1;	
	obj["IsCompareX2"] = g_GuoLvQueXian.IsCompareX2;
    obj["IsCompareY1"] = g_GuoLvQueXian.IsCompareY1;
	obj["IsCompareY2"] = g_GuoLvQueXian.IsCompareY2;
	obj["DifferenceX1"] = g_GuoLvQueXian.DifferenceX1;	// double
	obj["DifferenceX2"] = g_GuoLvQueXian.DifferenceX2;
	obj["DifferenceY1"] = g_GuoLvQueXian.DifferenceY1;
	obj["DifferenceY2"] = g_GuoLvQueXian.DifferenceY2;
	obj["PointStartX1"] = g_GuoLvQueXian.PointStartX1;
	obj["PointStartY1"] = g_GuoLvQueXian.PointStartY1;	
	obj["PointEndX1"] = g_GuoLvQueXian.PointEndX1;		
	obj["PointEndY1"] = g_GuoLvQueXian.PointEndY1;		
	obj["PointStartX2"] = g_GuoLvQueXian.PointStartX2;	
	obj["PointStartY2"] = g_GuoLvQueXian.PointStartY2;	
	obj["PointEndX2"] = g_GuoLvQueXian.PointEndX2;		
	obj["PointEndY2"] = g_GuoLvQueXian.PointEndY2;		
	// wangwei add end
    return obj;
}

void CCscanPanel::SetValueToImage(UTImage &image, float datavalue, QPoint point, bool direction, QSize target)
{
    QPoint directionpoint;
    if (direction) //方向水平
    {
		if(point.x() >= target.width() - 4)
		{
			directionpoint = QPoint(-1, 0);
		}
		else
		{
			directionpoint = QPoint(1, 0);
		}
    }
    else			//方向垂直
    {
		if(point.y() >= target.height() - 4)
		{
			directionpoint = QPoint(0, -1);
		}
		else
		{
			directionpoint = QPoint(0, 1);
		}
		
    }
	if (point.x() >= target.width())
	{
		point.setX(target.width() - 5);
	}
	if (point.y() >= target.height())
	{
		point.setY(target.height() - 5);
	}
    for (int i = 0; i < 4; i++)
    {
        image(point) = datavalue;
        point = point + directionpoint;
    }
}
//dong qi add end on 2013-11-22


//chenyuzhu add start on 2014-5-12

//////////////////////////////////////////////////////////////////////////
/** 
* 画标尺
* @since     1.0
*/
void CCscanPanel::m_DrawVRuler(QPainter& painter,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)
{
	//QPainter painter(device);
	painter.setPen(Qt::black);
	if (nEnd == 20)
	{
		int test = 0;
	}
	QRect rectText;
	int nNumber = 0;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( nTextLocation == 1 ) 
	{
		rectText.setRight(rect.right()-10);

		iLLineStart = rect.right() - 18;
		iLLineEnd	 = rect.right() - 2;

		iMLineStart = rect.right() - 12;
		iMLineEnd	 = rect.right() - 2;

		iSLineStart = rect.right() - 8;
		iSLineEnd	 = rect.right() - 2;

	}
	else if( nTextLocation == 0 ) 
	{
		rectText.setLeft(rect.left()+10);

		iLLineStart = rect.left()  + 2;
		iLLineEnd	 = rect.left()  + 18;

		iMLineStart = rect.left()  + 2;
		iMLineEnd	 = rect.left()  + 12;

		iSLineStart = rect.left() + 2;
		iSLineEnd	 = rect.left() + 8;
	}

	QString str;

	int iPos;


	bool bXiaoyu = FALSE;
	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}
	nStep = nStep/10;
	if (nStep%5 != 0)
	{
		if (nStep > 3)
		{
			nStep = (nStep/5 + 1)*5;
		}
		else
		{

		}
	}


	int nLinestart = (nStart/nStep + 1)*nStep;
	if (nStart == 0)
	{
		nLinestart = 0;
	}
	for (int i = nLinestart;i<=nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{
			if( nTextDirection == 0 )  
			{
				iPos = rect.top() + (rect.height()*(i - nStart))/(nEnd - nStart);
				rectText.setTop(iPos-10);
			}
			else if(nTextDirection == 1 )
			{
				iPos = rect.bottom() - (rect.height()*(i - nStart))/(nEnd - nStart);
				rectText.setTop(iPos-13);
			}
			rectText.setBottom(rectText.top()+20);

			if (i % (nStep*5) == 0)
			{
				painter.drawLine(iMLineStart,iPos,iMLineEnd,iPos);		
				if (i % (nStep*10) == 0)
				{
					painter.drawLine(iLLineStart,iPos,iLLineEnd,iPos);	
					if( nTextLocation == 0 )
					{
						rectText.setRight(rectText.left()+32);
					}
					else
					{
						rectText.setLeft(rectText.right()-30);
					}

					if (rectText.top() < rect.top())
					{
						rectText.setTop(rect.top()+1);
					}
					if (rectText.bottom() > rect.bottom())
					{
						rectText.setBottom(rect.bottom()+10);
					}
					if (bXiaoyu)
					{
						nNumber=100-i/100;
						// 						if (nNumber<0)
						// 						{
						// 							nNumber=-nNumber;
						// 						}
						str.setNum(nNumber);
					}
					else	
					{
						str.setNum(i);
					}
					if( nTextLocation == 0 )
					{
						painter.drawText( rectText,Qt::AlignHCenter,str  );
					}
					else
					{
						painter.drawText( rectText,Qt::AlignHCenter,str );
					}

					continue;
				}
			}
			painter.drawLine(iSLineStart,iPos,iSLineEnd,iPos);		

		}
	}
	return;
}

/** 
* 画标尺
* @since     1.0
*/
//nTextLocation为1时，刻度画在区域的底部；为0时，刻度画在区域的顶部
void CCscanPanel::m_DrawHRuler(QPainter& painter,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)
{

	//QPainter painter(device);
	painter.setPen(Qt::black);

	int iStartPos = 0;  

	int iLLineStart,iLLineEnd;
	int iMLineStart,iMLineEnd;
	int iSLineStart,iSLineEnd;

	QRect rectText;
	if( nTextLocation == 1 ) 
	{

		rectText.setTop(rect.bottom()- 25);
		rectText.setBottom(rect.bottom()- 10);

		iLLineStart = rect.bottom() - 9;
		iLLineEnd	 = rect.bottom() - 2;

		iMLineStart = rect.bottom() - 6;
		iMLineEnd	 = rect.bottom() - 2;

		iSLineStart = rect.bottom() - 4;
		iSLineEnd	 = rect.bottom() - 2;

	}
	else if(nTextLocation == 0)
	{

		rectText.setTop(rect.top()+20);
		rectText.setBottom(rect.top()+ 35);

		iLLineStart = rect.top() + 2;
		iLLineEnd = rect.top() + 18;

		iMLineStart = rect.top() + 2;
		iMLineEnd = rect.top() + 12;

		iSLineStart = rect.top() + 2;
		iSLineEnd = rect.top() + 8;
	}

	QString str;
	int iPos;
	int iSel = 0;
	int nLinestart = 0;

	bool bXiaoyu = FALSE;
	int  i = 0;
	int  iSayac = 0;

	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}

	nStep = nStep/10;

	if (nStep%5 != 0)
	{
		nStep = (nStep/5 + 1)*5;
	}

	nLinestart = (nStart/nStep + 1)*nStep; 
	if (nStart == 0)
	{
		nLinestart = 0;
	}

	for (i = nLinestart;i < nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{
			if (nTextDirection == 0)
			{
				iPos = rect.left() + ((rect.width())*(i - nStart))/(nEnd - nStart);
				rectText.setLeft(iPos - 50);
			}
			else
			{
				iPos = rect.right() - ((rect.width())*(i - nStart))/(nEnd - nStart);
				rectText.setLeft(iPos-50);
			}

			if (i % (nStep*5) == 0)
			{
				painter.drawLine(iPos,iMLineStart,iPos,iMLineEnd);	
				if (i % (nStep*10) == 0)
				{
					painter.drawLine(iPos,iLLineStart,iPos,iLLineEnd);	

					rectText.setRight(iPos+60);  
					if (rectText.right() > rect.right())
					{
						rectText.setRight(rect.right()+25);
					}
					if (bXiaoyu)
					{
						str.setNum(i/100);
					}
					else	
					{
						str.setNum(i);
					}
					if(   nTextLocation == 1 ) 
					{
						painter.drawText( rectText, Qt::AlignHCenter ,str);
					}
					else
					{
						painter.drawText( rectText, Qt::AlignHCenter ,str);
					}

					continue;
				}
				continue;
			}
			painter.drawLine(iPos,iSLineStart,iPos,iSLineEnd);
		}
	}
	return;
}

//chenyuzhu add end on 2014-5-12

//chenyuzhu add start on 2014-5-22
void CCscanPanel::OnIfPrint()
{
	m_IfPrint = true;
	OnMenuPrint();
}
//chenyuzhu add end on 2014-5-22

//chenyuzhu add start on 2014-6-9
void CCscanPanel::OnDestroy()
{
	//根据权限设置按钮功能
	if ( QString("管理员") != g_strUserPer)
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("当前用户无法执行此操作"));
		return;
	}
	//保存参数验证
	CInspectUser *pInspectUser = new CInspectUser(g_sqlDB);
	pInspectUser->exec();
	if (pInspectUser->m_isOK)
	{
		m_nLogicXStart = m_pWgtCscanDetectView->m_nLogicXStart;
		m_nLogicXEnd = m_pWgtCscanDetectView->m_nLogicXEnd;
		m_nLogicYStart = m_pWgtCscanDetectView->m_nLogicYStart;
		m_nLogicYEnd = m_pWgtCscanDetectView->m_nLogicYEnd;

		if (qFuzzyCompare(m_nLogicXStart, 0) && qFuzzyCompare(m_nLogicXEnd, 0) && qFuzzyCompare(m_nLogicYStart, 0) && qFuzzyCompare(m_nLogicYEnd, 0)) {
			QMessageBox::information(this,"CCscanPanel::OnDestroy",tr("没有选中伤点！请在放大区选中要删除的伤点！"));
			return ;
		}

		if(QMessageBox::Yes == QMessageBox::warning(NULL, "warning", tr("此操作不可恢复，要删除此面伤吗?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
			BOOL bRet = DeleteZoomInFlawInFile(m_nLogicXStart, m_nLogicXEnd, m_nLogicYStart, m_nLogicYEnd);		

			if (!bRet) {
				return;
			}	

			QString strFileName = QDir::currentPath() + "/DianShang.txt";
			QString strYasuoFileName = QDir::currentPath() + "/DianShang.rar";
			g_CS.lock();
			QString strID = g_MaxCheckId;
			g_CS.unlock();
			
			//将伤点文件生成压缩包
			if (!g_pPlateDraw->isNull()) {
				bRet = g_pPlateDraw->dynamicCall("ZipFile(QString, QString)", strFileName, strYasuoFileName).toBool();
				if (!bRet) {
					QMessageBox::information(this,"SaveResult2DB",tr("压缩点伤文件失败,请确认是否注册了正确版本的OCX控件"));
					return ;
				}
			}	

#if HAVE_DATA_BASE
			// 把点伤文件压缩包存到t_DianShang2数据库里，id为t_PlateCheckInfo
			QSqlQuery query(*g_sqlDB);
			QFile file(strYasuoFileName);
			file.open(QIODevice::ReadOnly);
			QByteArray data = file.readAll();
			QVariant var(data);
			QString strSQL;
			//strSQL = QString("%1").arg("update t_DianShang2(Check_id,Data) values(?,?)");
			strSQL = QString("%1").arg("update T_PlateCheckInfo set Data_File =? where ID = ?");
			query.clear();
			query.prepare(strSQL);
			query.addBindValue(var);
			query.addBindValue(strID);
			bRet = query.exec();		
#else
			QMessageBox::information(this, "CCscanPanel::OnDestroy", tr("未连接数据库"));
#endif
			emit SignalToDeleteReDraw();
		}
		else
		{
			return;
		}
	}
}

BOOL CCscanPanel::DeleteZoomInFlawInFile(const double& dMinX, const double& dMaxX,
										const double& dMinY, const double& dMaxY)
{	
	QString strFilePath = QDir::currentPath()+ "/DianShang.txt";
	QString strTmpFilePath = QDir::currentPath() + "/TmpDianShang.txt";
	QFile file(strFilePath);
	

	bool bRet = file.open(QIODevice::ReadOnly);
	if (bRet)//文件打开正常
	{
		file.seek(0);
		quint64 nFileSize = file.size();
		
		QFile fileTmp(strTmpFilePath);
		fileTmp.open(QIODevice::ReadWrite);
		fileTmp.seek(0);

		// 清除点伤文件里的此区域的点
		stDianShang tmp;
		quint16 size = sizeof(stDianShang);

		while(nFileSize) {
			quint64 nBytesRead = file.read((char*)&tmp, size);

			if (tmp.fX >= (dMinX - g_IniParams.m_DelKuoXMin) 
			 && tmp.fX <= (dMaxX + g_IniParams.m_DelKuoXMax) 
			 && tmp.fY >= (dMinY - g_IniParams.m_DelKuoYMin) 
			 && tmp.fY <= (dMaxY + g_IniParams.m_DelKuoYMax)) {
			} else {
				fileTmp.write((char*)&tmp, sizeof(stDianShang));
			}

			nFileSize -= nBytesRead;
		}

		file.close();
		fileTmp.close();
		// 删除原点伤文件，将过滤后的点伤文件改为原文件名
		BOOL bRet = QFile::remove(strFilePath);
		if (bRet)
		{
			bool bRen = QFile::rename( strTmpFilePath, strFilePath);
		}
		else
		{
			QMessageBox::information(this,"错误",tr("删除点伤文件失败！"));
			return FALSE;
		}
	}
	else//文件打开失败
	{
		QMessageBox::information(this,"错误",tr("伤点文件大开失败！"));
		return FALSE;
	}
	return TRUE;
}
//chenyuzhu add end on 2014-6-9

//chenyuzhu add start on 2014-6-24
void CCscanPanel::DrawTanTouLine(int x, bool bdraw)
{
	//全局视图探头画线

	m_bTanTouDraw = bdraw;
	m_fTanTouY = GetX(x);
	update();
//	DrawFlawDC(m_pWgtWholeView->m_nXSize,m_pWgtWholeView->m_nYSize,m_pWgtWholeView,m_pWgtWholeView->m_pWholeImage);
//	QPainter widgetPainter(m_pWgtWholeView);
//	widgetPainter.drawImage(QRect(46,1,m_pWgtWholeView->rect().width()-47,m_pWgtWholeView->rect().height()-40),*(m_pWgtWholeView->m_pWholeImage),QRect(0,0,m_pWgtWholeView->m_pWholeImage->rect().width(),m_pWgtWholeView->m_pWholeImage->rect().height()));
}

float CCscanPanel::GetX(int nLuoJiNum)
{
	float fY;
	int nBoxIndex = 0;
	int nProbeIndex = 0;
	int nJiOu = 0;

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

	if (nLuoJiNum >= 1000 && nLuoJiNum < 5000)
	{
		nBoxIndex = (nLuoJiNum % 1000) / 3;
		nProbeIndex = (nLuoJiNum % 1000) % 3;		
	}

	 if (nLuoJiNum >= 3000 && nLuoJiNum < 4000)	// 内探1
	{
		fY = (nBoxIndex - g_NeiTanBoxStartNum.m_nNeitan1StartNum) * 120 + nProbeIndex * 20 + dx.m_fNeiTan1DeltaX;		
	}
	else if (nLuoJiNum >= 4000 && nLuoJiNum < 5000)	// 内探2
	{
		fY = (nBoxIndex - g_NeiTanBoxStartNum.m_nNeitan2StartNum) * 120 + nProbeIndex * 20 + dx.m_fNeiTan2DeltaX;
	}
	return fY;
}
//chenyuzhu add end on 2014-6-24


//chenyuzhu add start on 2014-6-26
void CCscanPanel::OnCidentLog(void)
{
	//chenyuzhu add start on 2014.5.8
	if (!g_bStart&&!g_bStop)
	{
		QMessageBox::information(this,"CCscanPanel::OnMenuPrint",tr("探伤未开始"));
		return;
	}
	if (!m_IfInitBiaoZhun)
	{
		QMessageBox::information(this,"CCscanPanel::OnMenuPrint",tr("标准未初始化"));
		return;
	}
	//chenyuzhu add end on 2014.5.8
	//注释掉了用于调试报表 by chenyuzhu
	SaveResult2DB();                     //在数据库中保存报表数

	PrintLogResult();               //打印报表
}


// 
void CCscanPanel::PrintLogResult()
{
	// 
	QString strBiaoZhun = g_strBiaoZhun;
	strBiaoZhun.trimmed();
	// 用到预览对话框类
	m_PreviewWnd.InitTableItem(strBiaoZhun);
	// 根据ID从数据库读值
	long lID;
	g_CS.lock();
	lID = g_MaxCheckId.toLong();
	g_CS.unlock();
	ReadDBValue(lID);
	WriteLog();   
}

void CCscanPanel::WriteLog()
{
	QString strFileName = QDir::currentPath()+ "/CidentLog.txt";
	QFile file(strFileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Append))
	{
		QMessageBox::warning(NULL,"InitLog",QObject::tr("CidentLog.txt打开失败"));
		return ;
	}
	QTextStream stream(&file);
	stream << tr("事件日志") <<"\r\n";
	//////////////////////////////////////////////////////////////////////////

	// 打印钢板信息
	stream << tr("钢板信息") <<"\r\n";   
	
	int nSelectNum = m_PreviewWnd.m_plateArray.size();
	QString strKey;
	QString strValue;
	for (int iLoop = 0; iLoop <nSelectNum;iLoop++)
	{
		strKey = m_PreviewWnd.m_plateArray[iLoop];
		strValue = m_Map[strKey];
		strKey += tr(": ");	
		stream << strKey << strValue <<"\r\n"; 
	}

	// 显示探伤基本信息
	stream << "\r\n";   
	stream << tr("探伤基本信息") <<"\r\n";   
	int nTanshangSelectedNum = m_PreviewWnd.m_TanShangArray.size();
	for (int iLoop = 0; iLoop <nTanshangSelectedNum;iLoop++)
	{
		strKey = m_PreviewWnd.m_TanShangArray[iLoop];
		strValue = m_Map[strKey];
		strKey += tr(": ");
		stream << strKey << strValue <<"\r\n"; 
	}	

	// 显示探伤结果信息
	stream << "\r\n";   
	stream << tr("探伤结果信息") <<"\r\n"; 
	int nResultSelectedNum = m_PreviewWnd.m_ResultArray.size();
	for (int iLoop = 0; iLoop <nResultSelectedNum;iLoop++)
	{
		strKey = m_PreviewWnd.m_ResultArray[iLoop];
		strValue = m_Map[strKey];
		strKey += tr(": ");
		stream << strKey << strValue <<"\r\n"; 
	}

	//////////////////////////////////////////////////////////////////////////
	file.close();	
}
//chenyuzhu add end on 2014-6-26

//chenyuzhu add start on 2014-7-17
void  CCscanPanel::ReadDatatoList()
{
	/*******读取SheBeiFile文件********/

	QString strFileName1 = QDir::currentPath()+ "/SheBeiFile.dbfl";
	QFile file1(strFileName1);
	if (!file1.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"ReadDatatoList",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	SHEBEICANSHU SheBeiCanShu;

	file1.seek(0);

	quint64 nBytesRead1 = file1.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));//读文件到结构体中
	file1.close();
	/*******读取platefile.dbfl文件********/

	QString strFileName2 = QDir::currentPath()+ "/platefile.dbfl";
	QFile file2(strFileName2);
	if (!file2.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"ReadDatatoList",QObject::tr("platefile.dbfl打开失败"));
		return ;
	}
	PLATEFILE_PARAM PlateParam;

	file2.seek(0);

	quint64 nBytesRead2 = file2.read((char*)&PlateParam,sizeof(PLATEFILE_PARAM));//读文件到结构体中
	file2.close();
	/*******读取PlateCheckInfoFile文件********/

	QString strFileName3 = QDir::currentPath()+ "/PlateCheckInfoFile.dbfl";
	QFile file3(strFileName3);
	if (!file3.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(NULL,"ReadDatatoList",QObject::tr("PlateCheckInfoFile.dbfl打开失败"));
		return ;
	}
	PLATECHECKINFOFILE PlateCheckInfo;

	file3.seek(0);

	quint64 nBytesRead3 = file3.read((char*)&PlateCheckInfo,sizeof(PLATECHECKINFOFILE));//读文件到结构体中
	file3.close();
	
	//填充list
	//QList<QString> list;
	m_PlateInfo.clear();
	m_PlateInfo.insert(0, g_MaxCheckId);
	m_PlateInfo.insert(1, PlateParam.strName);
	m_PlateInfo.insert(2, PlateParam.strLu);
	m_PlateInfo.insert(3, PlateParam.strMuBan);
	m_PlateInfo.insert(4, PlateParam.strShenChan);
	m_PlateInfo.insert(5, PlateParam.strHeTong);
	m_PlateInfo.insert(6, PlateParam.strGangJi);
	m_PlateInfo.insert(7, PlateParam.strJiaoHuoStatus);
	m_PlateInfo.insert(8, PlateParam.strPlateType);
	
	//chenyuzhu modify on 2014-7-18 暂时修改为重钢板文件中获得长宽厚重量 
	//m_PlateInfo.insert(9, QString::number(SheBeiCanShu.Plate_Type.fLength));
	//m_PlateInfo.insert(10, QString::number(SheBeiCanShu.Plate_Type.fWidth));
	//m_PlateInfo.insert(11, QString::number(SheBeiCanShu.Plate_Type.fThick));
	//m_PlateInfo.insert(12, QString::number(SheBeiCanShu.Plate_Type.fWeight));
	m_PlateInfo.insert(9, QString::number(PlateParam.fLength));
	m_PlateInfo.insert(10, QString::number(PlateParam.fWidth));
	m_PlateInfo.insert(11, QString::number(PlateParam.fThick));
	m_PlateInfo.insert(12, QString::number(PlateParam.fWeight));
	//chenyuzhu modify on 2014-7-18
	
	m_PlateInfo.insert(13, PlateCheckInfo.strResult);
	m_PlateInfo.insert(14, PlateCheckInfo.strCheckTime);
	m_PlateInfo.insert(15, PlateCheckInfo.strCheckClass);
	m_PlateInfo.insert(16, QString::number(PlateCheckInfo.nDianShangCount));
	m_PlateInfo.insert(17, QString::number(PlateCheckInfo.nMianShangCount));
	m_PlateInfo.insert(18, QString::number(PlateCheckInfo.fMianShangAllMianJi));
	m_PlateInfo.insert(19, QString::number(PlateCheckInfo.fQueXianPercent));
	m_PlateInfo.insert(20, QString::number(PlateCheckInfo.fQueXianMaxLength));
	m_PlateInfo.insert(21, QString::number(PlateCheckInfo.fQuexianMaxWidth));
	m_PlateInfo.insert(22, PlateCheckInfo.strCheckName);
	m_PlateInfo.insert(23, PlateCheckInfo.strName);
	m_PlateInfo.insert(24, PlateCheckInfo.strCheckMethod);
	m_PlateInfo.insert(25, SheBeiCanShu.TanShangGongYi.strYiQiXingHao);
	m_PlateInfo.insert(26, SheBeiCanShu.TanShangGongYi.strDetector);
	m_PlateInfo.insert(27, SheBeiCanShu.TanShangGongYi.strTanShangMethod);
	m_PlateInfo.insert(28, QString::number(SheBeiCanShu.YiQiParam.fTanTouZhongXinJuLi));
	//m_PlateInfo.insert(29, SheBeiCanShu.TanShangGongYi.strTanShangLinMingDu);
	if (g_strBiaoZhun == "BS_EN_ISO_10893")
	{
		m_PlateInfo.insert(29, "Φ 6");
	}
	else
	{
		m_PlateInfo.insert(29, "Φ 5");
	}
	
	m_PlateInfo.insert(30, QString::number(SheBeiCanShu.TanShangGongYi.fShaoMiaoJianGe));
	m_PlateInfo.insert(31, SheBeiCanShu.TanShangGongYi.strOuHeJi);
	m_PlateInfo.insert(32, QString::number(PlateCheckInfo.bIsReEvaluate));
	m_PlateInfo.insert(33, PlateCheckInfo.strCheckBodyLevel);
	m_PlateInfo.insert(34, PlateCheckInfo.strCheckBiaoZhun);
	m_PlateInfo.insert(35, QString::number(PlateCheckInfo.fAllMaxQueXianLv));
	m_PlateInfo.insert(36, PlateParam.strGangZhong);
	m_PlateInfo.insert(37, QString::number(PlateCheckInfo.fQueXianMaxArea));
	m_PlateInfo.insert(38, SheBeiCanShu.Plate_Type.strInfo);
	m_PlateInfo.insert(39, PlateCheckInfo.strCheckEdgeLevel);
	m_PlateInfo.insert(40, QString::number(PlateCheckInfo.fBodyMaxZhiShiLength));
	m_PlateInfo.insert(41, QString::number(PlateCheckInfo.fBodyMaxQueXianarea));
	m_PlateInfo.insert(42, QString::number(PlateCheckInfo.fBodyMaxLength));
	m_PlateInfo.insert(43, QString::number(PlateCheckInfo.fBodyMaxWidth));
	m_PlateInfo.insert(44, QString::number(PlateCheckInfo.fEdgeMaxZhiShiLength));
	m_PlateInfo.insert(45, QString::number(PlateCheckInfo.fEdgeMaxArea));
	m_PlateInfo.insert(46, QString::number(PlateCheckInfo.fEdgeMaxLength));
	m_PlateInfo.insert(47, QString::number(PlateCheckInfo.fEdgeMaxWidth));
	m_PlateInfo.insert(48, QString::number(PlateCheckInfo.fBodyAvgQueXianLv));
	m_PlateInfo.insert(49, QString::number(PlateCheckInfo.fEdgeAvgQueXianLv));
	m_PlateInfo.insert(50, QString::number(PlateCheckInfo.fBodyMaxQueXianLv));
	m_PlateInfo.insert(51, QString::number(PlateCheckInfo.fEdgeMaxQueXianLv));
	m_PlateInfo.insert(52, QString::number(PlateCheckInfo.fBodyMaxCountSquarMeter));
	m_PlateInfo.insert(53, QString::number(PlateCheckInfo.fEdgeMaxCountPerMeter));
	m_PlateInfo.insert(54, QString::number(PlateCheckInfo.fAllMaxCountSquarMeter));
	m_PlateInfo.insert(55, QString::number(PlateCheckInfo.nFlag));
	m_PlateInfo.insert(56, PlateCheckInfo.strCheckBodyLevel);
	m_PlateInfo.insert(57, PlateCheckInfo.strCheckBiaoZhun);
	m_PlateInfo.insert(58, PlateCheckInfo.strCheckEdgeLevel);
	return ;
}
//chenyuzhu add end on 2014-7-17

//chenyuzhu add start on 2014-7-17
void  CCscanPanel::OnToolsToUpdate(void)
{
    if (recognizer.isNull())
    {
        return;
    }
    update();
    JiSuanBiaoZhun();
}

void  CCscanPanel::JiSuanBiaoZhun(void)
{
    if (recognizer.isNull())
    {
        QMessageBox::information(NULL, "CCscanPanel::JiSuanBiaoZhun", "无效标准");
        return;
    }
    float result = recognizer->GetPartValue("Result");
    if (result >= 1.0)
    {
        m_strResult = tr("不合格");
    }
    else
    {
        m_strResult = tr("合格");
    }

	if (m_strResult == tr("不合格"))
	{
		//m_pCbDetectResult->setCurrentIndex(0); 
		m_pPstBtnDetectResult->setText(tr("不合格"));
		m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0); border-style: ridge;border-color: rgb(255, 255, 255);"));//红色
	}else if (m_strResult == tr("合格"))
	{
		//m_pCbDetectResult->setCurrentIndex(1); 
		m_pPstBtnDetectResult->setText(tr("合格"));
		m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0); border-style: ridge;border-color: rgb(255, 255, 255);"));//绿色
	}
	else
	{
		m_pPstBtnDetectResult->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0); border-style: ridge;border-color: rgb(255, 255, 255);"));//黄色
	}
}

DetailInfo CCscanPanel::GetDetailInfo(const QString& strBiaoZhun)
{
    DetailInfo info;

    bool bAllMaxQueXianLv = FALSE;
    bool bBodyAvgQueXianLv = FALSE;
    bool bBodyMaxQueXianLv = FALSE;
    bool bEdgeAvgQueXianLv = FALSE;
    bool bEdgeMaxQueXianLv = FALSE;
    bool bBodyMaxShangCountPerSquareMeter = FALSE;
    bool bEdgeMaxShangCountPerMeter = FALSE;
    bool bAllMaxShangCountPerSquareMeter = FALSE;
    bool bBodyMaxShangZhiShiLength = FALSE;
    bool bBodyMaxLength = FALSE;
    bool bBodyMaxWidth = FALSE;
    bool bBodyMaxQueXianArea = FALSE;
    bool bEdgeMaxShangZhiShiLength = FALSE;
    bool bEdgeMaxQueXianArea = FALSE;
    bool bEdgeMaxShangLength = FALSE;
    bool bEdgeMaxShangWidth = FALSE;

    QString strSQL;
    strSQL = QString("select * from t_PlatePrintSet where name = '%1'").arg(strBiaoZhun);

#if HAVE_DATA_BASE
    QSqlQuery query(*g_sqlDB);
    query.exec(strSQL);
    query.next();

    // 获取字段名
    bBodyMaxShangZhiShiLength = query.value(33).toBool();
    bBodyMaxQueXianArea = query.value(34).toBool();
    bBodyMaxLength = query.value(35).toBool();
    bBodyMaxWidth = query.value(36).toBool();
    bEdgeMaxQueXianArea = query.value(37).toBool();
    bEdgeMaxShangZhiShiLength = query.value(38).toBool();
    bEdgeMaxShangLength = query.value(39).toBool();
    bEdgeMaxShangWidth = query.value(40).toBool();
    bBodyAvgQueXianLv = query.value(41).toBool();
    bEdgeAvgQueXianLv = query.value(42).toBool();
    bBodyMaxQueXianLv = query.value(43).toBool();
    bEdgeMaxQueXianLv = query.value(44).toBool();
    bAllMaxQueXianLv = query.value(45).toBool();
    bBodyMaxShangCountPerSquareMeter = query.value(46).toBool();
    bEdgeMaxShangCountPerMeter = query.value(47).toBool();
    bAllMaxShangCountPerSquareMeter = query.value(48).toBool();
#endif

    if(bAllMaxQueXianLv)
    {
        info.m_AllMaxQueXianLv = recognizer->GetPartValue("MaxDensityAll");
    }
    if(bBodyAvgQueXianLv)
    {
        info.m_BodyAvgQueXianLv = recognizer->GetPartValue("AvgRatioBody");
    }
    if(bBodyMaxQueXianLv)
    {
        info.m_BodyMaxQueXianLv = recognizer->GetPartValue("MaxRatioBody");
    }
    if(bEdgeAvgQueXianLv)
    {
        info.m_EdgeAvgQueXianLv = recognizer->GetPartValue("AvgRatioEdge");
    }
    if(bEdgeMaxQueXianLv)
    {
        info.m_EdgeMaxQueXianLv = recognizer->GetPartValue("MaxRatioEdge");
    }
    if(bBodyMaxShangCountPerSquareMeter)
    {
       // info.m_perBodyMaxQueXianCount = g_pPlateDraw->dynamicCall("GetBodyMaxShangCountPerSquareMeter()").toDouble();
        info.m_perBodyMaxQueXianCount = recognizer->GetPartValue("PerSquareMeterMaxNumBody");
    }
    if(bEdgeMaxShangCountPerMeter)
    {
       // info.m_perEdgeMaxQueXianCount = g_pPlateDraw->dynamicCall("GetEdgeMaxShangCountPerMeter()").toDouble();
        info.m_perEdgeMaxQueXianCount = recognizer->GetPartValue("PerMeterMaxNumEdge");
    }
    if(bAllMaxShangCountPerSquareMeter)
    {
       // info.m_perAllMaxQueXianCount = g_pPlateDraw->dynamicCall("GetAllMaxShangCountPerSquareMeter()").toDouble();
       // info.m_perAllMaxQueXianCount = recognizer->GetPartValue("PerMeterMaxNumEdge");
    }
    if(bBodyMaxShangZhiShiLength)
    {
       // info.m_BodyMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetBodyMaxShangZhiShiLength()").toDouble();
        info.m_BodyMaxZhiShiLength = recognizer->GetPartValue("MaxIndiLengthBody");
    }
    if(bBodyMaxLength)
    {
       // info.m_BodyMaxLength = g_pPlateDraw->dynamicCall("GetBodyMaxLength()").toDouble();
        info.m_BodyMaxLength = recognizer->GetPartValue("MaxLengthBody");
    }
    if(bBodyMaxWidth)
    {
      //  info.m_BodyMaxWidth = g_pPlateDraw->dynamicCall("GetBodyMaxWidth()").toDouble();
        info.m_BodyMaxWidth = recognizer->GetPartValue("MaxWidthBody");
    }
    if(bBodyMaxQueXianArea)
    {
      //  info.m_BodyMaxArea = g_pPlateDraw->dynamicCall("GetBodyMaxQueXianArea()").toDouble();
        info.m_BodyMaxArea = recognizer->GetPartValue("MaxAreaBody");
    }
    if(bEdgeMaxShangZhiShiLength)
    {
      //  info.m_EdgeMaxZhiShiLength = g_pPlateDraw->dynamicCall("GetEdgeMaxShangZhiShiLength()").toDouble();
        info.m_EdgeMaxZhiShiLength = recognizer->GetPartValue("MaxIndiLengthEdge");
    }
    if(bEdgeMaxQueXianArea)
    {
      //  info.m_EdgeMaxArea = g_pPlateDraw->dynamicCall("GetEdgeMaxQueXianArea()").toDouble();
        info.m_EdgeMaxArea = recognizer->GetPartValue("MaxAreaEdge");
    }
    if(bEdgeMaxShangLength)
    {
      //  info.m_EdgeMaxLength = g_pPlateDraw->dynamicCall("GetEdgeMaxLength()").toDouble();
        info.m_EdgeMaxLength = recognizer->GetPartValue("MaxLengthEdge");
    }
    if(bEdgeMaxShangWidth)
    {
      //  info.m_EdgeMaxWidth = g_pPlateDraw->dynamicCall("GetEdgeMaxWidth()").toDouble();
        info.m_EdgeMaxWidth = recognizer->GetPartValue("MaxWidthEdge");
    }
    return info;
}

void CCscanPanel::OnEnglishUpdata(int isEnglish)
{		
	g_bEnglish = isEnglish;
	if (isEnglish)
	{
		m_PreviewWnd.UpdateTitleMap();
	}
	else
	{
		m_PreviewWnd.InitTitleMap();
	}
}