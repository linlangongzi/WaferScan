#ifndef GUANTAN_H
#define GUANTAN_H

#include <QtGui/QDialog>
#include "ui_guantan.h"
#include "ui_ascan.h"
#include "ascan.h"
#include <Windows.h>
#include <QtCore/QSettings>
#include <QSettings>
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
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
	void paintEvent(QPaintEvent *event);	//Qt绘制函数
	void mousePressEvent(QMouseEvent *e);	//鼠标点击响应函数
	void mouseMoveEvent(QMouseEvent *e);	//鼠标移动响应函数
	void mouseReleaseEvent(QMouseEvent *e);	//鼠标抬起响应函数
	void GetScreenInfo();		//获得屏幕分辨率
	void DistributeRegion();	//画图区域分配函数
	void UiInit();				//UI初始化
	void TestResultProcess();	//缺陷统计函数
	void CalcFlaw(vector<QPointF> vecPoint,int& nFlawNum,int& nMaxFlawSize,float& fMaxFlawHeight);	//缺陷统计-适用于 内外部，横纵向
	void CalcBadCouple(vector<QPointF> vecPoint,int& nFlawNum,int& nMaxFlawSize);					//缺陷统计-适用于耦合不良
	void CalcThick(vector<QPointF> vecPointHigh,vector<QPointF> vecPointLow);						//缺陷统计-适用于壁厚测量
	void DataJudge();			//检测结果评判
	void ReadOPCItem(OPCHANDLE *hServer,int nItemCount); //读取opc
	BOOL WriteOPCItem(OPCHANDLE* phServer, VARIANT* itemArray,  const unsigned long & ulSize);//写opc
private:
	int m_nRealSteelL;		//编码器记录的实际钢管长度 用于监测后盲区wangwei add on 2013-12-14
	bool m_bSteelLengthIsValue;	//每次检测是否给钢管实际长度赋值过 wangwei add on 2013-12-14
	void ZeroEncipheror();		//编码器清零
	float m_fEncipheror;		//当前编码器的值
	float m_fEncipheror_mark;	//编码器打标变量
	WORD m_wStartFlag;			//探伤使能；0，为结束；1，为开始
	BOOL m_bRunFlag;			//探伤过程在线为1，离线为0；
	BOOL m_bOver;				//单批检测结束
	DWORD m_dwProbeBoxUpDown;	//探头盒起落状态	wangwei add on 2013-12-12
	int m_nStartDeadZone;		//始端盲区	wangwei add on 2013-12-12
	int m_nEndDeadZone;			//末端盲区	wangwei add on 2013-12-12
	void SaveData(QString fileName,vector<QPointF> ptVector);	//保存各缺陷扫描原始数据
	Traversalfile m_FindData;
	QList<QString> m_DataList;
	QString m_strCurrentDataPath;
	void ReadDataFile(QString& strDataFile);//读取解析数据文件
	void ReadDataFileAndDraw(QString strDataFile,QRect rectFeature,QPixmap& bmpFeature,QColor clrFeature);	//测厚和耦合不适用
	void ReadDataFileAndDraw_V(QString strDataFile,QRect rectFeature,QPixmap& bmpFeature,QColor clrFeature);//读取并画图纵向缺陷
	void ClearScreen();						//清屏，每次开始新的探测或者读取新的文件前调用
	void keyPressEvent(QKeyEvent *event);	//重写esc响应函数
public:
	CSimpleFree m_OPCComponent;
private:
	BOOL bDBorPCT;//False为纵标尺DB显示，TRUE为纵标尺百分比显示；
	BOOL m_bInFeacher;		//扫描过程中用于打标的内部缺陷存在的标志
	BOOL m_bOutFeacher;		//扫描过程中用于打标的外部缺陷存在的标志
	BOOL m_bThickFeacher;	//扫描过程中用于打标的壁厚分层缺陷存在的标志
	BOOL m_bEncipherorChange2;//编码器值变了10以上
	BOOL m_bEncipherorChange3;//编码器值变了10以上
	BOOL m_bEncipherorChange4;//编码器值变了10以上
	BOOL m_bEncipherorChange5;//编码器值变了10以上
private:
	QPointF ptOutVFeatureStartPoint;		//外部纵向缺陷图曲线起始点
	QPointF ptInVFeatureStartPoint;			//内部纵向缺陷图曲线起始点
	QPointF ptOutHFeatureStartPoint;		//外部横向缺陷图曲线起始点
	QPointF ptInHFeatureStartPoint;			//内部横向缺陷图曲线起始点
	QPointF ptThickFeatureStartPoint;		//壁厚测量缺陷图曲线起始点
	QPointF ptLayerFeatureStartPoint;		//分层缺陷图曲线起始点
	QPointF ptBadCoupleFeatureStartPoint;	//耦合不良曲线起始点
	QPixmap bmpVFeature;			//纵向缺陷图画布
	QPixmap bmpHFeature;			//横向缺陷图画布
	QPixmap bmpThickFeature;		//测厚缺陷图画布
	QPixmap bmpLayerFeature;		//分层缺陷图画布
	QPixmap bmpBadCoupleFeature;	//耦合不良画布
	vector<QPointF> m_vectorOutVFeature;		//外部纵向缺陷曲线点集
	vector<QPointF> m_vectorInVFeature;			//内部纵向缺陷曲线点集
	vector<QPointF> m_vectorOutHFeature;		//外部横向缺陷曲线点集
	vector<QPointF> m_vectorInHFeature;			//内部横向缺陷曲线点集
	vector<QPointF> m_vectorThickHighFeature;	//测厚-增厚缺陷厚面曲线点集
	vector<QPointF> m_vectorThickLowFeature;	//测厚-减薄缺陷厚面曲线点集
	vector<QPointF> m_vectorThickFeature1;		//测厚-平均壁厚曲线点集
	vector<QPointF> m_vectorThickFeature2;		//测厚-平均壁厚曲线点集
	vector<QPointF> m_vectorThickFeature3;		//测厚-平均壁厚曲线点集
	vector<QPointF> m_vectorThickFeature4;		//测厚-平均壁厚曲线点集
	vector<QPointF> m_vectorThickFeature5;		//测厚-平均壁厚曲线点集
	vector<QPointF> m_vectorThickTempFeature1;	//测厚-探头盒1-10mm内所有点临时存储
	vector<QPointF> m_vectorThickTempFeature2;	//测厚-探头盒2-10mm内所有点临时存储
	vector<QPointF> m_vectorThickTempFeature3;	//测厚-探头盒3-10mm内所有点临时存储
	vector<QPointF> m_vectorThickTempFeature4;	//测厚-探头盒4-10mm内所有点临时存储
	vector<QPointF> m_vectorThickTempFeature5;	//测厚-探头盒5-10mm内所有点临时存储
	vector<QPointF> m_vectorLayerFeature;		//分层缺陷曲线点集
	vector<QPointF> m_vectorBadCoupleFeature;	//耦合不良曲线点集
private:
	int m_nOutVFeatureFlawNum;			//外部纵向缺陷数量
	int m_nInVFeatureFlawNum;			//内部纵向缺陷数量
	int m_nOutHFeatureFlawNum;			//外部横向缺陷数量
	int m_nInHFeatureFlawNum;			//内部横向缺陷数量
	int m_nThickFeatureFlawNumAdd;		//壁厚-增厚缺陷数量
	int m_nThickFeatureFlawNumCut;		//壁厚-减薄缺陷数量
	int m_nLayerFeatureFlawNum;			//分层缺陷数量
	int m_nBadCoupleFeatureFlawNum;		//偶合不良缺陷数量
	int m_nOutVFeatureMaxFlawSize;		//外部纵向最大缺陷尺寸
	int m_nInVFeatureMaxFlawSize;		//内部纵向最大缺陷尺寸
	int m_nOutHFeatureMaxFlawSize;		//外部横向最大缺陷尺寸
	int m_nInHFeatureMaxFlawSize;		//内部横向最大缺陷尺寸
	int m_nThickFeatureMaxFlawSizeAdd;	//壁厚-增厚最大缺陷尺寸
	int m_nThickFeatureMaxFlawSizeCut;	//壁厚-减薄最大缺陷尺寸
	int m_nLayerFeatureMaxFlawSize;		//分层缺陷最大缺陷尺寸
	int m_nBadCoupleFeatureMaxFlawSize;	//耦合不良最大缺陷尺寸
	int m_nBadCoupleBox[5];				//探头盒耦合不良计数数组
	int m_nBadCoupleNum;				//离线查看耦合不良计数
	int m_nBadCoupleBoxNum;				//各盒耦合不良报警值
	float m_fOutVFeatureMaxFlawGain;	//外部纵向缺陷最大长度所占百分比
	float m_fInVFeatureMaxFlawGain;		//内部纵向缺陷最大长度所占百分比
	float m_fOutHFeatureMaxFlawGain;	//外部横向缺陷最大长度所占百分比
	float m_fInHFeatureMaxFlawGain;		//内部横向缺陷最大长度所占百分比
	float m_fThickFeatureMaxFlawGain;	//壁厚增厚缺陷最大长度所占百分比
	float m_fThickFeatureMinFlawGain;	//壁厚减薄缺陷最大长度所占百分比
	float m_fLayerFeatureMax;			//分层缺陷最大长度所占百分比
	float m_fPIPE_LENGTH;				//最大显示长度
	float m_fThickMaxTolerance;			//壁厚允许最大公差
	float m_fThickMinTolerance;			//壁厚允许最小公差
	float m_fRangeMax;					//最大范围
	float m_fRangeMin;					//最小范围
	float m_fCurThick;					//当前壁厚
	float m_fCurPersent;				//壁厚标准所占比例
	float m_fProbeCorrect[5];			//5个盒子测厚探头校准
private:
	int m_nTOTAL;					//涡流扫描结果-总数
	int m_nACCEPTS_CONT;			//涡流扫描结果-合格品数量
	float m_fACCEPTS_PERSENT;		//涡流扫描结果-合格品百分比
	QString m_strACCEPTS_PERSENT;	//涡流扫描结果-合格品百分比用于报表打印
	int m_nREJECTS_CONT;			//涡流扫描结果-不合格品数量
	float m_fREJECTS_PERSENT;		//涡流扫描结果-不合格品百分比
	QString m_strREJECTS_PERSENT;	//涡流扫描结果-不合格品百分比用于报表打印
	QString m_strSingleReport1;	//外部纵向
	QString m_strSingleReport2;	//内部纵向
	QString m_strSingleReport3;	//外部横向
	QString m_strSingleReport4;	//内部横向
	QString m_strSingleReport5;	//壁厚增厚
	QString m_strSingleReport6;	//壁厚减薄
	QString m_strSingleReport7;	//分层
	QString m_strSingleReport8;	//耦合不良
	QString m_strSingleReport9;	//平均壁厚
	QString m_strJudgeResult;	//检测评判结果
	QString m_strNewResultFile;	//刚刚检测完毕的钢管数据的总离线文件路径
	QRect m_rectSingleReprot;
private:
	QString m_qsShowOrder;		//订单参数显示
	QString m_qsShowShow;		//显示参数显示
	QString m_qsShowJudge;		//评判参数显示
	QString m_qsShowUltrasound;	//超声参数显示
	QString m_qsShowAll;		//总体参数显示
	QString m_strFormItem;		//报表配置文件路径
	QString m_strOperator;		//操作工
	QString m_strSteelTestDate;	//钢管检测日期
	QString m_strSteelTestTime;	//钢管检测时间
private:
	void DrawHRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//水平标尺 
	void DrawVRuler(const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);	//竖直标尺
	void LeftPartShow();		//主界面数据显示：缺陷数量，缺陷长度，缺陷百分比
	void InitializeConfig();			//用于下发数据的结构体的初始化
	void DrawFeatureInfo();				//主界面缺陷绘图
	void DrawOutVFeature(int i);		//画外部纵向缺陷图
	void DrawInVFeature(int i);			//画内部纵向缺陷图
	void DrawOutHFearure(int i);		//画外部横向缺陷图
	void DrawInHFearure(int i);			//画内部横向缺陷图
	void DrawThickFeature(int i);		//画测厚缺陷图
	void DrawLayerFeature(int i);		//画分层缺陷图
	void DrawBadCoupleFeature(int i);	//画耦合缺陷图
	void Order_Parameter();		//从文件更新订单参数到UI
	void Ultrasound_Parameter();//从文件更新超声参数到UI
	void Show_Parameter();		//从文件更显显示参数到UI
	void Judge_Parameter();		//从文件更新评判参数到UI
	void BurstDelay();			//从文件更新延迟参数到UI
	void CGain();				//从文件更新参考增益参数到UI
	void Gain();				//从文件更新增益参数到UI
	void ProbeSet();			//从文件更新探头设置参数到UI
	void ReFreq();				//从文件更新重复频率参数到UI
	void SoundSpeed();			//从文件更新声速参数到UI
	void Width();				//从文件更新带宽参数到UI
	void ImpedanceSet();		//从文件更新阻抗参数到UI
	void DeadZoneSet();			//从文件更新盲区参数到UI wangwei add on 2013-12-14
	void Suppression();			//从文件更新抑制参数到UI wangwei add on 2013-12-14
	void ReadEddyCurrent();		//读取涡流结果
	void customEvent(QEvent *event);//
	void closeEvent(QCloseEvent *event);//退出信号处理函数
	int	m_nWaveSpeed;
	int m_nThick;
public slots:
	void WriteReFreq();//下发重复频率
signals:
	void DefectAlarmChanged();  //缺陷报警信号
	void CouplingAlarmChanged();//耦合报警信号
private slots:
//A扫
	void AScanAction();		//A扫按钮响应函数	
	void AScanSave();		//A扫保存参数按钮
//参数设置
	void FormSetting();		//参数设置窗口显示
	void ShowOpen();		//显示窗口打开
	void ShowDo();			//显示调用
	void UltrasoundOpen();	//超声窗口打开
	void UltrasoundDo();	//超声调用
	void OrderOpen();		//订单窗口打开
	void OrderDo();			//订单调用
	void JudgeOpen();		//评判窗口打开
	void JudgeDo();			//评判调用
	void PrintScreen();					//界面打印响应函数
	void PaintScreen(QPrinter *printer);//界面打印的绘制
	void UserManagementShow();			//用户管理显示
	void WriteEddyCurrent();			//给涡流建立文件写信息
	void DateTimeUpdate();				//日期时间显示
	void filePrintPreviewSingle();				//单根报表打印预览
	void PrintSingle();							//单根报表直接打印
	void printPreviewSingle(QPrinter *printer);	//单根报表绘制
	void filePrintPreviewWhole();				//整体报表打印预览
	void PrintWhole();							//整体报表直接打印
	void printPreviewWhole(QPrinter *printer);	//整体报表绘制
	void HelpAction();		//帮助
	void toolbar_show();	//工具栏显示函数
	void toolbar_hide();	//工具栏隐藏函数
	void LastPipeData();	//上一个数据
	void OpenPipeData();	//打开数据文件
	void NextPipeData();	//下一个数据文件
	void DefectAlarm();		//缺陷报警
	void CouplingAlarm();	//耦合报警
	
	void AllParameterYes();		//参数设置窗口确定
	void AllParameterNo();		//参数设置窗口取消
	void AllParameterSaveAs();	//参数设置窗口另存为
	void AllParameterDo();		//参数设置整体调用
	void OrderParameterYes();	//订单参数确定
	void OrderParameterNo();	//订单参数取消
	void OrderParameterSaveAs();//订单参数另存为
	void OrderDiaAdd();			//订单参数直径添加
	void OrderDiaCut();			//订单参数直径删除
	void OrderLengthAdd();		//订单参数长度添加
	void OrderLengthCut();		//订单参数长度删除
	void OrderPlyAdd();			//订单参数壁厚添加
	void OrderPlyCut();			//订单参数壁厚删除
	void OrderRankAdd();		//订单参数级别添加
	void OrderRankCut();		//订单参数级别删除
	void UltrasoundParameterYes();		//超声参数确定
	void UltrasoundParameterNo();		//超声参数取消
	void UltrasoundParameterSaveAs();	//超声参数另存为
	void ShowParameterYes();	//显示参数确定
	void ShowParameterNo();		//显示参数取消
	void ShowParameterSaveAs();	//显示参数另存为
	void JudgeParameterYes();	//评判参数确定
	void JudgeParameterNo();	//评判参数取消
	void JudgeParameterSaveAs();//评判参数另存为
	void LeftPartUpdate();		//局部放大左侧部分更新
	void RightPartUpdate();		//局部放大右侧部分更新
	void SliderNoPresse();		//slider不点击时的画图规则
	void BurstDelayShow();		//延迟显示
	void CGainShow();			//参考增益显示
	void GainShow();			//增益显示
	void GateSetShow();			//闸门设置显示
	void ImpedanceSetShow();	//阻抗显示
	void JudgeShow();			//闸门开关状态设置显示
	void ProbeSetShow();		//探头设置显示
	void ReFreqShow();			//重复频率设置显示
	void SoundSpeedShow();		//声速设置显示
	void StaNoiseShow();		//统计降噪数显示
	void WidthShow();			//抑制设置显示
	void DeadZoneShow();		//盲区设置
	void BurstDelayYes();		//延迟确定
	void BurstDelayNo();		//延迟取消
	void CGainYes();			//参考增益确定
	void CGainNo();				//参考增益取消
	void GainYes();				//增益确定
	void GainNo();				//增益取消
	void GateSetYes();			//闸门设置确定
	void GateSetNo();			//闸门设置取消
	void ImpedanceSetYes();		//阻抗确定
	void ImpedanceSetNo();		//阻抗取消
	void JudgeYes();			//闸门开关确定
	void JudgeNo();				//闸门开关取消
	void ProbeSetYes();			//探头设置确定
	void ProbeSetNo();			//探头设置取消
	void ReFreqYes();			//重复频率确定
	void ReFreqNo();			//重复频率取消
	void SoundSpeedYes();		//声速设置确定
	void SoundSpeedNo();		//声速设置取消
	void StaNoiseYes();			//统计降噪数确定
	void StaNoiseNo();			//统计降噪数取消
	void WidthYes();			//抑制设置确定
	void WidthNo();				//抑制设置取消
	void DeadZoneYes();			//盲区确定
	void DeadZoneNo();			//盲区取消
	void StaNoiseCheck1_10();	//统计降噪1-10读取文件
	void StaNoiseCheck11_20();	//统计降噪11-20读取文件
	void StaNoiseCheck21_30();	//统计降噪21-30读取文件
	void GateSetCheck1_10();	//闸门设置1-10读取文件
	void GateSetCheck11_20();	//闸门设置11-20读取文件
	void GateSetCheck21_30();	//闸门设置21-30读取文件
	void JudgeCheck1_10();		//闸门开关设置1-10读取文件
	void JudgeCheck11_20();		//闸门开关设置11-20读取文件
	void JudgeCheck21_30();		//闸门开关设置21-30读取文件
	void GateSetAllI();		//闸门设置全部选择I
	void GateSetAllA();		//闸门设置全部选择A
	void GateSetAllB();		//闸门设置全部选择B
	void GateSetAllI2();	//闸门设置全部选择I2
	void GateSetAnyI();		//闸门设置条件选择I
	void GateSetAnyA();		//闸门设置条件选择A
	void GateSetAnyB();		//闸门设置条件选择B
	void GateSetAnyI2();	//闸门设置条件选择I2
	void StaNoiseAllI();	//统计降噪数全部选择I
	void StaNoiseAllA();	//统计降噪数全部选择A
	void StaNoiseAllB();	//统计降噪数全部选择B
	void StaNoiseAllI2();	//统计降噪数全部选择I2
	void StaNoiseAnyI();	//统计降噪数条件选择I
	void StaNoiseAnyA();	//统计降噪数条件选择A
	void StaNoiseAnyB();	//统计降噪数条件选择B
	void StaNoiseAnyI2();	//统计降噪数条件选择I2
	void JudgeAllI();	//闸门开关设置全部选择I
	void JudgeAllA();	//闸门开关设置全部选择A
	void JudgeAllB();	//闸门开关设置全部选择B
	void JudgeAllI2();	//闸门开关设置全部选择I2
	void JudgeAnyI();	//闸门开关设置条件选择I
	void JudgeAnyA();	//闸门开关设置条件选择A
	void JudgeAnyB();	//闸门开关设置条件选择B
	void JudgeAnyI2();	//闸门开关设置条件选择I2
	void FormItemSet();				//报表设置界面
	void FormItemYes();				//报表确定
	void FormItemOperatorAdd();		//报表操作工添加
	void FormItemOperatorCut();		//报表操作工删除
	void FormItemShiftAdd();		//报表班次添加
	void FormItemShiftCut();		//报表班次删除
	void FormItemStandardAdd();		//报表检测标准添加
	void FormItemStandardCut();		//报表检测标准删除
	void FormItemRankAdd();			//报表检测级别添加	
	void FormItemRankCut();			//报表检测级别删除
	void FormItemCurSteelNumber();	//设置即将检测的的钢管编号
	void FormItemTestRankUpdate();	//检测级别更新
	void FormItemPrintScreen();		//屏幕打印
public:
	QPoint startPnt;
	QPoint pLeftTop;
	QPoint pRightBottom;
	QTimer *timer;
private:
	QRect m_rectHRuler;				//水平标尺绘制区域	
  	QRect m_rectVRuler_first;		//竖直标尺1绘制区域
 	QRect m_rectVRuler_second;		//竖直标尺2绘制区域
 	QRect m_rectVRuler_third;		//竖直标尺3绘制区域
 	QRect m_rectVRuler_fourth;		//竖直标尺4绘制区域
 	QRect m_rectVRuler_fifth;		//竖直标尺5绘制区域
 	QRect m_rectBlank;				//顶角方块绘制区域
	QRect m_rectWaveSliderDown;		//局部放大底部绘图区域
	QRect m_rectWaveSliderUp;		//局部放大上层绘制区域
	QRect m_rectThickMaxTolerance;		//壁厚允许最大公差
	QRect m_rectThickMinTolerance;		//壁厚允许最小公差
	QRect m_rectRangeMax;				//测量最大范围
	QRect m_rectRangeMin;				//测量最小范围
	QRect m_rectCurThick;				//当前设置的壁厚
	QRect m_rectHRulerSingle;			//报表中的水平标尺打印范围
	QRect m_rectTitle;					//报表标题打印范围
	QRect m_rectSteelNum;				//钢管编号打印范围
	QRect m_rectSteelLength;			//钢管长度打印范围
	QRect m_rectSteelDia;				//钢管直径打印范围
	QRect m_rectSteelThick;				//钢管壁厚打印范围
	QRect m_rectOperator;				//操作工打印范围
	QRect m_rectSortingResult;			//分选结果打印范围
	QRect m_rectTestStandard;			//超声检测标准打印范围
	QRect m_rectTestRank;				//超声检测级别打印范围
	QRect m_rectTestTime;				//单根检测时间打印范围
	QRect m_rectTestTime1;				//整体检测时间打印范围
	QRect m_rectPrintTime;				//打印时间打印范围
	QRect m_rectOutVFeature;			//外部纵向缺陷打印范围
	QRect m_rectInVFeature;				//内部纵向缺陷打印范围
	QRect m_rectOutHFeature;			//外部横向缺陷打印范围
	QRect m_rectInHFeature;				//内部横向缺陷打印范围
	QRect m_rectThickAdd;				//壁厚增厚打印范围
	QRect m_rectThickCut;				//壁厚减薄打印范围
	QRect m_rectLayerFeature;			//分层缺陷打印范围
	QRect m_rectBadCoupleFeature;		//耦合不良打印范围
	QRect m_rectOrderNumber;			//订单编号打印范围
	QRect m_rectSteelCount;				//钢管数量打印范围
	QRect m_rectSteelRank;				//钢管钢级打印范围
	QRect m_rectAllParameter;			//总参数文件打印范围
	QRect m_rectOrderParameter;			//订单参数文件打印范围
	QRect m_rectUltrasoundParameter;	//超声参数文件打印范围
	QRect m_rectShowParameter;			//显示参数文件打印范围
	QRect m_rectJudgeParameter;			//评判参数文件打印范围
	QRect m_rectResult;					//检测结果打印范围
	QRect m_rectTotal;					//超声总计
	QRect m_rectQualified;				//超声合格品
	QRect m_rectFeature;				//超声缺陷品
	QRect m_rectBadCouple;				//超声耦合不良品
	QRect m_rectCount;					//超声件数
	QRect m_rectLength;					//超声长度
	QRect m_rectCountPercent;			//超声件数所占百分比
	QRect m_rectLengthPercent;			//超声长度所占百分比
	QRect m_rectName;					//签名打印范围
	QRect m_rectEddyCurrentTestStandard;//涡流检测标准
	QRect m_rectEddyCurrentTestRank;	//涡流检测级别
	QRect m_rectSoundResult;			//超声检测结果
	QRect m_rectEddyCurrentResult;		//涡流检测结果

	QRect m_rectSteelNum_;				//钢管编号值打印范围
	QRect m_rectSteelLength_;			//钢管长度值打印范围
	QRect m_rectSteelDia_;				//钢管直径值打印范围
	QRect m_rectSteelThick_;			//钢管壁厚值打印范围
	QRect m_rectOperator_;				//操作工值打印范围
	QRect m_rectSortingResult_;			//分选结果值打印范围
	QRect m_rectTestStandard_;			//检测标准值打印范围
	QRect m_rectTestRank_;				//检测级别值打印范围
	QRect m_rectTestTime_;				//单根检测时间值打印范围
	QRect m_rectTestTime1_;				//整体检测时间值打印范围
	QRect m_rectPrintTime_;				//打印时间值打印范围
	QRect m_rectOutVFeature_;			//外部纵向缺陷值打印范围
	QRect m_rectInVFeature_;			//内部纵向缺陷值打印范围
	QRect m_rectOutHFeature_;			//外部横向缺陷值打印范围
	QRect m_rectInHFeature_;			//内部横向缺陷值打印范围
	QRect m_rectThickAdd_;				//壁厚增厚值打印范围
	QRect m_rectThickCut_;				//壁厚减薄值打印范围
	QRect m_rectLayerFeature_;			//分层缺陷打印范围
	QRect m_rectBadCoupleFeature_;		//耦合不良值打印范围
	QRect m_rectOrderNumber_;			//订单编号值打印范围
	QRect m_rectSteelCount_;			//钢管数量值打印范围
	QRect m_rectSteelRank_;				//钢管钢级值打印范围
	QRect m_rectSingleSteelRank_;		//单根钢管钢级打印范围
	QRect m_rectAllParameter_;			//总参数文件值打印范围
	QRect m_rectOrderParameter_;		//订单参数文件值打印范围
	QRect m_rectUltrasoundParameter_;	//超声参数文件值打印范围
	QRect m_rectShowParameter_;			//显示参数文件值打印范围
	QRect m_rectJudgeParameter_;		//评判参数文件值打印范围
	QRect m_rectResult_;				//检测结果值打印范围
	QRect m_rectTotal_;					//总计值打印范围
	QRect m_rectQualified_;				//合格品值打印范围
	QRect m_rectFeature_;				//缺陷品值打印范围
	QRect m_rectBadCouple_;				//耦合不良品值打印范围
	QRect m_rectCount_;					//件数值打印范围
	QRect m_rectLength_;				//长度值打印范围
	QRect m_rectCountPercent_;			//件数所占百分比值打印范围
	QRect m_rectLengthPercent_;			//长度所占百分比值打印范围
	QRect m_rectEddyCurrentTestStandard_;//涡流检测标准
	QRect m_rectEddyCurrentTestRank_;	//涡流检测级别
	QRect m_rectEddyCurrentTotal_;		//涡流结果总计
	QRect m_rectEddyCurrentAcNum_;		//涡流合格品数量
	QRect m_rectEddyCurrentAcPer_;		//涡流合格品百分比
	QRect m_rectEddyCurrentReNum_;		//涡流不合格品数量
	QRect m_rectEddyCurrentRePer_;		//涡流不合格品百分比
	QRect m_rectCountTotal_;			//超声总计件数
	QRect m_rectCountPercentTotal_;		//超声总计件数百分比
	QRect m_rectLengthTotal_;			//超声总计长度
	QRect m_rectLengthPercentTotal_;	//超声总计长度百分比
	QRect m_rectAcCount_;				//超声合格品件数
	QRect m_rectAcCountPercent_;		//超声合格品件数百分比
	QRect m_rectAcLength_;				//超声合格品长度
	QRect m_rectAcLengthPercent_;		//超声合格品长度百分比
	QRect m_rectReCount_;				//超声不合格品件数
	QRect m_rectReCountPercent_;		//超声不合格品件数百分比
	QRect m_rectReLength_;				//超声不合格品长度
	QRect m_rectReLengthPercent_;		//超声不合格品长度百分比
	QRect m_rectBaCount_;				//超声耦合不良件数
	QRect m_rectBaCountPercent_;		//超声耦合不良件数百分比
	QRect m_rectBaLength_;				//超声耦合不良长度
	QRect m_rectBaLengthPercent_;		//超声耦合不良长度百分比
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
	QString m_strIniFlieName; //配置文件名
	QString strEddyCurrent;		//读取涡流的文件路径
	QSettings *m_pSettings; 
	CChannelInfo m_Parameter_[CHANNEL_NUM];
	ORDER_PARAMETER m_ORDER_PARAMETER;
	SHOW_PARAMETER m_SHOW_PARAMETER;
	JADGE_PARAMETER m_JADGE_PARAMETER;
	UserManagement m_UserManagement;
public:
	BOOL m_bEncoderMessage; //编码器消息框 1为已弹窗未消除 0为弹窗消除
	BOOL m_bOrderNew;		//订单信息有更新
	BOOL m_bIsRunning;
	BOOL m_bSetOne;
	BOOL *m_bPlateStart;
	BOOL m_bAScanIsOpen;//A扫开关
	BOOL m_bBadCoupleOrNo;//是否耦合不良
	BOOL m_bSelChannel;
	BOOL *m_bPlateEnd;
	BOOL m_bDefectAlarm[30];//缺陷报警数组
	BOOL m_bCouplingAlarm[30];//耦合报警数组
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

	int m_nCountPercentTotal;	//超声总计件数百分比
	int m_nLengthTotal;			//超声总计长度
	int m_nLengthPercentTotal;	//超声总计长度百分比
	int m_nAcCount;				//超声合格品件数
	int m_nAcCountPercent;		//超声合格品件数百分比
	int m_nAcLength;			//超声合格品长度
	int m_nAcLengthPercent;		//超声合格品长度百分比
	int m_nReCount;				//超声不合格品件数
	int m_nReCountPercent;		//超声不合格品件数百分比
	int m_nReLength;			//超声不合格品长度
	int m_nReLengthPercent;		//超声不合格品长度百分比
	int m_nBaCount;				//超声耦合不良件数
	int m_nBaCountPercent;		//超声耦合不良件数百分比
	int m_nBaLength;			//超声耦合不良长度
	int m_nBaLengthPercent;		//超声耦合不良长度百分比
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
