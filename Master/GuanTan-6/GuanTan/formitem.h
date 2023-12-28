#ifndef FORMITEM_H
#define FORMITEM_H

#include <QtGui/QDialog>
#include <QMouseEvent>
#include "ui_Parameter.h"
#include "formonline.h"
#include "formoffline.h"

class FormItem : public QDialog
{
	Q_OBJECT

public:
	FormItem(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormItem();
private:
	void UiInit();
public:
	Ui::FormItemClass ui;
public:
	FormOnLine m_FormOnline;
	FormOffLine m_FormOffLine;
private slots:
	void OnLine();
	void OffLine();
	void Yes();
	void No();
public:
	int m_nTestStandard;
};

#endif // FORMITEM_H
