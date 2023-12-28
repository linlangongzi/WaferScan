#ifndef FORMONLINE_H
#define FORMONLINE_H

#include <QtGui/QDialog>
#include "ui_formonline.h"

class FormOnLine : public QDialog
{
	Q_OBJECT

public:
	FormOnLine(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormOnLine();

private:
	Ui::FormOnLineClass ui;
};

#endif // FORMONLINE_H
