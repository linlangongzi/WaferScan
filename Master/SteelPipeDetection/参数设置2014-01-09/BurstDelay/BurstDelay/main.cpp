#include "burstdelay.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BurstDelay w;
	w.show();
	return a.exec();
}
