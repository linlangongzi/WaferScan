#ifndef PROBESET_H
#define PROBESET_H

#include <QtGui/QDialog>
#include "ui_probeset.h"

class ProbeSet : public QDialog
{
	Q_OBJECT

public:
	ProbeSet(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ProbeSet();

private:
	Ui::ProbeSetClass ui;
};

#endif // PROBESET_H
