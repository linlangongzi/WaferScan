#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class UserManagement : public QDialog
{
	Q_OBJECT

public:
	UserManagement(QWidget *parent = 0, Qt::WFlags flags = 0);
	~UserManagement();

public:
	Ui::UserManagementClass ui;
	void Lock();
	void Open();
private:
	void UiInit();
	QString m_strUser;
private slots:
	void Verify();//—È÷§
	void Add();
	void Cut();
};

#endif // USERMANAGEMENT_H
