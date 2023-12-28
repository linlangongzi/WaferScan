/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 其他控制详细面板
* Filename: OtherControlWidget.h
*
* @author        Dong Qi
* @version       1.00         Date: 2013-05-30
*/

#ifndef COTHERCONTROLWIDGET_H
#define COTHERCONTROLWIDGET_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>



class COtherControlWidget :public QWidget
{
	Q_OBJECT
public:
	COtherControlWidget(QWidget *parent = NULL);
	~COtherControlWidget(void);

private:
	void InitCOtherControlWidget(void);
	void retranslate(void);

public:
	QVBoxLayout *verticalLayout_2;
	QGroupBox *m_pGroup;
	QVBoxLayout *verticalLayout;
	QGroupBox *m_pGroupDatabase;
	QGridLayout *gridLayout;
	QLabel *m_plAddress;
	QLineEdit *m_pleAddress;
	QLabel *m_plUserName;
	QLineEdit *m_pleUserName;
	QLabel *m_plPWD;
	QLineEdit *m_plePWD;
	QSpacerItem *verticalSpacer;
};


#endif //COTHERCONTROLWIDGET_H