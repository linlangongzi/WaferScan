/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 验证用户界面
* Filename: InspectUser.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-05-27
*/

#include <QString>
#include <QFont>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QTextCodec>
#include "DataDefine.h"
#include "InspectUser.h"

CInspectUser::CInspectUser(QSqlDatabase *pDatabaseConnection):m_pDDatabaseConnection(pDatabaseConnection)
{
	m_isOK = false;//用户验证通过标识变量
	InitDialog();
	connect(m_pPstBtnCancel,SIGNAL(clicked ()),this,SLOT(DestroyLogOnDialog()));
	connect(m_pPstBtnOK,SIGNAL(clicked()),this,SLOT(CheckOnInfor()));  
}

CInspectUser::~CInspectUser()
{

}
void CInspectUser::InitDialog()
{
	setStyleSheet(QString::fromUtf8("background-color: rgb(73, 73, 73)"));
	setWindowTitle(tr("请输入用户名及密码"));
	setWindowOpacity(0.9);
	setWindowFlags(Qt::SubWindow);
	setMaximumSize(300,180);
	setMinimumSize(300,180);

	QLabel *lbUser = new QLabel(this);
	QFont ft;
	ft.setPointSize(15);
	lbUser->setFont(ft);
	lbUser->setText(tr("用户名:"));
	lbUser->setAlignment(Qt::AlignLeft);
	lbUser->setGeometry(20,20,80,26);


	QLabel *lblPwd = new QLabel(this);
	ft.setPointSize(15);
	lblPwd->setFont(ft);
	lblPwd->setText(tr("密码:"));
	lblPwd->setAlignment(Qt::AlignLeft);
	lblPwd->setGeometry(20,70,80,26);


	m_pCbUser = new QComboBox(this);
	m_pCbUser->setStyleSheet("font-size:20px;");
	m_pCbUser->setEditable(true);
	m_pCbUser->setMaxVisibleItems(5);
	m_pCbUser->setGeometry(120,20,120,26);


	m_pLePwd = new QLineEdit(this);
	ft.setPointSize(15);
	m_pLePwd->setFont(ft);
	m_pLePwd->setGeometry(120,70,120,26);
	m_pLePwd->setEchoMode(QLineEdit::Password);

	m_pPstBtnOK  = new QPushButton(this);   
	ft.setPointSize(15);
	m_pPstBtnOK->setFont(ft);	
	m_pPstBtnOK->setText(tr("确定"));
	m_pPstBtnOK->setGeometry(60,130,60,26);

	m_pPstBtnCancel  = new QPushButton(this);
	ft.setPointSize(15);
	m_pPstBtnCancel->setFont(ft);
	m_pPstBtnCancel->setText(tr("取消"));
	m_pPstBtnCancel->setGeometry(180,130,60,26);

	// 初始化下拉列表
#if HAVE_DATA_BASE
	QSqlQuery initQuery(*m_pDDatabaseConnection);
	try
	{
		initQuery.exec("select User_Name from UserInfo");//chenyuzhu modify on 2014-7-16
	}
	catch (QString exception)
	{
		QMessageBox::critical(0,tr("initQuery.exec"),m_pDDatabaseConnection->lastError().text());
	}
	while(initQuery.next())
	{
		QString temp = initQuery.value(0).toString();
		m_pCbUser->addItem(temp);
	} 
#else 
	QMessageBox::information(this, "CInspectUser::InitDialog", tr("未连接数据库"));
#endif
}
void CInspectUser::DestroyLogOnDialog(void)
{
	this->close();
}

//判断用户名密码是否正确
void CInspectUser::CheckOnInfor(void)
{
#if HAVE_DATA_BASE
	QString strCUName = m_pCbUser->currentText();
	QString strCPWD   = m_pLePwd->text();
	QSqlQuery CoQuery(*m_pDDatabaseConnection);
	QString strSql = QString("%1%2%3%4").arg("select User_Name, User_Pwd, User_Group from UserInfo where User_Name = ").arg("'").arg(strCUName).arg("'");//chenyuzhu modify on 2014-7-16
	CoQuery.exec(strSql);
	while (CoQuery.next())
	{  	 
		if(!QString::compare(CoQuery.value(0).toString(), strCUName))
		{
			//chenyuzhu add start on 2014-7-16
			QString pwdmd5;
			QByteArray bb;
            bb = QCryptographicHash::hash (strCPWD.toLocal8Bit(), QCryptographicHash::Md5 );
			pwdmd5.append(bb.toHex());  
			pwdmd5 = pwdmd5.toLower();
			QString pwdmd = CoQuery.value(1).toString();
			pwdmd = pwdmd.toLower();
			//chenyuzhu add end on 2014-7-16
			if (!QString::compare(pwdmd,pwdmd5))//chenyuzhu modity on 2014-7-16
			{
				//登陆成功直接退出模式对话框
				this->close();
				m_isOK = true;
				return;
			}else
			{
				QMessageBox::information(NULL, "Error", tr("密码错误!!!"), 
					QMessageBox::Yes, QMessageBox::Yes);
				m_isOK = false;
				return;
			}                   
		}	 
	}
	QMessageBox::information(NULL, "Error", tr("用户名不存在!!!"), QMessageBox::Yes, QMessageBox::Yes);
	m_isOK = false;
#else
	this->close();
	m_isOK = true; 
	QMessageBox::information(this, "CInspectUser::CheckOnInfor", tr("未连接数据库"));
	return;
#endif
}
