/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 工具面板
* Filename: UserManager.h
*
* @author        DongQi
* @version       1.00         Date: 2013-08-26
*/
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QWidget> 
#include <QString>
#include <QMessageBox>
#include "AddUser.h"
#include "EditPwdDlg.h"
#include "EditUserPerDlg.h"


class UserManager :public QWidget
{
	Q_OBJECT
public:
	UserManager(QWidget* parent = NULL);
	~UserManager(void);
public:
	void InitDialog(void);
	void translate(void);
	void InsertDataToTableView(void);
public slots:
	void OnAddUser(void);                   //添加用户 
	void OnEditPwd(void);                   //修改密码
	void UpdateTable(void);                 //响应刷新TabelView
	void OnDeleteUser(void);                //删除记录
	void OnEditUserPermission(void);        //修改用户权限
public:
	QHBoxLayout *horizontalLayout_2;
	QGroupBox *groupBox;
	QHBoxLayout *horizontalLayout;
	QTableView *m_pUserInformation;
	QStandardItemModel *m_pUserInfoModel;
	QVBoxLayout *m_pRightButtonLayOut;
	QPushButton *m_pbEditUserPer;
	QSpacerItem *m_pFVerSpacer;
	QPushButton *m_pbDeleteUser;
	QSpacerItem *m_pSVerSpacer;
	QPushButton *m_pbAddUser;
	QSpacerItem *m_pTVerSpacer;
	QPushButton *m_pbEditPassword;
	QSpacerItem *m_pFoVerSpacer;
	QPushButton *m_pbExit;
	AddUser     *m_pbUserDialog;
	EditPwdDlg  *m_pEditPwdDlg;
	EditUserPerDlg *m_pEditUserPerDlg;
	
};





#endif //USERMANAGER_H