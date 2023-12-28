#include "judgeparameter.h"

JudgeParameter::JudgeParameter(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	m_bSoundOpen = false;
	m_bLightOpen = false;
	connect(ui.pushButtonSoundOpen,SIGNAL(clicked()),this,SLOT(SoundOpen()));
	connect(ui.pushButtonSoundClose,SIGNAL(clicked()),this,SLOT(SoundClose()));
	connect(ui.pushButtonLightOpen,SIGNAL(clicked()),this,SLOT(LightOpen()));
	connect(ui.pushButtonLightClose,SIGNAL(clicked()),this,SLOT(LightClose()));
}

JudgeParameter::~JudgeParameter()
{

}
void JudgeParameter::SoundOpen()
{
	ui.pushButtonSoundOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	ui.pushButtonSoundClose->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	m_bSoundOpen = true;
}

void JudgeParameter::SoundClose()
{
	ui.pushButtonSoundOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	ui.pushButtonSoundClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	m_bSoundOpen = false;
}

void JudgeParameter::LightOpen()
{
	ui.pushButtonLightOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	ui.pushButtonLightClose->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	m_bLightOpen = true;
}

void JudgeParameter::LightClose()
{
	ui.pushButtonLightOpen->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 127);"));
	ui.pushButtonLightClose->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));
	m_bLightOpen = false;
}
