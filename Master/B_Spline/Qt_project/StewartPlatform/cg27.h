#ifndef CG27_H
#define CG27_H

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <QThread>
#include <QDebug>
#include <QMutex>
#include "windows.h"
#include <iostream>

class CG27 :
    public QThread
{
    Q_OBJECT
public:
    CG27(void);
    ~CG27(void);
public:
    bool InitializeG27();
    QVector<qreal> ReadData();
protected:
    void run();
signals:
    void GetG27Info();
};

#endif // CG27_H
