/**
* Copyright(c) 2014 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 标定模式勾选对话框
* Filename: QBiaoDingcpp
*
* @author        Chen Yuzhu
* @version       1.00         Date: 2014-11-06
*/
#include "QBiaoDing.h"

#include <QMessageBox>



QBiaoDing::QBiaoDing(QWidget *parent)
	: QDialog(parent)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
//	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

QBiaoDing::~QBiaoDing()
{

}

void QBiaoDing::InitDialog(void)
{
	setStyleSheet("QDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLabel{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLineEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0);}\
				  QComboBox{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}");
	setMaximumSize(QSize(250, 150));
	setMinimumSize(QSize(250, 150));

	m_plbPwd = new QLabel(this);
	m_plbPwd->setMinimumSize(QSize(100, 50));
	m_plbPwd->setGeometry(QRect(10, 10, 110, 50));
	m_pBiaodingCheck = new QCheckBox(this);
	m_pBiaodingCheck->setGeometry(QRect(130, 10, 50, 50));
	m_pbOK = new QPushButton(this);
	m_pbOK->setGeometry(QRect(70, 80, 110, 30));

}


void QBiaoDing::translate(void)
{
	setWindowTitle(tr("标定模式选择面板"));
	m_plbPwd->setText(tr("标定选中"));
	m_pbOK->setText(tr("确定"));

}

void QBiaoDing::OnCancel()
{
	//m_IfLock = false;
	close();
}

void QBiaoDing::OnOK()
{
	//提取信息

/*	QString strPwd = m_plePwd->text();
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
	m_IfLock = true;*/
	close();
}