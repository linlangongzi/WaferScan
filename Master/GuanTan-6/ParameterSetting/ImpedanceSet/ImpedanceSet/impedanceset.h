#ifndef IMPEDANCESET_H
#define IMPEDANCESET_H

#include <QtGui/QDialog>
#include "ui_impedanceset.h"

class ImpedanceSet : public QDialog
{
	Q_OBJECT

public:
	ImpedanceSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ImpedanceSet();

private:
	Ui::ImpedanceSetClass ui;
};

#endif // IMPEDANCESET_H
