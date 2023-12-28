#include "judgeparameter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JudgeParameter w;
	w.show();
	return a.exec();
}
