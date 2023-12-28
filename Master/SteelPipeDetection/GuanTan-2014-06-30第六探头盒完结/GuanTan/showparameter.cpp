#include "showparameter.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

ShowParameter::ShowParameter(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

ShowParameter::~ShowParameter()
{

}
void ShowParameter::UiInit()
{
 	QRegExp RegN0_60000("^0|60000|[1-5][0-9]{0,4}|[6-9][0-9]{0,3}$");	//��ʾ������ 0��60000 ��������ʽ
// 	QRegExp RegN_20_0("^0|-20|-1[0-9]{0,1}|-[2-9]$");	//��ʾ������ -20��0 ��������ʽ
// 	QRegExp RegN0_20("^0|20|1[0-9]{0,1}|[2-9]$");	//��ʾ������ 0��20 ��������ʽ
	QRegExp RegN_20_0("^(-0[.][0-9]|-20[.]0|-1[0-9]{0,1}[.][0-9]|-[2-9][.][0-9])$");	//��ʾ������ -20.0��0.0 ��������ʽ
	QRegExp RegN0_20("^(0[.][0-9]|20[.]0|1[0-9]{0,1}[.][0-9]|[2-9][.][0-9])$");	//��ʾ������ 0.0��20.0 ��������ʽ
	ui.lineEditLength->setValidator(new QRegExpValidator(RegN0_60000,ui.lineEditLength));
	ui.lineEditToleranceMin->setValidator(new QRegExpValidator(RegN_20_0,ui.lineEditToleranceMin));
	ui.lineEditToleranceMax->setValidator(new QRegExpValidator(RegN0_20,ui.lineEditToleranceMax));
	ui.lineEditLimitsMin->setValidator(new QRegExpValidator(RegN_20_0,ui.lineEditLimitsMin));
	ui.lineEditLimitsMax->setValidator(new QRegExpValidator(RegN0_20,ui.lineEditLimitsMax));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("��ʾ��������"));
}
