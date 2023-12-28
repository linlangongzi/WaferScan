#ifndef SHOWPARAMETER_H
#define SHOWPARAMETER_H

#include <QtGui/QDialog>
#include "ui_showparameter.h"

class ShowParameter : public QDialog
{
	Q_OBJECT

public:
	ShowParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ShowParameter();

private:
	Ui::ShowParameterClass ui;
};

#endif // SHOWPARAMETER_H
