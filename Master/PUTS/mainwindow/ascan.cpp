#include "ascan.h"
#include "ui_ascan.h"
#include <QPainter>
#include <QTimer>
#include <QVector>
#include <QDesktopWidget>
#include <QObject>
#include <QMouseEvent>
#include <QSettings>
#include "GlobalFunction.h"

const int A_SCAN_MAX_RANGE = 1000;
const int GATE_MAX_RANGE = 1000;
const int GATE_PEAK_MAX_RANGE = 100;
extern QMap<int,QVector<ST_CHANEL> > m_mapChanel;//KEY值为探头盒号
extern DWORD g_dwActiveChannel;
extern DWORD g_dwActiveChannelLuoJi;//chenyuzhu add on 2014-6-20
extern DWORD g_dwActiveFlawOrBottom;
extern CH_CONFIG allCH_CONFIG[];
map<WORD,CPicData*> m_MapCPicData;
extern PLATE_INFO g_Info[1];
extern int nPlate;
extern BOOL g_bStart;
extern HANDLE g_hWaveReadyEvent;
extern AECWorkThread** g_pAECWorkThread; //指向线程对象数组的指针
extern int g_nSetOneChannel;
extern DWORD g_dwActiveFlawOrBottom;//0伤波 1底波
extern QSqlDatabase *g_sqlDB;
extern int g_nADCFreq;
extern OtherParameters g_OtherParameters;
int g_ADataAvgNum = 1;
float g_GainSale[500];
float MaxHeightValue = 500;

AScan::AScan(QWidget *parent, Qt::WindowFlags flags): QDialog(parent, flags)
{
	m_nWaveSpeed = WAVESPEED_STEEL;
	m_nLines = 10;
	m_fXMinRuler = 0;
	m_fXMaxRuler = 100.0f;
	setupUi(this);
	setMouseTracking(true);
	DataMemberInit();
	DataBaseAscanSyn();
	UiInit();                   // 调用探头盒函数,给g_dwActiveChannel,和Map赋值，并更新界面。
	DataMemberInit();
	setWindowModality(Qt::WindowModal);
	widget_3->setMouseTracking(true);
	widget->setMouseTracking(true);
	widget_4->setMouseTracking(true);
	verticalSliderGain->setMouseTracking(true);
	label_10->setMouseTracking(true);
	memset(m_nGrid,0,sizeof(m_nGrid));
	ProbeBoxChange();
	
	comboBoxGate->setEnabled(true); //chenyuzhu add start on 2014-11-2 闸门下拉列表
	comboBoxGate->clear();
	comboBoxGate->insertItems(0, QStringList()
        << QApplication::translate("AScanClass", "    I")
        << QApplication::translate("AScanClass", "    I2"));

	//QString iniFlieName = QDir::currentPath() += "/PUTS.ini";
	//QSettings * pSettings = new QSettings(iniFlieName, QSettings::IniFormat); //初始化配置对象

	QString strPath = QDir::currentPath() + "/PUTS.ini" ;
	QSettings setting(strPath,QSettings::IniFormat);
	g_ADataAvgNum = setting.value("ADataAvgNum/DataNum","").toInt();
	if (g_ADataAvgNum < 1 || g_ADataAvgNum > 1000)
	{
		g_ADataAvgNum = 1;
		setting.setValue("ADataAvgNum/DataNum", 1);
	}
	MaxHeightValue = setting.value("MaxHeight/MaxHeightValue","").toFloat();
	if (MaxHeightValue < 1 || MaxHeightValue > 2000)
	{
		MaxHeightValue = 490;
		setting.setValue("MaxHeight/MaxHeightValue", 490);
	}
}

AScan::~AScan()
{
	if ( m_nTIMER_Sel_Channel != 0 )
	{
		killTimer(m_nTIMER_Sel_Channel);
	}
}

void AScan::mousePressEvent(QMouseEvent *e)
{
	if (e->button()==Qt::LeftButton)
	{
		QPoint pos = e->pos();
		//闸门I
		if (pos.x()>m_pGateILeftTop.x()+3 && pos.y()>m_pGateILeftTop.y() && pos.x()<m_pGateIRightBottom.x()-3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right()) //整体选中
		{
			m_bGateIIsPressed = true;
			m_bGateIIsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateILeftTop.x()-3 && pos.y()>m_pGateILeftTop.y() && pos.x()<=m_pGateILeftTop.x()+3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())//左
		{
			m_bGateIIsPressed = true;
			m_bGateILIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateIRightBottom.x()-3 && pos.y()>m_pGateILeftTop.y() && pos.x()<=m_pGateIRightBottom.x()+3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right()) //右
		{
			m_bGateIIsPressed = true;
			m_bGateIRIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=false;
			m_bGateRight=true;
		}
		else
		{
			m_bGateIIsPressed = false;
			update();
		}

		if (m_bGateIIsPresse||m_bGateILIsPresse||m_bGateIRIsPresse)
			startPnt = e->pos();

		//闸门I2
		if (pos.x()>m_pGateI2LeftTop.x()+3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<m_pGateI2RightBottom.x()-3 && pos.y()<m_pGateI2RightBottom.y() && (m_bGateIIsPressed == false) && pos.x()>=m_rectWave.left() && pos.x()<=m_rectWave.right())
		{
			m_bGateI2IsPressed = true;
			m_bGateI2IsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateI2LeftTop.x()-3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<=m_pGateI2LeftTop.x()+3 && pos.y()<m_pGateI2RightBottom.y()&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateI2IsPressed = true;
			m_bGateI2LIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateI2RightBottom.x()-3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<=m_pGateI2RightBottom.x()+3 && pos.y()<m_pGateI2RightBottom.y() && (m_bGateIIsPressed == false) && pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateI2IsPressed = true;
			m_bGateI2RIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=false;
			m_bGateRight=true;
		}
		else
		{
			m_bGateI2IsPressed = false;
			update();
		}
		if (m_bGateI2IsPresse||m_bGateI2LIsPresse||m_bGateI2RIsPresse)
			startPnt = e->pos();
	}
}

void AScan::mouseMoveEvent(QMouseEvent *e)
{
	//鼠标样式变化
	if (!(m_bGateIIsPresse||m_bGateILIsPresse||m_bGateIRIsPresse || m_bGateI2IsPresse ||m_bGateI2LIsPresse || m_bGateI2RIsPresse))
	{
		if ((e->pos().x()>m_pGateILeftTop.x()+3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<m_pGateIRightBottom.x()-3 && e->pos().y()<m_pGateIRightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right())
			||(e->pos().x()>m_pGateI2LeftTop.x()+3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<m_pGateI2RightBottom.x()-3 && e->pos().y()<m_pGateI2RightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right()))
		{
			setCursor(Qt::OpenHandCursor);
		}
		else if ((e->pos().x()>=m_pGateILeftTop.x()-3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<=m_pGateILeftTop.x()+3 && e->pos().y()<m_pGateIRightBottom.y())
			||(e->pos().x()>=m_pGateI2LeftTop.x()-3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<=m_pGateI2LeftTop.x()+3 && e->pos().y()<m_pGateI2RightBottom.y()))
		{
			setCursor(Qt::SizeHorCursor);
		}
		else if ((e->pos().x()>=m_pGateIRightBottom.x()-3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<=m_pGateIRightBottom.x()+3 && e->pos().y()<m_pGateIRightBottom.y())
			||(e->pos().x()>=m_pGateI2RightBottom.x()-3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<=m_pGateI2RightBottom.x()+3 && e->pos().y()<m_pGateI2RightBottom.y()))
		{
			setCursor(Qt::SizeHorCursor);
		}
		else if (e->pos().y()<widget->rect().bottom() || e->pos().x()>verticalSliderGain->rect().left() || e->pos().x()>label_10->rect().left())
		{
			setCursor(Qt::ArrowCursor);  //在范围之外，箭头鼠标
		}
		else
		{
			setCursor(Qt::ArrowCursor);  //在范围之外，箭头鼠标
		}
	}
	//I移动
	if (m_bGateIIsPressed && m_bGateIIsPresse)  //整体拖动
	{
		QPoint sub = e->pos() - startPnt;    //拖动的距离(startPnt 鼠标点击的点在坐标)
		if (((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0 + m_fSync-m_fSync/100) //左
			|| ((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > g_Info[nPlate].height*1.2 + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay)))   //右
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0)  //下
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))  //上
			;    //判断移动的距离是否超限
		else
		{
			m_pGateILeftTop = m_pGateILeftTop + sub;         //赋值
			m_pGateIRightBottom = m_pGateIRightBottom + sub;
		}
		m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);  //矩形框的RECT
		startPnt=e->pos();
		m_fGateILeft=m_rectGateI.left();     //闸门的左侧
		m_fGateIRight=m_rectGateI.right();	 //闸门的右侧

	}
	else if (m_bGateIIsPressed && m_bGateILIsPresse)  //左拉伸
	{
		QPoint sub = e->pos() - startPnt;
		if (((m_pGateILeftTop + sub).x() > m_pGateIRightBottom.x())
			|| ((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0 + m_fSync-m_fSync/100) 
			|| ((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > g_Info[nPlate].height*1.2 + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
		{
			;
		}
		else
		{
			m_pGateILeftTop = m_pGateILeftTop + sub;
			m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);
			startPnt=e->pos();
			m_fGateILeft=m_rectGateI.left();
		}
	}	
	else if (m_bGateIIsPressed && m_bGateIRIsPresse)  //右拉伸
	{
		QPoint sub = e->pos() - startPnt;
		if (((m_pGateIRightBottom + sub).x() < m_pGateILeftTop.x()) 
			|| ((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100)
			|| ((((m_pGateIRightBottom + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > GATE_MAX_RANGE + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay)))
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
		{
			;
		}
		else
		{
			m_pGateIRightBottom = m_pGateIRightBottom + sub;
			m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);
			startPnt=e->pos();
			m_fGateIRight=m_rectGateI.right();
		}
	}
	//I2
	if (m_bGateI2IsPressed && m_bGateI2IsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if (((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) >  g_Info[nPlate].height*1.2 + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
			;
		else
		{
			m_pGateI2LeftTop = m_pGateI2LeftTop + sub;
			m_pGateI2RightBottom = m_pGateI2RightBottom + sub;
		}
		m_rectGateI2 = QRect(m_pGateI2LeftTop,m_pGateI2RightBottom);
		startPnt=e->pos();
		m_fGateI2Left=m_rectGateI2.left();
		m_fGateI2Right=m_rectGateI2.right();
	}
	else if (m_bGateI2IsPressed && m_bGateI2LIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateI2LeftTop + sub).x() > m_pGateI2RightBottom.x()
			|| ((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) >  g_Info[nPlate].height*1.2 + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
		{
			;
		}   
		else
		{
			m_pGateI2LeftTop = m_pGateI2LeftTop + sub;
			m_rectGateI2 = QRect(m_pGateI2LeftTop,m_pGateI2RightBottom);
			startPnt=e->pos();
			m_fGateI2Left=m_rectGateI2.left();
		}
	}	
	else if (m_bGateI2IsPressed && m_bGateI2RIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateI2RightBottom + sub).x() < m_pGateI2LeftTop.x()
			|| ((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateI2RightBottom + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) >  GATE_MAX_RANGE + m_fSync-5*m_fSync/100+Dot2Range(VSOUND, Range2Dot(VSOUND, m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateI2LeftTop + sub).y()+((double)(m_pGateI2RightBottom + sub).y()-(double)(m_pGateI2LeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
		{
			;
		}
		else
		{
			m_pGateI2RightBottom = m_pGateI2RightBottom + sub;
			m_rectGateI2 = QRect(m_pGateI2LeftTop,m_pGateI2RightBottom);
			startPnt=e->pos();
			m_fGateI2Right=m_rectGateI2.right();
		}
	}
	update();
}

void AScan::mouseReleaseEvent(QMouseEvent *e)
{
    m_bGateIIsPresse = false;
    m_bGateI2IsPresse = false;
	m_bGateILIsPresse = false;
	m_bGateIRIsPresse = false;
	m_bGateI2LIsPresse = false;
	m_bGateI2RIsPresse = false;
	m_bGateMove=true;
	setCursor(Qt::ArrowCursor);
}

void AScan::GetScreenInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();//获取可用桌面大小
	QRect deskRect = desktopWidget->availableGeometry();//获取设备屏幕大小
	QRect screenRect = desktopWidget->screenGeometry();
	m_nActScreenX = screenRect.width();
	m_nActScreenY = screenRect.height();
	m_nScreenCount = desktopWidget->screenCount();
}

void AScan::DataMemberInit()	//成员变量初始化
{
	m_bSelChannel = FALSE;
	m_bSetOne=false;
	m_bSetAll = FALSE;
	m_ncomboBoxGate=0;
	m_nProbeBox=0;
// 	m_fGateFrontOut = 5;
// 	m_fGateFrontIn = 5;
// 	m_fGateWidthOut = 20;
// 	m_fGateWidthIn = 20;
// 	m_fGatePeakOut = 0;
// 	m_fGatePeakIn = 0;
	m_fZeroPoint=0.0;
// 	m_fDelayOut=0;
// 	m_fDelay=0.0;
//  	m_fRangeOut=0;
// 	m_fRange=30;
	m_fReFreq=600.0;
	m_fGainOut=0;
	m_fGain=0.0;
// 	m_fSuppressionOut=0;
// 	m_fSuppression=0.0;
	m_bGateI=TRUE;
	m_bGateI2=TRUE;
	m_bProbe1Box1=1;
	m_bProbe1Box2=0;
	m_bProbe1Box3=0;
	m_bProbe1Box4=0;
	m_bProbe1Box5=0;
	m_bProbe2Box1=0;
	m_bProbe2Box2=0;
	m_bProbe2Box3=0;
	m_bProbe2Box4=0;
	m_bProbe2Box5=0;
	m_bProbe3Box1=0;
	m_bProbe3Box2=0;
	m_bProbe3Box3=0;
	m_bProbe3Box4=0;
	m_bProbe3Box5=0;
	m_bProbe4Box1=0;
	m_bProbe4Box2=0;
	m_bProbe4Box3=0;
	m_bProbe4Box4=0;
	m_bProbe4Box5=0;
	m_bProbe5Box1=0;
	m_bProbe5Box2=0;
	m_bProbe5Box3=0;
	m_bProbe5Box4=0;
	m_bProbe5Box5=0;
	m_bProbe6Box1 = 0;
	m_bProbe6Box2 = 0;
	m_bProbe6Box3 = 0;
	m_bProbe6Box4 = 0;
	m_bProbe6Box5 = 0;

	m_bGateIIsPressed=true;
	m_fGateILeftUpdate=0.05f;
	m_fGateITopUpdate=0.02f;
	m_bGateI2IsPressed=false;
	m_fGateI2LeftUpdate=0.05f;
	m_fGateI2TopUpdate=0.11f;
	m_bGateIIsPresse=false;
	m_bGateILIsPresse=false;
	m_bGateIRIsPresse=false;
	m_bGateI2IsPresse=false;

	m_bGateI2LIsPresse=false;
	m_bGateI2RIsPresse=false;

	m_bGateMove=true;
	m_bGateLeft=false;
	m_bGateRight=false;
	m_bGateIFirstPressed = false;
	m_bGateI2FirstPressed = false;

// 	m_fIGateFront=5;
// 	m_fIGateWidth=20;
// 	m_fIGatePeak=5;
// 	m_fI2GateFront=5;
// 	m_fI2GateWidth=20;
// 	m_fI2GatePeak=20;
	m_nFlawOrBottom=1;
	m_fSync = 0.0;
	m_qsProbe = tr("探头1");
	DataBaseAscanSyn();
	emit AscanInfoChanged(); //改变界面的数值
}

void AScan::DataBaseAscanSyn() //从数据同步到A扫控件成员变量
{
	
	m_fGateFrontIn = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	m_fGateFrontOut = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	m_fI2GateFront = Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME);

	m_fGateWidthOut =  Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME) - Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	m_fGateWidthIn =  Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME) - Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	m_fI2GateWidth =  Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_MAXBACKTIME) - Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME);

	m_fGatePeakOut = 50;
	m_fGatePeakIn = 50;

//	m_fDelayOut= Dot2Range(VSOUND,allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT); //延迟
//	m_fDelay=Dot2Range(VSOUND,allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT);
	m_fDelayOut = (float)allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT / ((float)g_nADCFreq * 0.000001);//chenyuzhu modify on 2014-10-13
	m_fDelay = (float)allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT / ((float)g_nADCFreq * 0.000001);//chenyuzhu modify on 2014-10-13
   //增益
	m_fGainOut = (Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D) - 4.2) / g_GainSale[g_dwActiveChannel];
	m_fGain = (Data2Gain(allCH_CONFIG[g_dwActiveChannel].CH_GAIN_C + allCH_CONFIG[g_dwActiveChannel].CH_GAIN_D) -4.2) / g_GainSale[g_dwActiveChannel];


	//chenyuzhu modify start on 2014-7-10
	//抑制
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

/*	QSqlQuery sqlQuery(*g_sqlDB);
	QString strQuery = QString("%1").arg("select ADCFreq,ADCaiYangShu from t_YiQi where name='PUTS-ARS144'");
	sqlQuery.exec(strQuery);
	QString strADCaiYangShu;
	sqlQuery.next();
	strADCaiYangShu = sqlQuery.value(1).toString();*/
//	int nADCaiYangShu = strADCaiYangShu.toInt();
	int nADCaiYangShu = SheBeiCanShu.YiQiParam.nADCaiYangShu;
	m_fSuppressionOut=allCH_CONFIG[g_dwActiveChannel].CH_REJECT_VALUE * 100.0 /pow(2.0,nADCaiYangShu - 1);
	m_fSuppression=m_fSuppressionOut;
	m_fRange = g_Info[0].height * 1.5;
	if (m_fRange < 30)
	{
		m_fRange = 30;
	}
	//chenyuzhu modify end on 2014-7-10
}

void AScan::UiInit()	//Ui初始化
{
	GetScreenInfo();
	resize(m_nActScreenX-3,m_nActScreenY-60);
	setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);
	pushButtonOptionShow->hide();
	checkBoxGateSwitch->setChecked(true);
	
	//chenyuzhu add start on 2014.5.16 
	//在所有的connect中传了Qt::UniqueConnection类型参数，作用只有在没有重复连接的情形下，新的连接才会建立。如果已有一个同样的连接，新的连接就会失败同时connect返回false. 
	//chenyuzhu add end on 2014.5.16 
	
	//闸门前沿
	connect(spinBoxGateFront, SIGNAL(valueChanged(double)), this, SLOT(slotSpinbox_SliderGateFront()),Qt::UniqueConnection);  //通过spinbox改变滑条的值
	connect(horizontalSliderGateFront, SIGNAL(valueChanged(int)), this, SLOT(slotSlider_SpinboxGateFront()),Qt::UniqueConnection);//通过滑条改变BOX
	connect(spinBoxGateFront,SIGNAL(valueChanged(double)),this,SLOT(GateFrontChange()),Qt::UniqueConnection);                      //改变前沿的值
	horizontalSliderGateFront->setRange(0,int(g_Info[nPlate].height*1.2*10));    //chenyuzhu amend on 2014.5.15                      
	spinBoxGateFront->setRange(0,g_Info[nPlate].height*1.2);
	spinBoxGateFront->setSingleStep(0.1);

	//闸门宽度
	connect(spinBoxGateWidth,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGateWidth()),Qt::UniqueConnection);
	connect(horizontalSliderGateWidth,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGateWidth()),Qt::UniqueConnection);
	connect(spinBoxGateWidth,SIGNAL(valueChanged(double)),this,SLOT(GateWidthChange()),Qt::UniqueConnection);
	horizontalSliderGateWidth->setRange(0,1000);
	spinBoxGateWidth->setRange(0,(double)100);
	spinBoxGateWidth->setSingleStep(0.1);

	//闸门阈值
	connect(spinBoxGatePeak,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGatePeak()),Qt::UniqueConnection);
	connect(horizontalSliderGatePeak,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGatePeak()),Qt::UniqueConnection);
	connect(spinBoxGatePeak,SIGNAL(valueChanged(double)),this,SLOT(GatePeakChange()),Qt::UniqueConnection);
	horizontalSliderGatePeak->setRange(0,GATE_PEAK_MAX_RANGE*1000);
	spinBoxGatePeak->setRange(0,(double)GATE_PEAK_MAX_RANGE);
	spinBoxGatePeak->setSingleStep(0.1);
    //延迟
	connect(spinBoxDelay,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderDelay()),Qt::UniqueConnection);
	connect(horizontalSliderDelay,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxDelay()),Qt::UniqueConnection);
	connect(spinBoxDelay,SIGNAL(valueChanged(double)),this,SLOT(DelayChange()),Qt::UniqueConnection);
	horizontalSliderDelay->setRange(0,10000);
	spinBoxDelay->setRange(0,(double)100);
	spinBoxDelay->setWrapping(1);
	spinBoxDelay->setSingleStep(0.01);

	connect(spinBoxXRange,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderXRange()),Qt::UniqueConnection);
	connect(horizontalSliderXRange,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxXRange()),Qt::UniqueConnection);
	connect(spinBoxXRange,SIGNAL(valueChanged(double)),this,SLOT(RangeChange()),Qt::UniqueConnection);
	horizontalSliderXRange->setRange(300,10000);//范围	//chenyuzhu modify on 2014-11-5
	spinBoxXRange->setRange(30,1000);	//chenyuzhu modify on 2014-11-5
	spinBoxXRange->setSingleStep(0.1);
   //抑制
	connect(spinBoxSuppression,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderSuppression()),Qt::UniqueConnection);
	connect(verticalSliderSuppression,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxSuppression()),Qt::UniqueConnection);
	connect(spinBoxSuppression,SIGNAL(valueChanged(double)),this,SLOT(SuppressionChange()),Qt::UniqueConnection);
	verticalSliderSuppression->setRange(0,1000);
	spinBoxSuppression->setRange(0,100);
	spinBoxSuppression->setSingleStep(0.1);
    //增益
	connect(spinBoxGain,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGain()),Qt::UniqueConnection);
	connect(verticalSliderGain,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGain()),Qt::UniqueConnection);
	connect(spinBoxGain,SIGNAL(valueChanged(double)),this,SLOT(GainChange()),Qt::UniqueConnection); 
	verticalSliderGain->setRange(0,800);
	spinBoxGain->setRange(0,80);
	spinBoxGain->setSingleStep(0.1);

	//中值
	connect(spinBoxZhongZhi, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBox_ZhongZhi()), Qt::UniqueConnection); 
	
	//增益系数
	connect(spinBoxZengYiXiShu, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBox_ZhengYiXiShu()), Qt::UniqueConnection);
	spinBoxZengYiXiShu->setSingleStep(0.01);

	connect(comboBoxProbeBox, SIGNAL(activated(int)), this, SLOT(ProbeBoxChange()),Qt::UniqueConnection);
	connect(comboBoxProbe, SIGNAL(activated(int)), this, SLOT(ProbeChange()),Qt::UniqueConnection);

	connect(pushButtonOptionHide, SIGNAL(clicked()), this, SLOT(toolbar_hide()),Qt::UniqueConnection);
	connect(pushButtonOptionShow, SIGNAL(clicked()), this, SLOT(toolbar_show()),Qt::UniqueConnection);
	connect(pushButtonSaveScreen, SIGNAL(clicked()), this, SLOT(pushButtonSaveScreenRespond()),Qt::UniqueConnection);
	connect(pushButtonRecordVideo, SIGNAL(clicked()), this, SLOT(pushButtonRecordVideoRespond()),Qt::UniqueConnection);
	connect(pushButtonFreezeWave, SIGNAL(clicked()), this, SLOT(pushButtonFreezeWaveRespond()),Qt::UniqueConnection);
	connect(pushButtonDisplayParaSet, SIGNAL(clicked()), this, SLOT(pushButtonDisplayParaSetRespond()),Qt::UniqueConnection);
	connect(pushButtonFunctionhide, SIGNAL(clicked()), this, SLOT(pushButtonFunctionhideRespond()),Qt::UniqueConnection);
	connect(pushButtonAdd2, SIGNAL(clicked()), this, SLOT(pushButtonAdd2Respond()),Qt::UniqueConnection);
	connect(pushButtonCut2, SIGNAL(clicked()), this, SLOT(pushButtonCut2Respond()),Qt::UniqueConnection);
	connect(pushButtonAScanSave, SIGNAL(clicked()), this, SLOT(pushButtonAScanSaveRespond()),Qt::UniqueConnection);

	connect(checkBoxProbe1, SIGNAL(clicked()), this, SLOT(checkBoxProbe1Change()),Qt::UniqueConnection);
	connect(checkBoxProbe2, SIGNAL(clicked()), this, SLOT(checkBoxProbe2Change()),Qt::UniqueConnection);
	connect(checkBoxProbe3, SIGNAL(clicked()), this, SLOT(checkBoxProbe3Change()),Qt::UniqueConnection);
	connect(radioButtonFlawEcho, SIGNAL(clicked()), this, SLOT(radioButtonFBChange()),Qt::UniqueConnection);
	connect(radioButtonBottomEcho, SIGNAL(clicked()), this, SLOT(radioButtonFBChange()),Qt::UniqueConnection);

	connect(this, SIGNAL(AscanInfoChanged()), this, SLOT(AscanInfoUpdate()),Qt::UniqueConnection); //各滑块更新函数  
	connect(checkBoxLock, SIGNAL(clicked()), this, SLOT(AScanLock()),Qt::UniqueConnection);
	comboBoxProbeBox->clear();  //chenyuzhu add on 2014-7-21
	comboBoxProbeBox->addItem(tr("边探1"));
	comboBoxProbeBox->addItem(tr("边探2"));

	// wangwei modified on 2016-01-14
	// 太钢2016年更新后，将太钢和武钢合并，一些差异性参数统一放到OtherParameters.ini中配置
	int NeiTan1Num = g_OtherParameters.insideCar1BoxCount;
	int NeiTanAllNum = NeiTan1Num + g_OtherParameters.insideCar2BoxCount;
	
	for (int i = 1;i <= NeiTanAllNum;i++)
	{
		if (i >= 1 && i <= NeiTan1Num)
		{
			QString str = tr("内探1●探头盒") + QString::number(i);
			comboBoxProbeBox->addItem(str);
		}
		else
		{
			QString str = tr("内探2●探头盒") + QString::number(i-NeiTan1Num);
			comboBoxProbeBox->addItem(str);
		}
	}
	//初始化后选择第一个
	comboBoxProbeBox->setCurrentIndex(0);
	ProbeBoxChange();  //探头盒切换，判断哪些探头选中然后设置选中状态
	radioButtonFlawEcho->setChecked(true);
	connect(comboBoxGate, SIGNAL(activated(int)), this, SLOT(GateBoxChange(int)),Qt::UniqueConnection);//chenyuzhu add start on 2014-11-2
	spinBoxZhongZhi->setVisible(false);
	spinBoxZengYiXiShu->setVisible(false);
	label_31->setVisible(false);
	label_32->setVisible(false);
}

void AScan::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QBrush brush(Qt::black);     // 画刷
	painter.setBrush(brush);            // 设置画刷
	painter.setPen(Qt::black);
	DistributeRegion();       //画几个区域
	GateMoveUpdate();         //整体拖动图和控件变化改变闸门变量
	painter.drawRect(m_rectWave); 
	DrawGrid(painter, m_rectWave, m_fDelayOut, m_nGrid, m_nLines);
	DrawWave(painter, m_rectWave);
	GateIPainter(painter);  //画闸门 通知改变控件对应变量的值
	GateI2Painter(painter);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.drawRect(m_rectHRuler);
	painter.drawRect(m_rectVRuler);
	painter.drawRect(m_rectBlank);
	DrawVRuler(painter, m_rectVRuler, -10, 100, 5, 1, 0);
	DrawRuler(painter, m_rectHRuler, (int)m_fXMinRuler, (int)m_fXMaxRuler, 10, 1, 1);
}

void AScan::DistributeRegion()//画图区域分配函数
{
	m_rectWave.setRect(widget_3->rect().left()+widget_2->rect().width()+34, widget_3->rect().top()+widget->rect().height()+1, widget_3->rect().width()-34, widget_3->rect().height()-31);
	m_rectBlank.setRect(widget_3->rect().left()+widget_2->rect().width()-1,widget_3->rect().bottom()+widget->rect().height()-29,35,30);
	m_rectHRuler.setRect(widget_3->rect().left()+widget_2->rect().width()+34,widget_3->rect().bottom()+widget->rect().height()-29,widget_3->rect().width()-34, 30);
	m_rectVRuler.setRect(widget_3->rect().left()+widget_2->rect().width()-1, widget_3->rect().top()+widget->rect().height()+1, 35, widget_3->rect().height()-31);
}

void AScan::DrawGrid(QPainter &painter, const QRect& rect,double dXMove,int *nGrid,int nlines)	//画网格函数
{
	painter.setPen(Qt::green);
	for (int i = 0; i< 11; i++)
	{
		nGrid[i] = rect.top() + (float)rect.height() / 11.0 * i;
	}

	//横向网格虚线
	for(int k = 1;k<11;++k)
	{
		for(int j = 0;j< 10;++j)
		{
			QPoint pt1;
			QPoint pt2;
			pt1.setX(1.0/10.0*rect.width()*j);
			pt1.setY(2.0/10.0*rect.height()*k);
			pt2.setX(1.0/10.0*rect.width()*(j+1));
			pt2.setY(2.0/10.0*rect.height()*k);
			for (int i = 0;i < 10;i++)
			{
				//painter.drawLine(pt1.x()+1.0/10*(pt2.x() - pt1.x())*i + rect.left(), pt1.y()+1.0/10*(pt2.y() - pt1.y())*i + rect.top(),pt1.x()+1.0/10*(pt2.x() - pt1.x())*(i+1)+rect.left(),pt1.y()+1.0/10*(pt2.y() - pt1.y())*(i+1) + rect.top());
				painter.drawLine(pt1.x()+1.0/10*(pt2.x() - pt1.x())*i + rect.left(), nGrid[k], pt1.x()+1.0/10*(pt2.x() - pt1.x())*(i+1)+rect.left(),nGrid[k]);
				i++;
			}
		}
	}
	//纵向网格虚线
	//for(int i = 1;i<= nlines;++i)
	for (int i = 0; i< 10; i++)
	{
		nGrid[i] = rect.left() + (float)rect.width() / 10.0 * i;
	}
	
	for(int i = 1;i<= 10; ++i)
	{
		for(int j = 0;j<5;++j)
		{
			QPoint pt1;
			QPoint pt2;
			pt1.setX(1.0/10.0*rect.width()*i);
			pt1.setY(2.0/10.0*rect.height()*j);
			pt2.setX(1.0/10.0*rect.width()*i);
			pt2.setY(2.0/10.0*rect.height()*(j+1));
			for (int k = 0;k < 10;k++)
			{
				painter.drawLine(nGrid[i-1], pt1.y()+1.0/10*(pt2.y() - pt1.y())*k + rect.top(),nGrid[i-1] , pt1.y()+1.0/10*(pt2.y() - pt1.y())*(k+1) + rect.top());
				k++;
			}
		}
	}
}

//nTextLocation为1时，刻度画在区域的底部；为0时，刻度画在区域的顶部
void AScan::DrawHRuler(QPainter &painter, const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)	//画水平标尺函数 
{
	painter.setPen(Qt::black);
	int iStartPos = 0;  
	QRect rectText;
	m_nLines = 0;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( nTextLocation == 1 ) 
	{
		rectText.setTop(rect.bottom()-29);
		rectText.setBottom(rect.bottom()-15);

		iLLineStart = rect.bottom() - 18;
		iLLineEnd	 = rect.bottom() - 2;

		iMLineStart = rect.bottom() - 12;
		iMLineEnd	 = rect.bottom() - 2;

		iSLineStart = rect.bottom() - 8;
		iSLineEnd	 = rect.bottom() - 2;

	}
	else if(nTextLocation == 0)
	{

		rectText.setTop(rect.top()+19);
		rectText.setBottom(rect.top()+30);

		iLLineStart = rect.top() + 2;
		iLLineEnd = rect.top() + 18;

		iMLineStart = rect.top() + 2;
		iMLineEnd = rect.top() + 12;

		iSLineStart = rect.top() + 2;
		iSLineEnd = rect.top() + 8;

	}

	QString str;
	int iPos;
//	int iSel = 0;
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

	for (i = nLinestart;i<nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{

			if (nTextDirection == 0)
			{
				iPos = rect.left() + ((rect.width())*(i - nStart))/(nEnd - nStart);
				rectText.setLeft(iPos-53);
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
					m_nGrid[m_nLines] = iPos;
					
					if (m_nLines<19)
						m_nLines +=1;
					else
						m_nLines = 0;
					rectText.setRight(iPos+60); 
					if (rectText.right() > rect.right())
					{
						rectText.setRight(rect.right()+35);
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
						painter.drawText( rectText, Qt::AlignHCenter ,str  );
					}
					else
					{
						painter.drawText( rectText, Qt::AlignHCenter ,str );
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

void AScan::DrawVRuler(QPainter &painter, const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)	//画纵向标尺函数
{
	painter.setPen(Qt::black);
	if (nEnd == 20)
	{
		int test = 0;
	}

	QRect rectText;
//	int iSayac = 0;
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
//	int iSel = 0;


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

void AScan::DrawWave(QPainter &painter, const QRect& rect)	//画波形
{
	DWORD dwWaitObjectIndex = WaitForSingleObject(g_hWaveReadyEvent, 0); //当map里有数据时画图

	painter.setPen(Qt::yellow);
	int nSize = MONITOR_SIZE;
	int height = rect.height() - 1;
	float yb = rect.height() / MaxHeightValue * WAVE_ZOOM_SCALE;
	float xb = rect.width() / float(nSize);

	map<WORD,CPicData*>::iterator iter;
	iter = m_MapCPicData.begin();

	WORD wTempProbeNum = iter->first;
	QColor colorY = QColor(255,255,0);//黄色
	QColor colorR = QColor(255,0,0);//红色
	//QColor colorB = QColor(0,0,255);//蓝色
	QColor colorB = QColor(255,97,0);//橙色

	if (wTempProbeNum > 15)
	{
		if ((wTempProbeNum - 1) % 3 == 0)
		{
			painter.setPen(colorY);
		}
		else if ((wTempProbeNum - 1) % 3 == 1)
		{
			painter.setPen(colorR);
		}
		else if ((wTempProbeNum - 1) % 3 == 2)
		{
			painter.setPen(colorB);
		}
	}
	else if (wTempProbeNum > 7)
	{
		if ((wTempProbeNum - 2) % 3 == 0)
		{
			painter.setPen(colorY);
		}
		else if ((wTempProbeNum - 2) % 3 == 1)
		{
			painter.setPen(colorR);
		}
		else if ((wTempProbeNum - 2) % 3 == 2)
		{
			painter.setPen(colorB);
		}
	}
	else if(wTempProbeNum >= 0)
	{
		if (wTempProbeNum % 3 == 0)
		{
			painter.setPen(colorY);
		}
		else if (wTempProbeNum % 3 == 1)
		{
			painter.setPen(colorR);
		}
		else if (wTempProbeNum % 3 == 2)
		{
			painter.setPen(colorB);
		}
	}

	QPoint ptStart(rect.left(),rect.top() + height - (iter->second)->m_wData[0]*yb);
	QPoint ptEnd(rect.left(),rect.top());
	
	for (int i=0; i < nSize; i++)
	{
		if ((iter->second)->m_wData[i] > MaxHeightValue) 
			          (iter->second)->m_wData[i] = MaxHeightValue;
		ptEnd.setX(rect.left()+ i*xb);
		ptEnd.setY(rect.top() + height - (iter->second)->m_wData[i]*yb);
		painter.drawLine(ptStart,ptEnd);
		ptStart.setX(rect.left() + i*xb);
		ptStart.setY(rect.top() + height - (iter->second)->m_wData[i]*yb);
	}

	//显示最大波峰的值
	if (m_bGateIIsPressed)  //如果闸门1选中
	{
		int start = m_fIGateFront/ m_fRange * 512;
		int end = m_fI2GateFront /m_fRange * 512;
		int nMax = 0;
		int nIndex = 0;

		if (start < 0) 
		{
			return;
		}

		for (int i = start; i<= end ; i++)
		{
			if(iter->second->m_wData[i]>nMax)
			{
				nIndex = i;
				nMax = iter->second->m_wData[i];
			}
		}
		labelGateRange->setText(QObject::tr("闸门I内最大波峰值幅度: "));
		labelGateTime->setText(QObject::tr("闸门I同步时间: "));
		labelGateRangeShow->setText(QString::number((float)nMax / MaxHeightValue * 110.0));
		labelGateTimeShow->setText(QString::number(m_fRange / 512 *nIndex));
	}
	else if (m_bGateI2IsPressed) //如果闸门2选中
	{

		int start = m_fI2GateFront/ m_fRange * 512;
		int end = 0;
		if (m_fI2GateFront + m_fI2GateWidth > m_fRange)
		{
			end = 512;
		}
		else
		{
			end =(m_fI2GateFront + m_fI2GateWidth)/m_fRange * 512;	
		}

		int nMax = 0;
		int nIndex = 0;
		for (int i = start; i<= end ; i++)
		{
			if(iter->second->m_wData[i]>nMax)
			{
				nIndex = i;
				nMax = iter->second->m_wData[i];
			}
		}
		labelGateRange->setText(QObject::tr("闸门I2内最大波峰值幅度: "));
		labelGateTime->setText(QObject::tr("闸门I2同步时间: "));
		labelGateRangeShow->setText(QString::number((float)nMax / MaxHeightValue * 110.0));
		labelGateTimeShow->setText(QString::number(m_fRange / 512 *nIndex));
	}

	ResetEvent(g_hWaveReadyEvent);
}

void AScan::SetAScanData(WORD wChannelNum,int nFlawOrBottom,CPicData* pPicData)
{
	update();
}

void AScan::AScanLock()			//A扫控件锁定与解锁
{
	//chenyuzhu add start on 2014.4.29

	if (checkBoxLock->isChecked())
	{
		
		m_Lock = new CAscanLock();
		m_Lock->exec();
		if (true == m_Lock->m_IfLock)
		{
			radioButtonFlawEcho->setEnabled(false);
			radioButtonBottomEcho->setEnabled(false);
			horizontalSliderGateFront->setEnabled(false);
			horizontalSliderGateWidth->setEnabled(false);
			horizontalSliderGatePeak->setEnabled(false);
			horizontalSliderDelay->setEnabled(false);
			horizontalSliderXRange->setEnabled(false);
			spinBoxGateFront->setEnabled(false);
			spinBoxGatePeak->setEnabled(false);
			spinBoxGateWidth->setEnabled(false);
			spinBoxDelay->setEnabled(false);
			spinBoxXRange->setEnabled(false);
		}
		else
		{
			checkBoxLock->setChecked(false);
		}
	
	}
	else
	{
		m_UnLock = new CAscanUnLock();
		m_UnLock->exec();
		if (true == m_UnLock->m_IfUnLock)
		{

			radioButtonFlawEcho->setEnabled(true);
			radioButtonBottomEcho->setEnabled(true);
			horizontalSliderGateFront->setEnabled(true);
			horizontalSliderGateWidth->setEnabled(true);
			horizontalSliderGatePeak->setEnabled(true);
			horizontalSliderDelay->setEnabled(true);
			horizontalSliderXRange->setEnabled(true);
			spinBoxGateFront->setEnabled(true);
			spinBoxGatePeak->setEnabled(true);
			spinBoxGateWidth->setEnabled(true);
			spinBoxDelay->setEnabled(true);
			spinBoxXRange->setEnabled(true);
		}
		else
		{
			checkBoxLock->setChecked(true);
		}
	}
	//chenyuzhu add end on 2014.4.29
}

void AScan::toolbar_show()	//工具栏显示函数
{
	m_bGateIIsPressed=false;
	m_bGateI2IsPressed=false;
	pushButton_06->show();
	pushButton_05->show();
	pushButton_04->show();
	pushButton_02->show();
	pushButton_01->show();
	pushButtonOptionShow->hide();
	pushButtonOptionHide->show();
	pushButton_5->show();
	widgetProbeOption_2->show();
	widgetProbeOption->show();
	widgetGateOption->show();
	widgetProbeParam->show();
	widgetToolHide->show();	
	m_bGateMove=true;
}

void AScan::toolbar_hide()	//工具栏隐藏函数
{
	m_bGateIIsPressed=false;
	m_bGateI2IsPressed=false;
	pushButton_06->hide();
	pushButton_05->hide();
	pushButton_04->hide();
	pushButton_02->hide();
	pushButton_01->hide();
	pushButton_5->hide();
	widgetProbeOption_2->hide();
	pushButtonOptionHide->hide();
	pushButtonOptionShow->show();
	widgetProbeOption->hide();
	widgetGateOption->hide();
	widgetProbeParam->hide();
	widgetToolHide->hide();
	m_bGateMove=true;
}

void AScan::ProbeBoxChange()	//探头盒切换函数
{
	//chenyuzhu add start on 2014-8-11//默认选中闸门一
	m_bGateIIsPressed = true;
	m_bGateI2IsPressed = false;
	//chenyuzhu add end on 2014-8-11
	m_nProbeBox = comboBoxProbeBox->currentIndex(); 
	//探头盒切换后只会选中第一个探头
	int nTemp = m_mapChanel[m_nProbeBox].value(0).PhysicalNo; //物理通道号

	//chenyuzhu add start on 2014-6-20
	int nTempLuoji = m_mapChanel[m_nProbeBox].value(0).LogicNo; //逻辑通道号
	g_dwActiveChannelLuoJi = nTempLuoji;
	//chenyuzhu add end on 2014-6-20

	//选中第一个复选框
	checkBoxProbe1->setChecked(true);
	checkBoxProbe2->setChecked(false);
	checkBoxProbe3->setChecked(false);
	g_dwActiveChannel = nTemp;
	m_ndwActiveNum = g_dwActiveChannel;
	g_nSetOneChannel = g_dwActiveChannel;
	
	
	//更新右侧第二个下拉框，固定为探头1
	//UpdateProbe();    //更新右侧Combobox更新当前选中状态
	comboBoxProbe->clear();  //当前选中探头控件
	comboBoxProbe->addItem(tr("探头1"));
	comboBoxProbe->setCurrentIndex(0);
    //更新m_qsProbe
	m_qsProbe = tr("探头1");
	UpdateActiveChannelInfo(nTemp);//更改探头后，更新和探头相关的控件数据
	//更新map
	m_MapCPicData.clear();
	CPicData* pCPicData = new CPicData;
	m_MapCPicData.insert(pair<WORD,CPicData*>(nTemp,pCPicData));
	if (g_bStart)
	{
		m_bSelChannel = TRUE;
	}
}

//chenyuzhu add start on 2014-11-3
void AScan::ChannelNumChange()	//c扫探头盒切换函数及
{
	//chenyuzhu add start on 2014-8-11//默认选中闸门一
	m_bGateIIsPressed = true;
	m_bGateI2IsPressed = false;
	//chenyuzhu add end on 2014-8-11
	//chenyuzhu add start on 2014-11-3
	m_nProbeBox = comboBoxProbeBox->currentIndex(); //获取探头合号
	int TantouNum = 0;
	if ("探头1" == m_qsProbe)
	{
		checkBoxProbe1->setChecked(true);
		checkBoxProbe2->setChecked(false);
		checkBoxProbe3->setChecked(false);
		TantouNum = 0;
	}
	else if ("探头2" == m_qsProbe)
	{
		checkBoxProbe1->setChecked(false);
		checkBoxProbe2->setChecked(true);
		checkBoxProbe3->setChecked(false);
		TantouNum = 1;
	}
	else if ("探头3" == m_qsProbe)
	{
		checkBoxProbe1->setChecked(false);
		checkBoxProbe2->setChecked(false);
		checkBoxProbe3->setChecked(true);
		TantouNum = 2;
	}
	//chenyuzhu add end on 2014-11-3
	//探头盒切换后只会选中第一个探头
	int nTemp = m_mapChanel[m_nProbeBox].value(TantouNum).PhysicalNo; //物理通道号 //chenyuzhu modify on 2014-11-3

	//chenyuzhu add start on 2014-6-20
	int nTempLuoji = m_mapChanel[m_nProbeBox].value(TantouNum).LogicNo; //逻辑通道号 //chenyuzhu modify on 2014-11-3
	g_dwActiveChannelLuoJi = nTempLuoji;
	//chenyuzhu add end on 2014-6-20

	//选中第一个复选框
	
	//checkBoxProbe1->setChecked(true);
	//checkBoxProbe2->setChecked(false);
	//checkBoxProbe3->setChecked(false);
	g_dwActiveChannel = nTemp;
	m_ndwActiveNum = g_dwActiveChannel;
	g_nSetOneChannel = g_dwActiveChannel;
	//更新右侧第二个下拉框，固定为探头1
	//UpdateProbe();    //更新右侧Combobox更新当前选中状态
	comboBoxProbe->clear();  //当前选中探头控件
	//comboBoxProbe->addItem(tr("探头1"));
	comboBoxProbe->addItem(m_qsProbe);//chenyuzhu add on 2014-11-3
	comboBoxProbe->setCurrentIndex(0);
	//更新m_qsProbe
	//m_qsProbe = tr("探头1");
	UpdateActiveChannelInfo(nTemp);//更改探头后，更新和探头相关的控件数据
	//更新map
	m_MapCPicData.clear();
	CPicData* pCPicData = new CPicData;
	m_MapCPicData.insert(pair<WORD,CPicData*>(nTemp,pCPicData));
	if (g_bStart)
	{
		m_bSelChannel = TRUE;
	}
}
//chenyuzhu add end on 2014-11-3

void AScan::ProbeChange()
{
	CurSelectProbe();
}

void AScan::pushButtonSaveScreenRespond()	//截屏
{
	//QMessageBox::information(this,"rr",tr("adf")); //chenyuzhu add on 2014.5.4	
}

void AScan::pushButtonRecordVideoRespond()	//录像
{

}

void AScan::pushButtonFreezeWaveRespond()	//冰冻波形
{
	
}

void AScan::pushButtonDisplayParaSetRespond()	//悬浮参数显示
{

}

void AScan::pushButtonFunctionhideRespond()	//功能按钮隐藏
{
	static bool show = true;
	
	spinBoxZhongZhi->setVisible(show);
	spinBoxZengYiXiShu->setVisible(show);
	label_31->setVisible(show);
	label_32->setVisible(show);
	if (show == true)
	{
		show = false;
	}
	else
	{
		show = true;
	}

}

void AScan::pushButtonAdd2Respond()	//+2dB
{
	if (m_fGain + 2 > 800)
	{
		m_fGain = 800;
	}
	else
	{
		m_fGain +=2;
	}
	emit AscanInfoChanged();
}

void AScan::pushButtonCut2Respond()	//-2dB
{
	if (m_fGain - 2 < 0)
	{
		m_fGain = 0;
	}
	else
	{
		m_fGain -=2;
	}
	emit AscanInfoChanged();
}

void AScan::pushButtonAScanSaveRespond()	//保存
{

}

//点击探头勾选框进入此函数
void AScan::CheckBoxProbeChange(int i)
{
	WORD m_wMapChannelNum = m_mapChanel[m_nProbeBox].value(i).PhysicalNo; //物理通道号
	//chenyuzhu add start on 2014-6-20
	int nTempLuoji = m_mapChanel[m_nProbeBox].value(i).LogicNo; //逻辑通道号
	g_dwActiveChannelLuoJi = nTempLuoji;
	//chenyuzhu add end on 2014-6-20
	if (g_dwActiveChannel == m_wMapChannelNum) //若是已经选中，则不作操作
	{
		//不许取消探头
	}
	else //切换探头
	{
		if (i == 0)
		{
			checkBoxProbe1->setChecked(true);
			checkBoxProbe2->setChecked(false);
			checkBoxProbe3->setChecked(false);
			g_dwActiveChannel = m_wMapChannelNum;
			g_nSetOneChannel = g_dwActiveChannel;
			m_ndwActiveNum = g_dwActiveChannel;
			//更新第二个下拉框
			comboBoxProbe->clear();  //当前选中探头控件
			comboBoxProbe->addItem(tr("探头1"));
			comboBoxProbe->setCurrentIndex(0);
			m_qsProbe = tr("探头1");
			
		}
		else if(i == 1)
		{
			checkBoxProbe1->setChecked(false);
			checkBoxProbe2->setChecked(true);
			checkBoxProbe3->setChecked(false);
			g_dwActiveChannel = m_wMapChannelNum;
			m_ndwActiveNum = g_dwActiveChannel;
			g_nSetOneChannel = g_dwActiveChannel;
			//更新第二个下拉框
			comboBoxProbe->clear();  //当前选中探头控件
			comboBoxProbe->addItem(tr("探头2"));
			comboBoxProbe->setCurrentIndex(0);
			m_qsProbe = tr("探头2");
		}
		else if(i == 2)
		{
			checkBoxProbe1->setChecked(false);
			checkBoxProbe2->setChecked(false);
			checkBoxProbe3->setChecked(true);
			g_dwActiveChannel = m_wMapChannelNum;
			m_ndwActiveNum = g_dwActiveChannel;
			g_nSetOneChannel = g_dwActiveChannel;
			//更新第二个下拉框
			comboBoxProbe->clear();  //当前选中探头控件
			comboBoxProbe->addItem(tr("探头3"));
			comboBoxProbe->setCurrentIndex(0);
			m_qsProbe = tr("探头3");
		}
		//更新MAP
		m_MapCPicData.clear();
		CPicData* pCPicData = new CPicData;
		m_MapCPicData.insert(pair<WORD,CPicData*>(m_wMapChannelNum, pCPicData));
		//更新界面
		UpdateActiveChannelInfo(m_wMapChannelNum); //更改探头后，更新和探头相关的控件数据
        //将进入线程的setChannel
		if (g_bStart)
		{
			m_bSelChannel=true;
			m_bSetOne = false;
		}
	}
}

//点击勾选框 “探头1” 调用此函数
void AScan::checkBoxProbe1Change()	//探头1开关函数
{
	//chenyuzhu add start on 2014-8-11//默认选中闸门一
	m_bGateIIsPressed = true;
	m_bGateI2IsPressed = false;
	//chenyuzhu add end on 2014-8-11
	CheckBoxProbeChange(0);	
}

//点击勾选框 “探头2” 调用此函数
void AScan::checkBoxProbe2Change()	//探头2开关函数
{
	//chenyuzhu add start on 2014-8-11//默认选中闸门一
	m_bGateIIsPressed = true;
	m_bGateI2IsPressed = false;
	//chenyuzhu add end on 2014-8-11
	CheckBoxProbeChange(1);
}
//点击勾选框 “探头3” 调用此函数
void AScan::checkBoxProbe3Change()	//探头3开关函数
{
	//chenyuzhu add start on 2014-8-11//默认选中闸门一
	m_bGateIIsPressed = true;
	m_bGateI2IsPressed = false;
	//chenyuzhu add end on 2014-8-11
	CheckBoxProbeChange(2);
}

void AScan::radioButtonFBChange()			//伤波底波切换
{
	if (radioButtonFlawEcho->isChecked())
	{
		g_dwActiveFlawOrBottom = 0;  //伤波
	//	g_nSetOneChannel = m_ndwActiveNum;
		SelChannel(m_ndwActiveNum + 256 * 0);
	}
	else if (radioButtonBottomEcho->isChecked())
	{
		g_dwActiveFlawOrBottom = 1;  //低波
		SelChannel(m_ndwActiveNum + 256 * 1);
	}
}

//移动闸门前沿，前沿值发生变化，同时I闸门的宽度也变化
void AScan::GateFrontChange()	//闸门前沿变量跟踪滑块变化函数
{
	m_fGateFrontOut=(float)spinBoxGateFront->value();
	m_fGateFrontIn = m_fGateFrontOut;
	if (m_bGateIIsPressed)
	{
		m_fIGateFront = m_fGateFrontOut;
		allCH_CONFIG[g_nSetOneChannel].CH_FRONTTIME = Distance2Data(m_fIGateFront, VSOUND);//闸门前沿
	}
	else if (m_bGateI2IsPressed)
	{
		m_fI2GateFront = m_fGateFrontOut;
		//更新闸门一的宽度
		m_fGateWidthIn = m_fI2GateFront - m_fIGateFront;
		allCH_CONFIG[g_nSetOneChannel].CH_REARTIME = Distance2Data(m_fI2GateFront, VSOUND);//闸门hou沿
	}
	if (g_bStart)
	{
		m_bSetOne=true;
	}
}
//移动闸门宽度，闸门宽度发生变化，同时I闸门的后沿发生变化
void AScan::GateWidthChange()	       //闸门宽度变量跟踪滑块变化函数
{
	m_fGateWidthOut=(float)spinBoxGateWidth->value();
	m_fGateWidthIn=m_fGateWidthOut;
	if (m_bGateIIsPressed)  //闸门1宽度变化,更新闸门2的前沿
	{
		m_fIGateWidth = m_fGateWidthOut; //闸门一的宽度变化
		//更新闸门2的前沿
		m_fI2GateFront = m_fIGateFront + m_fIGateWidth;
		allCH_CONFIG[g_nSetOneChannel].CH_REARTIME = Range2Dot(VSOUND, static_cast<float>(m_fIGateFront + m_fIGateWidth)) + 0.5;//闸门后沿
	}
	else if (m_bGateI2IsPressed) //I2的宽度变化,更新I2的后沿
	{
		m_fI2GateWidth = m_fGateWidthOut;
	    allCH_CONFIG[g_nSetOneChannel].CH_MAXBACKTIME = Range2Dot(VSOUND, static_cast<float>(m_fI2GateFront + m_fI2GateWidth)) + 0.5;//闸门后沿
	}

	if (g_bStart)
	{
		m_bSetOne = true;
	}
}

void AScan::GatePeakChange()		//闸门阈值变量跟踪滑块变化函数
{
	m_fGatePeakOut = (float)spinBoxGatePeak->value();
	m_fGatePeakIn = m_fGatePeakOut;

	if (g_bStart)
	{
		m_bSetOne=true;
	}
}

void AScan::DelayChange()		//延迟变量跟踪滑块变化函数
{
	m_fDelayOut = (float)spinBoxDelay->value();
	m_fDelay = m_fDelayOut;
	allCH_CONFIG[g_dwActiveChannel].CH_ZEROCORRECT = m_fDelay * g_nADCFreq * 0.000001; //chenyuzhu add on 2014-10-10
	int temp = m_fDelay * g_nADCFreq * 0.000001; //chenyuzhu add on 2014-10-10

	if (g_bStart)
	{
		WaveDelay(g_pAECWorkThread[g_dwActiveChannel / MAX_CARD_CHANNEL_NO]->m_pAecThreadParam.hDev, temp);
		m_bSetOne = true;
	}
	update();
}

void AScan::RangeChange()		//X轴范围变量跟踪滑块变化函数
{
 	m_fRangeOut = (float)spinBoxXRange->value();
	m_fRange = m_fRangeOut;        //控件变量
	m_fXMaxRuler = m_fRange + m_fXMinRuler;  
	if (g_bStart)
	{
		float nRange = m_fRange - Dot2Range(VSOUND, MONITOR_SIZE);
		WORD nInterSpace[FLAW_DOT_MAX_NO];
		RangeProcess(VSOUND, nRange, nInterSpace);
		SetMonitorRange(g_pAECWorkThread[g_dwActiveChannel / MAX_CARD_CHANNEL_NO]->m_pAecThreadParam.hDev, nInterSpace);
		m_bSetOne=true;
	}
	update();
}

/*void AScan::GainChange()	//增益变量跟踪滑块变化函数
{
	m_fGainOut=(float)spinBoxGain->value();
	m_fGain = m_fGainOut;
	float gain = m_fGain * g_GainSale[g_dwActiveChannel];	
//	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C =  Gain2Data(gain/ 2.0);//板探
//	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D =  Gain2Data(gain/ 2.0);//板探	

	WORD wGainData = Gain2Data(gain);
	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C = wGainData / 2;
	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D = wGainData - wGainData / 2;

	if (g_bStart)
	{
		m_bSetOne=true;
	}
}
*/

void AScan::GainChange()	//增益变量跟踪滑块变化函数
{
	m_fGainOut=(float)spinBoxGain->value();
	m_fGain = m_fGainOut;
	float gain = m_fGain * g_GainSale[g_dwActiveChannel];	
	//	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C =  Gain2Data(gain/ 2.0);//板探
	//	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D =  Gain2Data(gain/ 2.0);//板探	

	float gain_c,gain_d;
	float nOffset = 2.1f;
	float nSection = 29.7f;

	if( gain < nSection)
	{
		gain_c = gain + nOffset;
		gain_d = nOffset;
	}
	else
	{
		gain_c = nSection + nOffset;
		gain_d = gain - nSection + nOffset;
	}

	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C = Gain2Data(gain_c);
	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D = Gain2Data(gain_d);

	// 考虑一下上面这种模式下如何保证不出现2次阶段误差

	//WORD wGainData = Gain2Data(gain);
	//allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C = wGainData;
	//allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D = wGainData - wGainData / 2;

	if (g_bStart)
	{
		m_bSetOne=true;
	}
}


void AScan::SuppressionChange()	//抑制变量跟踪滑块变化函数
{
	m_fSuppressionOut = (float)spinBoxSuppression->value();
	m_fSuppression = m_fSuppressionOut;
    //allCH_CONFIG[g_dwActiveChannel].CH_REJECT_VALUE = m_fSuppression / 100.0 * MAX_HEIGHT;
	allCH_CONFIG[g_nSetOneChannel].CH_REJECT_VALUE = m_fSuppression * 512.0f/100.0f;
	if (g_bStart)
	{
		m_bSetOne=true;
	}
}

/*各滑块变量更新函数*/
void AScan::AscanInfoUpdate()
{	
	// wangwei modified start on 2016-01-18
	if (qFuzzyCompare(m_fGateFrontOut, m_fGateFrontIn))//闸门前沿
	{
		m_fGateFrontOut = m_fGateFrontIn;
		spinBoxGateFront->setValue((double)m_fGateFrontOut);
	}
	if (qFuzzyCompare(m_fGateWidthOut, m_fGateWidthIn))//闸门宽度
	{
		m_fGateWidthOut = m_fGateWidthIn;
		spinBoxGateWidth->setValue((double)m_fGateWidthOut);
	}	
	if (qFuzzyCompare(m_fGatePeakOut, m_fGatePeakIn))//阈值
	{
		m_fGatePeakOut=m_fGatePeakIn;
		spinBoxGatePeak->setValue((double)m_fGatePeakOut);
	}
	if (qFuzzyCompare(m_fDelayOut, m_fDelay))//板探---延迟
	{
		m_fDelayOut = m_fDelay;
		spinBoxDelay->setValue((double)(m_fDelayOut/10));
	}
    if (qFuzzyCompare(m_fRangeOut, m_fRange))//范围xxxxx
	{
		m_fRangeOut = m_fRange;
		spinBoxXRange->setValue((double)m_fRangeOut);
	}
	if (qFuzzyCompare(m_fGainOut, m_fGain))// 增益
	{
		m_fGainOut = m_fGain;
		spinBoxGain->setValue(m_fGainOut);
	}
	if (qFuzzyCompare(m_fSuppressionOut, m_fSuppression))//抑制
	{
		m_fSuppressionOut = m_fSuppression;
		spinBoxSuppression->setValue(m_fSuppressionOut);
	}
	// wangwei modified end on 2016-01-18
}

void AScan::slotSlider_SpinboxGateFront()
{
	spinBoxGateFront->setValue((double)(horizontalSliderGateFront->value()) * 0.1);//chenyuzhu amend on 2014.5.15
}

void AScan::slotSpinbox_SliderGateFront()
{
	horizontalSliderGateFront->setValue((int)(spinBoxGateFront->value()*10));//chenyuzhu amend on 2014.5.15
}

void AScan::slotSlider_SpinboxGateWidth()
{
	spinBoxGateWidth->setValue((double)(horizontalSliderGateWidth->value()) * 0.1);
}

void AScan::slotSpinbox_SliderGateWidth()
{
	horizontalSliderGateWidth->setValue((int)(spinBoxGateWidth->value() * 10));
}

void AScan::slotSlider_SpinboxGatePeak()
{
	spinBoxGatePeak->setValue((double)(horizontalSliderGatePeak->value()) * 0.001);
}

void AScan::slotSpinbox_SliderGatePeak()
{
	horizontalSliderGatePeak->setValue((int)(spinBoxGatePeak->value() * 1000));
}

void AScan::slotSlider_SpinboxDelay()
{
	spinBoxDelay->setValue((double)(horizontalSliderDelay->value()) * 0.01);//chenyuzhu amend on 2014.5.15
}

void AScan::slotSpinbox_SliderDelay()//板探
{
	horizontalSliderDelay->setValue((int)(spinBoxDelay->value() * 100 + 0.001));//chenyuzhu amend on 2014.5.15
}

void AScan::slotSlider_SpinboxXRange()
{
	spinBoxXRange->setValue(static_cast<double>(horizontalSliderXRange->value()) * 0.1);//chenyuzhu amend on 2014.5.15
}

void AScan::slotSpinbox_SliderXRange()
{
	horizontalSliderXRange->setValue(static_cast<int>(spinBoxXRange->value() * 10));//chenyuzhu amend on 2014.5.15
}

void AScan::slotSlider_SpinboxSuppression()//抑制
{
	spinBoxSuppression->setValue(static_cast<double>(verticalSliderSuppression->value()) * 0.1);//chenyuzhu amend on 2014.5.15
}

void AScan::slotSpinbox_SliderSuppression()
{
	verticalSliderSuppression->setValue(static_cast<int>(spinBoxSuppression->value() * 10));//chenyuzhu amend on 2014.5.15
}

void AScan::slotSlider_SpinboxGain()//增益
{
	spinBoxGain->setValue((double)(verticalSliderGain->value()) * 0.1);//chenyuzhu amend on 2014.5.15
}

void AScan::slotSpinbox_SliderGain()// 增益
{
	verticalSliderGain->setValue((int)(spinBoxGain->value() * 10));//chenyuzhu amend on 2014.5.15
}

//I闸门绘制函数
void AScan::GateIPainter(QPainter &painter)
{
	painter.setPen(QColor(255,0,0));
	painter.drawLine(m_fGateILeft, m_fGateITop, m_fGateILeft, m_fGateIBottom);
	painter.drawLine(m_fGateILeft, m_fGateIMiddle, m_fGateIRight, m_fGateIMiddle);
	painter.drawLine(m_fGateIRight, m_fGateITop, m_fGateIRight, m_fGateIBottom);

	if (m_bGateIIsPressed == true)
	{
		m_bGateI2FirstPressed = false;
		painter.setBrush(QColor(0,0,255));
		QRect rectLeft(m_fGateILeft-3,m_fGateIMiddle-2,5,5);
		QRect rectRight(m_fGateIRight-3,m_fGateIMiddle-2,5,5);
		painter.drawRect(rectLeft);
		painter.drawRect(rectRight);

		m_fGateFrontIn=(double)(m_fGateILeft - m_rectWave.left()) * m_fRangeOut / m_rectWave.width() + m_fXMinRuler;
		m_fGateWidthIn=(double)(m_fGateIRight - m_rectWave.left()) * m_fRangeOut / m_rectWave.width() + m_fXMinRuler - m_fGateFrontIn;
		m_fGatePeakIn=(double)(m_rectWave.bottom() - m_fGateIMiddle) * 110 / m_rectWave.height();

		m_fIGateFront=m_fGateFrontIn;
		m_fIGateWidth=m_fGateWidthIn;
		m_fIGatePeak=m_fGatePeakIn;

		if (m_bGateIFirstPressed)
		{
			m_bGateIFirstPressed = false;
			comboBoxGate->setCurrentIndex(0);
			m_ncomboBoxGate=0;
			labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));
			if (m_bGateI==0)
			{
				checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			}
			else if (m_bGateI==1)
			{
				checkBoxGateSwitch->setCheckState(Qt::Checked);
			}
		}

		

		//emit AscanInfoChanged();
	}
}

void AScan::GateI2Painter(QPainter &painter)
{
	painter.setPen(QColor(255,255,0));
	painter.drawLine(m_fGateI2Left,m_fGateI2Top,m_fGateI2Left,m_fGateI2Bottom);
	painter.drawLine(m_fGateI2Left,m_fGateI2Middle,m_fGateI2Right,m_fGateI2Middle);
	painter.drawLine(m_fGateI2Right,m_fGateI2Top,m_fGateI2Right,m_fGateI2Bottom);

	if (m_bGateI2IsPressed)
	{
		painter.setBrush(QColor(255,0,0));
		QRect rectLeft(m_fGateI2Left-3,m_fGateI2Middle-2,5,5);
		QRect rectRight(m_fGateI2Right-3,m_fGateI2Middle-2,5,5);
		painter.drawRect(rectLeft);
		painter.drawRect(rectRight);
		m_fGateFrontIn=(double)(m_fGateI2Left-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler;
		m_fGateWidthIn=(double)((m_fGateI2Right-m_rectWave.left())*m_fRangeOut)/m_rectWave.width()+m_fXMinRuler-m_fGateFrontIn;
		m_fGatePeakIn=((double)(m_rectWave.bottom()-m_fGateI2Middle))*110/m_rectWave.height();

		m_fI2GateFront=m_fGateFrontOut;
		m_fI2GateWidth=m_fGateWidthOut;
		m_fI2GatePeak=m_fGatePeakOut;

		comboBoxGate->setCurrentIndex(1); //chenyuzhu modify on 2014-11-2
		m_ncomboBoxGate=1;//chenyuzhu modify on 2014-11-2
		labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
		if (m_bGateI2==0)
		{
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
		}
		else if (m_bGateI2==1)
		{
			checkBoxGateSwitch->setCheckState(Qt::Checked);
		}

		//emit AscanInfoChanged();
	}
}

void AScan::GateMoveUpdate()
{
	if (m_bGateMove)
	{	
		m_rectGateI.setRect(m_fGateILeft,m_fGateITop,m_fGateIRight-m_fGateILeft,m_fGateIBottom-m_fGateITop);
		if (!m_bGateIIsPresse)
		{
 			m_fGateILeft=m_rectWave.left()+(m_fIGateFront-m_fXMinRuler)*m_rectWave.width() / m_fRangeOut;
			m_fGateIRight=m_rectWave.left()+(((m_fIGateWidth+m_fIGateFront-m_fXMinRuler)*m_rectWave.width())) / m_fRangeOut;
			m_fGateIMiddle=m_rectWave.bottom()-m_fIGatePeak*m_rectWave.height() / 110;
			m_fGateITop=m_fGateIMiddle-m_rectWave.height() * 0.01;
			m_fGateIBottom=m_fGateIMiddle+m_rectWave.height() * 0.01;
			m_pGateILeftTop=QPoint(m_fGateILeft,m_fGateITop);
			m_pGateIRightBottom=QPoint(m_fGateIRight,m_fGateIBottom);
		}	
		m_rectGateI2.setRect(m_fGateI2Left,m_fGateI2Top,m_fGateI2Right-m_fGateI2Left,m_fGateI2Bottom-m_fGateI2Top);
		if (!m_bGateI2IsPresse)
		{
  			m_fGateI2Left=m_rectWave.left( )+(m_fI2GateFront-m_fXMinRuler)*m_rectWave.width()/m_fRangeOut;
			m_fGateI2Right=m_rectWave.left()+(((m_fI2GateWidth+m_fI2GateFront-m_fXMinRuler)*m_rectWave.width()))/m_fRangeOut;
			m_fGateI2Middle=m_rectWave.bottom()-m_fI2GatePeak*m_rectWave.height()/110;
			m_fGateI2Top=m_fGateI2Middle-m_rectWave.height()*0.01;
			m_fGateI2Bottom=m_fGateI2Middle+m_rectWave.height()*0.01;
			m_pGateI2LeftTop=QPoint(m_fGateI2Left,m_fGateI2Top);
			m_pGateI2RightBottom=QPoint(m_fGateI2Right,m_fGateI2Bottom);
		}
	}
}
//将从数据库中取得值更新到界面
void AScan::UpdateActiveChannelInfo(WORD wActiveChannel)
{
	//I1闸门前沿
	m_fGateFrontIn = Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_FRONTTIME);
	horizontalSliderGateFront->setRange(0,static_cast<int>(g_Info[nPlate].height*1.2*10));//chenyuzhu modify on 2014.5.15
	spinBoxGateFront->setValue(m_fGateFrontIn);
	m_fIGateFront = m_fGateFrontIn;
	//I2闸门的前沿
	m_fI2GateFront = Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_REARTIME);
	//I1闸门宽度 
	//m_fGateWidthIn = m_fI2GateFront - m_fGateFrontIn;	
	m_fIGateWidth = Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_REARTIME) - Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_FRONTTIME);
	spinBoxGateWidth->setValue(m_fIGateWidth);
	//底波闸门后沿------------------//最大底波位置	
	//m_fGateI2backIn = Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_MAXBACKTIME);
	//闸门宽度I2闸门
		m_fI2GateWidth = Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_MAXBACKTIME) - Dot2Range(VSOUND, allCH_CONFIG[g_dwActiveChannel].CH_REARTIME);
	//阈值
	m_fGatePeakIn = 50;
	m_fIGatePeak = 50;
	m_fI2GatePeak = 50;
	spinBoxGatePeak->setValue(m_fGatePeakIn);
	// 增益
	m_fGain = (Data2Gain(allCH_CONFIG[wActiveChannel].CH_GAIN_C + allCH_CONFIG[wActiveChannel].CH_GAIN_D) - 4.2) / g_GainSale[wActiveChannel];
	spinBoxGain->setValue(m_fGain);
	//抑制
	m_fSuppression = allCH_CONFIG[wActiveChannel].CH_REJECT_VALUE;
	spinBoxSuppression->setValue(m_fSuppression);
	//延迟-------保留小数点后一位
	//范围
	m_fRange = g_Info[0].height * 1.5;
	if (m_fRange < 30)
	{
		m_fRange = 30;
	}
	//m_fDelay = static_cast<float>((static_cast<int>(Dot2Range(VSOUND, allCH_CONFIG[wActiveChannel].CH_ZEROCORRECT)*10+0.5))/10.0f);
//	m_fDelayOut= Dot2Range(VSOUND,allCH_CONFIG[wActiveChannel].CH_ZEROCORRECT); //延迟
	m_fDelayOut = (float)allCH_CONFIG[wActiveChannel].CH_ZEROCORRECT / ((float)g_nADCFreq * 0.000001);//chenyuzhu modify on 2014-10-10
	spinBoxDelay->setValue(m_fDelayOut);
	spinBoxZhongZhi->setValue(allCH_CONFIG[wActiveChannel].MIDDLE_VALUE);
	spinBoxZengYiXiShu->setValue(g_GainSale[wActiveChannel]);
	//将变量的值置向各控件与之关联的编辑框
	emit AscanInfoChanged();   //更新另外一些控件的值
}

void AScan::timerEvent( QTimerEvent *event )
{	
	/*
	if(event->timerId() == m_nTIMER_Sel_Channel)//下发当前选中探头盒中选中探头的数据
	{
		if (m_wMapCurrentChannelNum >= m_MapCPicData.size() )
		{
			m_wMapCurrentChannelNum = 0;
		}

		map<WORD,CPicData*>::iterator iter;
		int i = 0;
		QVector<ST_CHANEL> temp;
		temp = m_mapChanel[m_nProbeBox];
		for (iter = m_MapCPicData.begin(); iter != m_MapCPicData.end(); iter++)
		{			
			if (i == m_wMapCurrentChannelNum && temp[i].isSelect)
			{
				g_dwActiveChannel = iter->first;
				m_bSelChannel = TRUE;
				break;
			}
			i++;
		}
		m_wMapCurrentChannelNum++;
	}
	*/
}

//此函数是更改探头下拉框的选项，添加删除勾选的探头选项
void AScan::UpdateProbe()      //探头合变化更新当前选中探头
{
	comboBoxProbe->clear();  //当前选中探头控件
	if (m_mapChanel[m_nProbeBox].value(0).isSelect)
	{
		comboBoxProbe->addItem(tr("探头1"));
	}
	if (m_mapChanel[m_nProbeBox].value(1).isSelect)
	{
		comboBoxProbe->addItem(tr("探头2"));
	}
	if (m_mapChanel[m_nProbeBox].value(2).isSelect)
	{
		comboBoxProbe->addItem(tr("探头3"));
	}
}
//因为板探有两个地方可以切换通道。即可以在A扫中下拉列表框切换，又可以在主界面通道切换区切换

void AScan::CurSelectProbe()//选则当前选中的探头（用于通过界面修改数据）后，记录此探头是1还是2还是3
{
	int nCurrentChannel = -1;
	if (comboBoxProbe->currentText() == tr("探头1"))
	{
		nCurrentChannel = m_mapChanel[m_nProbeBox].value(0).PhysicalNo;//g_dwActiveChannel存的是真正的物理通道号，并不连续
		m_qsProbe = tr("探头1");
	}
	else if (comboBoxProbe->currentText() == tr("探头2"))
	{
		nCurrentChannel = m_mapChanel[m_nProbeBox].value(1).PhysicalNo;//g_dwActiveChannel存的是真正的物理通道号，并不连续
		m_qsProbe = tr("探头2");
	}
	else if (comboBoxProbe->currentText() == tr("探头3"))
	{
		nCurrentChannel = m_mapChanel[m_nProbeBox].value(2).PhysicalNo;//g_dwActiveChannel存的是真正的物理通道号，并不连续
		m_qsProbe = tr("探头3");
	}
	else   
	{
		m_qsProbe = "";
		return;
	}

	UpdateActiveChannelInfo(nCurrentChannel); //更改探头后，更新和探头相关的控件数据
}


//
void AScan::ProbeRemember()
{


	
	int n = comboBoxProbe->count();
	for (int i = 0;i < n;i++)
	{
		comboBoxProbe->setCurrentIndex(i);
		if (m_qsProbe == comboBoxProbe->currentText())
		{
			break;
		}
		else if (i == n-1)
		{
			comboBoxProbe->setCurrentIndex(0);
			m_qsProbe = comboBoxProbe->currentText();
			CurSelectProbe(); //切换探头时调用的函数，更改m_qsProbe
		}
	}
	if (n == 0)
	{
		CurSelectProbe();
	}
	
}

//chenyuzhu add start on 2014-11-2
void AScan::GateBoxChange(int index)
{
	if(0 == index)
	{
		m_bGateIIsPressed = true;
		m_bGateI2IsPressed = false;
	}
	else
	{
		m_bGateIIsPressed = false;
		m_bGateI2IsPressed = true;
	}
	update();
}
//chenyuzhu add end on 2014-11-2

void AScan::slotSpinBox_ZhongZhi()
{
	allCH_CONFIG[g_dwActiveChannel].MIDDLE_VALUE = (int)spinBoxZhongZhi->value();
	if (g_bStart)
	{
		m_bSetOne=true;
	}
	update();
}
void AScan::slotSpinBox_ZhengYiXiShu()
{
	g_GainSale[g_dwActiveChannel] = (double)spinBoxZengYiXiShu->value();
	m_fGainOut=(float)spinBoxGain->value();
	m_fGain = m_fGainOut;
	float gain = m_fGain * g_GainSale[g_dwActiveChannel];
	float gain_c,gain_d;
	float nOffset = 2.1f;
	float nSection = 29.7f;

	if( gain < nSection)
	{
		gain_c = gain + nOffset;
		gain_d = nOffset;
	}
	else
	{
		gain_c = nSection + nOffset;
		gain_d = gain - nSection + nOffset;
	}

	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_C = Gain2Data(gain_c);
	allCH_CONFIG[g_nSetOneChannel].CH_GAIN_D = Gain2Data(gain_d);
	if (g_bStart)
	{
		m_bSetOne=true;
	}
}