#include "formoffline.h"

FormOffLine::FormOffLine(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.pushButtonYes,SIGNAL(clicked()),this,SLOT(Yes()));
	connect(ui.pushButtonNo,SIGNAL(clicked()),this,SLOT(No()));
	m_bQualified = 0;
	m_bDefect = 0;
	m_bBadCouple = 0;
	m_bSteelBegin = 0;
	m_bSteelEnd = 0;
	m_bCur = 0;
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/tanshangliebiao_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("¿Îœﬂ¥Ú”°"));
}

FormOffLine::~FormOffLine()
{

}
void FormOffLine::Yes()
{
	close();
	m_bCur = 1;
}
void FormOffLine::No()
{
	close();
	m_bCur = 0;
}
