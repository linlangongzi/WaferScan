#ifndef BURSTDELAY_H
#define BURSTDELAY_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class BurstDelay : public QDialog
{
	Q_OBJECT

public:
	BurstDelay(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BurstDelay();

public:
	Ui::BurstDelayClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // BURSTDELAY_H
