#ifndef FORMOFFLINE_H
#define FORMOFFLINE_H

#include <QtGui/QDialog>
#include "ui_formoffline.h"

class FormOffLine : public QDialog
{
	Q_OBJECT

public:
	FormOffLine(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormOffLine();

private:
	Ui::FormOffLineClass ui;
};

#endif // FORMOFFLINE_H
