/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
*打印报表控制
* Filename: PrintReportControl.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#include "PrintReportControl.h"

extern bool g_bJieguo;

CPrintReportControl::CPrintReportControl(QWidget* parent):QWidget(parent)
{
	InitCPrintReportControl();
	retranslate();
	connect(m_pJieguoInfo, SIGNAL(stateChanged(int)), this, SLOT(OnJieguoInfo(int)));
}

CPrintReportControl::~CPrintReportControl(void)
{


}

void CPrintReportControl::InitCPrintReportControl(void)
{

	setStyleSheet("CPrintReportControl{background-color:rgb(73,73,73)}\
				  QCheckBox{font-size:20px;color:rgb(255,255,255)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);border-color:rgb(255,255,255);border-width:1px; border-style:solid}\
				  QPushButton::pressed{border-color:rgb(255,255,0);color:rgb(255,255,0)}");

    resize(654, 579);
	horizontalLayout = new QHBoxLayout(this);
	horizontalLayout->setSpacing(6);
	horizontalLayout->setContentsMargins(11, 11, 11, 11);
	
	m_pGroup = new QGroupBox(this);

	gridLayout = new QGridLayout(m_pGroup);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);

	m_pbPreview = new QPushButton(m_pGroup);
	m_pbPreview->setMinimumSize(QSize(461, 51));
	gridLayout->addWidget(m_pbPreview, 0, 0, 1, 2);

	m_pCDependOnMInput = new QCheckBox(m_pGroup);
	gridLayout->addWidget(m_pCDependOnMInput, 2, 0, 1, 1);

	m_pPrintReport = new QPushButton(m_pGroup);
	m_pPrintReport->setMinimumSize(QSize(461, 51));
	gridLayout->addWidget(m_pPrintReport, 1, 0, 1, 2);

	m_pCPrintPointPicture = new QCheckBox(m_pGroup);
	gridLayout->addWidget(m_pCPrintPointPicture, 2, 1, 1, 1);

	m_pEnglishReport = new QCheckBox(m_pGroup);
	gridLayout->addWidget(m_pEnglishReport, 3, 0, 1, 1);

	m_pJieguoInfo = new QCheckBox(m_pGroup);
	gridLayout->addWidget(m_pJieguoInfo, 3, 1, 1, 1);

	horizontalLayout->addWidget(m_pGroup);
}

void CPrintReportControl::retranslate()
{
	m_pGroup->setTitle(tr("打印报表控制"));
	m_pbPreview->setText(tr("预览"));
	m_pCDependOnMInput->setText(tr("人工输入数据为准"));
	m_pPrintReport->setText(tr("打印版级报表"));
	m_pCPrintPointPicture->setText(tr("打印伤点详细图"));
	m_pEnglishReport->setText(tr("英文报表"));
	m_pJieguoInfo->setText((tr("打印探伤结果参数")));
} 

void CPrintReportControl::OnJieguoInfo(int bValue)
{
	g_bJieguo = bValue;
}