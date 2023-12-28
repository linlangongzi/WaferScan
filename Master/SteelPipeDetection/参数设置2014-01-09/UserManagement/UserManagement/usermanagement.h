#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QtGui/QDialog>
#include "ui_usermanagement.h"

class UserManagement : public QDialog
{
	Q_OBJECT

public:
	UserManagement(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UserManagement();

private:
	Ui::UserManagementClass ui;
};

#endif // USERMANAGEMENT_H
