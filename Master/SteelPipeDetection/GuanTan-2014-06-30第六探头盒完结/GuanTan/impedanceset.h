#ifndef IMPEDANCESET_H
#define IMPEDANCESET_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class ImpedanceSet : public QDialog
{
	Q_OBJECT

public:
	ImpedanceSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImpedanceSet();

public:
	Ui::ImpedanceSetClass ui;
};

#endif // IMPEDANCESET_H
