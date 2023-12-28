/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 日志查看
* Filename: LogViewWidget.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#ifndef CLOGVIEWWIDGET_H
#define CLOGVIEWWIDGET_H


#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CLogViewWidget :public QWidget
{
	Q_OBJECT
public:
	CLogViewWidget(QWidget *parent = NULL);
	~CLogViewWidget(void);
public:
	void retranslate(void);
private:
	void InitLogViewWidget(void);

public:
	QVBoxLayout *m_pMainLayout;
	QGroupBox *m_pGroup;
	QVBoxLayout *m_pGroupLayout;
	QPushButton *m_pbVersionLog;
	QPushButton *m_pbEventLog;
	QPushButton *m_pbErrorLog;
};


#endif //CLOGVIEWWIDGET_H