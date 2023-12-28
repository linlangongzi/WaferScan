/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 登录界面
* Filename: CLogOnDialog.h
*
* @author        DongQi
* @version       1.00         Date: 2013-04-27
*/

#ifndef LOGONDIALOG_H
#define LOGONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSqlDatabase>
#include <QGridLayout>
#include <QKeyEvent>


class CLogOnDialog : public QDialog
{
	Q_OBJECT

public:
	CLogOnDialog(QSqlDatabase *pDatabaseConnection);
	~CLogOnDialog(void);
private:
	void InitMyDialog (void);
public:
	QComboBox *m_pcbUser;  //用户名
private:
	QLabel *m_plblUser;        
	QLabel *m_plblPwd;
	QLineEdit *m_plePwd;
	QPushButton *m_pbgOk;
	QPushButton *m_pbgCancel;
	QSqlDatabase *m_pDDatabaseConnection;
	QGridLayout  *m_pMainGridLayout;
	QGridLayout  *m_pShowGridLayout;

	//定义槽函数
public slots:
		void CheckOnInfor(void);
		void DestroyLogOnDialog(void);
protected:
	void keyPressEvent(QKeyEvent *e);   //过滤ESC

};

#endif // LOGONDIALOG_H
