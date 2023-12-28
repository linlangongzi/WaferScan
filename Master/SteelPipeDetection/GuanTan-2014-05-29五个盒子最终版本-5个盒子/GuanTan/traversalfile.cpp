#include "traversalfile.h"
#include <QtCore>
#include <QtGui>
#include <QVariant>
#include <QDebug>

Traversalfile::Traversalfile()
{
}
QList<QString> Traversalfile::findfile(const QString &path)
{

	QList<QString> static pathlist;



	QDir dir(path);
	//    if (!dir.exists())
	//        return false;
	dir.setFilter(QDir::Dirs|QDir::Files);
	dir.setSorting(QDir::DirsFirst);

	QFileInfoList list = dir.entryInfoList();
	int i=0;
	do{
		QFileInfo fileInfo = list.at(i);
		if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
		{
			i++;
			continue;
		}

		bool bisDir=fileInfo.isDir();
		if(bisDir)
		{

			findfile(fileInfo.filePath());
		}
		else{

			if(fileInfo.suffix()=="dt"){
				pathlist<<fileInfo.filePath();
				//qDebug()<<pathlist;

			}

		}
		i++;
	}while(i<list.size());

	return pathlist;

}