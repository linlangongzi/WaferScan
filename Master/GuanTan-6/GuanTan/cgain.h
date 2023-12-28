#ifndef CGAIN_H
#define CGAIN_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class CGain : public QDialog
{
	Q_OBJECT

public:
	CGain(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CGain();

public:
	Ui::CGainClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // CGAIN_H
