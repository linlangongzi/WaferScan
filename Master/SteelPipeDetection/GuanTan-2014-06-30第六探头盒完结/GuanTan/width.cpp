#include "width.h"

Width::Width(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

Width::~Width()
{

}

void Width::UiInit()
{
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("抑制设置"));
	connect(ui.radioButtonAll,SIGNAL(clicked()),this,SLOT(SelectAll()));
	connect(ui.radioButtonAny,SIGNAL(clicked()),this,SLOT(SelectAny()));
	connect(ui.pushButtonAllIn,SIGNAL(clicked()),this,SLOT(AllIn()));
	connect(ui.pushButtonAnyIn,SIGNAL(clicked()),this,SLOT(AnyIn()));
	ui.pushButtonAllIn->setEnabled(false);
	ui.pushButtonAnyIn->setEnabled(false);
	ui.lineEditAny->setEnabled(false);
	ui.lineEditAll->setEnabled(false);

	QRegExp RegN0_100("^(100[.]00|[1-9][0-9]{0,1}[.][0-9]{0,2}|0[.][0-9]{0,2})$");	//超声参数-增益: 0.00到100.00 的正则表达式
	ui.lineEditAll->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAny->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe1->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe2->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe3->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe4->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe5->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe6->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe7->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe8->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe9->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe10->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe11->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe12->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe13->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe14->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe15->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe16->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe17->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe18->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe19->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe20->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe21->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe22->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe23->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe24->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe25->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe26->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe27->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe28->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe29->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe30->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe31->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe32->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe33->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe34->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe35->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditProbe36->setValidator(new QRegExpValidator(RegN0_100,this));
}

void Width::SelectAll()
{
	ui.checkBoxProbe1->setChecked(true);
	ui.checkBoxProbe2->setChecked(true);
	ui.checkBoxProbe3->setChecked(true);
	ui.checkBoxProbe4->setChecked(true);
	ui.checkBoxProbe5->setChecked(true);
	ui.checkBoxProbe6->setChecked(true);
	ui.checkBoxProbe7->setChecked(true);
	ui.checkBoxProbe8->setChecked(true);
	ui.checkBoxProbe9->setChecked(true);
	ui.checkBoxProbe10->setChecked(true);
	ui.checkBoxProbe11->setChecked(true);
	ui.checkBoxProbe12->setChecked(true);
	ui.checkBoxProbe13->setChecked(true);
	ui.checkBoxProbe14->setChecked(true);
	ui.checkBoxProbe15->setChecked(true);
	ui.checkBoxProbe16->setChecked(true);
	ui.checkBoxProbe17->setChecked(true);
	ui.checkBoxProbe18->setChecked(true);
	ui.checkBoxProbe19->setChecked(true);
	ui.checkBoxProbe20->setChecked(true);
	ui.checkBoxProbe21->setChecked(true);
	ui.checkBoxProbe22->setChecked(true);
	ui.checkBoxProbe23->setChecked(true);
	ui.checkBoxProbe24->setChecked(true);
	ui.checkBoxProbe25->setChecked(true);
	ui.checkBoxProbe26->setChecked(true);
	ui.checkBoxProbe27->setChecked(true);
	ui.checkBoxProbe28->setChecked(true);
	ui.checkBoxProbe29->setChecked(true);
	ui.checkBoxProbe30->setChecked(true);
	ui.checkBoxProbe31->setChecked(true);
	ui.checkBoxProbe32->setChecked(true);
	ui.checkBoxProbe33->setChecked(true);
	ui.checkBoxProbe34->setChecked(true);
	ui.checkBoxProbe35->setChecked(true);
	ui.checkBoxProbe36->setChecked(true);
	ui.pushButtonAllIn->setEnabled(true);
	ui.pushButtonAnyIn->setEnabled(false);
	ui.lineEditAll->setEnabled(true);
	ui.lineEditAny->setEnabled(false);
	ui.checkBoxProbe1->setEnabled(false);
	ui.checkBoxProbe2->setEnabled(false);
	ui.checkBoxProbe3->setEnabled(false);
	ui.checkBoxProbe4->setEnabled(false);
	ui.checkBoxProbe5->setEnabled(false);
	ui.checkBoxProbe6->setEnabled(false);
	ui.checkBoxProbe7->setEnabled(false);
	ui.checkBoxProbe8->setEnabled(false);
	ui.checkBoxProbe9->setEnabled(false);
	ui.checkBoxProbe10->setEnabled(false);
	ui.checkBoxProbe11->setEnabled(false);
	ui.checkBoxProbe12->setEnabled(false);
	ui.checkBoxProbe13->setEnabled(false);
	ui.checkBoxProbe14->setEnabled(false);
	ui.checkBoxProbe15->setEnabled(false);
	ui.checkBoxProbe16->setEnabled(false);
	ui.checkBoxProbe17->setEnabled(false);
	ui.checkBoxProbe18->setEnabled(false);
	ui.checkBoxProbe19->setEnabled(false);
	ui.checkBoxProbe20->setEnabled(false);
	ui.checkBoxProbe21->setEnabled(false);
	ui.checkBoxProbe22->setEnabled(false);
	ui.checkBoxProbe23->setEnabled(false);
	ui.checkBoxProbe24->setEnabled(false);
	ui.checkBoxProbe25->setEnabled(false);
	ui.checkBoxProbe26->setEnabled(false);
	ui.checkBoxProbe27->setEnabled(false);
	ui.checkBoxProbe28->setEnabled(false);
	ui.checkBoxProbe29->setEnabled(false);
	ui.checkBoxProbe30->setEnabled(false);
	ui.checkBoxProbe31->setEnabled(false);
	ui.checkBoxProbe32->setEnabled(false);
	ui.checkBoxProbe33->setEnabled(false);
	ui.checkBoxProbe34->setEnabled(false);
	ui.checkBoxProbe35->setEnabled(false);
	ui.checkBoxProbe36->setEnabled(false);
}

void Width::SelectAny()
{
	ui.checkBoxProbe1->setEnabled(true);
	ui.checkBoxProbe2->setEnabled(true);
	ui.checkBoxProbe3->setEnabled(true);
	ui.checkBoxProbe4->setEnabled(true);
	ui.checkBoxProbe5->setEnabled(true);
	ui.checkBoxProbe6->setEnabled(true);
	ui.checkBoxProbe7->setEnabled(true);
	ui.checkBoxProbe8->setEnabled(true);
	ui.checkBoxProbe9->setEnabled(true);
	ui.checkBoxProbe10->setEnabled(true);
	ui.checkBoxProbe11->setEnabled(true);
	ui.checkBoxProbe12->setEnabled(true);
	ui.checkBoxProbe13->setEnabled(true);
	ui.checkBoxProbe14->setEnabled(true);
	ui.checkBoxProbe15->setEnabled(true);
	ui.checkBoxProbe16->setEnabled(true);
	ui.checkBoxProbe17->setEnabled(true);
	ui.checkBoxProbe18->setEnabled(true);
	ui.checkBoxProbe19->setEnabled(true);
	ui.checkBoxProbe20->setEnabled(true);
	ui.checkBoxProbe21->setEnabled(true);
	ui.checkBoxProbe22->setEnabled(true);
	ui.checkBoxProbe23->setEnabled(true);
	ui.checkBoxProbe24->setEnabled(true);
	ui.checkBoxProbe25->setEnabled(true);
	ui.checkBoxProbe26->setEnabled(true);
	ui.checkBoxProbe27->setEnabled(true);
	ui.checkBoxProbe28->setEnabled(true);
	ui.checkBoxProbe29->setEnabled(true);
	ui.checkBoxProbe30->setEnabled(true);
	ui.checkBoxProbe31->setEnabled(true);
	ui.checkBoxProbe32->setEnabled(true);
	ui.checkBoxProbe33->setEnabled(true);
	ui.checkBoxProbe34->setEnabled(true);
	ui.checkBoxProbe35->setEnabled(true);
	ui.checkBoxProbe36->setEnabled(true);
	ui.checkBoxProbe1->setChecked(false);
	ui.checkBoxProbe2->setChecked(false);
	ui.checkBoxProbe3->setChecked(false);
	ui.checkBoxProbe4->setChecked(false);
	ui.checkBoxProbe5->setChecked(false);
	ui.checkBoxProbe6->setChecked(false);
	ui.checkBoxProbe7->setChecked(false);
	ui.checkBoxProbe8->setChecked(false);
	ui.checkBoxProbe9->setChecked(false);
	ui.checkBoxProbe10->setChecked(false);
	ui.checkBoxProbe11->setChecked(false);
	ui.checkBoxProbe12->setChecked(false);
	ui.checkBoxProbe13->setChecked(false);
	ui.checkBoxProbe14->setChecked(false);
	ui.checkBoxProbe15->setChecked(false);
	ui.checkBoxProbe16->setChecked(false);
	ui.checkBoxProbe17->setChecked(false);
	ui.checkBoxProbe18->setChecked(false);
	ui.checkBoxProbe19->setChecked(false);
	ui.checkBoxProbe20->setChecked(false);
	ui.checkBoxProbe21->setChecked(false);
	ui.checkBoxProbe22->setChecked(false);
	ui.checkBoxProbe23->setChecked(false);
	ui.checkBoxProbe24->setChecked(false);
	ui.checkBoxProbe25->setChecked(false);
	ui.checkBoxProbe26->setChecked(false);
	ui.checkBoxProbe27->setChecked(false);
	ui.checkBoxProbe28->setChecked(false);
	ui.checkBoxProbe29->setChecked(false);
	ui.checkBoxProbe30->setChecked(false);
	ui.checkBoxProbe31->setChecked(false);
	ui.checkBoxProbe32->setChecked(false);
	ui.checkBoxProbe33->setChecked(false);
	ui.checkBoxProbe34->setChecked(false);
	ui.checkBoxProbe35->setChecked(false);
	ui.checkBoxProbe36->setChecked(false);
	ui.pushButtonAllIn->setEnabled(false);
	ui.pushButtonAnyIn->setEnabled(true);
	ui.lineEditAll->setEnabled(false);
	ui.lineEditAny->setEnabled(true);
}

void Width::AllIn()
{
	QString strAll = ui.lineEditAll->text();
	if (strAll != NULL)
	{
		ui.lineEditProbe1->setText(QString(strAll));
		ui.lineEditProbe2->setText(QString(strAll));
		ui.lineEditProbe3->setText(QString(strAll));
		ui.lineEditProbe4->setText(QString(strAll));
		ui.lineEditProbe5->setText(QString(strAll));
		ui.lineEditProbe6->setText(QString(strAll));
		ui.lineEditProbe7->setText(QString(strAll));
		ui.lineEditProbe8->setText(QString(strAll));
		ui.lineEditProbe9->setText(QString(strAll));
		ui.lineEditProbe10->setText(QString(strAll));
		ui.lineEditProbe11->setText(QString(strAll));
		ui.lineEditProbe12->setText(QString(strAll));
		ui.lineEditProbe13->setText(QString(strAll));
		ui.lineEditProbe14->setText(QString(strAll));
		ui.lineEditProbe15->setText(QString(strAll));
		ui.lineEditProbe16->setText(QString(strAll));
		ui.lineEditProbe17->setText(QString(strAll));
		ui.lineEditProbe18->setText(QString(strAll));
		ui.lineEditProbe19->setText(QString(strAll));
		ui.lineEditProbe20->setText(QString(strAll));
		ui.lineEditProbe21->setText(QString(strAll));
		ui.lineEditProbe22->setText(QString(strAll));
		ui.lineEditProbe23->setText(QString(strAll));
		ui.lineEditProbe24->setText(QString(strAll));
		ui.lineEditProbe25->setText(QString(strAll));
		ui.lineEditProbe26->setText(QString(strAll));
		ui.lineEditProbe27->setText(QString(strAll));
		ui.lineEditProbe28->setText(QString(strAll));
		ui.lineEditProbe29->setText(QString(strAll));
		ui.lineEditProbe30->setText(QString(strAll));
		ui.lineEditProbe31->setText(QString(strAll));
		ui.lineEditProbe32->setText(QString(strAll));
		ui.lineEditProbe33->setText(QString(strAll));
		ui.lineEditProbe34->setText(QString(strAll));
		ui.lineEditProbe35->setText(QString(strAll));
		ui.lineEditProbe36->setText(QString(strAll));
	}
}
void Width::AnyIn()
{
	QString strAny = ui.lineEditAny->text();
	if (strAny != NULL)
	{
		if (ui.checkBoxProbe1->isChecked())
			ui.lineEditProbe1->setText(QString(strAny));
		if (ui.checkBoxProbe2->isChecked())
			ui.lineEditProbe2->setText(QString(strAny));
		if (ui.checkBoxProbe3->isChecked())
			ui.lineEditProbe3->setText(QString(strAny));
		if (ui.checkBoxProbe4->isChecked())
			ui.lineEditProbe4->setText(QString(strAny));
		if (ui.checkBoxProbe5->isChecked())
			ui.lineEditProbe5->setText(QString(strAny));
		if (ui.checkBoxProbe6->isChecked())
			ui.lineEditProbe6->setText(QString(strAny));
		if (ui.checkBoxProbe7->isChecked())
			ui.lineEditProbe7->setText(QString(strAny));
		if (ui.checkBoxProbe8->isChecked())
			ui.lineEditProbe8->setText(QString(strAny));
		if (ui.checkBoxProbe9->isChecked())
			ui.lineEditProbe9->setText(QString(strAny));
		if (ui.checkBoxProbe10->isChecked())
			ui.lineEditProbe10->setText(QString(strAny));
		if (ui.checkBoxProbe11->isChecked())
			ui.lineEditProbe11->setText(QString(strAny));
		if (ui.checkBoxProbe12->isChecked())
			ui.lineEditProbe12->setText(QString(strAny));
		if (ui.checkBoxProbe13->isChecked())
			ui.lineEditProbe13->setText(QString(strAny));
		if (ui.checkBoxProbe14->isChecked())
			ui.lineEditProbe14->setText(QString(strAny));
		if (ui.checkBoxProbe15->isChecked())
			ui.lineEditProbe15->setText(QString(strAny));
		if (ui.checkBoxProbe16->isChecked())
			ui.lineEditProbe16->setText(QString(strAny));
		if (ui.checkBoxProbe17->isChecked())
			ui.lineEditProbe17->setText(QString(strAny));
		if (ui.checkBoxProbe18->isChecked())
			ui.lineEditProbe18->setText(QString(strAny));
		if (ui.checkBoxProbe19->isChecked())
			ui.lineEditProbe19->setText(QString(strAny));
		if (ui.checkBoxProbe20->isChecked())
			ui.lineEditProbe20->setText(QString(strAny));
		if (ui.checkBoxProbe21->isChecked())
			ui.lineEditProbe21->setText(QString(strAny));
		if (ui.checkBoxProbe22->isChecked())
			ui.lineEditProbe22->setText(QString(strAny));
		if (ui.checkBoxProbe23->isChecked())
			ui.lineEditProbe23->setText(QString(strAny));
		if (ui.checkBoxProbe24->isChecked())
			ui.lineEditProbe24->setText(QString(strAny));
		if (ui.checkBoxProbe25->isChecked())
			ui.lineEditProbe25->setText(QString(strAny));
		if (ui.checkBoxProbe26->isChecked())
			ui.lineEditProbe26->setText(QString(strAny));
		if (ui.checkBoxProbe27->isChecked())
			ui.lineEditProbe27->setText(QString(strAny));
		if (ui.checkBoxProbe28->isChecked())
			ui.lineEditProbe28->setText(QString(strAny));
		if (ui.checkBoxProbe29->isChecked())
			ui.lineEditProbe29->setText(QString(strAny));
		if (ui.checkBoxProbe30->isChecked())
			ui.lineEditProbe30->setText(QString(strAny));
		if (ui.checkBoxProbe31->isChecked())
			ui.lineEditProbe31->setText(QString(strAny));
		if (ui.checkBoxProbe32->isChecked())
			ui.lineEditProbe32->setText(QString(strAny));
		if (ui.checkBoxProbe33->isChecked())
			ui.lineEditProbe33->setText(QString(strAny));
		if (ui.checkBoxProbe34->isChecked())
			ui.lineEditProbe34->setText(QString(strAny));
		if (ui.checkBoxProbe35->isChecked())
			ui.lineEditProbe35->setText(QString(strAny));
		if (ui.checkBoxProbe36->isChecked())
			ui.lineEditProbe36->setText(QString(strAny));
	}
}
