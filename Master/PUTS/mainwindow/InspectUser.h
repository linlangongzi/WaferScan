/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 验证用户界面
* Filename: InspectUser.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-05-27
*/

#ifndef INSPECTUSER_H
#define INSPECTUSER_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSqlDatabase>

class CInspectUser : public QDialog
{
	Q_OBJECT

public:
	CInspectUser(QSqlDatabase *pDatabaseConnection);
	~CInspectUser();
private:
	QLabel *m_pLblUser;
	QLabel *m_pLblPwd;
	QComboBox *m_pCbUser;
	QLineEdit *m_pLePwd;
	QPushButton *m_pPstBtnOK;
	QPushButton *m_pPstBtnCancel;
	QSqlDatabase *m_pDDatabaseConnection;
private:
	void InitDialog();
public:
	bool m_isOK;//用户验证通过标识变量
//定义槽函数
public slots:
	void CheckOnInfor(void);
	void DestroyLogOnDialog(void);
	
};

#endif // INSPECTUSER_H
