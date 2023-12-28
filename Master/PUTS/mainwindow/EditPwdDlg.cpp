/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: EditPwdDlg.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-08-28
*/

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTextCodec>
#include "EditPwdDlg.h"

extern QSqlDatabase* g_sqlDB;


EditPwdDlg::EditPwdDlg(QWidget * parent):QDialog(parent)
{
	InitDialog();
	translate();
	connect(m_pbOK,SIGNAL(clicked()),this,SLOT(OnOK()));
	connect(m_pbCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
}

EditPwdDlg::~EditPwdDlg(void)
{
}


void EditPwdDlg::InsetDataToDialog(void)
{
	if (NULL == g_sqlDB)
	{
		QMessageBox::information(this," EditPwdDlg::InsetDataToDialog",tr("g_sqlDB为空"));
		return;
	}
	QSqlQuery query(*g_sqlDB);
	QString strSql = QString("%1%2%3").arg("select * from UserInfo where User_Name = '").arg(m_strUserName).arg("'");
	query.exec(strSql);
	if (!query.isActive())
	{
		QMessageBox::information(this,"EditPwdDlg::InsetDataToDialog",tr("SQL语句执行失败"));
		return;
	}
	query.next();
	m_pleUserName->setText(query.value(1).toString());
	m_pleUserName->setReadOnly(true);
	m_strOldPwd = query.value(2).toString();
	m_pleOldPwd->setText(QString(""));
	m_pleNewPwd->setText(QString(""));;
	m_pleComfirPwd->setText(QString(""));
}

void EditPwdDlg::OnCancel()
{
	close();
}
void EditPwdDlg::OnOK(void)
{
	QString tempPwd = m_pleOldPwd->text();

	QString pwdmd5;
	QByteArray bb;
	bb = QCryptographicHash::hash (tempPwd.toLocal8Bit(), QCryptographicHash::Md5 );
	pwdmd5.append(bb.toHex());  
	pwdmd5 = pwdmd5.toLower();
	m_strOldPwd = m_strOldPwd.toLower();

	if (m_strOldPwd != pwdmd5 )
	{
		QMessageBox::information(this,"EditPwdDlg::OnOK",tr("原密码输入不正确"));
		return;
	}
	QString strNPwd = m_pleNewPwd->text();
	QString strCNPwd = m_pleComfirPwd->text();
	if (strNPwd != strCNPwd)
	{
		QMessageBox::information(this,"EditPwdDlg::OnOK",tr("密码输入不一致"));
		return;
	}
	
	QString npwdmd5;
	QByteArray bbb;
	bbb = QCryptographicHash::hash (strNPwd.toLocal8Bit(), QCryptographicHash::Md5 );
	npwdmd5.append(bbb.toHex());  

	QString strSQL = QString("%1%2%3%4%5%6").arg("update UserInfo set User_Pwd = '").arg(npwdmd5).arg("'").arg("where User_Name = '").arg(m_strUserName).arg("'");
	QSqlQuery query(*g_sqlDB);
	query.exec(strSQL);
	if (!query.isActive())
	{
		QMessageBox::information(this,"EditPwdDlg::OnOK",tr("SQL语句执行失败"));
		return;
	}
	emit UpdateTableViewFromEditPwd();
	close();
}


void EditPwdDlg::InitDialog(void)
{
	setStyleSheet("QDialog{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLabel{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);}\
				  QLineEdit{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,0);}");
	
	this->resize(300,350);
	gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(20);
	gridLayout->setContentsMargins(11, 11, 11, 11);

	m_lbUserName = new QLabel(this);
	m_lbUserName->setMinimumSize(QSize(100, 0));
	m_lbUserName->setMaximumSize(QSize(100,25));

	gridLayout->addWidget(m_lbUserName, 0, 0, 1, 1);

	m_pleUserName = new QLineEdit(this);
	m_pleUserName->setMinimumSize(QSize(100, 25));
	m_pleUserName->setMaximumSize(QSize(250, 25));
	gridLayout->addWidget(m_pleUserName, 0, 1, 1, 1);

	m_plbOldPwd = new QLabel(this);
	m_plbOldPwd->setMinimumSize(QSize(100, 0));
	m_plbOldPwd->setMaximumSize(QSize(100,25));

	gridLayout->addWidget(m_plbOldPwd, 1, 0, 1, 1);

	m_pleOldPwd = new QLineEdit(this);
	m_pleOldPwd->setMaximumSize(QSize(250, 25));
	gridLayout->addWidget(m_pleOldPwd, 1, 1, 1, 1);

	m_plbNewPwd = new QLabel(this);
	m_plbNewPwd->setMinimumSize(QSize(100, 0));
	m_plbNewPwd->setMaximumSize(QSize(100,25));

	gridLayout->addWidget(m_plbNewPwd, 2, 0, 1, 1);

	m_pleNewPwd = new QLineEdit(this);
	m_pleNewPwd->setMaximumSize(QSize(250, 25));
	gridLayout->addWidget(m_pleNewPwd, 2, 1, 1, 1);

	m_plbComfirPwd = new QLabel(this);
	m_plbComfirPwd->setMinimumSize(QSize(100, 0));
	m_plbComfirPwd->setMaximumSize(QSize(100,25));


	gridLayout->addWidget(m_plbComfirPwd, 3, 0, 1, 1);

	m_pleComfirPwd = new QLineEdit(this);
	m_pleComfirPwd->setMaximumSize(QSize(250, 25));
	gridLayout->addWidget(m_pleComfirPwd, 3, 1, 1, 1);

    m_pButtomLayOut = new QHBoxLayout;
	m_phSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_pbOK = new QPushButton(this);
	m_pbOK->setMinimumSize(QSize(100, 0));
	m_pbOK->setMaximumSize(QSize(200, 25));
	m_pButtomLayOut->addWidget(m_pbOK);
	m_pButtomLayOut->addItem(m_phSpacer);

	m_pbCancel = new QPushButton(this);
	m_pbCancel->setMinimumSize(QSize(100, 0));
	m_pbCancel->setMaximumSize(QSize(200, 25));
	m_pButtomLayOut->addWidget(m_pbCancel);
	gridLayout->addLayout(m_pButtomLayOut,4,0,1,2);
}

void EditPwdDlg::translate(void)
{
	setWindowTitle(tr("修改密码"));
	m_lbUserName->setText(tr("用户名"));
	m_plbOldPwd->setText(tr("原密码"));
	m_plbNewPwd->setText(tr("新密码"));
	m_plbComfirPwd->setText(tr("再次确认"));
	m_pbOK->setText(tr("确定"));
	m_pbCancel->setText(tr("取消"));
}
