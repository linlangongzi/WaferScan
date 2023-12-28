/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 伤点控制详细面板
* Filename: InjPointControl.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/
#include <QMessageBox>
#include "InjPointControl.h"

extern bool g_DepthFilterSwitch;	// 深度过滤全局开关

CInjPointControl::CInjPointControl(QWidget* parent):QWidget(parent)
{
	InitInjPointControl();
	retranslate();

	connect(m_pWondFilterCheck, SIGNAL(clicked()), this, SLOT(WondFilterCheckEn()));
	connect(m_pDepthFilterCheck, SIGNAL(clicked()), this, SLOT(DepthFilterCheckEn()));
}

CInjPointControl::~CInjPointControl(void)
{
}


void CInjPointControl::InitInjPointControl(void)
{
	setStyleSheet("CInjPointControl{background-color:rgb(73,73,73)}\
				   QGroupBox{border-width:1px; border-style:solid;border-color:rgb(255, 255, 255);font-size:20px;color:rgb(255,255,255)}\
				   QLabel{font-size:20px;color:rgb(255,255,255)}\
				   QCheckBox{font-size:20px;color:rgb(255,255,255)}\
				   QLineEdit{font-size:20px;color:rgb(255,255,0)}");
    
	resize(600, 800);
	m_pMainVlayout = new QVBoxLayout(this);
	m_pMainVlayout->setSpacing(6);
	m_pMainVlayout->setContentsMargins(9, 9, 9, 9);

	m_pgbUp = new QGroupBox(this);

	m_pUpGLayout = new QGridLayout(m_pgbUp);
	m_pUpGLayout->setSpacing(6);
	m_pUpGLayout->setContentsMargins(9, 25, 9, 9);

	m_pTopCheck = new QCheckBox(m_pgbUp);
	m_pUpGLayout->addWidget(m_pTopCheck, 0, 0, 1, 1);

	m_pBottonCheck = new QCheckBox(m_pgbUp);
	m_pUpGLayout->addWidget(m_pBottonCheck, 0, 1, 1, 1);

	m_pLeftCheck = new QCheckBox(m_pgbUp);
	m_pUpGLayout->addWidget(m_pLeftCheck, 1, 0, 1, 1);

	m_pRightCheck = new QCheckBox(m_pgbUp);
	m_pUpGLayout->addWidget(m_pRightCheck, 1, 1, 1, 1);
///////////////////////////////////////////////////////////////////////////////////////////////////

	m_pCThicknessView  = new QCheckBox(m_pgbUp);       //厚度图
	m_pUpGLayout->addWidget(m_pCThicknessView, 2, 0, 1, 1);

	m_pCFrontView = new QCheckBox(m_pgbUp);           //正视图
	m_pUpGLayout->addWidget(m_pCFrontView, 2, 1, 1, 1);

	m_pCSideView = new QCheckBox(m_pgbUp);            //侧视图
	m_pUpGLayout->addWidget(m_pCSideView, 3, 0, 1, 1);

	m_pCOverView =new QCheckBox(m_pgbUp);            //总体视图
	m_pUpGLayout->addWidget(m_pCOverView, 3, 1, 1, 1);

///////////////////////////////////////////////////////////////////////////////////////////////////

	m_pMainVlayout->addWidget(m_pgbUp);

	m_pgbDown = new QGroupBox(this);

	m_pDownGLayout = new QGridLayout(m_pgbDown);
	m_pDownGLayout->setSpacing(6);
	m_pDownGLayout->setContentsMargins(9, 25, 9, 9);

	m_pReticleCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pReticleCheck, 0, 0, 1, 1);

	m_pPointRangeCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pPointRangeCheck, 0, 1, 1, 1);

	m_pWoundPathCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pWoundPathCheck, 1, 0, 1, 1);

	m_pWondFilterCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pWondFilterCheck, 2, 0, 1, 1);

	m_pgbWoundFilter = new QGroupBox(m_pgbDown);

	m_pInjPintFilterGLayout = new QGridLayout(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->setSpacing(6);
	m_pInjPintFilterGLayout->setContentsMargins(9, 9, 200, 9);

	m_pMM = new QLabel(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pMM, 1, 2, 1, 1);

	m_pWFBigerCheck = new QCheckBox(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pWFBigerCheck, 0, 0, 1, 1);

	m_pleWFBigger = new QLineEdit(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pleWFBigger, 0, 1, 1, 1);

	m_pleWFSmaller = new QLineEdit(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pleWFSmaller, 1, 1, 1, 1);

	m_pMM2 = new QLabel(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pMM2, 0, 2, 1, 1);

	m_pWFSmallerCheck = new QCheckBox(m_pgbWoundFilter);
	m_pInjPintFilterGLayout->addWidget(m_pWFSmallerCheck, 1, 0, 1, 1);

	// 深度过滤相关控件 
	m_pDepthFilterCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pDepthFilterCheck, 4, 0, 1, 1);

	m_pDepthGroupBox = new QGroupBox(m_pgbDown);
	
	m_pDepthFilterGLayout = new QGridLayout(m_pDepthGroupBox);
	m_pDepthFilterGLayout->setSpacing(6);
	m_pDepthFilterGLayout->setContentsMargins(9, 9, 9, 9);

	// 头端
	m_pHeadDepthFilter = new QLabel(m_pDepthGroupBox);
	m_pHeadDepthFilter->setText("头端(mm)");
	m_pDepthFilterGLayout->addWidget(m_pHeadDepthFilter, 0, 1, 1, 1);

	m_HeadDepthFilterBox = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_HeadDepthFilterBox, 0, 2, 1, 1);

	m_pHeadDepthFilterSpace = new QLabel(m_pDepthGroupBox);
	m_pHeadDepthFilterSpace->setText("至");
	m_pDepthFilterGLayout->addWidget(m_pHeadDepthFilterSpace, 0, 3, 1, 1);

	m_HeadDepthFilterBox2 = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_HeadDepthFilterBox2, 0, 4, 1, 2);

	// 尾端
	m_pEndDepthFilter = new QLabel(m_pDepthGroupBox);
	m_pEndDepthFilter->setText("尾端(mm)");
	m_pDepthFilterGLayout->addWidget(m_pEndDepthFilter, 1, 1, 1, 1);

	m_EndDepthFilterBox = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_EndDepthFilterBox, 1, 2, 1, 1);

	m_pEndDepthFilterSpace = new QLabel(m_pDepthGroupBox);
	m_pEndDepthFilterSpace->setText("至");
	m_pDepthFilterGLayout->addWidget(m_pEndDepthFilterSpace, 1, 3, 1, 1);

	m_EndDepthFilterBox2 = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_EndDepthFilterBox2, 1, 4, 1, 2);

	// 边缘
	m_pEdgeDepthFilter = new QLabel(m_pDepthGroupBox);
	m_pEdgeDepthFilter->setText("边缘(mm)");
	m_pDepthFilterGLayout->addWidget(m_pEdgeDepthFilter, 2, 1, 1, 1);

	m_EdgeDepthFilterBox = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_EdgeDepthFilterBox, 2, 2, 1, 1);

	m_pEdgeDepthFilterSpace = new QLabel(m_pDepthGroupBox);
	m_pEdgeDepthFilterSpace->setText("至");
	m_pDepthFilterGLayout->addWidget(m_pEdgeDepthFilterSpace, 2, 3, 1, 1);

	m_EdgeDepthFilterBox2 = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_EdgeDepthFilterBox2, 2, 4, 1, 2);

	// 本体
	m_pBodyDepthFilter = new QLabel(m_pDepthGroupBox);
	m_pBodyDepthFilter->setText("本体(mm)");
	m_pDepthFilterGLayout->addWidget(m_pBodyDepthFilter, 3, 1, 1, 1);

	m_BodyDepthFilterBox = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_BodyDepthFilterBox, 3, 2, 1, 1);

	m_pBodyDepthFilterSpace = new QLabel(m_pDepthGroupBox);
	m_pBodyDepthFilterSpace->setText("至");
	m_pDepthFilterGLayout->addWidget(m_pBodyDepthFilterSpace, 3, 3, 1, 1);

	m_BodyDepthFilterBox2 = new QLineEdit(m_pDepthGroupBox);
	m_pDepthFilterGLayout->addWidget(m_BodyDepthFilterBox2, 3, 4, 1, 2);

	m_pDepthGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pDownGLayout->addWidget(m_pDepthGroupBox, 5, 0, 1, 2);	
	// 深度过滤相关结束

	m_pDownGLayout->addWidget(m_pgbWoundFilter, 3, 0, 1, 2);

	m_pDefectDistantCheck = new QCheckBox(m_pgbDown);
	m_pDownGLayout->addWidget(m_pDefectDistantCheck, 6, 0, 1, 1);

	m_pgbDefectDistant = new QGroupBox(m_pgbDown);

	m_pNextDistantHLayout = new QHBoxLayout(m_pgbDefectDistant);
	m_pNextDistantHLayout->setSpacing(6);
	m_pNextDistantHLayout->setContentsMargins(9, 9, 9, 9);

	m_pleDDSmaller = new QLabel(m_pgbDefectDistant);
	m_pNextDistantHLayout->addWidget(m_pleDDSmaller);

	m_pleNextDistant = new QLineEdit(m_pgbDefectDistant);
	m_pNextDistantHLayout->addWidget(m_pleNextDistant);

	m_pDownGLayout->addWidget(m_pgbDefectDistant, 7, 0, 1, 2);

	m_pMainVlayout->addWidget(m_pgbDown);

	//chenyuzhu add start on 2014.5.6
	m_pWFBigerCheck->setEnabled(false);            
	m_pWFSmallerCheck->setEnabled(false);
	m_pleWFBigger->setEnabled(false);
	m_pleWFSmaller->setEnabled(false);
	//chenyuzhu add end on 2014.5.6

}

void CInjPointControl::retranslate(void)
{
	m_pgbUp->setTitle(tr("伤点视图"));
	m_pTopCheck->setText(tr("上侧边缘伤点视图"));
	m_pBottonCheck->setText(tr("下侧边缘伤点视图"));
	m_pLeftCheck->setText(tr("左侧边缘伤点视图"));
	m_pRightCheck->setText(tr("右侧边缘伤点视图"));
	m_pgbDown->setTitle(tr("伤点显示"));
	m_pReticleCheck->setText(tr("显示中心十字线"));
	m_pPointRangeCheck->setText(tr("显示伤点范围线"));
	m_pWoundPathCheck->setText(tr("显示伤点路径"));
	m_pWondFilterCheck->setText(tr("伤点过滤"));
	m_pDepthFilterCheck->setText(tr("深度过滤"));
	m_pgbWoundFilter->setTitle(QString());
	m_pMM->setText("平方厘米");//chenyuzhu modify on 2014-10-31
	m_pWFBigerCheck->setText(tr("大于"));
	m_pMM2->setText("平方厘米");//chenyuzhu modify on 2014-10-31
	m_pWFSmallerCheck->setText(tr("小于"));
	m_pDefectDistantCheck->setText(tr("相邻缺陷距离"));
	m_pgbDefectDistant->setTitle(QString());
	m_pleDDSmaller->setText(tr("小于"));
////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pCThicknessView->setText(tr("厚度图"));    //厚度图
	m_pCFrontView->setText(tr("正视图"));        //正视图
	m_pCSideView->setText(tr("侧视图"));          //侧视图
	m_pCOverView->setText(tr("总体视图"));          //总体视图
}

void CInjPointControl::WondFilterCheckEn()		
{
	if (m_pWondFilterCheck->isChecked()) {	
		m_pWFBigerCheck->setEnabled(true);            
		m_pWFSmallerCheck->setEnabled(true);
		m_pleWFBigger->setEnabled(true);
		m_pleWFSmaller->setEnabled(true);
	} else {
		m_pWFBigerCheck->setEnabled(false);            
		m_pWFSmallerCheck->setEnabled(false);
		m_pleWFBigger->setEnabled(false);
		m_pleWFSmaller->setEnabled(false);
	}
}

void CInjPointControl::DepthFilterCheckEn()
{
	if (m_pDepthFilterCheck->isChecked()) {
		g_DepthFilterSwitch = TRUE;
		m_HeadDepthFilterBox->setEnabled(TRUE);
		m_EndDepthFilterBox->setEnabled(TRUE);
		m_EdgeDepthFilterBox->setEnabled(TRUE);
		m_BodyDepthFilterBox->setEnabled(TRUE);

		m_HeadDepthFilterBox2->setEnabled(TRUE);
		m_EndDepthFilterBox2->setEnabled(TRUE);
		m_EdgeDepthFilterBox2->setEnabled(TRUE);
		m_BodyDepthFilterBox2->setEnabled(TRUE);
	} else {
		g_DepthFilterSwitch = FALSE;
		m_HeadDepthFilterBox->setEnabled(FALSE);
		m_EndDepthFilterBox->setEnabled(FALSE);
		m_EdgeDepthFilterBox->setEnabled(FALSE);
		m_BodyDepthFilterBox->setEnabled(FALSE);

		m_HeadDepthFilterBox2->setEnabled(FALSE);
		m_EndDepthFilterBox2->setEnabled(FALSE);
		m_EdgeDepthFilterBox2->setEnabled(FALSE);
		m_BodyDepthFilterBox2->setEnabled(FALSE);
	}
}