/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 锁定密码与解锁密码对话框类
* Filename: CAScanLock.h
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-04-29
*/

#include <QMessageBox>
#include "CAScanLock.h"

QString g_strPwd=0;

CAscanLock::CAscanLock(void)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

CAscanLock::~CAscanLock(void)
{
}
void CAscanLock::InitDialog(void)
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



	m_plbPwd = new QLabel(groupBox);
	m_plbPwd->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbPwd, 0, 0, 1, 1);

	m_plePwd = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_plePwd, 0, 1, 1, 1);

	m_plbComfirePwd = new QLabel(groupBox);
	m_plbComfirePwd->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbComfirePwd, 1, 0, 1, 1);

	m_pleComfirePwd = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_pleComfirePwd, 1, 1, 1, 1);
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


void CAscanLock::translate(void)
{
	setWindowTitle(tr("设置锁定密码"));
	m_plbPwd->setText(tr("锁定密码"));
	m_plbComfirePwd->setText(tr("确认密码"));
	m_pbOK->setText(tr("确定"));
	m_pbCancel->setText(tr("取消"));
}

void CAscanLock::OnCancel()
{
	m_IfLock = false;
	close();
}

void CAscanLock::OnOK()
{
	//提取信息

	QString strPwd = m_plePwd->text();
	if (strPwd.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("密码不能为空"));
		m_IfLock = false;
		return;
	}
	QString strPwdConfir = m_pleComfirePwd->text();
	if (strPwdConfir.isEmpty())
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("确认密码不能为空"));
		m_IfLock = false;
		return;
	}
	if (strPwd != strPwdConfir)
	{
		QMessageBox::information(this,"AddUser::OnOK",tr("密码不一致"));
		m_IfLock = false;
		return;
	}	
	g_strPwd = strPwd;
	m_IfLock = true;
	close();
}


CAscanUnLock::CAscanUnLock(void)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

CAscanUnLock::~CAscanUnLock(void)
{
}
void CAscanUnLock::InitDialog(void)
{
	setStyleSheet("QDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLabel{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLineEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0);}\
				  QComboBox{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}");
	horizontalLayout_2 = new QHBoxLayout(this);//水平布局
	horizontalLayout_2->setSpacing(6);
	horizontalLayout_2->setContentsMargins(11, 11, 11, 11);

	groupBox = new QGroupBox(this);
	verticalLayout = new QVBoxLayout(groupBox);//垂直布局
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(11, 11, 11, 11);
	m_pGLayoutTop = new QGridLayout();
	m_pGLayoutTop->setSpacing(6);
	m_pGLayoutTop->setVerticalSpacing(30);
	m_pGLayoutTop->setContentsMargins(0, 10, 0, 10);



	m_plbPwd = new QLabel(groupBox);
	m_plbPwd->setMinimumSize(QSize(100, 50));
	m_pGLayoutTop->addWidget(m_plbPwd, 0, 0, 1, 1);

	m_plePwd = new QLineEdit(groupBox);
	m_pGLayoutTop->addWidget(m_plePwd, 0, 1, 1, 1);

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


void CAscanUnLock::translate(void)
{
	setWindowTitle(tr("解锁密码"));
	m_plbPwd->setText(tr("解锁密码"));
	m_pbOK->setText(tr("确定"));
	m_pbCancel->setText(tr("取消"));
}

void CAscanUnLock::OnCancel()
{
	m_IfUnLock = false;
	close();
}

void CAscanUnLock::OnOK()
{
	//提取信息

	QString strPwd = m_plePwd->text();
	if (strPwd.isEmpty())
	{
		QMessageBox::information(this,"Unlock::OnOK",tr("密码不能为空"));
		m_IfUnLock = false;
		return;
	}

	if (g_strPwd == strPwd)
	{
		QMessageBox::information(this,"Unlock::OnOK",tr("密码正确"));

	}
	else
	{
		QMessageBox::information(this,"Unlock::OnOK",tr("密码错误"));
		m_IfUnLock = false;
		return;
	}
	m_IfUnLock = true;
	close();
}