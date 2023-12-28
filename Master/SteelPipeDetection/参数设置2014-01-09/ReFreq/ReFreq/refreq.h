#ifndef REFREQ_H
#define REFREQ_H

#include <QtGui/QDialog>
#include "ui_refreq.h"

class ReFreq : public QDialog
{
	Q_OBJECT

public:
	ReFreq(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ReFreq();

private:
	Ui::ReFreqClass ui;
};

#endif // REFREQ_H
