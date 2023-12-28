#pragma once

#include <string.h>
#include <Windows.h>
#include <QtGui/QDialog>
#include <QWidget>
#include <QPoint>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QRectF>
#include "ui_ascan.h"
#include "const.h"
#include "DataDefine.h"
#include <map>
using namespace std;

class AScan : public QDialog,
	public Ui_AScanClass
{
	Q_OBJECT
public:
	CH_CONFIG m_stAllCH_CONFIG[CHANNEL_NUM];
	WORD m_wChannelNo;
	CChannelInfo m_ChannelInfo[CHANNEL_NUM];
	CH_FLAWDATA_CSCAN_D m_CH_FLAWDATA_CSCAN_D;
	float m_fXMinRuler;		// 刻度尺最小值，即最小声程
	float m_fXMaxRuler;		// 刻度尺最大值，即最大声程	
	map<WORD,WORD> m_MapChannel_CONFIG;
	map<WORD,WORD> m_MapCONFIG_Channel;
	void InitChannelMap();
public:
	short m_nWaveSpeed;			// 声速
public:
	void SetAScanData(WORD wChannelNum,int nFlawOrBottom,CPicData* pPicData);//设置A扫波形数据接口
	void InitChannelInfo();
	void InitConfigInfo();
	void SetGateInfo(void);
	void SetProbeInfo(void);
public:
	bool m_bSetOne;
	bool m_bSetAll;
	bool m_bSelChannel;
public:
	AScan(QWidget *parent = 0, Qt::WFlags flags = 0);
	~AScan();
public:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void DistributeRegion();	//画图区域分配函数
	void setProbeInfo(void);	//设置探头参数函数
	void GateIPainter();
	void GateAPainter();
	void GateBPainter();
	void GateI2Painter();
	void GateMoveUpdate();
	void UpdateProbe();
	void CurSelectProbe();
	void ProbeRemember();
public:
	CPicData m_stData;
	map<WORD,CPicData*> m_MapCPicData;
	void ChangeMapCPicData(WORD wChannelNum);
	WORD m_wMapChannelNum;   //m_MapCPicData的顺序号，比通道号多5个
    WORD m_wMapCurrentChannelNum;
protected:
	void timerEvent( QTimerEvent *event );
	int m_nTIMER_Sel_Channel;
public:
	QPoint startPnt;
	QPoint m_pGateILeftTop;
	QPoint m_pGateIRightBottom;
	QPoint m_pGateALeftTop;
	QPoint m_pGateARightBottom;
	QPoint m_pGateBLeftTop;
	QPoint m_pGateBRightBottom;
	QPoint m_pGateI2LeftTop;
	QPoint m_pGateI2RightBottom;
signals:
	void AscanInfoChanged();
public slots:
	void ProbeChange();			//探头选择函数
private slots:
	void toolbar_show();		//工具栏显示函数
	void toolbar_hide();		//工具栏隐藏函数
	void GateChange();			//闸门切换函数
	void ProbeBoxChange();		//探头盒切换函数

	void pushButtonSaveScreenRespond();		//截屏
	void pushButtonRecordVideoRespond();	//录像
	void pushButtonFreezeWaveRespond();		//冰冻波形
	void pushButtonDisplayParaSetRespond();	//悬浮参数显示
	void pushButtonFunctionhideRespond();	//功能按钮隐藏
	void pushButtonAdd2Respond();			//+2dB
	void pushButtonCut2Respond();			//-2dB
	void pushButtonAScanSaveRespond();		//保存

	void checkBoxProbe1Change();		//探头1开关函数
	void checkBoxProbe2Change();		//探头2开关函数
	void checkBoxProbe3Change();		//探头3开关函数
	void checkBoxGateSwitchChange();	//闸门开关函数

	void GateFrontChange();	//闸门前沿变量跟踪滑块变化函数
	void GateWidthChange();	//闸门宽度变量跟踪滑块变化函数
	void GatePeakChange();	//闸门阈值变量跟踪滑块变化函数
	void ZeroPointChange();	//零点变量跟踪滑块变化函数
	void DelayChange();		//延迟变量跟踪滑块变化函数
	void RangeChange();		//X轴范围变量跟踪滑块变化函数
	void ReferGainChange();	//参考增益变量跟踪滑块变化函数
	void ReFreqChange();		//重复频率变量跟踪滑块变化函数
	void GainChange();		//增益变量跟踪滑块变化函数
	void SuppressionChange();//抑制变量跟踪滑块变化函数

 	void AscanInfoUpdate();	//各滑块变量更新函数


	void slotSlider_SpinboxGateFront();
	void slotSpinbox_SliderGateFront();
	void slotSlider_SpinboxGateWidth();
	void slotSpinbox_SliderGateWidth();
	void slotSlider_SpinboxGatePeak();
	void slotSpinbox_SliderGatePeak();
	void slotSlider_SpinboxZeroPoint();
	void slotSpinbox_SliderZeroPoint();
	void slotSlider_SpinboxDelay();
	void slotSpinbox_SliderDelay();
	void slotSlider_SpinboxXRange();
	void slotSpinbox_SliderXRange();
	void slotSlider_SpinboxYRange();
	void slotSpinbox_SliderYRange();
	void slotSlider_SpinboxReFreq();
	void slotSpinbox_SliderReFreq();
	void slotSlider_SpinboxSuppression();
	void slotSpinbox_SliderSuppression();
	void slotSlider_SpinboxGain();
	void slotSpinbox_SliderGain();
private:
	void DrawGrid(const QRect& rect,double dXMove,int *nGrid,int nlines);	//画网格函数
	void DrawHRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//画水平标尺函数  
	void DrawVRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//画纵向标尺函数 
	void DrawWave(const QRect& rect);	//画波形
	void UiInit();			//Ui初始化
	void DataMemberInit();	//成员变量初始化
	void GetScreenInfo();
	// private:
	// 	Ui::AScanClass ui;
private:
	QRect m_rectWave;
	QRect m_rectHRuler;
	QRect m_rectVRuler;
	QRect m_rectBlank;
	QRect m_rectGateI;
	QRect m_rectGateA;
	QRect m_rectGateB;
	QRect m_rectGateI2;
	QString m_qsProbe;
public:
	int m_ncomboBoxGate;	//闸门选择控件变量
	int m_nProbeBox;		//探头盒选择控件变量
	int m_nProbe;			//选择探头号
	int m_nFlawOrBottom;	//伤波底波切换变量 0是底波 1是伤波
	float m_fGateFrontOut;	//闸门前沿滑块变量
	float m_fGateFrontIn;
	float m_fGateWidthOut;	//闸门宽度滑块变量
	float m_fGateWidthIn;
	float m_fGatePeakOut;	//闸门阈值滑块变量
	float m_fGatePeakIn;	
	float m_fZeroPointOut;	//零点滑块变量
	float m_fDelayOut;		//延迟滑块变量
	float m_fRangeOut;		//显示范围滑块变量
	float m_fReferGainOut;	//参考增益滑块变量
	float m_fReFreqOut;		//重复频率滑块变量
	float m_fGainOut;		//增益滑块变量
	float m_fSuppressionOut;//抑制滑块变量

	float m_fZeroPoint;
	float m_fDelay;
	float m_fRange;
	float m_fCGain;
	float m_fReFreq;
	float m_fGain;
	float m_fSuppression;

	int m_nActScreenX;		//屏幕的尺寸
	int m_nActScreenY;
	int m_nScreenCount;		//屏幕个数
	int m_nGrid[20];
	int m_nLines;
	int m_nCurSelectProbe;	//当前选中探头

	float m_fGateILeft;
	float m_fGateITop;
	float m_fGateIBottom;
	float m_fGateIRight;
	float m_fGateIMiddle;
	float m_fGateALeft;
	float m_fGateATop;
	float m_fGateABottom;
	float m_fGateARight;
	float m_fGateAMiddle;
	float m_fGateBLeft;
	float m_fGateBTop;
	float m_fGateBBottom;
	float m_fGateBRight;
	float m_fGateBMiddle;
	float m_fGateI2Left;
	float m_fGateI2Top;
	float m_fGateI2Bottom;
	float m_fGateI2Right;
	float m_fGateI2Middle;
	float m_fGateILeftUpdate;
	float m_fGateITopUpdate;
	float m_fGateALeftUpdate;
	float m_fGateATopUpdate;
	float m_fGateBLeftUpdate;
	float m_fGateBTopUpdate;
	float m_fGateI2LeftUpdate;
	float m_fGateI2TopUpdate;

	float m_fIGateFront;
	float m_fIGateWidth;
	float m_fIGatePeak;
	float m_fAGateFront;
	float m_fAGateWidth;
	float m_fAGatePeak;
	float m_fBGateFront;
	float m_fBGateWidth;
	float m_fBGatePeak;
	float m_fI2GateFront;
	float m_fI2GateWidth;
	float m_fI2GatePeak;
	float m_fSync;
private:
	bool m_bProbeOnOrOff[47][3]; //各通道探头开关标志数组
	bool m_bGateI;			//闸门开关状态变量
	bool m_bGateA;
	bool m_bGateB;
	bool m_bGateI2;

	bool m_bProbe1Box1;		//各盒探头状态变量
	bool m_bProbe1Box2;
	bool m_bProbe1Box3;
	bool m_bProbe1Box4;
	bool m_bProbe1Box5;
	bool m_bProbe2Box1;
	bool m_bProbe2Box2;
	bool m_bProbe2Box3;
	bool m_bProbe2Box4;
	bool m_bProbe2Box5;
	bool m_bProbe3Box1;
	bool m_bProbe3Box2;
	bool m_bProbe3Box3;
	bool m_bProbe3Box4;
	bool m_bProbe3Box5;
	bool m_bProbe4Box1;
	bool m_bProbe4Box2;
	bool m_bProbe4Box3;
	bool m_bProbe4Box4;
	bool m_bProbe4Box5;
	bool m_bProbe5Box1;
	bool m_bProbe5Box2;
	bool m_bProbe5Box3;
	bool m_bProbe5Box4;
	bool m_bProbe5Box5;
	bool m_bProbe6Box1;
	bool m_bProbe6Box2;
	bool m_bProbe6Box3;
	bool m_bProbe6Box4;
	bool m_bProbe6Box5;

	bool m_bGateIIsPressed;
	bool m_bGateAIsPressed;
	bool m_bGateBIsPressed;
	bool m_bGateI2IsPressed;
	bool m_bGateIIsPresse;
	bool m_bGateILIsPresse;
	bool m_bGateIRIsPresse;
	bool m_bGateAIsPresse;
	bool m_bGateALIsPresse;
	bool m_bGateARIsPresse;
	bool m_bGateBIsPresse;
	bool m_bGateBLIsPresse;
	bool m_bGateBRIsPresse;
	bool m_bGateI2IsPresse;
	bool m_bGateI2LIsPresse;
	bool m_bGateI2RIsPresse;

	bool m_bGateMove;
	bool m_bGateLeft;
	bool m_bGateRight;
};
