/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板信息参数显示界面
* Filename: PlateInfoPanel.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/


#include <QDir>
#include <QSettings>
#include "PlateInfoPanel.h"
#include "DataDefine.h"

bool g_IfAtoPrint = false;//判断自动打印是否勾选 chenyuzhu add on 2014.5.22  

CPlateInfoPanel::CPlateInfoPanel(QWidget *parent): QDockWidget(parent)
{
	InitDialog();
	retranslate();
	connect(m_prbERP,SIGNAL(toggled(bool)),this,SLOT(OnIsRadioErp(bool)));
	connect(m_pcbAtoPrint,SIGNAL(toggled(bool)),this,SLOT(OnRadioAtoPrint(bool)));//chenyuzhu add on 2014.5.22
}

CPlateInfoPanel::~CPlateInfoPanel()
{

}


void CPlateInfoPanel::InitDialog(void)
{
	
	this->setStyleSheet("QWidget{background-color: rgb(73, 73, 73)}\
						QLabel{background-color: rgb(217, 219, 246)}");
	m_pCenWidget = new QWidget(this);
	this->setWidget(m_pCenWidget);
	m_pMainLayout = new QVBoxLayout(m_pCenWidget);     //主面板垂直布局

	m_pTopWidget = new QWidget(this);
	m_pTopWidget->setStyleSheet("background-color: rgb(103,103,105)");

	m_pverticalLayout = new QVBoxLayout(m_pTopWidget);  //上半部分垂直布局
	m_pverticalLayout->setSpacing(6);
	m_pverticalLayout->setContentsMargins(11, 11, 11, 11);

	m_plTName = new QLabel(this);
	m_plTName->setStyleSheet("background-color: rgb(217, 219, 246)");
	QFont font;
	font.setPointSize(20);
	m_plTName->setFont(font);
	m_plTName->setScaledContents(false);
	m_plTName->setAlignment(Qt::AlignCenter);
	m_pverticalLayout->addWidget(m_plTName);

	m_plineEdit = new QLineEdit(this);
	m_plineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_plineEdit->setFont(font);
	m_plineEdit ->setEnabled(false);
	m_pverticalLayout->addWidget(m_plineEdit);

	m_plDStandard = new QLabel(this);
	m_plDStandard->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plDStandard->setFont(font);
	m_plDStandard->setScaledContents(false);
	m_plDStandard->setAlignment(Qt::AlignCenter);
	m_pverticalLayout->addWidget(m_plDStandard);

	m_pleDStandard = new QLineEdit(this);
	m_pleDStandard->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleDStandard->setFont(font);
	m_pleDStandard ->setEnabled(false);
	m_pverticalLayout->addWidget(m_pleDStandard);

	m_plDGrade = new QLabel(this);
	m_plDGrade->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plDGrade->setFont(font);
	m_plDGrade->setScaledContents(false);
	m_plDGrade->setAlignment(Qt::AlignCenter);

	m_pverticalLayout->addWidget(m_plDGrade);

	m_pleDGrade = new QLineEdit(this);
	m_pleDGrade->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleDGrade->setFont(font);
	m_pleDGrade ->setEnabled(false);
	m_pverticalLayout->addWidget(m_pleDGrade);

	m_plSNumber = new QLabel(this);
	m_plSNumber->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plSNumber->setFont(font);
	m_plSNumber->setScaledContents(false);
	m_plSNumber->setAlignment(Qt::AlignCenter);
	m_pverticalLayout->addWidget(m_plSNumber);

	m_pleSNumber = new QLineEdit(this);
	m_pleSNumber->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleSNumber->setFont(font);
	m_pleSNumber ->setEnabled(false);
	m_pverticalLayout->addWidget(m_pleSNumber);

    m_phlLayout = new QHBoxLayout;  //水平布局
	m_plMM = new QLabel(this);
	m_plMM->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plMM->setMaximumWidth(35);
	m_plMM->setFont(font);
	m_phorizontalSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_phlLayout->addItem(m_phorizontalSpacer);
	m_phlLayout->addWidget(m_plMM);
	m_pverticalLayout->addLayout(m_phlLayout);

	m_pgridLayout = new QGridLayout();   //网格布局
	m_pgridLayout->setSpacing(6);
	m_pgridLayout->setHorizontalSpacing(60);

	m_plLength = new QLabel(this);
	m_plLength->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plLength->setFont(font);
	m_plLength->setAlignment(Qt::AlignCenter);
	m_pgridLayout->addWidget(m_plLength, 0, 0, 1, 1);

	m_pleLength = new QLineEdit(this);
	m_pleLength->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleLength->setFont(font);
	m_pleLength ->setEnabled(false);
	m_pgridLayout->addWidget(m_pleLength, 0, 1, 1, 1);

	m_plWidth = new QLabel(this);
	m_plWidth->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plWidth->setFont(font);
	m_plWidth->setAlignment(Qt::AlignCenter);
	m_pgridLayout->addWidget(m_plWidth, 1, 0, 1, 1);

	m_pleWidth = new QLineEdit(this);
	m_pleWidth->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleWidth->setFont(font);
	m_pleWidth ->setEnabled(false);
	m_pgridLayout->addWidget(m_pleWidth, 1, 1, 1, 1);

	m_plThickness = new QLabel(this);
	m_plThickness->setStyleSheet("background-color: rgb(217, 219, 246)");
	m_plThickness->setFont(font);
	m_plThickness->setAlignment(Qt::AlignCenter);
	m_pgridLayout->addWidget(m_plThickness, 2, 0, 1, 1);

	m_pleThickness = new QLineEdit(this);
	m_pleThickness->setStyleSheet("background-color: rgb(0, 0, 0);color:rgb(255, 255, 0)");
	m_pleThickness->setFont(font);
	m_pleThickness ->setEnabled(false);

	m_pgridLayout->addWidget(m_pleThickness, 2, 1, 1, 1);
    m_pverticalLayout->addLayout(m_pgridLayout);
	m_pMainLayout->addWidget(m_pTopWidget);

	m_phorizontalLayout = new QHBoxLayout();  //水平布局
	m_phorizontalLayout->setSpacing(10);
	m_phorizontalLayout->setContentsMargins(10, 10, 10, 10);

	QFont font1;
	font1.setPointSize(15);
	m_prbERP = new QRadioButton(this);
	m_prbERP->setStyleSheet("color: rgb(255,255,255)");
	m_phorizontalLayout->addWidget(m_prbERP);
	m_prbERP->setFont(font1);

	m_prbMInput = new QRadioButton(this);
	m_prbMInput->setStyleSheet("color: rgb(255,255,255)");
	m_prbMInput->setFont(font1);
	m_phorizontalLayout->addWidget(m_prbMInput);

	m_pcbAtoPrint = new QCheckBox(this);
	m_pcbAtoPrint->setStyleSheet("color: rgb(255,255,255)");
	m_pcbAtoPrint->setFont(font1);

	m_phorizontalLayout->addWidget(m_pcbAtoPrint);
	m_pMainLayout->addLayout(m_phorizontalLayout);

	m_plBInfo = new QLabel(this);
	m_plBInfo->setFont(font);
	m_plBInfo->setScaledContents(false);
	m_plBInfo->setAlignment(Qt::AlignCenter);
	m_pMainLayout->addWidget(m_plBInfo);

	m_ptextEdit = new QTextEdit(this);
	m_ptextEdit->setStyleSheet("background-color: rgb(103,103,105);color: rgb(255,255,0)");
	font.setPointSize(10);
	m_ptextEdit -> setCurrentFont(font);
	m_ptextEdit->setReadOnly(true);
	m_pMainLayout->addWidget(m_ptextEdit);
}


void CPlateInfoPanel::retranslate(void)
{
	m_plTName->setText(tr("工艺名称"));
	m_plDStandard->setText(tr("探伤标准"));
	m_plDGrade->setText(tr("探伤级别"));
	m_plSNumber->setText(tr("钢板号"));
	m_plMM->setText("mm");
	m_plLength->setText(tr("长度"));
	m_plWidth->setText(tr("宽度"));
	m_plThickness->setText(tr("厚度"));
	m_prbERP->setText(tr("ERP"));
	m_prbMInput->setText(tr("手动输入"));
	m_pcbAtoPrint->setText(tr("自动打印"));
	m_plBInfo->setText(tr("伤点信息"));
} 


//用来接受语言切换事件
void CPlateInfoPanel::changeEvent(QEvent *e)
{
	QDockWidget::changeEvent(e);  
	switch (e->type())  
	{  
	case QEvent::LanguageChange:  
		this ->retranslate();
		break;  
	default:  
		break;  
	}
}
void CPlateInfoPanel::UpdatePointInformation(QString text) //更新鼠标悬浮伤点信息
{
	m_ptextEdit->clear();
	//chenyuzhu add start on 2014-10-30
	QFont font = QFont("Times", 15, QFont::Bold);
	m_ptextEdit->setCurrentFont(font);
	//chenyuzhu add start on 2014-10-30
	m_ptextEdit->setText(text);
}


void CPlateInfoPanel::OnIsRadioErp(bool IsChecked)
{  
	if(IsChecked) {
 		QString fileName = QDir::currentPath() += "/PUTS.ini";
		QSettings settings(fileName, QSettings::IniFormat);
		settings.setValue("PlateInfo/InputMode", "1");

	} else {
		QString fileName = QDir::currentPath() += "/PUTS.ini";
		QSettings settings(fileName, QSettings::IniFormat);
		settings.setValue("PlateInfo/InputMode", "0");
	}
}

//chenyuzhu add start on 2014.5.22
void CPlateInfoPanel::OnRadioAtoPrint(bool IsChecked)
{  
	if(IsChecked)
	{
		g_IfAtoPrint = true;

	}else
	{
		g_IfAtoPrint = false;
	}
}
//chenyuzhu add end on 2014.5.22