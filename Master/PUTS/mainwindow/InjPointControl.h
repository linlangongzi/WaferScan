/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 伤点控制详细面板
* Filename: InjPointControl.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#ifndef CINJPOINTCONTROL_H
#define CINJPOINTCONTROL_H


#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QDoubleSpinBox>

class CInjPointControl : public QWidget
{
	Q_OBJECT

public:
	CInjPointControl(QWidget* parent = NULL);
	~CInjPointControl(void);

private:
	void InitInjPointControl(void);
	void retranslate(void);

public:
	QVBoxLayout *m_pMainVlayout;
	QGroupBox *m_pgbUp;
	QGridLayout *m_pUpGLayout;
	QCheckBox *m_pTopCheck;             // 显示上侧
	QCheckBox *m_pBottonCheck;          // 显示下侧
	QCheckBox *m_pLeftCheck;            // 显示左侧
	QCheckBox *m_pRightCheck;           // 显示右侧

	QCheckBox *m_pCThicknessView;       // 厚度图
	QCheckBox *m_pCFrontView;           // 正视图
	QCheckBox *m_pCSideView;            // 侧视图
	QCheckBox *m_pCOverView;            // 总体视图

	QGroupBox *m_pgbDown;
	QGridLayout *m_pDownGLayout;
	QCheckBox *m_pReticleCheck;         // 显示中心十字线
	QCheckBox *m_pPointRangeCheck;      // 伤点范围线
	QCheckBox *m_pWoundPathCheck;       // 伤点路径
	QCheckBox *m_pWondFilterCheck;      // 伤点过滤
	QGroupBox *m_pgbWoundFilter;
	QGridLayout *m_pInjPintFilterGLayout;
	QLabel *m_pMM;
	QCheckBox *m_pWFBigerCheck;            
	QLineEdit *m_pleWFBigger;
	QLineEdit *m_pleWFSmaller;
	QLabel *m_pMM2;
	QCheckBox *m_pWFSmallerCheck;
	QCheckBox *m_pDefectDistantCheck;      //相邻缺陷距离
	QGroupBox *m_pgbDefectDistant;
	QHBoxLayout *m_pNextDistantHLayout;
	QLabel *m_pleDDSmaller;
	QLineEdit *m_pleNextDistant;

	QGroupBox *m_pDepthGroupBox;			// 深度过滤组
	QCheckBox *m_pDepthFilterCheck;			// 深度过滤功能总开关
	QGridLayout *m_pDepthFilterGLayout;		// 深度过滤布局

	QLabel *m_pHeadDepthFilter;
	QLabel *m_pHeadDepthFilterSpace;
	QLineEdit *m_HeadDepthFilterBox;	
	QLineEdit *m_HeadDepthFilterBox2;

	QLabel *m_pEndDepthFilter;
	QLabel *m_pEndDepthFilterSpace;
	QLineEdit *m_EndDepthFilterBox;
	QLineEdit *m_EndDepthFilterBox2;

	QLabel *m_pEdgeDepthFilter;
	QLabel *m_pEdgeDepthFilterSpace;
	QLineEdit *m_EdgeDepthFilterBox;
	QLineEdit *m_EdgeDepthFilterBox2;

	QLabel *m_pBodyDepthFilter;
	QLabel *m_pBodyDepthFilterSpace;
	QLineEdit *m_BodyDepthFilterBox;
	QLineEdit *m_BodyDepthFilterBox2;

	//chenyuzhu add start on 2014.5.6
private slots:
	void WondFilterCheckEn();			//A扫控件锁定与解锁
	void DepthFilterCheckEn();

};






#endif //CINJPOINTCONTROL_H