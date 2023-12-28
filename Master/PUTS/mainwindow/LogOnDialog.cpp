/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 登录主界面
* Filename:LogOnDialog.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-10
*/

#include <QString>
#include <QFont>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QCryptographicHash>
#include <QTextCodec>
#include "DataDefine.h"

#include "LogOnDialog.h"

extern QString g_strUserPer;

//QString g_LogonName;

void CLogOnDialog::InitMyDialog(void)
{
	this->setWindowTitle(tr("登陆界面"));
	this->setAutoFillBackground(true);
	this->setWindowFlags(Qt::FramelessWindowHint); //去掉标题栏
	this->setWindowState(Qt::WindowMaximized);

    // 可执行程序必须要和图片在同一个目录下才能显示背景图片
	this ->setStyleSheet(QObject::tr("QDialog{border-image: url(:/Resources/logon.bmp);}"));
	m_pMainGridLayout = new QGridLayout;
	m_pShowGridLayout = new QGridLayout;


	m_plblUser = new QLabel(this);
	QFont ft;
	ft.setPointSize(15);
    m_plblUser->setFont(ft);
    m_plblUser ->setText(tr("用户名"));
    m_plblUser ->setAlignment(Qt::AlignCenter);
	m_plblUser->setMinimumWidth(150);

	m_pShowGridLayout->addWidget(m_plblUser,0,0,1,1);

	m_plblPwd = new QLabel(this);
	ft.setPointSize(15);
	m_plblPwd->setFont(ft);
	m_plblPwd ->setText(tr("密码"));
	m_plblPwd ->setAlignment(Qt::AlignCenter);
	m_plblPwd->setMinimumWidth(150);
	m_pShowGridLayout->addWidget(m_plblPwd,1,0,1,1);

	m_pcbUser = new QComboBox(this);
	m_pcbUser->setStyleSheet("font-size:20px;");
	m_pcbUser ->setEditable(true);
	m_pcbUser ->setMaxVisibleItems(5);
	m_pcbUser->setMaximumWidth(150);
	m_pcbUser->setMinimumWidth(150);
	m_pShowGridLayout->addWidget(m_pcbUser,0,1,1,1);

	m_plePwd = new QLineEdit(this);
	ft.setPointSize(15);
	m_plePwd->setFont(ft);
	m_plePwd->setMaximumWidth(150);
	m_plePwd->setEchoMode(QLineEdit::Password);
	m_plePwd->setMinimumWidth(150);
	m_pShowGridLayout->addWidget(m_plePwd,1,1,1,1);

	m_pbgOk  = new QPushButton(this);   
	ft.setPointSize(15);
	m_pbgOk->setFont(ft);	
	m_pbgOk -> setText(tr("确定"));
	m_pbgOk->setMinimumWidth(130);
	m_pShowGridLayout->addWidget(m_pbgOk,0,2,1,1);

	m_pbgCancel  = new QPushButton(this);
	ft.setPointSize(15);
	m_pbgCancel->setFont(ft);
	m_pbgCancel -> setText(tr("取消"));
	m_pbgCancel->setMinimumWidth(130);
	m_pShowGridLayout->addWidget(m_pbgCancel,1,2,1,1);

	m_pShowGridLayout->setContentsMargins(1,1,1,1);
	m_pShowGridLayout->setSpacing(1);

	QRect rect = this->rect();
	QSpacerItem *pLeftSpacer;
	pLeftSpacer = new QSpacerItem(rect.width()*2/3, rect.height()*1/7, QSizePolicy::Expanding, QSizePolicy::Maximum);

	QSpacerItem *pRightSpacer;
	pRightSpacer = new QSpacerItem(rect.width()*2/3, rect.height()*1/7, QSizePolicy::Expanding, QSizePolicy::Minimum);

	QSpacerItem *pTopSpacer;
	pTopSpacer = new QSpacerItem(rect.width()*1/3, rect.height()*6/7, QSizePolicy::Expanding, QSizePolicy::Minimum);

	QSpacerItem *pBottonSpacer;
	pBottonSpacer = new QSpacerItem(rect.width()*1/3, rect.height()*1/7, QSizePolicy::Expanding, QSizePolicy::Minimum);
    
	m_pMainGridLayout->addItem(pTopSpacer,0,1,1,1);
	m_pMainGridLayout->addItem(pBottonSpacer,2,1,1,1);
	m_pMainGridLayout->addItem(pLeftSpacer,1,0,1,1);
	m_pMainGridLayout->addItem(pLeftSpacer,1,2,1,1);
	m_pMainGridLayout->addLayout(m_pShowGridLayout,1,1,1,1);

	this->setLayout(m_pMainGridLayout);


    // 初始化下拉列表
#if HAVE_DATA_BASE
	QSqlQuery initQuery(*m_pDDatabaseConnection);
	try
	{
		initQuery.exec("select User_Name from UserInfo");//chenyuzhu modify on 2014-7-11
	}
	catch (QString exception)
	{
		QMessageBox::critical(0,tr("initQuery.exec"),m_pDDatabaseConnection->lastError().text());
	}
	while(initQuery.next())
	{
		QString temp = initQuery.value(0).toString();
		m_pcbUser->addItem(temp);
	} 
#endif

	//dong  qi add start on 2013.11.29
	//设置为上一次用户登陆的用户名
	QString strPath = QDir::currentPath() + "/PUTS.ini";
	QSettings setting(strPath,QSettings::IniFormat);
	QString strOldName = setting.value("UserSet/UserName").toString();
	
	int index = m_pcbUser->findText(strOldName);
	if (-1 != index )
	{
		m_pcbUser->setCurrentIndex(index);
	}
	else
	{
		m_pcbUser->setCurrentIndex(1);
	}	
	//dong  qi add end on 2013.11.29

}

//构造函数
CLogOnDialog::CLogOnDialog(QSqlDatabase *pDatabaseConnection):m_pDDatabaseConnection(pDatabaseConnection)
{
  InitMyDialog();
 //连接信号和槽
  connect(m_pbgCancel,SIGNAL(clicked ()),this,SLOT(DestroyLogOnDialog()));
  connect(m_pbgOk,SIGNAL(clicked()),this,SLOT(CheckOnInfor()));  
//  connect(m_pbgOk,SIGNAL(),this,SLOT(CheckOnInfor())); 
}

CLogOnDialog::~CLogOnDialog(void)
{

}

void CLogOnDialog::DestroyLogOnDialog(void)
{
	 exit(0);
}

//判断用户名密码是否正确

void CLogOnDialog::CheckOnInfor(void)
{
 #if HAVE_DATA_BASE
	 QString strCUName = m_pcbUser->currentText();
	 QString strCPWD   = m_plePwd->text();
	 QSqlQuery CoQuery(*m_pDDatabaseConnection);
	 QString strSql = QString("%1%2%3%4").arg("select User_Name, User_Pwd, User_Group from UserInfo where User_Name = ").arg("'").arg(strCUName).arg("'");//chenyuzhu modity on 2014-7-11
	 CoQuery.exec(strSql);
	 while (CoQuery.next())
	 {  	 
		 if(!QString::compare(CoQuery.value(0).toString(), strCUName))
		 {
			 //chenyuzhu add start on 2014-7-11
			 QString pwdmd5;
			 QByteArray bb;
             bb = QCryptographicHash::hash (strCPWD.toLocal8Bit(), QCryptographicHash::Md5 );
			 pwdmd5.append(bb.toHex());  
			 pwdmd5 = pwdmd5.toLower();
			 QString pwdmd = CoQuery.value(1).toString();
			 pwdmd = pwdmd.toLower();
			 //chenyuzhu add end on 2014-7-11
			 if (!QString::compare(pwdmd,pwdmd5))//chenyuzhu modity on 2014-7-11
             {
				 //登陆成功直接退出模式对话框
				 g_strUserPer = CoQuery.value(2).toString();
				 this -> close();
				 //dong  qi add start on 2013.11.29
				 //将当前用户写到配置文件中
				 QString strPath = QDir::currentPath() + "/PUTS.ini";
				 QSettings setting(strPath,QSettings::IniFormat);
				 setting.setValue("UserSet/UserName",strCUName);
			//	 g_LogonName = strCUName; //保存当前登录人的账号

				 //dong  qi add end on 2013.11.29
				 return;
             }else
			 {
                 QMessageBox::information(NULL, "InitDatabase", tr("密码错误"), 
					                      QMessageBox::Yes, QMessageBox::Yes);
				 return;
			 }                   
		 }	 
	 }
	 QMessageBox::information(NULL, "InitDatabase", tr("用户名不存在"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes); 
#else
	 this -> close(); 
	 g_strUserPer = QString("管理员");
	 return;
#endif
}

 void CLogOnDialog::keyPressEvent(QKeyEvent *e)    //过滤ESC
 {
	 if (e->key() == Qt::Key_Home)
	 {
		 return;
	 }
	 if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) //chenyuzhu add start on 2014-10-30
	 { 
		 CheckOnInfor();//按钮槽函数
	 }
	 else
	 {
		 QWidget::keyPressEvent(e);
	 }

 }

