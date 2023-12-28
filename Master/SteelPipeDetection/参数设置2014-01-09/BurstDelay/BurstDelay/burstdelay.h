#ifndef BURSTDELAY_H
#define BURSTDELAY_H

#include <QtGui/QDialog>
#include "ui_burstdelay.h"

class BurstDelay : public QDialog
{
	Q_OBJECT

public:
	BurstDelay(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BurstDelay();

private:
	Ui::BurstDelayClass ui;
};

#endif // BURSTDELAY_H
