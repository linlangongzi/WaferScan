#ifndef DEADZONE_H
#define DEADZONE_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class DeadZone : public QDialog
{
	Q_OBJECT

public:
	DeadZone(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DeadZone();

public:
	Ui::DeadZoneClass ui;
};

#endif // DEADZONE_H
