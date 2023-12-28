#include "usermanagement.h"
#include <QSettings>
#include <QMessageBox>
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

extern QString g_strCurUser;
extern QString g_strCurPassword;
UserManagement::UserManagement(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

UserManagement::~UserManagement()
{

}

void UserManagement::UiInit()
{
	m_strUser = "./Login.ini";
	connect(ui.pushButtonTest,SIGNAL(clicked()),this,SLOT(Verify()));
	connect(ui.pushButtonAdd,SIGNAL(clicked()),this,SLOT(Add()));
	connect(ui.pushButtonCut,SIGNAL(clicked()),this,SLOT(Cut()));
	QRegExp RegUserOrPassword("^[0-9A-Za-z_]{0,15}$");	//�û�������������,����,��ĸ,_
	ui.comboBoxUser->setValidator(new QRegExpValidator(RegUserOrPassword,ui.comboBoxUser));
	ui.lineEditAdmin->setValidator(new QRegExpValidator(RegUserOrPassword,ui.lineEditAdmin));
	ui.lineEditNewUser->setValidator(new QRegExpValidator(RegUserOrPassword,ui.lineEditNewUser));
	ui.lineEditNewUserPassword->setValidator(new QRegExpValidator(RegUserOrPassword,ui.lineEditNewUserPassword));
	ui.lineEditNewUserPasswordAgain->setValidator(new QRegExpValidator(RegUserOrPassword,ui.lineEditNewUserPasswordAgain));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/yonghuguanli_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("�û�����"));
}

void UserManagement::Lock()
{
	ui.lineEditAdmin->clear();
	ui.lineEditNewUser->clear();
	ui.lineEditNewUserPassword->clear();
	ui.lineEditNewUserPasswordAgain->clear();
	ui.comboBoxUser->setEnabled(false);
	ui.lineEditNewUser->setEnabled(false);
	ui.lineEditNewUserPassword->setEnabled(false);
	ui.lineEditNewUserPasswordAgain->setEnabled(false);
	ui.pushButtonAdd->setEnabled(false);
	ui.pushButtonCut->setEnabled(false);

	ui.comboBoxUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(110,110,110);color: rgb(110, 110, 110);font: 12pt;"));
	ui.lineEditNewUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(110,110,110);color: rgb(110, 110, 110);font: 12pt;"));
	ui.lineEditNewUserPassword->setStyleSheet(QString::fromUtf8("border:1px solid rgb(110,110,110);color: rgb(110, 110, 110);font: 12pt;"));
	ui.lineEditNewUserPasswordAgain->setStyleSheet(QString::fromUtf8("border:1px solid rgb(110,110,110);color: rgb(110, 110, 110);font: 12pt;"));
	ui.pushButtonAdd->setStyleSheet(QString::fromUtf8("color: rgb(110, 110, 110);font: 12pt;"));
	ui.pushButtonCut->setStyleSheet(QString::fromUtf8("color: rgb(110, 110, 110);font: 12pt;"));
}

void UserManagement::Open()
{
	ui.lineEditAdmin->clear();
	ui.lineEditNewUser->clear();
	ui.lineEditNewUserPassword->clear();
	ui.lineEditNewUserPasswordAgain->clear();
	ui.comboBoxUser->setEnabled(true);
	ui.lineEditNewUser->setEnabled(true);
	ui.lineEditNewUserPassword->setEnabled(true);
	ui.lineEditNewUserPasswordAgain->setEnabled(true);
	ui.pushButtonAdd->setEnabled(true);
	ui.pushButtonCut->setEnabled(true);

	ui.comboBoxUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt;"));
	ui.lineEditNewUser->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt;"));
	ui.lineEditNewUserPassword->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt;"));
	ui.lineEditNewUserPasswordAgain->setStyleSheet(QString::fromUtf8("border:1px solid rgb(255,255,255);color: rgb(255, 255, 255);font: 12pt;"));
	ui.pushButtonAdd->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);font: 12pt;"));
	ui.pushButtonCut->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);font: 12pt;"));
}

void UserManagement::Verify()//��֤
{
	if (ui.lineEditAdmin->text() == "ARS_NDT")
	{
		Open();
	}
	else
	{
		Lock();
	}
}

void UserManagement::Add()
{
	QSettings setting(m_strUser,QSettings::IniFormat);//�������ļ�
	QString strUser = ui.lineEditNewUser->text();
	QString strPassword = ui.lineEditNewUserPassword->text();
	QString strPasswordAgain = ui.lineEditNewUserPasswordAgain->text();
	int nNum = ui.comboBoxUser->count();//�û�����
	int nAdd = 0;
	for(int i=0;i<nNum;i++)
	{
		ui.comboBoxUser->setCurrentIndex(i);
		if (ui.comboBoxUser->currentText() != strUser)
		{
			nAdd +=1; 
		}
	}
	if (nAdd == nNum)
	{
		if (strPassword  == NULL || strPasswordAgain == NULL)
		{
			QMessageBox::warning(this,tr("����û�"),tr("���벻��Ϊ��!"));
		}
		else if (strPassword == strPasswordAgain)
		{
			ui.comboBoxUser->addItem(strUser);
			setting.beginGroup("admin");
			setting.setValue((QString::number(nAdd+1)),QString(strUser));
			setting.endGroup();
			setting.beginGroup("password");
			setting.setValue((QString::number(nAdd+1)),QString(strPassword));
			setting.endGroup();
			QMessageBox::information(this,tr("����û�"),tr("��ϲ��!���û���ӳɹ�!"));
			ui.lineEditNewUser->clear();
			ui.lineEditNewUserPassword->clear();
			ui.lineEditNewUserPasswordAgain->clear();
			ui.comboBoxUser->setCurrentIndex(0);
		}
		else
		{
			QMessageBox::warning(this,tr("����û�"),tr("�Բ���,��������������벻��ͬ,��������������!"));
			ui.lineEditNewUserPassword->clear();
			ui.lineEditNewUserPasswordAgain->clear();
		}
	}
	else
	{
		QMessageBox::warning(this,tr("����û�"),tr("�Բ���,���û����Ѿ�����,���������������û�����!"));
		ui.lineEditNewUser->clear();
	}
}

void UserManagement::Cut()
{
	QSettings setting(m_strUser,QSettings::IniFormat);//�������ļ�
	QString strUser = ui.comboBoxUser->currentText();
	int nCurNum = ui.comboBoxUser->currentIndex();
	int nNum = ui.comboBoxUser->count();//�û�����
	//�ж��Ƿ�����
	if (strUser == g_strCurUser)
	{
		QMessageBox::warning(this,tr("ɾ���û�"),tr("�Բ���,����ɾ����ǰ��¼�û�!"));
		return;
	}
	//ɾ���ؼ���ǰ�û�
	ui.comboBoxUser->removeItem(nCurNum);
	//ɾ���ļ���ǰ�û�
	setting.beginGroup("admin");
	for (int i = 0;i < nNum;i++)
	{
		setting.setValue(QString::number(i + 1),"");
	}
	for (int i = 0;i < nNum - 1;i++)
	{
		ui.comboBoxUser->setCurrentIndex(i);
		setting.setValue(QString::number(i + 1),ui.comboBoxUser->currentText());
	}
	setting.endGroup();
	//���õ�ǰ�ؼ�ɸѡ���ո�ɾ�����û�������
	setting.beginGroup("password");
	for (int i = 0;i < nNum - 1;i++)
	{
		ui.comboBoxUser->removeItem(0);
	}
	for (int i = 0;i < nNum;i++)
	{
		ui.comboBoxUser->addItem(setting.value(QString::number(i + 1)).toString());
	}
	ui.comboBoxUser->removeItem(nCurNum);
	for (int i = 0;i < nNum;i++)
	{
		setting.setValue(QString::number(i + 1),"");
	}
	for (int i = 0;i < nNum - 1;i++)
	{
		ui.comboBoxUser->setCurrentIndex(i);
		setting.setValue(QString::number(i + 1),ui.comboBoxUser->currentText());
	}
	setting.endGroup();
	//��ԭ��ǰӵ�е��û�
	setting.beginGroup("admin");
	for (int i = 0;i < nNum - 1;i++)
	{
		ui.comboBoxUser->removeItem(0);
	}
	for (int i = 0;i < nNum - 1;i++)
	{
		ui.comboBoxUser->addItem(setting.value(QString::number(i + 1)).toString());
	}
	setting.endGroup();
	ui.comboBoxUser->setCurrentIndex(0);
	QMessageBox::information(this,tr("ɾ���û�"),tr("�û�ɾ���ɹ�!"));
}