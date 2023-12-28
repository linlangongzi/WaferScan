#include "judgeparameter.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

JudgeParameter::JudgeParameter(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	m_bSoundOpen = 0;
	m_bLightOpen = 1;
	connect(ui.pushButtonSoundOpen,SIGNAL(clicked()),this,SLOT(SoundOpen()));
	connect(ui.pushButtonSoundClose,SIGNAL(clicked()),this,SLOT(SoundClose()));
	connect(ui.pushButtonLightOpen,SIGNAL(clicked()),this,SLOT(LightOpen()));
	connect(ui.pushButtonLightClose,SIGNAL(clicked()),this,SLOT(LightClose()));
	UiInit();
}

JudgeParameter::~JudgeParameter()
{

}
void JudgeParameter::UiInit()
{
	QRegExp RegN0_60000("^0|60000|[1-5][0-9]{0,4}|[6-9][0-9]{0,3}$");	//评判参数中 0到60000 的正则表达式
	ui.lineEditYesCouplingNumber->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditYesCouplingLength->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditYesFaultNumber->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditYesFaultLength->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditNoCouplingNumber->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditNoCouplingLength->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditNoFaultNumber->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditNoFaultLength->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditCouplingNumber->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditCouplingLength->setValidator(new QRegExpValidator(RegN0_60000,this));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("评判参数设置"));
}
void JudgeParameter::SoundOpen()
{
	ui.pushButtonSoundOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	ui.pushButtonSoundClose->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	m_bSoundOpen = 1;
}

void JudgeParameter::SoundClose()
{
	ui.pushButtonSoundOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	ui.pushButtonSoundClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	m_bSoundOpen = 0;
}

void JudgeParameter::LightOpen()
{
	ui.pushButtonLightOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	ui.pushButtonLightClose->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	m_bLightOpen = 1;
}

void JudgeParameter::LightClose()
{
	ui.pushButtonLightOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	ui.pushButtonLightClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	m_bLightOpen = 0;
}
