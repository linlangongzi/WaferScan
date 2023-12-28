#ifndef SOUNDSPEED_H
#define SOUNDSPEED_H

#include <QtGui/QDialog>
#include "ui_soundspeed.h"

class SoundSpeed : public QDialog
{
	Q_OBJECT

public:
	SoundSpeed(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SoundSpeed();

private:
	Ui::SoundSpeedClass ui;
};

#endif // SOUNDSPEED_H
