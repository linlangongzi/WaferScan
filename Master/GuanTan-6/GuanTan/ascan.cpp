#include "ascan.h"
#include "ui_ascan.h"
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QObject>
#include <QMouseEvent>
#include "GlobalFunction.h"
#include <map>
#include <QMutex>
const int A_SCAN_MAX_RANGE = 1000;
const int GATE_MAX_RANGE = 1000;//最大范围值
const int GATE_PEAK_MAX_RANGE = 100;
extern DWORD g_dwActiveChannel;//DEFAULT_CHANNEL;
extern DWORD g_dwActiveFlawOrBottom;
map<WORD,CPicData*> m_MapCPicData;
CH_FLAWDATA_CSCAN_D g_CH_FLAWDATA_CSCAN_D;
QVector<int> g_vecADataIndex;
QMutex g_ADataMutex;

AScan::AScan(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	InitChannelMap();
	g_vecADataIndex.clear();
	m_MapCPicData.clear();
	for (int i = 0; i < 36; i++)
	{
		CPicData* pCPicData = NULL;
		while (!pCPicData) {
			pCPicData = new CPicData;
		}
	
		m_MapCPicData[i] = pCPicData;
	}

	m_wChannelNo = 0;
	m_nWaveSpeed = WAVESPEED_STEEL;
	m_nLines = 10;
	m_fXMinRuler = 0;
	m_fXMaxRuler = 100.0f;
	setupUi(this);
	setMouseTracking(true);
	UiInit();
	DataMemberInit();
	setWindowModality(Qt::WindowModal);
	widget_3->setMouseTracking(true);
	widget->setMouseTracking(true);
	widget_4->setMouseTracking(true);
	verticalSliderGain->setMouseTracking(true);
	label_10->setMouseTracking(true);
	memset(m_nGrid,0,sizeof(m_nGrid));
	for(int i=0; i<CHANNEL_NUM;i++) // 1->channel_Num
	{
		m_stAllCH_CONFIG[i].stPlus.wAPlus = Gain2Data(20);
		m_stAllCH_CONFIG[i].stPlus.wBPlus = Gain2Data(20);
		m_stAllCH_CONFIG[i].stPlus.wCPlus = Gain2Data(0);
		m_stAllCH_CONFIG[i].stPlus.wDPlus = Gain2Data(0);
		m_stAllCH_CONFIG[i].wBottomFlawPlusDiff = 0;//Gain2Data(20);
		m_stAllCH_CONFIG[i].wTCGEn = 0;
		for (int j=0; j<2; j++)
		{
			for (int k=0; k<4; k++)
			{
				m_stAllCH_CONFIG[i].stWaveConfigure[j].stGate[k].wGateEn = 1;
				m_stAllCH_CONFIG[i].stWaveConfigure[j].stGate[k].wGateStart = 750;
				m_stAllCH_CONFIG[i].stWaveConfigure[j].stGate[k].wGateWidth = 1050;
				m_stAllCH_CONFIG[i].stWaveConfigure[j].stGate[k].wGateHeight = 60+k*20;
			}
			m_stAllCH_CONFIG[i].stWaveConfigure[j].wDelay = 0;
			m_stAllCH_CONFIG[i].stWaveConfigure[j].wMiddleValue =490; // 511;
			m_stAllCH_CONFIG[i].stWaveConfigure[j].wDetectMode = 0;
			m_stAllCH_CONFIG[i].stWaveConfigure[j].wSuppression = 0;
		}
	}
	emit AscanInfoChanged();
	InitChannelInfo();
	SetGateInfo();
	SetProbeInfo();
	g_vecADataIndex.clear();
	g_vecADataIndex.push_back(0);
	m_nTIMER_Sel_Channel = startTimer(50);//加定时器
	m_bSelChannel = TRUE;
}

AScan::~AScan()
{
	if ( m_nTIMER_Sel_Channel != 0 )
		killTimer(m_nTIMER_Sel_Channel);

	g_vecADataIndex.clear();

	for (map<WORD,CPicData*>::iterator it = m_MapCPicData.begin(); it != m_MapCPicData.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
}

void AScan::mousePressEvent(QMouseEvent *e)
{
	if (e->button()==Qt::LeftButton)
	{
		QPoint pos = e->pos();
		//闸门I
		if (pos.x()>m_pGateILeftTop.x()+3 && pos.y()>m_pGateILeftTop.y() && pos.x()<m_pGateIRightBottom.x()-3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateIIsPressed = true;
			m_bGateIIsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateILeftTop.x()-3 && pos.y()>m_pGateILeftTop.y() && pos.x()<=m_pGateILeftTop.x()+3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateIIsPressed = true;
			m_bGateILIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateIRightBottom.x()-3 && pos.y()>m_pGateILeftTop.y() && pos.x()<=m_pGateIRightBottom.x()+3 && pos.y()<m_pGateIRightBottom.y()&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
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
		//闸门A
		if (pos.x()>m_pGateALeftTop.x()+3 && pos.y()>m_pGateALeftTop.y() && pos.x()<m_pGateARightBottom.x()-3 && pos.y()<m_pGateARightBottom.y()&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateAIsPressed = true;
			m_bGateAIsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateALeftTop.x()-3 && pos.y()>m_pGateALeftTop.y() && pos.x()<=m_pGateALeftTop.x()+3 && pos.y()<m_pGateARightBottom.y()&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateAIsPressed = true;
			m_bGateALIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateARightBottom.x()-3 && pos.y()>m_pGateALeftTop.y() && pos.x()<=m_pGateARightBottom.x()+3 && pos.y()<m_pGateARightBottom.y()&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateAIsPressed = true;
			m_bGateARIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=false;
			m_bGateRight=true;
		}
		else
		{
			m_bGateAIsPressed = false;
			update();
		}
		if (m_bGateAIsPresse||m_bGateALIsPresse||m_bGateARIsPresse)
			startPnt = e->pos();
		//闸门B
		if (pos.x()>m_pGateBLeftTop.x()+3 && pos.y()>m_pGateBLeftTop.y() && pos.x()<m_pGateBRightBottom.x()-3 && pos.y()<m_pGateBRightBottom.y()&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateBIsPressed = true;
			m_bGateBIsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateBLeftTop.x()-3 && pos.y()>m_pGateBLeftTop.y() && pos.x()<=m_pGateBLeftTop.x()+3 && pos.y()<m_pGateBRightBottom.y()&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateBIsPressed = true;
			m_bGateBLIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateBRightBottom.x()-3 && pos.y()>m_pGateBLeftTop.y() && pos.x()<=m_pGateBRightBottom.x()+3 && pos.y()<m_pGateBRightBottom.y()&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateBIsPressed = true;
			m_bGateBRIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=false;
			m_bGateRight=true;
		}
		else
		{
			m_bGateBIsPressed = false;
			update();
		}
		if (m_bGateBIsPresse||m_bGateBLIsPresse||m_bGateBRIsPresse)
			startPnt = e->pos();
		//闸门I2
		if (pos.x()>m_pGateI2LeftTop.x()+3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<m_pGateI2RightBottom.x()-3 && pos.y()<m_pGateI2RightBottom.y()&&(m_bGateBIsPressed == false)&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateI2IsPressed = true;
			m_bGateI2IsPresse = true;
			m_bGateMove=true;
			m_bGateLeft=false;
			m_bGateRight=false;
			setCursor(Qt::ClosedHandCursor);
		}
		else if (pos.x()>=m_pGateI2LeftTop.x()-3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<=m_pGateI2LeftTop.x()+3 && pos.y()<m_pGateI2RightBottom.y()&&(m_bGateBIsPressed == false)&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
		{
			m_bGateI2IsPressed = true;
			m_bGateI2LIsPresse = true;
			m_bGateMove=false;
			m_bGateLeft=true;
			m_bGateRight=false;
		}
		else if (pos.x()>=m_pGateI2RightBottom.x()-3 && pos.y()>m_pGateI2LeftTop.y() && pos.x()<=m_pGateI2RightBottom.x()+3 && pos.y()<m_pGateI2RightBottom.y()&&(m_bGateBIsPressed == false)&&(m_bGateAIsPressed == false)&&(m_bGateIIsPressed == false)&&pos.x()>=m_rectWave.left()&&pos.x()<=m_rectWave.right())
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
	if (!(m_bGateIIsPresse||m_bGateILIsPresse||m_bGateIRIsPresse||m_bGateAIsPresse||m_bGateALIsPresse
		||m_bGateARIsPresse||m_bGateBIsPresse||m_bGateBLIsPresse||m_bGateBRIsPresse||m_bGateI2IsPresse
		||m_bGateI2LIsPresse||m_bGateI2RIsPresse))
	{
		if ((e->pos().x()>m_pGateILeftTop.x()+3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<m_pGateIRightBottom.x()-3 && e->pos().y()<m_pGateIRightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right())
			||(e->pos().x()>m_pGateALeftTop.x()+3 && e->pos().y()>m_pGateALeftTop.y() && e->pos().x()<m_pGateARightBottom.x()-3 && e->pos().y()<m_pGateARightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right())
			||(e->pos().x()>m_pGateBLeftTop.x()+3 && e->pos().y()>m_pGateBLeftTop.y() && e->pos().x()<m_pGateBRightBottom.x()-3 && e->pos().y()<m_pGateBRightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right())
			||(e->pos().x()>m_pGateI2LeftTop.x()+3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<m_pGateI2RightBottom.x()-3 && e->pos().y()<m_pGateI2RightBottom.y()&&e->pos().x()>=m_rectWave.left()&&e->pos().x()<=m_rectWave.right()))
		{
			setCursor(Qt::OpenHandCursor);
		}
		else if ((e->pos().x()>=m_pGateILeftTop.x()-3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<=m_pGateILeftTop.x()+3 && e->pos().y()<m_pGateIRightBottom.y())
			||(e->pos().x()>=m_pGateALeftTop.x()-3 && e->pos().y()>m_pGateALeftTop.y() && e->pos().x()<=m_pGateALeftTop.x()+3 && e->pos().y()<m_pGateARightBottom.y())
			||(e->pos().x()>=m_pGateBLeftTop.x()-3 && e->pos().y()>m_pGateBLeftTop.y() && e->pos().x()<=m_pGateBLeftTop.x()+3 && e->pos().y()<m_pGateBRightBottom.y())
			||(e->pos().x()>=m_pGateI2LeftTop.x()-3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<=m_pGateI2LeftTop.x()+3 && e->pos().y()<m_pGateI2RightBottom.y()))
		{
			setCursor(Qt::SizeHorCursor);
		}
		else if ((e->pos().x()>=m_pGateIRightBottom.x()-3 && e->pos().y()>m_pGateILeftTop.y() && e->pos().x()<=m_pGateIRightBottom.x()+3 && e->pos().y()<m_pGateIRightBottom.y())
			||(e->pos().x()>=m_pGateARightBottom.x()-3 && e->pos().y()>m_pGateALeftTop.y() && e->pos().x()<=m_pGateARightBottom.x()+3 && e->pos().y()<m_pGateARightBottom.y())
			||(e->pos().x()>=m_pGateBRightBottom.x()-3 && e->pos().y()>m_pGateBLeftTop.y() && e->pos().x()<=m_pGateBRightBottom.x()+3 && e->pos().y()<m_pGateBRightBottom.y())
			||(e->pos().x()>=m_pGateI2RightBottom.x()-3 && e->pos().y()>m_pGateI2LeftTop.y() && e->pos().x()<=m_pGateI2RightBottom.x()+3 && e->pos().y()<m_pGateI2RightBottom.y()))
		{
			setCursor(Qt::SizeHorCursor);
		}
		else if (e->pos().y()<widget->rect().bottom() || e->pos().x()>verticalSliderGain->rect().left() || e->pos().x()>label_10->rect().left())
			setCursor(Qt::ArrowCursor);
		else
			setCursor(Qt::ArrowCursor);
	}
	//I移动
	if (m_bGateIIsPressed&&m_bGateIIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if (((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0 + m_fSync-m_fSync/100) 
			|| ((((m_pGateILeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > GATE_MAX_RANGE + m_fSync-5*m_fSync/100+Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, MicroSecond2Dot(m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateILeftTop + sub).y()+((double)(m_pGateIRightBottom + sub).y()-(double)(m_pGateILeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
			;
		else
		{
		m_pGateILeftTop = m_pGateILeftTop + sub;
		m_pGateIRightBottom = m_pGateIRightBottom + sub;
		}
		m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);
		startPnt=e->pos();
		m_fGateILeft=m_rectGateI.left();
		m_fGateITop=m_rectGateI.top();
		m_fGateIRight=m_rectGateI.right();
		m_fGateIBottom=m_rectGateI.bottom();
		m_fGateIMiddle=m_fGateITop+(m_fGateIBottom-m_fGateITop)/2;
	}
	else if (m_bGateIIsPressed&&m_bGateILIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateILeftTop + sub).x()<=m_pGateIRightBottom.x())
		{
			m_pGateILeftTop = m_pGateILeftTop + sub;
			m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);
			startPnt=e->pos();
			m_fGateILeft=m_rectGateI.left();
		}
	}	
	else if (m_bGateIIsPressed&&m_bGateIRIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateIRightBottom + sub).x()>=m_pGateILeftTop.x())
		{
			m_pGateIRightBottom = m_pGateIRightBottom + sub;
			m_rectGateI = QRect(m_pGateILeftTop,m_pGateIRightBottom);
			startPnt=e->pos();
			m_fGateIRight=m_rectGateI.right();
		}
	}
	if (m_bGateAIsPressed&&m_bGateAIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if (((((m_pGateALeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateALeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > GATE_MAX_RANGE+ m_fSync-5*m_fSync/100+Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, MicroSecond2Dot(m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateALeftTop + sub).y()+((double)(m_pGateARightBottom + sub).y()-(double)(m_pGateALeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateALeftTop + sub).y()+((double)(m_pGateARightBottom + sub).y()-(double)(m_pGateALeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
			;
		else
		{
			m_pGateALeftTop = m_pGateALeftTop + sub;
			m_pGateARightBottom = m_pGateARightBottom + sub;
		}
		m_rectGateA = QRect(m_pGateALeftTop,m_pGateARightBottom);
		startPnt=e->pos();
		m_fGateALeft=m_rectGateA.left();
		m_fGateATop=m_rectGateA.top();
		m_fGateARight=m_rectGateA.right();
		m_fGateABottom=m_rectGateA.bottom();
		m_fGateAMiddle=m_fGateATop+(m_fGateABottom-m_fGateATop)/2;
	}
	else if (m_bGateAIsPressed&&m_bGateALIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateALeftTop + sub).x()<=m_pGateARightBottom.x())
		{
			m_pGateALeftTop = m_pGateALeftTop + sub;
			m_rectGateA = QRect(m_pGateALeftTop,m_pGateARightBottom);
			startPnt=e->pos();
			m_fGateALeft=m_rectGateA.left();
		}
	}	
	else if (m_bGateAIsPressed&&m_bGateARIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateARightBottom + sub).x()>=m_pGateALeftTop.x())
		{
			m_pGateARightBottom = m_pGateARightBottom + sub;
			m_rectGateA = QRect(m_pGateALeftTop,m_pGateARightBottom);
			startPnt=e->pos();
			m_fGateARight=m_rectGateA.right();
		}
	}

	if (m_bGateBIsPressed&&m_bGateBIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if (((((m_pGateBLeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateBLeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) > GATE_MAX_RANGE+ m_fSync-5*m_fSync/100+Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, MicroSecond2Dot(m_fDelay))) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateBLeftTop + sub).y()+((double)(m_pGateBRightBottom + sub).y()-(double)(m_pGateBLeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) < 0) 
			|| (((double)(m_rectWave.bottom()-((double)(m_pGateBLeftTop + sub).y()+((double)(m_pGateBRightBottom + sub).y()-(double)(m_pGateBLeftTop + sub).y())/2))*GATE_PEAK_MAX_RANGE/m_rectWave.height()) > GATE_PEAK_MAX_RANGE))
			;
		else
		{
			m_pGateBLeftTop = m_pGateBLeftTop + sub;
			m_pGateBRightBottom = m_pGateBRightBottom + sub;
		}
		m_rectGateB = QRect(m_pGateBLeftTop,m_pGateBRightBottom);
		startPnt=e->pos();
		m_fGateBLeft=m_rectGateB.left();
		m_fGateBTop=m_rectGateB.top();
		m_fGateBRight=m_rectGateB.right();
		m_fGateBBottom=m_rectGateB.bottom();
		m_fGateBMiddle=m_fGateBTop+(m_fGateBBottom-m_fGateBTop)/2;
	}
	else if (m_bGateBIsPressed&&m_bGateBLIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateBLeftTop + sub).x()<=m_pGateBRightBottom.x())
		{
			m_pGateBLeftTop = m_pGateBLeftTop + sub;
			m_rectGateB = QRect(m_pGateBLeftTop,m_pGateBRightBottom);
			startPnt=e->pos();
			m_fGateBLeft=m_rectGateB.left();
		}
	}	
	else if (m_bGateBIsPressed&&m_bGateBRIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateBRightBottom + sub).x()>=m_pGateBLeftTop.x())
		{
			m_pGateBRightBottom = m_pGateBRightBottom + sub;
			m_rectGateB = QRect(m_pGateBLeftTop,m_pGateBRightBottom);
			startPnt=e->pos();
			m_fGateBRight=m_rectGateB.right();
		}
	}
	if (m_bGateI2IsPressed&&m_bGateI2IsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if (((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) < 0+ m_fSync-m_fSync/100) 
			|| ((((m_pGateI2LeftTop + sub).x()-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler) >  GATE_MAX_RANGE+ m_fSync-5*m_fSync/100+Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, MicroSecond2Dot(m_fDelay))) 
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
		m_fGateI2Top=m_rectGateI2.top();
		m_fGateI2Right=m_rectGateI2.right();
		m_fGateI2Bottom=m_rectGateI2.bottom();
		m_fGateI2Middle=m_fGateI2Top+(m_fGateI2Bottom-m_fGateI2Top)/2;
	}
	else if (m_bGateI2IsPressed&&m_bGateI2LIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateI2LeftTop + sub).x()<=m_pGateI2RightBottom.x())
		{
			m_pGateI2LeftTop = m_pGateI2LeftTop + sub;
			m_rectGateI2 = QRect(m_pGateI2LeftTop,m_pGateI2RightBottom);
			startPnt=e->pos();
			m_fGateI2Left=m_rectGateI2.left();
		}   
	}	
	else if (m_bGateI2IsPressed&&m_bGateI2RIsPresse)
	{
		QPoint sub = e->pos() - startPnt;
		if ((m_pGateI2RightBottom + sub).x()>=m_pGateI2LeftTop.x())
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
	m_bGateIIsPresse=false;
	m_bGateAIsPresse=false;
	m_bGateBIsPresse=false;
	m_bGateI2IsPresse=false;
	m_bGateILIsPresse = false;
	m_bGateIRIsPresse = false;
	m_bGateALIsPresse = false;
	m_bGateARIsPresse = false;
	m_bGateBLIsPresse = false;
	m_bGateBRIsPresse = false;
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
	//获取系统设置的屏幕个数(屏幕拷贝方式该值为1)
	m_nScreenCount = desktopWidget->screenCount();
}

void AScan::keyPressEvent(QKeyEvent *event)//重写esc响应
{
	if (event->key() == Qt::Key_Escape) 
	{  
	}
}

void AScan::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);
	DistributeRegion();
}

void AScan::DataMemberInit()	//成员变量初始化
{
	m_bSelChannel = FALSE;
	m_bSetOne=false;
	m_bSetAll = FALSE;
	m_ncomboBoxGate=0;
	m_nProbeBox=0;
	m_nProbe = 0;
	m_fGateFrontOut=0;
	m_fGateFrontIn=0;
	m_fGateWidthOut=0;
	m_fGateWidthIn=0;
	m_fGatePeakOut=0;
	m_fGatePeakIn=0;
	m_fZeroPointOut=0;
	m_fZeroPoint=0.0;
	m_fDelayOut=0;
	m_fDelay=0.0;
	m_fRangeOut=0;
	m_fRange=0.0;
	m_fReferGainOut=0;
	m_fCGain=0.0;
	m_fReFreqOut=0;
	m_fReFreq=600.0;
	m_fGainOut=0;
	m_fGain=0.0;
	m_fSuppressionOut=0;
	m_fSuppression=0.0;
	m_bGateI=0;
	m_bGateA=0;
	m_bGateB=0;
	m_bGateI2=0;
	m_bGateIIsPressed=true;
	m_fGateILeftUpdate=0.05f;
	m_fGateITopUpdate=0.02f;
	m_bGateAIsPressed=false;
	m_fGateALeftUpdate=0.05f;
	m_fGateATopUpdate=0.05f;
	m_bGateBIsPressed=false;
	m_fGateBLeftUpdate=0.05f;
	m_fGateBTopUpdate=0.08f;
	m_bGateI2IsPressed=false;
	m_fGateI2LeftUpdate=0.05f;
	m_fGateI2TopUpdate=0.11f;
	m_bGateIIsPresse=false;
	m_bGateILIsPresse=false;
	m_bGateIRIsPresse=false;
	m_bGateAIsPresse=false;
	m_bGateALIsPresse=false;
	m_bGateARIsPresse=false;
	m_bGateBIsPresse=false;
	m_bGateBLIsPresse=false;
	m_bGateBRIsPresse=false;
	m_bGateI2IsPresse=false;
	m_bGateI2LIsPresse=false;
	m_bGateI2RIsPresse=false;
	m_bGateMove=true;
	m_bGateLeft=false;
	m_bGateRight=false;
	m_fIGateFront=5;
	m_fIGateWidth=20;
	m_fIGatePeak=5;
	m_fAGateFront=5;
	m_fAGateWidth=20;
	m_fAGatePeak=10;
	m_fBGateFront=5;
	m_fBGateWidth=20;
	m_fBGatePeak=15;
	m_fI2GateFront=5;
	m_fI2GateWidth=20;
	m_fI2GatePeak=20;
	m_nFlawOrBottom=1;
	m_fSync = 0.0;
	m_nCurSelectProbe = 0;
	m_qsProbe = tr("纵伤探头1");
	emit AscanInfoChanged();
}

void AScan::UiInit()	//Ui初始化
{
 	GetScreenInfo();
 	resize(m_nActScreenX-3,m_nActScreenY-60);
	setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);
	comboBoxProbeBox->addItem(tr(" 探头盒6"));
	pushButtonOptionShow->hide();
	checkBoxProbe1->setChecked(true);
	checkBoxGateSwitch->setChecked(true);
	label_25->hide();
	horizontalSliderZeroPoint->hide();
	spinBoxZeroPoint->hide();
	label_26->hide();
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/A_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("A扫"));
	comboBoxProbe->addItem(tr("纵伤探头1"));

	connect(spinBoxGateFront, SIGNAL(valueChanged(double)), this, SLOT(slotSpinbox_SliderGateFront()));  
	connect(horizontalSliderGateFront, SIGNAL(valueChanged(int)), this, SLOT(slotSlider_SpinboxGateFront()));
	connect(spinBoxGateFront,SIGNAL(valueChanged(double)),this,SLOT(GateFrontChange()));
	horizontalSliderGateFront->setRange(0,GATE_MAX_RANGE*1000);
	spinBoxGateFront->setRange(0,(double)GATE_MAX_RANGE);
	spinBoxGateFront->setSingleStep(0.1);

	connect(spinBoxGateWidth,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGateWidth()));
	connect(horizontalSliderGateWidth,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGateWidth()));
	connect(spinBoxGateWidth,SIGNAL(valueChanged(double)),this,SLOT(GateWidthChange()));
	horizontalSliderGateWidth->setRange(0,GATE_MAX_RANGE*1000);
	spinBoxGateWidth->setRange(0,(double)GATE_MAX_RANGE);
	spinBoxGateWidth->setSingleStep(0.1);

	connect(spinBoxGatePeak,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGatePeak()));
	connect(horizontalSliderGatePeak,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGatePeak()));
	connect(spinBoxGatePeak,SIGNAL(valueChanged(double)),this,SLOT(GatePeakChange()));
	horizontalSliderGatePeak->setRange(0,GATE_PEAK_MAX_RANGE*1000);
	spinBoxGatePeak->setRange(0,(double)GATE_PEAK_MAX_RANGE);
	spinBoxGatePeak->setSingleStep(0.1);

	connect(spinBoxZeroPoint,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderZeroPoint()));
	connect(horizontalSliderZeroPoint,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxZeroPoint()));
	connect(spinBoxZeroPoint,SIGNAL(valueChanged(double)),this,SLOT(ZeroPointChange()));
	horizontalSliderZeroPoint->setRange(0,50*1000);
	spinBoxZeroPoint->setRange(0,50);
	spinBoxZeroPoint->setSingleStep(0.1);

	connect(spinBoxDelay,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderDelay()));
	connect(horizontalSliderDelay,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxDelay()));
	connect(spinBoxDelay,SIGNAL(valueChanged(double)),this,SLOT(DelayChange()));
	horizontalSliderDelay->setRange(0,200*100);
	spinBoxDelay->setRange(0,200);
	spinBoxDelay->setSingleStep(0.1);

	connect(spinBoxXRange,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderXRange()));
	connect(horizontalSliderXRange,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxXRange()));
	connect(spinBoxXRange,SIGNAL(valueChanged(double)),this,SLOT(RangeChange()));
	horizontalSliderXRange->setRange(30,A_SCAN_MAX_RANGE);
	spinBoxXRange->setRange(30,(double)A_SCAN_MAX_RANGE);
	spinBoxXRange->setSingleStep(0.1);

	connect(spinBoxYRange,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderYRange()));
	connect(horizontalSliderYRange,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxYRange()));
	connect(spinBoxYRange,SIGNAL(valueChanged(double)),this,SLOT(ReferGainChange()));
	horizontalSliderYRange->setRange(0,60);
	spinBoxYRange->setRange(0,60);
	spinBoxYRange->setSingleStep(0.1);

	connect(spinBoxReFreq,SIGNAL(valueChanged(int)),this,SLOT(slotSpinbox_SliderReFreq()));
	connect(horizontalSliderReFreq,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxReFreq()));
	connect(spinBoxReFreq,SIGNAL(valueChanged(int)),this,SLOT(ReFreqChange()));
	horizontalSliderReFreq->setRange(MIN_RE_FREQ,MAX_RE_FREQ);
	spinBoxReFreq->setRange(MIN_RE_FREQ,MAX_RE_FREQ);
	spinBoxReFreq->setSingleStep(1);

	connect(spinBoxSuppression,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderSuppression()));
	connect(verticalSliderSuppression,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxSuppression()));
	connect(spinBoxSuppression,SIGNAL(valueChanged(double)),this,SLOT(SuppressionChange()));
	verticalSliderSuppression->setRange(0,100*1000);
	spinBoxSuppression->setRange(0,100);
	spinBoxSuppression->setSingleStep(0.1);

	connect(spinBoxGain,SIGNAL(valueChanged(double)),this,SLOT(slotSpinbox_SliderGain()));
	connect(verticalSliderGain,SIGNAL(valueChanged(int)),this,SLOT(slotSlider_SpinboxGain()));
	connect(spinBoxGain,SIGNAL(valueChanged(double)),this,SLOT(GainChange())); 
	verticalSliderGain->setRange(0,80*1000);
	spinBoxGain->setRange(0,80);
	spinBoxGain->setSingleStep(0.1);

	connect(comboBoxGate, SIGNAL(activated(int)), this, SLOT(GateChange()));
	connect(comboBoxProbeBox, SIGNAL(activated(int)), this, SLOT(ProbeBoxChange()));
	connect(comboBoxProbe, SIGNAL(activated(int)), this, SLOT(ProbeChange()));

	connect(pushButtonOptionHide, SIGNAL(clicked()), this, SLOT(toolbar_hide()));
	connect(pushButtonOptionShow, SIGNAL(clicked()), this, SLOT(toolbar_show()));
	connect(pushButtonSaveScreen, SIGNAL(clicked()), this, SLOT(pushButtonSaveScreenRespond()));
	connect(pushButtonRecordVideo, SIGNAL(clicked()), this, SLOT(pushButtonRecordVideoRespond()));
	connect(pushButtonFreezeWave, SIGNAL(clicked()), this, SLOT(pushButtonFreezeWaveRespond()));
	connect(pushButtonDisplayParaSet, SIGNAL(clicked()), this, SLOT(pushButtonDisplayParaSetRespond()));
	connect(pushButtonFunctionhide, SIGNAL(clicked()), this, SLOT(pushButtonFunctionhideRespond()));
	connect(pushButtonAdd2, SIGNAL(clicked()), this, SLOT(pushButtonAdd2Respond()));
	connect(pushButtonCut2, SIGNAL(clicked()), this, SLOT(pushButtonCut2Respond()));
	connect(pushButtonAScanSave, SIGNAL(clicked()), this, SLOT(pushButtonAScanSaveRespond()));

	connect(checkBoxProbe1, SIGNAL(clicked()), this, SLOT(checkBoxProbe1Change()));
	connect(checkBoxProbe2, SIGNAL(clicked()), this, SLOT(checkBoxProbe2Change()));
	connect(checkBoxProbe3, SIGNAL(clicked()), this, SLOT(checkBoxProbe3Change()));
	connect(checkBoxProbe4, SIGNAL(clicked()), this, SLOT(checkBoxProbe4Change()));
	connect(checkBoxProbe5, SIGNAL(clicked()), this, SLOT(checkBoxProbe5Change()));
	connect(checkBoxProbe6, SIGNAL(clicked()), this, SLOT(checkBoxProbe6Change()));
	connect(checkBoxGateSwitch, SIGNAL(clicked()), this, SLOT(checkBoxGateSwitchChange()));

	connect(this, SIGNAL(AscanInfoChanged()), this, SLOT(AscanInfoUpdate())); //各滑块更新函数
	connect(checkBoxLock, SIGNAL(clicked()), this, SLOT(AScanLock()));

// 	checkBoxProbe1->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#008F74"));
// 	checkBoxProbe2->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#88C122"));
// 	checkBoxProbe3->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#BA296B"));
// 	checkBoxProbe4->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#F08961"));
// 	checkBoxProbe5->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#FFF100"));
// 	checkBoxProbe6->setStyleSheet(QString("color:%1;font:15pt;background:rgb(103,103,105);").arg("#30B5C3"));
}

void AScan::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QBrush brush(Qt::black);     // 画刷
	painter.setBrush(brush);            // 设置画刷
	painter.setPen(Qt::black);
	GateMoveUpdate();
	painter.drawRect(m_rectWave); 
	DrawGrid(painter, m_rectWave,m_fDelayOut,m_nGrid,m_nLines);
	DrawWave(painter, m_rectWave);
 	GateIPainter(painter);
 	GateAPainter(painter);
 	GateBPainter(painter);
 	GateI2Painter(painter);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.drawRect(m_rectHRuler);
	painter.drawRect(m_rectVRuler);
	painter.drawRect(m_rectBlank);
	DrawVRuler(painter, m_rectVRuler,0,100,5,1,0);
	DrawHRuler(painter, m_rectHRuler,(int)m_fXMinRuler,(int)m_fXMaxRuler,(int)(m_fXMaxRuler-m_fXMinRuler)/10,0,0);
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
	//横向网格虚线
	for(int i = 1;i<5;++i)
	{
		for(int j = 0;j< 10;++j)
		{
			QPoint pt1;
			QPoint pt2;
			pt1.setX(1.0/10.0*rect.width()*j);
			pt1.setY(2.0/10.0*rect.height()*i);
			pt2.setX(1.0/10.0*rect.width()*(j+1));
			pt2.setY(2.0/10.0*rect.height()*i);
			for (int i = 0;i < 10;i++)
			{
				painter.drawLine(pt1.x()+1.0/10*(pt2.x() - pt1.x())*i + rect.left(), pt1.y()+1.0/10*(pt2.y() - pt1.y())*i + rect.top(),pt1.x()+1.0/10*(pt2.x() - pt1.x())*(i+1)+rect.left(),pt1.y()+1.0/10*(pt2.y() - pt1.y())*(i+1) + rect.top());
				i++;
			}
		}
	}
	//纵向网格虚线
	for(int i = 1;i<= nlines;++i)
	{
		for(int j = 0;j<5;++j)
		{
			QPoint pt1;
			QPoint pt2;
			pt1.setX(1.0/10.0*rect.width()*i);
			pt1.setY( 2.0/10.0*rect.height()*j);
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
	int iSayac = 0;
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
	int iSel = 0;
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

void AScan::DrawWave(QPainter &painter, const QRect& rect)	
{
	painter.setPen(Qt::yellow);

	int nSize = MONITOR_SIZE;
	int height = rect.height() - 1;
	float yb = rect.height() / MAX_HEIGHT * WAVE_ZOOM_SCALE;
	float xb = rect.width()/float(nSize);

	map<WORD,CPicData*>::iterator cPicData;
	map<WORD,CPicData*> m_MapCPicDataTemp;
	g_ADataMutex.lock();
	for (cPicData = m_MapCPicData.begin(); cPicData != m_MapCPicData.end(); cPicData++)
	{
		int channelTemp = cPicData->first;
		for (int i = 0; i < g_vecADataIndex.count(); ++i)
		{
			if (channelTemp == g_vecADataIndex.value(i))
			{
				m_MapCPicDataTemp[channelTemp] = cPicData->second;
			}
		}
	}
	g_ADataMutex.unlock();

	map<WORD,CPicData*>::iterator itPicData;
	for (itPicData = m_MapCPicDataTemp.begin();itPicData != m_MapCPicDataTemp.end(); itPicData++)
	{
		if (!itPicData->second)
		{
			QMessageBox::warning(NULL, tr("指针错误"), tr("AScan::DrawWave（）：itPicData->second 为空"));
			return;
		}

		QPoint ptStart(rect.left(),rect.top() + height - (itPicData->second)->m_wData[0]*yb);
		QPoint ptEnd(rect.left(),rect.top());
		WORD wTemp = itPicData->first;
		if (wTemp < 24)
		{
			if (wTemp % 4 == 0)
			{
				painter.setPen(QColor(0,143,116));
			}
			else if (wTemp % 4 == 2)
			{
				painter.setPen(QColor(136,193,34));
			}
			else if (wTemp % 4 == 1)
			{
				painter.setPen(QColor(186,41,107));
			}
			else if (wTemp % 4 == 3)
			{
				painter.setPen(QColor(240,137,97));
			}
		}
		else if (wTemp >= 24 && wTemp <= 29)
		{
			painter.setPen(QColor(255,241,0));
		}
		else 
		{
			painter.setPen(QColor(48,181,195));
		}

		for (int i=0; i < nSize; i++)
		{
			if ((itPicData->second)->m_wData[i] > MAX_HEIGHT) 
			{
				(itPicData->second)->m_wData[i] = MAX_HEIGHT;
			}
			ptEnd.setX(rect.left()+ i*xb);
			ptEnd.setY(rect.top() + height - (itPicData->second)->m_wData[i]*yb);
			painter.drawLine(ptStart,ptEnd);
			ptStart.setX(rect.left() + i*xb);
			ptStart.setY(rect.top() + height - (itPicData->second)->m_wData[i]*yb);
		}
	}
}

void AScan::SetAScanData(WORD wChannelNum,int nFlawOrBottom,CPicData* pPicData)
{
	update();
}

void AScan::toolbar_show()	//工具栏显示函数
{
	pushButton_07->show();
	pushButton_06->show();
	pushButton_05->show();
	pushButton_04->show();
	pushButton_02->show();
	pushButton_01->show();
	pushButtonOptionShow->hide();
	pushButtonOptionHide->show();
	widgetProbeOption->show();
	widgetGateOption->show();
	widgetProbeParam->show();
	widgetToolHide->show();	
}

void AScan::toolbar_hide()	//工具栏隐藏函数
{
	pushButton_07->hide();
	pushButton_06->hide();
	pushButton_05->hide();
	pushButton_04->hide();
	pushButton_02->hide();
	pushButton_01->hide();
	pushButtonOptionHide->hide();
	pushButtonOptionShow->show();
	widgetProbeOption->hide();
	widgetGateOption->hide();
	widgetProbeParam->hide();
	widgetToolHide->hide();
}

void AScan::GateChange()	//闸门切换函数
{
	m_ncomboBoxGate=comboBoxGate->currentIndex();
	if (m_ncomboBoxGate==0)
	{
		labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,0,0)"));
		m_bGateAIsPressed=false;
		m_bGateBIsPressed=false;
		m_bGateI2IsPressed=false;
		if (m_bGateI == 0)
		{
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			m_bGateIIsPressed=false;
		}
		else if (m_bGateI == 1)
		{
			checkBoxGateSwitch->setCheckState(Qt::Checked);
			m_bGateIIsPressed=true;
		}
	}
	else if (m_ncomboBoxGate == 1)
	{
		labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
		m_bGateIIsPressed=false;
		m_bGateBIsPressed=false;
		m_bGateI2IsPressed=false;
		if (m_bGateA == 0)
		{
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			m_bGateAIsPressed=false;
		}
		else if (m_bGateA == 1)
		{
			checkBoxGateSwitch->setCheckState(Qt::Checked);
			m_bGateAIsPressed=true;
		}
	}
	else if (m_ncomboBoxGate == 2)
	{
		labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,255)"));
		m_bGateIIsPressed=false;
		m_bGateAIsPressed=false;
		m_bGateI2IsPressed=false;
		if (m_bGateB == 0)
		{
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			m_bGateBIsPressed=false;
		}
		else if (m_bGateB == 1)
		{
			checkBoxGateSwitch->setCheckState(Qt::Checked);
			m_bGateBIsPressed=true;
		}
	}
	else if (m_ncomboBoxGate == 3)
	{
		labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
		m_bGateIIsPressed=false;
		m_bGateAIsPressed=false;
		m_bGateBIsPressed=false;
		if (m_bGateI2 == 0)
		{
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			m_bGateI2IsPressed=false;
		}
		else if (m_bGateI2 == 1)
		{
			checkBoxGateSwitch->setCheckState(Qt::Checked);
			m_bGateI2IsPressed=true;
		}
	}
}

void AScan::ProbeBoxChange()	//探头盒切换函数
{
	int nTemp = m_nProbeBox;
	m_nProbeBox=comboBoxProbeBox->currentIndex();
	int nProbeBoxChange = m_nProbeBox - nTemp;

	if (m_nProbe > 3)
	{	
	    m_wChannelNo = 24 + m_nProbeBox;
	}
	else
	{
		m_wChannelNo = m_nProbeBox * 4 + m_nCurSelectProbe + 1;
	}

	QVector<int> vecTemp;
	g_ADataMutex.lock();
	for (int i = 0; i < g_vecADataIndex.count(); ++i)
	{
		int indexTemp = g_vecADataIndex.value(i);
		if (indexTemp < 24)
		{
			WORD wTempChannelNum = indexTemp + nProbeBoxChange * 4;
			vecTemp.push_back(wTempChannelNum);
		}
		else
		{
			WORD wTempChannelNum = indexTemp + nProbeBoxChange;
			vecTemp.push_back(wTempChannelNum);
		}
	}

    g_vecADataIndex.clear();
	g_vecADataIndex = vecTemp;
	g_ADataMutex.unlock();

	SetProbeInfo();
	SetGateInfo();
	UpdateProbe();
	ProbeRemember();
	GateChange();
	ProbeChange();
}

void AScan::AScanLock()			//A扫控件锁定与解锁
{
	if (checkBoxLock->isChecked())
	{
		checkBoxGateSwitch->setEnabled(false);
		horizontalSliderGateFront->setEnabled(false);
		horizontalSliderGateWidth->setEnabled(false);
		horizontalSliderGatePeak->setEnabled(false);
		horizontalSliderZeroPoint->setEnabled(false);
		horizontalSliderDelay->setEnabled(false);
		horizontalSliderXRange->setEnabled(false);
		horizontalSliderYRange->setEnabled(false);
		horizontalSliderReFreq->setEnabled(false);
		spinBoxGateFront->setEnabled(false);
		spinBoxGateWidth->setEnabled(false);
		spinBoxGatePeak->setEnabled(false);
		spinBoxZeroPoint->setEnabled(false);
		spinBoxDelay->setEnabled(false);
		spinBoxXRange->setEnabled(false);
		spinBoxYRange->setEnabled(false);
		spinBoxReFreq->setEnabled(false);
		verticalSliderSuppression->setEnabled(false);
		spinBoxSuppression->setEnabled(false);
		verticalSliderGain->setEnabled(false);
		spinBoxGain->setEnabled(false);
		pushButtonAdd2->setEnabled(false);
		pushButtonCut2->setEnabled(false);
	}
	else
	{
		checkBoxGateSwitch->setEnabled(true);
		horizontalSliderGateFront->setEnabled(true);
		horizontalSliderGateWidth->setEnabled(true);
		horizontalSliderGatePeak->setEnabled(true);
		horizontalSliderZeroPoint->setEnabled(true);
		horizontalSliderDelay->setEnabled(true);
		horizontalSliderXRange->setEnabled(true);
		horizontalSliderYRange->setEnabled(true);
		horizontalSliderReFreq->setEnabled(true);
		spinBoxGateFront->setEnabled(true);
		spinBoxGateWidth->setEnabled(true);
		spinBoxGatePeak->setEnabled(true);
		spinBoxZeroPoint->setEnabled(true);
		spinBoxDelay->setEnabled(true);
		spinBoxXRange->setEnabled(true);
		spinBoxYRange->setEnabled(true);
		spinBoxReFreq->setEnabled(true);
		verticalSliderSuppression->setEnabled(true);
		spinBoxSuppression->setEnabled(true);
		verticalSliderGain->setEnabled(true);
		spinBoxGain->setEnabled(true);
		pushButtonAdd2->setEnabled(true);
		pushButtonCut2->setEnabled(true);
	}
}

void AScan::ProbeChange()
{
	CurSelectProbe();
	GateChange();
}

void AScan::UpdateCData()
{
	g_ADataMutex.lock();
	m_CH_FLAWDATA_CSCAN_D = g_CH_FLAWDATA_CSCAN_D;
	g_ADataMutex.unlock();
}

void AScan::pushButtonSaveScreenRespond()	//截屏
{

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

}

void AScan::pushButtonAdd2Respond()	//+2dB
{
	if (m_fGain + 2 > 80)
		m_fGain = 80;
	else
		m_fGain +=2;
	emit AscanInfoChanged();
}

void AScan::pushButtonCut2Respond()	//-2dB
{
	if (m_fGain - 2 < 0)
		m_fGain = 0;
	else
		m_fGain -=2;
	emit AscanInfoChanged();
}

void AScan::pushButtonAScanSaveRespond()	//保存
{

}

void AScan::checkBoxProbe1Change()	//探头1开关函数
{
	if (!checkBoxProbe1->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe1->setChecked(true);
	}
	else
	{
		m_nProbe = 0;
		m_nFlawOrBottom = 1;
		m_wMapChannelNum = m_nProbeBox * 4 + m_nProbe;	// 1 5 9 13 17 21
		ChangeMapCPicData(m_wMapChannelNum);
		UpdateProbe();
		ProbeRemember();
	}
}

void AScan::checkBoxProbe2Change()	//探头2开关函数
{
	if (!checkBoxProbe2->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe2->setChecked(true);
	}
	else
	{
		m_nProbe = 1;
		m_nFlawOrBottom = 1;
		m_wMapChannelNum = m_nProbeBox * 4 + m_nProbe;
		ChangeMapCPicData(m_MapChannel_CONFIG[m_wMapChannelNum]);
		UpdateProbe();
		ProbeRemember();
	}
}

void AScan::checkBoxProbe3Change()	//探头3开关函数
{
	if (!checkBoxProbe3->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe3->setChecked(true);
	}
	else
	{
		m_nProbe = 2;
		m_nFlawOrBottom = 1;
		m_wMapChannelNum = m_nProbeBox * 4 + m_nProbe;
		ChangeMapCPicData(m_MapChannel_CONFIG[m_wMapChannelNum]);
		UpdateProbe();
		ProbeRemember();
	}
}

void AScan::checkBoxProbe4Change()	//探头4开关函数
{
	if (!checkBoxProbe4->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe4->setChecked(true);
	}
	else
	{
		m_nProbe = 3;
		m_nFlawOrBottom = 1;
		m_wMapChannelNum = m_nProbeBox * 4 + m_nProbe;
		ChangeMapCPicData(m_wMapChannelNum);
		UpdateProbe();
		ProbeRemember();
	}
}

void AScan::checkBoxProbe5Change()	//探头5 底波 分层 开关函数  
{
	if (!checkBoxProbe5->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe5->setChecked(true);
	}
	else
	{
		m_nProbe = 4;
		m_nFlawOrBottom = 1;
		m_wMapChannelNum = 24 + m_nProbeBox;
		ChangeMapCPicData(m_wMapChannelNum);
		UpdateProbe();
		ProbeRemember();
	}
}
void AScan::checkBoxProbe6Change()	//探头6 测厚 开关函数
{
	if (!checkBoxProbe6->isChecked() && comboBoxProbe->count() == 1)
	{
		checkBoxProbe6->setChecked(true);
	}
	else
	{
		m_nProbe = 5;
		m_nFlawOrBottom = 0;
		m_wMapChannelNum = 30 + m_nProbeBox;
		ChangeMapCPicData(m_wMapChannelNum);
		UpdateProbe();
		ProbeRemember();
	}
}

void AScan::checkBoxGateSwitchChange()	//闸门开关函数
{
	if (m_ncomboBoxGate==0)
	{
		if (m_bGateI==0)
		{
			m_bGateI=1;
			m_bGateIIsPressed=true;
			m_bGateAIsPressed=false;
			m_bGateBIsPressed=false;
			m_bGateI2IsPressed=false;
		}
		else if (m_bGateI==1)
		{
			m_bGateI=0;
			m_bGateIIsPressed=false;
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
		}
	}
	if (m_ncomboBoxGate==1)
	{
		if (m_bGateA==0)
		{
			m_bGateA=1;
			m_bGateIIsPressed=false;
			m_bGateAIsPressed=true;
			m_bGateBIsPressed=false;
			m_bGateI2IsPressed=false;
		}
		else if (m_bGateA==1)
		{
			m_bGateA=0;
			m_bGateAIsPressed=false;
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
		}
	}
	if (m_ncomboBoxGate==2)
	{
		if (m_bGateB==0)
		{
			m_bGateB=1;
			m_bGateIIsPressed=false;
			m_bGateAIsPressed=false;
			m_bGateBIsPressed=true;
			m_bGateI2IsPressed=false;
		}
		else if (m_bGateB==1)
		{
			m_bGateB=0;
			m_bGateBIsPressed=false;
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
		}
	}
	if (m_ncomboBoxGate==3)
	{
		if (m_bGateI2==0)
		{
			m_bGateI2=1;
			m_bGateIIsPressed=false;
			m_bGateAIsPressed=false;
			m_bGateBIsPressed=false;
			m_bGateI2IsPressed=true;
		}
		else if (m_bGateI2==1)
		{
			m_bGateI2=0;
			m_bGateI2IsPressed=false;
			checkBoxGateSwitch->setCheckState(Qt::Unchecked);
		}
	}
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[0].wGateEn = m_bGateI;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[1].wGateEn = m_bGateA;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[2].wGateEn = m_bGateB;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[3].wGateEn = m_bGateI2;
	if (m_nFlawOrBottom == 0)
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[0].m_bOpen = m_bGateI;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[1].m_bOpen = m_bGateA;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[2].m_bOpen = m_bGateB;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[3].m_bOpen = m_bGateI2;
	}
	else
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_bOpen = m_bGateI;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_bOpen = m_bGateA;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_bOpen = m_bGateB;
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_bOpen = m_bGateI2;
	}
}

void AScan::GateFrontChange()	//闸门前沿变量跟踪滑块变化函数
{
	m_fGateFrontOut=(float)spinBoxGateFront->value();
	m_fGateFrontIn=m_fGateFrontOut;
	if (m_bGateIIsPressed)
	{
		m_fIGateFront=m_fGateFrontOut;
		DWORD dwDelay = MicroSecond2Dot(m_fDelay);
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[0].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fIGateFront) - dwDelay;
		if (m_nFlawOrBottom == 0)
		{
		      m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[0].m_fFrontGate = m_fIGateFront;
		}
		else
		{
		      m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fFrontGate = m_fIGateFront;
		}

	}
	if (m_bGateAIsPressed)
	{
		m_fAGateFront=m_fGateFrontOut;
		DWORD dwDelay = 0;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[1].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fAGateFront) - dwDelay;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[1].m_fFrontGate = m_fAGateFront;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fFrontGate = m_fAGateFront;
		}
	}
	if (m_bGateBIsPressed)
	{
		m_fBGateFront=m_fGateFrontOut;
		DWORD dwDelay = 0;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[2].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fBGateFront) - dwDelay;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[2].m_fFrontGate = m_fBGateFront;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fFrontGate = m_fBGateFront;
		}
	}
	if (m_bGateI2IsPressed)
	{
		m_fI2GateFront=m_fGateFrontOut;
		DWORD dwDelay = MicroSecond2Dot(m_fDelay);
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[3].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fI2GateFront) - dwDelay;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[3].m_fFrontGate = m_fI2GateFront;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fFrontGate = m_fI2GateFront;
		}
	}
	m_bSetOne=true;
}

void AScan::GateWidthChange()	//闸门宽度变量跟踪滑块变化函数
{
	m_fGateWidthOut=(float)spinBoxGateWidth->value();
	m_fGateWidthIn=m_fGateWidthOut;
	if (m_bGateIIsPressed)
	{
		m_fIGateWidth=m_fGateWidthOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[0].wGateWidth = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fIGateWidth);
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[0].m_fBackGate = m_fIGateWidth;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fBackGate = m_fIGateWidth;
		}
	}
	if (m_bGateAIsPressed)
	{
		m_fAGateWidth=m_fGateWidthOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[1].wGateWidth = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fAGateWidth);
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[1].m_fBackGate = m_fAGateWidth;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[1].m_fBackGate = m_fAGateWidth;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fBackGate = m_fAGateWidth;
		}
	}
	if (m_bGateBIsPressed)
	{
		m_fBGateWidth=m_fGateWidthOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[2].wGateWidth = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fBGateWidth);
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[2].m_fBackGate = m_fBGateWidth;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[2].m_fBackGate = m_fBGateWidth;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fBackGate = m_fBGateWidth;
		}
	}
	if (m_bGateI2IsPressed)
	{
		m_fI2GateWidth=m_fGateWidthOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[3].wGateWidth = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fI2GateWidth);
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[3].m_fBackGate = m_fI2GateWidth;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[3].m_fBackGate = m_fI2GateWidth;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fBackGate = m_fI2GateWidth;
		}
	}
	m_bSetOne=true;
}

void AScan::GatePeakChange()		//闸门阈值变量跟踪滑块变化函数
{
	m_fGatePeakOut=(float)spinBoxGatePeak->value();
	m_fGatePeakIn=m_fGatePeakOut;
	if (m_bGateIIsPressed)
	{
		m_fIGatePeak=m_fGatePeakOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[0].wGateHeight = m_fIGatePeak/100.0*MAX_HEIGHT;
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[0].m_fPeakGate = m_fIGatePeak;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[0].m_fPeakGate = m_fIGatePeak;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fPeakGate = m_fIGatePeak;
		}
	}
	if (m_bGateAIsPressed)
	{
		m_fAGatePeak=m_fGatePeakOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[1].wGateHeight = m_fAGatePeak/100.0*MAX_HEIGHT;
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[1].m_fPeakGate = m_fAGatePeak;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[1].m_fPeakGate = m_fAGatePeak;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fPeakGate = m_fAGatePeak;
		}
	}
	if (m_bGateBIsPressed)
	{
		m_fBGatePeak=m_fGatePeakOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[2].wGateHeight = m_fBGatePeak/100.0*MAX_HEIGHT;
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[2].m_fPeakGate = m_fBGatePeak;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[2].m_fPeakGate = m_fBGatePeak;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fPeakGate = m_fBGatePeak;
		}
	}
	if (m_bGateI2IsPressed)
	{
		m_fI2GatePeak=m_fGatePeakOut;
		m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[3].wGateHeight = m_fI2GatePeak/100.0*MAX_HEIGHT;
		//m_ChannelInfo[m_wChannelNo].m_GateInfo[3].m_fPeakGate = m_fI2GatePeak;
		if (m_nFlawOrBottom == 0)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo+6]].m_GateInfo[3].m_fPeakGate = m_fI2GatePeak;
		}
		else
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fPeakGate = m_fI2GatePeak;
		}
	}
	m_bSetOne=true;
}

void AScan::ZeroPointChange()	//零点变量跟踪滑块变化函数
{
	m_fZeroPointOut=(float)spinBoxZeroPoint->value();
	m_fZeroPoint=m_fZeroPointOut;
	m_bSetOne=true;
}

void AScan::DelayChange()		//延迟变量跟踪滑块变化函数
{
	m_fDelayOut=(float)spinBoxDelay->value();
	m_fDelay=m_fDelayOut;
	DWORD dwDelay = MicroSecond2Dot(m_fDelay);
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[0].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fIGateFront) - dwDelay;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].stGate[3].wGateStart = Range2Dot(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, m_fIGateFront) - dwDelay;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[0].wDelay = dwDelay;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[1].wDelay = dwDelay;
	//m_ChannelInfo[m_wChannelNo].m_fChannelDelay = m_fDelay;
	if (m_nFlawOrBottom == 0)
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_fChannelDelay = m_fDelay;
	}
	else
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_fChannelDelay = m_fDelay;
	}
	m_fXMinRuler = Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, dwDelay);
	m_fXMaxRuler = m_fRange + m_fXMinRuler; 
	m_bSetOne=true;
}

void AScan::RangeChange()		//X轴范围变量跟踪滑块变化函数
{
	m_fRangeOut=(float)spinBoxXRange->value();
	m_fRange=m_fRangeOut;
	if (m_nFlawOrBottom == 0)
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_fChannelRange = m_fRange;
	}
	else
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_fChannelRange = m_fRange;
	}
	m_fXMaxRuler = m_fRange + m_fXMinRuler; 
	m_bSetOne=true;
}

void AScan::ReferGainChange()		//参考增益变量跟踪滑块变化函数
{
	m_fReferGainOut=(float)spinBoxYRange->value();
	m_fCGain=m_fReferGainOut;
	m_stAllCH_CONFIG[m_wChannelNo].wBottomFlawPlusDiff = m_fCGain;
	if (m_nFlawOrBottom == 0)
	{
		if (m_MapCONFIG_Channel[m_wChannelNo + 6] >= 30 && m_MapCONFIG_Channel[m_wChannelNo + 6] <= 35)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6] - 6].m_fChannelCPlus = m_fCGain;
		}
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_fChannelCPlus = m_fCGain;
	}
	else
	{
		if (m_MapCONFIG_Channel[m_wChannelNo] >= 24 && m_MapCONFIG_Channel[m_wChannelNo] <= 29)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo] + 6].m_fChannelCPlus = m_fCGain;
		}
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_fChannelCPlus = m_fCGain;
	}
	m_bSetOne=true;
}

void AScan::ReFreqChange()		//重复频率变量跟踪滑块变化函数
{
	m_fReFreqOut=(float)spinBoxReFreq->value();
	m_fReFreq=m_fReFreqOut;
	for (int i = 0;i<36;i++) //所有通道的重复频率是同时改变的,反应到硬件上是只对应一个变量
	{
		m_ChannelInfo[i].m_ProbeInfo.m_wReFreq = m_fReFreq;
	}
	m_bSetOne=true;
}

void AScan::GainChange()			//增益变量跟踪滑块变化函数
{
	m_fGainOut=(float)spinBoxGain->value();
	m_fGain=m_fGainOut;
	m_stAllCH_CONFIG[m_wChannelNo].stPlus.wCPlus = Gain2Data(m_fGain/2);
	m_stAllCH_CONFIG[m_wChannelNo].stPlus.wDPlus = Gain2Data(m_fGain/2);
	if (m_nFlawOrBottom == 0)
	{
		if (m_MapCONFIG_Channel[m_wChannelNo + 6] >= 30 && m_MapCONFIG_Channel[m_wChannelNo + 6] <= 35)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6] - 6].m_fChannelPlus = m_fGain;
		}
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_fChannelPlus = m_fGain;
	}
	else
	{
		if (m_MapCONFIG_Channel[m_wChannelNo] >= 24 && m_MapCONFIG_Channel[m_wChannelNo] <= 29)
		{
			m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo] + 6].m_fChannelPlus = m_fGain;
		}
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_fChannelPlus = m_fGain;
	}
	m_bSetOne=true;
}

void AScan::SuppressionChange()	//抑制变量跟踪滑块变化函数
{
	m_fSuppressionOut=(float)spinBoxSuppression->value();
	m_fSuppression=m_fSuppressionOut;
	m_stAllCH_CONFIG[m_wChannelNo].stWaveConfigure[m_nFlawOrBottom].wSuppression = m_fSuppression / 100.0 * MAX_HEIGHT;
	//m_ChannelInfo[m_wChannelNo].m_fRestrain = m_fSuppression;
	if (m_nFlawOrBottom == 0)
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_fRestrain = m_fSuppression;
	}
	else
	{
		m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_fRestrain = m_fSuppression;
	}
	m_bSetOne=true;
}

/*各滑块变量更新函数*/
void AScan::AscanInfoUpdate()
{
	if (!qFuzzyCompare(m_fGateFrontOut, m_fGateFrontIn))
	{
		m_fGateFrontOut=m_fGateFrontIn;
		spinBoxGateFront->setValue((double)m_fGateFrontOut);
	}
	if (!qFuzzyCompare(m_fGateWidthOut, m_fGateWidthIn))
	{
		m_fGateWidthOut=m_fGateWidthIn;
		spinBoxGateWidth->setValue((double)m_fGateWidthOut);
	}
	if (!qFuzzyCompare(m_fGatePeakOut, m_fGatePeakIn))
	{
		m_fGatePeakOut=m_fGatePeakIn;
		spinBoxGatePeak->setValue((double)m_fGatePeakOut);
	}
	if (!qFuzzyCompare(m_fZeroPointOut, m_fZeroPoint))
	{
		m_fZeroPointOut=m_fZeroPoint;
		spinBoxZeroPoint->setValue((double)m_fZeroPointOut);
	}
	if (!qFuzzyCompare(m_fDelayOut, m_fDelay))
	{
		m_fDelayOut=m_fDelay;
		spinBoxDelay->setValue((double)m_fDelayOut);
	}
	if (!qFuzzyCompare(m_fRangeOut, m_fRange))
	{
		m_fRangeOut=m_fRange;
		spinBoxXRange->setValue((double)m_fRangeOut);
	}
	if (!qFuzzyCompare(m_fReferGainOut, m_fCGain))
	{
		m_fReferGainOut=m_fCGain;
		spinBoxYRange->setValue((double)m_fReferGainOut);
	}
	if (!qFuzzyCompare(m_fReFreqOut, m_fReFreq))
	{
		m_fReFreqOut=m_fReFreq;
		spinBoxReFreq->setValue((double)m_fReFreqOut);
	}
	if (!qFuzzyCompare(m_fGainOut, m_fGain))
	{
		m_fGainOut=m_fGain;
		spinBoxGain->setValue((double)m_fGainOut);
	}
	if (!qFuzzyCompare(m_fSuppressionOut, m_fSuppression))
	{
		m_fSuppressionOut=m_fSuppression;
		spinBoxSuppression->setValue((double)m_fSuppressionOut);
	}
}

void AScan::slotSlider_SpinboxGateFront()
{
	spinBoxGateFront->setValue((double)(horizontalSliderGateFront->value())*0.001);
}

void AScan::slotSpinbox_SliderGateFront()
{
	horizontalSliderGateFront->setValue((int)(spinBoxGateFront->value()*1000));
}

void AScan::slotSlider_SpinboxGateWidth()
{
	spinBoxGateWidth->setValue((double)(horizontalSliderGateWidth->value())*0.001);
}

void AScan::slotSpinbox_SliderGateWidth()
{
	horizontalSliderGateWidth->setValue((int)(spinBoxGateWidth->value()*1000));
}

void AScan::slotSlider_SpinboxGatePeak()
{
	spinBoxGatePeak->setValue((double)(horizontalSliderGatePeak->value())*0.001);
}

void AScan::slotSpinbox_SliderGatePeak()
{
	horizontalSliderGatePeak->setValue((int)(spinBoxGatePeak->value()*1000));
}

void AScan::slotSlider_SpinboxZeroPoint()
{
	spinBoxZeroPoint->setValue((double)(horizontalSliderZeroPoint->value())*0.001);
}

void AScan::slotSpinbox_SliderZeroPoint()
{
	horizontalSliderZeroPoint->setValue((int)(spinBoxZeroPoint->value()*1000));
}
void AScan::slotSlider_SpinboxDelay()
{
	spinBoxDelay->setValue((double)(horizontalSliderDelay->value())*0.01);
}

void AScan::slotSpinbox_SliderDelay()
{
	horizontalSliderDelay->setValue((int)(spinBoxDelay->value()*100));
}

void AScan::slotSlider_SpinboxXRange()
{
	spinBoxXRange->setValue((double)(horizontalSliderXRange->value()));
}

void AScan::slotSpinbox_SliderXRange()
{
	horizontalSliderXRange->setValue((int)(spinBoxXRange->value()));
}

void AScan::slotSlider_SpinboxYRange()
{
	spinBoxYRange->setValue((double)(horizontalSliderYRange->value()));
}

void AScan::slotSpinbox_SliderYRange()
{
	horizontalSliderYRange->setValue((int)(spinBoxYRange->value()));
}

void AScan::slotSlider_SpinboxReFreq()
{
	spinBoxReFreq->setValue((double)(horizontalSliderReFreq->value()));
}

void AScan::slotSpinbox_SliderReFreq()
{
	horizontalSliderReFreq->setValue((int)(spinBoxReFreq->value()));
}

void AScan::slotSlider_SpinboxSuppression()
{
	spinBoxSuppression->setValue((double)(verticalSliderSuppression->value())*0.001);
}

void AScan::slotSpinbox_SliderSuppression()
{
	verticalSliderSuppression->setValue((int)(spinBoxSuppression->value()*1000));
}

void AScan::slotSlider_SpinboxGain()
{
	spinBoxGain->setValue((double)(verticalSliderGain->value())*0.001);
}

void AScan::slotSpinbox_SliderGain()
{
	verticalSliderGain->setValue((int)(spinBoxGain->value()*1000));
}

void AScan::GateIPainter(QPainter &painter)
{
	if (m_bGateI)
	{
		painter.setPen(QColor(255,0,0));
		painter.drawLine(m_fGateILeft,m_fGateITop,m_fGateILeft,m_fGateIBottom);
		painter.drawLine(m_fGateILeft,m_fGateIMiddle,m_fGateIRight,m_fGateIMiddle);
		painter.drawLine(m_fGateIRight,m_fGateITop,m_fGateIRight,m_fGateIBottom);

		if (m_bGateIIsPressed == true)
		{
			painter.setBrush(QColor(0,0,255));
			QRect rectLeft(m_fGateILeft-3,m_fGateIMiddle-2,5,5);
			QRect rectRight(m_fGateIRight-3,m_fGateIMiddle-2,5,5);
			painter.drawRect(rectLeft);
			painter.drawRect(rectRight);

			m_fGateFrontIn=(double)(m_fGateILeft-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler;
			m_fGateWidthIn=(double)(m_fGateIRight-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler-m_fGateFrontIn;
			m_fGatePeakIn=(double)(m_rectWave.bottom()-m_fGateIMiddle)*100/m_rectWave.height();

			m_fIGateFront=m_fGateFrontIn;
			m_fIGateWidth=m_fGateWidthIn;
			m_fIGatePeak=m_fGatePeakIn;

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
			char cGate1SyncTime[10];
			char cGate1MPAmp[10];
			char cXLocation[10];
			//编码器值
			float tempX = static_cast<float>(m_CH_FLAWDATA_CSCAN_D.wXHigh16 * 65536  + m_CH_FLAWDATA_CSCAN_D.wXLow16) / (X_TO_LENGTH);
			if (m_nProbeBox == 0)
			{
				tempX -= PROBEBOX_DISTANCE1;
			}
			else if (m_nProbeBox == 1)
			{
				tempX -= PROBEBOX_DISTANCE2;
			}
			else if (m_nProbeBox == 2)
			{
				tempX -= PROBEBOX_DISTANCE3;
			}
			else if (m_nProbeBox == 3)
			{
				tempX -= PROBEBOX_DISTANCE4;
			}
			else if (m_nProbeBox == 4)
			{
				tempX -= PROBEBOX_DISTANCE5;
			}
			else if (m_nProbeBox == 5)
			{
				tempX -= PROBEBOX_DISTANCE6;
			}
			sprintf(cXLocation,"%.2f",tempX);
			labelCoderShow->setText(QString(cXLocation));
			//幅度
			labelGateRange->setText(QObject::tr("闸门I内最大波峰值幅度: "));
			float fMPAmp= (float(m_CH_FLAWDATA_CSCAN_D.wGate1MPAmp)*100.0)/512.0;
			sprintf(cGate1MPAmp,"%.2f",fMPAmp);
			labelGateRangeShow->setText(QString(cGate1MPAmp));
			//时间
			labelGateTime->setText(QObject::tr("闸门I同步时间: "));
			float temp = Dot2Range(float(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed), m_CH_FLAWDATA_CSCAN_D.wGate1SyncTime + MicroSecond2Dot(m_fDelay));
			sprintf(cGate1SyncTime,"%.2f",temp);
			labelGateTimeShow->setText(QString(cGate1SyncTime));
			emit AscanInfoChanged();
		}
	}
}

void AScan::GateAPainter(QPainter &painter)
{
	if (m_bGateA)
	{
		painter.setPen(QColor(0,255,0));
		painter.drawLine(m_fGateALeft,m_fGateATop,m_fGateALeft,m_fGateABottom);
		painter.drawLine(m_fGateALeft,m_fGateAMiddle,m_fGateARight,m_fGateAMiddle);
		painter.drawLine(m_fGateARight,m_fGateATop,m_fGateARight,m_fGateABottom);

		if (m_bGateAIsPressed)
		{
			painter.setBrush(QColor(255,0,0));
			QRect rectLeft(m_fGateALeft-3,m_fGateAMiddle-2,5,5);
			QRect rectRight(m_fGateARight-3,m_fGateAMiddle-2,5,5);
			painter.drawRect(rectLeft);
			painter.drawRect(rectRight);

			float fSync = (float(m_CH_FLAWDATA_CSCAN_D.wGate1SyncTime)/ ADFREQUENCY + m_fDelayOut) * m_ChannelInfo[m_wChannelNo].m_nWaveSpeed / 1000.0f / 2.0f;
			m_fGateFrontIn=(double)(m_fGateALeft-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler-fSync;
			m_fGateWidthIn=(double)((m_fGateARight-m_rectWave.left())*m_fRangeOut)/m_rectWave.width()+m_fXMinRuler-m_fGateFrontIn-fSync;
			m_fGatePeakIn=((double)(m_rectWave.bottom()-m_fGateAMiddle))*100/m_rectWave.height();

			m_fAGateFront=m_fGateFrontOut;
			m_fAGateWidth=m_fGateWidthOut;
			m_fAGatePeak=m_fGatePeakOut;

			comboBoxGate->setCurrentIndex(1);
			m_ncomboBoxGate=1;
			labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(0,255,0)"));
			if (m_bGateA==0)
			{
				checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			}
			else if (m_bGateA==1)
			{
				checkBoxGateSwitch->setCheckState(Qt::Checked);
			}
			char cGate2MPTime[10];
			char cGate2MPAmp[10];
			char cXLocation[10];
			//sprintf(cGate2MPTime,"%d",m_CH_FLAWDATA_CSCAN_D.wGate2MPTime);

			float tempX = static_cast<float>(m_CH_FLAWDATA_CSCAN_D.wXHigh16 * 65536  + m_CH_FLAWDATA_CSCAN_D.wXLow16) / (X_TO_LENGTH);
			if (m_nProbeBox == 0)
			{
				tempX -= PROBEBOX_DISTANCE1;
			}
			else if (m_nProbeBox == 1)
			{
				tempX -= PROBEBOX_DISTANCE2;
			}
			else if (m_nProbeBox == 2)
			{
				tempX -= PROBEBOX_DISTANCE3;
			}
			else if (m_nProbeBox == 3)
			{
				tempX -= PROBEBOX_DISTANCE4;
			}
			else if (m_nProbeBox == 4)
			{
				tempX -= PROBEBOX_DISTANCE5;
			}
			else if (m_nProbeBox == 5)
			{
				tempX -= PROBEBOX_DISTANCE6;
			}
 			sprintf(cXLocation,"%.2f",tempX);
 			labelCoderShow->setText(QString(cXLocation));
			//幅度
			labelGateRange->setText(QObject::tr("闸门A内最大波峰值幅度: "));
			float fMPAmp= (float(m_CH_FLAWDATA_CSCAN_D.wGate2MPAmp)*100.0)/512.0;
			sprintf(cGate2MPAmp,"%.2f",fMPAmp);
			labelGateRangeShow->setText(QString(cGate2MPAmp));
			//时间
			labelGateTime->setText(QObject::tr("闸门A内最大波峰值时间: "));
			float temp = Dot2Range(float(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed), m_CH_FLAWDATA_CSCAN_D.wGate2MPTime + MicroSecond2Dot(m_fDelay));
			sprintf(cGate2MPTime,"%.2f",temp);
			labelGateTimeShow->setText(QString(cGate2MPTime));
			emit AscanInfoChanged();
		}
	}
}

void AScan::GateBPainter(QPainter &painter)
{
	if (m_bGateB)
	{
		painter.setPen(QColor(0,0,255));
		painter.drawLine(m_fGateBLeft,m_fGateBTop,m_fGateBLeft,m_fGateBBottom);
		painter.drawLine(m_fGateBLeft,m_fGateBMiddle,m_fGateBRight,m_fGateBMiddle);
		painter.drawLine(m_fGateBRight,m_fGateBTop,m_fGateBRight,m_fGateBBottom);

		if (m_bGateBIsPressed)
		{
			painter.setBrush(QColor(255,0,0));
			QRect rectLeft(m_fGateBLeft-3,m_fGateBMiddle-2,5,5);
			QRect rectRight(m_fGateBRight-3,m_fGateBMiddle-2,5,5);
			painter.drawRect(rectLeft);
			painter.drawRect(rectRight);

			float fSync = (float(m_CH_FLAWDATA_CSCAN_D.wGate1SyncTime)/ ADFREQUENCY + m_fDelayOut) * m_ChannelInfo[m_wChannelNo].m_nWaveSpeed / 1000.0f / 2.0f;
			m_fGateFrontIn=(double)(m_fGateBLeft-m_rectWave.left())*m_fRangeOut/m_rectWave.width()+m_fXMinRuler-fSync;
			m_fGateWidthIn=(double)((m_fGateBRight-m_rectWave.left())*m_fRangeOut)/m_rectWave.width()+m_fXMinRuler-m_fGateFrontIn-fSync;
			m_fGatePeakIn=((double)(m_rectWave.bottom()-m_fGateBMiddle))*100/m_rectWave.height();

			m_fBGateFront=m_fGateFrontOut;
			m_fBGateWidth=m_fGateWidthOut;
			m_fBGatePeak=m_fGatePeakOut;

			comboBoxGate->setCurrentIndex(2);
			m_ncomboBoxGate=2;
			labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,255)"));
			if (m_bGateB==0)
			{
				checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			}
			else if (m_bGateB==1)
			{
				checkBoxGateSwitch->setCheckState(Qt::Checked);
			}
			char cGate3MPTime[10];
			char cGate3MPAmp[10];
			char cXLocation[10];
			//编码器值
			float tempX = static_cast<float>(m_CH_FLAWDATA_CSCAN_D.wXHigh16 * 65536  + m_CH_FLAWDATA_CSCAN_D.wXLow16) / (X_TO_LENGTH);
			if (m_nProbeBox == 0)
			{
				tempX -= PROBEBOX_DISTANCE1;
			}
			else if (m_nProbeBox == 1)
			{
				tempX -= PROBEBOX_DISTANCE2;
			}
			else if (m_nProbeBox == 2)
			{
				tempX -= PROBEBOX_DISTANCE3;
			}
			else if (m_nProbeBox == 3)
			{
				tempX -= PROBEBOX_DISTANCE4;
			}
			else if (m_nProbeBox == 4)
			{
				tempX -= PROBEBOX_DISTANCE5;
			}
			else if (m_nProbeBox == 5)
			{
				tempX -= PROBEBOX_DISTANCE6;
			}
			sprintf(cXLocation,"%.2f",tempX);
			labelCoderShow->setText(QString(cXLocation));
			//幅度
			labelGateRange->setText(QObject::tr("闸门B内最大波峰值幅度: "));
			float fMPAmp= (float(m_CH_FLAWDATA_CSCAN_D.wGate3MPAmp)*100.0)/512.0;
			sprintf(cGate3MPAmp,"%.2f",fMPAmp);
			labelGateRangeShow->setText(QString(cGate3MPAmp));
			//时间
			labelGateTime->setText(QObject::tr("闸门B内最大波峰值时间: "));
			float temp = Dot2Range(float(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed), m_CH_FLAWDATA_CSCAN_D.wGate3MPTime + MicroSecond2Dot(m_fDelay));
			sprintf(cGate3MPTime,"%.2f",temp);
			labelGateTimeShow->setText(QString(cGate3MPTime));
			emit AscanInfoChanged();
		}
	}
}

void AScan::GateI2Painter(QPainter &painter)
{
	if (m_bGateI2)
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
			m_fGatePeakIn=((double)(m_rectWave.bottom()-m_fGateI2Middle))*100/m_rectWave.height();

			m_fI2GateFront=m_fGateFrontOut;
			m_fI2GateWidth=m_fGateWidthOut;
			m_fI2GatePeak=m_fGatePeakOut;

			comboBoxGate->setCurrentIndex(3);
			m_ncomboBoxGate=3;
			labelGateColor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,0)"));
			if (m_bGateI2==0)
			{
				checkBoxGateSwitch->setCheckState(Qt::Unchecked);
			}
			else if (m_bGateI2==1)
			{
				checkBoxGateSwitch->setCheckState(Qt::Checked);
			}
			char cGate4SyncTime[10];
			char cGate4MPAmp[10];
			char cXLocation[10];
			//编码器值
			float tempX = static_cast<float>(m_CH_FLAWDATA_CSCAN_D.wXHigh16 * 65536  + m_CH_FLAWDATA_CSCAN_D.wXLow16) / (X_TO_LENGTH);
			if (m_nProbeBox == 0)
			{
				tempX -= PROBEBOX_DISTANCE1;
			}
			else if (m_nProbeBox == 1)
			{
				tempX -= PROBEBOX_DISTANCE2;
			}
			else if (m_nProbeBox == 2)
			{
				tempX -= PROBEBOX_DISTANCE3;
			}
			else if (m_nProbeBox == 3)
			{
				tempX -= PROBEBOX_DISTANCE4;
			}
			else if (m_nProbeBox == 4)
			{
				tempX -= PROBEBOX_DISTANCE5;
			}
			else if (m_nProbeBox == 5)
			{
				tempX -= PROBEBOX_DISTANCE6;
			}
			sprintf(cXLocation,"%.2f",tempX);
			labelCoderShow->setText(QString(cXLocation));
			//幅度
			labelGateRange->setText(QObject::tr("闸门I2内最大波峰值幅度: "));
			float fMPAmp= (float(m_CH_FLAWDATA_CSCAN_D.wGate4MPAmp)*100.0)/512.0;
			sprintf(cGate4MPAmp,"%.2f",fMPAmp);
			labelGateRangeShow->setText(QString(cGate4MPAmp));
			//时间
			labelGateTime->setText(QObject::tr("闸门I2同步时间: "));
			float temp = Dot2Range(float(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed), m_CH_FLAWDATA_CSCAN_D.wGate4SyncTime + MicroSecond2Dot(m_fDelay));
			sprintf(cGate4SyncTime,"%.2f",temp);
			labelGateTimeShow->setText(QString(cGate4SyncTime));
			emit AscanInfoChanged();
		}
	}
}

void AScan::GateMoveUpdate()
{
	if (m_bGateMove)
	{	
		m_rectGateI.setRect(m_fGateILeft,m_fGateITop,m_fGateIRight-m_fGateILeft,m_fGateIBottom-m_fGateITop);
		if (!m_bGateIIsPresse)
		{
			m_fGateILeft=m_rectWave.left()+(m_fIGateFront-m_fXMinRuler)*m_rectWave.width()/m_fRangeOut;
			m_fGateIRight=m_rectWave.left()+(((m_fIGateWidth+m_fIGateFront-m_fXMinRuler)*m_rectWave.width()))/m_fRangeOut;
			m_fGateIMiddle=m_rectWave.bottom()-m_fIGatePeak*m_rectWave.height()/100;
			m_fGateITop=m_fGateIMiddle-m_rectWave.height()*0.01;
			m_fGateIBottom=m_fGateIMiddle+m_rectWave.height()*0.01;
			m_pGateILeftTop=QPoint(m_fGateILeft,m_fGateITop);
			m_pGateIRightBottom=QPoint(m_fGateIRight,m_fGateIBottom);
		}

		m_rectGateA.setRect(m_fGateALeft,m_fGateATop,m_fGateARight-m_fGateALeft,m_fGateABottom-m_fGateATop);
		if (!m_bGateAIsPresse)
		{
			float fSync = (float(m_CH_FLAWDATA_CSCAN_D.wGate1SyncTime)/ ADFREQUENCY + m_fDelayOut) * m_ChannelInfo[m_wChannelNo].m_nWaveSpeed / 1000.0f / 2.0f;
			m_fGateALeft=m_rectWave.left()+(m_fAGateFront-m_fXMinRuler  + fSync )*m_rectWave.width()/m_fRangeOut;
			m_fGateARight=m_rectWave.left()+(((m_fAGateWidth+m_fAGateFront-m_fXMinRuler  + fSync)*m_rectWave.width()))/m_fRangeOut;
			m_fGateAMiddle=m_rectWave.bottom()-m_fAGatePeak*m_rectWave.height()/100;
			m_fGateATop=m_fGateAMiddle-m_rectWave.height()*0.01;
			m_fGateABottom=m_fGateAMiddle+m_rectWave.height()*0.01;
			m_pGateALeftTop=QPoint(m_fGateALeft,m_fGateATop);
			m_pGateARightBottom=QPoint(m_fGateARight,m_fGateABottom);
		}

		m_rectGateB.setRect(m_fGateBLeft,m_fGateBTop,m_fGateBRight-m_fGateBLeft,m_fGateBBottom-m_fGateBTop);
		if (!m_bGateBIsPresse)
		{
			float fSync = (float(m_CH_FLAWDATA_CSCAN_D.wGate1SyncTime)/ ADFREQUENCY + m_fDelayOut) * m_ChannelInfo[m_wChannelNo].m_nWaveSpeed / 1000.0f / 2.0f;
			m_fGateBLeft=m_rectWave.left()+(m_fBGateFront-m_fXMinRuler + fSync)*m_rectWave.width()/m_fRangeOut;
			m_fGateBRight=m_rectWave.left()+(((m_fBGateWidth+m_fBGateFront-m_fXMinRuler + fSync)*m_rectWave.width()))/m_fRangeOut;
			m_fGateBMiddle=m_rectWave.bottom()-m_fBGatePeak*m_rectWave.height()/100;
			m_fGateBTop=m_fGateBMiddle-m_rectWave.height()*0.01;
			m_fGateBBottom=m_fGateBMiddle+m_rectWave.height()*0.01;
			m_pGateBLeftTop=QPoint(m_fGateBLeft,m_fGateBTop);
			m_pGateBRightBottom=QPoint(m_fGateBRight,m_fGateBBottom);
		}

		m_rectGateI2.setRect(m_fGateI2Left,m_fGateI2Top,m_fGateI2Right-m_fGateI2Left,m_fGateI2Bottom-m_fGateI2Top);
		if (!m_bGateI2IsPresse)
		{
			m_fGateI2Left=m_rectWave.left()+(m_fI2GateFront-m_fXMinRuler)*m_rectWave.width()/m_fRangeOut;
			m_fGateI2Right=m_rectWave.left()+(((m_fI2GateWidth+m_fI2GateFront-m_fXMinRuler)*m_rectWave.width()))/m_fRangeOut;
			m_fGateI2Middle=m_rectWave.bottom()-m_fI2GatePeak*m_rectWave.height()/100;
			m_fGateI2Top=m_fGateI2Middle-m_rectWave.height()*0.01;
			m_fGateI2Bottom=m_fGateI2Middle+m_rectWave.height()*0.01;
			m_pGateI2LeftTop=QPoint(m_fGateI2Left,m_fGateI2Top);
			m_pGateI2RightBottom=QPoint(m_fGateI2Right,m_fGateI2Bottom);
		}
	}
}
void AScan::InitChannelInfo()
{
	for (int i=0; i<CHANNEL_NUM; i++)
	{
		for (int j=0; j<4; j++)
		{
			m_ChannelInfo[i].m_GateInfo[j].m_bOpen = m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateEn;
			m_ChannelInfo[i].m_GateInfo[j].m_fFrontGate = Dot2Range(m_ChannelInfo[i].m_nWaveSpeed, m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateStart);
			m_ChannelInfo[i].m_GateInfo[j].m_fBackGate = Dot2Range(m_ChannelInfo[i].m_nWaveSpeed, m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateWidth);
			m_ChannelInfo[i].m_GateInfo[j].m_fPeakGate = m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateHeight / MAX_HEIGHT * 100;
			emit AscanInfoChanged();
		}
		m_ChannelInfo[i].m_fChannelDelay = Dot2MicroSecond(m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].wDelay);
		int nMin = Dot2Range(m_ChannelInfo[i].m_nWaveSpeed, MONITOR_SIZE);
		if (m_ChannelInfo[i].m_fChannelRange < nMin)
		{
			m_ChannelInfo[i].m_fChannelRange = nMin;
		}
		m_ChannelInfo[i].m_fChannelPlus = Data2Gain(m_stAllCH_CONFIG[i].stPlus.wCPlus + m_stAllCH_CONFIG[i].stPlus.wDPlus);
		m_ChannelInfo[i].m_fRestrain = m_stAllCH_CONFIG[i].stWaveConfigure[m_nFlawOrBottom].wSuppression;
		emit AscanInfoChanged();
	}
}

void AScan::InitConfigInfo()
{
	for (int i = 0;i < 36;i++)
	{
		if (i >= 0 && i <= 29)
		{
			m_nFlawOrBottom = 1;
			for (int j = 0;j < 4;j++)
			{
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateEn = m_ChannelInfo[i].m_GateInfo[j].m_bOpen;
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateStart = Range2Dot(m_ChannelInfo[i].m_nWaveSpeed,m_ChannelInfo[i].m_GateInfo[j].m_fFrontGate) - MicroSecond2Dot(m_ChannelInfo[i].m_fChannelDelay);
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateWidth = Range2Dot(m_ChannelInfo[i].m_nWaveSpeed,m_ChannelInfo[i].m_GateInfo[j].m_fBackGate);
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateHeight = m_ChannelInfo[i].m_GateInfo[j].m_fPeakGate / 100 * MAX_HEIGHT;
			}
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].wDelay = MicroSecond2Dot(m_ChannelInfo[i].m_fChannelDelay);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].wSuppression = m_ChannelInfo[i].m_fRestrain / 100.0 * MAX_HEIGHT;
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stPlus.wCPlus = Gain2Data(m_ChannelInfo[i].m_fChannelPlus / 2);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stPlus.wDPlus = Gain2Data(m_ChannelInfo[i].m_fChannelPlus / 2);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].wBottomFlawPlusDiff = m_ChannelInfo[i].m_fChannelCPlus;
		}
		else if (i >= 30 && i <= 35)
		{
			m_nFlawOrBottom = 0;
			for (int j = 0;j < 4;j++)
			{
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateEn = m_ChannelInfo[i].m_GateInfo[j].m_bOpen;
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateStart = Range2Dot(m_ChannelInfo[i].m_nWaveSpeed,m_ChannelInfo[i].m_GateInfo[j].m_fFrontGate) - MicroSecond2Dot(m_ChannelInfo[i].m_fChannelDelay);
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateWidth = Range2Dot(m_ChannelInfo[i].m_nWaveSpeed,m_ChannelInfo[i].m_GateInfo[j].m_fBackGate);
				m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].stGate[j].wGateHeight = m_ChannelInfo[i].m_GateInfo[j].m_fPeakGate / 100 * MAX_HEIGHT;
			}
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].wDelay = MicroSecond2Dot(m_ChannelInfo[i].m_fChannelDelay);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stWaveConfigure[m_nFlawOrBottom].wSuppression = m_ChannelInfo[i].m_fRestrain / 100.0 * MAX_HEIGHT;
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stPlus.wCPlus = Gain2Data(m_ChannelInfo[i].m_fChannelPlus / 2);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].stPlus.wDPlus = Gain2Data(m_ChannelInfo[i].m_fChannelPlus / 2);
			m_stAllCH_CONFIG[m_MapChannel_CONFIG[i]].wBottomFlawPlusDiff = m_ChannelInfo[i].m_fChannelCPlus;
		}
	}
}

void AScan::SetGateInfo(void)
{
	if (m_nFlawOrBottom == 0)
	{
		if (m_MapCONFIG_Channel[m_wChannelNo + 6] > CHANNEL_NUM + 3)
		{
			QMessageBox::warning(NULL, tr("通道号错误"), tr("AScan::SetGateInfo(): 通道号不满足 m_MapCONFIG_Channel[m_wChannelNo + 6] > CHANNEL_NUM + 3 条件"));
			return;
		}

		m_bGateI = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[0].m_bOpen ;
		m_bGateA = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[1].m_bOpen ;
		m_bGateB = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[2].m_bOpen ;
		m_bGateI2 = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[3].m_bOpen ;
		m_fIGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[0].m_fFrontGate;
		m_fAGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[1].m_fFrontGate;
		m_fBGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[2].m_fFrontGate;
		m_fI2GateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[3].m_fFrontGate;
		m_fIGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[0].m_fBackGate;
		m_fAGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[1].m_fBackGate;
		m_fBGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[2].m_fBackGate;
		m_fI2GateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[3].m_fBackGate;
		m_fIGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[0].m_fPeakGate;
		m_fAGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[1].m_fPeakGate;
		m_fBGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[2].m_fPeakGate;
		m_fI2GatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo + 6]].m_GateInfo[3].m_fPeakGate;
	}
	else
	{
		if (m_MapCONFIG_Channel[m_wChannelNo] > CHANNEL_NUM + 3)
		{
			QMessageBox::warning(NULL, tr("通道号错误"), tr("AScan::SetGateInfo(): 通道号不满足 m_MapCONFIG_Channel[m_wChannelNo] > CHANNEL_NUM + 3 条件"));
			return;
		}

		m_bGateI = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_bOpen ;
		m_bGateA = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_bOpen ;
		m_bGateB = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_bOpen ;
		m_bGateI2 = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_bOpen ;
		m_fIGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fFrontGate;
		m_fAGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fFrontGate;
		m_fBGateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fFrontGate;
		m_fI2GateFront = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fFrontGate;
		m_fIGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fBackGate;
		m_fAGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fBackGate;
		m_fBGateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fBackGate;
		m_fI2GateWidth = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fBackGate;
		m_fIGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[0].m_fPeakGate;
		m_fAGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[1].m_fPeakGate;
		m_fBGatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[2].m_fPeakGate;
		m_fI2GatePeak = m_ChannelInfo[m_MapCONFIG_Channel[m_wChannelNo]].m_GateInfo[3].m_fPeakGate;
	}
	emit AscanInfoChanged();
}

void AScan::SetProbeInfo(void)
{
	if (m_nFlawOrBottom == 0)
	{
		int channelNum = m_MapCONFIG_Channel[m_wChannelNo + 6];

		if ( (channelNum > CHANNEL_NUM + 3) || (channelNum < 0))
		{
			QMessageBox::warning(NULL, tr("通道号错误"), tr("AScan::SetProbeInfo(): 通道号不满足 (channelNum > CHANNEL_NUM + 3) || (channelNum < 0) 条件"));
			return;
		}

		m_fDelay = m_ChannelInfo[channelNum].m_fChannelDelay;
		int nMin = Dot2Range(m_ChannelInfo[channelNum].m_nWaveSpeed, MONITOR_SIZE);

		if (m_ChannelInfo[channelNum].m_fChannelRange < nMin)
		{
			m_ChannelInfo[channelNum].m_fChannelRange = nMin;
		}

		m_fRange = m_ChannelInfo[channelNum].m_fChannelRange;
		m_fCGain = m_ChannelInfo[channelNum].m_fChannelCPlus;
		m_fGain = m_ChannelInfo[channelNum].m_fChannelPlus;
		//更新抑制
		m_fSuppression = m_ChannelInfo[channelNum].m_fRestrain;
		//更新重复频率
		m_fReFreq = m_ChannelInfo[channelNum].m_ProbeInfo.m_wReFreq;

		DWORD dwDelay = MicroSecond2Dot(m_fDelay);
		m_fXMinRuler = Dot2Range(m_ChannelInfo[m_wChannelNo + 6].m_nWaveSpeed, dwDelay);
		m_fXMaxRuler = m_fRange + m_fXMinRuler;
		emit AscanInfoChanged();
	}
	else
	{
		int channelNum = m_MapCONFIG_Channel[m_wChannelNo];

		if ( (channelNum > CHANNEL_NUM + 3) || (channelNum < 0))
		{
			QMessageBox::warning(NULL, tr("通道号错误"), tr("AScan::SetProbeInfo(): 通道号不满足 (channelNum > CHANNEL_NUM + 3) || (channelNum < 0) 条件"));
			return;
		}

		m_fDelay = m_ChannelInfo[channelNum].m_fChannelDelay;
		int nMin = Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, MONITOR_SIZE);

		if (m_ChannelInfo[channelNum].m_fChannelRange < nMin)
		{
			m_ChannelInfo[channelNum].m_fChannelRange = nMin;
		}

		m_fRange = m_ChannelInfo[channelNum].m_fChannelRange;
		m_fCGain = m_ChannelInfo[channelNum].m_fChannelCPlus;
		m_fGain = m_ChannelInfo[channelNum].m_fChannelPlus;
		//更新抑制
		m_fSuppression = m_ChannelInfo[channelNum].m_fRestrain;
		//更新重复频率
		m_fReFreq = m_ChannelInfo[channelNum].m_ProbeInfo.m_wReFreq;
		DWORD dwDelay = MicroSecond2Dot(m_fDelay);
		m_fXMinRuler = Dot2Range(m_ChannelInfo[m_wChannelNo].m_nWaveSpeed, dwDelay);
		m_fXMaxRuler = m_fRange + m_fXMinRuler;
		emit AscanInfoChanged();
	}
}

void AScan::ChangeMapCPicData(WORD wChannelNum)
{
	BOOL bExist = FALSE;

	g_ADataMutex.lock();
	for (int i = 0; i < g_vecADataIndex.count(); ++i)
	{
		if (wChannelNum == g_vecADataIndex.value(i))
		{
			bExist = TRUE;

			g_vecADataIndex.remove(i);
			break;
		}
	}
	g_ADataMutex.unlock();

	if (!bExist)
	{
		g_ADataMutex.lock();
		g_vecADataIndex.push_back(wChannelNum);
		g_ADataMutex.unlock();
	}
	
}

void AScan::timerEvent( QTimerEvent *event )
{
	if (event->timerId() == m_nTIMER_Sel_Channel)
	{
		int mapSize = g_vecADataIndex.count();

		if (m_wMapCurrentChannelNum > mapSize)
		{
			m_wMapCurrentChannelNum = 1;
		}

		if (mapSize <= 1) // 单通道模式
		{
			return;
		}
		else							// 多通道轮训设置
		{
			int index = 0;
			for (int i = 0; i < mapSize; ++i)
			{
				index++;
				if (index == m_wMapCurrentChannelNum)
				{
					int tempChannel = g_vecADataIndex.value(i);
					if (tempChannel > 29)
					{		
						g_dwActiveChannel = tempChannel - 6;
						g_dwActiveFlawOrBottom = 0;
						m_bSelChannel = TRUE;
					}
					else
					{
						g_dwActiveChannel = tempChannel;
						g_dwActiveFlawOrBottom = 1;
						m_bSelChannel = TRUE;
					}
				}	
			}

			m_wMapCurrentChannelNum++;
		}
	}
}

void AScan::UpdateProbe()
{
	comboBoxProbe->clear();
	
	if (checkBoxProbe1->isChecked())
		comboBoxProbe->addItem(tr("纵伤探头1"));

	if (checkBoxProbe2->isChecked())
		comboBoxProbe->addItem(tr("纵伤探头2"));

	if (checkBoxProbe3->isChecked())
		comboBoxProbe->addItem(tr("横伤探头1"));

	if (checkBoxProbe4->isChecked())
		comboBoxProbe->addItem(tr("横伤探头2"));

	if (checkBoxProbe5->isChecked())
		comboBoxProbe->addItem(tr("分层探头"));

	if (checkBoxProbe6->isChecked())
		comboBoxProbe->addItem(tr("测厚探头"));	

	if (comboBoxProbe->count() > 0)
	{
		comboBoxProbe->setCurrentIndex(0);
	}
}

void AScan::CurSelectProbe()
{
	if (comboBoxProbe->count() > 0)
	{
		if (comboBoxProbe->currentText() == tr("纵伤探头1"))
		{
			m_nCurSelectProbe = 0;
			m_qsProbe = tr("纵伤探头1");
		}
		else if (comboBoxProbe->currentText() == tr("纵伤探头2"))
		{
			m_nCurSelectProbe = 1;
			m_qsProbe = tr("纵伤探头2");
		}
		else if (comboBoxProbe->currentText() == tr("横伤探头1"))
		{
			m_nCurSelectProbe = 2;
			m_qsProbe = tr("横伤探头1");
		}
		else if (comboBoxProbe->currentText() == tr("横伤探头2"))
		{
			m_nCurSelectProbe = 3;
			m_qsProbe = tr("横伤探头2");
		}
		else if (comboBoxProbe->currentText() == tr("分层探头"))
		{
			m_nCurSelectProbe = 4;
			m_qsProbe = tr("分层探头");
		}
		else if (comboBoxProbe->currentText() == tr("测厚探头"))
		{
			m_nCurSelectProbe = 5;
			m_qsProbe = tr("测厚探头");
		}
	}
	

	if (m_nCurSelectProbe > 3)
	{	
		m_wChannelNo = m_MapChannel_CONFIG[24 + m_nProbeBox];
	}
	else
	{
		m_wChannelNo = m_MapChannel_CONFIG[m_nProbeBox * 4 + m_nCurSelectProbe];
	}

	if (m_nCurSelectProbe == 5)
	{
		m_nFlawOrBottom = 0;
	}
	else
	{
		m_nFlawOrBottom = 1;
	}

	SetProbeInfo();
	SetGateInfo();

	g_dwActiveChannel = m_wChannelNo;
	g_dwActiveFlawOrBottom = m_nFlawOrBottom;
	m_bSelChannel = TRUE;
}

void AScan::InitChannelMap()
{
	// 	// 六个盒子，前置接线4231，重新编排对应方式 wangwei modified on 2014-6-13
	// 	for (int i = 0;i < 30;i++)
	// 	{
	// 		m_MapChannel_CONFIG[i] = i + 1;
	// 	}
	// 	for (int i = 30;i < 36;i++)
	// 	{
	// 		m_MapChannel_CONFIG[i] = i - 5;
	// 	}
	// 	for (int i = 1;i < 37;i++)
	// 	{
	// 		m_MapCONFIG_Channel[i] = i - 1;
	// 	}
	// 	// 六个盒子，前置接线4231，重新编排对应方式 wangwei modified on 2014-6-13

	// 六个盒子，更改前置接线4321 wangwei modified start on 2016-01-19
	for (int i = 0; i < 24; i++)
	{
		if (i % 4 == 1)
		{
			m_MapChannel_CONFIG[i] = i + 1;
			m_MapCONFIG_Channel[i] = i + 1;
		}
		else if (i % 4 == 2)
		{
			m_MapChannel_CONFIG[i] = i - 1;
			m_MapCONFIG_Channel[i] = i - 1;
		}
		else
		{
			m_MapChannel_CONFIG[i] = i;
			m_MapCONFIG_Channel[i] = i;
		}
	}

	for (int i = 24; i < 36; i++)
	{
		if (i < 30)
		{
			m_MapChannel_CONFIG[i] = i;
		}
		else
		{
			m_MapChannel_CONFIG[i] = i - 6;
		}	

		m_MapCONFIG_Channel[i] = i;
	}
	// 六个盒子，更改前置接线4321 wangwei modified end on 2016-01-19
}

void AScan::ProbeRemember()
{
	int n = comboBoxProbe->count();

	for (int i = 0; i < n; i++)
	{
		comboBoxProbe->setCurrentIndex(i);

		if (m_qsProbe == comboBoxProbe->currentText())
		{
			CurSelectProbe();
			break;
		}
		else if (i == n-1)
		{
			comboBoxProbe->setCurrentIndex(0);
			m_qsProbe = comboBoxProbe->currentText();
			CurSelectProbe();
		}
	}

	// ???
	if (n == 1)
		CurSelectProbe();
}
