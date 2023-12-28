#ifndef GATESET_H
#define GATESET_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class GateSet : public QDialog
{
	Q_OBJECT

public:
	GateSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GateSet();

public:
	Ui::GateSetClass ui;
private:
	void UiInit();
private slots:
	void Check1_10();
	void Check11_20();
	void Check21_30();
 	void SelectAll();
 	void SelectAny();

	void GateSetAllI();
	void GateSetAllA();
	void GateSetAllB();
	void GateSetAllI2();
	void GateSetAnyI();
	void GateSetAnyA();
	void GateSetAnyB();
	void GateSetAnyI2();
};

#endif // GATESET_H
