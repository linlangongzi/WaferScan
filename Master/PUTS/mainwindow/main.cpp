/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 中厚板探伤主界面
* Filename: main.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-04-27
*/


#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QMap>
#include <qdebug.h>

#include "LogOnDialog.h"
#include "PlateParaSetERPDialog.h"
#include "PlateParaSetDialog.h"

int main(int argc, char **argv)
{ 
    QApplication app(argc, argv);
//	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	//chenyuzhu add start on 2014-6-25
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
#endif
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
	//chenyuzhu add end on 2014-6-25

    MainWindow mainWin;
    mainWin.resize(1920, 1050);
    mainWin.showMaximized();	
    return app.exec();
}
