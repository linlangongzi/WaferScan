#include "orderparameter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OrderParameter w;
	w.show();
	return a.exec();
}
