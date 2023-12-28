/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 钢板信息参数显示界面
* Filename: PlateInfoPanel.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-17
*/

#ifndef PLATEINFO_H
#define PLATEINFO_H

#include <QDockWidget>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QString>
#include <QTranslator>
#include <QWidget>



class CPlateInfoPanel : public QDockWidget
{
	Q_OBJECT

public:
	CPlateInfoPanel(QWidget *parent = NULL);
	~CPlateInfoPanel();
private:
	void InitDialog(void);
	void retranslate(void);
	void changeEvent(QEvent *e);

public slots:
	void OnIsRadioErp(bool IsChecked); //判断输入方式
	void UpdatePointInformation(QString text); //更新鼠标悬浮伤点信息

	void OnRadioAtoPrint(bool IsChecked);//chenyuzhu add on 2014.5.22

public:
	QVBoxLayout *m_pverticalLayout;     //垂直布局
	QGridLayout *m_pgridLayout;  
	QHBoxLayout *m_phorizontalLayout;
	QHBoxLayout *m_phlLayout;
	QVBoxLayout *m_pMainLayout;			//垂直布局
	QWidget *m_pTopWidget;				//上半部分的Widget
	QRadioButton *m_prbERP;				//ERP标示
	QRadioButton *m_prbMInput;			//手动输入标示
  	QLabel *m_plTName;                  //时间
	QLineEdit *m_plineEdit;             //工艺名称  
	QLabel *m_plDStandard;             
	QLineEdit *m_pleDStandard;			//探伤标准
	QLabel *m_plDGrade;
	QLineEdit *m_pleDGrade;				//探伤等级
	QLabel *m_plSNumber;
	QLineEdit *m_pleSNumber;			//钢板号
	QLabel *m_plMM;   
	QLabel *m_plLength;
	QLineEdit *m_pleLength;				//长度
	QLabel *m_plWidth;
	QLineEdit *m_pleWidth;              //宽度
	QLabel *m_plThickness;
	QLineEdit *m_pleThickness;          //厚度
	QCheckBox *m_pcbAtoPrint;           //自动打印
	QLabel *m_plBInfo;
	QTextEdit *m_ptextEdit;             //备注
	bool m_bChinese;
	QTranslator *m_pTranslator;         //国际化对象
	QWidget *m_pCenWidget;              //布局用的Widget
	QSpacerItem *m_phorizontalSpacer; 
};

#endif // PLATEINFO_H
