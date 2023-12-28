/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板参数手动配置界面
* Filename: PlateParaSetDialog.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/

#ifndef SPLATEEHETTING_H
#define SPLATEEHETTING_H


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
#include <QRadioButton>
#include <QCheckBox>
#include "DataDefine.h"

class CPlateParaSetDialog : public QDialog
{
	Q_OBJECT
public:
	CPlateParaSetDialog(QMap<QString,int> &FlagMap,CPlateInfo& PalateInfo,QSqlDatabase *pDatabaseConnection = NULL,QWidget *parent = NULL);		                
	~CPlateParaSetDialog(void);
private:
	void initDialog(void);
	void insertData(void);
	void retranslate(void);

public slots:
	void OnSwitch(void);       //切换到ERP窗口
	void OnOK(void);           //确认
	void OnCancel(void);       //取消
	void reset(void);          //重置 
	void OnSelChangeComboStandard(int Index);   //响应Combobox
protected:
	void keyPressEvent(QKeyEvent *e);   //过滤ESC
public:
	bool m_bIsOK;                 //标志点确定还是取消
	bool m_bIsSwitch;             //切换配置窗口
private:	
	QGridLayout *m_pGroupLeftGridLayout;
	QGridLayout *m_pGroupRightGridLayout;
	QVBoxLayout *m_pLeftCommentLayout;
	QVBoxLayout *m_pRightCommentLayout;
	QHBoxLayout *m_pButtonLayout;
	QGridLayout *m_pMainLayout;
	QGroupBox *m_pgbUp;
	QLabel *m_plSPNumber;
	QLabel *m_plTime;
	QLineEdit *m_pleTime;
	QLineEdit *m_pleSPNumber;
	QLabel *m_plPGrade;
	QLineEdit *m_pleHNumber;
	QLabel *m_plFNumber;
	QLineEdit *m_pleFNumber;
	QLabel *m_plHNumber;
	QLineEdit *m_plePGrade;
	QLabel *m_plComment1;
	QTextEdit *m_pteComment1;
	QGroupBox *m_pgbDown;
	QLabel *m_plThickness;
	QLineEdit *m_pleLength;
	QLineEdit *m_pleThickness;
	QLabel *m_plDStandard;
	QLabel *m_plWidth;
	QLineEdit *m_pleWidth;
	QLabel *m_plELevel;
	QLabel *m_plBLevel;
	QLabel *m_plProbeType;
	QComboBox *m_pcbDStandard;
	QComboBox *m_pcbBLevel;
	QComboBox *m_pcbELevel;
	QComboBox *m_pcbProbeType;
	QLabel *m_plLength;
	QLabel *m_plComment2;
	QTextEdit *m_pteComment2;
	QPushButton *m_ppbCancel;
	QSpacerItem *m_horizontalSpacer;
	QPushButton *m_pbReput;
	QSpacerItem *m_phorizontalSpacern;
	QPushButton *m_ppbOk;
	QPushButton *m_ppbSwitch;
	QSqlDatabase *m_pDatabaseConnection;
	CPlateInfo   *m_pPlateInfo;
    QMap<QString,int> *m_pFlagMap;
	QSettings *m_settings;           //配置文件对象
	QString m_IniFlieName;              //配置文件名

    //chenyuzhu add start on 2014.5.13
	QLabel *m_pShiban;
	//QRadioButton *m_pShibanCheck;
	QCheckBox *m_pShibanCheck;
	//chenyuzhu add end on 2014.5.13
	QLabel *m_pRenYuan;
	QLineEdit *m_pRenYuanEdit;
	QLabel *m_pBanJi;
	QLineEdit *m_pBanJiEdit;

};


#endif // SPLATEEHETTING_H