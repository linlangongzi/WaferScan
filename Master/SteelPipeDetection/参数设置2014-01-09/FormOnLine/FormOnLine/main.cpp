#include "formonline.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FormOnLine w;
	w.show();
	return a.exec();
}
