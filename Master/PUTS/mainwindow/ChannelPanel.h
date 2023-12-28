/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 通道切换面板
* Filename: ChannelPanel.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-04-27
*/

#ifndef DOCKWIDGETCHANNELCTR_H
#define DOCKWIDGETCHANNELCTR_H

#include <QWidget>
#include <QDockWidget>
#include <QVector>
#include <QSqlDatabase>
#include <QString>
#include <QMap>
#include <QPixmap>
#include "GlobalFunction.h"
#include "PlateInfoPanel.h"
#include "DataDefine.h"
#include "DockForm.h"

class CWgtChannelCtrl : public QWidget
{
	Q_OBJECT
public:
	CWgtChannelCtrl(QSqlDatabase *pDatabaseConnection=NULL,CPlateInfo *pPlateInfo =NULL,QWidget *parent = NULL);
	~CWgtChannelCtrl(void);
public:
	void GetInfoFromDB(void);         //初始化通道个数信息
	void paintEvent(QPaintEvent *e);
	void DrawingBackground(void);    //画背景
	void ActiveChannelChange();		//切换通道
protected:
	void mousePressEvent(QMouseEvent * e);  //用于捕捉鼠标左键按下消息
	void mouseReleaseEvent(QMouseEvent * e);  //用于捕捉鼠标左键弹起消息
	bool eventFilter(QObject *obj, QEvent *event); //重写事件过滤器，用于显示浮动窗口
	void resizeEvent(QResizeEvent * e);      //重载窗口大小改变窗口
	void timerEvent( QTimerEvent *event );   //定时器事件
private:
	void InitMaps(void);  //初始化map
public:
	QSqlDatabase *m_pDatabaseConnection; //数据库连接标示  
	CPlateInfo *m_pPlateInfo;           //钢板参数信息


	// 头尾探数
	int m_nTouWeiTanNumber;				
	int m_nTouWeiTanWuLiBegin;			// 物理起始号
	int m_nTouWeiTanLogicBegin;			// 逻辑起始号

	int m_nBianTan1ChanelNumber;        //边探1通道数
	int m_nBianTan2ChanelNumber;		//边探2通道数
	int m_nNeiTan1ChanelNumber;			//内探1通道数
	int m_nNeiTan2ChanelNumber;			//内探2通道数

	//边探
	int m_nBianTan1WuliBegin;           //边探1物理编号开始
	int m_nBianTan2WuliBegin;			//边探2物理编号开始
	int m_nBianTan1LogicBegin;			// 逻辑起始号
	int m_nBianTan2LogicBegin;			// 逻辑起始号

	//内探
	int m_nNeiTan1WuliBegin;			//内探1物理编号开始
	int m_nNeiTan2WuliBegin;			//内探2物理编号开始
	int m_nNeiTan3WuLiBegin;			//内探3物理起始号
	int m_nNeiTan4WuLiBegin;			//内探4物理起始号
	int m_nNeiTan1LogicBegin;			// 逻辑起始号
	int m_nNeiTan2LogicBegin;			// 逻辑起始号
	int m_nNeiTan3LogicBegin;			// 逻辑起始号
	int m_nNeiTan4LogicBegin;			// 逻辑起始号

	int m_nVirtualBianTan1Number;
	int m_nVirtualBianTan2Number;
	int m_nVirtualNeitan1Number;
	int m_nVirtualNeitan2Number;
	int m_nVirtualNeitan3Number;
	int m_nVirtualNeitan4Number;

	QVector<QRect> m_EdgeDetectionVector;   //存贮边探矩形的容器
	QMap<int,int>  m_EdgeVectorToWLNum;      //从m_EdgeDetectionVector的序号映射到物理通道号
	QVector<QRect> m_InsideDetectionVector; //存储内探矩形的容器
	QMap<int,int>  m_InsideVectorToWLNum;      //从m_InsideDetectionVector的序号映射到物理通道号
	CDockForm *m_pDockShowDetial;          //悬浮窗体
	QPixmap *m_pBitmap;                     //保存当前窗口位图
	int      m_nCurrentRect;               //用于标示当前选中的矩形在Vector中的序号
	bool     m_bIndexFlag;                 //用于标记是边探还是内探0边探，1内探
	int  m_nTimeId;                        //定时器ID
	int  m_nTimeCount;                     //接受次数
	int m_nWuLiTongDaoHao;					//当前选中通道的物理通道号
	int m_nLuoJiTongDaoHao;					//当前选中逻辑通道号

signals:
	void SignalToDrawTanTouLine(int x, bool bdraw);

};

class CChannelPanel :public QDockWidget
{
	Q_OBJECT
public:
	CChannelPanel(QWidget *parent,CPlateInfo *pPlateInfo =NULL,QSqlDatabase *pDatabaseConnection=NULL);
	virtual ~CChannelPanel(void);
public:
	CWgtChannelCtrl *m_pmyWidget;			//画图Widget
	QSqlDatabase *m_pDatabaseConnection;	//数据库连接标示
	CPlateInfo *m_pPlateInfo;				//钢板参数
};

#endif //DOCKWIDGETCHANNELCTR_H