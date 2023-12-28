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
	float m_fXMinRuler;		// �̶ȳ���Сֵ������С����
	float m_fXMaxRuler;		// �̶ȳ����ֵ�����������	
	map<WORD,WORD> m_MapChannel_CONFIG;
	map<WORD,WORD> m_MapCONFIG_Channel;
	void InitChannelMap();
public:
	short m_nWaveSpeed;			// ����
public:
	void SetAScanData(WORD wChannelNum,int nFlawOrBottom,CPicData* pPicData);//����Aɨ�������ݽӿ�
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
	void DistributeRegion();	//��ͼ������亯��
	void setProbeInfo(void);	//����̽ͷ��������
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
	WORD m_wMapChannelNum;   //m_MapCPicData��˳��ţ���ͨ���Ŷ�5��
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
	void ProbeChange();			//̽ͷѡ����
private slots:
	void toolbar_show();		//��������ʾ����
	void toolbar_hide();		//���������غ���
	void GateChange();			//բ���л�����
	void ProbeBoxChange();		//̽ͷ���л�����

	void pushButtonSaveScreenRespond();		//����
	void pushButtonRecordVideoRespond();	//¼��
	void pushButtonFreezeWaveRespond();		//��������
	void pushButtonDisplayParaSetRespond();	//����������ʾ
	void pushButtonFunctionhideRespond();	//���ܰ�ť����
	void pushButtonAdd2Respond();			//+2dB
	void pushButtonCut2Respond();			//-2dB
	void pushButtonAScanSaveRespond();		//����

	void checkBoxProbe1Change();		//̽ͷ1���غ���
	void checkBoxProbe2Change();		//̽ͷ2���غ���
	void checkBoxProbe3Change();		//̽ͷ3���غ���
	void checkBoxGateSwitchChange();	//բ�ſ��غ���

	void GateFrontChange();	//բ��ǰ�ر������ٻ���仯����
	void GateWidthChange();	//բ�ſ�ȱ������ٻ���仯����
	void GatePeakChange();	//բ����ֵ�������ٻ���仯����
	void ZeroPointChange();	//���������ٻ���仯����
	void DelayChange();		//�ӳٱ������ٻ���仯����
	void RangeChange();		//X�᷶Χ�������ٻ���仯����
	void ReferGainChange();	//�ο�����������ٻ���仯����
	void ReFreqChange();		//�ظ�Ƶ�ʱ������ٻ���仯����
	void GainChange();		//����������ٻ���仯����
	void SuppressionChange();//���Ʊ������ٻ���仯����

 	void AscanInfoUpdate();	//������������º���


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
	void DrawGrid(const QRect& rect,double dXMove,int *nGrid,int nlines);	//��������
	void DrawHRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//��ˮƽ��ߺ���  
	void DrawVRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//�������ߺ��� 
	void DrawWave(const QRect& rect);	//������
	void UiInit();			//Ui��ʼ��
	void DataMemberInit();	//��Ա������ʼ��
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
	int m_ncomboBoxGate;	//բ��ѡ��ؼ�����
	int m_nProbeBox;		//̽ͷ��ѡ��ؼ�����
	int m_nProbe;			//ѡ��̽ͷ��
	int m_nFlawOrBottom;	//�˲��ײ��л����� 0�ǵײ� 1���˲�
	float m_fGateFrontOut;	//բ��ǰ�ػ������
	float m_fGateFrontIn;
	float m_fGateWidthOut;	//բ�ſ�Ȼ������
	float m_fGateWidthIn;
	float m_fGatePeakOut;	//բ����ֵ�������
	float m_fGatePeakIn;	
	float m_fZeroPointOut;	//��㻬�����
	float m_fDelayOut;		//�ӳٻ������
	float m_fRangeOut;		//��ʾ��Χ�������
	float m_fReferGainOut;	//�ο����滬�����
	float m_fReFreqOut;		//�ظ�Ƶ�ʻ������
	float m_fGainOut;		//���滬�����
	float m_fSuppressionOut;//���ƻ������

	float m_fZeroPoint;
	float m_fDelay;
	float m_fRange;
	float m_fCGain;
	float m_fReFreq;
	float m_fGain;
	float m_fSuppression;

	int m_nActScreenX;		//��Ļ�ĳߴ�
	int m_nActScreenY;
	int m_nScreenCount;		//��Ļ����
	int m_nGrid[20];
	int m_nLines;
	int m_nCurSelectProbe;	//��ǰѡ��̽ͷ

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
	bool m_bProbeOnOrOff[47][3]; //��ͨ��̽ͷ���ر�־����
	bool m_bGateI;			//բ�ſ���״̬����
	bool m_bGateA;
	bool m_bGateB;
	bool m_bGateI2;

	bool m_bProbe1Box1;		//����̽ͷ״̬����
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
