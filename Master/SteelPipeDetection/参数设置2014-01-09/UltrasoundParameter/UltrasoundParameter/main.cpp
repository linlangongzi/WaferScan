#include "ultrasoundparameter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UltrasoundParameter w;
	w.show();
	return a.exec();
}
