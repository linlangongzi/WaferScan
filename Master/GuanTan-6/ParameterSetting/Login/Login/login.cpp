#include "login.h"
#include <QSettings>
#include <QMessageBox>

Login::Login(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.pushButtonLogin,SIGNAL(clicked()),this,SLOT(LoginAction()));
	setWindowFlags(Qt::Widget);
	showMaximized();
}

Login::~Login()
{

}

void Login::LoginAction()
{
	m_qsLogin = QString(tr("./Login.ini"));
	QSettings setting(m_qsLogin,QSettings::IniFormat);
	for (int i = 1;i <= 20;i++)
	{
		setting.beginGroup("admin");
		if ((setting.value(QString::number(i)).toString() == ui.comboBoxAdmin->currentText()) && (ui.comboBoxAdmin->currentText() != ""))
		{
			setting.endGroup();
			setting.beginGroup("password");
			if ((setting.value(QString::number(i)).toString() == ui.lineEditAdminPassWord->text()) && (ui.lineEditAdminPassWord->text() != ""))
			{
				close();
				return; 
			}
			else if (ui.lineEditAdminPassWord->text() == "")
			{
				QMessageBox::StandardButton button;  
				button = QMessageBox::warning(this, tr("登陆程序"),  
					QString(tr("密码不能为空!")),  
					QMessageBox::Yes | QMessageBox::No); 
				if (button == QMessageBox::No)
					close();
				return;
			}
			else
			{
				QMessageBox::StandardButton button;  
				button = QMessageBox::warning(this, tr("登陆程序"),  
					QString(tr("密码错误!")),  
					QMessageBox::Yes | QMessageBox::No); 
				if (button == QMessageBox::No)
					close();
				return;
			}
		}
		else if (ui.comboBoxAdmin->currentText() == "")
		{
			QMessageBox::StandardButton button;  
			button = QMessageBox::warning(this, tr("登陆程序"),  
				QString(tr("用户名不能为空!")),  
				QMessageBox::Yes | QMessageBox::No); 
			if (button == QMessageBox::No)
				close();
			return;
		}
		else if (i == 10)
		{
			QMessageBox::StandardButton button;  
			button = QMessageBox::warning(this, tr("登陆程序"),  
				QString(tr("用户名不存在!")),  
				QMessageBox::Yes | QMessageBox::No);
			if (button == QMessageBox::No)
				close();
		}	
		setting.endGroup();
	}
}

void Login::QuitAction()
{
	close();
}
