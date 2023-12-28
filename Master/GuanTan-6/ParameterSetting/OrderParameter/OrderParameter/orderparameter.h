#ifndef ORDERPARAMETER_H
#define ORDERPARAMETER_H

#include <QtGui/QDialog>
#include "ui_orderparameter.h"

class OrderParameter : public QDialog
{
	Q_OBJECT

public:
	OrderParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OrderParameter();

private:
	Ui::OrderParameterClass ui;
};

#endif // ORDERPARAMETER_H
