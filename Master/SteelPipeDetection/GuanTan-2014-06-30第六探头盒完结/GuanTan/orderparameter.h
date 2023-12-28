#ifndef ORDERPARAMETER_H
#define ORDERPARAMETER_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"



class OrderParameter : public QDialog
{
	Q_OBJECT

public:
	OrderParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OrderParameter();
public:
	Ui::OrderParameterClass ui;
private:
	void UiInit();
private slots:
	void OrderParameterYes();
};

#endif // ORDERPARAMETER_H
