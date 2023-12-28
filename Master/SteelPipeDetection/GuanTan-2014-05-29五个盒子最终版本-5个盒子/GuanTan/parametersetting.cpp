#include "parametersetting.h"
#include <QFileDialog>
#include <QSettings>

ParameterSetting::ParameterSetting(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	m_bOrder = false;
	m_bJudge = false;
	m_bUltrasound = false;
	m_bShow = false;
	ui.label_All->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	ui.label_Judge->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	ui.label_Order->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	ui.label_Show->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	ui.label_Ultrasound->setStyleSheet(QString::fromUtf8("background-color: rgb(255,0,0);"));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("²ÎÊıÉèÖÃ"));
}

 ParameterSetting::~ParameterSetting()
 {
 
 }
 
