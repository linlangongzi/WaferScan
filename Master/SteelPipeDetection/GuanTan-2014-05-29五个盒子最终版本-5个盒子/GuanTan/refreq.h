#ifndef REFREQ_H
#define REFREQ_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class ReFreq : public QDialog
{
	Q_OBJECT

public:
	ReFreq(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ReFreq();

public:
	Ui::ReFreqClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // REFREQ_H
