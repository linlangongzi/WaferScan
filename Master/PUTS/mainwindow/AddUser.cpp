/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 添加用户界面
* Filename: AddUser.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-27
*/



#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "AddUser.h"


extern QSqlDatabase* g_sqlDB;
AddUser::AddUser(QWidget *parent):QDialog(parent)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

AddUser::~AddUser(void)
{

}

void AddUser::InitDialog(void)
{
	setStyleSheet("QDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLabel{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLineEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0);}\
				  QComboBox{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}");

	horizontalLayout_2 = new QHBoxLayout(this);
	horizontalLayout_2->setSpacing(6);
	horizontalLayout_2->setContentsMargins(11, 11, 11, 11);

	groupBox = new QGroupBox(this);
	verticalLayout = new QVBoxLayout(groupBox);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(11, 11, 11, 11);
	m_pGLayoutTop = new QGridLayout();
	m_pGLayoutTop->setSpacing(6);
	m_pGLayoutTop->setVerticalSpacing(30);
	m_pGLayoutTop->setContentsMargins(0, 10, 0, 10);
	m_plbUserName = new QLabel(groupBox);

	m_plbUserName->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbUserName, 0, 0, 1, 1);

	m_pleUserName = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_pleUserName, 0, 1, 1, 1);

	m_plbUserPer = new QLabel(groupBox);
	m_plbUserPer->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbUserPer, 1, 0, 1, 1);

	m_pcbUserPer = new QComboBox(groupBox);
	m_pcbUserPer->insertItem(0,QString::fromLocal8Bit("admin"));
	m_pcbUserPer->insertItem(1,QString::fromLocal8Bit("xxx"));
	m_pGLayoutTop->addWidget(m_pcbUserPer, 1, 1, 1, 1);

	m_plbPwd = new QLabel(groupBox);
	m_plbPwd->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbPwd, 2, 0, 1, 1);

	m_plePwd = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_plePwd, 2, 1, 1, 1);

	m_plbComfirePwd = new QLabel(groupBox);
	m_plbComfirePwd->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbComfirePwd, 3, 0, 1, 1);

	m_pleComfirePwd = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_pleComfirePwd, 3, 1, 1, 1);
	verticalLayout->addLayout(m_pGLayoutTop);

	m_phLayoutGroup = new QHBoxLayout();
	m_phLayoutGroup->setSpacing(6);
	m_phLayoutGroup->setSizeConstraint(QLayout::SetDefaultConstraint);
	m_phLayoutGroup->setContentsMargins(0, 60, 1, 1);

	m_pbOK = new QPushButton(groupBox);
	m_pbOK->setMinimumSize(QSize(100, 25));
	m_phLayoutGroup->addWidget(m_pbOK);

	m_phSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_phLayoutGroup->addItem(m_phSpacer);

	m_pbCancel = new QPushButton(groupBox);
	m_pbCancel->setMinimumSize(QSize(100, 25));
	m_phLayoutGroup->addWidget(m_pbCancel);
	verticalLayout->addLayout(m_phLayoutGroup);
	horizontalLayout_2->addWidget(groupBox);
}


void AddUser::translate(void)
{
	setWindowTitle(tr("添加用户"));
	m_plbUserName->setText(tr("用户名"));
	m_plbUserPer->setText(tr("权限"));
	m_plbPwd->setText(tr("密码"));
	m_plbComfirePwd->setText(tr("确认密码"));
	m_pbOK->setText(tr("确定"));
	m_pbCancel->setText(tr("取消"));
}

void AddUser::OnCancel()
{
	close();
}

void AddUser::OnOK()
{
	//提取信息
	QString strUerName = m_pleUserName->text();
	if (strUerName.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("用户名不能为空"));
		return;
	}
	QString strUserPer = m_pcbUserPer->currentText();
	if (strUserPer.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("用户权限不能为空"));
		return;
	}
	QString strPwd = m_plePwd->text();
	if (strPwd.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("密码不能为空"));
		return;
	}
	QString strPwdConfir = m_pleComfirePwd->text();
	if (strPwdConfir.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("确认密码不能为空"));
		return;
	}
	if (strPwd != strPwdConfir)
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("密码不一致"));
		return;
	}
	QString strSql = QString("%1%2%3%4%5%6%7%8%9%10").arg("insert into t_User(UserName,PassWord,ugroup,name) values(").arg("'").arg(strUerName).arg("','")
		.arg(strPwd).arg("','").arg(strUserPer).arg("','").arg(QString("DOTA2")).arg("')");

	if (NULL == g_sqlDB)
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("g_sqlDB为空"));
	}
	QSqlQuery query(*g_sqlDB);
	query.exec(strSql);
	if (!query.isActive())
	{
		QString strError = query.lastError().text();
		QMessageBox::information(this,"AddUser::OnOK",tr("SQL语句执行失败,%1").arg(strError));
	}
	//更新视图
	emit UpdateTableView();
	close();
}
