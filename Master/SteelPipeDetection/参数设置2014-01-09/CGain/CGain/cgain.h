#ifndef CGAIN_H
#define CGAIN_H

#include <QtGui/QDialog>
#include "ui_cgain.h"

class CGain : public QDialog
{
	Q_OBJECT

public:
	CGain(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CGain();

private:
	Ui::CGainClass ui;
};

#endif // CGAIN_H
