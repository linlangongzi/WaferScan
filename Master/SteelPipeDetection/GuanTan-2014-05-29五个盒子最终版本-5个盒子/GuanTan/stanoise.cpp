#include "stanoise.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

StaNoise::StaNoise(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

StaNoise::~StaNoise()
{

}

void StaNoise::UiInit()
{
	ui.pushButtonAllI->setEnabled(false);
	ui.pushButtonAllA->setEnabled(false);
	ui.pushButtonAllB->setEnabled(false);
	ui.pushButtonAllI2->setEnabled(false);
	ui.pushButtonAnyI->setEnabled(false);
	ui.pushButtonAnyA->setEnabled(false);
	ui.pushButtonAnyB->setEnabled(false);
	ui.pushButtonAnyI2->setEnabled(false);
	connect(ui.checkBoxProbe1_10, SIGNAL(clicked()), this, SLOT(Check1_10()));
	connect(ui.checkBoxProbe11_20, SIGNAL(clicked()), this, SLOT(Check11_20()));
	connect(ui.checkBoxProbe21_30, SIGNAL(clicked()), this, SLOT(Check21_30()));
	connect(ui.radioButtonAll, SIGNAL(clicked()), this, SLOT(SelectAll()));
	connect(ui.radioButtonAny, SIGNAL(clicked()), this, SLOT(SelectAny()));
	connect(ui.pushButtonAllI, SIGNAL(clicked()), this, SLOT(AllI()));
	connect(ui.pushButtonAllA, SIGNAL(clicked()), this, SLOT(AllA()));
	connect(ui.pushButtonAllB, SIGNAL(clicked()), this, SLOT(AllB()));
	connect(ui.pushButtonAllI2, SIGNAL(clicked()), this, SLOT(AllI2()));
	connect(ui.pushButtonAnyI, SIGNAL(clicked()), this, SLOT(AnyI()));
	connect(ui.pushButtonAnyA, SIGNAL(clicked()), this, SLOT(AnyA()));
	connect(ui.pushButtonAnyB, SIGNAL(clicked()), this, SLOT(AnyB()));
	connect(ui.pushButtonAnyI2, SIGNAL(clicked()), this, SLOT(AnyI2()));
	QRegExp RegN0_60000("^0|[1-5][0-9]{0,4}|60000|[6-9][0-9]{0,3}$");	//超声参数-统计降噪: 0到60000 的正则表达式
	ui.lineEditAllI->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAllA->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAllB->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAllI2->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAnyI->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAnyA->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAnyB->setValidator(new QRegExpValidator(RegN0_60000,this));
	ui.lineEditAnyI2->setValidator(new QRegExpValidator(RegN0_60000,this));
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("统计降噪数"));
}

void StaNoise::Check1_10()
{
	ui.checkBoxProbe11_20->setChecked(false);
	ui.checkBoxProbe21_30->setChecked(false);
	if (ui.checkBoxProbe1_10->isChecked())
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
	}
}

void StaNoise::Check11_20()
{
	ui.checkBoxProbe1_10->setChecked(false);
	ui.checkBoxProbe21_30->setChecked(false);
	if (ui.checkBoxProbe11_20->isChecked())
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
	}
	if (ui.radioButtonAll->isChecked())
	{
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
	}
	else if (ui.radioButtonAny->isChecked())
	{
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
	}
}

void StaNoise::Check21_30()
{
	ui.checkBoxProbe1_10->setChecked(false);
	ui.checkBoxProbe11_20->setChecked(false);
	if (ui.checkBoxProbe21_30->isChecked())
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
	}
	if (ui.radioButtonAll->isChecked())
	{
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
	}
	else if (ui.radioButtonAny->isChecked())
	{
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
	}
}
void StaNoise::SelectAll()
{
	if (ui.checkBoxProbe1_10->isChecked())
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
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
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
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
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

void StaNoise::SelectAny()
{
	if (ui.checkBoxProbe1_10->isChecked())
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
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
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
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
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

void StaNoise::AllI()
{
	QString strAllI = ui.lineEditAllI->text();
	if (strAllI != NULL)
	{
		ui.lineEditIProbe1->setText(QString(strAllI));
		ui.lineEditIProbe2->setText(QString(strAllI));
		ui.lineEditIProbe3->setText(QString(strAllI));
		ui.lineEditIProbe4->setText(QString(strAllI));
		ui.lineEditIProbe5->setText(QString(strAllI));
		ui.lineEditIProbe6->setText(QString(strAllI));
		ui.lineEditIProbe7->setText(QString(strAllI));
		ui.lineEditIProbe8->setText(QString(strAllI));
		ui.lineEditIProbe9->setText(QString(strAllI));
		ui.lineEditIProbe0->setText(QString(strAllI));
	}
}
void StaNoise::AllA()
{
	QString strAllA = ui.lineEditAllA->text();
	if (strAllA != NULL)
	{
		ui.lineEditAProbe1->setText(QString(strAllA));
		ui.lineEditAProbe2->setText(QString(strAllA));
		ui.lineEditAProbe3->setText(QString(strAllA));
		ui.lineEditAProbe4->setText(QString(strAllA));
		ui.lineEditAProbe5->setText(QString(strAllA));
		ui.lineEditAProbe6->setText(QString(strAllA));
		ui.lineEditAProbe7->setText(QString(strAllA));
		ui.lineEditAProbe8->setText(QString(strAllA));
		ui.lineEditAProbe9->setText(QString(strAllA));
		ui.lineEditAProbe0->setText(QString(strAllA));
	}
}
void StaNoise::AllB()
{
	QString strAllB = ui.lineEditAllB->text();
	if (strAllB != NULL)
	{
		ui.lineEditBProbe1->setText(QString(strAllB));
		ui.lineEditBProbe2->setText(QString(strAllB));
		ui.lineEditBProbe3->setText(QString(strAllB));
		ui.lineEditBProbe4->setText(QString(strAllB));
		ui.lineEditBProbe5->setText(QString(strAllB));
		ui.lineEditBProbe6->setText(QString(strAllB));
		ui.lineEditBProbe7->setText(QString(strAllB));
		ui.lineEditBProbe8->setText(QString(strAllB));
		ui.lineEditBProbe9->setText(QString(strAllB));
		ui.lineEditBProbe0->setText(QString(strAllB));
	}
}
void StaNoise::AllI2()
{
	QString strAllI2 = ui.lineEditAllI2->text();
	if (strAllI2 != NULL)
	{
		ui.lineEditI2Probe1->setText(QString(strAllI2));
		ui.lineEditI2Probe2->setText(QString(strAllI2));
		ui.lineEditI2Probe3->setText(QString(strAllI2));
		ui.lineEditI2Probe4->setText(QString(strAllI2));
		ui.lineEditI2Probe5->setText(QString(strAllI2));
		ui.lineEditI2Probe6->setText(QString(strAllI2));
		ui.lineEditI2Probe7->setText(QString(strAllI2));
		ui.lineEditI2Probe8->setText(QString(strAllI2));
		ui.lineEditI2Probe9->setText(QString(strAllI2));
		ui.lineEditI2Probe0->setText(QString(strAllI2));
	}
}
void StaNoise::AnyI()
{
	if (ui.lineEditAnyI->text() != NULL)
	{
		if (ui.checkBoxProbe1_10->isChecked())
		{
			if (ui.checkBoxProbe1->isChecked())
				ui.lineEditIProbe1->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe2->isChecked())
				ui.lineEditIProbe2->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe3->isChecked())
				ui.lineEditIProbe3->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe4->isChecked())
				ui.lineEditIProbe4->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe5->isChecked())
				ui.lineEditIProbe5->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe6->isChecked())
				ui.lineEditIProbe6->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe7->isChecked())
				ui.lineEditIProbe7->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe8->isChecked())
				ui.lineEditIProbe8->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe9->isChecked())
				ui.lineEditIProbe9->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe10->isChecked())
				ui.lineEditIProbe0->setText(QString(ui.lineEditAnyI->text()));		
		}
		else if (ui.checkBoxProbe11_20->isChecked())
		{
			if (ui.checkBoxProbe11->isChecked())
				ui.lineEditIProbe1->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe12->isChecked())
				ui.lineEditIProbe2->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe13->isChecked())
				ui.lineEditIProbe3->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe14->isChecked())
				ui.lineEditIProbe4->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe15->isChecked())
				ui.lineEditIProbe5->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe16->isChecked())
				ui.lineEditIProbe6->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe17->isChecked())
				ui.lineEditIProbe7->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe18->isChecked())
				ui.lineEditIProbe8->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe19->isChecked())
				ui.lineEditIProbe9->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe20->isChecked())
				ui.lineEditIProbe0->setText(QString(ui.lineEditAnyI->text()));
		}
		else if (ui.checkBoxProbe21_30->isChecked())
		{
			if (ui.checkBoxProbe21->isChecked())
				ui.lineEditIProbe1->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe22->isChecked())
				ui.lineEditIProbe2->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe23->isChecked())
				ui.lineEditIProbe3->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe24->isChecked())
				ui.lineEditIProbe4->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe25->isChecked())
				ui.lineEditIProbe5->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe26->isChecked())
				ui.lineEditIProbe6->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe27->isChecked())
				ui.lineEditIProbe7->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe28->isChecked())
				ui.lineEditIProbe8->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe29->isChecked())
				ui.lineEditIProbe9->setText(QString(ui.lineEditAnyI->text()));
			if (ui.checkBoxProbe30->isChecked())
				ui.lineEditIProbe0->setText(QString(ui.lineEditAnyI->text()));
		}
	}
}
void StaNoise::AnyA()
{
	if (ui.lineEditAnyA->text() != NULL)
	{
		if (ui.checkBoxProbe1_10->isChecked())
		{
			if (ui.checkBoxProbe1->isChecked())
				ui.lineEditAProbe1->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe2->isChecked())
				ui.lineEditAProbe2->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe3->isChecked())
				ui.lineEditAProbe3->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe4->isChecked())
				ui.lineEditAProbe4->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe5->isChecked())
				ui.lineEditAProbe5->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe6->isChecked())
				ui.lineEditAProbe6->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe7->isChecked())
				ui.lineEditAProbe7->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe8->isChecked())
				ui.lineEditAProbe8->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe9->isChecked())
				ui.lineEditAProbe9->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe10->isChecked())
				ui.lineEditAProbe0->setText(QString(ui.lineEditAnyA->text()));		
		}
		else if (ui.checkBoxProbe11_20->isChecked())
		{
			if (ui.checkBoxProbe11->isChecked())
				ui.lineEditAProbe1->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe12->isChecked())
				ui.lineEditAProbe2->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe13->isChecked())
				ui.lineEditAProbe3->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe14->isChecked())
				ui.lineEditAProbe4->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe15->isChecked())
				ui.lineEditAProbe5->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe16->isChecked())
				ui.lineEditAProbe6->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe17->isChecked())
				ui.lineEditAProbe7->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe18->isChecked())
				ui.lineEditAProbe8->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe19->isChecked())
				ui.lineEditAProbe9->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe20->isChecked())
				ui.lineEditAProbe0->setText(QString(ui.lineEditAnyA->text()));
		}
		else if (ui.checkBoxProbe21_30->isChecked())
		{
			if (ui.checkBoxProbe21->isChecked())
				ui.lineEditAProbe1->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe22->isChecked())
				ui.lineEditAProbe2->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe23->isChecked())
				ui.lineEditAProbe3->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe24->isChecked())
				ui.lineEditAProbe4->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe25->isChecked())
				ui.lineEditAProbe5->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe26->isChecked())
				ui.lineEditAProbe6->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe27->isChecked())
				ui.lineEditAProbe7->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe28->isChecked())
				ui.lineEditAProbe8->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe29->isChecked())
				ui.lineEditAProbe9->setText(QString(ui.lineEditAnyA->text()));
			if (ui.checkBoxProbe30->isChecked())
				ui.lineEditAProbe0->setText(QString(ui.lineEditAnyA->text()));
		}
	}
}
void StaNoise::AnyB()
{
	if (ui.lineEditAnyB->text() != NULL)
	{
		if (ui.checkBoxProbe1_10->isChecked())
		{
			if (ui.checkBoxProbe1->isChecked())
				ui.lineEditBProbe1->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe2->isChecked())
				ui.lineEditBProbe2->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe3->isChecked())
				ui.lineEditBProbe3->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe4->isChecked())
				ui.lineEditBProbe4->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe5->isChecked())
				ui.lineEditBProbe5->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe6->isChecked())
				ui.lineEditBProbe6->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe7->isChecked())
				ui.lineEditBProbe7->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe8->isChecked())
				ui.lineEditBProbe8->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe9->isChecked())
				ui.lineEditBProbe9->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe10->isChecked())
				ui.lineEditBProbe0->setText(QString(ui.lineEditAnyB->text()));		
		}
		else if (ui.checkBoxProbe11_20->isChecked())
		{
			if (ui.checkBoxProbe11->isChecked())
				ui.lineEditBProbe1->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe12->isChecked())
				ui.lineEditBProbe2->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe13->isChecked())
				ui.lineEditBProbe3->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe14->isChecked())
				ui.lineEditBProbe4->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe15->isChecked())
				ui.lineEditBProbe5->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe16->isChecked())
				ui.lineEditBProbe6->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe17->isChecked())
				ui.lineEditBProbe7->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe18->isChecked())
				ui.lineEditBProbe8->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe19->isChecked())
				ui.lineEditBProbe9->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe20->isChecked())
				ui.lineEditBProbe0->setText(QString(ui.lineEditAnyB->text()));
		}
		else if (ui.checkBoxProbe21_30->isChecked())
		{
			if (ui.checkBoxProbe21->isChecked())
				ui.lineEditBProbe1->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe22->isChecked())
				ui.lineEditBProbe2->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe23->isChecked())
				ui.lineEditBProbe3->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe24->isChecked())
				ui.lineEditBProbe4->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe25->isChecked())
				ui.lineEditBProbe5->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe26->isChecked())
				ui.lineEditBProbe6->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe27->isChecked())
				ui.lineEditBProbe7->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe28->isChecked())
				ui.lineEditBProbe8->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe29->isChecked())
				ui.lineEditBProbe9->setText(QString(ui.lineEditAnyB->text()));
			if (ui.checkBoxProbe30->isChecked())
				ui.lineEditBProbe0->setText(QString(ui.lineEditAnyB->text()));
		}
	}
}
void StaNoise::AnyI2()
{
	if (ui.lineEditAnyI2->text() != NULL)
	{
		if (ui.checkBoxProbe1_10->isChecked())
		{
			if (ui.checkBoxProbe1->isChecked())
				ui.lineEditI2Probe1->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe2->isChecked())
				ui.lineEditI2Probe2->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe3->isChecked())
				ui.lineEditI2Probe3->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe4->isChecked())
				ui.lineEditI2Probe4->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe5->isChecked())
				ui.lineEditI2Probe5->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe6->isChecked())
				ui.lineEditI2Probe6->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe7->isChecked())
				ui.lineEditI2Probe7->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe8->isChecked())
				ui.lineEditI2Probe8->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe9->isChecked())
				ui.lineEditI2Probe9->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe10->isChecked())
				ui.lineEditI2Probe0->setText(QString(ui.lineEditAnyI2->text()));		
		}
		else if (ui.checkBoxProbe11_20->isChecked())
		{
			if (ui.checkBoxProbe11->isChecked())
				ui.lineEditI2Probe1->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe12->isChecked())
				ui.lineEditI2Probe2->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe13->isChecked())
				ui.lineEditI2Probe3->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe14->isChecked())
				ui.lineEditI2Probe4->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe15->isChecked())
				ui.lineEditI2Probe5->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe16->isChecked())
				ui.lineEditI2Probe6->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe17->isChecked())
				ui.lineEditI2Probe7->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe18->isChecked())
				ui.lineEditI2Probe8->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe19->isChecked())
				ui.lineEditI2Probe9->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe20->isChecked())
				ui.lineEditI2Probe0->setText(QString(ui.lineEditAnyI2->text()));
		}
		else if (ui.checkBoxProbe21_30->isChecked())
		{
			if (ui.checkBoxProbe21->isChecked())
				ui.lineEditI2Probe1->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe22->isChecked())
				ui.lineEditI2Probe2->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe23->isChecked())
				ui.lineEditI2Probe3->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe24->isChecked())
				ui.lineEditI2Probe4->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe25->isChecked())
				ui.lineEditI2Probe5->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe26->isChecked())
				ui.lineEditI2Probe6->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe27->isChecked())
				ui.lineEditI2Probe7->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe28->isChecked())
				ui.lineEditI2Probe8->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe29->isChecked())
				ui.lineEditI2Probe9->setText(QString(ui.lineEditAnyI2->text()));
			if (ui.checkBoxProbe30->isChecked())
				ui.lineEditI2Probe0->setText(QString(ui.lineEditAnyI2->text()));
		}
	}
}
