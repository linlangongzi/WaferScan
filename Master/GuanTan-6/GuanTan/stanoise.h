#ifndef STANOISE_H
#define STANOISE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class StaNoise : public QDialog
{
	Q_OBJECT

public:
	StaNoise(QWidget *parent = 0, Qt::WFlags flags = 0);
	~StaNoise();

public:
	Ui::StaNoiseClass ui;
private:
	void UiInit();
private slots:
	void Check1_10();
	void Check11_20();
	void Check21_30();
	void SelectAll();
	void SelectAny();

	void AllI();
	void AllA();
	void AllB();
	void AllI2();
	void AnyI();
	void AnyA();
	void AnyB();
	void AnyI2();
};

#endif // STANOISE_H
