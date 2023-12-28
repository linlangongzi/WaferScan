#ifndef TRAVERSALFILE_H
#define TRAVERSALFILE_H
#include <QtCore>
#include <QtGui>

class Traversalfile
{
public:
	Traversalfile();
	QList<QString> findfile(const QString &path);
};

#endif // TRAVERSALFILE_H