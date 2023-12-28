#include "login.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	Login w;
	w.show();
	return a.exec();
}
