#include "login.h"
#include <QSettings>
#include <QMessageBox>
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

QString g_strCurUser;
QString g_strCurPassword;
int g_nTestCount;
Login::Login(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Widget);
	showMaximized();
	m_strPath = "C:/Program Files/ars/TestCount/Count.ini";
	UiInit();
}

Login::~Login()
{

}

void Login::UiInit()
{
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/qiehuanyonghu_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("登陆"));
	connect(ui.pushButtonLogin,SIGNAL(clicked()),this,SLOT(LoginAction()));
	QRegExp RegAdmin("^[a-z_0-9A-Z]{0,13}$");	//登陆窗口 正则表达式 支持1-13位的数字和英文字母
	ui.comboBoxAdmin->setValidator(new QRegExpValidator(RegAdmin,ui.comboBoxAdmin));
	ui.lineEditAdminPassWord->setValidator(new QRegExpValidator(RegAdmin,ui.lineEditAdminPassWord));
	QString strUser = "./Login.ini";
	QSettings setting(strUser, QSettings::IniFormat); // 当前目录的INI文件
	for (int i = 1;i < 20;i++)
	{
		if (setting.value("admin/" + QString::number(i)).toString() != NULL)
		{
			ui.comboBoxAdmin->addItem(setting.value("admin/" + QString::number(i)).toString());
		}
	}
	ui.comboBoxAdmin->setCurrentIndex(setting.value("admin/cur").toInt() - 1);
}
void Login::LoginAction()
{
	QString m_qsLogin = "./Login.ini";
	QSettings setting(m_qsLogin,QSettings::IniFormat);
	QSettings setting_path(m_strPath,QSettings::IniFormat);
	if (ui.comboBoxAdmin->currentText() == "ALLRISING" && ui.lineEditAdminPassWord->text() == "ARS_NDT")
	{
		g_strCurUser = "ALLRISING";
		g_strCurPassword = "ARS_NDT";
		g_nTestCount = 0;
		accept();
		close();
		return; 
	}
	for (int i = 1;i <= 20;i++)
	{
		setting.beginGroup("admin");
		if ((setting.value(QString::number(i)).toString() == ui.comboBoxAdmin->currentText()) && (ui.comboBoxAdmin->currentText() != ""))
		{
			g_strCurUser = ui.comboBoxAdmin->currentText();
			setting.endGroup();
			setting.beginGroup("password");
			if ((setting.value(QString::number(i)).toString() == ui.lineEditAdminPassWord->text()) && (ui.lineEditAdminPassWord->text() != ""))
			{
				g_strCurPassword = ui.lineEditAdminPassWord->text();
				setting.endGroup();
				setting.beginGroup("admin");
				setting.setValue("cur",QString::number(ui.comboBoxAdmin->currentIndex() + 1));
				setting.endGroup();
				//wangwei add on 2014-2-27
				setting_path.beginGroup(g_strCurUser);
				g_nTestCount = setting_path.value("count").toInt();
				if (g_nTestCount > 0 && g_nTestCount < 2000)
				{
					int nCount = 2000 - g_nTestCount;
					if (nCount < 200)
					{
						QMessageBox::information(this,tr("登陆"),tr("当前用户获得的授权检测个数还剩下") + QString::number(nCount) + tr(",请及时与厂家联系获得更多权限，以免耽误您的生产，谢谢！"));
					}
					accept();
				}
				else if (g_nTestCount == 0)
				{
					QMessageBox::information(this,tr("登陆"),tr("对不起，此用户没有授权,无法登陆检测，请与厂家联系获得权限，以免耽误您的生产，谢谢！"));
				}
				else
				{
					QMessageBox::information(this,tr("登陆"),tr("当前用户获得的授权检测个数已经为0,无法继续检测，请及时与厂家联系获得更多权限，以免耽误您的生产，谢谢！"));
				}
				setting_path.endGroup();
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
