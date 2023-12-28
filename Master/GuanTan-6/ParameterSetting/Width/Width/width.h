#ifndef WIDTH_H
#define WIDTH_H

#include <QtGui/QDialog>
#include "ui_width.h"

class Width : public QDialog
{
	Q_OBJECT

public:
	Width(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Width();

private:
	Ui::WidthClass ui;
};

#endif // WIDTH_H
