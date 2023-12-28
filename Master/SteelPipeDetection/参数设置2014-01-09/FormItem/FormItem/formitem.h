#ifndef FORMITEM_H
#define FORMITEM_H

#include <QtGui/QDialog>
#include "ui_formitem.h"

class FormItem : public QDialog
{
	Q_OBJECT

public:
	FormItem(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FormItem();

private:
	Ui::FormItemClass ui;
};

#endif // FORMITEM_H
