#ifndef PROBESET_H
#define PROBESET_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class ProbeSet : public QDialog
{
	Q_OBJECT

public:
	ProbeSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ProbeSet();

public:
	Ui::ProbeSetClass ui;
};

#endif // PROBESET_H
