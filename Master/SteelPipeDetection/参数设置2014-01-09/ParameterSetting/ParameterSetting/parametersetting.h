#ifndef PARAMETERSETTING_H
#define PARAMETERSETTING_H

#include <QtGui/QDialog>
#include "ui_parametersetting.h"

class ParameterSetting : public QDialog
{
	Q_OBJECT

public:
	ParameterSetting(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ParameterSetting();

private:
	Ui::ParameterSettingClass ui;
};

#endif // PARAMETERSETTING_H
