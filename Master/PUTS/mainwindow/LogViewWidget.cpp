/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 日志查看
* Filename: LogViewWidget.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#include "LogViewWidget.h"

CLogViewWidget::CLogViewWidget(QWidget *parent):QWidget(parent)
{
	InitLogViewWidget();
	retranslate();
}

CLogViewWidget::~CLogViewWidget(void)
{

}

void CLogViewWidget::InitLogViewWidget(void)
{

	setStyleSheet("CLogViewWidget{background-color:rgb(73,73,73)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
				  QPushButton{background-color:rgb(73,73,73);font-size:20px;color:rgb(255,255,255);border-color:rgb(255,255,255);border-width:1px; border-style:solid}\
				  QPushButton::pressed{border-color:rgb(255,255,0);color:rgb(255,255,0)}");
	resize(600, 800);
	m_pMainLayout = new QVBoxLayout(this);
	m_pMainLayout->setSpacing(6);
	m_pMainLayout->setContentsMargins(11, 11, 11, 11);

	m_pGroup = new QGroupBox(this);

	m_pGroupLayout = new QVBoxLayout(m_pGroup);
	m_pGroupLayout->setSpacing(6);
	m_pGroupLayout->setContentsMargins(11, 11, 11, 11);

	m_pbVersionLog = new QPushButton(m_pGroup);
	m_pbVersionLog->setMinimumSize(QSize(461, 51));
	m_pGroupLayout->addWidget(m_pbVersionLog);

	m_pbEventLog = new QPushButton(m_pGroup);
	m_pbEventLog->setMinimumSize(QSize(461, 51));
	m_pGroupLayout->addWidget(m_pbEventLog);

	m_pbErrorLog = new QPushButton(m_pGroup);
	m_pbErrorLog->setMinimumSize(QSize(461, 51));
	m_pGroupLayout->addWidget(m_pbErrorLog);

	m_pMainLayout->addWidget(m_pGroup);

}


void CLogViewWidget::retranslate()
{
	m_pGroup->setTitle(tr("日志查看"));
	m_pbVersionLog->setText(tr("检测版本日志查看"));
	m_pbEventLog->setText(tr("事件日志查看"));
	m_pbErrorLog->setText(tr("系统错误日志查看"));
}
