#include "formoffline.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FormOffLine w;
	w.show();
	return a.exec();
}
