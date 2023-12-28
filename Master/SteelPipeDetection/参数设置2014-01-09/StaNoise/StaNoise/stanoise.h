#ifndef STANOISE_H
#define STANOISE_H

#include <QtGui/QDialog>
#include "ui_stanoise.h"

class StaNoise : public QDialog
{
	Q_OBJECT

public:
	StaNoise(QWidget *parent = 0, Qt::WFlags flags = 0);
	~StaNoise();

private:
	Ui::StaNoiseClass ui;
};

#endif // STANOISE_H
