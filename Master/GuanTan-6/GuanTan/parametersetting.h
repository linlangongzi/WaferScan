#ifndef PARAMETERSETTING_H
#define PARAMETERSETTING_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"
#include "judgeparameter.h"
#include "orderparameter.h"
#include "showparameter.h"
#include "ultrasoundparameter.h"

class ParameterSetting : public QDialog
{
	Q_OBJECT

public:
	ParameterSetting(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ParameterSetting();

public:
	Ui::ParameterSettingClass ui;

public:
	JudgeParameter m_JudgeParameter;
	ShowParameter m_ShowParameter;
	UltrasoundParameter m_UltrasoundParameter;
	OrderParameter m_OrderParameter;

public:
	QString m_qsOrderParameter;
	QString m_qsJudgeParameter;
	QString m_qsShowParameter;
	QString m_qsUltrasoundparameter;
	QString m_qsParameter;

public:
	bool m_bOrder;
	bool m_bJudge;
	bool m_bUltrasound;
	bool m_bShow;
};

#endif // PARAMETERSETTING_H
