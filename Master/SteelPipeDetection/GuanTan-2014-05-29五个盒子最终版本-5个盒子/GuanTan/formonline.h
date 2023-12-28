#ifndef FORMONLINE_H
#define FORMONLINE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class FormOnLine : public QDialog
{
	Q_OBJECT

public:
	FormOnLine(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormOnLine();

public:
	Ui::FormOnLineClass ui;
private slots:
	void Yes();
	void No();
	void TestOutPut();//限定函数
public:
	bool m_bSingleOut;		//单一报表输出
	bool m_bWholeOut;		//整体报表输出
	bool m_bTestOut;		//检测界面输出
	bool m_bQualified;		//合格品
	bool m_bDefect;			//缺陷品
	bool m_bBadCouple;		//耦合不良品
	bool m_bCur;			//当前窗口状态
};

#endif // FORMONLINE_H
