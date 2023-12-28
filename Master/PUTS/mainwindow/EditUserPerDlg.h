/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: EditUserPerDlg.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-27
*/

#ifndef EDITUSERPERDLG_H
#define EDITUSERPERDLG_H


#include <QDialog>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>


class EditUserPerDlg :public QDialog
{
	Q_OBJECT
public:
	EditUserPerDlg(QWidget * parent);
	~EditUserPerDlg(void);

public:
	void InitDialog(void);
	void translate(void);
	void InsertDataToDialog(void);
signals:
	void OnUpdateTableViewFEUPDlg(void);
public slots:
	void OnOK();
	void OnCancel();
public:
	QHBoxLayout *horizontalLayout;
	QPushButton *m_pbOK;
	QSpacerItem *m_pHSpacer;
	QPushButton *m_pbCancel;
	QGridLayout *gridLayout;
	QComboBox *m_pcbUserPer;
	QLineEdit *m_pleUserName;
	QLabel *m_plbUserName;
	QLabel *m_plbUserPer;
	QString m_strUserName;
};




#endif   //EDITUSERPERDLG_H
