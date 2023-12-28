/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: EditPwdDlg.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-28
*/

#pragma once
#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QSpacerItem>



class EditPwdDlg :public QDialog
{
	Q_OBJECT
public:
	EditPwdDlg(QWidget * parent = NULL);
	~EditPwdDlg(void);
public:
	void InitDialog(void);
	void translate(void);
	void InsetDataToDialog(void);
public slots:
	void OnOK(void);
	void OnCancel();
signals:
	void UpdateTableViewFromEditPwd();


public:
	QGridLayout *gridLayout;
	QHBoxLayout *m_pButtomLayOut;
	QSpacerItem *m_phSpacer;

	QLabel *m_lbUserName;
	QLineEdit *m_pleUserName;
	QLabel *m_plbOldPwd;
	QLineEdit *m_pleOldPwd;
	QLabel *m_plbNewPwd;
	QLineEdit *m_pleNewPwd;
	QLabel *m_plbComfirPwd;
	QLineEdit *m_pleComfirPwd;
	QPushButton *m_pbOK;
	QPushButton *m_pbCancel;
	QString     m_strUserName;
	QString     m_strOldPwd;

};
