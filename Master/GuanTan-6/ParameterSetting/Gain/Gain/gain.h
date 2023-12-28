#ifndef GAIN_H
#define GAIN_H

#include <QtGui/QDialog>
#include "ui_gain.h"

class Gain : public QDialog
{
	Q_OBJECT

public:
	Gain(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Gain();

private:
	Ui::GainClass ui;
};

#endif // GAIN_H
