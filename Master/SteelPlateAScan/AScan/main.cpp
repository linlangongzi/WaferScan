#include "ascan.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	AScan w;
	w.show();
	return a.exec();
}
