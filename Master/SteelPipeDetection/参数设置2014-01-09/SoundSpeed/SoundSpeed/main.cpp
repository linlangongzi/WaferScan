#include "soundspeed.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SoundSpeed w;
	w.show();
	return a.exec();
}
