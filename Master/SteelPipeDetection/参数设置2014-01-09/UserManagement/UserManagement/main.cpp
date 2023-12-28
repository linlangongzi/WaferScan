#include "usermanagement.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UserManagement w;
	w.show();
	return a.exec();
}
