/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: EditUserPerDlg.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-27
*/


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "EditUserPerDlg.h"


extern QSqlDatabase* g_sqlDB;

EditUserPerDlg::EditUserPerDlg(QWidget * parent):QDialog(parent)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

EditUserPerDlg::~EditUserPerDlg(void)
{

}

void EditUserPerDlg::InitDialog(void)
{
    this->resize(300,300);
	setStyleSheet("QDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLabel{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLineEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0);}\
				  QComboBox{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}");

	horizontalLayout = new QHBoxLayout;
	horizontalLayout->setSpacing(6);
	horizontalLayout->setContentsMargins(5, 5, 5, 5);

	m_pbOK = new QPushButton(this);
	m_pbOK->setMinimumSize(QSize(100, 25));
	m_pbOK->setMaximumSize(QSize(150, 25));
	horizontalLayout->addWidget(m_pbOK);

	m_pHSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(m_pHSpacer);

	m_pbCancel = new QPushButton(this);
	m_pbCancel->setMinimumSize(QSize(100, 25));
	m_pbCancel->setMaximumSize(QSize(150, 25));
	horizontalLayout->addWidget(m_pbCancel);


	gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->setContentsMargins(0, 0, 0, 0);

	m_plbUserName = new QLabel(this);
	m_plbUserName->setMinimumSize(QSize(60, 25));
	m_plbUserName->setMaximumSize(QSize(60,25));
	m_plbUserName->setAlignment(Qt::AlignLeft);
	gridLayout->addWidget(m_plbUserName, 0, 0, 1, 1);

	m_pleUserName = new QLineEdit(this);
	m_pleUserName->setMinimumSize(QSize(120, 25));
	m_pleUserName->setMaximumSize(QSize(120, 25));
	m_pleUserName->setReadOnly(true);
	gridLayout->addWidget(m_pleUserName, 0, 1, 1, 1);

	m_plbUserPer = new QLabel(this);
	m_plbUserPer->setMinimumSize(QSize(60, 25));
	m_plbUserPer->setMaximumSize(QSize(60, 25));
	m_plbUserPer->setAlignment(Qt::AlignLeft);
	gridLayout->addWidget(m_plbUserPer, 1, 0, 1, 1);

	m_pcbUserPer = new QComboBox(this);
	m_pcbUserPer->setMinimumSize(QSize(120, 25));
	m_pcbUserPer->setMaximumSize(QSize(120, 25));
	m_pcbUserPer->insertItem(0,QString("admin"));
	m_pcbUserPer->insertItem(1,QString("xxx"));
	gridLayout->addWidget(m_pcbUserPer, 1, 1, 1, 1);
	gridLayout->addLayout(horizontalLayout,2,0,1,2);


}

void EditUserPerDlg::translate(void)
{
	setWindowTitle(tr("修改权限"));
	m_pbOK->setText(tr("确定"));
	m_pbCancel->setText(tr("取消"));
	m_plbUserName->setText(tr("用户名"));
	m_plbUserPer->setText(tr("权限"));

}

void EditUserPerDlg::InsertDataToDialog(void)
{
	QString strSql = QString("%1%2%3").arg("select * from t_User where UserName ='").arg(m_strUserName).arg("'");
	QSqlQuery query(*g_sqlDB);
	query.exec(strSql);
	if (!query.isActive())
	{
		QMessageBox::information(this,"InsertDataToTableView",tr("SQL语句执行失败"));
		return;
	}
	query.next();
	m_pleUserName->setText(query.value(1).toString());
}

void EditUserPerDlg::OnCancel()
{
	close();
}
void EditUserPerDlg::OnOK()
{
	QString strUserPer = m_pcbUserPer->currentText();
	QString strSql = QString("%1%2%3%4%5%6").arg("update t_User set ugroup = '").arg(strUserPer).arg("'").arg(" where UserName = '").arg(m_strUserName).arg("'");
	QSqlQuery query(*g_sqlDB);
	query.exec(strSql);

	if (!query.isActive())
	{
		QMessageBox::information(this,"InsertDataToTableView",tr("SQL语句执行失败"));
		return;
	}
	emit OnUpdateTableViewFEUPDlg();
	close();
}
