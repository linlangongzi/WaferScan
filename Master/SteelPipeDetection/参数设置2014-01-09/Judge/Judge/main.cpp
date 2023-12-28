#include "judge.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Judge w;
	w.show();
	return a.exec();
}
