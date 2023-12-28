#include "impedanceset.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImpedanceSet w;
	w.show();
	return a.exec();
}
