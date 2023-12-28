#include "parametersetting.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ParameterSetting w;
	w.show();
	return a.exec();
}
