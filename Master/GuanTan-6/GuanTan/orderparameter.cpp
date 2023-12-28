#include "orderparameter.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

OrderParameter::OrderParameter(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

OrderParameter::~OrderParameter()
{

}

void OrderParameter::UiInit()
{
	QRegExp RegN0_60000("^0|60000|[1-5][0-9]{0,4}|[6-9][0-9]{0,3}$");	//订单参数中 0到60000 的正则表达式 王威-2013-10-23
	QRegExp RegN0_100("^(100[.]0|[1-9][0-9]{0,1}[.][0-9]{0,1}|0[.][0-9]{0,1})$");	//订单参数中 0.0到100.0 的正则表达式 王威-2013-10-23
	QRegExp RegOrder("^[0-9A-Za-z]{0,15}$");	//订单参数 订单的正则表达式	王威-2013-10-23
	ui.lineEditOrderNumber->setValidator(new QRegExpValidator(RegOrder,ui.lineEditOrderNumber));
	ui.lineEditSteelNumber->setValidator(new QRegExpValidator(RegN0_60000,ui.lineEditSteelNumber));
	ui.comboBoxSteelDia->setValidator(new QRegExpValidator(RegN0_60000,ui.comboBoxSteelDia));
	ui.comboBoxSteelLength->setValidator(new QRegExpValidator(RegN0_60000,ui.comboBoxSteelLength));
	ui.comboBoxSteelPly->setValidator(new QRegExpValidator(RegN0_100,ui.comboBoxSteelPly));
	ui.comboBoxSteelRank->setValidator(new QRegExpValidator(RegOrder,ui.comboBoxSteelRank));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("订单参数设置"));
}
void OrderParameter::OrderParameterYes()
{
	close();

}