#include "gateset.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GateSet w;
	w.show();
	return a.exec();
}
