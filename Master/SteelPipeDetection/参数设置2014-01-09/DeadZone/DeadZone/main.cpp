#include "deadzone.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DeadZone w;
	w.show();
	return a.exec();
}
