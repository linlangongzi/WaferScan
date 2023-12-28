/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 标定模式勾选对话框
* Filename: QBiaoDing.h
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-11-06
*/

#ifndef QBIAODING_H
#define QBIAODING_H

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
#include <QCheckBox>

class QBiaoDing : public QDialog
{
	Q_OBJECT

public:
	QBiaoDing(QWidget *parent);
	~QBiaoDing();

private:

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

	QCheckBox *m_pBiaodingCheck;  
public slots:
		void OnOK(void);
		void OnCancel(void);
	
};

#endif // QBIAODING_H
