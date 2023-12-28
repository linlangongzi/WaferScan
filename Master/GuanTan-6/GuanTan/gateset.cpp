#include "gateset.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

GateSet::GateSet(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

GateSet::~GateSet()
{

}

void GateSet::UiInit()
{
	ui.pushButtonAllI->setEnabled(false);
	ui.pushButtonAllA->setEnabled(false);
	ui.pushButtonAllB->setEnabled(false);
	ui.pushButtonAllI2->setEnabled(false);
	ui.pushButtonAnyI->setEnabled(false);
	ui.pushButtonAnyA->setEnabled(false);
	ui.pushButtonAnyB->setEnabled(false);
	ui.pushButtonAnyI2->setEnabled(false);
	connect(ui.checkBoxProbe1_12, SIGNAL(clicked()), this, SLOT(Check1_10()));
	connect(ui.checkBoxProbe13_24, SIGNAL(clicked()), this, SLOT(Check11_20()));
	connect(ui.checkBoxProbe25_36, SIGNAL(clicked()), this, SLOT(Check21_30()));
	connect(ui.radioButtonAll, SIGNAL(clicked()), this, SLOT(SelectAll()));
	connect(ui.radioButtonAny, SIGNAL(clicked()), this, SLOT(SelectAny()));
	connect(ui.pushButtonAllI,SIGNAL(clicked()),this,SLOT(GateSetAllI()));
	connect(ui.pushButtonAllA,SIGNAL(clicked()),this,SLOT(GateSetAllA()));
	connect(ui.pushButtonAllB,SIGNAL(clicked()),this,SLOT(GateSetAllB()));
	connect(ui.pushButtonAllI2,SIGNAL(clicked()),this,SLOT(GateSetAllI2()));
	connect(ui.pushButtonAnyI,SIGNAL(clicked()),this,SLOT(GateSetAnyI()));
	connect(ui.pushButtonAnyA,SIGNAL(clicked()),this,SLOT(GateSetAnyA()));
	connect(ui.pushButtonAnyB,SIGNAL(clicked()),this,SLOT(GateSetAnyB()));
	connect(ui.pushButtonAnyI2,SIGNAL(clicked()),this,SLOT(GateSetAnyI2()));
	QRegExp RegN0_1000("^(1000[.]00|[1-9][0-9]{0,2}[.][0-9]{0,2}|0[.][0-9]{0,2})$");	//超声参数-闸门前沿/宽度: 0.00到1000.00 的正则表达式
	ui.lineEditAllIB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllIL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllAB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllAL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllBB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllBL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllI2B->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAllI2L->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyIB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyIL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyAB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyAL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyBB->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyBL->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyI2B->setValidator(new QRegExpValidator(RegN0_1000,this));
	ui.lineEditAnyI2L->setValidator(new QRegExpValidator(RegN0_1000,this));
	QRegExp RegN0_100("^(100[.]00|[1-9][0-9]{0,1}[.][0-9]{0,2}|0[.][0-9]{0,2})$");	//超声参数-闸门阈值: 0.00到100.00 的正则表达式
	ui.lineEditAllIT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAllAT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAllBT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAllI2T->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAnyIT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAnyAT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAnyBT->setValidator(new QRegExpValidator(RegN0_100,this));
	ui.lineEditAnyI2T->setValidator(new QRegExpValidator(RegN0_100,this));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("闸门设置"));
}

void GateSet::Check1_10()
{
	ui.checkBoxProbe13_24->setChecked(false);
	ui.checkBoxProbe25_36->setChecked(false);
	if (ui.checkBoxProbe1_12->isChecked())
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
	else
	{
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
	if (ui.radioButtonAll->isChecked())
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
	}
	else if (ui.radioButtonAny->isChecked())
	{
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
	}
}

void GateSet::Check11_20()
{
	ui.checkBoxProbe1_12->setChecked(false);
	ui.checkBoxProbe25_36->setChecked(false);
	if (ui.checkBoxProbe13_24->isChecked())
	{
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
	else
	{
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
	if (ui.radioButtonAll->isChecked())
	{
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
	}
	else if (ui.radioButtonAny->isChecked())
	{
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
	}
}

void GateSet::Check21_30()
{
	ui.checkBoxProbe1_12->setChecked(false);
	ui.checkBoxProbe13_24->setChecked(false);
	if (ui.checkBoxProbe25_36->isChecked())
	{
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
	}
	else
	{
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
	if (ui.radioButtonAll->isChecked())
	{
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
	}
	else if (ui.radioButtonAny->isChecked())
	{
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
	}
}

void GateSet::SelectAll()
{
	if (ui.checkBoxProbe1_12->isChecked())
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
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
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
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
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
	}
	ui.pushButtonAllI->setEnabled(true);
	ui.pushButtonAllA->setEnabled(true);
	ui.pushButtonAllB->setEnabled(true);
	ui.pushButtonAllI2->setEnabled(true);
	ui.pushButtonAnyI->setEnabled(false);
	ui.pushButtonAnyA->setEnabled(false);
	ui.pushButtonAnyB->setEnabled(false);
	ui.pushButtonAnyI2->setEnabled(false);
}

void GateSet::SelectAny()
{
	if (ui.checkBoxProbe1_12->isChecked())
	{
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
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
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
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
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
	}
	ui.pushButtonAllI->setEnabled(false);
	ui.pushButtonAllA->setEnabled(false);
	ui.pushButtonAllB->setEnabled(false);
	ui.pushButtonAllI2->setEnabled(false);
	ui.pushButtonAnyI->setEnabled(true);
	ui.pushButtonAnyA->setEnabled(true);
	ui.pushButtonAnyB->setEnabled(true);
	ui.pushButtonAnyI2->setEnabled(true);
}

void GateSet::GateSetAllI()
{
	if (ui.lineEditAllIB->text() != NULL)
	{
		ui.lineEditProbe1IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe2IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe3IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe4IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe5IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe6IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe7IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe8IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe9IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe10IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe11IB->setText(QString(ui.lineEditAllIB->text()));
		ui.lineEditProbe12IB->setText(QString(ui.lineEditAllIB->text()));
	}
	if (ui.lineEditAllIL->text() != NULL)
	{
		ui.lineEditProbe1IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe2IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe3IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe4IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe5IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe6IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe7IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe8IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe9IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe10IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe11IL->setText(QString(ui.lineEditAllIL->text()));
		ui.lineEditProbe12IL->setText(QString(ui.lineEditAllIL->text()));
	}
	if (ui.lineEditAllIT->text() != NULL)
	{
		ui.lineEditProbe1IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe2IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe3IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe4IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe5IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe6IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe7IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe8IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe9IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe10IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe11IT->setText(QString(ui.lineEditAllIT->text()));
		ui.lineEditProbe12IT->setText(QString(ui.lineEditAllIT->text()));
	}
}
void GateSet::GateSetAllA()
{
	if (ui.lineEditAllAB->text() != NULL)
	{
		ui.lineEditProbe1AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe2AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe3AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe4AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe5AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe6AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe7AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe8AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe9AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe10AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe11AB->setText(QString(ui.lineEditAllAB->text()));
		ui.lineEditProbe12AB->setText(QString(ui.lineEditAllAB->text()));
	}
	if (ui.lineEditAllAL->text() != NULL)
	{
		ui.lineEditProbe1AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe2AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe3AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe4AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe5AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe6AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe7AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe8AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe9AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe10AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe11AL->setText(QString(ui.lineEditAllAL->text()));
		ui.lineEditProbe12AL->setText(QString(ui.lineEditAllAL->text()));
	}
	if (ui.lineEditAllAT->text() != NULL)
	{
		ui.lineEditProbe1AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe2AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe3AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe4AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe5AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe6AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe7AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe8AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe9AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe10AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe11AT->setText(QString(ui.lineEditAllAT->text()));
		ui.lineEditProbe12AT->setText(QString(ui.lineEditAllAT->text()));
	}
}
void GateSet::GateSetAllB()
{
	if (ui.lineEditAllBB->text() != NULL)
	{
		ui.lineEditProbe1BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe2BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe3BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe4BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe5BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe6BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe7BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe8BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe9BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe10BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe11BB->setText(QString(ui.lineEditAllBB->text()));
		ui.lineEditProbe12BB->setText(QString(ui.lineEditAllBB->text()));
	}
	if (ui.lineEditAllBL->text() != NULL)
	{
		ui.lineEditProbe1BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe2BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe3BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe4BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe5BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe6BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe7BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe8BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe9BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe10BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe11BL->setText(QString(ui.lineEditAllBL->text()));
		ui.lineEditProbe12BL->setText(QString(ui.lineEditAllBL->text()));
	}
	if (ui.lineEditAllBT->text() != NULL)
	{
		ui.lineEditProbe1BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe2BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe3BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe4BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe5BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe6BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe7BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe8BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe9BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe10BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe11BT->setText(QString(ui.lineEditAllBT->text()));
		ui.lineEditProbe12BT->setText(QString(ui.lineEditAllBT->text()));
	}
}
void GateSet::GateSetAllI2()
{
	if (ui.lineEditAllI2B->text() != NULL)
	{
		ui.lineEditProbe1I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe2I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe3I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe4I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe5I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe6I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe7I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe8I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe9I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe10I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe11I2B->setText(QString(ui.lineEditAllI2B->text()));
		ui.lineEditProbe12I2B->setText(QString(ui.lineEditAllI2B->text()));
	}
	if (ui.lineEditAllI2L->text() != NULL)
	{
		ui.lineEditProbe1I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe2I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe3I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe4I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe5I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe6I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe7I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe8I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe9I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe10I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe11I2L->setText(QString(ui.lineEditAllI2L->text()));
		ui.lineEditProbe12I2L->setText(QString(ui.lineEditAllI2L->text()));
	}
	if (ui.lineEditAllI2T->text() != NULL)
	{
		ui.lineEditProbe1I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe2I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe3I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe4I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe5I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe6I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe7I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe8I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe9I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe10I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe11I2T->setText(QString(ui.lineEditAllI2T->text()));
		ui.lineEditProbe12I2T->setText(QString(ui.lineEditAllI2T->text()));
	}
}
void GateSet::GateSetAnyI()
{
	if (ui.checkBoxProbe1_12->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe1IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe1IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe1IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe2->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe2IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe2IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe2IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe3->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe3IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe3IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe3IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe4->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe4IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe4IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe4IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe5->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe5IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe5IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe5IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe6->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe6IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe6IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe6IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe7->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe7IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe7IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe7IT->setText(QString(ui.lineEditAnyIT->text()));
			}	
		}
		if (ui.checkBoxProbe8->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe8IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe8IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe8IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe9->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe9IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe9IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe9IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe10->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe10IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe10IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe10IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe11->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe11IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe11IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe11IT->setText(QString(ui.lineEditAnyIT->text()));
			}		
		}
		if (ui.checkBoxProbe12->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe12IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe12IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe12IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
		if (ui.checkBoxProbe13->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe1IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe1IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe1IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe14->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe2IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe2IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe2IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe15->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe3IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe3IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe3IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe16->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe4IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe4IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe4IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe17->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe5IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe5IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe5IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe18->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe6IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe6IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe6IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe19->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe7IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe7IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe7IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe20->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe8IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe8IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe8IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe21->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe9IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe9IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe9IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe22->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe10IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe10IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe10IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe23->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe11IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe11IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe11IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe24->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe12IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe12IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe12IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
		if (ui.checkBoxProbe25->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe1IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe1IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe1IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe26->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe2IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe2IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe2IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe27->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe3IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe3IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe3IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe28->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe4IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe4IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe4IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe29->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe5IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe5IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe5IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe30->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe6IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe6IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe6IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe31->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe7IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe7IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe7IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe32->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe8IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe8IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe8IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe33->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe9IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe9IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe9IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe34->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe10IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe10IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe10IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe35->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe11IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe11IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe11IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
		if (ui.checkBoxProbe36->isChecked())
		{
			if (ui.lineEditAnyIB->text() != NULL)
			{
				ui.lineEditProbe12IB->setText(QString(ui.lineEditAnyIB->text()));
			}
			if (ui.lineEditAnyIL->text() != NULL)
			{
				ui.lineEditProbe12IL->setText(QString(ui.lineEditAnyIL->text()));
			}
			if (ui.lineEditAnyIT->text() != NULL)
			{
				ui.lineEditProbe12IT->setText(QString(ui.lineEditAnyIT->text()));
			}
		}
	}
}
void GateSet::GateSetAnyA()
{
	if (ui.checkBoxProbe1_12->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe1AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe1AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe1AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe2->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe2AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe2AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe2AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe3->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe3AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe3AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe3AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe4->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe4AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe4AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe4AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe5->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe5AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe5AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe5AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe6->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe6AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe6AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe6AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe7->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe7AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe7AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe7AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe8->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe8AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe8AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe8AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe9->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe9AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe9AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe9AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe10->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe10AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe10AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe10AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe11->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe11AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe11AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe11AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe12->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe12AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe12AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe12AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
		if (ui.checkBoxProbe13->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe1AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe1AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe1AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe14->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe2AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe2AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe2AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe15->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe3AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe3AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe3AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe16->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe4AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe4AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe4AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe17->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe5AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe5AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe5AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe18->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe6AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe6AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe6AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe19->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe7AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe7AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe7AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe20->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe8AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe8AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe8AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe21->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe9AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe9AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe9AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe22->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe10AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe10AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe10AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe23->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe11AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe11AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe11AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe24->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe12AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe12AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe12AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
		if (ui.checkBoxProbe25->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe1AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe1AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe1AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe26->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe2AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe2AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe2AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe27->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe3AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe3AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe3AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe28->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe4AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe4AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe4AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe29->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe5AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe5AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe5AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe30->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe6AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe6AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe6AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe31->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe7AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe7AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe7AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe32->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe8AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe8AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe8AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe33->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe9AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe9AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe9AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe34->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe10AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe10AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe10AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe35->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe11AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe11AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe11AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
		if (ui.checkBoxProbe36->isChecked())
		{
			if (ui.lineEditAnyAB->text() != NULL)
			{
				ui.lineEditProbe12AB->setText(QString(ui.lineEditAnyAB->text()));
			}
			if (ui.lineEditAnyAL->text() != NULL)
			{
				ui.lineEditProbe12AL->setText(QString(ui.lineEditAnyAL->text()));
			}
			if (ui.lineEditAnyAT->text() != NULL)
			{
				ui.lineEditProbe12AT->setText(QString(ui.lineEditAnyAT->text()));
			}
		}
	}
}
void GateSet::GateSetAnyB()
{
	if (ui.checkBoxProbe1_12->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe1BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe1BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe1BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe2->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe2BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe2BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe2BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe3->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe3BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe3BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe3BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe4->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe4BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe4BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe4BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe5->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe5BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe5BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe5BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe6->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe6BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe6BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe6BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe7->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe7BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe7BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe7BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe8->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe8BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe8BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe8BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe9->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe9BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe9BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe9BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe10->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe10BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe10BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe10BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe11->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe11BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe11BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe11BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe12->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe12BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe12BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe12BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
		if (ui.checkBoxProbe13->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe1BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe1BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe1BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe14->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe2BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe2BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe2BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe15->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe3BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe3BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe3BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe16->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe4BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe4BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe4BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe17->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe5BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe5BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe5BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe18->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe6BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe6BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe6BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe19->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe7BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe7BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe7BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe20->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe8BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe8BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe8BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe21->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe9BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe9BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe9BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe22->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe10BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe10BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe10BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe23->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe11BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe11BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe11BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe24->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe12BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe12BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe12BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
		
		if (ui.checkBoxProbe25->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe1BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe1BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe1BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe26->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe2BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe2BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe2BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe27->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe3BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe3BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe3BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe28->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe4BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe4BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe4BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe29->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe5BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe5BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe5BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe30->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe6BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe6BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe6BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe31->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe7BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe7BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe7BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe32->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe8BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe8BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe8BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe33->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe9BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe9BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe9BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe34->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe10BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe10BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe10BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe35->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe11BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe11BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe11BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
		if (ui.checkBoxProbe36->isChecked())
		{
			if (ui.lineEditAnyBB->text() != NULL)
			{
				ui.lineEditProbe12BB->setText(QString(ui.lineEditAnyBB->text()));
			}
			if (ui.lineEditAnyBL->text() != NULL)
			{
				ui.lineEditProbe12BL->setText(QString(ui.lineEditAnyBL->text()));
			}
			if (ui.lineEditAnyBT->text() != NULL)
			{
				ui.lineEditProbe12BT->setText(QString(ui.lineEditAnyBT->text()));
			}
		}
	}
}
void GateSet::GateSetAnyI2()
{
	if (ui.checkBoxProbe1_12->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe1I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe1I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe1I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe2->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe2I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe2I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe2I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe3->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe3I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe3I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe3I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe4->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe4I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe4I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe4I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe5->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe5I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe5I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe5I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe6->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe6I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe6I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe6I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe7->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe7I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe7I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe7I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe8->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe8I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe8I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe8I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe9->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe9I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe9I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe9I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe10->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe10I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe10I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe10I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe11->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe11I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe11I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe11I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe12->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe12I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe12I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe12I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
	}
	else if (ui.checkBoxProbe13_24->isChecked())
	{
		if (ui.checkBoxProbe13->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe1I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe1I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe1I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe14->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe2I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe2I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe2I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe15->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe3I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe3I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe3I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe16->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe4I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe4I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe4I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe17->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe5I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe5I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe5I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe18->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe6I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe6I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe6I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe19->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe7I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe7I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe7I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe20->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe8I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe8I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe8I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe21->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe9I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe9I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe9I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe22->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe10I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe10I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe10I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe23->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe11I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe11I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe11I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe24->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe12I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe12I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe12I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
	}
	else if (ui.checkBoxProbe25_36->isChecked())
	{
		if (ui.checkBoxProbe25->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe1I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe1I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe1I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe26->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe2I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe2I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe2I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe27->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe3I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe3I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe3I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe28->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe4I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe4I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe4I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe29->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe5I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe5I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe5I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe30->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe6I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe6I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe6I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe31->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe7I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe7I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe7I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe32->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe8I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe8I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe8I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe33->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe9I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe9I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe9I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe34->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe10I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe10I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe10I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe35->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe11I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe11I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe11I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
		if (ui.checkBoxProbe36->isChecked())
		{
			if (ui.lineEditAnyI2B->text() != NULL)
			{
				ui.lineEditProbe12I2B->setText(QString(ui.lineEditAnyI2B->text()));
			}
			if (ui.lineEditAnyI2L->text() != NULL)
			{
				ui.lineEditProbe12I2L->setText(QString(ui.lineEditAnyI2L->text()));
			}
			if (ui.lineEditAnyI2T->text() != NULL)
			{
				ui.lineEditProbe12I2T->setText(QString(ui.lineEditAnyI2T->text()));
			}
		}
	}
}
