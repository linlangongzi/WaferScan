#pragma once

#include <string.h>
#include <Windows.h>
#include <QDialog>
#include <QWidget>
#include <QPoint>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QRectF>
#include "ui_ascan.h"
#include "const.h"
#include <map>
#include "DataDefine.h"
#include "CAScanLock.h"		//Chen Yuzhu add on 2014-04-29

using namespace std;

class AScan : public QDialog,
    public Ui_AScanClass
{
	Q_OBJECT
public:
	CH_FLAWDATA_CSCAN_D m_CH_FLAWDATA_CSCAN_D;
	float m_fXMinRuler;		// 刻度尺最小值，即最小声程
	float m_fXMaxRuler;		// 刻度尺最大值，即最大声程	
	short m_nWaveSpeed;			// 声速
public:
	void SetAScanData(WORD wChannelNum,int nFlawOrBottom,CPicData* pPicData);    //设置A扫波形数据接口(只是UpDate)
	void UpdateActiveChannelInfo(WORD wActiveChannel);   //wangqianfen  加的
public:
	bool m_bSetOne;
	bool m_bSetAll;
	bool m_bSelChannel;
public:
    AScan(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~AScan();
public:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);//鼠标点击选中待操作闸门
	void mouseMoveEvent(QMouseEvent *e);//鼠标拖动响应函数
	void mouseReleaseEvent(QMouseEvent *e);//鼠标抬起响应
	void DistributeRegion();				//画图区域分配函数
	void GateIPainter(QPainter &painter);	//I闸门绘制
	void GateI2Painter(QPainter &painter);	//I2闸门绘制
	void GateMoveUpdate();	//整体拖动图和控件变化时会改变闸门的值
	void UpdateProbe();		//更新当前选中在探头
	void CurSelectProbe();   //选则当前选中的探头（用于通过界面修改数据）后，记录此探头是1还是2还是3（原先只是为了改变通道号和g_qsProbe）
	void ProbeRemember();    //切换探头盒后，通过CurSelectProbe（）记录的探头，比如“探头2”，如果此盒下探头2也是选中的，那么保留上盒的选中状态，否则选中由左侧第一个勾选的探头
	void DataBaseAscanSyn(); //从数据同步到A扫控件成员变量
	void ChannelNumChange();	//c扫探头盒切换函数及 //chenyuzhu add on 2014-11-3

public:
	CPicData m_stData;
	WORD m_wMapChannelNum;					//m_MapCPicData的顺序号，比通道号多5个

	WORD m_wMapCurrentChannelNum;
protected:
	void timerEvent( QTimerEvent *event );
public:
	int m_nTIMER_Sel_Channel;
	QPoint startPnt;         //鼠标点击的点在坐标
	QPoint m_pGateILeftTop;  //I闸门左上角的坐标
	QPoint m_pGateIRightBottom;//I闸门右下角的坐标
	QPoint m_pGateI2LeftTop;//I2闸门左上角的坐标
	QPoint m_pGateI2RightBottom;//I2闸门右下角的坐标
signals:
	void AscanInfoChanged();     //拖图，通道切换 做判断的
	void AscanParameterChanged();   //没用到

public slots:
	void ProbeChange();			//右侧 探头选择函数

private slots:
	void AScanLock();			//A扫控件锁定与解锁
	void toolbar_show();		//工具栏显示函数
	void toolbar_hide();		//工具栏隐藏函数

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
	void CheckBoxProbeChange(int i);	// 探头开关处理函数
	void radioButtonFBChange();			//伤波底波切换
//控件改变响应的函数
	void GateFrontChange();	  //闸门前沿变量跟踪滑块变化函数  （）
	void GateWidthChange();	//闸门宽度变量跟踪滑块变化函数
	void GatePeakChange();	//闸门阈值变量跟踪滑块变化函数
	void DelayChange();		//延迟变量跟踪滑块变化函数
	void RangeChange();		//X轴范围变量跟踪滑块变化函数
	void GainChange();		//增益变量跟踪滑块变化函数
	void SuppressionChange();//抑制变量跟踪滑块变化函数
	void AscanInfoUpdate();	//各滑块变量更新函数

	void slotSlider_SpinboxGateFront();//Slider只识别int，DoubleSpinBox识别Double类型，传数据需要以下的精度保留操作
	void slotSpinbox_SliderGateFront();
	void slotSlider_SpinboxGateWidth();
	void slotSpinbox_SliderGateWidth();
	void slotSlider_SpinboxGatePeak();
	void slotSpinbox_SliderGatePeak();
	void slotSlider_SpinboxDelay();
	void slotSpinbox_SliderDelay();
	void slotSlider_SpinboxXRange();
	void slotSpinbox_SliderXRange();
	void slotSlider_SpinboxSuppression();
	void slotSpinbox_SliderSuppression();
	void slotSlider_SpinboxGain();
	void slotSpinbox_SliderGain();

	void slotSpinBox_ZhongZhi();
	void slotSpinBox_ZhengYiXiShu();

public slots:
	void ProbeBoxChange();		//探头盒切换函数
	void GateBoxChange(int index); //闸门切换函数 //chenyuzhu add start on 2014-11-2
public:
		void UiInit();			//Ui初始化
private:
	void DrawGrid(QPainter &painter, const QRect& rect,double dXMove,int *nGrid,int nlines);	//画网格函数
	void DrawHRuler(QPainter &painter, const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//画水平标尺函数  
	void DrawVRuler(QPainter &painter, const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//画纵向标尺函数 
	void DrawWave(QPainter &painter, const QRect& rect);	//画波形
	void DataMemberInit();	//成员变量初始化
	void GetScreenInfo();
	bool CaculatePeakValue(const float& fDistance1, const float& fDistance2,WORD dataDraw[], int nSize,int& nIndex, int& nValue);
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
	//QString m_qsProbe;当前选定探头
public:
	int m_ncomboBoxGate;	//闸门选择控件变量（没用到）
	int m_nProbeBox;		//探头盒选择控件变量
	//int m_nProbe;			//选择探头号
	int m_nFlawOrBottom;	//伤波底波切换变量 0是底波 1是伤波
	float m_fGateFrontOut;	//闸门前沿滑块变量
	float m_fGateFrontIn;   //切换通道读数据结构式变化，直接给控件赋值
	float m_fGateWidthOut;	//闸门宽度滑块变量
	float m_fGateWidthIn;
	float m_fGatePeakOut;	//闸门阈值滑块变量
	float m_fGatePeakIn;	
	float m_fDelayOut;		//延迟滑块变量
	float m_fRangeOut;		//显示范围滑块变量
	float m_fGainOut;		//增益滑块变量
	float m_fSuppressionOut;//抑制滑块变量

	float m_fZeroPoint;
	float m_fDelay;
	float m_fRange;
	float m_fReFreq;
	float m_fGain;
	float m_fSuppression;
	int m_nActScreenX;		//屏幕的尺寸
	int m_nActScreenY;
	int m_nScreenCount;		//屏幕个数
	int m_nGrid[20];
	int m_nLines;

	float m_fGateILeft;
	float m_fGateITop;
	float m_fGateIBottom;
	float m_fGateIRight;
	float m_fGateIMiddle;
	float m_fGateI2Left;
	float m_fGateI2Top;
	float m_fGateI2Bottom;
	float m_fGateI2Right;
	float m_fGateI2Middle;
	float m_fGateILeftUpdate;
	float m_fGateITopUpdate;
	float m_fGateI2LeftUpdate;
	float m_fGateI2TopUpdate;

	float m_fIGateFront;
	float m_fIGateWidth;
	float m_fIGatePeak;
	float m_fAGateFront;
	float m_fI2GateFront;
	float m_fI2GateWidth;	
	float m_fGateI2backIn;   //底波闸门后沿------------------//最大底波位置	
	float m_fI2GatePeak;
	float m_fSync;
	QString m_qsProbe;      //当前选定探头
private:
	bool m_bProbeOnOrOff[47][3]; //各通道探头开关标志数组
	bool m_bGateI;			//闸门开关状态变量
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
	bool m_bGateIFirstPressed;
	bool m_bGateI2FirstPressed;

	bool m_bGateMove;
	bool m_bGateLeft;
	bool m_bGateRight;
	int m_ndwActiveNum;
	
	//chenyuzhu add start on 2014.4.29
public:
	CAscanLock *m_Lock;
	CAscanUnLock *m_UnLock;

	//chenyuzhu add end on 2014.4.29
};
