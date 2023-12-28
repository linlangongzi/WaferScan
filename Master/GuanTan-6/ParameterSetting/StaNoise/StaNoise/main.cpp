#include "stanoise.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StaNoise w;
	w.show();
	return a.exec();
}
