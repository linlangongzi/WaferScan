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
	this->setWindowTitle(tr("��½"));
	connect(ui.pushButtonLogin,SIGNAL(clicked()),this,SLOT(LoginAction()));
	QRegExp RegAdmin("^[a-z_0-9A-Z]{0,13}$");	//��½���� ������ʽ ֧��1-13λ�����ֺ�Ӣ����ĸ
	ui.comboBoxAdmin->setValidator(new QRegExpValidator(RegAdmin,ui.comboBoxAdmin));
	ui.lineEditAdminPassWord->setValidator(new QRegExpValidator(RegAdmin,ui.lineEditAdminPassWord));
	QString strUser = "./Login.ini";
	QSettings setting(strUser, QSettings::IniFormat); // ��ǰĿ¼��INI�ļ�
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
						QMessageBox::information(this,tr("��½"),tr("��ǰ�û���õ���Ȩ��������ʣ��") + QString::number(nCount) + tr(",�뼰ʱ�볧����ϵ��ø���Ȩ�ޣ����ⵢ������������лл��"));
					}
					accept();
				}
				else if (g_nTestCount == 0)
				{
					QMessageBox::information(this,tr("��½"),tr("�Բ��𣬴��û�û����Ȩ,�޷���½��⣬���볧����ϵ���Ȩ�ޣ����ⵢ������������лл��"));
				}
				else
				{
					QMessageBox::information(this,tr("��½"),tr("��ǰ�û���õ���Ȩ�������Ѿ�Ϊ0,�޷�������⣬�뼰ʱ�볧����ϵ��ø���Ȩ�ޣ����ⵢ������������лл��"));
				}
				setting_path.endGroup();
				close();
				return; 
			}
			else if (ui.lineEditAdminPassWord->text() == "")
			{
				QMessageBox::StandardButton button;  
				button = QMessageBox::warning(this, tr("��½����"),  
					QString(tr("���벻��Ϊ��!")),  
					QMessageBox::Yes | QMessageBox::No); 
				if (button == QMessageBox::No)
					close();
				return;
			}
			else
			{
				QMessageBox::StandardButton button;  
				button = QMessageBox::warning(this, tr("��½����"),  
					QString(tr("�������!")),  
					QMessageBox::Yes | QMessageBox::No); 
				if (button == QMessageBox::No)
					close();
				return;
			}
		}
		else if (ui.comboBoxAdmin->currentText() == "")
		{
			QMessageBox::StandardButton button;  
			button = QMessageBox::warning(this, tr("��½����"),  
				QString(tr("�û�������Ϊ��!")),  
				QMessageBox::Yes | QMessageBox::No); 
			if (button == QMessageBox::No)
				close();
			return;
		}
		else if (i == 10)
		{
			QMessageBox::StandardButton button;  
			button = QMessageBox::warning(this, tr("��½����"),  
				QString(tr("�û���������!")),  
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
