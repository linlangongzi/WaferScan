#include "formitem.h"
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

FormItem::FormItem(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	UiInit();
}

FormItem::~FormItem()
{

}

void FormItem::UiInit()
{
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/tanshangliebiao_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("报表设置"));
	m_nTestStandard = 0;
	connect(ui.pushButtonYes,SIGNAL(clicked()),this,SLOT(Yes()));
	connect(ui.pushButtonNo,SIGNAL(clicked()),this,SLOT(No()));
	connect(ui.radioButtonOnLine,SIGNAL(clicked()),this,SLOT(OnLine()));
	connect(ui.radioButtonOffLine,SIGNAL(clicked()),this,SLOT(OffLine()));

	QRegExp RegName0_6("^.{0,15}$");	//报表设置-操作工/班次-输入框: 只允许汉字
	ui.comboBoxOperator->setValidator(new QRegExpValidator(RegName0_6,ui.comboBoxOperator));
	ui.comboBoxClasse->setValidator(new QRegExpValidator(RegName0_6,ui.comboBoxClasse));

	QRegExp RegStandardRank("^[0-9A-Za-z \-/]{0,25}$");	//检测标准,检测级别限定输入
	ui.comboBoxTestStandard->setValidator(new QRegExpValidator(RegStandardRank,ui.comboBoxTestStandard));
	ui.comboBoxTestRank->setValidator(new QRegExpValidator(RegStandardRank,ui.comboBoxTestRank));

	QRegExp RegSteelNumber("^[1-9][0-9]{0,4}$");	//检测标准,检测级别限定输入
	ui.lineEditNumber->setValidator(new QRegExpValidator(RegSteelNumber,this));
}

void FormItem::Yes()
{
	close();
	if (m_FormOnline.m_bCur)
	{
		if (m_FormOnline.ui.checkBoxSingle->isChecked())
		{
			m_FormOnline.m_bSingleOut = 1;
		}
		else
		{
			m_FormOnline.m_bSingleOut = 0;
		}
		if (m_FormOnline.ui.checkBoxWhole->isChecked())
		{
			m_FormOnline.m_bWholeOut = 1;
		}
		else
		{
			m_FormOnline.m_bWholeOut = 0;
		}
		if (m_FormOnline.ui.checkBoxTestOutPut->isChecked())
		{
			m_FormOnline.m_bTestOut = 1;
		}
		else
		{
			m_FormOnline.m_bTestOut = 0;
		}
		if (m_FormOnline.ui.checkBoxQualified->isChecked())
		{
			m_FormOnline.m_bQualified = 1;
		}
		else
		{
			m_FormOnline.m_bQualified = 0;
		}
		if (m_FormOnline.ui.checkBoxDefect->isChecked())
		{
			m_FormOnline.m_bDefect = 1;
		}
		else
		{
			m_FormOnline.m_bDefect = 0;
		}
		if (m_FormOnline.ui.checkBoxBadCouple->isChecked())
		{
			m_FormOnline.m_bBadCouple = 1;
		}
		else
		{
			m_FormOnline.m_bBadCouple = 0;
		}
	}
	if (m_FormOffLine.m_bCur)
	{
		if (m_FormOffLine.ui.checkBoxQualified->isChecked())
		{
			m_FormOffLine.m_bQualified = 1;
		}
		else
		{
			m_FormOffLine.m_bQualified = 0;
		}
		if (m_FormOffLine.ui.checkBoxFeature->isChecked())
		{
			m_FormOffLine.m_bDefect = 1;
		}
		else
		{
			m_FormOffLine.m_bDefect = 0;
		}
		if (m_FormOffLine.ui.checkBoxBadCouple->isChecked())
		{
			m_FormOffLine.m_bBadCouple = 1;
		}
		else
		{
			m_FormOffLine.m_bBadCouple = 0;
		}
		if (m_FormOffLine.ui.checkBoxNumberBegin->isChecked())
		{
			m_FormOffLine.m_bSteelBegin = 1;
		}
		else
		{
			m_FormOffLine.m_bSteelBegin = 0;
		}
		if (m_FormOffLine.ui.checkBoxNumberEnd->isChecked())
		{
			m_FormOffLine.m_bSteelEnd = 1;
		}
		else
		{
			m_FormOffLine.m_bSteelEnd = 0;
		}
	}
}

void FormItem::No()
{
	close();
}

void FormItem::OnLine()
{
	if (m_FormOnline.m_bSingleOut)
	{
		m_FormOnline.ui.checkBoxSingle->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxSingle->setChecked(false);
	}
	if (m_FormOnline.m_bWholeOut)
	{
		m_FormOnline.ui.checkBoxWhole->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxWhole->setChecked(false);
	}
	if (m_FormOnline.m_bTestOut)
	{
		m_FormOnline.ui.checkBoxTestOutPut->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxTestOutPut->setChecked(false);
	}
	if (m_FormOnline.m_bQualified)
	{
		m_FormOnline.ui.checkBoxQualified->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxQualified->setChecked(false);
	}
	if (m_FormOnline.m_bDefect)
	{
		m_FormOnline.ui.checkBoxDefect->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxDefect->setChecked(false);
	}
	if (m_FormOnline.m_bBadCouple)
	{
		m_FormOnline.ui.checkBoxBadCouple->setChecked(true);
	}
	else
	{
		m_FormOnline.ui.checkBoxBadCouple->setChecked(false);
	}
	m_FormOnline.show();
}

void FormItem::OffLine()
{
	if (m_FormOffLine.m_bQualified)
	{
		m_FormOffLine.ui.checkBoxQualified->setChecked(true);
	}
	else
	{
		m_FormOffLine.ui.checkBoxQualified->setChecked(false);
	}
	if (m_FormOffLine.m_bDefect)
	{
		m_FormOffLine.ui.checkBoxFeature->setChecked(true);
	}
	else
	{
		m_FormOffLine.ui.checkBoxFeature->setChecked(false);
	}
	if (m_FormOffLine.m_bBadCouple)
	{
		m_FormOffLine.ui.checkBoxBadCouple->setChecked(true);
	}
	else
	{
		m_FormOffLine.ui.checkBoxBadCouple->setChecked(false);
	}
	if (m_FormOffLine.m_bSteelBegin)
	{
		m_FormOffLine.ui.checkBoxNumberBegin->setChecked(true);
	}
	else
	{
		m_FormOffLine.ui.checkBoxNumberBegin->setChecked(false);
	}
	if (m_FormOffLine.m_bSteelEnd)
	{
		m_FormOffLine.ui.checkBoxNumberEnd->setChecked(true);
	}
	else
	{
		m_FormOffLine.ui.checkBoxNumberEnd->setChecked(false);
	}
	m_FormOffLine.show();
}
