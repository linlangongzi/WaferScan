#include "probeset.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ProbeSet w;
	w.show();
	return a.exec();
}
