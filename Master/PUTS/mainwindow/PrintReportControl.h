/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
*打印报表控制
* Filename: PrintReportControl.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#ifndef PRINTREPORTCONTROL_H
#define PRINTREPORTCONTROL_H



#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QWidget>


class CPrintReportControl :public QWidget
{
	Q_OBJECT
public:
	CPrintReportControl(QWidget* parent = NULL);
	~CPrintReportControl(void);
	
private:
	void InitCPrintReportControl(void);
	void retranslate(void);

public:
	QHBoxLayout *horizontalLayout;
	QGroupBox *m_pGroup;
	QGridLayout *gridLayout;
	QPushButton *m_pbPreview;
	QCheckBox *m_pCDependOnMInput;
	QPushButton *m_pPrintReport;
	QCheckBox *m_pCPrintPointPicture;
	QCheckBox *m_pEnglishReport;
	QCheckBox *m_pJieguoInfo;

private slots:
	void OnJieguoInfo(int bValue);

};




#endif // PRINTREPORTCONTROL_H
