#include "guantan.h"
#include "login.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QValidator>
#include <QRegExp>
#include <QRegExpValidator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	Login dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		GuanTan NDT;
		NDT.show();
		return a.exec();
	}
	else
		return 0;
}
