#include "showparameter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ShowParameter w;
	w.show();
	return a.exec();
}
