#ifndef SHOWPARAMETER_H
#define SHOWPARAMETER_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class ShowParameter : public QDialog
{
	Q_OBJECT

public:
	ShowParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ShowParameter();

public:
	Ui::ShowParameterClass ui;
private:
	void UiInit();
};

#endif // SHOWPARAMETER_H
