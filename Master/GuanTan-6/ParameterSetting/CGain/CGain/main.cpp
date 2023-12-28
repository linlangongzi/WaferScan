#include "cgain.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CGain w;
	w.show();
	return a.exec();
}
