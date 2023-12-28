#include "judge.h"

Judge::Judge(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

Judge::~Judge()
{

}

void Judge::UiInit()
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
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/gangbancanshushezhi_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("闸门开关设置"));
}

void Judge::Check1_10()
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

void Judge::Check11_20()
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

void Judge::Check21_30()
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
void Judge::SelectAll()
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
void Judge::SelectAny()
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
void Judge::AllI()
{
	ui.comboBoxIProbe1->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe2->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe3->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe4->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe5->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe6->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe7->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe8->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe9->setCurrentIndex(ui.comboBoxAllI->currentIndex());
	ui.comboBoxIProbe0->setCurrentIndex(ui.comboBoxAllI->currentIndex());
}
void Judge::AllA()
{
	ui.comboBoxAProbe1->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe2->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe3->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe4->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe5->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe6->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe7->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe8->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe9->setCurrentIndex(ui.comboBoxAllA->currentIndex());
	ui.comboBoxAProbe0->setCurrentIndex(ui.comboBoxAllA->currentIndex());
}
void Judge::AllB()
{
	ui.comboBoxBProbe1->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe2->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe3->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe4->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe5->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe6->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe7->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe8->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe9->setCurrentIndex(ui.comboBoxAllB->currentIndex());
	ui.comboBoxBProbe0->setCurrentIndex(ui.comboBoxAllB->currentIndex());
}
void Judge::AllI2()
{
	ui.comboBoxI2Probe1->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe2->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe3->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe4->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe5->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe6->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe7->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe8->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe9->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
	ui.comboBoxI2Probe0->setCurrentIndex(ui.comboBoxAllI2->currentIndex());
}
void Judge::AnyI()
{
	if (ui.checkBoxProbe1_10->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
			ui.comboBoxIProbe1->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe2->isChecked())
			ui.comboBoxIProbe2->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe3->isChecked())
			ui.comboBoxIProbe3->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe4->isChecked())
			ui.comboBoxIProbe4->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe5->isChecked())
			ui.comboBoxIProbe5->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe6->isChecked())
			ui.comboBoxIProbe6->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe7->isChecked())
			ui.comboBoxIProbe7->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe8->isChecked())
			ui.comboBoxIProbe8->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe9->isChecked())
			ui.comboBoxIProbe9->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe10->isChecked())
			ui.comboBoxIProbe0->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
		if (ui.checkBoxProbe11->isChecked())
			ui.comboBoxIProbe1->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe12->isChecked())
			ui.comboBoxIProbe2->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe13->isChecked())
			ui.comboBoxIProbe3->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe14->isChecked())
			ui.comboBoxIProbe4->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe15->isChecked())
			ui.comboBoxIProbe5->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe16->isChecked())
			ui.comboBoxIProbe6->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe17->isChecked())
			ui.comboBoxIProbe7->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe18->isChecked())
			ui.comboBoxIProbe8->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe19->isChecked())
			ui.comboBoxIProbe9->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe20->isChecked())
			ui.comboBoxIProbe0->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
		if (ui.checkBoxProbe21->isChecked())
			ui.comboBoxIProbe1->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe22->isChecked())
			ui.comboBoxIProbe2->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe23->isChecked())
			ui.comboBoxIProbe3->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe24->isChecked())
			ui.comboBoxIProbe4->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe25->isChecked())
			ui.comboBoxIProbe5->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe26->isChecked())
			ui.comboBoxIProbe6->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe27->isChecked())
			ui.comboBoxIProbe7->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe28->isChecked())
			ui.comboBoxIProbe8->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe29->isChecked())
			ui.comboBoxIProbe9->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
		if (ui.checkBoxProbe30->isChecked())
			ui.comboBoxIProbe0->setCurrentIndex(ui.comboBoxAnyI->currentIndex());
	}
}
void Judge::AnyA()
{
	if (ui.checkBoxProbe1_10->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
			ui.comboBoxAProbe1->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe2->isChecked())
			ui.comboBoxAProbe2->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe3->isChecked())
			ui.comboBoxAProbe3->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe4->isChecked())
			ui.comboBoxAProbe4->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe5->isChecked())
			ui.comboBoxAProbe5->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe6->isChecked())
			ui.comboBoxAProbe6->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe7->isChecked())
			ui.comboBoxAProbe7->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe8->isChecked())
			ui.comboBoxAProbe8->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe9->isChecked())
			ui.comboBoxAProbe9->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe10->isChecked())
			ui.comboBoxAProbe0->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
		if (ui.checkBoxProbe11->isChecked())
			ui.comboBoxAProbe1->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe12->isChecked())
			ui.comboBoxAProbe2->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe13->isChecked())
			ui.comboBoxAProbe3->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe14->isChecked())
			ui.comboBoxAProbe4->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe15->isChecked())
			ui.comboBoxAProbe5->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe16->isChecked())
			ui.comboBoxAProbe6->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe17->isChecked())
			ui.comboBoxAProbe7->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe18->isChecked())
			ui.comboBoxAProbe8->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe19->isChecked())
			ui.comboBoxAProbe9->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe20->isChecked())
			ui.comboBoxAProbe0->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
		if (ui.checkBoxProbe21->isChecked())
			ui.comboBoxAProbe1->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe22->isChecked())
			ui.comboBoxAProbe2->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe23->isChecked())
			ui.comboBoxAProbe3->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe24->isChecked())
			ui.comboBoxAProbe4->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe25->isChecked())
			ui.comboBoxAProbe5->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe26->isChecked())
			ui.comboBoxAProbe6->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe27->isChecked())
			ui.comboBoxAProbe7->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe28->isChecked())
			ui.comboBoxAProbe8->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe29->isChecked())
			ui.comboBoxAProbe9->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
		if (ui.checkBoxProbe30->isChecked())
			ui.comboBoxAProbe0->setCurrentIndex(ui.comboBoxAnyA->currentIndex());
	}
}
void Judge::AnyB()
{
	if (ui.checkBoxProbe1_10->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
			ui.comboBoxBProbe1->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe2->isChecked())
			ui.comboBoxBProbe2->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe3->isChecked())
			ui.comboBoxBProbe3->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe4->isChecked())
			ui.comboBoxBProbe4->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe5->isChecked())
			ui.comboBoxBProbe5->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe6->isChecked())
			ui.comboBoxBProbe6->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe7->isChecked())
			ui.comboBoxBProbe7->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe8->isChecked())
			ui.comboBoxBProbe8->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe9->isChecked())
			ui.comboBoxBProbe9->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe10->isChecked())
			ui.comboBoxBProbe0->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
		if (ui.checkBoxProbe11->isChecked())
			ui.comboBoxBProbe1->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe12->isChecked())
			ui.comboBoxBProbe2->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe13->isChecked())
			ui.comboBoxBProbe3->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe14->isChecked())
			ui.comboBoxBProbe4->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe15->isChecked())
			ui.comboBoxBProbe5->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe16->isChecked())
			ui.comboBoxBProbe6->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe17->isChecked())
			ui.comboBoxBProbe7->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe18->isChecked())
			ui.comboBoxBProbe8->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe19->isChecked())
			ui.comboBoxBProbe9->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe20->isChecked())
			ui.comboBoxBProbe0->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
		if (ui.checkBoxProbe21->isChecked())
			ui.comboBoxBProbe1->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe22->isChecked())
			ui.comboBoxBProbe2->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe23->isChecked())
			ui.comboBoxBProbe3->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe24->isChecked())
			ui.comboBoxBProbe4->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe25->isChecked())
			ui.comboBoxBProbe5->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe26->isChecked())
			ui.comboBoxBProbe6->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe27->isChecked())
			ui.comboBoxBProbe7->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe28->isChecked())
			ui.comboBoxBProbe8->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe29->isChecked())
			ui.comboBoxBProbe9->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
		if (ui.checkBoxProbe30->isChecked())
			ui.comboBoxBProbe0->setCurrentIndex(ui.comboBoxAnyB->currentIndex());
	}
}
void Judge::AnyI2()
{
	if (ui.checkBoxProbe1_10->isChecked())
	{
		if (ui.checkBoxProbe1->isChecked())
			ui.comboBoxI2Probe1->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe2->isChecked())
			ui.comboBoxI2Probe2->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe3->isChecked())
			ui.comboBoxI2Probe3->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe4->isChecked())
			ui.comboBoxI2Probe4->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe5->isChecked())
			ui.comboBoxI2Probe5->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe6->isChecked())
			ui.comboBoxI2Probe6->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe7->isChecked())
			ui.comboBoxI2Probe7->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe8->isChecked())
			ui.comboBoxI2Probe8->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe9->isChecked())
			ui.comboBoxI2Probe9->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe10->isChecked())
			ui.comboBoxI2Probe0->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
	}
	else if (ui.checkBoxProbe11_20->isChecked())
	{
		if (ui.checkBoxProbe11->isChecked())
			ui.comboBoxI2Probe1->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe12->isChecked())
			ui.comboBoxI2Probe2->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe13->isChecked())
			ui.comboBoxI2Probe3->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe14->isChecked())
			ui.comboBoxI2Probe4->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe15->isChecked())
			ui.comboBoxI2Probe5->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe16->isChecked())
			ui.comboBoxI2Probe6->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe17->isChecked())
			ui.comboBoxI2Probe7->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe18->isChecked())
			ui.comboBoxI2Probe8->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe19->isChecked())
			ui.comboBoxI2Probe9->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe20->isChecked())
			ui.comboBoxI2Probe0->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
	}
	else if (ui.checkBoxProbe21_30->isChecked())
	{
		if (ui.checkBoxProbe21->isChecked())
			ui.comboBoxI2Probe1->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe22->isChecked())
			ui.comboBoxI2Probe2->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe23->isChecked())
			ui.comboBoxI2Probe3->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe24->isChecked())
			ui.comboBoxI2Probe4->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe25->isChecked())
			ui.comboBoxI2Probe5->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe26->isChecked())
			ui.comboBoxI2Probe6->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe27->isChecked())
			ui.comboBoxI2Probe7->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe28->isChecked())
			ui.comboBoxI2Probe8->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe29->isChecked())
			ui.comboBoxI2Probe9->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
		if (ui.checkBoxProbe30->isChecked())
			ui.comboBoxI2Probe0->setCurrentIndex(ui.comboBoxAnyI2->currentIndex());
	}
}
