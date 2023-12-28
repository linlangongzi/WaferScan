#ifndef ULTRASOUNDPARAMETER_H
#define ULTRASOUNDPARAMETER_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"
#include "burstdelay.h"
#include "cgain.h"
#include "gain.h"
#include "gateset.h"
#include "impedanceset.h"
#include "judge.h"
#include "refreq.h"
#include "probeset.h"
#include "soundspeed.h"
#include "stanoise.h"
#include "width.h"
#include "deadzone.h"

class UltrasoundParameter : public QDialog
{
	Q_OBJECT

public:
	UltrasoundParameter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UltrasoundParameter();

public:
	Ui::UltrasoundParameterClass ui;
public:
	BurstDelay m_BurstDelay;
	CGain m_CGain;
	Gain m_Gain;
	GateSet m_GateSet;
	ImpedanceSet m_ImpedanceSet;
	Judge m_Judge;
	ProbeSet m_ProbeSet;
	ReFreq m_ReFreq;
	SoundSpeed m_SoundSpeed;
	StaNoise m_StaNoise;
	Width m_Width;
	DeadZone m_DeadZone;
};

#endif // ULTRASOUNDPARAMETER_H
