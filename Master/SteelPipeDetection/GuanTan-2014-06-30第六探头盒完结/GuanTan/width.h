#ifndef WIDTH_H
#define WIDTH_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class Width : public QDialog
{
	Q_OBJECT

public:
	Width(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Width();

public:
	Ui::WidthClass ui;
private:
	void UiInit();
private slots:
	void SelectAll();
	void SelectAny();
	void AllIn();
	void AnyIn();
};

#endif // WIDTH_H
