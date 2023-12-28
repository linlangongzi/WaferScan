#ifndef FORMOFFLINE_H
#define FORMOFFLINE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class FormOffLine : public QDialog
{
	Q_OBJECT

public:
	FormOffLine(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormOffLine();

public:
	Ui::FormOffLineClass ui;
private slots:
	void Yes();
	void No();
public:
	bool m_bQualified;
	bool m_bDefect;
	bool m_bBadCouple;
	bool m_bSteelBegin;
	bool m_bSteelEnd;
	bool m_bCur;
};

#endif // FORMOFFLINE_H
