#include "formonline.h"

FormOnLine::FormOnLine(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	connect(ui.pushButtonYes,SIGNAL(clicked()),this,SLOT(Yes()));
	connect(ui.pushButtonNo,SIGNAL(clicked()),this,SLOT(No()));
	connect(ui.checkBoxTestOutPut,SIGNAL(clicked()),this,SLOT(TestOutPut()));
	ui.checkBoxQualified->setEnabled(false);//UI��ѡ��״̬����
	ui.checkBoxDefect->setEnabled(false);
	ui.checkBoxBadCouple->setEnabled(false);
	QIcon icon;
	icon.addFile(QString::fromUtf8("./Resources/tanshangliebiao_select.png"), QSize(), QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);
	this->setWindowTitle(tr("���ߴ�ӡ"));
	m_bSingleOut = 0;		//��һ�������
	m_bWholeOut = 0;		//���屨�����
	m_bTestOut = 0;			//���������
	m_bQualified = 0;		//�ϸ�Ʒ
	m_bDefect = 0;			//ȱ��Ʒ
	m_bBadCouple = 0;		//��ϲ���Ʒ
	m_bCur = 0;				//��ǰ����״̬
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
void FormOnLine::TestOutPut()//�޶�����
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
