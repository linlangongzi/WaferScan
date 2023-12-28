#ifndef LOGIN_H
#define LOGIN_H

#include <QtGui/QDialog>
#include "ui_Parameter.h"

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Login();

private:
	Ui::LoginClass ui;
private:
	void UiInit();
private slots:
	void LoginAction();
	void QuitAction();
private:
	QString m_qsLogin;
	QString m_strPath;
};

#endif // LOGIN_H
