/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 通道切换面板
* Filename: ChannelPanel.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-04-27
*/


#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include <QDockWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include "ChannelPanel.h"
#include "DataDefine.h"
#include "GlobalFunction.h"

extern CH_CONFIG allCH_CONFIG[];
extern DWORD g_dwActiveChannel;
extern CIniParams g_IniParams;						//为了使用其中MainWindow对象，用于访问A扫指针
extern QMap<int,QVector<ST_CHANEL> > m_mapChanel;	//KEY值为探头盒号
extern CH_CONFIG_GLOBAL ch_config_global;
extern BOOL g_bStart;  //探伤是否结束
extern BOOL g_bStop;   //探伤是否结束
extern OtherParameters g_OtherParameters;
extern bool *g_bInjPointPath;//chenyuzhu add on 2014-11-4

CChannelPanel::CChannelPanel(QWidget *parent,CPlateInfo *pPlateInfo,QSqlDatabase *pDatabaseConnection)
										: QDockWidget(parent),m_pDatabaseConnection(pDatabaseConnection),m_pPlateInfo(pPlateInfo)
{
	setWindowTitle(tr("通道控制"));
	setMinimumSize(400,73);
	setMaximumSize(QSize(1920, 73));
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73);"));
	setFeatures(QDockWidget::DockWidgetFloatable);
	setAllowedAreas(Qt::AllDockWidgetAreas);
	setEnabled(true);
	m_pmyWidget = new CWgtChannelCtrl(m_pDatabaseConnection,m_pPlateInfo,this);
	setWidget(m_pmyWidget);
}

CChannelPanel::~CChannelPanel(void)
{
	if (NULL != m_pmyWidget)
	{
		delete m_pmyWidget;
		m_pmyWidget = NULL;
	}
}


CWgtChannelCtrl::CWgtChannelCtrl(QSqlDatabase *pDatabaseConnection,CPlateInfo *pPlateInfo,QWidget *parent)
										    :QWidget(parent),m_pDatabaseConnection(pDatabaseConnection),m_pPlateInfo(pPlateInfo)
{
	m_pBitmap = NULL;
	m_nCurrentRect = -1;
	m_bIndexFlag = false;
	m_nTimeId = -1;                        //定时器ID
	m_nTimeCount = 0;                     //接受次数
	m_pDockShowDetial = new CDockForm;
	m_pDockShowDetial->setWindowTitle(tr("通道状态"));
	setWindowFlags(Qt::FramelessWindowHint);
	setMouseTracking(true);
	installEventFilter(this); //安装事件过滤器
	GetInfoFromDB();   //查询数据库，初始化通道变量
	INIT_CONFIG(allCH_CONFIG, &ch_config_global);  //初始化通道信息
}

CWgtChannelCtrl::~CWgtChannelCtrl(void)
{
	if(NULL != m_pDockShowDetial)
	{
		delete m_pDockShowDetial;
		m_pDockShowDetial = NULL;
	}	
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

//chenyuzhu modify start on 2014-7-10
void CWgtChannelCtrl::GetInfoFromDB(void)
{
	//chenyuzhu add start on 2014-7-10

	//从数据库中读取文件/SheBeifile.dbfl

	QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";

	QFile file1(strFileName);
	bool bRet1 = file1.open(QIODevice::ReadOnly);

	if (!bRet1)
	{
		QMessageBox::warning(NULL,"InitConfig",QObject::tr("SheBeifile.dbfl打开失败"));
		return ;
	}
	file1.seek(0);
	SHEBEICANSHU SheBeiCanShu;
	quint64 nBytesRead = file1.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	file1.close();
	//chenyuzhu add start on 2014-7-10

	m_nBianTan1ChanelNumber = SheBeiCanShu.YiQiParam.nBianTan1TongDaoShu;        //边探1通道数
	m_nBianTan2ChanelNumber = SheBeiCanShu.YiQiParam.nBianTan2TongDaoShu;		//边探2通道数
	m_nNeiTan1ChanelNumber = SheBeiCanShu.YiQiParam.nNeiTan1TongDaoShu;		//内探1通道数
	m_nNeiTan2ChanelNumber = SheBeiCanShu.YiQiParam.nNeiTan2TongDaoShu;			//内探2通道数

	m_nBianTan1WuliBegin = SheBeiCanShu.YiQiParam.nBianTan1WuLiBegin;			//边探1物理开始通道号
	m_nBianTan2WuliBegin = SheBeiCanShu.YiQiParam.nBianTan2WuLiBegin;          //边探2物理开始通道号
	m_nNeiTan1WuliBegin = SheBeiCanShu.YiQiParam.nNeiTan1WuLiBegin;          //内探1物理开始通道号
	m_nNeiTan2WuliBegin = SheBeiCanShu.YiQiParam.nNeiTan2WuLiBegin;;          //内探2物理开始通道号

	m_nBianTan1LogicBegin = SheBeiCanShu.YiQiParam.nBianTan1LogicBegin;
	m_nBianTan2LogicBegin = SheBeiCanShu.YiQiParam.nBianTan2LogicBegin;
	m_nNeiTan1LogicBegin = SheBeiCanShu.YiQiParam.nNeiTan1LogicBegin;		// 逻辑起始号
	m_nNeiTan2LogicBegin = SheBeiCanShu.YiQiParam.nNeiTan2LogicBegin;
	
	m_nNeiTan1ChanelNumber = g_OtherParameters.insideCar1BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	//内探1通道数
	m_nNeiTan2ChanelNumber = g_OtherParameters.insideCar2BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	//内探2通道数
	m_nNeiTan2WuliBegin = g_OtherParameters.insideCar2PhysicalChannelStart;											//内探2物理开始通道号
	
	/*QSqlQuery sqlQuery(*m_pDatabaseConnection);
	sqlQuery.exec("select * from t_YiQi");
	while (sqlQuery.next())
	{
		m_nBianTan1ChanelNumber = sqlQuery.value(5).toInt();        //边探1通道数
		m_nBianTan2ChanelNumber = sqlQuery.value(21).toInt();		//边探2通道数
	    m_nNeiTan1ChanelNumber = sqlQuery.value(7).toInt();		//内探1通道数
		m_nNeiTan2ChanelNumber = sqlQuery.value(9).toInt();			//内探2通道数

		m_nBianTan1WuliBegin = sqlQuery.value(20).toInt();			//边探1物理开始通道号
		m_nBianTan2WuliBegin = sqlQuery.value(23).toInt();          //边探2物理开始通道号
		m_nNeiTan1WuliBegin = sqlQuery.value(24).toInt();          //内探1物理开始通道号
		m_nNeiTan2WuliBegin = sqlQuery.value(25).toInt();          //内探2物理开始通道号
	}*/
	memset(allCH_CONFIG,0,sizeof(CH_CONFIG)*500);
}
//chenyuzhu modify end on 2014-7-10


void CWgtChannelCtrl::InitMaps(void)   //初始化map
{
	if(m_InsideDetectionVector.isEmpty())
	{
		QMessageBox::information(this,"CWgtChannelCtrl::InitMaps",tr("m_InsideDetectionVector为空"));
	}
	int nSize = m_InsideDetectionVector.size();
	//初始化内探1映射
	m_InsideVectorToWLNum.clear();
	for (int i=0;i<m_nNeiTan1ChanelNumber && i<nSize;i++)
	{
		m_InsideVectorToWLNum[i] = m_nNeiTan1WuliBegin +i;
	}
	//初始化内探2映射
	for (int i=0;i<m_nNeiTan2ChanelNumber && i<nSize;i++)
	{
		m_InsideVectorToWLNum[m_nNeiTan1ChanelNumber + i] = m_nNeiTan2WuliBegin +i;
	}
	//初始化边探1
	nSize = m_EdgeDetectionVector.size();
	m_EdgeVectorToWLNum.clear();
	for (int i=0;i<m_nBianTan1ChanelNumber && i<nSize;i++)
	{
		m_EdgeVectorToWLNum[i] = m_nBianTan1WuliBegin +i;
	}
	//初始化边探2
	for (int i=0;i<m_nBianTan2ChanelNumber && i<nSize;i++)
	{
		m_EdgeVectorToWLNum[m_nBianTan1ChanelNumber + i] = m_nBianTan2WuliBegin +i;
	}
}

void CWgtChannelCtrl::DrawingBackground(void)
{
   //当界面变化时删除原有的矩形区域
	m_EdgeDetectionVector.clear();
	m_InsideDetectionVector.clear();
	
	QRect rect = this->rect();				//获得Widget1的RECT
	double nx = rect.left();                //获得坐标
	double ny = rect.top();
	double nClw = rect.right();
	double nClh = rect.bottom();
	double nSmallRW = 30;                        // 左右两边小矩形的宽
	double nBigRW = nClw - 2*nSmallRW-20-2;		 // 中间矩形的宽
	double nRH = nClh ;							 // 三个矩形的高
	//画图
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}  
	m_pBitmap = new QPixmap(this->rect().width(),this->rect().height());
	QPainter pPainter;
	pPainter.begin(m_pBitmap);
	pPainter.setPen(QColor(73,73,73,255));
	pPainter.setBrush(QColor(73,73,73,255));
	pPainter.drawRect(nx,ny,nClw,nClh);       // 画一遍背景和原来一样
	//三个矩形
	pPainter.setPen(QColor(0,0,0,255));     
	pPainter.drawRect(nx+1,ny,nSmallRW,nRH);
	pPainter.drawRect(nClw-nSmallRW,ny,nSmallRW-1,nRH);
	pPainter.setPen(QColor(73,73,73,255)); 
	pPainter.drawRect(nx+nSmallRW+10,ny,nBigRW,nRH);

	//根据需求画线
	int nNum = m_nBianTan1ChanelNumber + m_nBianTan2ChanelNumber + m_nNeiTan1ChanelNumber + m_nNeiTan2ChanelNumber; // 探头的总个数
	int nLNum = m_nBianTan1ChanelNumber + m_nBianTan2ChanelNumber;     //边探个数                           
	int nLRow = ceil((double (nLNum))/(double(2)));  // 边探的行数
 
	int nMNum = nNum - nLNum;						      // 内探的个数
	int nMRow = 3;                                        //内探行数
	int nCMRow = nMRow+1;                                 // 总行数
	int nMCol =  ceil((double (nMNum))/(double(nMRow)));  //内探的列数

	//画边探
	double nLSBX = nx + 1;                       //左边矩形起点的X
	double nLSEX = nSmallRW;                     //左边矩形终点的X
	double nLSY  = ny;                           //左边矩形起点的Y
	double nRSBX = nx + 20 + nSmallRW + nBigRW+2;  //右边矩形起点的X
	double nRSEX = nRSBX + nSmallRW-1;             //右边矩形终点的X   

   //初始化左边探的VECTOR
	for (int i = 0;i<(nLNum/2);i++)
	{
		QRect tempRect(QPoint(nLSBX+1,nLSY+nRH*i/(nLNum/2)+1),QPoint(nLSBX+nSmallRW-1,(nLSY+nRH*(i+1)/(nLNum/2))-1));
		m_EdgeDetectionVector.append(tempRect);
	}
	//初始化右边探的VECTOR
	for (int i = 0;i<(nLNum/2);i++)
	{
		QRect tempRect(QPoint(nRSBX+1,nLSY+nRH*i/(nLNum/2)+1),QPoint(nRSBX+nSmallRW-1,(nLSY+nRH*(i+1)/(nLNum/2))-1));
		m_EdgeDetectionVector.append(tempRect);
	}
   //画边探的格子
	pPainter.setPen(QColor(0,0,0,255));
	double nTemp = nRH/nLRow;
	for (int i=0; i<nLRow;i++)
	{
		pPainter.drawLine(nLSBX,nLSY,nLSEX,nLSY);
		pPainter.drawLine(nRSBX,nLSY,nRSEX,nLSY);
		nLSY += nTemp;
	}

	//画内探
	double nMBX =  nx+nSmallRW+10;
	double nMEX = nBigRW + nx+nSmallRW+10;
	double nMY  = ny;
	double temp = nRH/nCMRow;

	int nFlag = 0;                            //换列的标记
	double nXBegin = nMBX+1;      
	double nYBegin = nMY+temp+1;
	double nXChange = (nMEX-nMBX)/nMCol;
	double nYChange = temp;
	//初始化内探的Vector
	for (int i=0;i<nMNum;i++)
	{
		if (nMRow == nFlag)
		{
			nXBegin +=nXChange;
			nYBegin = nMY+temp+1;
			nFlag = 0;
		}
		m_InsideDetectionVector.append(QRect(QPoint(nXBegin,nYBegin),QPoint(nXBegin+nXChange-3,nYBegin+nYChange-2)));
		nYBegin += nYChange;
		nFlag++;
	}
//画行	
	for (int i = 0; i<=nCMRow;i++)
	{
		if(0 == i)
		{
			nMY += temp;
			continue;
		}
		pPainter.drawLine(nMBX,nMY-1,nMEX,nMY-1);
		nMY += temp;
	}	
	double nMBX2 =  nMBX ;
	double nMBY  = ny+temp+1;
	double nMEY = nMBY + nRH;
	temp = nBigRW/nMCol;
	pPainter.setPen(QPen (Qt::black, 2));
	for (int i = 0; i<nMCol;i++)
	{
		pPainter.drawLine(nMBX2,nMBY,nMBX2,nMEY);
		nMBX2 += temp;
	}
	pPainter.drawLine(nx+nSmallRW+10+nBigRW,nMBY,nx+nSmallRW+10+nBigRW,nMEY);
    //画探头的状态
	if (!m_pPlateInfo->m_strArtName.isEmpty())
	{
		pPainter.setPen(Qt::NoPen);
		//画边探
		if(g_bStart && !g_bStop)
		{	
			pPainter.setBrush(QColor(0,255,0,255));
		}
		else
		{
			pPainter.setBrush(QColor(96,96,96,255));
		}		
		for (int i = 0;i<nLNum;i++)
		{   //左边探
			if(i<nLNum/2)
			{
				if (allCH_CONFIG[m_nBianTan1WuliBegin+i].CH_EN != 0)
				{
					pPainter.drawRect(m_EdgeDetectionVector.at(i));
				}
			}
			else
			{
				if (allCH_CONFIG[m_nBianTan2WuliBegin+i-nLNum/2].CH_EN != 0)
				{
					pPainter.drawRect(m_EdgeDetectionVector.at(i));
				}
			}
		}
		//画内探
		for (int i = 0;i<nMNum;i++)
		{
			//内探1
			if (i < m_nNeiTan1ChanelNumber)
			{
				if (allCH_CONFIG[m_nNeiTan1WuliBegin+i].CH_EN != 0)
				{
					pPainter.drawRect(m_InsideDetectionVector.at(i));
				}
			}
			else //内探2
			{
				if (allCH_CONFIG[m_nNeiTan2WuliBegin+i - m_nNeiTan1ChanelNumber].CH_EN != 0)
				{
					pPainter.drawRect(m_InsideDetectionVector.at(i));
				}
			}
		}
		InitMaps();//初始化相关Map
	}

	pPainter.end();
}

void CWgtChannelCtrl::mouseReleaseEvent(QMouseEvent * e)  //用于捕捉鼠标左键弹起消息
{
	
}

void CWgtChannelCtrl::mousePressEvent(QMouseEvent * e)  //用于捕捉鼠标左键按下消息
{
	if (!m_pPlateInfo->m_strArtName.isEmpty() && g_bStart && !g_bStop)
	{
		if((e->button() == Qt::LeftButton)) 
		{
			QPoint tempPoint(e->pos().x(),e->pos().y());
			for (int i = 0; i< m_EdgeDetectionVector.size();i++)
			{
				if (m_EdgeDetectionVector[i].contains(tempPoint))
				{
					m_nCurrentRect = i;   
					m_bIndexFlag = false;
					break;
				}
			}
			if (-1 == m_nCurrentRect)
			{
				for (int i = 0; i< m_InsideDetectionVector.size();i++)
				{
					if (m_InsideDetectionVector[i].contains(tempPoint))
					{
						m_nCurrentRect = i;
						m_bIndexFlag = true;
						break;
					}
				}
			}
			//启动定时器
			m_nTimeId = startTimer(100);  //开启定时器用于让选中的通道号闪几下
			DrawingBackground(); 

			//chenyuzhu add start on 2014-11-3
			//切换通道
			allCH_CONFIG[m_nWuLiTongDaoHao].CH_NO = m_nWuLiTongDaoHao;
			allCH_CONFIG[m_nWuLiTongDaoHao].CH_REF_NO = m_nLuoJiTongDaoHao;
			g_dwActiveChannel = m_nWuLiTongDaoHao;
			//用于控制A扫切换通道的下拉框
			ActiveChannelChange();	
			//chenyuzhu add end on 2014-11-3
		}
	}
	//chenyuzhu add start on 2014-11-4
	else if(!m_pPlateInfo->m_strArtName.isEmpty() && !g_bStart && g_bStop)
	{
		if((e->button() == Qt::LeftButton)) 
		{
			bool bIfDraw;
		//	static int ndraw = 0;
		//	ndraw = ndraw%2;
			QPoint tempPoint(e->pos().x(),e->pos().y());
			for (int i = 0; i< m_EdgeDetectionVector.size();i++)
			{
				if (m_EdgeDetectionVector[i].contains(tempPoint))
				{
					m_nCurrentRect = i;   
					m_bIndexFlag = false;
					break;
				}
			}
			if (-1 == m_nCurrentRect)
			{
				for (int i = 0; i< m_InsideDetectionVector.size();i++)
				{
					if (m_InsideDetectionVector[i].contains(tempPoint))
					{
						m_nCurrentRect = i;
						m_bIndexFlag = true;
						break;
					}
				}
			}
			//启动定时器

			//	m_nTimeId = startTimer(100);  //开启定时器用于让选中的通道号闪几下
			DrawingBackground(); 

			//////////////////////////////////////////////////////////////////////////
			//画图

			if (-1 != m_nCurrentRect)
			{ 
				QPixmap tempPix(m_pBitmap->width(),m_pBitmap->height());
				QPainter pixPainter(&tempPix);
				pixPainter.drawPixmap(tempPix.rect(),*m_pBitmap,m_pBitmap->rect());
				//画矩形
				QRect tempRect;
				if (m_bIndexFlag) // 区分内探和边探的标示
				{
					tempRect = m_InsideDetectionVector[m_nCurrentRect];
				}else
				{
					tempRect = m_EdgeDetectionVector[m_nCurrentRect];
				}
				pixPainter.setPen(Qt::NoPen);

				//chenyuzhu add start on 2014-5-12
			//	if(ndraw == 0)
			//	{
					pixPainter.setBrush(QColor(255,255,0));
					bIfDraw = true;
			//	}
			//	if(ndraw == 1)
			//	{
			//		pixPainter.setBrush(QColor(0,255,0));
			//		bIfDraw = false;
			//	}
				//chenyuzhu add end on 2014-5-12

				pixPainter.drawRect(tempRect);
				pixPainter.end();
				QPainter painter(m_pBitmap);
				painter.drawPixmap(m_pBitmap->rect(),tempPix,tempPix.rect());
				painter.end();
				update();
				m_nCurrentRect = -1;            
				m_bIndexFlag = false; 
			}
			//chenyuzhu add start on 2014-6-23
			if(true == *g_bInjPointPath)//chenyuzhu add on 2014-11-4
			{
				emit SignalToDrawTanTouLine(m_nLuoJiTongDaoHao, bIfDraw);
			}
			//chenyuzhu add start on 2014-6-23
		}
	}
	//chenyuzhu add end on 2014-11-4


	//chenyuzhu add start on 2014-6-23
//	if (!m_pPlateInfo->m_strArtName.isEmpty())
//	{
/*		bool bIfDraw;
		static int ndraw = 0;
		ndraw = ndraw%2;
		if((e->button() == Qt::RightButton)) 
		{
			QPoint tempPoint(e->pos().x(),e->pos().y());
			for (int i = 0; i< m_EdgeDetectionVector.size();i++)
			{
				if (m_EdgeDetectionVector[i].contains(tempPoint))
				{
					m_nCurrentRect = i;   
					m_bIndexFlag = false;
					break;
				}
			}
			if (-1 == m_nCurrentRect)
			{
				for (int i = 0; i< m_InsideDetectionVector.size();i++)
				{
					if (m_InsideDetectionVector[i].contains(tempPoint))
					{
						m_nCurrentRect = i;
						m_bIndexFlag = true;
						break;
					}
				}
			}
			//启动定时器
	
			//	m_nTimeId = startTimer(100);  //开启定时器用于让选中的通道号闪几下
			DrawingBackground(); 
	
			//////////////////////////////////////////////////////////////////////////
			//画图

			if (-1 != m_nCurrentRect)
			{ 
				QPixmap tempPix(m_pBitmap->width(),m_pBitmap->height());
				QPainter pixPainter(&tempPix);
				pixPainter.drawPixmap(tempPix.rect(),*m_pBitmap,m_pBitmap->rect());
				//画矩形
				QRect tempRect;
				if (m_bIndexFlag) // 区分内探和边探的标示
				{
					tempRect = m_InsideDetectionVector[m_nCurrentRect];
				}else
				{
					tempRect = m_EdgeDetectionVector[m_nCurrentRect];
				}
				pixPainter.setPen(Qt::NoPen);

				//chenyuzhu add start on 2014-5-12
				if(ndraw == 0)
				{
					pixPainter.setBrush(QColor(255,255,0));
					bIfDraw = true;
				}
				if(ndraw == 1)
				{
					pixPainter.setBrush(QColor(0,255,0));
					bIfDraw = false;
				}
				//chenyuzhu add end on 2014-5-12

				pixPainter.drawRect(tempRect);
				pixPainter.end();
				QPainter painter(m_pBitmap);
				painter.drawPixmap(m_pBitmap->rect(),tempPix,tempPix.rect());
				painter.end();
				update();
				m_nCurrentRect = -1;            
				m_bIndexFlag = false; 
			}
			//////////////////////////////////////////////////////////////////////////
			
			//chenyuzhu add start on 2014-6-23
			emit SignalToDrawTanTouLine(m_nLuoJiTongDaoHao, bIfDraw);
			//chenyuzhu add start on 2014-6-23

			//切换通道
			//			allCH_CONFIG[m_nWuLiTongDaoHao].CH_NO = m_nWuLiTongDaoHao;
			//			allCH_CONFIG[m_nWuLiTongDaoHao].CH_REF_NO = m_nLuoJiTongDaoHao;
			//			g_dwActiveChannel = m_nWuLiTongDaoHao;
			//用于控制A扫切换通道的下拉框
			//			ActiveChannelChange();			
		}
		ndraw++;*/
//	}
	//chenyuzhu add end on 2014-6-23
}
void CWgtChannelCtrl::ActiveChannelChange()
{
	//1.操作MAP：分几种情况
	//第一种：当前选择通道在MAP中【是选中项且是唯一项】则不用做特殊处理
	//第二种：当前选择通道在MAP中【是选中项但不是唯一项】则将当前操作通道改成当前切换的的通道并也选中
	//第三种：当前选择通道在MAP中【不是选中项但有选中项】则将当前操作通道改成当前切换的的通道并也选中
	//计算探头盒号
	UINT i = 0;//存储要操作的探头号
	UINT j =  GetCurrentProbeBoxNum();//探头盒号
	for ( i = 0;i < 3; ++i)
	{
		if (m_mapChanel[j].value(i).PhysicalNo == m_nWuLiTongDaoHao)
		{
			//无论MAP里选中没选中都将当前用户切换的通道选中
			QVector<ST_CHANEL> temp;
			temp = m_mapChanel[j];
			temp[i].isSelect = TRUE;
			m_mapChanel[j] = temp;
			break;
		}
	}
	switch(i)
	{
		case 0:
			{
				g_IniParams.m_pMainWindow->m_pAscan->m_qsProbe = tr("探头1");
				break;
			}
		case 1:
			{
				g_IniParams.m_pMainWindow->m_pAscan->m_qsProbe = tr("探头2");
				break;
			}
		case 2:
			{
				g_IniParams.m_pMainWindow->m_pAscan->m_qsProbe = tr("探头3");
				break;
			}
	}
	g_IniParams.m_pMainWindow->m_pAscan->m_nProbeBox = j;
	//切换A扫中探头所在的探头盒
	g_IniParams.m_pMainWindow->m_pAscan->comboBoxProbeBox->setCurrentIndex(j);
	//2.给A扫发送通道切换
	//g_IniParams.m_pMainWindow->m_pAscan->ProbeBoxChange();
	g_IniParams.m_pMainWindow->m_pAscan->ChannelNumChange();	//chenyuzhu add on 2014-11-3
}
void CWgtChannelCtrl::timerEvent( QTimerEvent *event )   //定时器事件
{
	//QMessageBox::information(this,"DrawPrintText",tr("111！！"));
	if (event->timerId() == m_nTimeId) 
	{
		if (m_nTimeCount < 5)
		{
			//画图
 			if (-1 != m_nCurrentRect)
			{ 
				if (0 == m_nTimeCount % 2)
				{
					QPixmap tempPix(m_pBitmap->width(),m_pBitmap->height());
			 		QPainter pixPainter(&tempPix);
			 		pixPainter.drawPixmap(tempPix.rect(),*m_pBitmap,m_pBitmap->rect());
			 		//画矩形
			 		QRect tempRect;
			 		if (m_bIndexFlag) // 区分内探和边探的标示
			 		{
			 			tempRect = m_InsideDetectionVector[m_nCurrentRect];
			 		}else
			 		{
						tempRect = m_EdgeDetectionVector[m_nCurrentRect];
		 			}
			 		pixPainter.setPen(Qt::NoPen);

					//chenyuzhu add start on 2014-5-12
			 		pixPainter.setBrush(QColor(0,255,0));
					//chenyuzhu add end on 2014-5-12
			 		
					pixPainter.drawRect(tempRect);
			 		pixPainter.end();
					QPainter painter(m_pBitmap);
					painter.drawPixmap(m_pBitmap->rect(),tempPix,tempPix.rect());
					painter.end();
					update();
				}
				else
				{
					QPixmap tempPix(m_pBitmap->width(),m_pBitmap->height());
					QPainter pixPainter(&tempPix);
					pixPainter.drawPixmap(tempPix.rect(),*m_pBitmap,m_pBitmap->rect());
					//画矩形
					QRect tempRect;
					if (m_bIndexFlag) // 区分内探和边探的标示
					{
						tempRect = m_InsideDetectionVector[m_nCurrentRect];
					}else
					{
						tempRect = m_EdgeDetectionVector[m_nCurrentRect];
					}
					pixPainter.setPen(Qt::NoPen);

					//chenyuzhu add start on 2014-5-12
					pixPainter.setBrush(QColor(138,43,226));
					//chenyuzhu add start on 2014-5-12
					
					pixPainter.drawRect(tempRect);
					pixPainter.end();
					QPainter painter(m_pBitmap);
					painter.drawPixmap(m_pBitmap->rect(),tempPix,tempPix.rect());
					painter.end();
					update();
				}
		    }
			m_nTimeCount ++;
		}
		else
		{
			m_nTimeCount =0;
			killTimer(m_nTimeId);
			m_nTimeId =-1;
	        m_nCurrentRect = -1;            
			m_bIndexFlag = false;               
		}
	}
//	QMessageBox::information(this,"DrawPrintText",tr("222！！"));
}

void CWgtChannelCtrl::resizeEvent(QResizeEvent * e)     //重载窗口大小改变窗口
{
	DrawingBackground();
}
void CWgtChannelCtrl::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.drawPixmap(this->rect(),*m_pBitmap,m_pBitmap->rect());
	painter.end();
}

bool CWgtChannelCtrl::eventFilter(QObject *obj, QEvent *event)
{	
	if (obj == this)
	{
		//if (!m_pPlateInfo->m_strArtName.isEmpty() && g_bStart && !g_bStop)
		if (!m_pPlateInfo->m_strArtName.isEmpty())//chenyuzhu modify on 2014-8-12
		{
			if (event->type() == QEvent::Enter)
			{	
				for (int i=0;i<m_EdgeDetectionVector.size();i++)
				{
					QPoint showPoint(this->rect().bottomRight());
					showPoint = mapToGlobal(showPoint);
					QPoint currentPoint(QCursor::pos().x(),QCursor::pos().y());
					currentPoint = mapFromGlobal(currentPoint);
					if (m_EdgeDetectionVector[i].contains(currentPoint))
					{
						if ((QCursor::pos().x()+ 200)>this->width())
						{
								m_pDockShowDetial->move(QCursor::pos().x()-300, QCursor::pos().y()+10);
						}else
						{
								m_pDockShowDetial->move(QCursor::pos().x(), QCursor::pos().y()+10);
						}
						m_pDockShowDetial->resize(200,200);
						m_pDockShowDetial->SetSize(200,200);//chenyuzhu add on 2014-10-30
						m_pDockShowDetial->setWindowTitle(tr("边探详细信息"));
						
						
						if (NULL != m_pDockShowDetial->m_pBitmap)
						{
 							delete m_pDockShowDetial->m_pBitmap;
 							m_pDockShowDetial->m_pBitmap = NULL;
						}
 						m_pDockShowDetial->m_pBitmap = new QPixmap(200,200);
						m_pDockShowDetial->m_pBitmap->fill(QColor(73,73,73));
						QPainter painter(m_pDockShowDetial->m_pBitmap);	
						painter.setPen(QColor(255,255,0));
						//chenyuzhu add start on 2014-10-30
						QFont font = QFont("Times", 15, QFont::Bold);
						painter.setFont(font);
						//chenyuzhu add start on 2014-10-30
						int nWuLiTongDaoHao = m_EdgeVectorToWLNum[i];
						int nLuoJiTongDaoHao = allCH_CONFIG[nWuLiTongDaoHao].CH_REF_NO;
						int height = painter.fontMetrics().height();
						painter.drawText(1,height,tr("物理通道号： %1").arg(QString::number(nWuLiTongDaoHao)));
						painter.drawText(1,2*height+4,tr("逻辑通道号： %1").arg(QString::number(nLuoJiTongDaoHao)));
						
						//chenyuzhu add start on 2014.5.8
						int nHehao;
						int nNeiTanNo;
						if ((nLuoJiTongDaoHao >= 1000)&&(nLuoJiTongDaoHao < 2000))
						{
							nNeiTanNo = 1;
						}
						else if ((nLuoJiTongDaoHao >= 2000))
						{

							nNeiTanNo = 2;
						}	
						painter.drawText(1,3*height+8,tr("边探： %1").arg(QString::number(nNeiTanNo)));
						//chenyuzhu add end on 2014.5.8

						painter.end();
						//chenyuzhu modify start on 2014-8-12
						if(g_bStart && !g_bStop)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();	
						}
						//chenyuzhu modify end on 2014-8-12
						//chenyuzhu add start on 2014-11-4
						else if(*g_bInjPointPath)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
						}
						//chenyuzhu add end on 2014-11-4
					}
				}
				for (int i=0;i<m_InsideDetectionVector.size();i++)
				{
					QPoint showPoint(this->rect().bottomRight());
					showPoint = mapToGlobal(showPoint);
					QPoint currentPoint(QCursor::pos().x(),QCursor::pos().y());
					currentPoint = mapFromGlobal(currentPoint);
					if (m_InsideDetectionVector[i].contains(currentPoint))
					{
						m_pDockShowDetial->setWindowTitle(tr("内探详细信息"));
						if ((QCursor::pos().x()+ 200)>this->width())
						{
							m_pDockShowDetial->move(QCursor::pos().x()-300, QCursor::pos().y()+10);
						}else
						{
							m_pDockShowDetial->move(QCursor::pos().x(), QCursor::pos().y()+10);
						}
						m_pDockShowDetial->resize(200,200);
						m_pDockShowDetial->SetSize(200,200);//chenyuzhu add on 20141030
 						if (NULL != m_pDockShowDetial->m_pBitmap)
 						{
 							delete m_pDockShowDetial->m_pBitmap;
 							m_pDockShowDetial->m_pBitmap = NULL;
 						}
						m_pDockShowDetial->m_pBitmap = new QPixmap(200,200);
						m_pDockShowDetial->m_pBitmap->fill(QColor(73,73,73));
						QPainter painter(m_pDockShowDetial->m_pBitmap);
						painter.setPen(QColor(255,255,0));
						//chenyuzhu add start on 2014-10-30
						QFont font = QFont("Times", 15, QFont::Bold);
						painter.setFont(font);
						//chenyuzhu add start on 2014-10-30
						int nWuLiTongDaoHao = m_InsideVectorToWLNum[i];
						int nLuoJiTongDaoHao = allCH_CONFIG[nWuLiTongDaoHao].CH_REF_NO;
						int height = painter.fontMetrics().height();
						painter.drawText(1,height,tr("物理通道号： %1").arg(QString::number(nWuLiTongDaoHao)));
						painter.drawText(1,2*height+4,tr("逻辑通道号： %1").arg(QString::number(nLuoJiTongDaoHao)));

						//chenyuzhu add start on 2014.5.8
						int nHehao;
						int nNeiTanNo;
						if ((nLuoJiTongDaoHao  >= 3000)&&(nLuoJiTongDaoHao < 4000))
						{
							nHehao = (nLuoJiTongDaoHao - 3000)/3 + 1;
							nNeiTanNo = 1;
						}
						else if ((nLuoJiTongDaoHao >= 4000))
						{
							nHehao = (nLuoJiTongDaoHao - 4000)/3 + 1;
							nNeiTanNo = 2;
						}	
						painter.drawText(1,3*height+8,tr("内探： %1").arg(QString::number(nNeiTanNo)));
						painter.drawText(1,4*height+12,tr("盒号： %1").arg(QString::number(nHehao)));
						//chenyuzhu add end on 2014.5.8

						painter.end();
						//chenyuzhu modify start on 2014-8-12
						if(g_bStart && !g_bStop)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();	
						}
						//chenyuzhu modify end on 2014-8-12
						//chenyuzhu add start on 2014-11-4
						else if(*g_bInjPointPath)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
						}
						//chenyuzhu add end on 2014-11-4
						
					}
				}	
			}	
			else if(event->type()==QEvent::MouseMove)
			{
				int flag = 0;
				for (int i=0;i<m_EdgeDetectionVector.size();i++)
				{
					QPoint currentPoint(QCursor::pos().x(),QCursor::pos().y());
					currentPoint = mapFromGlobal(currentPoint);
					QPoint showPoint(this->rect().bottomRight());
					showPoint = mapToGlobal(showPoint);
					if (m_EdgeDetectionVector[i].contains(currentPoint))
					{
						if ((QCursor::pos().x()+ 200)>this->width())
						{
							m_pDockShowDetial->move(QCursor::pos().x()-300, QCursor::pos().y()+10);
						}else
						{
							m_pDockShowDetial->move(QCursor::pos().x(), QCursor::pos().y()+10);
						}
						m_pDockShowDetial->setWindowTitle(tr("边探详细信息"));
						if (NULL != m_pDockShowDetial->m_pBitmap)
						{
							delete m_pDockShowDetial->m_pBitmap;
							m_pDockShowDetial->m_pBitmap = NULL;
						}
						m_pDockShowDetial->m_pBitmap = new QPixmap(200,200);
						m_pDockShowDetial->m_pBitmap->fill(QColor(73,73,73));
						QPainter painter(m_pDockShowDetial->m_pBitmap);
						painter.setPen(QColor(255,255,0));
						//chenyuzhu add start on 2014-10-30
						QFont font = QFont("Times", 15, QFont::Bold);
						painter.setFont(font);
						//chenyuzhu add start on 2014-10-30
						m_nWuLiTongDaoHao = m_EdgeVectorToWLNum[i];
						m_nLuoJiTongDaoHao = allCH_CONFIG[m_nWuLiTongDaoHao].CH_REF_NO;
						int height = painter.fontMetrics().height();
						painter.drawText(1,height,tr("物理通道号： %1").arg(QString::number(m_nWuLiTongDaoHao)));
						painter.drawText(1,2*height+4,tr("逻辑通道号： %1").arg(QString::number(m_nLuoJiTongDaoHao)));
						
						//chenyuzhu add start on 2014.5.8
						int nHehao;
						int nNeiTanNo;
						if ((m_nLuoJiTongDaoHao >= 1000)&&(m_nLuoJiTongDaoHao < 2000))
						{
							nNeiTanNo = 1;
						}
						else if ((m_nLuoJiTongDaoHao >= 2000))
						{

							nNeiTanNo = 2;
						}	
						painter.drawText(1,3*height+8,tr("边探： %1").arg(QString::number(nNeiTanNo)));
						//chenyuzhu add end on 2014.5.8

						painter.end();
						//chenyuzhu modify start on 2014-8-12
						if(g_bStart && !g_bStop)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
							flag = 1;
						}
						//chenyuzhu modify end on 2014-8-12
						//chenyuzhu add start on 2014-11-4
						else if(*g_bInjPointPath)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
							flag = 1;
						}
						//chenyuzhu add end on 2014-11-4
					}
				}
				for (int i=0;i<m_InsideDetectionVector.size();i++)
				{
					if ((QCursor::pos().x()+ 200)>this->width())
					{
						m_pDockShowDetial->move(QCursor::pos().x()-300, QCursor::pos().y()+10);
					}else
					{
						m_pDockShowDetial->move(QCursor::pos().x(), QCursor::pos().y()+10);
					}
					QPoint currentPoint(QCursor::pos().x(),QCursor::pos().y());
					currentPoint = mapFromGlobal(currentPoint);
					QPoint showPoint(this->rect().bottomRight());
					showPoint = mapToGlobal(showPoint);
					if (m_InsideDetectionVector[i].contains(currentPoint))
					{
						m_pDockShowDetial->setWindowTitle(tr("内探详细信息"));
						m_pDockShowDetial->resize(200,200);
						m_pDockShowDetial->SetSize(200,200);//chenyuzhu add on 20141030
						if (NULL != m_pDockShowDetial->m_pBitmap)
						{
							delete m_pDockShowDetial->m_pBitmap;
							m_pDockShowDetial->m_pBitmap = NULL;
						}
						m_pDockShowDetial->m_pBitmap = new QPixmap(200,200);
						m_pDockShowDetial->m_pBitmap->fill(QColor(73,73,73));
						QPainter painter(m_pDockShowDetial->m_pBitmap);
						painter.setPen(QColor(255,255,0));

						//chenyuzhu add start on 2014-10-30
						QFont font = QFont("Times", 15, QFont::Bold);
						painter.setFont(font);
						//chenyuzhu add start on 2014-10-30
						m_nWuLiTongDaoHao = m_InsideVectorToWLNum[i];
						m_nLuoJiTongDaoHao = allCH_CONFIG[m_nWuLiTongDaoHao].CH_REF_NO;
						int height = painter.fontMetrics().height();
						painter.drawText(1,height,tr("物理通道号： %1").arg(QString::number(m_nWuLiTongDaoHao)));
						painter.drawText(1,2*height+4,tr("逻辑通道号： %1").arg(QString::number(m_nLuoJiTongDaoHao)));

						//chenyuzhu add start on 2014.5.8
						int nHehao;
						int nNeiTanNo;
						if ((m_nLuoJiTongDaoHao  >= 3000)&&(m_nLuoJiTongDaoHao < 4000))
						{
							nHehao = (m_nLuoJiTongDaoHao - 3000)/3 + 1;
							nNeiTanNo = 1;
						}
						else if ((m_nLuoJiTongDaoHao >= 4000))
						{
							nHehao = (m_nLuoJiTongDaoHao - 4000)/3 + 1;
							nNeiTanNo = 2;
						}	
					    painter.drawText(1,3*height+8,tr("内探： %1").arg(QString::number(nNeiTanNo)));
						painter.drawText(1,4*height+12,tr("盒号： %1").arg(QString::number(nHehao)));
						//chenyuzhu add end on 2014.5.8
					
						painter.end();
						//chenyuzhu modify start on 2014-8-12
						if(g_bStart && !g_bStop)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
							flag = 1;
						}
						//chenyuzhu modify end on 2014-8-12
						//chenyuzhu add start on 2014-11-4
						else if(*g_bInjPointPath)
						{
							m_pDockShowDetial->update();
							m_pDockShowDetial->show();
							flag = 1;
						}
						//chenyuzhu add end on 2014-11-4
					}
				}
				if (0 == flag)
				{
					m_pDockShowDetial->hide();
				}
			}
			if (event->type() == QEvent::Leave) //离开
			{
				m_pDockShowDetial->hide();
			}
		}
	}
	return QWidget::eventFilter(obj,event);
}



