/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板参数ERP配置界面
* Filename: PlateParaSetERPDialog.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/


#ifndef SPLATEESETTING_H
#define SPLATEESETTING_H

#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSettings>
#include <QKeyEvent>
#include "DataDefine.h"

class CPlateParaSetERPDialog : public QDialog
{
	Q_OBJECT
public:
	CPlateParaSetERPDialog(QMap<QString,int> &FlagMap,CPlateInfo& PalateInfo,QSqlDatabase *pDatabaseConnection = NULL,QWidget *parent = NULL);
	~CPlateParaSetERPDialog(void);
private:
	void initDialog(void);
	void initDataBase(void);
	void translate(void);
public slots:
	void insertData();                 //插入数据
	void OnSelChangeComboStandard(int Index);   //响应探伤标准
	void OnOK(void);                            //确定
	void OnCancel(void);                        //取消
 	void keyPressEvent(QKeyEvent *e);   //过滤ESC
	void OnSwitch();                    //切换到手动模式  
public:
	bool m_bIsOK;                 //标志点确定还是取消
	bool m_bIsSwitch;             //标志是否切换到手动输入状态
private:
    QHBoxLayout  *m_pTopLeftLayout; //左上角标签和组合框的布局
	QVBoxLayout  *m_pLeftVBoxLayOut;
	QGridLayout  *m_pGroup1GridLayOut;
	QGridLayout  *m_pGroup2GridLayOut;
	QHBoxLayout  *m_pButtonHBoxLayOut;
	QGridLayout  *m_pMainGridLayout;
	QVBoxLayout  *m_pCommentleftLayOut;
	QVBoxLayout  *m_pCommentRightLayOut;
	QLabel *m_pSteelNum;
	QLineEdit *m_plePlateDPINumber;   //钢板号编辑框
	QPushButton *m_ppbPlateDPINumber; //钢板号确认按钮
	QTextEdit *m_pteLeft;
	QGroupBox *m_pgroupBox;
	QLabel *m_plSPNumber;
	QLineEdit *m_pleSPNumber;   //钢板号
	QLabel *m_plTime;
	QLineEdit *m_pleTime;
	QLabel *m_plPGrade;
	QLineEdit *m_pleSGrade;
	QLabel *m_plANumber;
	QLineEdit *m_pleANumber;
	QLabel *m_plHNumber;
	QLineEdit *m_pleHNumber;    //合同号
	QGroupBox *m_pRightgroupBox;
	QLabel *m_plLength;
	QLineEdit *m_pleLength;
	QLabel *m_plDStandard;
	QLabel *m_plWidth;
	QLineEdit *m_pleWidth;
	QLabel *m_plBLevel;
	QLabel *m_plThickness;
	QLineEdit *m_pleThickness;
	QLabel *m_plELevel;
	QLabel *m_plProbeType;
	QComboBox *m_pcbDStandard;
	QComboBox *m_pcbBLevel;
	QComboBox *m_pcbELevel;
	QComboBox *m_pcbProbeType;
	QTextEdit *m_pteComment1;
	QTextEdit *m_pteComment2;
	QPushButton *m_ppbExit;
	QSpacerItem *m_phorizontalSpacer;
	QPushButton *m_ppbOk;
	QPushButton *m_ppbSwitch;       //切换手动按钮
	QLabel      *m_plbCommentleft;
	QLabel      *m_plbCommentRight;
	QGroupBox *m_pTopLeftGroupBox;
	QSqlDatabase *m_pDatabaseConnection;
	CPlateInfo   *m_pPalateInfo;  //钢板信息对象
    CPlateInfo   *m_pTempInfo;
    //标示
	QMap<QString,int> *m_pFlagMap;
	QSettings *m_settings;           //配置文件对象
	QString m_IniFlieName;              //配置文件名

	QLabel *m_pRenYuan;
	QLineEdit *m_pRenYuanEdit;
	QLabel *m_pBanJi;
	QLineEdit *m_pBanJiEdit;

};


#endif // SPLATEESETTING_H