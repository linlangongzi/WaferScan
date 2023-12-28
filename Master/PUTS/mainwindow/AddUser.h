/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 添加用户界面
* Filename: AddUser.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-27
*/


#ifndef ADDUSER_H
#define ADDUSER_H

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

class AddUser:public QDialog
{
	Q_OBJECT
public:
	AddUser(QWidget *parent = NULL);
	~AddUser(void);
public:
	void InitDialog(void);
	void translate(void);
public slots:
	void OnOK();
	void OnCancel();
signals:
	void UpdateTableView();

public:
	QHBoxLayout *horizontalLayout_2;
	QGroupBox *groupBox;
	QVBoxLayout *verticalLayout;
	QGridLayout *m_pGLayoutTop;
	QLabel *m_plbUserName;
	QLineEdit *m_pleUserName;
	QLabel *m_plbUserPer;
	QComboBox *m_pcbUserPer;
	QLabel *m_plbPwd;
	QLineEdit *m_plePwd;
	QLabel *m_plbComfirePwd;
	QLineEdit *m_pleComfirePwd;
	QHBoxLayout *m_phLayoutGroup;
	QPushButton *m_pbOK;
	QSpacerItem *m_phSpacer;
	QPushButton *m_pbCancel;
};


#endif     // ADDUSER_H
