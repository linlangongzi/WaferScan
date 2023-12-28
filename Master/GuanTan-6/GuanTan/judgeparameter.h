#ifndef JUDGEPARAMETER_H
#define JUDGEPARAMETER_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class JudgeParameter : public QDialog
{
	Q_OBJECT

public:
	JudgeParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~JudgeParameter();

public:
	Ui::JudgeParameterClass ui;
private:
	void UiInit();
private slots:
	void SoundOpen();
	void SoundClose();
	void LightOpen();
	void LightClose();
public:
	bool m_bSoundOpen;
	bool m_bLightOpen;
};

#endif // JUDGEPARAMETER_H
