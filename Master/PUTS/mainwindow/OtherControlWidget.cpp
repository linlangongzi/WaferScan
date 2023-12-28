/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 其他控制详细面板
* Filename: OtherControlWidget.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/


#include "OtherControlWidget.h"



COtherControlWidget::COtherControlWidget(QWidget *parent):QWidget(parent)
{
	InitCOtherControlWidget();
	retranslate();

}

COtherControlWidget::~COtherControlWidget(void)
{

}

void COtherControlWidget::InitCOtherControlWidget(void)
{

	setStyleSheet("COtherControlWidget{background-color:rgb(73,73,73)}\
				  QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
				  QLabel{font-size:20px;color:rgb(255,255,255)}\
				  QLineEdit{font-size:20px;color:rgb(255,255,0)}");
	resize(599, 575);
	verticalLayout_2 = new QVBoxLayout(this);
	verticalLayout_2->setSpacing(6);
	verticalLayout_2->setContentsMargins(11, 30, 11, 11);

	m_pGroup = new QGroupBox(this);
	verticalLayout = new QVBoxLayout(m_pGroup);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(11, 30, 11, 11);

	m_pGroupDatabase = new QGroupBox(m_pGroup);

	gridLayout = new QGridLayout(m_pGroupDatabase);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(40, 30, 40, 11);

	m_plAddress = new QLabel(m_pGroupDatabase);
	gridLayout->addWidget(m_plAddress, 0, 0, 1, 1);

	m_pleAddress = new QLineEdit(m_pGroupDatabase);
	m_pleAddress->setInputMask("000.000.000.000");
	m_pleAddress->setMaximumSize(QSize(160, 30));
	
	gridLayout->addWidget(m_pleAddress, 0, 1, 1, 1);

	m_plUserName = new QLabel(m_pGroupDatabase);
	gridLayout->addWidget(m_plUserName, 1, 0, 1, 1);

	m_pleUserName = new QLineEdit(m_pGroupDatabase);
	m_pleUserName->setMaximumSize(QSize(160, 30));
	gridLayout->addWidget(m_pleUserName, 1, 1, 1, 1);

	m_plPWD = new QLabel(m_pGroupDatabase);
	gridLayout->addWidget(m_plPWD, 2, 0, 1, 1);

	m_plePWD = new QLineEdit(m_pGroupDatabase);
	m_plePWD->setMaximumSize(QSize(160, 30));
	gridLayout->addWidget(m_plePWD, 2, 1, 1, 1);


	verticalLayout->addWidget(m_pGroupDatabase);
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	verticalLayout->addItem(verticalSpacer);
	verticalLayout_2->addWidget(m_pGroup);

}

void COtherControlWidget::retranslate()
{
	m_pGroup->setTitle(tr("其他控制"));
	m_pGroupDatabase->setTitle(tr("服务器设置"));
	m_plAddress->setText(tr("服务器地址"));
	m_plUserName->setText(tr("用户名"));
	m_plPWD->setText(tr("密码"));
}
