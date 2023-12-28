#ifndef ULTRASOUNDPARAMETER_H
#define ULTRASOUNDPARAMETER_H

#include <QtGui/QDialog>
#include "ui_ultrasoundparameter.h"

class UltrasoundParameter : public QDialog
{
	Q_OBJECT

public:
	UltrasoundParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UltrasoundParameter();

private:
	Ui::UltrasoundParameterClass ui;
};

#endif // ULTRASOUNDPARAMETER_H
