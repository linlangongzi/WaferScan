#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui/QDialog>
#include "ui_login.h"

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Login();

private:
	Ui::LoginClass ui;
private slots:
	void LoginAction();
	void QuitAction();
private:
	QString m_qsLogin;
};

#endif // LOGIN_H
