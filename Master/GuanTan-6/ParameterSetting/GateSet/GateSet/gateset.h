#ifndef GATESET_H
#define GATESET_H

#include <QtGui/QDialog>
#include "ui_gateset.h"

class GateSet : public QDialog
{
	Q_OBJECT

public:
	GateSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GateSet();

private:
	Ui::GateSetClass ui;
};

#endif // GATESET_H
