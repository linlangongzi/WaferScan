#ifndef GUANTAN_H
#define GUANTAN_H

#include <QtGui/QDialog>
#include "ui_guantan.h"
#include "ui_ascan.h"
#include "ascan.h"
#include <Windows.h>
#include <QtCore/QSettings>
#include <QSettings>
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI��������̬���ӿ�
#include "..\pci9054_dll\p9054_lib.h"
#include <windows.h>
#include "const.h"
#include <qobject.h>
#include <QEvent>
#include "DataDefine.h"
#include "PLXCommunication.h"
#include "EventDefine.h"
#include "parametersetting.h"
#include "usermanagement.h"
#include "formitem.h"
#include <vector>
#include "SimpleFree.h"
#include "traversalfile.h"
using namespace std;

class GuanTan : public QDialog,
	public Ui_GuanTanClass
{
	Q_OBJECT

public:
	GuanTan(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GuanTan();
public:
	void paintEvent(QPaintEvent *event);	//Qt���ƺ���
	void mousePressEvent(QMouseEvent *e);	//�������Ӧ����
	void mouseMoveEvent(QMouseEvent *e);	//����ƶ���Ӧ����
	void mouseReleaseEvent(QMouseEvent *e);	//���̧����Ӧ����
	void GetScreenInfo();		//�����Ļ�ֱ���
	void DistributeRegion();	//��ͼ������亯��
	void UiInit();				//UI��ʼ��
	void TestResultProcess();	//ȱ��ͳ�ƺ���
	void CalcFlaw(vector<QPointF> vecPoint,int& nFlawNum,int& nMaxFlawSize,float& fMaxFlawHeight);	//ȱ��ͳ��-������ ���ⲿ��������
	void CalcBadCouple(vector<QPointF> vecPoint,int& nFlawNum,int& nMaxFlawSize);					//ȱ��ͳ��-��������ϲ���
	void CalcThick(vector<QPointF> vecPointHigh,vector<QPointF> vecPointLow);						//ȱ��ͳ��-�����ڱں����
	void DataJudge();			//���������
	void ReadOPCItem(OPCHANDLE *hServer,int nItemCount); //��ȡopc
	BOOL WriteOPCItem(OPCHANDLE* phServer, VARIANT* itemArray,  const unsigned long & ulSize);//дopc
private:
	int m_nRealSteelL;		//��������¼��ʵ�ʸֹܳ��� ���ڼ���ä��wangwei add on 2013-12-14
	bool m_bSteelLengthIsValue;	//ÿ�μ���Ƿ���ֹ�ʵ�ʳ��ȸ�ֵ�� wangwei add on 2013-12-14
	void ZeroEncipheror();		//����������
	float m_fEncipheror;		//��ǰ��������ֵ
	float m_fEncipheror_mark;	//������������
	WORD m_wStartFlag;			//̽��ʹ�ܣ�0��Ϊ������1��Ϊ��ʼ
	BOOL m_bRunFlag;			//̽�˹�������Ϊ1������Ϊ0��
	BOOL m_bOver;				//����������
	DWORD m_dwProbeBoxUpDown;	//̽ͷ������״̬	wangwei add on 2013-12-12
	int m_nStartDeadZone;		//ʼ��ä��	wangwei add on 2013-12-12
	int m_nEndDeadZone;			//ĩ��ä��	wangwei add on 2013-12-12
	void SaveData(QString fileName,vector<QPointF> ptVector);	//�����ȱ��ɨ��ԭʼ����
	Traversalfile m_FindData;
	QList<QString> m_DataList;
	QString m_strCurrentDataPath;
	void ReadDataFile(QString& strDataFile);//��ȡ���������ļ�
	void ReadDataFileAndDraw(QString strDataFile,QRect rectFeature,QPixmap& bmpFeature,QColor clrFeature);	//������ϲ�����
	void ReadDataFileAndDraw_V(QString strDataFile,QRect rectFeature,QPixmap& bmpFeature,QColor clrFeature);//��ȡ����ͼ����ȱ��
	void ClearScreen();						//������ÿ�ο�ʼ�µ�̽����߶�ȡ�µ��ļ�ǰ����
	void keyPressEvent(QKeyEvent *event);	//��дesc��Ӧ����
public:
	CSimpleFree m_OPCComponent;
private:
	BOOL bDBorPCT;//FalseΪ�ݱ��DB��ʾ��TRUEΪ�ݱ�߰ٷֱ���ʾ��
	BOOL m_bInFeacher;		//ɨ����������ڴ����ڲ�ȱ�ݴ��ڵı�־
	BOOL m_bOutFeacher;		//ɨ����������ڴ����ⲿȱ�ݴ��ڵı�־
	BOOL m_bThickFeacher;	//ɨ����������ڴ��ıں�ֲ�ȱ�ݴ��ڵı�־
	BOOL m_bEncipherorChange2;//������ֵ����10����
	BOOL m_bEncipherorChange3;//������ֵ����10����
	BOOL m_bEncipherorChange4;//������ֵ����10����
	BOOL m_bEncipherorChange5;//������ֵ����10����
private:
	QPointF ptOutVFeatureStartPoint;		//�ⲿ����ȱ��ͼ������ʼ��
	QPointF ptInVFeatureStartPoint;			//�ڲ�����ȱ��ͼ������ʼ��
	QPointF ptOutHFeatureStartPoint;		//�ⲿ����ȱ��ͼ������ʼ��
	QPointF ptInHFeatureStartPoint;			//�ڲ�����ȱ��ͼ������ʼ��
	QPointF ptThickFeatureStartPoint;		//�ں����ȱ��ͼ������ʼ��
	QPointF ptLayerFeatureStartPoint;		//�ֲ�ȱ��ͼ������ʼ��
	QPointF ptBadCoupleFeatureStartPoint;	//��ϲ���������ʼ��
	QPixmap bmpVFeature;			//����ȱ��ͼ����
	QPixmap bmpHFeature;			//����ȱ��ͼ����
	QPixmap bmpThickFeature;		//���ȱ��ͼ����
	QPixmap bmpLayerFeature;		//�ֲ�ȱ��ͼ����
	QPixmap bmpBadCoupleFeature;	//��ϲ�������
	vector<QPointF> m_vectorOutVFeature;		//�ⲿ����ȱ�����ߵ㼯
	vector<QPointF> m_vectorInVFeature;			//�ڲ�����ȱ�����ߵ㼯
	vector<QPointF> m_vectorOutHFeature;		//�ⲿ����ȱ�����ߵ㼯
	vector<QPointF> m_vectorInHFeature;			//�ڲ�����ȱ�����ߵ㼯
	vector<QPointF> m_vectorThickHighFeature;	//���-����ȱ�ݺ������ߵ㼯
	vector<QPointF> m_vectorThickLowFeature;	//���-����ȱ�ݺ������ߵ㼯
	vector<QPointF> m_vectorThickFeature1;		//���-ƽ���ں����ߵ㼯
	vector<QPointF> m_vectorThickFeature2;		//���-ƽ���ں����ߵ㼯
	vector<QPointF> m_vectorThickFeature3;		//���-ƽ���ں����ߵ㼯
	vector<QPointF> m_vectorThickFeature4;		//���-ƽ���ں����ߵ㼯
	vector<QPointF> m_vectorThickFeature5;		//���-ƽ���ں����ߵ㼯
	vector<QPointF> m_vectorThickTempFeature1;	//���-̽ͷ��1-10mm�����е���ʱ�洢
	vector<QPointF> m_vectorThickTempFeature2;	//���-̽ͷ��2-10mm�����е���ʱ�洢
	vector<QPointF> m_vectorThickTempFeature3;	//���-̽ͷ��3-10mm�����е���ʱ�洢
	vector<QPointF> m_vectorThickTempFeature4;	//���-̽ͷ��4-10mm�����е���ʱ�洢
	vector<QPointF> m_vectorThickTempFeature5;	//���-̽ͷ��5-10mm�����е���ʱ�洢
	vector<QPointF> m_vectorLayerFeature;		//�ֲ�ȱ�����ߵ㼯
	vector<QPointF> m_vectorBadCoupleFeature;	//��ϲ������ߵ㼯
private:
	int m_nOutVFeatureFlawNum;			//�ⲿ����ȱ������
	int m_nInVFeatureFlawNum;			//�ڲ�����ȱ������
	int m_nOutHFeatureFlawNum;			//�ⲿ����ȱ������
	int m_nInHFeatureFlawNum;			//�ڲ�����ȱ������
	int m_nThickFeatureFlawNumAdd;		//�ں�-����ȱ������
	int m_nThickFeatureFlawNumCut;		//�ں�-����ȱ������
	int m_nLayerFeatureFlawNum;			//�ֲ�ȱ������
	int m_nBadCoupleFeatureFlawNum;		//ż�ϲ���ȱ������
	int m_nOutVFeatureMaxFlawSize;		//�ⲿ�������ȱ�ݳߴ�
	int m_nInVFeatureMaxFlawSize;		//�ڲ��������ȱ�ݳߴ�
	int m_nOutHFeatureMaxFlawSize;		//�ⲿ�������ȱ�ݳߴ�
	int m_nInHFeatureMaxFlawSize;		//�ڲ��������ȱ�ݳߴ�
	int m_nThickFeatureMaxFlawSizeAdd;	//�ں�-�������ȱ�ݳߴ�
	int m_nThickFeatureMaxFlawSizeCut;	//�ں�-�������ȱ�ݳߴ�
	int m_nLayerFeatureMaxFlawSize;		//�ֲ�ȱ�����ȱ�ݳߴ�
	int m_nBadCoupleFeatureMaxFlawSize;	//��ϲ������ȱ�ݳߴ�
	int m_nBadCoupleBox[5];				//̽ͷ����ϲ�����������
	int m_nBadCoupleNum;				//���߲鿴��ϲ�������
	int m_nBadCoupleBoxNum;				//������ϲ�������ֵ
	float m_fOutVFeatureMaxFlawGain;	//�ⲿ����ȱ����󳤶���ռ�ٷֱ�
	float m_fInVFeatureMaxFlawGain;		//�ڲ�����ȱ����󳤶���ռ�ٷֱ�
	float m_fOutHFeatureMaxFlawGain;	//�ⲿ����ȱ����󳤶���ռ�ٷֱ�
	float m_fInHFeatureMaxFlawGain;		//�ڲ�����ȱ����󳤶���ռ�ٷֱ�
	float m_fThickFeatureMaxFlawGain;	//�ں�����ȱ����󳤶���ռ�ٷֱ�
	float m_fThickFeatureMinFlawGain;	//�ں����ȱ����󳤶���ռ�ٷֱ�
	float m_fLayerFeatureMax;			//�ֲ�ȱ����󳤶���ռ�ٷֱ�
	float m_fPIPE_LENGTH;				//�����ʾ����
	float m_fThickMaxTolerance;			//�ں�������󹫲�
	float m_fThickMinTolerance;			//�ں�������С����
	float m_fRangeMax;					//���Χ
	float m_fRangeMin;					//��С��Χ
	float m_fCurThick;					//��ǰ�ں�
	float m_fCurPersent;				//�ں��׼��ռ����
	float m_fProbeCorrect[5];			//5�����Ӳ��̽ͷУ׼
private:
	int m_nTOTAL;					//����ɨ����-����
	int m_nACCEPTS_CONT;			//����ɨ����-�ϸ�Ʒ����
	float m_fACCEPTS_PERSENT;		//����ɨ����-�ϸ�Ʒ�ٷֱ�
	QString m_strACCEPTS_PERSENT;	//����ɨ����-�ϸ�Ʒ�ٷֱ����ڱ����ӡ
	int m_nREJECTS_CONT;			//����ɨ����-���ϸ�Ʒ����
	float m_fREJECTS_PERSENT;		//����ɨ����-���ϸ�Ʒ�ٷֱ�
	QString m_strREJECTS_PERSENT;	//����ɨ����-���ϸ�Ʒ�ٷֱ����ڱ����ӡ
	QString m_strSingleReport1;	//�ⲿ����
	QString m_strSingleReport2;	//�ڲ�����
	QString m_strSingleReport3;	//�ⲿ����
	QString m_strSingleReport4;	//�ڲ�����
	QString m_strSingleReport5;	//�ں�����
	QString m_strSingleReport6;	//�ں����
	QString m_strSingleReport7;	//�ֲ�
	QString m_strSingleReport8;	//��ϲ���
	QString m_strSingleReport9;	//ƽ���ں�
	QString m_strJudgeResult;	//������н��
	QString m_strNewResultFile;	//�ոռ����ϵĸֹ����ݵ��������ļ�·��
	QRect m_rectSingleReprot;
private:
	QString m_qsShowOrder;		//����������ʾ
	QString m_qsShowShow;		//��ʾ������ʾ
	QString m_qsShowJudge;		//���в�����ʾ
	QString m_qsShowUltrasound;	//����������ʾ
	QString m_qsShowAll;		//���������ʾ
	QString m_strFormItem;		//���������ļ�·��
	QString m_strOperator;		//������
	QString m_strSteelTestDate;	//�ֹܼ������
	QString m_strSteelTestTime;	//�ֹܼ��ʱ��
private:
	void DrawHRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//ˮƽ��� 
	void DrawVRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//��ֱ���
	void LeftPartShow();		//������������ʾ��ȱ��������ȱ�ݳ��ȣ�ȱ�ݰٷֱ�
	void InitializeConfig();			//�����·����ݵĽṹ��ĳ�ʼ��
	void DrawFeatureInfo();				//������ȱ�ݻ�ͼ
	void DrawOutVFeature(int i);		//���ⲿ����ȱ��ͼ
	void DrawInVFeature(int i);			//���ڲ�����ȱ��ͼ
	void DrawOutHFearure(int i);		//���ⲿ����ȱ��ͼ
	void DrawInHFearure(int i);			//���ڲ�����ȱ��ͼ
	void DrawThickFeature(int i);		//�����ȱ��ͼ
	void DrawLayerFeature(int i);		//���ֲ�ȱ��ͼ
	void DrawBadCoupleFeature(int i);	//�����ȱ��ͼ
	void Order_Parameter();		//���ļ����¶���������UI
	void Ultrasound_Parameter();//���ļ����³���������UI
	void Show_Parameter();		//���ļ�������ʾ������UI
	void Judge_Parameter();		//���ļ��������в�����UI
	void BurstDelay();			//���ļ������ӳٲ�����UI
	void CGain();				//���ļ����²ο����������UI
	void Gain();				//���ļ��������������UI
	void ProbeSet();			//���ļ�����̽ͷ���ò�����UI
	void ReFreq();				//���ļ������ظ�Ƶ�ʲ�����UI
	void SoundSpeed();			//���ļ��������ٲ�����UI
	void Width();				//���ļ����´��������UI
	void ImpedanceSet();		//���ļ������迹������UI
	void DeadZoneSet();			//���ļ�����ä��������UI wangwei add on 2013-12-14
	void Suppression();			//���ļ��������Ʋ�����UI wangwei add on 2013-12-14
	void ReadEddyCurrent();		//��ȡ�������
	void customEvent(QEvent *event);//
	void closeEvent(QCloseEvent *event);//�˳��źŴ�����
	int	m_nWaveSpeed;
	int m_nThick;
public slots:
	void WriteReFreq();//�·��ظ�Ƶ��
signals:
	void DefectAlarmChanged();  //ȱ�ݱ����ź�
	void CouplingAlarmChanged();//��ϱ����ź�
private slots:
//Aɨ
	void AScanAction();		//Aɨ��ť��Ӧ����	
	void AScanSave();		//Aɨ���������ť
//��������
	void FormSetting();		//�������ô�����ʾ
	void ShowOpen();		//��ʾ���ڴ�
	void ShowDo();			//��ʾ����
	void UltrasoundOpen();	//�������ڴ�
	void UltrasoundDo();	//��������
	void OrderOpen();		//�������ڴ�
	void OrderDo();			//��������
	void JudgeOpen();		//���д��ڴ�
	void JudgeDo();			//���е���
	void PrintScreen();					//�����ӡ��Ӧ����
	void PaintScreen(QPrinter *printer);//�����ӡ�Ļ���
	void UserManagementShow();			//�û�������ʾ
	void WriteEddyCurrent();			//�����������ļ�д��Ϣ
	void DateTimeUpdate();				//����ʱ����ʾ
	void filePrintPreviewSingle();				//���������ӡԤ��
	void PrintSingle();							//��������ֱ�Ӵ�ӡ
	void printPreviewSingle(QPrinter *printer);	//�����������
	void filePrintPreviewWhole();				//���屨���ӡԤ��
	void PrintWhole();							//���屨��ֱ�Ӵ�ӡ
	void printPreviewWhole(QPrinter *printer);	//���屨�����
	void HelpAction();		//����
	void toolbar_show();	//��������ʾ����
	void toolbar_hide();	//���������غ���
	void LastPipeData();	//��һ������
	void OpenPipeData();	//�������ļ�
	void NextPipeData();	//��һ�������ļ�
	void DefectAlarm();		//ȱ�ݱ���
	void CouplingAlarm();	//��ϱ���
	
	void AllParameterYes();		//�������ô���ȷ��
	void AllParameterNo();		//�������ô���ȡ��
	void AllParameterSaveAs();	//�������ô������Ϊ
	void AllParameterDo();		//���������������
	void OrderParameterYes();	//��������ȷ��
	void OrderParameterNo();	//��������ȡ��
	void OrderParameterSaveAs();//�����������Ϊ
	void OrderDiaAdd();			//��������ֱ�����
	void OrderDiaCut();			//��������ֱ��ɾ��
	void OrderLengthAdd();		//���������������
	void OrderLengthCut();		//������������ɾ��
	void OrderPlyAdd();			//���������ں����
	void OrderPlyCut();			//���������ں�ɾ��
	void OrderRankAdd();		//���������������
	void OrderRankCut();		//������������ɾ��
	void UltrasoundParameterYes();		//��������ȷ��
	void UltrasoundParameterNo();		//��������ȡ��
	void UltrasoundParameterSaveAs();	//�����������Ϊ
	void ShowParameterYes();	//��ʾ����ȷ��
	void ShowParameterNo();		//��ʾ����ȡ��
	void ShowParameterSaveAs();	//��ʾ�������Ϊ
	void JudgeParameterYes();	//���в���ȷ��
	void JudgeParameterNo();	//���в���ȡ��
	void JudgeParameterSaveAs();//���в������Ϊ
	void LeftPartUpdate();		//�ֲ��Ŵ���ಿ�ָ���
	void RightPartUpdate();		//�ֲ��Ŵ��Ҳಿ�ָ���
	void SliderNoPresse();		//slider�����ʱ�Ļ�ͼ����
	void BurstDelayShow();		//�ӳ���ʾ
	void CGainShow();			//�ο�������ʾ
	void GainShow();			//������ʾ
	void GateSetShow();			//բ��������ʾ
	void ImpedanceSetShow();	//�迹��ʾ
	void JudgeShow();			//բ�ſ���״̬������ʾ
	void ProbeSetShow();		//̽ͷ������ʾ
	void ReFreqShow();			//�ظ�Ƶ��������ʾ
	void SoundSpeedShow();		//����������ʾ
	void StaNoiseShow();		//ͳ�ƽ�������ʾ
	void WidthShow();			//����������ʾ
	void DeadZoneShow();		//ä������
	void BurstDelayYes();		//�ӳ�ȷ��
	void BurstDelayNo();		//�ӳ�ȡ��
	void CGainYes();			//�ο�����ȷ��
	void CGainNo();				//�ο�����ȡ��
	void GainYes();				//����ȷ��
	void GainNo();				//����ȡ��
	void GateSetYes();			//բ������ȷ��
	void GateSetNo();			//բ������ȡ��
	void ImpedanceSetYes();		//�迹ȷ��
	void ImpedanceSetNo();		//�迹ȡ��
	void JudgeYes();			//բ�ſ���ȷ��
	void JudgeNo();				//բ�ſ���ȡ��
	void ProbeSetYes();			//̽ͷ����ȷ��
	void ProbeSetNo();			//̽ͷ����ȡ��
	void ReFreqYes();			//�ظ�Ƶ��ȷ��
	void ReFreqNo();			//�ظ�Ƶ��ȡ��
	void SoundSpeedYes();		//��������ȷ��
	void SoundSpeedNo();		//��������ȡ��
	void StaNoiseYes();			//ͳ�ƽ�����ȷ��
	void StaNoiseNo();			//ͳ�ƽ�����ȡ��
	void WidthYes();			//��������ȷ��
	void WidthNo();				//��������ȡ��
	void DeadZoneYes();			//ä��ȷ��
	void DeadZoneNo();			//ä��ȡ��
	void StaNoiseCheck1_10();	//ͳ�ƽ���1-10��ȡ�ļ�
	void StaNoiseCheck11_20();	//ͳ�ƽ���11-20��ȡ�ļ�
	void StaNoiseCheck21_30();	//ͳ�ƽ���21-30��ȡ�ļ�
	void GateSetCheck1_10();	//բ������1-10��ȡ�ļ�
	void GateSetCheck11_20();	//բ������11-20��ȡ�ļ�
	void GateSetCheck21_30();	//բ������21-30��ȡ�ļ�
	void JudgeCheck1_10();		//բ�ſ�������1-10��ȡ�ļ�
	void JudgeCheck11_20();		//բ�ſ�������11-20��ȡ�ļ�
	void JudgeCheck21_30();		//բ�ſ�������21-30��ȡ�ļ�
	void GateSetAllI();		//բ������ȫ��ѡ��I
	void GateSetAllA();		//բ������ȫ��ѡ��A
	void GateSetAllB();		//բ������ȫ��ѡ��B
	void GateSetAllI2();	//բ������ȫ��ѡ��I2
	void GateSetAnyI();		//բ����������ѡ��I
	void GateSetAnyA();		//բ����������ѡ��A
	void GateSetAnyB();		//բ����������ѡ��B
	void GateSetAnyI2();	//բ����������ѡ��I2
	void StaNoiseAllI();	//ͳ�ƽ�����ȫ��ѡ��I
	void StaNoiseAllA();	//ͳ�ƽ�����ȫ��ѡ��A
	void StaNoiseAllB();	//ͳ�ƽ�����ȫ��ѡ��B
	void StaNoiseAllI2();	//ͳ�ƽ�����ȫ��ѡ��I2
	void StaNoiseAnyI();	//ͳ�ƽ���������ѡ��I
	void StaNoiseAnyA();	//ͳ�ƽ���������ѡ��A
	void StaNoiseAnyB();	//ͳ�ƽ���������ѡ��B
	void StaNoiseAnyI2();	//ͳ�ƽ���������ѡ��I2
	void JudgeAllI();	//բ�ſ�������ȫ��ѡ��I
	void JudgeAllA();	//բ�ſ�������ȫ��ѡ��A
	void JudgeAllB();	//բ�ſ�������ȫ��ѡ��B
	void JudgeAllI2();	//բ�ſ�������ȫ��ѡ��I2
	void JudgeAnyI();	//բ�ſ�����������ѡ��I
	void JudgeAnyA();	//բ�ſ�����������ѡ��A
	void JudgeAnyB();	//բ�ſ�����������ѡ��B
	void JudgeAnyI2();	//բ�ſ�����������ѡ��I2
	void FormItemSet();				//�������ý���
	void FormItemYes();				//����ȷ��
	void FormItemOperatorAdd();		//������������
	void FormItemOperatorCut();		//���������ɾ��
	void FormItemShiftAdd();		//���������
	void FormItemShiftCut();		//������ɾ��
	void FormItemStandardAdd();		//�������׼���
	void FormItemStandardCut();		//�������׼ɾ��
	void FormItemRankAdd();			//�����⼶�����	
	void FormItemRankCut();			//�����⼶��ɾ��
	void FormItemCurSteelNumber();	//���ü������ĵĸֹܱ��
	void FormItemTestRankUpdate();	//��⼶�����
	void FormItemPrintScreen();		//��Ļ��ӡ
public:
	QPoint startPnt;
	QPoint pLeftTop;
	QPoint pRightBottom;
	QTimer *timer;
private:
	QRect m_rectHRuler;				//ˮƽ��߻�������	
  	QRect m_rectVRuler_first;		//��ֱ���1��������
 	QRect m_rectVRuler_second;		//��ֱ���2��������
 	QRect m_rectVRuler_third;		//��ֱ���3��������
 	QRect m_rectVRuler_fourth;		//��ֱ���4��������
 	QRect m_rectVRuler_fifth;		//��ֱ���5��������
 	QRect m_rectBlank;				//���Ƿ����������
	QRect m_rectWaveSliderDown;		//�ֲ��Ŵ�ײ���ͼ����
	QRect m_rectWaveSliderUp;		//�ֲ��Ŵ��ϲ��������
	QRect m_rectThickMaxTolerance;		//�ں�������󹫲�
	QRect m_rectThickMinTolerance;		//�ں�������С����
	QRect m_rectRangeMax;				//�������Χ
	QRect m_rectRangeMin;				//������С��Χ
	QRect m_rectCurThick;				//��ǰ���õıں�
	QRect m_rectHRulerSingle;			//�����е�ˮƽ��ߴ�ӡ��Χ
	QRect m_rectTitle;					//��������ӡ��Χ
	QRect m_rectSteelNum;				//�ֹܱ�Ŵ�ӡ��Χ
	QRect m_rectSteelLength;			//�ֹܳ��ȴ�ӡ��Χ
	QRect m_rectSteelDia;				//�ֹ�ֱ����ӡ��Χ
	QRect m_rectSteelThick;				//�ֹܱں��ӡ��Χ
	QRect m_rectOperator;				//��������ӡ��Χ
	QRect m_rectSortingResult;			//��ѡ�����ӡ��Χ
	QRect m_rectTestStandard;			//��������׼��ӡ��Χ
	QRect m_rectTestRank;				//������⼶���ӡ��Χ
	QRect m_rectTestTime;				//�������ʱ���ӡ��Χ
	QRect m_rectTestTime1;				//������ʱ���ӡ��Χ
	QRect m_rectPrintTime;				//��ӡʱ���ӡ��Χ
	QRect m_rectOutVFeature;			//�ⲿ����ȱ�ݴ�ӡ��Χ
	QRect m_rectInVFeature;				//�ڲ�����ȱ�ݴ�ӡ��Χ
	QRect m_rectOutHFeature;			//�ⲿ����ȱ�ݴ�ӡ��Χ
	QRect m_rectInHFeature;				//�ڲ�����ȱ�ݴ�ӡ��Χ
	QRect m_rectThickAdd;				//�ں������ӡ��Χ
	QRect m_rectThickCut;				//�ں������ӡ��Χ
	QRect m_rectLayerFeature;			//�ֲ�ȱ�ݴ�ӡ��Χ
	QRect m_rectBadCoupleFeature;		//��ϲ�����ӡ��Χ
	QRect m_rectOrderNumber;			//������Ŵ�ӡ��Χ
	QRect m_rectSteelCount;				//�ֹ�������ӡ��Χ
	QRect m_rectSteelRank;				//�ֹּܸ���ӡ��Χ
	QRect m_rectAllParameter;			//�ܲ����ļ���ӡ��Χ
	QRect m_rectOrderParameter;			//���������ļ���ӡ��Χ
	QRect m_rectUltrasoundParameter;	//���������ļ���ӡ��Χ
	QRect m_rectShowParameter;			//��ʾ�����ļ���ӡ��Χ
	QRect m_rectJudgeParameter;			//���в����ļ���ӡ��Χ
	QRect m_rectResult;					//�������ӡ��Χ
	QRect m_rectTotal;					//�����ܼ�
	QRect m_rectQualified;				//�����ϸ�Ʒ
	QRect m_rectFeature;				//����ȱ��Ʒ
	QRect m_rectBadCouple;				//������ϲ���Ʒ
	QRect m_rectCount;					//��������
	QRect m_rectLength;					//��������
	QRect m_rectCountPercent;			//����������ռ�ٷֱ�
	QRect m_rectLengthPercent;			//����������ռ�ٷֱ�
	QRect m_rectName;					//ǩ����ӡ��Χ
	QRect m_rectEddyCurrentTestStandard;//��������׼
	QRect m_rectEddyCurrentTestRank;	//������⼶��
	QRect m_rectSoundResult;			//���������
	QRect m_rectEddyCurrentResult;		//���������

	QRect m_rectSteelNum_;				//�ֹܱ��ֵ��ӡ��Χ
	QRect m_rectSteelLength_;			//�ֹܳ���ֵ��ӡ��Χ
	QRect m_rectSteelDia_;				//�ֹ�ֱ��ֵ��ӡ��Χ
	QRect m_rectSteelThick_;			//�ֹܱں�ֵ��ӡ��Χ
	QRect m_rectOperator_;				//������ֵ��ӡ��Χ
	QRect m_rectSortingResult_;			//��ѡ���ֵ��ӡ��Χ
	QRect m_rectTestStandard_;			//����׼ֵ��ӡ��Χ
	QRect m_rectTestRank_;				//��⼶��ֵ��ӡ��Χ
	QRect m_rectTestTime_;				//�������ʱ��ֵ��ӡ��Χ
	QRect m_rectTestTime1_;				//������ʱ��ֵ��ӡ��Χ
	QRect m_rectPrintTime_;				//��ӡʱ��ֵ��ӡ��Χ
	QRect m_rectOutVFeature_;			//�ⲿ����ȱ��ֵ��ӡ��Χ
	QRect m_rectInVFeature_;			//�ڲ�����ȱ��ֵ��ӡ��Χ
	QRect m_rectOutHFeature_;			//�ⲿ����ȱ��ֵ��ӡ��Χ
	QRect m_rectInHFeature_;			//�ڲ�����ȱ��ֵ��ӡ��Χ
	QRect m_rectThickAdd_;				//�ں�����ֵ��ӡ��Χ
	QRect m_rectThickCut_;				//�ں����ֵ��ӡ��Χ
	QRect m_rectLayerFeature_;			//�ֲ�ȱ�ݴ�ӡ��Χ
	QRect m_rectBadCoupleFeature_;		//��ϲ���ֵ��ӡ��Χ
	QRect m_rectOrderNumber_;			//�������ֵ��ӡ��Χ
	QRect m_rectSteelCount_;			//�ֹ�����ֵ��ӡ��Χ
	QRect m_rectSteelRank_;				//�ֹּܸ�ֵ��ӡ��Χ
	QRect m_rectSingleSteelRank_;		//�����ֹּܸ���ӡ��Χ
	QRect m_rectAllParameter_;			//�ܲ����ļ�ֵ��ӡ��Χ
	QRect m_rectOrderParameter_;		//���������ļ�ֵ��ӡ��Χ
	QRect m_rectUltrasoundParameter_;	//���������ļ�ֵ��ӡ��Χ
	QRect m_rectShowParameter_;			//��ʾ�����ļ�ֵ��ӡ��Χ
	QRect m_rectJudgeParameter_;		//���в����ļ�ֵ��ӡ��Χ
	QRect m_rectResult_;				//�����ֵ��ӡ��Χ
	QRect m_rectTotal_;					//�ܼ�ֵ��ӡ��Χ
	QRect m_rectQualified_;				//�ϸ�Ʒֵ��ӡ��Χ
	QRect m_rectFeature_;				//ȱ��Ʒֵ��ӡ��Χ
	QRect m_rectBadCouple_;				//��ϲ���Ʒֵ��ӡ��Χ
	QRect m_rectCount_;					//����ֵ��ӡ��Χ
	QRect m_rectLength_;				//����ֵ��ӡ��Χ
	QRect m_rectCountPercent_;			//������ռ�ٷֱ�ֵ��ӡ��Χ
	QRect m_rectLengthPercent_;			//������ռ�ٷֱ�ֵ��ӡ��Χ
	QRect m_rectEddyCurrentTestStandard_;//��������׼
	QRect m_rectEddyCurrentTestRank_;	//������⼶��
	QRect m_rectEddyCurrentTotal_;		//��������ܼ�
	QRect m_rectEddyCurrentAcNum_;		//�����ϸ�Ʒ����
	QRect m_rectEddyCurrentAcPer_;		//�����ϸ�Ʒ�ٷֱ�
	QRect m_rectEddyCurrentReNum_;		//�������ϸ�Ʒ����
	QRect m_rectEddyCurrentRePer_;		//�������ϸ�Ʒ�ٷֱ�
	QRect m_rectCountTotal_;			//�����ܼƼ���
	QRect m_rectCountPercentTotal_;		//�����ܼƼ����ٷֱ�
	QRect m_rectLengthTotal_;			//�����ܼƳ���
	QRect m_rectLengthPercentTotal_;	//�����ܼƳ��Ȱٷֱ�
	QRect m_rectAcCount_;				//�����ϸ�Ʒ����
	QRect m_rectAcCountPercent_;		//�����ϸ�Ʒ�����ٷֱ�
	QRect m_rectAcLength_;				//�����ϸ�Ʒ����
	QRect m_rectAcLengthPercent_;		//�����ϸ�Ʒ���Ȱٷֱ�
	QRect m_rectReCount_;				//�������ϸ�Ʒ����
	QRect m_rectReCountPercent_;		//�������ϸ�Ʒ�����ٷֱ�
	QRect m_rectReLength_;				//�������ϸ�Ʒ����
	QRect m_rectReLengthPercent_;		//�������ϸ�Ʒ���Ȱٷֱ�
	QRect m_rectBaCount_;				//������ϲ�������
	QRect m_rectBaCountPercent_;		//������ϲ��������ٷֱ�
	QRect m_rectBaLength_;				//������ϲ�������
	QRect m_rectBaLengthPercent_;		//������ϲ������Ȱٷֱ�
protected:
	void timerEvent( QTimerEvent *event );
	int m_nTIMER_THREAD_PROCESSId;
	int m_nTIMER_SAVE_DMA_DATA;
	int m_nMECHANICAL_TIMER;
	int m_nTIMER_SAVE_PICTURE;
public:
	CPLXCommunication m_PLXCommunication;
	AECTHREAD_PARAM m_stThreadParam[MAX_CARD_NO];
	HANDLE hThread[MAX_CARD_NO];  
	unsigned threadID[MAX_CARD_NO]; 
    CH_CONFIG m_stAllCH_CONFIG[CHANNEL_NUM];
	QString m_strIniFlieName; //�����ļ���
	QString strEddyCurrent;		//��ȡ�������ļ�·��
	QSettings *m_pSettings; 
	CChannelInfo m_Parameter_[CHANNEL_NUM];
	ORDER_PARAMETER m_ORDER_PARAMETER;
	SHOW_PARAMETER m_SHOW_PARAMETER;
	JADGE_PARAMETER m_JADGE_PARAMETER;
	UserManagement m_UserManagement;
public:
	BOOL m_bEncoderMessage; //��������Ϣ�� 1Ϊ�ѵ���δ���� 0Ϊ��������
	BOOL m_bOrderNew;		//������Ϣ�и���
	BOOL m_bIsRunning;
	BOOL m_bSetOne;
	BOOL *m_bPlateStart;
	BOOL m_bAScanIsOpen;//Aɨ����
	BOOL m_bBadCoupleOrNo;//�Ƿ���ϲ���
	BOOL m_bSelChannel;
	BOOL *m_bPlateEnd;
	BOOL m_bDefectAlarm[30];//ȱ�ݱ�������
	BOOL m_bCouplingAlarm[30];//��ϱ�������
	BOOL m_bSliderLeftPressed;
	BOOL m_bSliderLeftPresse;
	BOOL m_bSliderRightPressed;
	BOOL m_bSliderRightPresse;
	BOOL m_bSliderPressed;
	BOOL m_bSliderPresse;
	BOOL m_bGain;
	BOOL m_bCGain;
	BOOL m_bSoundSpeed;
	BOOL m_bDelay;
	BOOL m_bGateSet;
	BOOL m_bWidth;
	BOOL m_bJudge;
	BOOL m_bStaNoise;
	BOOL m_bReFreq;
	BOOL m_bImpedanceSet;
	BOOL m_bProbeSet;
	BOOL m_bDeadZone;
private:
	int m_nActScreenX;
	int m_nActScreenY;
	int m_nScreenCount;
	int m_nLeftPart;
	int m_nRightPart;

	int m_nCountPercentTotal;	//�����ܼƼ����ٷֱ�
	int m_nLengthTotal;			//�����ܼƳ���
	int m_nLengthPercentTotal;	//�����ܼƳ��Ȱٷֱ�
	int m_nAcCount;				//�����ϸ�Ʒ����
	int m_nAcCountPercent;		//�����ϸ�Ʒ�����ٷֱ�
	int m_nAcLength;			//�����ϸ�Ʒ����
	int m_nAcLengthPercent;		//�����ϸ�Ʒ���Ȱٷֱ�
	int m_nReCount;				//�������ϸ�Ʒ����
	int m_nReCountPercent;		//�������ϸ�Ʒ�����ٷֱ�
	int m_nReLength;			//�������ϸ�Ʒ����
	int m_nReLengthPercent;		//�������ϸ�Ʒ���Ȱٷֱ�
	int m_nBaCount;				//������ϲ�������
	int m_nBaCountPercent;		//������ϲ��������ٷֱ�
	int m_nBaLength;			//������ϲ�������
	int m_nBaLengthPercent;		//������ϲ������Ȱٷֱ�
private:
	AScan m_Ascan;
	ParameterSetting m_ParameterSetting;
	FormItem m_FormItem;
private:
	double m_dProportionLeft;
	double m_dProportionRight;
	double m_dRectWidth;
};

#endif // GUANTAN_H
