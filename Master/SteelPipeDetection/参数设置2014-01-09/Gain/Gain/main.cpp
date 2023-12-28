#include "gain.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Gain w;
	w.show();
	return a.exec();
}
