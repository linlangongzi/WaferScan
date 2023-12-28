#include "refreq.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ReFreq w;
	w.show();
	return a.exec();
}
