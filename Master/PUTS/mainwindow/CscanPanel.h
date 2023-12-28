/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* C扫界面
* Filename: CscanPanel.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-04-27
*/

#ifndef CSCANPANEL_H
#define CSCANPANEL_H

#include <QWidget>
#include <QDockWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QPoint>
#include <QWheelEvent>
#include <QList>
#include <QSettings>
#include "DataDefine.h"
#include "CustumPlateTablePreview.h"
#include "BiaoZhun.h"
#include "BiaoZhun_GBT2970.h"
#include <QPrinter>
#include "../Source/arsCore/UTImage.h"

class CDockForm;
class CInspectUser;
class CScanThicknessWidget;
class CScanFrontWidget;//正视图
class CScanSideWidget;//侧视图
class CScanWholeWidget;//整体视图
class CScanDetectWidget;//伤点图
class CScanLeftWidget;//左侧边缘伤点图
class CScanRightWidget;//右侧边缘伤点图
class CScanTopWidget;//上侧边缘伤点图
class CScanBottomWidget;//下侧边缘伤点图
class PUTSRecognizer;
class UTImage;

class CCscanPanel : public QDockWidget
{
	Q_OBJECT
public:
	CCscanPanel(QSqlDatabase *pDatabaseConnection = NULL,QWidget *parent = NULL);
	~CCscanPanel();
	void resizeEvent(QResizeEvent *e);
	// 替换点伤数据的CheckID
	void ChangeFileCheckID(const QString& strFileName, const QString& strNewID );
	// 检索本体和边缘级别
	void GetBodyAndEdgeLevel(QString& strBodyLevel,	QString& strEdgeLevel);
	void PrintBiaoZhunResult();	 	// 打印与标准相对应的板级报表
	void InitBiaoZhun();            //初始化标准
	void BiaoZhunVectorInit2970();  //2970标准

    QScopedPointer<PUTSRecognizer> recognizer;
private:
	void InitCscanWindow();
	void ConectFun();
	void DrawFlawDC(int XSize,int YSize,QWidget *CurrentWidget,QImage *&CurrentImage);
	void DrawFlawDetail(QPainter& painter,int xSize,int ySize,const double& fStart,const double& fEnd);
	void DrawCrossSymbol(QPainter& painter, QPoint& pointMin, QPoint& pointMax, const QColor &color);
	void DrawSquareSymbol(QPainter& painter, QPoint& pointMin, QPoint& pointMax, const QColor &color);
	unsigned short ShadowStr2Array(QString& str, int* pArray, const unsigned short &nSize);		// 将投影数据字符串转换为数组
	void UpdateFlawDataList();    //更新伤点链表
    int  CalculatePointForPannel(int Flag,QList<double>& argList,QList<int>& ResultList);  //给视图计算坐标的方法
	bool SaveResult2DB();
	void SaveDPIData();
	// 打印板极报表
	void PrintPlateTable();
	void ReadDBValue(long CheckID);
	void ReadData(long CheckID,QVector<QString>& strArray);
	void DrawPrintText(QRect& rect,QPainter& painter);
	void DrawPrintWhole(QRect& rect,QPainter& painter);
	void DrawPrintFront(QRect& rect,QPainter& painter);
	void DrawPrintSide(QRect& rect,QPainter& painter);
	void DrawEasyVScale(QPoint Start,QPoint End,int nNum,QPainter& painter,int nMax);
	void DrawEasyHScale(QPoint Start,QPoint End,int nNum,QPainter& painter,int nMax);
	bool PLCSelfTest();
	
	QString ChineseToEnglish(QString str);


protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void contextMenuEvent(QContextMenuEvent * e);
	void timerEvent( QTimerEvent *event );  //定时器事件处理器
signals:
	bool SignalToDrawing();                              //通知主界面画图
	void SignalToDetectListDlg();                        //通知DetectListDlg更新Model
	void SignalToSetPlateId();                           //MainWindow重新设置钢板信息

	void SignalToDeleteReDraw();						//通知伤点删除后重新计算并重绘各视图chenyuzhu add on 2014-6-10
public slots:
	void OnPlateScanEnd();//探伤结束
	void ModifyDetectResult(int);
	void DrawFrontDetial();//正视图
	void DrawXShadowRegion();// 画X投影区域
	void DrawSideDetial(); //侧视图
	void DrawYShadowRegion();// 画Y投影区域
	void DrawWholeDetial(); //全局视图视图
	void DrawRunningWholeDetial(); //探测中的全局视图视图
    void DrawDetectDetial(); //局部视图
	void DrawRunningDetectDetial();   //探测时的局部视图 
	void DrawThicknessDetial(); //厚度图
	void DrawRunningThicknessDetial(); //探测中的厚度图
	//局部视图中实时画的点信息
	void OnAddFlawData(CH_FLAWDATA* pData);  //更新m_flowStore数据，并画图
	//整体视图中实时画的点信息
	void OnAddFlawDataToWhole(CH_FLAWDATA* pData);  //更新m_FlawCoordinateArray数据，并画图
	void OnChangeSelectRect(int x,int y,int width,int height);           //响应全局视图的鼠标单击事件
	void DrawPlateFlawSection();
	void OnMenuPrint();           //响应保存并打印菜单 
	void OnRecticCheck(int nCheck);  //中心十字线
	void OnPointRangeCheck(int nCheck); //伤点范围线

	void OnEnglishUpdata(int isEnglish);

	void OnDefectDistantCheck(int nCheck);//面伤合并 chenyuzhu add on 2014-6-19
	
	void OnPrintPreview();              //打印预览
	void OnDrawPrintPreviewDialog(QPrinter* printer); //打印预览画图
	void OnDrawSelectedFile(QList<QString> list);  //响应TableView 双击事件
	void OnCscanDrawDashRect(int id);  //响应信号通知Cscan画选中虚框
	void OnPlateScanStart();        //探伤开始时初始化标准等工作

	void OnIfPrint();//chenyuzhu add on 2014.5.22
	void OnDestroy();//chenyuzhu add on 2014.6.9

	void DrawTanTouLine(int x, bool bdraw);//chenyuzhu add on 2014.6.23
	float GetX(int nLuoJiNum);////chenyuzhu add on 2014.6.23
	void OnCidentLog(void);	//chenyuzhu add on 2014.6.26
	void OnToolsToUpdate(void);//chenyuzhu add on 2014-7-24
	void DrawTop(); //chenyuzhu add on 2014-8-1
	void DrawBottom(); //chenyuzhu add on 2014-8-1
	void DrawLeft(); //chenyuzhu add on 2014-8-1
	void DrawRight(); //chenyuzhu add on 2014-8-1

    void OnDrawFrontBoostSection(double minX,double maxX,double minY,double maxY);
    void OnDrawSideBoostSection(double minX,double maxX,double minY,double maxY); //响应画侧视图放大部分

    void OnRePanShang();

	

public:
	void CalcResultData();  // 计算一些结果，用于存数据库
    QJsonObject GetObjForBiaozhun();
    void SetValueToImage(UTImage &image, float datavalue, QPoint point, bool direction, QSize target);
	void OnTimerSelfTest(); //定时器TIMER_SELF_TEST的响应函数

	//chenyuzhu add start on 2014-5-12
	void m_DrawVRuler(QPainter& painter,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);
	void m_DrawHRuler(QPainter& painter,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);

	//chenyuzhu add end on 2014-5-12
	BOOL DeleteZoomInFlawInFile(const double& dMinX, const double& dMaxX,
		const double& dMinY, const double& dMaxY);//chenyuzhu add on 2014-6-9
	QColor CalcFillColor(const int& nvalue);//chenyuzhu add on 2014-6-13
	
	void PrintLogResult();	//chenyuzhu add on 2014.6.26
	void WriteLog();	//chenyuzhu add on 2014.6.26
	void ReadDatatoList();//chenyuzhu add on 2014-7-17
	void JiSuanBiaoZhun();//chenyuzhu add on 2014-7-25
    DetailInfo GetDetailInfo(const QString& strBiaoZhun);

private:
	//布局
	QGridLayout *m_pMainlayout;
	QVBoxLayout *m_pOtherSystemlayout;
	QWidget *m_pWdtCscanMain;//dock上必须放一个WIGDGET才能布局	

	QSqlDatabase *m_pDDatabaseConnection;//数据库连接变量
	//探测画图时用到的一些变量
	//伤点列表
	QList<CH_FLAWDATA> m_FlawList;// 局部钢板区域伤点列表
	QList<CH_FLAWDATA> m_TouTanFlawList;		// 头探区域伤点列表
	QList<CH_FLAWDATA> m_WeiTanFlawList;		// 尾探区域伤点列表
	int m_nInvalidEnd;
	QVector<CH_FLAWDATA> m_FlawStore;
	QVector<Flaw_Coordinate_Info> m_FlawCoordinateVector;   // 伤点坐标数组
	QMap<QString,QString> m_Map;  	// 表项和值的映射
	bool m_bCentreCrosingLine;     //中心十字线 
	bool m_bInjPointRangeLine;    //伤点范围线
	
public:
	//C扫面板
	QWidget *m_pWgtOtherSystem;//其他系统状态放在一个单独的QWidget上的
	QWidget *m_pWgtDetectResult;	//检测结果的BUTTON是放在一个单独的QWidget上的
	CScanThicknessWidget *m_pWgtThicknessView;//厚度图
	CScanFrontWidget *m_pWgtFrontView;//正视图
	CScanSideWidget *m_pWgtSideView;//侧视图
	CScanWholeWidget *m_pWgtWholeView;//整体视图
	CScanDetectWidget *m_pWgtCscanDetectView;//伤点图

	CScanLeftWidget *m_pWgtLeftView;//左侧边缘伤点图
	CScanRightWidget *m_pWgtRightView;//右侧边缘伤点图
	CScanTopWidget *m_pWgtTopView;//上侧边缘伤点图
	CScanBottomWidget *m_pWgtBottomView;//下侧边缘伤点图

	//其他系统状态
	QLabel *m_PLblPLCStatus;//PLC
	QLabel *m_pLblUSTStatus;//UST
	QLabel *m_pLblServerStatus;//数据库

	//检测结果
	QPushButton *m_pPstBtnDetectResult;//调用及显示探伤结果的BUTTON
	QComboBox *m_pCbDetectResult;//双击后可显示的更改探伤结果的QComboBox
	// 伤点总数（可能会与m_nInvalidEnd重复）
	UINT m_nPointCount;
	// 面伤总数
	UINT m_nSurfaceFlawCount;
	//弹出的对话框
	CDockForm *m_pDockPLC;//PLC悬浮窗体
	CDockForm *m_pDockUST;//UST悬浮窗体
	CDockForm *m_pDockServer;//SERVER悬浮窗体
	
	CDockForm *m_pDetectResult;//结果悬浮窗体//chenyuzhu add on 2014-7-28

	CInspectUser *m_pInspectUser;//用户修改探伤结果时弹出的用户验证窗体

	int m_nPlateYStart;   //局部钢板区域中长度的起始
	int m_nPlateYEnd;     //局部钢板区域中长度的终点
	QRect m_RegionRect;   //全局钢板选中区域
	int m_nThickMaxValue;	// 当前厚度偏差区域最大值
	zero_pos m_ZeroPosFlag; //零点标示

	QMenu* m_mPopUpMenu;
	QAction *m_aSaveAndPrint;
	
	QAction *m_aDestroy;//chenyuzhu add on 2014-6-9
	// 面伤总面积
	double m_nSurfaceAreaSum;
	// 当前钢板id
	int m_nPlateId;
	// 面伤信息列表
	QList<SurfaceFlawInfo> m_surfaceFlawInfoList;
	CCustumPlateTablePreview m_PreviewWnd;
	QVector<DLPointInfo> m_vHeaderRect; //DelectListDlg 列的相关信息
	int  m_nDoubleClickedDoshRectID;   //当前m_vHeaderRect 选中的ID
	//PLC UST SERVER定时器ID
	int m_nTimerSTID;
	// 标注对象指针
	CBiaoZhun* m_pBiaoZhun;
	//和标准有关的变量
	QString m_strResult;
	
	bool m_IfInitBiaoZhun;//判断标准初始化是否完成chenyuzhu add on 2014-5-8
	bool m_IfPrint;//判断是否按下的是右键弹出的保存并打印按键 chenyuzhu add on 2014-5-22
	//chenyuzhu add start on 2014-6-9
	double m_nLogicXStart;
	double m_nLogicXEnd;
	double m_nLogicYStart;
	double m_nLogicYEnd;
	//chenyuzhu add end on 2014-6-9
	QVector<DLPointInfo> m_vHRect;//chenyuzhu add on 2014-6-19
	QVector<DLPointInfo> m_vHBiaoZhunRect;//chenyuzhu add on 2014-7-25
	int * m_pDefectDistaneSmaller;//chenyuzhu add on 2014-6-19 
	bool m_bPointJoin;          //面伤合并 chenyuzhu add on 2014-6-19
	float m_fTanTouY;  //chenyuzhu add on 2014-6-23
	bool m_bTanTouDraw;//chenyuzhu add on 2014-6-23
	QList<QString> m_PlateInfo; //chenyuzhu add on 2014-7-17

	QSettings *m_pSettings;                //配置文件对象//chenyuzhu add on 2014-7-24
	QString m_IniFlieName;                 //配置文件名//chenyuzhu add on 2014-7-24
	double m_dMaxArea;//chenyuzhu add on 2014-7-28
	double m_dMaxLength;//chenyuzhu add on 2014-7-28

	bool m_bBiSeBiao; //chenyuzhu add on 2014-11-4

	bool *m_pbDependOnMInput;
	bool *m_pbPrintInjPointDetailPic;
};

//左侧边缘伤点图
class CScanLeftWidget : public QWidget
{
	Q_OBJECT
public:
	CScanLeftWidget(QWidget *parent = NULL);
	~CScanLeftWidget(void);

private:
	QSize sizeHint() const  
	{  
		return QSize(36, 473); /* 初始大小 */  
	} 

public:
	void paintEvent(QPaintEvent *e);
	//chenyuzhu add start on 2014-8-3
public:
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	QImage *m_pImage;
	bool    m_bBoostFlag;         //放大标示
	int m_nXStart;
	int m_nXEnd;
	int m_nYStart;
	int m_nYEnd;

signals:
	void signalDrawLeftDetial(); //通知Panel更新内存中的图像

public slots:
	void OnSignalDrawCScanLeftSection(double,double,double,double);
		//chenyuzhu add end on 2014-8-3
	
};

//右侧边缘伤点图
class CScanRightWidget : public QWidget
{
	Q_OBJECT
public:
	CScanRightWidget(QWidget *parent = NULL);
	~CScanRightWidget(void);
private:
	QSize sizeHint() const  
	{  
		return QSize(36, 473); /* 初始大小 */  
	} 

public:
	void paintEvent(QPaintEvent *e);
	//chenyuzhu add start on 2014-8-3
public:
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	QImage *m_pImage;
	bool    m_bBoostFlag;         //放大标示
	int m_nXStart;
	int m_nXEnd;
	int m_nYStart;
	int m_nYEnd;

signals:
	void signalDrawRightDetial(); //通知Panel更新内存中的图像

public slots:
	void OnSignalDrawCScanRightSection(double,double,double,double);
	//chenyuzhu add end on 2014-8-3

};

//上侧边缘伤点图
class CScanTopWidget : public QWidget
{
	Q_OBJECT
public:
	CScanTopWidget(QWidget *parent = NULL);
	~CScanTopWidget(void);
private:
	QSize sizeHint() const  
	{  
		return QSize(1331, 36); /* 初始大小 */  
	} 

public:
	void paintEvent(QPaintEvent *e);
	//chenyuzhu add start on 2014-8-3
public:
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	QImage *m_pImage;
	bool    m_bBoostFlag;         //放大标示
	int m_nXStart;
	int m_nXEnd;

signals:
	void signalDrawTopDetial(); //通知Panel更新内存中的图像
	
public slots:
	void OnSignalDrawCScanTopSection(double,double,double,double);
	//chenyuzhu add end on 2014-8-3
};

//下侧边缘伤点图
class CScanBottomWidget : public QWidget
{
	Q_OBJECT
public:
	CScanBottomWidget(QWidget *parent = NULL);
	~CScanBottomWidget(void);
private:
	QSize sizeHint() const  
	{  
		return QSize(1331, 36); /* 初始大小 */  
	} 

public:
	void paintEvent(QPaintEvent *e);
	//chenyuzhu add start on 2014-8-3
public:
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	QImage *m_pImage;
	bool    m_bBoostFlag;         //放大标示
	int m_nXStart;
	int m_nXEnd;

signals:
	void signalDrawBottomDetial(); //通知Panel更新内存中的图像

public slots:
	void OnSignalDrawCScanBottomSection(double,double,double,double);
	//chenyuzhu add end on 2014-8-3
};

//厚度图
class CScanThicknessWidget : public QWidget
{
	Q_OBJECT
public:
	CScanThicknessWidget(QWidget *parent = NULL);
	~CScanThicknessWidget(void);
private:
	QSize sizeHint() const  
	{  
	   return QSize(1331, 126); /* 初始大小 */  
	} 

public:
	void paintEvent(QPaintEvent *e);
signals:
	void signalDrawThicknessDetial();
public:
	QImage *m_pThicknessImage;	
	double	m_minX;
	double  m_maxX;
	double  m_minY;
	double  m_maxY;

public slots:
	void OnSignalDrawCScanThicknessSection(double minX,double maxX,double minY,double maxY);
};
//正视图
class CScanFrontWidget : public QWidget
{
	Q_OBJECT
public:
	CScanFrontWidget(QWidget *parent = NULL);
	~CScanFrontWidget(void);

public:
	void paintEvent(QPaintEvent *e);
	QColor CalcFillColor(const int& nvalue);//chenyuzhu add on 2014-6-13
signals:
	void signalDrawFrontDetial();
	void signalDrawXShadowRegion();
public slots:

public:
	int *m_nPlateYStart;   //局部钢板区域中长度的起始
	int *m_nPlateYEnd;     //局部钢板区域中长度的终点
	QImage *m_pFrontImage;
	QImage *m_pBoostImage;
	bool    m_bBoostFlag;      //放大标示
	QRect m_CurrentSectionRect;
	QRect m_BoostImageRect;
};

//侧视图
class CScanSideWidget : public QWidget
{
	Q_OBJECT
public:
	CScanSideWidget(QWidget *parent = NULL);
	~CScanSideWidget(void);

public:
	void paintEvent(QPaintEvent *e);
	QColor CalcFillColor(const int& nvalue);//chenyuzhu add on 2014-6-13
signals:
	void signalDrawSideDetial();
	void signalDrawYShadowRegion();
public:
	int m_nXSize; //内存位图的宽
	int m_nYSize; //内存位图的高
	QImage *m_pSideImage;
	QImage *m_pBoostImage;
	bool    m_bBoostFlag;      //放大标示
	QRect m_CurrentSectionRect;
	int *m_nPlateYStart;   //局部钢板区域中长度的起始
	int *m_nPlateYEnd;     //局部钢板区域中长度的终点
	QRect m_BoostImageRect;
};

//整体视图
class CScanWholeWidget : public QWidget
{
	Q_OBJECT
public:
	CScanWholeWidget(QWidget *parent = NULL);
	~CScanWholeWidget(void);

public:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *event);
	
signals:
	void signalDrawWholeDetial(); //通知Panel更新内存中的图像
	void signalChangeSelectRect(int x,int y,int width, int height);
	void signalDrawWholeRunning();//chenyuzhu add start on 2014-10-29
public slots:
    void OnSignalDrawWholeDashSection(double minX,double maxX,double minY,double maxY); //响应放大时候画虚框
public:
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	QImage *m_pWholeImage;
	QRect* m_pRegionRect;         //对应全局钢板选中区域指针
	bool    m_bBoostFlag;         //放大标示
	QImage *m_pBoostDashImage;    //放大时的虚框视图
};

//伤点图
class CScanDetectWidget : public QWidget
{
	Q_OBJECT
public:
	CScanDetectWidget(QWidget *parent = NULL);
	~CScanDetectWidget(void);

public:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void paintBoostSection(void);
	void CalculateLogicRange(double& minX,double& maxX,double& minY,double& maxY);
protected:
 	void mouseDoubleClickEvent(QMouseEvent *event);//双击伤点图边缘显示边缘视图
	void mouseMoveEvent(QMouseEvent *event);       
	void mouseReleaseEvent(QMouseEvent *e);        //鼠标弹起
	void mousePressEvent(QMouseEvent * e); 
	void wheelEvent(QWheelEvent *event);           //滚轮消息
signals:
	void mouseDoubleClicked(QMouseEvent *);//自定义双击信号
	void signalDrawDetectDetial(); //通知Panel更新内存中的图像
	void signalDrawPlateFlawSection();//探伤过程中，画大概伤点位置的点
	void signalToChannelPannel(QString text);  //显示悬浮伤点信息
	void signalDrawFrontBoostSection(double minX,double maxX,double minY,double maxY); //通知正视图画放大区域
	void signalDrawSideBoostSection(double minX,double maxX,double minY,double maxY); //通知侧视图画放大区域
	void signalDrawWholeDashSection(double minX,double maxX,double minY,double maxY); //通知整体图画画虚框
	void signalDrawThicknessSection(double minX,double maxX,double minY,double maxY); //通知厚度图//chenyuzhu add on 2014-7-23
	void signalDrawTopSection(double minX,double maxX,double minY,double maxY); //通知上边图//chenyuzhu add on 2014-8-3
	void signalDrawBottomSection(double minX,double maxX,double minY,double maxY); //通知上边图//chenyuzhu add on 2014-8-3
	void signalDrawRightSection(double minX,double maxX,double minY,double maxY); //通知上边图//chenyuzhu add on 2014-8-3
	void signalDrawLeftSection(double minX,double maxX,double minY,double maxY); //通知上边图//chenyuzhu add on 2014-8-3

public:
	int* m_pPlateYStart;   //局部钢板区域中长度的起始
	int* m_pPlateYEnd;     //局部钢板区域中长度的终点
	int m_nXSize;                 //QImage的宽
	int m_nYSize;				  //QImage的高
	int m_nXStart;
	int m_nXEnd;
	QImage *m_pImage;
	QRect  m_CurrentWholeSection; //再内存位图中选中的区域
	QImage *m_pSectionImage;      //保存选中的Image
	bool   m_bSectionImage;       //标记是否已经保存选中图片
	QPoint m_nBeginPoint;        //选中矩形开始点
	QPoint m_nEndPoint;			 //选中矩形结束点
	int    m_nSectionFlag;       //放大标记
	bool   m_bDrawingSectRect;   //画选中框标记
	int    m_nWheelFlag;         //滚轮标记 1为放大 2为缩小
	QVector<DLPointInfo>*  m_pvHeaderRect;  //指向CSANPANNEL中的DelectListDlg 列的相关信息
	QVector<DLPointInfo>*  m_pvHRect;//chenyuzhu add on2014-6-19 
	bool* m_pbPointJoin;//chenyuzhu add on2014-6-19 

	//chenyuzhu add start on 2014-6-10
	double m_nLogicXStart;
	double m_nLogicXEnd;
	double m_nLogicYStart;
	double m_nLogicYEnd;
	//chenyuzhu add end on 2014-6-10
	bool m_bBiSeBiao; //chenyuzhu add on 2014-11-4

};

#endif // CSCANPANEL_H
