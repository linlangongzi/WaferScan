#ifndef GAIN_H
#define GAIN_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class Gain : public QDialog
{
	Q_OBJECT

public:
	Gain(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Gain();

public:
	Ui::GainClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // GAIN_H
