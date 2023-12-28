#include "formonline.h"

FormOnLine::FormOnLine(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.pushButtonYes,SIGNAL(clicked()),this,SLOT(Yes()));
	connect(ui.pushButtonNo,SIGNAL(clicked()),this,SLOT(No()));
	connect(ui.checkBoxTestOutPut,SIGNAL(clicked()),this,SLOT(TestOutPut()));
	ui.checkBoxQualified->setEnabled(false);//UI可选择状态锁定
	ui.checkBoxDefect->setEnabled(false);
	ui.checkBoxBadCouple->setEnabled(false);
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/tanshangliebiao_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("在线打印"));
	m_bSingleOut = 0;		//单一报表输出
	m_bWholeOut = 0;		//整体报表输出
	m_bTestOut = 0;			//检测界面输出
	m_bQualified = 0;		//合格品
	m_bDefect = 0;			//缺陷品
	m_bBadCouple = 0;		//耦合不良品
	m_bCur = 0;				//当前窗口状态
}

FormOnLine::~FormOnLine()
{

}

void FormOnLine::Yes()
{
	m_bCur = 1;
	close();
}
void FormOnLine::No()
{
	m_bCur = 0;
	close();
}
void FormOnLine::TestOutPut()//限定函数
{
	if (ui.checkBoxTestOutPut->isChecked())
	{
		ui.checkBoxQualified->setEnabled(true);
		ui.checkBoxDefect->setEnabled(true);
		ui.checkBoxBadCouple->setEnabled(true);
	}
	else
	{
		ui.checkBoxQualified->setChecked(false);
		ui.checkBoxDefect->setChecked(false);
		ui.checkBoxBadCouple->setChecked(false);
		m_bQualified = 0;
		m_bDefect = 0;
		m_bBadCouple = 0;
		ui.checkBoxQualified->setEnabled(false);
		ui.checkBoxDefect->setEnabled(false);
		ui.checkBoxBadCouple->setEnabled(false);
	}
}
