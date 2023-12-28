/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 锁定密码与解锁密码对话框类
* Filename: CAScanLock.h
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-04-29
*/

#pragma once
//#include "c:\qt\4.8.4\src\gui\dialogs\qdialog.h"
#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>



class CAscanLock :
	public QDialog
{
	Q_OBJECT
public:
	CAscanLock(void);
	~CAscanLock(void);
public:
	void InitDialog(void);
	void translate(void);
public:
	QHBoxLayout *horizontalLayout_2;
	QGroupBox *groupBox;
	QVBoxLayout *verticalLayout;
	QGridLayout *m_pGLayoutTop;
	QLabel *m_plbPwd;
	QLineEdit *m_plePwd;
	QLabel *m_plbComfirePwd;
	QLineEdit *m_pleComfirePwd;
	QHBoxLayout *m_phLayoutGroup;
	QPushButton *m_pbOK;
	QSpacerItem *m_phSpacer;
	QPushButton *m_pbCancel;

	QString m_strPwd;

	bool m_IfLock;
public slots:
		void OnOK(void);
		void OnCancel(void);
};

class CAscanUnLock :
	public QDialog
{
	Q_OBJECT
public:
	CAscanUnLock(void);
	~CAscanUnLock(void);
public:
	void InitDialog(void);
	void translate(void);
public:
	QHBoxLayout *horizontalLayout_2;
	QGroupBox *groupBox;
	QVBoxLayout *verticalLayout;
	QGridLayout *m_pGLayoutTop;
	QLabel *m_plbPwd;
	QLineEdit *m_plePwd;
	QHBoxLayout *m_phLayoutGroup;
	QPushButton *m_pbOK;
	QSpacerItem *m_phSpacer;
	QPushButton *m_pbCancel;

	QString m_strPwd;
	
	bool m_IfUnLock;
public slots:
	void OnOK(void);
	void OnCancel(void);
};
