#ifndef SOUNDSPEED_H
#define SOUNDSPEED_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class SoundSpeed : public QDialog
{
	Q_OBJECT

public:
	SoundSpeed(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SoundSpeed();

public:
	Ui::SoundSpeedClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // SOUNDSPEED_H
